/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "muaacc.h"

#include "muaimf.h"
#include "muapo3.h"

#include "crt.h"

using namespace muaacc;

namespace update_ {
	namespace {
		typedef crt::qCRATEdl( muamel::dId ) dIds;
		qW( Ids );

		typedef bch::qBUNCHdl( muamel::sRow ) dMRows;
		qW( MRows );

		bso::sBool Exists(
			muamel::sRow Mail,
			const dMRows &Mails )
		{
			sdr::sRow Row = Mails.First();

			while ( ( Row != qNIL ) && ( Mails( Row ) != Mail ) )
				Row = Mails.Next( Row );

			return Row != qNIL;
		}

		void Add_(
			muaagt::sRow Agent,
			const muapo3::dUIDLs &Ids,
			muamel::dMails &Mails,
			dMRows &Rows )
		{
		qRH
			sdr::sRow Row = qNIL;
			muamel::sRow Mail = qNIL;
		qRB
			Row = Ids.First();

			while ( Row != qNIL ) {
				if ( ( Mail = muamel::Search( Agent, Ids( Row ), Mails ) ) == qNIL  )
					Mail = muamel::Add( Agent, Ids( Row ), Mails );

				Rows.Add( Mail );

				Row = Ids.Next( Row );
			}
		qRR
		qRT
		qRE
		}

		void Remove_(
			muaagt::sRow AgentRow,
			const dMRows &Rows,
			muamel::dMails &Mails )
		{
			muamel::sRow Row = Mails.First(), Next = qNIL;

			while ( Row != qNIL ) {
				Next = Mails.Next( Row );

				if ( ( Mails( Row ).Agent() == AgentRow ) && !Exists( Row, Rows ) )
					Mails.Remove( Row );

				Row = Next;
			}
		}

		void Update_(
			muaagt::sRow Agent,
			const muapo3::dUIDLs &Ids,
			muamel::dMails &Mails )
		{
		qRH
			wMRows Rows;
		qRB
			Rows.Init();

			Add_( Agent, Ids, Mails, Rows );

			Remove_( Agent, Rows, Mails );
		qRR
		qRT
		qRE
		}

		void Update_(
			muaagt::sRow AgentRow,
			const muaagt::dAgent &Agent,
			muamel::dMails &Mails )
		{
		qRH
			csdbnc::rIODriver Driver;
			muapo3::wNumbers Numbers;
			muapo3::wUIDLs UIDLs;
		qRB
			muaagt::InitAndAuthenticate( Agent, Driver );

			tol::Init( Numbers, UIDLs );

			muapo3::GetUIDLs( Driver, Numbers, UIDLs );

			muapo3::Quit( Driver );

			Update_( AgentRow, UIDLs, Mails);
		qRR
		qRT
		qRE
		}
	}

	void Update(
		const muaagt::dAgents &Agents,
		muamel::dMails &Mails )
	{
		muaagt::sRow Row = Agents.First();

		while ( Row != qNIL ) {
			Update_( Row, Agents.Core( Row ), Mails );

			Row = Agents.Next( Row );
		}
	}

}

void muaacc::dAccount::Update( void )
{
	update_::Update( Agents, Mails );
}

namespace get_fields_ {
	namespace {
		using muapo3::sNumber;
		using muapo3::dNumbers;
		using muapo3::dUIDLs;

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
			if ( Number != 0 ) {	// 'Number' can be 0 it the corresponding 'UIDL4 was not found (mail erased).
				muapo3::GetHeader( Number, Driver, Body );

				Fields.Init();
				muaimf::Fill(Body.GetDriver(), Fields );

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

		void Get_(
			const dNumbers &Numbers,
			fdr::rIODriver &Driver,
			str::dStrings &Subjects )
		{
			sdr::sRow Row = Numbers.First();

			while ( Row != qNIL ) {
				Get_( Numbers( Row ), Driver, Subjects );

				Row = Numbers.Next( Row );
			}
		}

		void Get_(
			muaagt::sRow Agent,
			const muamel::dRows &AllRows,
			const muamel::dMails &Mails,
			dUIDLs &UIDLs,
			muamel::dRows &Rows )
		{
			sdr::sRow Row = AllRows.First();
			muamel::sRow MRow = qNIL;

			while ( Row != qNIL ) {
				if ( ( Mails( MRow = AllRows( Row ) ) ).Agent() == Agent ) {
					UIDLs.Add( Mails( MRow ).Id );
					Rows.Append( MRow );
				}

				Row = AllRows.Next( Row );
			}
		}

		void Get_(
			const dUIDLs &UIDLs,
			const dUIDLs &AllUIDLs,
			const dNumbers AllNumbers,
			dNumbers Numbers )
		{
			sdr::sRow Row = UIDLs.First();
			sdr::sRow UIDLRow = qNIL;

			while ( Row != qNIL ) {
				UIDLRow = ctn::Search( UIDLs( Row ), AllUIDLs );

				if ( UIDLRow != qNIL )
					Numbers.Add( AllNumbers( Row ) );
				else
					Numbers.Add( (sNumber)0 );

				Row = UIDLs.Next( Row );
			}
		}

		void Get_(
			const dUIDLs &UIDLs,
			fdr::rIODriver &Driver,
			dNumbers &Numbers )
		{
		qRH
			muapo3::wNumbers AllNumbers;
			muapo3::wUIDLs AllUIDLs;
		qRB
			tol::Init( AllNumbers, AllUIDLs );

			muapo3::GetUIDLs( Driver, AllNumbers, AllUIDLs );

			Get_( UIDLs, AllUIDLs, AllNumbers, Numbers );
		qRR
		qRT
		qRE
		}

		void Get_(
			const dNumbers &Numbers,
			const muaagt::dAgent &Agent,
			str::dStrings &Subjects )
		{
		qRH
			csdbnc::rIODriver Driver;
		qRB
			muaagt::InitAndAuthenticate( Agent, Driver );

			Get_( Numbers, Driver, Subjects );

			muapo3::Quit( Driver );
		qRR
		qRT
		qRE
		}

		void Get_(
			const muamel::dRows &Mails,
			const muaagt::dAgent &Agent,
			str::dStrings &Subjects )
		{}
	}
}

void muaacc::dAccount::GetFields(
	const muamel::dRows &Mails,
	str::dStrings &Subjects ) const
{

}

