/*
	'clt' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'clt' header file ('clt.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define CLT__COMPILATION

#include "clt.h"

using namespace clt;

const char *clt::Host(
	const char *HostService,
	buffer__ Buffer )
{
	char *P;

	if ( strlen( HostService ) >= CLT_ADDRESS_SIZE_MAX )
		ERRl();

	strcpy( Buffer, HostService );

	P = strchr( Buffer, ':' );

	if ( P )
		*P = 0;

	return Buffer;
}

#ifdef CPE__BEOS
#	ifdef INADDR_NONE
#		error "Be OS include file have changed !"
#	else
#		define INADDR_NONE     (unsigned long)0xffffffff
#	endif
#endif

static inline bso::bool__ GetHost_(
	const char *Host,
	struct sockaddr_in &Nom )
{
	struct hostent *Hostent;
	bso::bool__ Return = true;

	if ( ( Hostent =(struct hostent *)gethostbyname( Host ) ) != NULL )
		memcpy((void *)&Nom.sin_addr,(void *)Hostent->h_addr,Hostent->h_length);
	else if ( ( Nom.sin_addr.s_addr = inet_addr( Host ) ) == INADDR_NONE )
		Return = false;

	return Return;
}

static inline bso::bool__ GetService_(
	const char *Service,
	struct sockaddr_in &Nom )
{
	struct servent *Servent;

	if ( ( Servent = getservbyname( Service, NULL ) ) != NULL )
		Nom.sin_port = (u_short)Servent->s_port;
	else
		Nom.sin_port = htons( (u_short)atoi( Service ) );

	return true;
}

socket__ clt::Connect(
	const char *Host,
	const char *Service,
	socket__ Desc,
	err::handle ErrHandle )
{
	struct sockaddr_in Nom;

	sck::Initialize();

	Nom.sin_family = AF_INET;

	if ( GetHost_( Host, Nom ) && GetService_( Service, Nom ) ) {
		if ( Desc == SCK_INVALID_SOCKET )
			Desc = CreateSocket( ErrHandle );

		if ( Desc != SCK_INVALID_SOCKET )
			if ( connect( Desc, (sockaddr *)&Nom, sizeof( Nom ) ) == SCK_SOCKET_ERROR )
				Desc = SCK_INVALID_SOCKET;

		if ( ( Desc == SCK_INVALID_SOCKET ) && ( ErrHandle == err::hUsual ) )
			ERRs();
	} else {
		Desc = SCK_INVALID_SOCKET;

		if ( ErrHandle == err::hUsual )
			ERRu();
	}

	return Desc;
}
