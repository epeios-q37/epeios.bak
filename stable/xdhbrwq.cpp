/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#include "misc.h"
#include "registry.h"
#include "query.h"
#include "session.h"
#include "agent.h"

#include "scldaemon.h"
#include "sclmisc.h"

#include "ssnmng.h"

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
	void HandleAction_(
		const str::string_ &Action,
		session::session___ &Session,
		str::string_ &Script )
	{
	qRH
		TOL_CBUFFER___ Buffer;
	qRB
		Session.UpstreamLaunch( "", Action.Convert( Buffer ), Script );
	qRR
	qRT
	qRE
	}

	void Report_(
		const str::string_ &Response,
		session::session___ &Session,
		str::string_ &Script )
	{
		Session.UpstreamReport( Response, Script );
	}

	void Handle_(
		str::string_ &Id,	// If empty, contains new id when returning.
		const str::string_ &Action,
		const str::string_ &Response,
		session::sessions___ &RawSessions,
		str::string_ &Script )
	{
	qRH
		session::row__ Row = qNIL;
		lck::exclusive_access___<session::sessions_unprotected_> Sessions;
		session::session___ *Session = NULL;
	qRB
		Sessions.Init( RawSessions );

		if ( Id.Amount() == 0 ) {
			Id.Init();
			Row = Sessions().New( Id );
		} else {
			Row = Sessions().Search( Id );

			if ( Row == qNIL )
				qRVct();
		}

		Session = Sessions().Sessions( Row );

		Sessions.reset();	// To unlock sessions.

		if ( Action.Amount() )
			HandleAction_( Action, *Session, Script );
		else if ( Session->IsBusy() )
			Report_( Response, *Session, Script );
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
		str::string Id, Target, CGI, Action, Result, Script;
	qRB
		Id.Init();
		Pairs.GetValue(str::string( "_session" ), Id );

		Target.Init();
		Pairs.GetValue(str::string( "_target" ), Target );

		CGI.Init();
		Pairs.GetValue(str::string( "_CGI" ), CGI );

		Action.Init();
		Pairs.GetValue(str::string( "_action" ), Action );

		Response.Init();
		Pairs.GetValue(str::string( "_result" ), Result );

		Script.Init();
		Handle_( Id, Action, Response, Sessions, Script );

		if ( Script.Amount() ) {
			Response.Append( "var query=\"" );

			Response.Append( CGI );
			Response.Append('?');

			Response.Append("_session=");
			Response.Append( Id );

			Response.Append("&_target=");
			Response.Append( Target );

			Response.Append("\";");

			Response.Append( Script );
		}
	qRR
	qRT
	qRE
	}

	typedef scldaemon::daemon___ _daemon__;

	class daemon__
	: public _daemon__
	{
	private:
		Q37_MRMDF( session::sessions___, _S, _Sessions );
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow ) override
		{
		qRH
			query::pairs Pairs;
			str::string Response;
		qRB
			Pairs.Init();
			Pairs.FillWith( Flow );

			Response.Init();
			Handle_( Pairs, _S(), Response );
			Flow << Response;
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
			_Sessions = NULL;
		}
		E_CVDTOR( daemon__ );
		void Init( session::sessions___ &Sessions )
		{
			_daemon__::Init();
			_Sessions = &Sessions;
		}
	};

	typedef scldaemon::callback__ _callback__;

	class xdhbrwq_callback__
	: public _callback__
	{
	private:
		Q37_MRMDF( session::sessions___, _S, _Sessions );
	protected:
		virtual scldaemon::daemon___ *SCLDAEMONNew( const char *Origin ) override
		{
			daemon__ *Daemon = new daemon__;

			if ( Daemon==NULL )
				qRAlc();

			Daemon->Init( _S() );

			return Daemon;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset(P);
			_Sessions = NULL;
		}
		E_CVDTOR( xdhbrwq_callback__ );
		void Init( session::sessions___ &Sessions )
		{
			_callback__::Init();
			_Sessions = &Sessions;
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

scldaemon::callback__ *scldaemon::SCLDAEMONGetCallback(
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

	Agent_.Init( xdhcbk::mMultiUser, ModuleFilename, Identification.Convert( Identification_ ) );

	SessionsUnprotected_.Init( 0, 0, Agent_ );

	Sessions_.Init( SessionsUnprotected_ );

	Callback->Init( Sessions_ );
qRR
qRT
qRE
	return Callback;
}


const char *sclmisc::SCLMISCTargetName = MISC_NAME_LC;
