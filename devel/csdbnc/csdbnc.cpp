/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define CSDBNC_COMPILATION_

#include "csdbnc.h"

using namespace csdbnc;

const char *csdbnc::Host(
	const char *HostService,
	buffer__ Buffer,
	qRPN )	// The error parameter is not used ; only here for standardization.
{
	char *P;

	if ( Buffer == NULL )
    qRFwk();

	if ( strlen( HostService ) >= CSDBNC_ADDRESS_SIZE_MAX )
		qRLmt();

	strcpy( Buffer, HostService );

	P = strchr( Buffer, ':' );

	if ( P )
		*P = 0;

	return Buffer;
}

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

socket__ csdbnc::Connect(
	const char *Host,
	const char *Service,
	socket__ Desc,
	err::handling__ ErrorHandling )
{
	struct sockaddr_in Nom;

	sck::Initialize();

	Nom.sin_family = AF_INET;

	if ( GetHost_( Host, Nom ) && GetService_( Service, Nom ) ) {
		if ( Desc == SCK_INVALID_SOCKET )
			Desc = CreateSocket( ErrorHandling );

		if ( Desc != SCK_INVALID_SOCKET )
			if ( connect( Desc, (sockaddr *)&Nom, sizeof( Nom ) ) == SCK_SOCKET_ERROR )
				Desc = SCK_INVALID_SOCKET;

		if ( ( Desc == SCK_INVALID_SOCKET ) && ( ErrorHandling == err::hThrowException ) )
			qRSys();
	} else {
		Desc = SCK_INVALID_SOCKET;

		if ( ErrorHandling == err::hThrowException )
			qRFwk();
	}

	return Desc;
}

