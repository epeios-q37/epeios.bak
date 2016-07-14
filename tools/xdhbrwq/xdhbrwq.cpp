/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'xdhbrwq'.

    'xdhbrwq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhbrwq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhbrwq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "query.h"
#include "misc.h"
#include "registry.h"
#include "session.h"

#include "scltool.h"
#include "sclerror.h"

#include "xdhutl.h"

#include "csdbns.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"xdhbrwq"
# define NAME_LC			"xdhbrwq"
# define NAME_UC			"XDHBRWQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace process_ {

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
						sclmisc::MGetValue( registry::parameter::script::Fallback, Script );

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

			typedef csdbns::cProcessing cProcessing_;
		}

		class sProcessingCallback
		: public cProcessing_
		{
		private:
			Q37_MRMDF( session::sessions___, S_, Sessions_ );
		protected:
			void *CSDSCBPreProcess( const ntvstr::char__ *Origin ) override
			{
				return NULL;
			}
			csdscb::action__ CSDSCBProcess(
				flw::ioflow__ &Flow,
				void *UP ) override
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
				return csdscb::aStop;
			}
			virtual void CSDSCBPostProcess( void *UP ) override
			{}
		public:
			void reset( bso::bool__ P = true )
			{
				Sessions_ = NULL;
			}
			E_CVDTOR( sProcessingCallback );
			void Init( session::sessions___ &Sessions )
			{
				Sessions_ = &Sessions;
			}
		};

	}



	void Process_( void )
	{
	qRH
		agent::agent___ Agent;
		session::sessions_unprotected SessionsUnprotected;
		session::sessions___ Sessions;
		str::wString Identification, ModuleFilename;
		TOL_CBUFFER___ IdentificationBuffer;
		csdbns::server___ Server;
		process_::sProcessingCallback Callback;
	qRB
		Identification.Init( MISC_NAME_LC " V" MISC_VERSION " Build " __DATE__ " " __TIME__ " - " );	
		Identification.Append( cpe::GetDescription() );

		ModuleFilename.Init();
		sclmisc::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

		Agent.Init( xdhcmn::mMultiUser, ModuleFilename, Identification.Convert( IdentificationBuffer ) );

		SessionsUnprotected.Init( 0, 0, Agent );

		Sessions.Init( SessionsUnprotected );

		Callback.Init( Sessions );

		Server.Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );
		Server.Process( NULL );
	qRR
	qRT
	qRE
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;

