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

#include "muaacc.h"
#include "muaimf.h"

using namespace muaaccp3;

void muaaccp3::Update(
	muaagt::sRow Agent,
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

	muaacc::Update( Agent, UIDLs, Tracker, Directory );

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
				muamel::dRows &Availables )
			{
				if ( UIDLs.Amount() != Wanted.Amount() )
					qRGnr();

				sdr::sRow Row = UIDLs.First();
				sdr::sRow UIDLRow = qNIL;

				while ( Row != qNIL ) {
					UIDLRow = ctn::Search( UIDLs( Row ), AllUIDLs );

					if ( UIDLRow != qNIL ) {
						Numbers.Add( AllNumbers( UIDLRow ) );
						Availables.Append( Wanted( Row ) );
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
			muamel::dRows &Availables )				{
		qRH
			muapo3::wNumbers AllNumbers;
			muapo3::wUIDLs AllUIDLs;
		qRB
			tol::Init( AllNumbers, AllUIDLs );

			muapo3::GetUIDLs( Driver, AllNumbers, AllUIDLs );

			Get_( Wanted, Ids, AllUIDLs, AllNumbers, Numbers, Availables );
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
				str::wString Subject;
			qRB
				muapo3::GetHeader( Number, Driver, Body );

				tol::Init( Subject );
				muaimf::GetField( muaimf::fSubject, Body.GetDriver(), Subject );

				Subjects.Add( Subject );
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
	muamel::dRows &Availables )
{
qRH
	muapo3::wNumbers Numbers;
qRB
	tol::Init( Numbers );
	::u2n_::Get( Wanted, Ids, Driver, Numbers, Availables );

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






