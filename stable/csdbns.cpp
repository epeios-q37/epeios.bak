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

#define CSDBNS__COMPILATION

#include "csdbns.h"

using namespace csdbns;

#include "cio.h"


#ifdef CPE_F_MT
# include "mtx.h"
# include "mtk.h"
# include "lstbch.h"
#endif


bso::bool__ csdbns::listener___::Init(
	port__ Port,
	int Amount,
	err::handling__ ErrorHandling )
{
	sockaddr_in nom;

	sck::Initialize();

	reset();

	Socket_ = CreateSocket();

	memset((char *)&nom,0,sizeof(nom));
	nom.sin_port=htons( Port );
	nom.sin_addr.s_addr=INADDR_ANY;
	nom.sin_family=AF_INET;

#if defined( CPE_S_POSIX )
	int Val = ~0;

	if ( setsockopt( Socket_, SOL_SOCKET, SO_REUSEADDR, &Val, sizeof( Val ) ) != 
0 )
		qRSys();
#endif

	if ( bind(Socket_, (struct sockaddr*)(&nom), sizeof(sockaddr_in)) ) {
		if ( ErrorHandling == err::hThrowException )
			qRSys();
		else
			return false;
	}

	if ( listen( Socket_, Amount ) )
		qRSys();
		
	return true;
}

socket__ csdbns::listener___::_Interroger(
	err::handling__ ErrorHandling,
	sck::duration__ Timeout,
	const char *&IP )
{
	fd_set fds;
	int Reponse;
	bso::bool__ Boucler = true;
	socket__ Socket;

	while( Boucler )
	{
qRH
	Socket = SCK_INVALID_SOCKET;
	timeval TimeoutStruct;
	struct sockaddr_in SockAddr;
#ifdef SCK__WIN
    int
#elif defined( SCK__POSIX )
	socklen_t
#else
# error
#endif
	SockAddrSize = sizeof( SockAddr );
qRB
		Boucler = false;
		FD_ZERO( &fds );
		FD_SET( Socket_, &fds );

		TimeoutStruct.tv_sec = Timeout / 1000;
		TimeoutStruct.tv_usec = ( (bso::u32__)Timeout % 1000UL ) * 1000;

		Reponse = select( (int)( Socket_ + 1 ), &fds, 0, 0, Timeout != SCK_INFINITE ? &TimeoutStruct : NULL );

		if ( Reponse == SCK_SOCKET_ERROR )
			qRSys();
		else if ( Reponse > 0 )
		{
			if ( ( Socket = accept( Socket_, (sockaddr *)&SockAddr, &SockAddrSize ) ) == SCK_INVALID_SOCKET ) {
				error__ Error = sck::Error();
//#ifdef CPE_F_CONSOLE
#if 0
				qRH
					cio::cerr___ cerr;
					tol::buffer__ Buffer;
				qRB
					cerr << tol::DateAndTime( Buffer ) << " (" << __FILE__ << ", " << (bso::ulong__)__LINE__  << ") : ("  << (bso::ulong__)Error << ") " << sck::ErrorDescription( Error ) << txf::nl << txf::commit;
				qRR
				qRT
				qRE
#endif
				if ( ( Error != SCK_EWOULDBLOCK ) && ( Error != SCK_EAGAIN ) )
					qRSys();
			}

			IP = inet_ntoa( SockAddr.sin_addr );

		}
qRR
	if ( ErrorHandling == err::hUserDefined )
	{
		ERRRst();
		Boucler = true;
	} else if ( ErrorHandling != err::hUserDefined )
		qRFwk();
qRT
qRE
	}

	return Socket;
}

bso::bool__ csdbns::listener___::Process(
	socket_callback__ &Callback,
	err::handling__ ErrorHandling,
	sck::duration__ Timeout )
{
	bso::bool__ Continue = true;
qRH
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	action__ Action = a_Undefined;
	const char *UP = NULL;
	bso::sBool CloseSocket = false;
qRB
	Socket = _Interroger( ErrorHandling, Timeout, UP );

	if ( Socket != SCK_INVALID_SOCKET ) {

			/* Bien que '_UP' et 'Functions' ne soient utiliss que dans cette fonction,
			ils sont stocks dans l'objet mme, pour tre disponible lors de l'appel du destructeur.
			On peut alors appeler le 'PostProcess' de l'utilisateur, lui permettant de dtruire
			correctement l'objet qu'il a cre (rfrenc par 'UP'), mme lors d'un ^C,
			qui nous jecte directement de cette fonction, mais provoque quand mme un appel du destructeur. */

		_UP = Callback.PreProcess( Socket, UP );
		_Callback = &Callback;

		while ( ( Action = Callback.Process( Socket, _UP ) ) == aContinue );

		switch( Action ) {
		case aContinue:
			qRFwk();
			break;
		case aStop:
			break;
		default:
			qRFwk();
			break;
		}

		Continue = false;
	}

qRR
qRT
	if ( _CallbackAvailable() ) {
		CloseSocket = Callback.PostProcess( _UP );
		_UP = NULL;
		_Callback = NULL;	// Pour empcher un autre appel au 'PostProcess' lors de l'appel du destructeur.
	}

	if ( Socket != SCK_INVALID_SOCKET )
		if ( CloseSocket )
			sck::Close( Socket );
qRE
	return Continue;
}

