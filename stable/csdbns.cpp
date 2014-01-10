/*
	'csdbns.cpp' by Claude SIMON (http://zeusw.org/).

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

#define CSDBNS__COMPILATION

#include "csdbns.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace csdbns;

#ifdef CPE_MT
# include "mtx.h"
# include "mtk.h"
# include "lstbch.h"
#endif

#ifdef CPE_CONSOLE
#	include "cio.h"
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

#if defined( CPE_LINUX ) || defined( CPE_CYGWIN )
	int Val = ~0;

	if ( setsockopt( Socket_, SOL_SOCKET, SO_REUSEADDR, &Val, sizeof( Val ) ) != 
0 )
		ERRSys();
#endif

	if( bind( Socket_, (struct sockaddr*)(&nom), sizeof(sockaddr_in) ) )
		if ( ErrorHandling == err::hThrowException )
			ERRSys();
		else
			return false;

	if ( listen( Socket_, Amount ) )
		ERRSys();
		
	return true;
}

socket__ csdbns::listener___::_Interroger(
	err::handling__ ErrorHandling,
	sck::duration__ TimeOut,
	const char *&IP )
{
	fd_set fds;
	int Reponse;
	bso::bool__ Boucler = true;
	socket__ Socket;

	while( Boucler )
	{
ERRProlog
	Socket = SCK_INVALID_SOCKET;
	timeval TimeOutStruct;
	struct sockaddr_in SockAddr;
#ifdef SCK__WIN
    int
#elif defined( SCK__POSIX )
	socklen_t
#else
# error
#endif
	SockAddrSize = sizeof( SockAddr );
ERRBegin
		Boucler = false;
		FD_ZERO( &fds );
		FD_SET( Socket_, &fds );

		TimeOutStruct.tv_sec = TimeOut / 1000;
		TimeOutStruct.tv_usec = ( (bso::u32__)TimeOut % 1000UL ) * 1000;

		Reponse = select( (int)( Socket_ + 1 ), &fds, 0, 0, TimeOut != SCK_INFINITE ? &TimeOutStruct : NULL );

		if ( Reponse == SCK_SOCKET_ERROR )
			ERRSys();
		else if ( Reponse > 0 )
		{
			if ( ( Socket = accept( Socket_, (sockaddr *)&SockAddr, &SockAddrSize ) ) == SCK_INVALID_SOCKET ) {
				error__ Error = sck::Error();
//#ifdef CPE_CONSOLE
#if 0
				ERRProlog
					cio::cerr___ cerr;
					tol::buffer__ Buffer;
				ERRBegin
					cerr << tol::DateAndTime( Buffer ) << " (" << __FILE__ << ", " << (bso::ulong__)__LINE__  << ") : ("  << (bso::ulong__)Error << ") " << sck::ErrorDescription( Error ) << txf::nl << txf::commit;
				ERRErr
				ERREnd
				ERREpilog
#endif
				if ( ( Error != SCK_EWOULDBLOCK ) && ( Error != SCK_EAGAIN ) )
					ERRSys();
			}

			IP = inet_ntoa( SockAddr.sin_addr );

		}
ERRErr
	if ( ErrorHandling == err::hUserDefined )
	{
		ERRRst();
		Boucler = true;
	} else if ( ErrorHandling != err::hUserDefined )
		ERRPrm();
ERREnd
ERREpilog
	}

	return Socket;
}

bso::bool__ csdbns::listener___::Process(
	socket_callback__ &Callback,
	err::handling__ ErrorHandling,
	sck::duration__ TimeOut )
{
	bso::bool__ Continue = true;
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	action__ Action = a_Undefined;
	const char *UP = NULL;
ERRBegin
	Socket = _Interroger( ErrorHandling, TimeOut, UP );

	if ( Socket != SCK_INVALID_SOCKET ) {

			/* Bien que '_UP' et 'Functions' ne soient utilisés que dans cette fonction,
			ils sont stockés dans l'objet même, pour être disponible lors de l'appel du destructeur.
			On peut alors appeler le 'PostProcess' de l'utilisateur, lui permettant de détruire
			correctement l'objet qu'il a crée (référencé par 'UP'), même lors d'un ^C,
			qui nous éjecte directement de cette fonction, mais provoque quand même un appel du destructeur. */

		_UP = Callback.PreProcess( Socket, UP );
		_Callback = &Callback;

		while ( ( Action = Callback.Process( Socket, _UP ) ) == aContinue );

		switch( Action ) {
		case aContinue:
			ERRFwk();
			break;
		case aStop:
			break;
		default:
			ERRFwk();
			break;
		}

		Continue = false;
	}

