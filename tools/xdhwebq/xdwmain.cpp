/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

    XDHWebQ is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHWebQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdwmain.h"

#include "misc.h"

#include "xdhutl.h"

using namespace xdwmain;

namespace {
	namespace {
		namespace {
			void HandleEvent_(
				const query::dPairs &Pairs,
				session::rSession &Session,
				str::string_ &Script )
			{
			qRH;
				str::string Digest, Id;
				xdhutl::event_abstract Abstract;
			qRB;
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
			qRR;
			qRT;
			qRE;
			}

			void HandleAction_(
				const str::string_ &Action,
				session::rSession &Session,
				str::string_ &Script )
			{
				Session.UpstreamLaunch( str::string(), Action, Script );
			}

			void Report_(
				const str::string_ &Response,
				session::rSession &Session,
				str::string_ &Script )
			{
				Session.UpstreamReport( Response, Script );
			}
		}

		bso::bool__ Handle_(
			str::string_ &SessionId,	// If empty, contains new id when returning.
			const str::string_ &Action,
			const str::string_ &Response,
			const str::string_ &Language,
			const query::dPairs &Pairs,
			session::rSessions &RawSessions,
			str::string_ &Script )
		{
			bso::bool__ Continue = true;
		qRH;
			session::row__ Row = qNIL;
			lck::exclusive_access___<session::dSessions> Sessions;
			session::rSession *Session = NULL;
			err::buffer__ ERRBuffer;
			str::string Token, AbortMessage;
			xdhujp::sProxyCallback *ProxyCallback = NULL;
		qRB;
			Sessions.Init( RawSessions );

			Token.Init();
			Pairs.GetValue( str::wString( "_token" ), Token );

			if ( SessionId.Amount() == 0 ) {
				SessionId.Init();
				Row = Sessions().New( SessionId, Language, Token, ProxyCallback);
				Session = Sessions().Sessions( Row );
				Sessions.Release();
				Session->Initialize( ProxyCallback, Language, Token );
			} else {
				Row = Sessions().Search( SessionId );
			}

			if ( Row != qNIL ) {
				Session = Sessions().Sessions( Row );

				if ( !Session->IsAlive() ) {
					Sessions().Close( Row );
					Row = qNIL;
					Session = NULL;
				}
			}

			if ( Row != qNIL ) {
				if ( Session == NULL ) {
					sclmisc::MGetValue( xdwrgstry::parameter::script::Fallback, Script );

					Continue = false;

					SessionId.Init();
				} else {
					Session->Lock();

					if ( Action == "_HandleEvent" )
						HandleEvent_( Pairs, *Session, Script );
					else if ( Action == "_Next" )
						Report_( Response, *Session, Script );
					else
						HandleAction_( Action, *Session, Script );
				}
			} else
				misc::Report( "No corresponding web application launched. See http://atlastk.org.", Script );
		qRR;
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
		qRT;
			if ( Session != NULL )
				if ( Session->IsLocked() )
					Session->Unlock();
			Sessions.reset();	// To unlock 'Sessions'.
		qRE;
			return Continue;
		}
	}

	void Handle_(
		const query::dPairs &Pairs,
		const str::dString &Action,
		session::rSessions &Sessions,
		str::dString &Response )
	{
	qRH;
		str::string SessionId, Target, CGI, Result, Language, Script;
		bso::bool__ Continue = false;
	qRB;
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
	qRR;
	qRT;
	qRE;
	}
}

void xdwmain::Handle(
	const query::dPairs &Pairs,
	session::rSessions &Sessions,
	str::dString &Response )
{
qRH;
	str::string Action;
qRB;
	Action.Init();
	Pairs.GetValue(str::string( "_action" ), Action );

	Handle_(  Pairs, Action, Sessions, Response );
qRR;
qRT;
qRE;
}
