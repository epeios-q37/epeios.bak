/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of xdhbrwq.

	xdhbrwq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xdhbrwq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xdhbrwq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "misc.h"
#include "registry.h"
#include "query.h"
#include "session.h"
#include "agent.h"

#include "scldaemon.h"
#include "sclmisc.h"

#include "xdhutl.h"

static void Read_( flw::ioflow__ &Flow )
{
qRH
	query::pairs Pairs;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
qRB
	Pairs.Init();
	Pairs.FillWith( Flow );

	TFlow.Init( Flow );

	Writer.Init( TFlow, xml::oIndent, xml::e_Default ) ;

	Pairs.Dump( Writer );
qRR
qRT
qRE
}

namespace {
	void HandleEvent_(
		const query::pairs_ &Pairs,
		session::session___ &Session,
		str::string_ &Script )
	{
	qRH
		str::string Digest, Id;
		xdhutl::event_abstract Abstract;
	qRB
		Digest.Init();
		Pairs.GetValue( str::string( "digest" ), Digest );

		Id.Init();
		Abstract.Init();
		if ( xdhutl::FetchEventAbstract(Digest, Id, Abstract) ) {
			if ( xdhutl::IsPredefined( Abstract.Action() ) )
				qRVct();
			else if ( Abstract.Action() == xdhutl::a_User )
				Session.UpstreamLaunch( Id, Abstract.UserAction, Script );
			else
				qRGnr();
		}
	qRR
	qRT
	qRE
	}

	void HandleAction_(
		const str::string_ &Action,
		session::session___ &Session,
		str::string_ &Script )
	{
		Session.UpstreamLaunch( str::string(), Action, Script );
	}

	void Report_(
		const str::string_ &Response,
		session::session___ &Session,
		str::string_ &Script )
	{
		Session.UpstreamReport( Response, Script );
	}

	bso::bool__ Handle_(
		str::string_ &SessionId,	// If empty, contains new id when returning.
		const str::string_ &Action,
		const str::string_ &Response,
		const str::string_ &Language,
		const query::pairs_ &Pairs,
		session::sessions___ &RawSessions,
		str::string_ &Script )
	{
		bso::bool__ Continue = true;
	qRH
		session::row__ Row = qNIL;
		lck::exclusive_access___<session::sessions_unprotected_> Sessions;
		session::session___ *Session = NULL;
		err::buffer__ ERRBuffer;
		str::string AbortMessage;
	qRB
		Sessions.Init( RawSessions );

		if ( SessionId.Amount() == 0 ) {
			SessionId.Init();
			Row = Sessions().New( SessionId, Language );
		} else {
			Row = Sessions().Search( SessionId );

			if ( Row == qNIL ) {
				sclmisc::MGetValue( registry::script::Fallback, Script );

				Continue = false;

				SessionId.Init();

				qRReturn;
			}
		}

		Session = Sessions().Sessions( Row );

		Session->Lock();	// Unlocked by 

		Sessions.reset();	// To unlock 'Sessions'.

		if ( Action == "_HandleEvent" )
			HandleEvent_( Pairs, *Session, Script );
		else if ( Action == "_Next" )
			Report_( Response, *Session, Script );
		else if ( Action.Amount() )
			HandleAction_( Action, *Session, Script );
	qRR
		if ( ERRFailure() ) {
			Script.Init();
			misc::Report( err::Message( ERRBuffer ), Script );
			Continue = false;
			ERRRst();
		} else if ( ERRType == err::t_Abort ) {
				AbortMessage.Init();
				sclmisc::GetSCLBasePendingErrorTranslation( AbortMessage );
				misc::Report( AbortMessage, Script );
				Continue = false;
				ERRRst();
		}
	qRT
		if ( Session != NULL )
			if ( Session->IsLocked() )
				Session->Unlock();
	qRE
		return Continue;
	}