ERRErr
ERREnd
	if ( _CallbackAvailable() ) {
		Callback.PostProcess( _UP );
		_UP = NULL;
		_Callback = NULL;	// Pour empêcher un autre appel au 'PostProcess' lors de l'appel du destructeur.
	}

ERREpilog
	return Continue;
}

#ifdef CPE_MT

struct socket_data__
{
	socket_callback__ *Callback;
	sck::socket__ Socket;
	const char *IP;
	mtx::handler__ Mutex;
};

/* Les objets et fonctions qui suivent sont pour permettre aux objets utilisateurs d'être
détruits correctement même en cas de ^C */

struct csdbns_repository_item__ {
	socket_callback__ *Callback;
	void *UP;
};

E_ROW( rrow__ );

static lstbch::E_LBUNCHt( csdbns_repository_item__, rrow__ ) Repository_;

mtx::handler__ Mutex_ = MTX_INVALID_HANDLER;

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
	rrow__ Row = E_NIL;

	Lock_();

	Row = Repository_.New();

	Repository_.Store( Item, Row );

	Unlock_();

	return Row;
}

inline static void UnsafeClean_( rrow__ Row )
{
	csdbns_repository_item__ Item;

	Repository_.Recall( Row, Item );

	Item.Callback->PostProcess( Item.UP );

	Repository_.Delete( Row );
}

inline static void Clean_( rrow__ Row )
{
	Lock_();

	UnsafeClean_( Row );

	Unlock_();
}

inline static void Clean_( void )
{
	Lock_();

	rrow__ Row = Repository_.First();

	while ( Row != E_NIL ) {
		UnsafeClean_( Row );

		Row = Repository_.Next( Row );
	}

	Repository_.Init();

	Unlock_();
}

static void Traiter_( void *PU )
{
	::socket_data__ &Data = *(::socket_data__ *)PU;
ERRFProlog
	bso::bool__ Close = true;
	socket_callback__ &Callback = *Data.Callback;
	socket__ Socket = Data.Socket;
	void *UP = NULL;
	action__ Action = a_Undefined;
	rrow__ Row = E_NIL;
	csdbns_repository_item__ Item;
	tol::E_FPOINTER___( char ) Buffer;
ERRFBegin
	if ( ( Buffer = malloc( strlen( Data.IP ) + 1 ) ) == NULL )
		ERRAlc();

	strcpy( Buffer, Data.IP );
	mtx::Unlock( Data.Mutex );

	ERRProlog
	ERRBegin
		UP = Callback.PreProcess( Socket, Buffer );

		Item.Callback = &Callback;
		Item.UP = UP;

		Row = New_( Item );

		while ( ( Action = Callback.Process( Socket, UP ) ) == aContinue );
	ERRErr
	ERREnd
		if ( Row != E_NIL )
			Clean_( Row );
	ERREpilog
ERRFErr
ERRFEnd
ERRFEpilog
}

void server___::Process(
	sck::duration__ TimeOut,
	err::handling__ ErrorHandling )
{
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	const char *IP = NULL;
	::socket_data__ Data = {NULL, SCK_INVALID_SOCKET, NULL, MTX_INVALID_HANDLER};
	bso::bool__ Continue = true;
ERRBegin

	Socket = listener___::GetConnection( IP, ErrorHandling, TimeOut );

	if ( Socket != SCK_INVALID_SOCKET ) {

		Data.Callback = _SocketCallback;
		Data.Mutex = mtx::Create();

		mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

		Data.Socket = Socket;
		Data.IP = IP;
	} else
		Continue = false;

	while ( Continue ) {
		mtk::Launch( Traiter_, &Data );

//		SCKClose( Socket );	// Only needed when using processes.

		Socket = SCK_INVALID_SOCKET;

		Socket = listener___::GetConnection( IP, ErrorHandling, TimeOut );

		if ( Socket != SCK_INVALID_SOCKET ) {
			mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

			Data.Socket = Socket;
			Data.IP = IP;
		} else
			Continue = false;
	}
ERRErr
ERREnd
	if ( Socket != SCK_INVALID_SOCKET )
		sck::Close( Socket );

	if ( Data.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Data.Mutex );
ERREpilog
}

#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdbnspersonnalization
{
public:
	csdbnspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
#ifdef CPE_MT
		Repository_.reset( false );
		Repository_.Init();
		Mutex_ = mtx::Create();
#endif
}
	~csdbnspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
#ifdef CPE_MT
		Clean_();

		if ( Mutex_ != MTX_INVALID_HANDLER )
			mtx::Delete( Mutex_ );
#endif
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static csdbnspersonnalization Tutor;
