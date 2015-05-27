/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDBNS__INC
# define CSDBNS__INC

# define CSDBNS_NAME		"CSDBNS"

# if defined( E_DEBUG ) && !defined( CSDBNS_NODBG )
#  define CSDBNS_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Client-Server Base Network Server

# include "err.h"
# include "sck.h"
# include "cpe.h"

# include "csdscb.h"

# define CSDNBS__DEFAULT_TIMEOUT	SCK__DEFAULT_TIMEOUT

namespace csdbns {
	using namespace sck;

	using namespace csdscb;

	//t The type of a port.
	typedef bso::u16__	port__;

	class socket_callback__ {
	protected:
		virtual void *CSDBNSPreProcess(
			sck::socket__ Socket,
			const char *IP ) = 0;
		virtual action__ CSDBNSProcess(
			sck::socket__ Socket,
			void *UP ) = 0;
		virtual void CSDBNSPostProcess( void *UP ) = 0;
	public:
		void *PreProcess(
			sck::socket__ Socket,
			const char *IP )
		{
			return CSDBNSPreProcess( Socket, IP );
		}
		action__ Process(
			sck::socket__ Socket,
			void *UP )
		{
			return CSDBNSProcess( Socket, UP );
		}
		void PostProcess( void *UP )
		{
			CSDBNSPostProcess( UP );
		}
	};

	//c To listen at a port..
	class listener___
	{
	private:
		socket__ Socket_;
		socket_callback__ *_Callback;
		void *_UP;
		bso::bool__ _CallbackAvailable( void ) const	// Retourne 'true', si un 'callback' est prsent.
		{
			return _Callback != NULL;
		}
		socket__ _Interroger(
			err::handling__ ErrorHandling,
			sck::duration__ TimeOut,
			const char *&IP );
	public:
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( Socket_ != SCK_INVALID_SOCKET )
					Close( Socket_ );

				if ( _CallbackAvailable() )
					_Callback->PostProcess( _UP );	// Mme si 'UP' != NULL;
			}

			Socket_ = SCK_INVALID_SOCKET;
			_Callback = NULL;
			_UP = NULL;
		}
		listener___( void )
		{
			reset( false );
		}
		~listener___( void )
		{
			reset( true );
		}
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default );
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
			return Init( Port, Amount, ErrorHandling );
		}
		//f Return the first available connection. BLOCKING FUNCTION if 'TimeOut == 'SCK_INFINITE'.
		socket__ GetConnection(
			const char *&IP,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = SCK_INFINITE )
		{
			return _Interroger( ErrorHandling, TimeOut, IP );
		}
		//f Initialize 'Socket' with the first connection available. BLOCKING FUNCTION if 'TimeOut' == 'SCK_INFINITE'.
		void GetConnection(
			socket__ &Socket,
			const char *&IP,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = 0 )
		{
			Socket = _Interroger( ErrorHandling, TimeOut, IP );
		}
		// If returned value = 'true', then exiting is because 'TimeOut' reached.
		// If returned value == 'false', then underlying user function retuns 'bStop'.
		bso::bool__ Process(
			socket_callback__ &Callback,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = SCK_INFINITE );
	};

#ifdef CPE_MT

	struct _flow_data__ {
		sck::socket_ioflow___ Flow;
		void *UP;
	};

	class _callback__
	: public socket_callback__
	{
	protected:
		virtual void *CSDBNSPreProcess(
			socket__ Socket,
			const char *IP )
		{
			_flow_data__ *Data = NULL;
		ERRProlog
		ERRBegin
			Data = new _flow_data__;

			if ( Data == NULL )
				ERRAlc();

			Data->Flow.Init( Socket );
			Data->UP = BaseCallback->PreProcess( IP );
		ERRErr
			if ( Data != NULL )
				delete Data;
			Data = NULL;
		ERREnd
		ERREpilog
			return Data;
		}
		virtual action__ CSDBNSProcess(
			socket__ Socket,
			void *UP )
		{
			_flow_data__ &Data = *(_flow_data__ *)UP;
#ifdef CSDNBS_DBG
			if ( Data.Flow.GetSocket() != Socket )
				ERRc();
#endif

			return BaseCallback->Process( Data.Flow, Data.UP );
		}
		virtual void CSDBNSPostProcess( void *UP )
		{
			if ( UP == NULL )
				ERRPrm();

			BaseCallback->PostProcess( ((_flow_data__ *)UP)->UP );

			delete (_flow_data__ *)UP;
		}
	public:
		csdscb::callback__ *BaseCallback;
	};

	class server___
	: public listener___
	{
	private:
		socket_callback__ *_SocketCallback;
		_callback__ _Callback;
	public:
		void reset( bool P = true )
		{
			listener___::reset( P );
		}
		server___( void )
		{
			reset( false );
			_SocketCallback = NULL;
		}
		~server___( void )
		{
			reset( true );
		}
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			socket_callback__ &SocketCallback,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_SocketCallback = &SocketCallback;
			
			return listener___::Init( Port, Amount, ErrorHandling );
		}
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			socket_callback__ &SocketCallback,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_SocketCallback = &SocketCallback;

			return listener___::Init( Port, Amount, ErrorHandling );
		}
		bso::bool__ Init(
			port__ Port,
			csdscb::callback__ &Callback,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_Callback.BaseCallback = &Callback;

			return Init( Port, this->_Callback, Amount, ErrorHandling );
		}
		bso::bool__ Init(
			port__ Port,
			csdscb::callback__ &Callback,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_Callback.BaseCallback = &Callback;

			return Init( Port, this->_Callback, Amount, ErrorHandling );
		}
		bso::bool__ LaunchService( const char *ServiceName )
		{
#ifdef CSD__HANDLE_SERVICE
			return _service__::Launch( ServiceName );
#else
			return false;
#endif
		}
		void Process(
			sck::duration__ TimeOut = CSDNBS__DEFAULT_TIMEOUT,
			err::handling__ ErrorHandling = err::h_Default );
	};

#endif
}
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
