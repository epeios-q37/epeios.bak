/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

# include "dmopool.h"

using namespace dmopool;

#include "prtcl.h"

#include "registry.h"

#include "bch.h"
#include "crt.h"
#include "csdbns.h"
#include "flx.h"
#include "mtk.h"
#include "sclmisc.h"
#include "str.h"


namespace {
	class rConnections_
	{
	public:
		sck::sSocket Socket;
		tht::rReadWrite Access;
		bso::sBool GiveUp;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Socket != sck::Undefined )
					sck::Close( Socket );
			}

			Socket = sck::Undefined;
			Access.reset( P );
			GiveUp = false;	// If at 'true', the client is deemed to be disconnected.
		}
		qCDTOR( rConnections_ );
		void Init( void )
		{
			reset();

			Access.Init();
			GiveUp = false;
		}
	};

	mtx::rHandler MutexHandler_ = mtx::Undefined;
	qROW( Row );
	crt::qMCRATEw( str::dString, sRow ) Tokens_;
	bch::qBUNCHw( rConnections_ *, sRow ) Clients_;
	csdbns::rListener Listener_;

	rConnections_ *TUSearch_( const str::dString &Token )
	{
		if ( !mtx::IsLocked( MutexHandler_ ) )
			qRGnr();

		sRow Row = Tokens_.First();

		while ( ( Row != qNIL ) && (Tokens_( Row ) != Token) )
			Row = Tokens_.Next( Row );

		if ( Row != qNIL )
			return Clients_( Row );
		else
			return NULL;
	}

	rConnections_ *TSSearch_( const str::dString &Token )
	{
		rConnections_ *Connections = NULL;
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.InitAndLock( MutexHandler_ );

		Connections = TUSearch_( Token );
	qRR;
	qRT;
	qRE;
		return Connections;
	}

	rConnections_ *Create_( const str::dString &Token )
	{
		rConnections_*Connections = NULL;
	qRH;
		mtx::rMutex Mutex;
		sRow Row = qNIL;
	qRB;
		Mutex.InitAndLock( MutexHandler_) ;

		if ( TUSearch_( Token ) != NULL )
			qRGnr();

		Row = Tokens_.Append( Token );

		if ( (Connections = new rConnections_) == NULL )
			qRAlc();

		Connections->Init();

		if ( Clients_.Append( Connections ) != Row )
			qRGnr();
	qRR;
		if ( Connections != NULL )
			delete Connections;
	qRT;
	qRE;
		return Connections;
	}

	void Get_(
		flw::sRFlow &Flow,
		str::dString &String )
	{
		prtcl::Get( Flow, String );
	}

	void Put_(
		const str::dString &String,
		flw::sWFlow &Flow )
	{
		prtcl::Put( String, Flow );
	}

	void NewConnexionRoutine_(
		void *UP,
		mtk::gBlocker &Blocker )
	{
	qRFH;
		sck::sSocket Socket = *(sck::sSocket *)UP;
		str::wString Token;
		sck::rRWFlow Flow;
		tol::bUUID UUID;
		rConnections_ *Connections = NULL;
		mtx::rMutex Mutex;
	qRFB;
		Blocker.Release();

		Flow.Init( Socket, false, sck::NoTimeout );

		Token.Init();
		Get_( Flow, Token );

		if ( Token.Amount() == 0 ) {
			Token.Append( tol::UUIDGen( UUID ) );

			Connections = Create_( Token );
		} else {
			Connections = TSSearch_( Token );
		}

		if ( Connections == NULL )
			Token.Init();
		else {
			Connections->Access.WriteBegin();
			Connections->Socket = Socket;
			Connections->Access.WriteEnd();
		}

		Put_( Token, Flow );
	qRFR;
	qRFT;
	qRFE( sclmisc::ErrFinal() );
	}

	void ListeningRoutine_( void * )
	{
	qRFH;
		sck::sSocket Socket = sck::Undefined;
		const char *IP;
	qRFB;
		while ( true ) {
			Socket = sck::Undefined;

			Socket = Listener_.GetConnection( IP );

			mtk::Launch( NewConnexionRoutine_, &Socket );
		}
	qRFR;
	qRFT;
	qRFE( sclmisc::ErrFinal() );
	}
}

void dmopool::Initialize( void )
{
qRH;
	csdbns::sService Service = csdbns::Undefined;
qRB;
	if ( (Service = sclmisc::OGetU16( registry::parameter::DemoService, csdbns::Undefined ) ) != csdbns::Undefined ) {
		Listener_.Init( Service );

		mtk::RawLaunch( ListeningRoutine_, NULL );
	}
qRR;
qRT;
qRE;
}

sck::sSocket dmopool::GetConnection( const str::dString &Token )
{
	sck::sSocket Socket = sck::Undefined;
	rConnections_ *Connections = TSSearch_( Token );

	if ( Connections == NULL )
		qRGnr();

	if ( !Connections->Access.ReadBegin( 1000 ) ) {	// Give 1 second to the client to respond.
		Connections->GiveUp = true;				// No available connections within 1 second, tells other to give up.
		Connections->Access.WriteDismiss();		// The 'ReadBegin()' from another thread will now succeed.
		qRGnr();
	}

	if ( Connections->GiveUp ) {	// 'ReadBegin()' succeeded, but we have were instructed to give up.
		Connections->Access.ReadEnd();	// For the following 'WriteDismiss()' to succeed.
		Connections->Access.WriteDismiss();		// The 'ReadBegin()' from another thread will now succeed.
		qRGnr();
	}

	Socket = Connections->Socket;
	Connections->Access.ReadEnd();

	return Socket;
}

qGCTOR( dmopool )
{
	MutexHandler_ = mtx::Create();
	Tokens_.Init();
	Clients_.Init();
}

qGDTOR( dmopool )
{
	if ( MutexHandler_ != mtx::Undefined )
		mtx::Delete( MutexHandler_, true );

	sRow Row = Clients_.First();

	while ( Row != qNIL ) {
		delete Clients_( Row );

		Row = Clients_.Next( Row );
	}
}

