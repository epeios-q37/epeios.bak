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

#include "common.h"

#include "registry.h"

using namespace common;

 #define C( name )\
	case m##name:\
		return #name;\
		break


const char *common::GetLabel( eMessage Message )
{
	switch ( Message ){
	C( TestMessage );
	C( NotLoggedIn );
	C( AgentWithSuchNameExists );
	C( AgentNameCanNotBeEmpty );
	C( UnknownAgent );
	C( HostPortCanNotBeEmpty );
	C( UsernameCanNotBeEmpty );
/*
	C(  );
*/
	default:
		qRGnr();
		break;
	}

	return NULL;
}

#undef C

namespace {
	typedef muaacc::cAccount cAccount_;

	namespace fill_account_ {
		namespace {
#define G( name ) sclmisc::MGetValue( rgstry::rTEntry( registry::definition::demo::agent::Tagged##name, Id ), name );
			void Fill_(
				const str::dString &Id,
				muaacc::dAccount &Account )
			{
			qRH
				str::wString HostPort, Username, Password;
			qRB
				tol::Init( HostPort, Username, Password );

				G( HostPort );
				G( Username );
				G( Password );

				Account.Agents.New( Id, HostPort, Username, Password );
			qRR
			qRT
			qRE
			}
#undef G
			void Fill_(
				const str::dStrings &Ids,
				muaacc::dAccount &Account )
			{
				sdr::sRow Row = Ids.First();

				while ( Row != qNIL ) {
					Fill_( Ids( Row ), Account );

					Row = Ids.Next( Row );
				}
			}
		}

		void Fill( muaacc::dAccount &Account )
		{
		qRH
			str::wStrings Ids;
		qRB
			Ids.Init();
			sclmisc::GetValues( registry::definition::demo::agent::Id, Ids );

			Fill_( Ids, Account );
		qRR
		qRT
		qRE
		}
	}

	class sAccountCallback_
	: public cAccount_
	{
	protected:
		virtual void MUAACCOnCreate( muaacc::dAccount &Account ) override
		{
			fill_account_::Fill( Account );
		}
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( sAccountCallback_ );
		void Init( void )
		{
			// Standardization.
		}
	};

	class rRack_ {
	private:
		sAccountCallback_ AccountCallback_;
	public:
		rAuthentication Authentication;
		muaacc::rRack Accounts;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Authentication, Accounts, AccountCallback_ );
		}
		void Init( void )
		{
			tol::Init( AccountCallback_ );
			Accounts.Init( AccountCallback_ );
			Authentication.Init( NULL, Accounts );
		}
	} Rack_;

	bso::bool__ IsInitialized_ = false;
}

void common::rAuthentication::Init(
	const char *Identifier,
	muaacc::lAccounts &Accounts )
{
	Retriever_.Init();
	sclmisc::Plug( muaplg::AuthenticationPluginTarget, Identifier, Retriever_ );

	lAuthentication_::Init( P_(), Accounts );
}

void common::Initialize( void )
{
	if ( IsInitialized() )
		qRGnr();

	Rack_.Init();

	IsInitialized_ = true;
}

bso::bool__ common::IsInitialized( void )
{
	return IsInitialized_;
}

muaacc::lAuthentication &common::Authentication( void )
{
	return Rack_.Authentication;
}

muaacc::lAccounts &common::Accounts( void )
{
	return Rack_.Accounts;
}

