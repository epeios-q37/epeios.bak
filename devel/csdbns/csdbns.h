/*
	'csdbns.h' by Claude SIMON (http://zeusw.org/).

	'csdbns' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
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

# include "csdsuf.h"

# define CSDNBS__DEFAULT_TIMEOUT	SCK__DEFAULT_TIMEOUT

namespace csdbns {
	using namespace sck;

	using namespace csdsuf;

	//t The type of a port.
	typedef bso::u16__	port__;

	//c User functions with socket.
	class socket_user_functions__ {
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
		/* Retourne une socket sur une connection. FONCTION BLOQUANTE.
		Lorsque 'IgnorerErreur' à vrai, toute les erreurs sont ignorées, ceci pour garantir
		la présence du processus d'écoute. */
		socket_user_functions__ *_UserFunctions;
		void *_UP;
		bso::bool__ _UserFunctionsCalled( void ) const	// Retourne 'true', si une fonction utilisateurs a été appelée.
		{
			return _UserFunctions != NULL;
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

				if ( _UserFunctionsCalled() )
					_UserFunctions->PostProcess( _UP );	// Même si 'UP' != NULL;
			}

			Socket_ = SCK_INVALID_SOCKET;
			_UserFunctions = NULL;
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
			socket_user_functions__ &Functions,
			err::handling__ ErrorHandling = err::h_Default,
			sck::duration__ TimeOut = SCK_INFINITE );
	};

#ifdef CPE_MT

	struct _flow_data__ {
		sck::socket_ioflow___ Flow;
		void *UP;
	};

	class _functions__
	: public socket_user_functions__
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
			Data->UP = UserFunctions->PreProcess( IP );
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

			return UserFunctions->Process( Data.Flow, Data.UP );
		}
		virtual void CSDBNSPostProcess( void *UP )
		{
			if ( UP == NULL )
				ERRPrm();

			UserFunctions->PostProcess( ((_flow_data__ *)UP)->UP );

			delete (_flow_data__ *)UP;
		}
	public:
		user_functions__ *UserFunctions;
	};

	/*c Handling a server, with process duplication for each client. */
	/* Sous Windows, pour utiliser comme service :
		- Pour installer le service : 
			- 'InitService(...)' (Pas d'autres 'Init(...)',
			- 'InstallService(...)'.
		- Pour lancer le service (lorsque l'application lancée par le gestionnaire de service de 'Windows', auqual cas 'argc' == '0').
			- 'Init(...)',
			- 'LaunchService()'.
	*/
	class server___
	: public listener___
	{
	private:
		socket_user_functions__ *_SocketFunctions;
		_functions__ _Functions;
	public:
		void reset( bool P = true )
		{
			listener___::reset( P );
		}
		server___( void )
		{
			reset( false );
			_SocketFunctions = NULL;
		}
		~server___( void )
		{
			reset( true );
		}
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			socket_user_functions__ &SocketFunctions,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_SocketFunctions = &SocketFunctions;
			
			return listener___::Init( Port, Amount, ErrorHandling );
		}
		/*f Initialzation with 'Port' as port to listen.
		A maximum of 'Amount' are accepted in the waiting queue. */
		bso::bool__ Init(
			port__ Port,
			socket_user_functions__ &SocketFunctions,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_SocketFunctions = &SocketFunctions;

			return listener___::Init( Port, Amount, ErrorHandling );
		}
		bso::bool__ Init(
			port__ Port,
			user_functions__ &UserFunctions,
			err::handling__ ErrorHandling = err::h_Default,
			int Amount = 5 )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_Functions.UserFunctions = &UserFunctions;

			return Init( Port, this->_Functions, Amount, ErrorHandling );
		}
		bso::bool__ Init(
			port__ Port,
			user_functions__ &UserFunctions,
			int Amount,
			err::handling__ ErrorHandling = err::h_Default )
		{
#ifdef CSD__HANDLE_SERVICE
			if ( !_service__::Init() )
				ERRFwk();
#endif
			_Functions.UserFunctions = &UserFunctions;

			return Init( Port, this->_Functions, Amount, ErrorHandling );
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
