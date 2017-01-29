/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "muaaccim.h"

#include "muaacc.h"
#include "muaimf.h"

using namespace muaaccim;

namespace update_ {
	bso::sUInt GetAmount( muaima::rSession &Session )
	{
		bso::sUInt Amount = 0;

		Session.GetMailAmount(str::wString("inbox"), Amount );

		return Amount;
	}
}

void muaaccim::Update(
	muaagt::sRow Agent,
	muaima::rSession &Session,
	muatrk::dTracker &Tracker,
	muadir::dDirectory &Directory )
{
qRH
	bso::sUInt Amount = 0;
	muaima::rUID UID;
	muamel::wId Id;
	muamel::wIds Ids;
qRB
	Amount = update_::GetAmount( Session );	// NOTA : makes a 'Select', so no need to provide a folder below.

	tol::Init( Ids );

	while ( Amount ) {
		tol::Init( UID );
		Session.GetUID( muaima::CurrentFolder, Amount--, UID );

		tol::Init( Id );
		flx::GetString(UID.GetDriver(), Id );

		if ( UID.EndStatus() != muaima::sOK )
			qRGnr();

		Ids.Append( Id );
	}

	muaacc::Update( Agent, Ids, Tracker, Directory );
qRR
qRT
qRE
}

namespace {
	// Only subject yet.
	bso::sBool GetSubject_(
		const str::dString &RawId,
		muaima::rSession &Session,
		str::dString &Subject )
	{
		bso::sBool Exists = false;	// Will be handled correctly later.
	qRH
		muaima::rRFC822 RFC822;
		bso::sUInt Id = 0;
		muaimf::wFields Fields;
	qRB
		RawId.ToNumber( Id );

		RFC822.Init();
		Session.GetRFC822( muaima::rpHeader, muaima::fUID, muaima::CurrentFolder, Id, RFC822 );

		muaimf::GetField(muaimf::fSubject, RFC822.GetDriver(), Subject );

		if ( RFC822.EndStatus() != muaima::sOK )
			qRGnr();

		Exists = true;
	qRR
	qRE
	qRT
		return Exists;
	}
}

void muaaccim::GetFields(
	const muamel::dRows &Wanted,
	const muamel::dIds &Ids,
	muaagt::sRow Agent,
	muaima::rSession &Session,
	str::dStrings &Subjects,
	muaagt::dRows &CorrespondingAgents,
	muamel::dRows &Availables )
{
qRH
	sdr::sRow Row = qNIL;
	str::wString Subject;
qRB
	sdr::sRow Row = Wanted.First();

	Session.Select(str::wString( "Inbox") );	// We only handle 'Inbox' yet ; other mailboxes will be handled later.

	while ( Row != qNIL ) {
		tol::Init( Subject );

		if ( GetSubject_( Ids( Row ), Session, Subject ) ) {
			Subjects.Append( Subject );
			CorrespondingAgents.Append( Agent );
			Availables.Append( Wanted( Row ) );
		}

		Row = Wanted.Next( Row );
	}
qRR
qRT
qRE
}

const str::dString &muaaccim::GetMail(
	const muamel::dId &RawId,
	muaima::rSession &Session,
	str::dString &Mail )
{
qRH
	muaima::rRFC822 RFC822;
	bso::sUInt Id = 0;
qRB
	RawId.ToNumber( Id );

	RFC822.Init();
	Session.GetRFC822( muaima::rpAll, muaima::fUID, str::wString("inbox"), Id, RFC822 );

	flx::GetString( RFC822.GetDriver(), Mail );

	if ( RFC822.EndStatus() != muaima::sOK )
		qRGnr();
qRR
qRE
qRT
	return Mail;
}



