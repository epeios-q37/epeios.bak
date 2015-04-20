/*
	'csm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'csm' header file ('csm.h').
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



//	$Id: csm.cpp,v 1.13 2012/11/14 16:06:26 csimon Exp $

#define CSM__COMPILATION

#include "csm.h"

class csmtutor
: public ttr_tutor
{
public:
	csmtutor( void )
	: ttr_tutor( CSM_NAME )
	{
#ifdef CSM_DBG
		Version = CSM_VERSION "\b\bD $";
#else
		Version = CSM_VERSION;
#endif
		Owner = CSM_OWNER;
		Date = "$Date: 2012/11/14 16:06:26 $";
	}
	virtual ~csmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "spp.h"
#include "mtk.h"

using namespace csm;


struct server_data__
{
	spp::shared_bipipe___ *Bipipe;
	csm::manager___ *Manager;
	mtx::mutex_handler__ Mutex;
	volatile bso::bool__ Error;
};

struct client_data__
{
	spp::shared_bipipe___ *Bipipe;
	csm::manager___ *Manager;
};

struct listener_data__ {
	srv::service__ Service;
	err::handle Handler;
	struct client_data__ Client;
	mtx::mutex_handler__ Mutex;
};

static void Client__(
	flw::ioflow___ &SocketFlow,
	client_data__ &CD )
{
ERRProlog
	void *PU;
	spp::master_shared_bipipe_ioflow___ Pipe;
ERRBegin
	Pipe.Init( *CD.Bipipe );

#ifdef CSM_DBG
	if ( CD.Manager == NULL )
		ERRc();
#endif

	PU = CD.Manager->CI( SocketFlow, Pipe );

	while( CD.Manager->CP( SocketFlow, Pipe, PU ) == bContinue ) {};
ERRErr
ERREnd
	CD.Manager->CE( PU );
ERREpilog
}

namespace {
	class internal_functions__
	: public srv::flow_functions__
	{
	protected:
		virtual void SRVProcess( flw::ioflow___ &SocketFlow )
		{
		ERRFProlog
		ERRFBegin
			Client__( SocketFlow, *ClientData );
		ERRFErr
		ERRFEnd
		ERRFEpilog
		}
	public:
		client_data__ *ClientData;
	};
}

static void Server_( void *P )
{
ERRFProlog
	server_data__ &SD = *(server_data__ *)P;
	void *PUS = NULL;
	spp::slave_shared_bipipe_ioflow___ Pipe;
ERRFBegin
	mtx::Lock( SD.Mutex );
	// Unlocked as soon as 'Listener()' managed to bind the socket.
	
	mtx::Unlock( SD.Mutex );
	mtx::Delete( SD.Mutex );
	
	if ( !SD.Error ) {
		SD.Mutex = MTX_INVALID_HANDLER;

		Pipe.Init( *SD.Bipipe );

	#ifdef CSM_DBG
		if ( SD.Manager == NULL )
			ERRc();
	#endif

		PUS = SD.Manager->SI();

		while ( SD.Manager->SP( Pipe, PUS ) == bContinue) {};
	} 
		
		
ERRFErr
ERRFEnd
	if ( SD.Error )
		SD.Error = false;
	else
		SD.Manager->SE( PUS );
ERRFEpilog
}

static bso::bool__ Listener_( void *P )
{
	bso::bool__ Success = true;
ERRProlog
	srv::server___ Server;
	listener_data__ &LD = *(listener_data__ *)P;
	internal_functions__ Functions;
	bso::bool__ BindSucceed = false;
ERRBegin
	Functions.ClientData = &LD.Client;

	if ( Server.Init( LD.Service, err::hSkip ) ) {
		mtx::Unlock( LD.Mutex );

		Server.Process( Functions, LD.Handler );
	} else
		Success = false;
ERRErr
ERREnd
ERREpilog
	return Success;
}


/* Lance le traitement en utilisaant pour fonction cliente
'void Client(sck_flo_socket_ &, void *PUS)' et pour fonction d'initialisation
de serveur 'void InitServeur( tub_tube_entre_sortie &, csm_tubes &, void *).
NE REND JAMAIS LA MAIN. */
void csm::manager___::Process(
	srv::service__ Service,
	err::handle Handle )
{
ERRProlog
	spp::shared_bipipe___ SharedBipipe;
	server_data__ SD = { NULL, NULL, MTX_INVALID_HANDLER, false };
	listener_data__ LD = {0, err::hUsual, { NULL, NULL }, MTX_INVALID_HANDLER };
ERRBegin
	SharedBipipe.Init();

	SD.Bipipe = &SharedBipipe;
	SD.Manager = this;

	LD.Handler = Handle;
	LD.Service = Service;
	LD.Client.Bipipe = &SharedBipipe;
	LD.Client.Manager = this;
	
	SD.Mutex = mtx::Create();
	LD.Mutex = SD.Mutex;
	
	mtx::Lock( SD.Mutex );	// Unlocked by 'Listener()'.

	mtk::Launch( Server_, &SD );

	if ( !Listener_( &LD ) ) {
		SD.Error = true;
		mtx::Unlock( SD.Mutex );
		while( SD.Error ) {};
		ERRs();
	}
		
	/* Ce qui est crit ci-dessous est faux. */

	// Sous Windows, on ne passe jamais par l.

	// Sous LINUX, on passe par l aprs dconnection du premier client.
	// DS sera donc dtruit; mais le dlai devrait tre suffisant pour que
	// les processus 'Serveur_' et 'Client_' ai le temps d'en copier le contenu ...
ERRErr
ERREnd
	if ( SD.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( SD.Mutex );
ERREpilog
}

#if 0
class direct
: public csm__manager
{
protected:
	virtual CSM_CP_T CSMCP( CSM_CP_P )
	{
		if ( PClient )
			return PClient( Client, Server, UP );
		else
			return csm__manager::CSMCP( Client, Server, UP );
	}
	virtual CSM_CI_T CSMCI( CSM_CI_P )
	{
		if ( IClient )
			return IClient( Client, Server );
		else
			return csm__manager::CSMCI( Client, Server );
	}
	virtual CSM_SP_T CSMSP( CSM_SP_P )
	{
		if ( PServer )
			return PServer( Client, UP );
		else
			return csm__manager::CSMSP( Client, UP );
	}
	virtual CSM_SI_T CSMSI( CSM_SI_P )
	{
		if ( IServer )
			return IServer();
		else
			return csm__manager::CSMSI();
	}
public:
	csm__client_processing PClient;
	csm__client_initialization IClient;
	csm__server_processing PServer;
	csm__server_initialization IServer;
};

void CSMProcess( 
		srv__port Port,
		csm__client_processing PClient,
		csm__client_initialization IClient,
		csm__server_processing PServer,
		csm__server_initialization IServer,
		err::handle Handle )
{
	direct Direct;

	Direct.PClient = PClient;
	Direct.IClient = IClient;
	Direct.PServer = PServer;
	Direct.IServer = IServer;

	Direct.Process( Port, Handle );
}
#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class csmpersonnalization
: public csmtutor
{
public:
	csmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static csmpersonnalization Tutor;

ttr_tutor &CSMTutor = Tutor;
