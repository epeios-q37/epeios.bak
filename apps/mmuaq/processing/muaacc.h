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

# include "bitbch.h"
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

	qENUM( Protocol ) {
		pPOP3,
		p_amount,
		p_Undefined
	};

	class dAgent
	{
	public:
		struct s {
			eProtocol Protocol;
			str::dString::s
				HostPort,
				Username,
				Password;
		} &S_;
		str::dString
			HostPort,
			Username,
			Password;
		dAgent( s &S )
		: S_( S ),
		  HostPort( S.HostPort ),
		  Username( S.Username ),
		  Password( S.Password )
		{}
		void reset( bso::sBool P = true )
		{
			S_.Protocol = p_Undefined;
			tol::reset( P, HostPort, Username, Password );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, HostPort, Username, Password );
		}
		dAgent operator =(const dAgent &A)
		{
			S_.Protocol = A.S_.Protocol;
			HostPort = A.HostPort;
			Username = A.Username;
			Password = A.Password;

			return *this;
		}
		void Init( void )
		{
			S_.Protocol = p_Undefined;
			tol::Init( HostPort, Username, Password );
		}
	};

	qW( Agent );

	qROW( ARow );

	namespace agent_ {

		typedef lstcrt::qLCRATEd( dAgent, sARow ) dAgents;

		typedef str::dString dLabel;
		typedef lstcrt::qLMCRATEd( dLabel, sARow ) dLabels;
	}


	class dAgents {
	public:
		struct s {
			agent_::dAgents::s Agents;
			agent_::dLabels::s Labels;
		};
		agent_::dAgents Agents;
		agent_::dLabels Labels;
		dAgents( s &S )
		: Agents( S.Agents ),
		  Labels( S.Labels )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Agents, Labels );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Agents, Labels );
		}
		dAgents &operator =(const dAgents &A)
		{
			Agents = A.Agents;
			Labels = A.Labels;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Agents, Labels );
		}
		sARow New(
			const str::dString &Label,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password );
	};

	class dAccount
	{
	private:
	public:
		struct s {
			dAgents::s Agents;
		};
		dAgents Agents;
		dAccount( s &S )
		: Agents( S.Agents )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Agents );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Agents );
		}
		dAccount &operator =( const dAccount &A )
		{
			Agents = A.Agents;
			return *this;
		}
		bso::sBool Init( void )
		{
			Agents.Init();

			return true;
		}
	};

	typedef lstbch::qLBUNCHd( dAccount *, sRow ) dAccountPointers_;
	typedef lstbch::qLBUNCHd( tht::rLocker, sRow ) dAccountLockers_;

	class dAccounts
	{
	private:
		void Free_( void );
	public:
		struct s {
			dAccountPointers_::s Pointers;
			dAccountLocks_::s Locks;
		};
		dAccountPointers_ Pointers;
		dAccountLocks_ Locks;
		dAccounts( s &S )
		: Pointers( S.Pointers ),
		  Locks( S.Locks )
		{}
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				Free_();
			}

			tol::reset( P, Pointers, Locks );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Pointers, Locks );
		}
		dAccounts &operator =(const dAccounts &A)
		{
			Pointers = A.Pointers;
			Locks = A.Locks;

			return *this;
		}
		void Init( void )
		{
			Free_();

			tol::Init( Pointers, Locks );
		}
	};


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
