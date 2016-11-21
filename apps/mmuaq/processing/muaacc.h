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

// MUA ACCount

#ifndef MUAACC__INC
# define MUAACC__INC

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muaagt.h"
# include "muabsc.h"
# include "muamel.h"

# include "bitbch.h"
# include "lck.h"
# include "lstbch.h"
# include "lstcrt.h"
# include "tht.h"

namespace muaacc {
	qROW( Row );	// Account row.

	class cProvider
	{
	protected:
		virtual sRow MUAACCProvide( void ) = 0;
	public:
		qCALLBACK( Provider );
		sRow Provide( void )
		{
			return MUAACCProvide();
		}
	};

	class lProvider
	{
	private:
		qRMV( cProvider, C_, Callback_ );
		tht::rLocker Locker_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Locker_, Callback_ );
		}
		qCDTOR( lProvider );
		void Init( cProvider &Callback )
		{
			Callback_ = &Callback;
			tol::Init( Locker_ );
		}
		sRow Provide( void )
		{
			sRow Row = qNIL;
		qRH
		qRB
			Locker_.Lock();
			Row = C_().Provide();
		qRR
		qRT
			Locker_.Unlock();
		qRE
			return Row;
		}
	};

	class cAuthentication
	{
	protected:
		virtual sRow MUAACCAuthenticate(
			const str::dString &Username,
			const str::dString &Password,
			lProvider &Provider ) = 0;
	public:
		qCALLBACK( Authentication );
		sRow Authenticate(
			const str::dString &Username,
			const str::dString &Password,
			lProvider &Provider )
		{
			return MUAACCAuthenticate( Username, Password, Provider );
		}
	};

	class dAccount
	{
	private:
	public:
		struct s {
			muaagt::dAgents::s Agents;
			muamel::dMails::s Mails;
		};
		muaagt::dAgents Agents;
		muamel::dMails Mails;
		dAccount( s &S )
		: Agents( S.Agents ),
		  Mails( S.Mails )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Agents, Mails );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Agents, Mails );
		}
		dAccount &operator =( const dAccount &A )
		{
			Agents = A.Agents;
			Mails = A.Mails;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Agents, Mails );
		}
		void GetAllMails( muamel::dRows &Rows ) const;
		void Update( void );	// Updates the mail with the content on the different agent.
		void GetFields(
			const muamel::dRows &Wanted,
			str::dStrings &Subjects,
			muamel::dRows &Available ) const;	// Returns only subjects now. Will return other fields in the future.
	};

	qW( Account );

	typedef lck::rTutor<dAccount> rAccountTutor_;

	class rAccountTutor
	: public rAccountTutor_
	{
	public:
		wAccount Core;
		void reset( bso::sBool P = true )
		{
			rAccountTutor_::reset( P );
			tol::reset( P, Core );
		}
		qCDTOR( rAccountTutor );
		void Init( void )
		{
			Core.Init();
			rAccountTutor_::Init( Core );
		}
	};


	typedef lck::rReadWriteAccess<dAccount> lAccount;

	class cAccount
	{
	protected:
		virtual void MUAACCOnCreate( dAccount &Account ) = 0;
	public:
		qCALLBACK( Account );
		void OnCreate( dAccount &Account )
		{
			return MUAACCOnCreate( Account );
		}
	};


	class lAccounts
	{
	private:
		tht::rLocker Locker_;
		lstbch::qLBUNCHw( rAccountTutor *, sRow ) Accounts_;
		qRMV( cAccount, C_, Callback_ );
		void Free_( void )
		{
			sRow Row = Accounts_.First();

			while ( Row != qNIL ) {
				delete Accounts_( Row );

				Row = Accounts_.Next( Row );
			}

			Accounts_.Init();
		}
		sRow New_( sRow Row )
		{
		qRH
			rAccountTutor *Account = NULL;
		qRB
			if ( ( Account = new rAccountTutor ) == NULL )
				qRAlc();

			Account->Init();

			Row = Accounts_.New( Row );

			C_().OnCreate( Account->Core );

			Accounts_.Store( Account, Row );
		qRR
			if ( Account != NULL )
				delete Account;
		qRE
		qRT
			return Row;
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				Free_();
			}

			tol::reset( P, Locker_, Accounts_, Callback_ );
		}
		void Init( cAccount &Callback )
		{
			Free_();

			tol::Init( Locker_, Accounts_ );
			Callback_ = &Callback;
		}
		sRow New( void )
		{
			sRow Row = qNIL;
		qRH
		qRB
			Locker_.Lock();
		
			Row = New_( qNIL );

			Locker_.Unlock();
		qRR
		qRT
			Locker_.UnlockIfLocked();
		qRE
			return Row;
		}
		rAccountTutor &Get( sRow Row )
		{
			rAccountTutor *Account = NULL;
		qRH
		qRB
			if ( Row == qNIL )
				qRGnr();

			Locker_.Lock();
		
			if ( !Accounts_.Exists( Row ) )
				New_( Row );

			Accounts_.Recall( Row, Account );

			Locker_.Unlock();
		qRR
		qRT
			Locker_.UnlockIfLocked();
		qRE
			return *Account;
		}
	};

	class rRack
	: public lAccounts
	{
	public:
		void reset( bso::sBool P = true )
		{
			lAccounts::reset( P );
		}
		qCDTOR( rRack );
		void Init( cAccount &Callback )
		{
			lAccounts::Init( Callback );
		}
	};

	class lAuthentication
	: public cProvider
	{
	private:
		tht::rLocker Locker_;
		qRMV( cAuthentication, C_, Callback_ );
		qRMV( lAccounts, A_, Accounts_ );
		lProvider Provider_;
		sRow Authenticate_(
			const str::dString &Username,
			const str::dString &Password )
		{
			sRow Row = qNIL;
		qRH
		qRB
			Locker_.Lock();
			Row = C_().Authenticate( Username, Password, Provider_ );
		qRR
		qRT
			Locker_.Unlock();
		qRE
			return Row;
		}
		sRow MUAACCProvide( void ) override
		{
			return A_().New();
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Provider_, Callback_, Accounts_, Locker_ );
		}
		qCDTOR( lAuthentication );
		void Init(
			cAuthentication &Callback,
			lAccounts &Accounts )
		{
			Callback_ = &Callback;
			Accounts_ = &Accounts;
			Provider_.Init( *this );
			Locker_.Init();
		}
		sRow Authenticate(
			const str::dString &Username,
			const str::dString &Password )
		{
			return Authenticate_( Username, Password );
		}
	};
}

#endif
