/*
	Header for the 'csdbnc' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: csdbnc.h,v 1.14 2013/04/09 17:57:27 csimon Exp $

#ifndef CSDBNC__INC
#define CSDBNC__INC

#define CSDBNC_NAME		"CSDBNC"

#define	CSDBNC_VERSION	"$Revision: 1.14 $"

#define CSDBNC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CSDBNC_NODBG )
#define CSDBNC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.14 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:27 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Base Network Client 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "sck.h"

#if defined( CPE_POSIX )
#	define CSDBNC__POSIX
#elif defined( CPE_WIN )
#	define CSDBNC__WIN
#else
#	error "Unknown target !"
#endif

#if defined( CSDBNC__POSIX )
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h>
#elif !defined( CSDBNC__WIN )
#	error "Unknow compiler enviroment"
#endif

#define CSDBNC_ADDRESS_SIZE_MAX	100

namespace csdbnc {
	using namespace sck;

	typedef bso::char__	buffer__[CSDBNC_ADDRESS_SIZE_MAX+1];

	//f Return the host name contained in 'HostService'.
	const char *Host(
		const char *HostService,
		buffer__ Buffer );

#ifndef CPE__MT
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
				ERRFwk();
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
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
