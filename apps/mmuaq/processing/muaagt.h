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

// MUA AGenT

#ifndef MUAAGT__INC
# define MUAAGT__INC

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muabsc.h"

# include "bitbch.h"
# include "lck.h"
# include "lstbch.h"
# include "lstcrt.h"
# include "tht.h"

namespace muaagt {
	qROW( Row );	// Agent row.

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

	namespace agent_ {

		typedef lstcrt::qLCRATEd( dAgent, sRow ) dAgents;

		typedef str::dString dName;
		typedef lstcrt::qLMCRATEd( dName, sRow ) dNames;
	}


	class dAgents {
	private:
		void Set_(
			dAgent &Agent,
			const str::dString &HostPort,
			const str::dString &Username )
		{
			Agent.HostPort = HostPort;
			Agent.Username = Username;
		}
		void Set_(
			dAgent &Agent,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password )
		{
			Set_( Agent, HostPort, Username );
			Agent.Password = Password;
		}
		void Init_(
			dAgent &Agent,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password )
		{
			Agent.Init();

			Set_( Agent, HostPort, Username, Password );
		}
	public:
		struct s {
			agent_::dAgents::s Core;
			agent_::dNames::s Names;
		};
		agent_::dAgents Core;
		agent_::dNames Names;
		dAgents( s &S )
		: Core( S.Core ),
		  Names( S.Names )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core, Names );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Core, Names );
		}
		dAgents &operator =(const dAgents &A)
		{
			Core = A.Core;
			Names = A.Names;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Core, Names );
		}
		qNAV( Names., sRow );
		sRow New(
			const str::dString &Name,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password )
		{
			sRow Row = Names.New();

			if ( Row != Core.New() )
				qRGnr();

			Names( Row ).Init( Name );

			Init_( Core( Row ), HostPort, Username, Password );

			return Row;
		}
		void Update(
			sRow Row,
			const str::dString &Name,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password )
		{
			if ( !Exists( Row ) )
				qRGnr();

			Names( Row ) = Name;
			Set_( Core( Row ), HostPort, Username, Password );

		}
		void Update(
			sRow Row,
			const str::dString &Name,
			const str::dString &HostPort,
			const str::dString &Username )
		{
			if ( !Exists( Row ) )
				qRGnr();

			Names( Row ) = Name ;
			Set_( Core( Row ), HostPort, Username );

		}
		sRow Search( const str::dString &Name ) const;
	};
}

#endif
