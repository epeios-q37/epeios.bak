/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// Client-Server Devices Muxed Network Client.

#ifndef CSDMNC__INC
# define CSDMNC__INC

# define CSDMNC_NAME		"CSDMNC"

# if defined( E_DEBUG ) && !defined( CSDMNC_NODBG )
#  define CSDMNC_DBG
# endif

# include "csdmxc.h"
# include "csdbnc.h"

# include "err.h"
# include "csdbnc.h"

#ifdef CPE_F_MT
#	define CSDMNC__MT
#endif

#ifdef CSDMNC__MT
#	include "mtx.h"
#endif

namespace csdmnc {
	typedef csdbnc::rIODriver rIODriver_;

	typedef csdmxc::cCallback cCallback_;

	class rCallback
	: public cCallback_
	{
	private:
		qCBUFFERr HostService_;
		sck::duration__ Timeout_;
		rIODriver_ *DriverAsPointer_( void *UP )
		{
			if ( UP == NULL )
				qRFwk();

			return (rIODriver_ *)UP;
		}
		rIODriver_ &D_( void *UP )
		{
			return *DriverAsPointer_( UP );
		}
	protected:
		virtual void *CSDMXCNew( void ) override
		{
			rIODriver_ *IODriver = new rIODriver_;

			if ( IODriver == NULL )
				qRAlc();

			if ( !IODriver->Init( HostService_, Timeout_, err::hUserDefined ) ) {
				delete IODriver;
				IODriver = NULL;
			}

			return IODriver;
		}
		virtual csdmxc::rIODriver &CSDMXCExtractDriver( void *UP ) override
		{
			return D_( UP );
		}
		virtual void CSDMXCRelease( void *UP ) override
		{
			delete DriverAsPointer_( UP );
		}
		virtual time_t CSDMXCEpochTimeStamp( void *UP ) override
		{
			return D_( UP ).EpochTimeStamp();
		}
	public:
		void reset( bso::sBool = true )
		{
			Timeout_ = sck::NoTimeout;
		}
		E_CVDTOR( rCallback );
		void Init(
			const char *HostService,
			sck::duration__ Timeout )
		{
			HostService_.Calloc( strlen( HostService ) + 1 );
			strcpy( HostService_, HostService );
			Timeout_ = Timeout;
		}
	};

	typedef csdmxc::rCore rCore_;

	using csdmxc::cLog;
	
	class rCore
	: public rCore_
	{
	private:
		rCallback Callback_;
	public:
		void reset( bso::sBool P = true )
		{
			rCore_::reset( P );
			Callback_.reset( P );
		}
		qCDTOR( rCore );
		bso::sBool Init(
			const char *HostService,
			bso::uint__ PingDelay,
			sck::duration__ Timeout,
			cLog *LogCallback = NULL )
		{
			Callback_.Init( HostService, Timeout );

			return rCore_::Init( Callback_, PingDelay, LogCallback );
		}
		bso::bool__ Init(
			const char *HostService,
			bso::uint__ PingDelay,
			sck::duration__ Timeout,
			cLog &LogCallback )
		{
			return Init( HostService, PingDelay, Timeout, &LogCallback );
		}
	};

	using csdmxc::rClientIOFlow;

}

#endif
