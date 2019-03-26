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

#define SCK__COMPILATION

#include "sck.h"

#ifdef CPE_S_POSIX
#	define SCK__IGNORE_SIGPIPE
#endif


#ifdef SCK__IGNORE_SIGPIPE
#	include <signal.h>
#endif

using namespace sck;

#ifdef CPE_S_WIN
bool sck::Ready_ = false;
#else
bool sck::Ready_ = true;
#endif

flw::size__ sck::Read(
	socket__ Socket,
	flw::size__ Amount,
	void *Buffer,
	duration__ Timeout )
{
	fd_set fds;
	int Result;

	FD_ZERO( &fds );
	FD_SET( Socket, &fds );

	if ( Timeout == SCK_INFINITE )
		Result = select( (int)( Socket + 1 ), &fds, NULL, NULL, NULL );
	else {
		timeval TV;

		TV.tv_sec = Timeout;
		TV.tv_usec = 0;

		Result = select( (int)( Socket + 1 ), &fds, NULL, NULL, &TV );
	}

	if ( Result == 1 ) {
		Result = recv( Socket, (cast__)Buffer, (int)Amount, 0 );

		if ( Result == SCK_SOCKET_ERROR ) {
			Result = SCK_DISCONNECTED;

			if ( Error() != SCK_ECONNRESET )
				qRLbr();
		} else if ( !Result && Amount )
			Result = SCK_DISCONNECTED;
	} else if ( Result == SCK_SOCKET_ERROR )
		qRLbr();
	else if ( Result == 0 )
		qRLbr();
	else
		qRSys();

	return Result;
}

flw::size__ sck::Write(
	socket__ Socket,
	const void *Buffer,
	flw::size__ Amount,
	duration__ Timeout )
{
	fd_set fds;
	int Result;

	FD_ZERO( &fds );
	FD_SET( Socket, &fds );

	if ( Timeout == SCK_INFINITE )
		Result = select( (int)( Socket + 1 ), NULL, &fds, NULL, NULL );
	else
	{
		timeval TV;

		TV.tv_sec = Timeout;
		TV.tv_usec = 0;

		Result = select( (int)( Socket + 1 ), NULL, &fds, NULL, &TV );
	}

	if ( Result == 1 )
	{
		Result = send( Socket, (const cast__)Buffer, (int)Amount, 0 );

		if ( Result == SCK_SOCKET_ERROR )
		{
			Result = SCK_DISCONNECTED;

			if ( Error() != SCK_ECONNRESET )
				qRLbr();
		}
		else if ( !Result && Amount )	// I assume that, because the send is call immediatly after the select, this can not be happen.
			qRFwk();
	}
	else if ( Result == SCK_SOCKET_ERROR )
		qRLbr();
	else if ( Result == 0 )
		qRLbr();
	else
		qRSys();

	return Result;
}

// Some errors are ignored.
bso::sBool sck::Close(
	socket__ Socket,
	qRPN )	// To set to 'qRPU' when called from destructors !
{
#ifdef SCK__WIN
	u_long Mode = 1;	// Non-blocking.
	bso::sBool Error = ioctlsocket( Socket, FIONBIO, &Mode ) != 0;

	Error |= shutdown( Socket, 2 ) != 0;	// '2' instead of 'SD_BOTH' because inlusion of 'winsock2.h' fails.

	if ( (closesocket( Socket ) != 0) || Error ) {
		if ( qRPT )
			qRLbr();

		return false;
	}

#elif defined( SCK__POSIX )
	u_long Mode = 1;	// Non-blocking.
	bso::sBool Error = ioctl( Socket, FIONBIO, &Mode ) != 0;

	Error |= shutdown( Socket, SHUT_RDWR ) != 0;

	//		while ( recv( Socket, (sCast_)Buffer, sizeof( Buffer ), MSG_DONTWAIT ) > 0 );
	if ( (close( Socket ) != 0) || Error ) {
		if ( qRPT )
			qRLbr();

		return false;
	}
#else
#	error
#endif
	return true;
}


Q37_GCTOR( sck )
{
#ifdef SCK__IGNORE_SIGPIPE
	signal( SIGPIPE, SIG_IGN );
#endif
}	
