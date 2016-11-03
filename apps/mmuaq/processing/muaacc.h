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

# include "muabsc.h"

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
		};
		dAccount( s &S )
		{}
		void reset( bso::bool__ P = true )
		{
		}
		void plug( qASd *AS )
		{
		}
		dAccount &operator =( const dAccount &A )
		{
			return *this;
		}
		bso::sBool Init( void )
		{
			return true;
		}
	};

	typedef lstcrt::qLCRATEd( dAccount, sRow ) dAccounts;
	qW( Accounts );

	class lAccounts
	{
	private:
		tht::rLocker Locker_;
		qRMV( dAccounts, A_, Accounts_ );
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Locker_, Accounts_ );
		}
		qCDTOR( lAccounts );
		void Init( dAccounts &Accounts )
		{
			tol::Init( Locker_ );
			Accounts_ = &Accounts;
		}
		sRow New( void )
		{
			sRow Row = qNIL;
		qRH
		qRB
			Locker_.Lock();

			Row = A_().New();
			A_()( Row ).Init();
		qRR
		qRT
			Locker_.Unlock();
		qRE
			return Row;
		}
	};

	class rRack
	: public lAccounts
	{
	private:
		wAccounts Accounts_;
	public:
		void reset( bso::sBool P = true )
		{
			lAccounts::reset( P );
			tol::reset( P, Accounts_ );
		}
		qCDTOR( rRack );
		void Init( void )
		{
			Accounts_.Init();
			lAccounts::Init( Accounts_ );
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
