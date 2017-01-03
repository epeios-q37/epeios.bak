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

#include "muaacc.h"

#include "muaimf.h"
#include "muapo3.h"

#include "crt.h"

using namespace muaacc;

void muaacc::dAccount::RemoveAgent( muaagt::sRow Agent )
{
qRH
	muamel::wRows Mails;
qRB
	Mails.Init();

	Tracker_.GetMails( Agent, Mails );

	Directory_.Remove( Mails );
	Tracker_.Remove( Mails );

	Agents_.Remove( Agent );
qRR
qRT
qRE
}


namespace update_ {
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

		void Update_(
			muaagt::sRow Agent,
			muaagt::dAgents &Agents,
			muatrk::dTracker &Tracker,
			muadir::dDirectory &Directory )
		{
		qRH
			csdbnc::rIODriver Driver;
			muapo3::wNumbers Numbers;
			muapo3::wUIDLs UIDLs;
		qRB
			if ( Agents.InitAndAuthenticateIfEnabled( Agent, Driver ) ) {
				tol::Init( Numbers, UIDLs );

				muapo3::GetUIDLs( Driver, Numbers, UIDLs );

				muapo3::Quit( Driver );

				Update_( Agent, UIDLs, Tracker, Directory );
			}
		qRR
		qRT
		qRE
		}
	}

	void Update(
		muaagt::dAgents &Agents,
		muatrk::dTracker &Tracker,
		muadir::dDirectory &Directory )
	{
		muaagt::sRow Row = Agents.First();

		while ( Row != qNIL ) {
			Update_( Row, Agents, Tracker, Directory );

			Row = Agents.Next( Row );
		}
	}

}

void muaacc::dAccount::Update( muaagt::sRow Agent )
{
	update_::Update_( Agent, Agents_, Tracker_, Directory_ );
}

void muaacc::dAccount::Update( void )
{
	update_::Update( Agents_, Tracker_, Directory_ );
}

namespace get_fields_ {
	namespace {
		using muapo3::sNumber;
		using muapo3::dNumbers;
		using muapo3::dUIDLs;

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
						muaagt::sRow OwningAgent,
						fdr::rIODriver &Driver,
						str::dStrings &Subjects,
						muaagt::dRows &Agents )
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
							Agents.Add( OwningAgent );
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
						Get_( Numbers( Row ), OwningAgent, Driver, Subjects, Agents );

						Row = Numbers.Next( Row );
					}
				}
			}

			void Get(
				const dUIDLs &UIDLs,
				const muamel::dRows &Wanted,
				muaagt::sRow Agent,
				muaagt::dAgents &Agents,
				str::dStrings &Subjects,
				muaagt::dRows &CorrespondingAgents,
				muamel::dRows &Available )
			{
			qRH
				csdbnc::rIODriver Driver;
				muapo3::wNumbers Numbers;
			qRB
				if ( Agents.InitAndAuthenticateIfEnabled( Agent, Driver ) ) {
					tol::Init( Numbers );
					u2n_::Get( UIDLs, Wanted, Driver, Numbers, Available );

					n2s_::Get( Numbers, Agent, Driver, Subjects, CorrespondingAgents );

					muapo3::Quit( Driver );
				}
			qRR
			qRT
			qRE
			}
		}
		
		void Get_(
			const muadir::dDirectory &Directory,
			const muatrk::dTracker &Tracker,
			const muamel::dRows &Wanted,
			muaagt::sRow AgentRow,
			muaagt::dAgents &Agents,
			str::dStrings &Subjects,
			muaagt::dRows &CorrespondingAgents,
			muamel::dRows &Available )
		{
		qRH
			muapo3::wUIDLs UIDLs;
			muamel::wRows Owned;
		qRB
			Owned.Init();
			Tracker.FilterOutMails( Wanted, AgentRow, Owned );

			if ( Owned.Amount() != 0 ) {
				UIDLs.Init();
				Directory.GetIds( Owned, UIDLs );

				u2s_::Get( UIDLs, Owned, AgentRow, Agents, Subjects, CorrespondingAgents, Available );
			}
		qRR
		qRT
		qRE
		}
	}

	void Get(
		const muadir::dDirectory &Directory,
		const muatrk::dTracker &Tracker,
		const muamel::dRows &Wanted,
		muaagt::dAgents &Agents,
		str::dStrings &Subjects,
		muaagt::dRows &CorrespondingAgents,
		muamel::dRows &Available )
	{
		muaagt::sRow Row = Agents.First();

		while ( Row != qNIL ) {
			Get_( Directory, Tracker, Wanted, Row, Agents, Subjects, CorrespondingAgents, Available );

			Row = Agents.Next( Row);
		}

	}
}

void muaacc::dAccount::GetFields(
	const muamel::dRows &Wanted,
	str::dStrings &Subjects,
	muaagt::dRows &CorrespondingAgents,
	muamel::dRows &Available )
{
	return get_fields_::Get( Directory_, Tracker_, Wanted, Agents_, Subjects, CorrespondingAgents, Available ); 
}

const str::dString &muaacc::dAccount::GetMail(
	muamel::sRow MailRow,
	str::dString &Mail )
{
qRH
	muaagt::sRow AgentRow = qNIL;
	csdbnc::rIODriver Driver;
	muapo3::sNumber Number = 0;
qRB
	if ( Agents_.InitAndAuthenticateIfEnabled(Tracker_.GetMailAgent(MailRow), Driver ) ) {
		Number = muapo3::GetNumberForUIDL( Directory_.Mails()( MailRow ).Id, Driver );

		if ( Number == 0 )
			qRGnr();

		muapo3::GetMessage( Number, Driver, Mail );

		muapo3::Quit( Driver );
	}
qRR
qRT
qRE
	return Mail;
}

