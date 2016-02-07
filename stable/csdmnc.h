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
	typedef csdbnc::flow___ rFlow_;

	typedef csdmxc::fCallback fCallback_;

	class rCallback
	: public fCallback_
	{
	private:
		qCBUFFERr HostService_;
		rFlow_ *FlowAsPointer_( void *UP )
		{
			if ( UP == NULL )
				qRFwk();

			return (rFlow_ *)UP;
		}
		rFlow_ &F_( void *UP )
		{
			return *FlowAsPointer_( UP );
		}
	protected:
		virtual void *CSDMXCNew( void ) override
		{
			rFlow_ *Flow = new rFlow_;

			if ( Flow == NULL )
				qRAlc();

			Flow->Init( HostService_ );

			return Flow;
		}
		virtual csdmxc::fFlow &CSDMXCExtractFlow( void *UP ) override
		{
			return F_( UP );
		}
		virtual void CSDMXCRelease( void *UP ) override
		{
			delete FlowAsPointer_( UP );
		}
		virtual time_t CSDMXCEpochTimeStamp( void *UP ) override
		{
			return F_( UP ).EpochTimeStamp();
		}
	public:
		void reset( bso::fBool P = true )
		{
			fCallback_::reset( P );
		}
		E_CVDTOR( rCallback );
		void Init( const char *HostService )
		{
			HostService_.Calloc( strlen( HostService ) + 1 );
			strcpy( HostService_, HostService );
		}
	};

	typedef csdmxc::rCore rCore_;

	using csdmxc::fLogCallback;
	
	class rCore
	: public rCore_
	{
	private:
		rCallback Callback_;
	public:
		void reset( bso::fBool P = true )
		{
			rCore_::reset( P );
			Callback_.reset( P );
		}
		qCDTOR( rCore );
		bso::fBool Init(
			const char *HostService,
			bso::uint__ PingDelay = 0,
			fLogCallback *LogCallback = NULL )
		{
			Callback_.Init( HostService );

			return rCore_::Init( Callback_, PingDelay, LogCallback );
		}
		bso::bool__ Init(
			const char *HostService,
			bso::uint__ PingDelay,
			fLogCallback &LogCallback )
		{
			return Init( HostService, PingDelay, &LogCallback );
		}
	};

	using csdmxc::rClientIOFlow;

}

#endif
