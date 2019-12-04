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

#ifndef COMMON_INC_
# define COMMON_INC_

# include "muaacc.h"
# include "muabsc.h"
# include "muainf.h"
# include "muaplg.h"

# include "fblbkd.h"

# include "sclbacknd.h"

namespace common {

	void Initialize( void );

	bso::bool__ IsInitialized( void );

	class rStuff	// Contains data peculiar to a backend, each (virtual) connection having its own backend.
	{
	private:
		muaacc::sRow Account_;
	public:
		void reset( bso::bool__ P  = true )
		{
			Account_ = qNIL;
		}
		E_CVDTOR( rStuff );
		void Init( void )
		{
			Account_ = qNIL;
		}
		void SetAccount( muaacc::sRow Account )
		{
			if ( Account_ != qNIL )
				qRGnr();

			Account_ = Account;
		}
		bso::sBool IsLogged( void ) const
		{
			return Account_ != qNIL; 
		}
		muaacc::sRow GetAccount( void ) const
		{
			if ( !IsLogged() )
				qRGnr();

			return Account_;
		}
	};

	typedef muaacc::lAuthentication lAuthentication_;

	class rAuthentication
	: public lAuthentication_
	{
	private:
		plgn::rRetriever<muaplg::sAuthentication> Retriever_;
		muaplg::sAuthentication &P_( void )
		{
			return Retriever_.Plugin();
		}
	public:
		void reset( bso::sBool P = true )
		{
			lAuthentication_::reset(P );
			Retriever_.reset( P );
		}
		qCDTOR( rAuthentication );
		void Init(
			const char *Identifier,
			muaacc::lAccounts &Accounts );
	};

	muaacc::lAuthentication &Authentication( void );

	muaacc::lAccounts &Accounts( void );
}

// '##' needed by 'clang++'/'g++'.
# define REPORT( message, ... ) sclmisc::ReportAndAbort( message_::message, ##__VA_ARGS__  )

# define BACKEND_ ( *(sclbacknd::rBackend *)BaseBackend.UP() )
# define STUFF_ ( *(common::rStuff *)BACKEND_.Stuff() )


// 'h' suffix : to put in error header  (between 'qRH' and 'qRB').
// 'b' suffix : to put in error body  (just after and 'qRB').

#define ACCOUNTh muaacc::lAccount AccountAccess;

#define ACCOUNTb\
	if ( !STUFF_.IsLogged() )\
		REPORT( NotLoggedIn );\
	AccountAccess.Init( common::Accounts().Get( STUFF_.GetAccount() ) );\
	muaacc::dAccount &Account = AccountAccess()

#define BACKENDb sclbacknd::rBackend &Backend = BACKEND_

#define STUFFb common::rStuff &Stuff = STUFF_

# define AUTHENTICATIONb	muaacc::lAuthentication &Authentication = common::Authentication()

#endif
