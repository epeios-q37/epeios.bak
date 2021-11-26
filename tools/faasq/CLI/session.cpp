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
#include "xdhutl.h"

using namespace session;

// #define LOG cio::COut << __LOC__ << tol::DateAndTime(DT) << txf::nl << txf::commit;

bso::sBool session::sUpstream_::XDHCUCProcess(
	const str::string_ &Script,
	tht::rBlocker *Blocker,
	bso::sBool *SuccessPointer,
	str::dString *ReturnedValue )
{
	bso::sBool Success = true;
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
		if ( Blocker != NULL ) {
      if ( SuccessPointer == NULL )
        qRGnr();

      *SuccessPointer = Success;
			Blocker->Unblock();
		}

		B_().WaitSelf();

		csdcmn::Get(Proxy, *ReturnedValue);
		Proxy.Dismiss();

		B_().UnblockGlobal();
	} else if ( Blocker != NULL )
		qRGnr();
  else if ( SuccessPointer != NULL )
    qRGnr();
qRR
	Success = false;
	ERRRst();
qRT
  if ( Blocker != NULL ) {
    if ( SuccessPointer == NULL )
      qRGnr();

    *SuccessPointer = Success;
    Blocker->Unblock();
  }
qRE
	return Success;
}

void session::rSession::Launch(void)
{
qRH
	str::wString Id, Action, Digest;
	qCBUFFERh Buffer;
qRB
	while ( true ) {
		Blockers_.WaitSelf();

		if ( Quit ) {
			Blockers_.UnblockGlobal();
			break;
		}

		tol::Init(Id, Action);

		csdcmn::Get(Proxy_, Id);
		csdcmn::Get(Proxy_, Action);

		Proxy_.Dismiss();
		Blockers_.UnblockGlobal();

		Digest.Init();
		xdhutl::BuildPseudoDigest(Id, Action, Digest);

		Session_.Handle(Digest.Convert(Buffer));

	// 'Id_' is the session id and must not be confused with the local variable 'Id',
	// which is the id of the DOM element on which there was 'Action' was applied.
		csdcmn::Put(Id_, Proxy_);

		csdcmn::Put(xdhcmn::ScriptNameForStandBy, Proxy_);

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


