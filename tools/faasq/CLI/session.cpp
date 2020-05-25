/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'FaaSq' tool.

	'FaaSq' is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'FaaSq' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with 'FaaSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "session.h"

#include "csdcmn.h"

using namespace session;

// #define LOG cio::COut << __LOC__ << tol::DateAndTime(DT) << txf::nl << txf::commit;

// Special script name, which are NOT converted to a script,
// but intercepted by the ATK proxy ('xdhqxdh'),
// to launch a special action.
namespace ssn_ { // Special Script Name
	qCDEF(char *, StandBy, "#StandBy_1");
	qCDEF(char *, Broadcast, "#Broadcast_1");
	qCDEF(char *, Quit, "#Quit_1");
}

// Empty (special) action ids.
// An empty action with this label is intercepted,
// to do special actions.
namespace eai_ { // Special Action Id,
	qCDEF(char *, Quit, "$Quit_1");
}

void session::sUpstream_::XDHCUCProcess(
	const str::string_ &Script,
	str::dString *ReturnedValue )
{
qRH
	flw::rDressedRWFlow<> Proxy;
qRB
	Proxy.Init(P_());

	csdcmn::Put(Id_, Proxy);
	csdcmn::Put("Execute_1", Proxy);

//    cio::COut << ">>>>>>>" << txf::tab << "'" << Script <<"'" << txf::tab << (int)Id_ << txf::nl << txf::commit;

	csdcmn::Put((bso::sU8)(ReturnedValue == NULL ? 0 : 1), Proxy);
	csdcmn::Put((bso::sU8)1,Proxy);
	csdcmn::Put(Script, Proxy);
	csdcmn::Put((bso::sU8)0, Proxy);


	Proxy.Commit();

	if ( ReturnedValue != NULL) {
		B_().WaitSelf();

		csdcmn::Get(Proxy, *ReturnedValue);
		Proxy.Dismiss();

		B_().UnblockGlobal();
	}
qRR
qRT
qRE
}

void session::rSession::Launch(void)
{
qRH
	str::wString Id, Action;
	qCBUFFERh IdBuffer, ActionBuffer;
	bso::sBool Exit = false;
qRB
	while ( !Exit ) {
		Blockers_.WaitSelf();

		tol::Init(Id, Action);

		csdcmn::Get(Proxy_, Id);
		csdcmn::Get(Proxy_, Action);

		Proxy_.Dismiss();
		Blockers_.UnblockGlobal();

		if ( Action.Amount() == 0 ) {
			if ( Id == eai_::Quit )
				Exit = true;
			else if ( Id.Amount() != 0 )	// On connection.
				qRGnr();
		}

		if ( !Exit )
			Session_.Launch(Id.Convert(IdBuffer), Action.Convert(ActionBuffer));

		// 'Id_' is the session id and must not be confused with the local variable 'Id',
		// which is the id of the DOM element on which there was 'Action' was applied.
		csdcmn::Put(Id_, Proxy_);

		if ( Exit )
			csdcmn::Put(ssn_::Quit, Proxy_);
		else
			csdcmn::Put(ssn_::StandBy, Proxy_);

		Proxy_.Commit();
	}
qRR
qRT
qRE
}

sRow session::Search(
	faas_::sId Id,
	const dIds &Ids)
{
	sRow Row = Ids.First();

	while ( ( Row != qNIL ) && ( Ids(Row) != Id) )
		Row = Ids.Next(Row);

	return Row;
}


