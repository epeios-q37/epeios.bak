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

// Client-Server Base Network Client 

#ifndef CSDBNC__INC
# define CSDBNC__INC

# define CSDBNC_NAME		"CSDBNC"

# if defined( E_DEBUG ) && !defined( CSDBNC_NODBG )
#  define CSDBNC_DBG
# endif

# include "err.h"
# include "flw.h"
# include "sck.h"

# if defined( CPE_S_POSIX )
#  define CSDBNC__POSIX
# elif defined( CPE_S_WIN )
#  define CSDBNC__WIN
# else
#  error
# endif

# if defined( CSDBNC__POSIX )
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
# elif !defined( CSDBNC__WIN )
#  error "Unknow compiler enviroment"
# endif

# define CSDBNC_ADDRESS_SIZE_MAX	100

/***************/
/***** OLD *****/
/***************/

namespace csdbnc {
	using namespace sck;

	typedef bso::char__	buffer__[CSDBNC_ADDRESS_SIZE_MAX+1];

	//f Return the host name contained in 'HostService'.
	const char *Host(
		const char *HostService,
		buffer__ Buffer );

#ifndef CPE_F_MT
	inline const char *Host( const char *HostService )
	{
		static buffer__ Buffer;

		return Host( HostService, Buffer );
	}
#endif

	//f Retourne le service contenu dans 'HostService', NULL si pas de service prsent (si 'ErrorHandling' a la bonne valeur).
	inline const char *Service(
		const char *HostService,
		err::handling__ ErrorHandling = err::h_Default )
	{
		const char *P;

		P = strchr( HostService, ':' );

		if ( P == NULL )
			if ( ErrorHandling == err::hThrowException )
				qRFwk();
			else
				return NULL;
		else
			return P + 1;

		return NULL;	// Pour viter un 'warnng'.
	}

	/*f Return a descriptor to a socket connected to 'Host' at 'Service'.
	'Host' can be in '212.95.72.3' format, or in 'www.epeios.org' format,
	and 'Service' can be in '80' format of in 'http' format.
	If 'Desc' != 'SCK_INVALID', the socket descriptor 'Desc' is used.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	socket__ Connect(
		const char *Host,
		const char *Service,
		socket__ Desc = SCK_INVALID_SOCKET,
		err::handling__ ErrorHandling = err::h_Default );

	/*f Return a descriptor to a socket connected to 'Host' at 'Service'.
	'Host' can be in '212.95.72.3' format, or in 'www.epeios.org' format,
	and 'Service' can be in '80' format of in 'http' format.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	inline socket__ Connect(
		const char *Host,
		const char *Service,
		err::handling__ ErrorHandling )
	{
		return Connect( Host, Service, SCK_INVALID_SOCKET, ErrorHandling );
	}

	/*f Return a descriptor to a socket connected to 'HostService'.
	'HostService' can be in '212.95.72.3:80' format, or in 'www.ensd.net:http' format.
	If 'Desc' != 'SCK_INVALID', the socket descriptor 'Desc' is used.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	inline socket__ Connect(
		const char *HostService,
		socket__ Desc = SCK_INVALID_SOCKET,
		err::handling__ ErrorHandling = err::h_Default )
	{
		buffer__ Buffer;

		return Connect( Host( HostService, Buffer ), Service( HostService ), Desc, ErrorHandling );
	} 

	/*f Return a descriptot to a socket connected to 'HostService'.
	'HostService' can be in '212.95.72.3:80' format, or in 'www.ensd.net:http' format.
	If connection failed, and 'ErrHandler' == 'err::hSkip', return 'SCK_INVALID'. */
	inline socket__ Connect(
		const char *HostService,
		err::handling__ ErrorHandling )
	{
		return Connect( HostService, SCK_INVALID_SOCKET, ErrorHandling );
	}

	typedef sck::socket_ioflow___ _flow___;

	class flow___
	: public _flow___
	{
	private:
		sck::socket__ Socket_;
	public:
		void reset( bso::bool__ P = true )
		{
			_flow___::reset( P );

			if ( P )
				if ( Socket_ != SCK_INVALID_SOCKET )
					sck::Close( Socket_ );

			Socket_ = SCK_INVALID_SOCKET;
		}
		E_CDTOR( flow___ );
		bso::bool__ Init(
			const char *Host,
			const char *Service,
			sck::duration__ Timeout,
			err::handling__ ErrorHandling )	// No default value, because a 'err::handling___' value is confused with a 'duration__'.
		{
			reset();

			Socket_ = Connect( Host, Service, ErrorHandling );

			if ( Socket_ != SCK_INVALID_SOCKET ) {
				_flow___::Init( Socket_, Timeout );
				return true;
			} else
				return false;
		}
		bso::bool__ Init(
			const char *HostService,
			sck::duration__ Timeout,
			err::handling__ ErrorHandling )	// No default value, because a 'err::handling___' value is confused with a 'duration__'.
		{
			reset();

			Socket_ = Connect( HostService, ErrorHandling );

			if ( Socket_ != SCK_INVALID_SOCKET ) {
				_flow___::Init( Socket_, Timeout );
				return true;
			} else
				return false;
		}

	};
}

/***************/
/***** NEW *****/
/***************/

namespace csdbnc {
	typedef flow___ rIOFlow;
}
#endif
