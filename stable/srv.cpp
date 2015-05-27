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

#define SRV__COMPILATION

#include "srv.h"

using namespace srv;

#ifdef CPE__T_MT
#include "mtx.h"
#include "mtk.h"
#endif

bso::bool__ srv::listener___::Init(
	service__ Service,
	int Amount,
	err::handle ErrHandle)
{
	sockaddr_in nom;

	sck::Initialize();

	reset();

	Socket_ = CreateSocket();

	memset((char *)&nom,0,sizeof(nom));
	nom.sin_port=htons( Service );
	nom.sin_addr.s_addr=INADDR_ANY;
	nom.sin_family=AF_INET;

#ifdef CPE__UNIX
	int Val = ~0;

	if ( setsockopt( Socket_, SOL_SOCKET, SO_REUSEADDR, &Val, sizeof( Val ) ) != 
0 )
		ERRs();
#endif

	if( bind( Socket_, (struct sockaddr*)(&nom), sizeof(sockaddr_in) ) )
		if ( ErrHandle == err::hUsual )
			ERRs();
		else
			return false;

	if ( listen( Socket_, Amount ) )
		ERRs();
		
	return true;
}

socket__ srv::listener___::Interroger_( err::handle ErrHandle )
{
	fd_set fds;
	int Reponse;
	bso::bool__ Boucler = true;
	socket__ Socket;

	while( Boucler )
	{
ERRProlog
		Socket = SCK_INVALID_SOCKET;
ERRBegin
		Boucler = false;
		FD_ZERO( &fds );
		FD_SET( Socket_, &fds );

		Reponse = select( (int)( Socket_ + 1 ), &fds, 0, 0, NULL );

		if ( Reponse == SCK_SOCKET_ERROR )
			ERRs();
		else if ( Reponse > 0 )
		{
			if ( ( Socket = accept( Socket_, NULL, NULL ) ) == SCK_INVALID_SOCKET )
				if ( Error() != SCK_EWOULDBLOCK )
					ERRs();
		}
ERRErr
	if ( ErrHandle == err::hSkip )
	{
		ERRRst();
		Boucler = true;
	}
	else if ( ErrHandle != err::hUsual )
		ERRu();
ERREnd
ERREpilog
	}

	return Socket;
}

void srv::listener___::Process(
	socket_functions__ &Functions,
	err::handle ErrHandle )
{
ERRProlog
	void *UP = NULL;
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	action__ Action = a_Undefined;
ERRBegin
	Socket = Interroger_( ErrHandle );

	UP = Functions.PreProcess( Socket );

	while ( ( Action = Functions.Process( Socket, UP ) ) == aContinue );

	switch( Action ) {
	case aContinue:
		ERRc();
		break;
	case aStop:
		break;
	default:
		ERRu();
		break;
	}

ERRErr
ERREnd
	if ( UP != NULL )
		Functions.PostProcess( UP );
ERREpilog
}

#ifdef CPE__T_MT
struct socket_data__
{
	socket_functions__ *Functions;
	sck::socket__ Socket;
	mtx::mutex_handler__ Mutex;
};

static void Traiter_( void *PU )
{
	::socket_data__ &Data = *(::socket_data__ *)PU;
ERRFProlog
	bso::bool__ Close = true;
	socket_functions__ &Functions = *Data.Functions;
	socket__ Socket = Data.Socket;
	void *UP = NULL;
	action__ Action = a_Undefined;
ERRFBegin
	mtx::Unlock( Data.Mutex );

	ERRProlog
	ERRBegin
		UP = Functions.PreProcess( Socket );

		while ( ( Action = Functions.Process( Socket, UP ) ) == aContinue );
	ERRErr
	ERREnd
		if ( UP != NULL )
			Functions.PostProcess( UP );
	ERREpilog
ERRFErr
ERRFEnd
ERRFEpilog
}

void server___::Process(
	socket_functions__ &Functions,
	err::handle ErrHandle )
{
ERRProlog
	sck::socket__ Socket = SCK_INVALID_SOCKET;
	::socket_data__ Data = {NULL, SCK_INVALID_SOCKET, MTX_INVALID_HANDLER};
ERRBegin

	Socket = listener___::GetConnection( ErrHandle );

	Data.Functions = &Functions;
	Data.Mutex = mtx::Create();

	mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

	Data.Socket = Socket;


	for(;;)
	{
		mtk::Launch( Traiter_, &Data );

//		SCKClose( Socket );	// Only needed when using processes.

		Socket = SCK_INVALID_SOCKET;

		Socket = listener___::GetConnection( ErrHandle );

		mtx::Lock( Data.Mutex );	// Unlocked by the 'Traiter_()' function.

		Data.Socket = Socket;
	}
ERRErr
ERREnd
	if ( Socket != SCK_INVALID_SOCKET )
		sck::Close( Socket );

	if ( Data.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Data.Mutex );
ERREpilog
}


namespace {
	struct flow_data__ {
		sck::socket_ioflow___ Flow;
		void *UP;
	};

	class internal_functions__
	: public socket_functions__
	{
	protected:
		virtual void *SRVPreProcess( socket__ Socket )
		{
			flow_data__ *Data = NULL;
		ERRProlog
		ERRBegin
			Data = new flow_data__;

			if ( Data == NULL )
				ERRa();

			Data->Flow.Init( Socket );
			Data->UP = Functions->PreProcess( Data->Flow );
		ERRErr
			if ( Data != NULL )
				delete Data;
			Data = NULL;
		ERREnd
		ERREpilog
			return Data;
		}
		virtual action__ SRVProcess(
			socket__ Socket,
			void *UP )
		{
			flow_data__ &Data = *(flow_data__ *)UP;
#ifdef SRV_DBG
			if ( Data.Flow.GetSocket() != Socket )
				ERRc();
#endif

			return Functions->Process( Data.Flow, Data.UP );
		}
		virtual void SRVPostProcess( void *UP )
		{
			if ( UP == NULL )
				ERRc();

			delete (flow_data__ *)UP;
		}
	public:
		flow_functions__ *Functions;
		sck::duration__ TimeOut;
	};
}

void server___::Process(
	flow_functions__ &Functions,
	sck::duration__ TimeOut,
	err::handle ErrHandle )
{
	internal_functions__ F;

	F.Functions = &Functions;
	F.TimeOut = TimeOut;

	Process( F, ErrHandle );
}


#endif

