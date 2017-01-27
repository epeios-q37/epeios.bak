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
#include "muaaccim.h"
#include "muaaccp3.h"

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
		void Update_(
			muaagt::sRow Agent,
			muaagt::dAgents &Agents,
			muatrk::dTracker &Tracker,
			muadir::dDirectory &Directory )
		{
		qRH
			muaagt::rRack Rack;
			muapo3::wNumbers Numbers;
			muapo3::wUIDLs UIDLs;
		qRB
			Rack.Init();
			switch ( Agents.InitAndAuthenticateIfEnabled( Agent, Rack ) ) {
			case muaagt::pPOP3:
				muaaccp3::Update( Rack.POP3(), Tracker, Directory );
				break;
			case muaagt::pIMAP:
				muaaccim::Update( Rack.IMAP(), Tracker, Directory );
				break;
			case muaagt::p_Undefined:
				break;
			default:
				qRFwk();
				break;
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

	namespace u2s_ {
		void Get(
			const muamel::dRows &Wanted,
			const muamel::dIds &Ids,
			muaagt::sRow Agent,
			muaagt::dAgents &Agents,
			str::dStrings &Subjects,
			muaagt::dRows &CorrespondingAgents,
			muamel::dRows &Available )
			{
			qRH
				muaagt::rRack Rack;
				muapo3::wNumbers Numbers;
			qRB
				Rack.Init();
				switch ( Agents.InitAndAuthenticateIfEnabled( Agent, Rack ) ) {
				case muaagt::pPOP3:
					muaaccp3::GetFields( Wanted, Ids, Agent, Rack.POP3(), Subjects, CorrespondingAgents, Available );
					break;
				case muaagt::pIMAP:
					muaaccim::GetFields( Wanted, Ids, Agent, Rack.IMAP(), Subjects, CorrespondingAgents, Available );
					break;
				case muaagt::p_Undefined:
					break;
				default:
					qRFwk();
					break;
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
		muamel::wIds Ids;
		muamel::wRows Owned;
	qRB
		Owned.Init();
		Tracker.FilterOutMails( Wanted, AgentRow, Owned );

		if ( Owned.Amount() != 0 ) {
			Ids.Init();
			Directory.GetIds( Owned, Ids );

			u2s_::Get( Owned, Ids, AgentRow, Agents, Subjects, CorrespondingAgents, Available );
		}
	qRR
	qRT
	qRE
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
	muaagt::rRack Rack;
qRB
	Rack.Init();
	switch ( Agents_.InitAndAuthenticateIfEnabled( Tracker_.GetMailAgent(MailRow), Rack ) ) {
	case muaagt::pPOP3:
		muaaccp3::GetMail( Directory_.Mails()( MailRow ).Id, Rack.POP3(), Mail );
		break;
	case muaagt::pIMAP:
		muaaccim::GetMail( Directory_.Mails()( MailRow ).Id, Rack.IMAP(), Mail );
		break;
	case muaagt::p_Undefined:
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Mail;
}

