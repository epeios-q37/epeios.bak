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

#ifndef COMMON__INC
# define COMMON__INC

# include "muaacc.h"
# include "muabsc.h"
# include "muainf.h"
# include "muaplg.h"

# include "fblbkd.h"

# include "sclbacknd.h"

namespace common {

	qENUM( Message )
	{
		mTestMessage,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( eMessage Message );

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
}

#define REPORT( message ) sclmisc::ReportAndAbort( common::GetLabel( common::m##message ) )

#define STUFF\
	sclbacknd::rBackend &Backend = *(sclbacknd::rBackend *)BaseBackend.UP();\
	common::rStuff &Stuff = *(common::rStuff *)Backend.Stuff()

# define AUTHENTICATION	muaacc::lAuthentication &Authentication = common::Authentication()

#endif
