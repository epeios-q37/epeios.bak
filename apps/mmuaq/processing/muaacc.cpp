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

		// _m_ails to _U_IDLs
		namespace m2u_ {
			void Get(
				muaagt::sRow Agent,
				const muamel::dMails &Mails,
				const muamel::dRows &Wanted,
				dUIDLs &UIDLs,
				muamel::dRows &Owned )	// Mails owned by agent.
			{
				sdr::sRow Row = Wanted.First();
				muamel::sRow MRow = qNIL;

				while ( Row != qNIL ) {
					if ( Mails( MRow = Wanted( Row ) ).Agent() == Agent ) {
						UIDLs.Add( Mails( MRow ).Id );
						Owned.Append( MRow );
					}

					Row = Wanted.Next( Row );
				}
			}
		}

		// _U_IDLs to _S_ubjects
		namespace u2s_ {
			// _U_idls to _n_umbers
			namespace u2n_ {
				namespace {
					void Get_(
						const dUIDLs &UIDLs,
						const muamel::dRows &Wanted,
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
								Numbers.Add( AllNumbers( Row ) );
								Available.Append( Wanted( Row ) );
							}

							Row = UIDLs.Next( Row );
						}
					}
				}

				void Get(
					const dUIDLs &UIDLs,
					const muamel::dRows &Wanted,
					fdr::rIODriver &Driver,
					dNumbers &Numbers,
					muamel::dRows &Available )				{
				qRH
					muapo3::wNumbers AllNumbers;
					muapo3::wUIDLs AllUIDLs;
				qRB
					tol::Init( AllNumbers, AllUIDLs );

					muapo3::GetUIDLs( Driver, AllNumbers, AllUIDLs );

					Get_( UIDLs, Wanted, AllUIDLs, AllNumbers, Numbers, Available );
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
				}

				void Get(
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
			}

			void Get(
				const dUIDLs &UIDLs,
				const muamel::dRows &Wanted,
				muaagt::sRow AgentRow,
				const muaagt::dAgent &Agent,
				str::dStrings &Subjects,
				muamel::dRows &Available )
			{
			qRH
				csdbnc::rIODriver Driver;
				muapo3::wNumbers Numbers;
			qRB
				muaagt::InitAndAuthenticate( Agent, Driver );

				tol::Init( Numbers );
				u2n_::Get( UIDLs, Wanted, Driver, Numbers, Available );

				n2s_::Get( Numbers, Driver, Subjects );

				muapo3::Quit( Driver );
			qRR
			qRT
			qRE
			}
		}
		
		void Get_(
			const muamel::dMails &Mails,
			const muamel::dRows &Wanted,
			muaagt::sRow AgentRow,
			const muaagt::dAgent &Agent,
			str::dStrings &Subjects,
			muamel::dRows &Available )
		{
		qRH
			csdbnc::rIODriver Driver;
			muapo3::wUIDLs UIDLs;
			muamel::wRows Owned;
		qRB
			tol::Init( UIDLs, Owned );	// Owned by agent.
			m2u_::Get( AgentRow, Mails, Wanted, UIDLs, Owned );

			muaagt::InitAndAuthenticate( Agent, Driver );

			u2s_::Get( UIDLs, Owned, AgentRow, Agent, Subjects, Available );
		qRR
		qRT
		qRE
		}
	}

	void Get(
		const muamel::dMails &Mails,
		const muamel::dRows &Wanted,
		const muaagt::dAgents &Agents,
		str::dStrings &Subjects,
		muamel::dRows &Available )
	{
		muaagt::sRow Row = Agents.First();

		while ( Row != qNIL ) {
			Get_( Mails, Wanted, Row, Agents.Core( Row ), Subjects, Available );

			Row = Agents.Next( Row);
		}

	}
}

void muaacc::dAccount::GetFields(
	const muamel::dRows &Wanted,
	str::dStrings &Subjects,
	muamel::dRows &Available ) const
{
	return get_fields_::Get( Mails, Wanted, Agents, Subjects, Available ); 
}

