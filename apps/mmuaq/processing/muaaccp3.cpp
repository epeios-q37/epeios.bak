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

#include "muaaccp3.h"

#include "muaimf.h"

using namespace muaaccp3;

namespace {
	typedef crt::qCRATEdl( muamel::dId ) dIds;
	qW( Ids );

	typedef bch::qBUNCHdl( muamel::sRow ) dMRows;
	qW( MRows );

	void Add_(
		muaagt::sRow Agent,
		const muapo3::dUIDLs &Ids,
		const muamel::dRows &Mails,
		muatrk::dTracker &Tracker,
		muadir::dDirectory &Directory,
		dMRows &Rows )
	{
	qRH
		sdr::sRow Row = qNIL;
		muamel::sRow Mail = qNIL;
	qRB
		Row = Ids.First();

		while ( Row != qNIL ) {
			if ( ( Mail = Directory.Search( Ids( Row ), Mails ) ) == qNIL ) {
				Mail = Directory.AddMail(Ids( Row ) );
				Tracker.Link( Mail, Agent );
			}

			Rows.Add( Mail );

			Row = Ids.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	void Remove_(
		muaagt::sRow AgentRow,
		const dMRows &New,
		const muamel::dRows &Old,
		muatrk::dTracker &Tracker,
		muadir::dDirectory &Directory )
	{
		sdr::sRow Row = Old.First(), Next = qNIL;
		muamel::sRow Mail = qNIL;

		while ( Row != qNIL ) {
			Next = Old.Next( Row );

			if ( New.Search( Mail = Old( Row ) ) == qNIL ) {
				Directory.Remove( Mail );
				Tracker.Remove( Mail );
			}

			Row = Next;
		}
	}

	void Update_(
		muaagt::sRow Agent,
		const muapo3::dUIDLs &Ids,
		muatrk::dTracker &Tracker,
		muadir::dDirectory &Directory )
	{
	qRH
		wMRows New, Old;
	qRB
		Old.Init();
		Tracker.GetMails( Agent, Old );

		New.Init();
		Add_( Agent, Ids, Old, Tracker, Directory, New );

		Remove_( Agent, New, Old, Tracker, Directory );
	qRR
	qRT
	qRE
	}
}

void muaaccp3::Update(
	fdr::rIODriver &Driver,
	muatrk::dTracker &Tracker,
	muadir::dDirectory &Directory )
{
qRH
	muapo3::wNumbers Numbers;
	muapo3::wUIDLs UIDLs;
qRB
	tol::Init( Numbers, UIDLs );

	muapo3::GetUIDLs( Driver, Numbers, UIDLs );

	muapo3::Quit( Driver );
qRR
qRT
qRE
}

namespace {
	using muapo3::sNumber;
	using muapo3::dNumbers;
	using muapo3::dUIDLs;

	// _U_idls to _n_umbers
	namespace u2n_ {
		namespace {
			void Get_(
				const muamel::dRows &Wanted,
				const dUIDLs &UIDLs,
				const dUIDLs &AllUIDLs,
				const dNumbers &AllNumbers,
				dNumbers &Numbers,
				muamel::dRows &Available )
			{
				if ( UIDLs.Amount() != Wanted.Amount() )
					qRGnr();

				sdr::sRow Row = UIDLs.First();
				sdr::sRow UIDLRow = qNIL;

				while ( Row != qNIL ) {
					UIDLRow = ctn::Search( UIDLs( Row ), AllUIDLs );

					if ( UIDLRow != qNIL ) {
						Numbers.Add( AllNumbers( UIDLRow ) );
						Available.Append( Wanted( Row ) );
					}

					Row = UIDLs.Next( Row );
				}
			}
		}

		void Get(
			const muamel::dRows &Wanted,
			const muamel::dIds &Ids,
			fdr::rIODriver &Driver,
			dNumbers &Numbers,
			muamel::dRows &Available )				{
		qRH
			muapo3::wNumbers AllNumbers;
			muapo3::wUIDLs AllUIDLs;
		qRB
			tol::Init( AllNumbers, AllUIDLs );

			muapo3::GetUIDLs( Driver, AllNumbers, AllUIDLs );

			Get_( Wanted, Ids, AllUIDLs, AllNumbers, Numbers, Available );
		qRR
		qRT
		qRE
		}
	}

	// 'POP3' message _n_umbers to _s_ubjects.
	namespace n2s_ {
		namespace {
			void Get_(
				sNumber Number,
				fdr::rIODriver &Driver,
				str::dStrings &Subjects )
			{
			qRH
				muapo3::hBody Body;
				muaimf::wFields Fields;
				str::wString Subject;
				muaimf::sFRow Row = qNIL;
			qRB
				if ( Number != 0 ) {	// 'Number' can be 0 it the corresponding 'UIDL' was not found (mail erased).
					muapo3::GetHeader( Number, Driver, Body );

					Fields.Init();
					muaimf::Fill( Body.GetDriver(), Fields );

					Subject.Init();
					Row = Fields.Search( muaimf::fSubject );

					if ( Row == qNIL )
						qRGnr();

					Subject.Init();
					Fields.GetBody( Row, Subject );
					Subjects.Add( Subject );
				}
			qRR
			qRT
			qRE
			}
		}

		void Get(
			const dNumbers &Numbers,
			muaagt::sRow OwningAgent,
			fdr::rIODriver &Driver,
			str::dStrings &Subjects,
			muaagt::dRows &Agents )
		{
			sdr::sRow Row = Numbers.First();

			while ( Row != qNIL ) {
				Get_( Numbers( Row ), Driver, Subjects );

				Agents.Add( OwningAgent );

				Row = Numbers.Next( Row );
			}
		}
	}
}


void muaaccp3::GetFields(
	const muamel::dRows &Wanted,
	const muamel::dIds &Ids,
	muaagt::sRow Agent,
	fdr::rIODriver &Driver,
	str::dStrings &Subjects,
	muaagt::dRows &CorrespondingAgents,
	muamel::dRows &Available )
{
qRH
	muapo3::wNumbers Numbers;
qRB
	tol::Init( Numbers );
	::u2n_::Get( Wanted, Ids, Driver, Numbers, Available );

	n2s_::Get( Numbers, Agent, Driver, Subjects, CorrespondingAgents );

	muapo3::Quit( Driver );
qRR
qRT
qRE
}

const str::dString &muaaccp3::GetMail(
	const muamel::dId &Id,
	fdr::rIODriver &Driver,
	str::dString &Mail )
{
qRH
	muapo3::sNumber Number = 0;
qRB
	Number = muapo3::GetNumberForUIDL( Id, Driver );

	if ( Number == 0 )
		qRGnr();

	muapo3::GetMessage( Number, Driver, Mail );

	muapo3::Quit( Driver );
qRR
qRT
qRE
	return Mail;
}






