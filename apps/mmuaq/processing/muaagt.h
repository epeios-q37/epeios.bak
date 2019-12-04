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

// MUA AGenT

#ifndef MUAAGT_INC_
# define MUAAGT_INC_

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muabsc.h"
# include "muaima.h"
# include "muapo3.h"

# include "bitbch.h"
# include "csdbnc.h"
# include "lck.h"
# include "lstbch.h"
# include "crt.h"
# include "lstcrt.h"
# include "tht.h"

namespace muaagt {
	qROW( Row );	// Agent row.
	qROWS( Row );

	qENUM( Protocol ) {
		pPOP3,
		pIMAP,
		p_amount,
		p_Undefined
	};

	inline eProtocol GetProtocol( const str::dString &Pattern )
	{
		if ( Pattern == "POP3" )
			return pPOP3;
		else if ( Pattern == "IMAP" )
			return pIMAP;
		else
			qRGnr();

		return p_Undefined;	// To avoid a warnin.
	}

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
		dAgent &operator =(const dAgent &A)
		{
			S_.Protocol = A.S_.Protocol;
			HostPort = A.HostPort;
			Username = A.Username;
			Password = A.Password;

			return *this;
		}
		void Init( eProtocol Protocol )
		{
			S_.Protocol = Protocol;
			tol::Init( HostPort, Username, Password );
		}
		qRODISCLOSEd( eProtocol, Protocol );
	};

	qW( Agent );

	typedef str::dString dName;
	typedef str::wString wName;

	class dMetaData {
	private:
		dName Name_;
	public:
		struct s {
			dName::s Name;
			bso::sBool Disabled;
		} &S_;
		dMetaData( s &S )
		: S_( S ),
		  Name_( S.Name )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Name_, S_.Disabled );
		}
		void plug( uys::sHook &Hook )
		{
			Name_.plug( Hook );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Name_ );
		}
		dMetaData &operator = (const dMetaData &MD)
		{
			Name_ = MD.Name_;
			S_.Disabled = MD.S_.Disabled;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Name_ );
			S_.Disabled = false;
		}
		void Init( const dName &Name )
		{
			this->Name_.Init( Name );
			S_.Disabled = false;
		}
		const str::dString &Name( void ) const
		{
			return Name_;
		}
		// For the 'tol::Search(...)' function.
		const str::dString &ID( void ) const
		{
			return Name_;
		}
		void SetDisabledState( bso::sBool Value )
		{
			S_.Disabled = Value;
		}
		bso::sBool GetDisabledState( void ) const
		{
			return S_.Disabled;
		}
	};

	qW( MetaData );

	typedef lstcrt::qLCRATEd( dAgent, sRow ) dAgents_;

	typedef lstcrt::qLMCRATEd( dMetaData, sRow ) dMetaDatas;

	class rRack
	{
	private:
		csdbnc::rIODriver NetDriver_;
		flx::rIOMonitor MonitorDriver_;
		muaima::rSession IMAPSession_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, NetDriver_, MonitorDriver_, IMAPSession_ );
		}
		qCDTOR( rRack );
		void Init( void );
		fdr::rIODriver &POP3( void )
		{
			return MonitorDriver_;
		}
		muaima::rSession &IMAP( void )
		{
			return IMAPSession_;
		}
		friend class dAgents;
	};

	class dAgents {
	private:
		dAgents_ Core_;
		dMetaDatas MetaDatas_;
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
			eProtocol Protocol,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password )
		{
			Agent.Init( Protocol );

			Set_( Agent, HostPort, Username, Password );
		}
		bso::sBool IsDisabled_( sRow Agent ) const
		{
			return MetaDatas_( Agent ).GetDisabledState();
		}
		bso::sBool IsEnabled_( sRow Agent ) const
		{
			return !IsDisabled_( Agent );
		}
		void Disable_( sRow Agent );
		// Returns 'false' if the agent is disabled.
	public:
		struct s {
			dAgents_::s Core;
			dMetaDatas::s MetaDatas;
		};
		dAgents( s &S )
		: Core_( S.Core ),
		  MetaDatas_( S.MetaDatas )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, MetaDatas_ );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Core_, MetaDatas_ );
		}
		dAgents &operator =(const dAgents &A)
		{
			Core_ = A.Core_;
			MetaDatas_ = A.MetaDatas_;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Core_, MetaDatas_ );
		}
		const dAgents_ &Core( void ) const
		{
			return Core_;
		}
		const str::dString &GetName(
			sRow Agent,
			str::dString &Name ) const
		{
			Name.Append( MetaDatas_( Agent ).Name() );

			return Name;
		}
		bso::sBool IsEnabled( sRow Agent ) const
		{
			return IsEnabled_( Agent );
		}
		qNAV( Core_., sRow );
		sRow New(
			const str::dString &Name,
			eProtocol Protocol,
			const str::dString &HostPort,
			const str::dString &Username,
			const str::dString &Password )
		{
			sRow Row = MetaDatas_.New();

			if ( Row != Core_.New() )
				qRGnr();

			MetaDatas_( Row ).Init( Name );

			Init_( Core_( Row ), Protocol, HostPort, Username, Password );

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

			MetaDatas_( Row ).Init( Name );
			Set_( Core_( Row ), HostPort, Username, Password );
		}
		void Update(
			sRow Row,
			const str::dString &Name,
			const str::dString &HostPort,
			const str::dString &Username )
		{
			if ( !Exists( Row ) )
				qRGnr();

			MetaDatas_( Row ).Init( Name );
			Set_( Core_( Row ), HostPort, Username );

		}
		void Remove( sRow Agent )
		{
			Core_.Remove( Agent );
			MetaDatas_.Remove( Agent );
		}
		sRow Search( const str::dString &Name ) const
		{
			return tol::Search<sRow>( Name, MetaDatas_ );
		}
		// If returns 'p_Undefined', this means failure.
		eProtocol InitAndAuthenticateIfEnabled(
			sRow Agent,
			rRack &Rack );
	};

}

#endif