	void Handle_(
		const query::pairs_ &Pairs,
		const str::string_ &Action,
		session::sessions___ &Sessions,
		str::string_ &Response )
	{
	qRH
		str::string SessionId, Target, CGI, Result, Language, Script;
		bso::bool__ Continue = false;
	qRB
		SessionId.Init();
		Pairs.GetValue(str::string( "_session" ), SessionId );

		Target.Init();
		Pairs.GetValue(str::string( "_target" ), Target );

		CGI.Init();
		Pairs.GetValue(str::string( "_CGI" ), CGI );

		Result.Init();
		Pairs.GetValue(str::string( "_result" ), Result );

		Language.Init();
		Pairs.GetValue(str::string( "_language" ), Language );

		Script.Init();
		Continue = Handle_( SessionId, Action, Result, Language, Pairs, Sessions, Script );

		if ( Script.Amount()  ) {
			if ( CGI.Amount() ) {
				Response.Append( "cgi='" );
				Response.Append( CGI );
				Response.Append("';");
			}

			if ( SessionId.Amount() ) {
				Response.Append( "session='" );
				Response.Append( SessionId );
				Response.Append("';");
			}

			if ( Target.Amount() ) {
				Response.Append( "target='" );
				Response.Append( Target );
				Response.Append("';");
			}

			if ( Continue )
				Response.Append("query='_action=_Next';");

			Response.Append( Script );
		}
	qRR
	qRT
	qRE
	}

	void Handle_(
		const query::pairs_ &Pairs,
		session::sessions___ &Sessions,
		str::string_ &Response )
	{
	qRH
		str::string Action;
	qRB
		Action.Init();
		Pairs.GetValue(str::string( "_action" ), Action );

		Handle_(  Pairs, Action, Sessions, Response );
	qRR
	qRT
	qRE
	}

	typedef scldaemon::daemon___ _daemon__;

	class daemon__
	: public _daemon__
	{
	private:
		Q37_MRMDF( session::sessions___, S_, Sessions_ );
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow ) override
		{
		qRH
			query::pairs Pairs;
			str::string Response;
			TOL_CBUFFER___ Buffer;
		qRB
			Pairs.Init();
			Pairs.FillWith( Flow );

			Response.Init();
			Handle_( Pairs, S_(), Response );
			Flow.Write(Response.Convert(Buffer), Response.Amount() );
			Flow.Commit();
		qRR
		qRT
		qRE
			return false;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_daemon__::reset(P);
			Sessions_ = NULL;
		}
		E_CVDTOR( daemon__ );
		void Init( session::sessions___ &Sessions )
		{
			_daemon__::Init();
			Sessions_ = &Sessions;
		}
	};

	typedef scldaemon::callback___ _callback___;

	class xdhbrwq_callback__
	: public _callback___
	{
	private:
		Q37_MRMDF( session::sessions___, S_, Sessions_ );
	protected:
		virtual scldaemon::daemon___ *SCLDAEMONNew( const ntvstr::char__ *Origin ) override
		{
			daemon__ *Daemon = new daemon__;

			if ( Daemon==NULL )
				qRAlc();

			Daemon->Init( S_() );

			return Daemon;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_callback___::reset(P);
			Sessions_ = NULL;
		}
		E_CVDTOR( xdhbrwq_callback__ );
		void Init( session::sessions___ &Sessions )
		{
			_callback___::Init( scldaemon::mBasic );
			Sessions_ = &Sessions;
		}
	};
}

namespace {
	agent::agent___ Agent_;
	session::sessions_unprotected SessionsUnprotected_;
	session::sessions___ Sessions_;
	TOL_CBUFFER___ Identification_;
}

Q37_GCTOR( xdhbrwq )
{

}

scldaemon::callback___ *scldaemon::SCLDAEMONGetCallback(
	csdleo::context__ Context,
	csdleo::mode__ Mode )
{
	xdhbrwq_callback__ *Callback = NULL;
qRH
	str::string Identification, ModuleFilename;
qRB
	Callback = new xdhbrwq_callback__;

	if ( Callback == NULL )
		qRAlc();

	Identification.Init( MISC_NAME_LC " V" MISC_VERSION " Build " __DATE__ " " __TIME__ " - " );	
	Identification.Append( cpe::GetDescription() );

	ModuleFilename.Init();
	sclmisc::MGetValue( registry::ModuleFilename, ModuleFilename );

	Agent_.Init( xdhcmn::mMultiUser, ModuleFilename, Identification.Convert( Identification_ ) );

	SessionsUnprotected_.Init( 0, 0, Agent_ );

	Sessions_.Init( SessionsUnprotected_ );

	Callback->Init( Sessions_ );
qRR
qRT
qRE
	return Callback;
}


const char *sclmisc::SCLMISCTargetName = MISC_NAME_LC;