#ifdef CPE_F_MT

struct socket_data__
{
	socket_callback__ *Callback;
	sck::socket__ Socket;
	const char *IP;
	mtx::handler___ Mutex;
};

/* Les objets et fonctions qui suivent sont pour permettre aux objets utilisateurs d'tre
dtruits correctement mme en cas de ^C */

struct csdbns_repository_item__ {
	socket_callback__ *Callback;
	void *UP;
};

E_ROW( rrow__ );

static lstbch::E_LBUNCHt( csdbns_repository_item__, rrow__ ) Repository_;

mtx::handler___ Mutex_ = mtx::Undefined;

inline static void Lock_( void )
{
	mtx::Lock( Mutex_ );
}

inline static void Unlock_( void )
{
	mtx::Unlock( Mutex_ );
}


inline static rrow__ New_( const csdbns_repository_item__ &Item )
{
	rrow__ Row = qNIL;

	Lock_();

	Row = Repository_.New();

	Repository_.Store( Item, Row );

	Unlock_();

	return Row;
}

inline static bso::sBool UnsafeClean_( rrow__ Row )
{
	bso::sBool Returning = false;
	csdbns_repository_item__ Item;

	Repository_.Recall( Row, Item );

	Returning = Item.Callback->PostProcess( Item.UP );

	Repository_.Delete( Row );

	return Returning;
}

inline static bso::sBool Clean_( rrow__ Row )
{
	bso::sBool Returning = false;

	Lock_();

	Returning = UnsafeClean_( Row );

	Unlock_();

	return Returning;
}

inline static void Clean_( void )
{
	Lock_();

	rrow__ Row = Repository_.First();

	while ( Row != qNIL ) {
		UnsafeClean_( Row );

		Row = Repository_.Next( Row );
	}

	Repository_.Init();

	Unlock_();
}

static void ErrFinal_( void )
{

	if ( ERRType != err::t_Abort ) {
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		qRH
		qRB
			if ( cio::IsInitialized() ) {
				if ( cio::Target() == cio::tConsole ) {
					cio::COut << txf::commit;
					cio::CErr << txf::nl << txf::tab;
				}

				cio::CErr << "{ " << Message << " }";

				if ( cio::Target() == cio::tConsole )
					cio::CErr << txf::nl;

				cio::CErr << txf::commit;
			} else
				qRFwk();
		qRR
		qRT
		qRE
	} else
		ERRRst();
}

static void Traiter_( void *PU )
{
	::socket_data__ &Data = *(::socket_data__ *)PU;
qRFH
	socket_callback__ &Callback = *Data.Callback;
	socket__ Socket = Data.Socket;
	void *UP = NULL;
	action__ Action = a_Undefined;
	rrow__ Row = qNIL;
	csdbns_repository_item__ Item;
	tol::E_FPOINTER___( char ) Buffer;
qRFB
	if ( ( Buffer = malloc( strlen( Data.IP ) + 1 ) ) == NULL )
		qRAlc();

	strcpy( Buffer, Data.IP );
	mtx::Unlock( Data.Mutex );

	qRH
	qRB
		UP = Callback.PreProcess( Socket, Buffer );

		Item.Callback = &Callback;
		Item.UP = UP;

		Row = New_( Item );

		while ( ( Action = Callback.Process( Socket, UP ) ) == aContinue );
	qRR
	qRT
		if ( Row != qNIL )
			if ( Clean_( Row ) )
				sck::Close( Socket );
	qRE
qRFR
qRFT
qRFE( ErrFinal_() )
}

void server___::Process(
	const bso::sBool *Freeze,
	sck::duration__ Timeout,
	err::handling__ ErrorHandling )
{
qRH
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	const char *IP = NULL;
	::socket_data__ Data = {NULL, SCK_INVALID_SOCKET, NULL, MTX_INVALID_HANDLER};
	bso::bool__ Continue = true;
qRB
	Socket = listener___::GetConnection( IP, ErrorHandling, Timeout );

	if ( Socket != SCK_INVALID_SOCKET ) {

		Data.Callback = _SocketCallback;
		Data.Mutex = mtx::Create();

		mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

		Data.Socket = Socket;
		Data.IP = IP;
	} else
		Continue = false;

	while ( Continue ) {
		mtk::RawLaunch( Traiter_, &Data );

//		SCKClose( Socket );	// Only needed when using processes.

		Socket = SCK_INVALID_SOCKET;

		Socket = listener___::GetConnection( IP, ErrorHandling, Timeout );

		if ( Freeze != NULL )
			while ( *Freeze )
				tht::Defer( 100 );

		if ( Socket != SCK_INVALID_SOCKET ) {
			mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

			Data.Socket = Socket;
			Data.IP = IP;
		} else
			Continue = false;
	}
qRR
qRT
	if ( Socket != SCK_INVALID_SOCKET )
		sck::Close( Socket );

	if ( Data.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Data.Mutex );
qRE
}

#endif

Q37_GCTOR( csdbns )
{
#ifdef CPE_F_MT
	Repository_.reset( false );
	Repository_.Init();
	Mutex_ = mtx::Create();
#endif
}

Q37_GDTOR( csdbns )
{
#ifdef CPE_F_MT
	Clean_();

	if ( Mutex_ != MTX_INVALID_HANDLER )
		mtx::Delete( Mutex_ );
#endif
}
