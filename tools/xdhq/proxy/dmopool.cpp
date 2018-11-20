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
#include "logq.h"
#include "mtk.h"
#include "sclmisc.h"
#include "str.h"


namespace {
	class rClient_
	{
	public:
		sck::sSocket Socket;
		tht::rReadWrite Access;
		bso::sBool GiveUp;
		str::wString IP;
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
		qCDTOR( rClient_ );
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
	crt::qMCRATEw( str::dString, sRow ) Heads_;
	bch::qBUNCHw( rClient_ *, sRow ) Clients_;
	csdbns::rListener Listener_;

	sRow TUSearch_( const str::dString &Token )
	{
		if ( !mtx::IsLocked( MutexHandler_ ) )
			qRGnr();

		sRow Row = Tokens_.First();

		while ( (Row != qNIL) && ( Tokens_( Row ) != Token) )
			Row = Tokens_.Next( Row );

		return Row;
	}

	rClient_ *TUClientSearch_( const str::dString &Token )
	{
		sRow Row = TUSearch_( Token );

		if ( Row != qNIL )
			return Clients_( Row );
		else
			return NULL;
	}

	rClient_ *TSClientSearch_( const str::dString &Token )
	{
		rClient_ *Client = NULL;
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.InitAndLock( MutexHandler_ );

		Client = TUClientSearch_( Token );
	qRR;
	qRT;
	qRE;
		return Client;
	}

	const str::dString &TUHeadSearch_(
		const str::dString &Token,
		str::dString &Head )
	{
		sRow Row = TUSearch_( Token );

		if ( Row != qNIL )
			Heads_.Recall( Row, Head );

		return Head;
	}

	const str::dString &TSHeadSearch_(
		const str::dString &Token,
		str::dString &Head )
	{
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.InitAndLock( MutexHandler_ );

		TUHeadSearch_( Token, Head );
	qRR;
	qRT;
	qRE;
		return Head;
	}

	rClient_ *Create_(
		const str::dString &Token,
		const str::dString &Head )
	{
		rClient_ *Client = NULL;
	qRH;
		mtx::rMutex Mutex;
		sRow Row = qNIL;
	qRB;
		Mutex.InitAndLock( MutexHandler_) ;

		Row = TUSearch_( Token );

		if ( Row == qNIL ) {
			Row = Tokens_.Append( Token );

			if ( Row != Clients_.New() )
				qRGnr();

			if ( Row != Heads_.New() )
				qRGnr();
		} else
			delete Clients_( Row );

		if ( (Client = new rClient_) == NULL )
			qRAlc();

		Client->Init();

		Clients_.Store( Client, Row );

		Heads_.Store( Head, Row );
	qRR;
		if ( Client != NULL )
			delete Client;
	qRT;
	qRE;
		return Client;
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

	struct sData_
	{
		sck::sSocket Socket = sck::Undefined;
		const char *IP = NULL;
	};

	void NewConnexionRoutine_(
		sData_ &Data,
		mtk::gBlocker &Blocker )
	{
	qRFH;
		sck::sSocket Socket = sck::Undefined;
		str::wString IP;
		str::wString Token, Head;
		sck::rRWFlow Flow;
		tol::bUUID UUID;
		rClient_ *Client = NULL;
		mtx::rMutex Mutex;
	qRFB;
		Socket = Data.Socket;
		IP.Init( Data.IP );

		Blocker.Release();

		Flow.Init( Socket, false, sck::NoTimeout );

		Token.Init();
		Get_( Flow, Token );

		if ( ( Token.Amount() == 0 )
			   || ( ( Token.Amount() > 1 ) &&
				    ( ( Token( 0 ) == '_' )	// This is deprecated; only '&' will be used as forced token marker in the future.
					   || ( Token( 0 ) == '&' ) ) ) ) {

			if ( Token.Amount() == 0 )
				Token.Append( tol::UUIDGen( UUID ) );
			else
				Token.Remove( Token.First() );

			Head.Init();
			Get_( Flow, Head );

			Client = Create_( Token, Head );
		} else {
			Client = TSClientSearch_( Token );
		}

		if ( Client == NULL )
			Token.Init();
		else {
			Client->Access.WriteBegin();
			Client->Socket = Socket;
			Client->IP.Init( IP );
			Client->Access.WriteEnd();
		}

		Put_( Token, Flow );
	qRFR;
	qRFT;
	qRFE( sclmisc::ErrFinal() );
	}

	void ListeningRoutine_( void * )
	{
	qRFH;
		sData_ Data;
	qRFB;
		while ( true ) {
			Data.Socket = Listener_.GetConnection( Data.IP );

			mtk::Launch( NewConnexionRoutine_, Data );
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

sck::sSocket dmopool::GetConnection(
	const str::dString &Token,
	str::dString &IP )
{
	sck::sSocket Socket = sck::Undefined;
	rClient_ *Client = TSClientSearch_( Token );

	if ( Client != NULL ) {
		if ( !Client->Access.ReadBegin( 1000 ) ) {	// Give 1 second to the client to respond.
			Client->GiveUp = true;				// No available connections within 1 second, tells other to give up.
			Client->Access.WriteDismiss();		// The 'ReadBegin()' from another thread will now succeed.
			qRGnr();
		}

		if ( Client->GiveUp ) {	// 'ReadBegin()' succeeded, but we have were instructed to give up.
			Client->Access.ReadEnd();	// For the following 'WriteDismiss()' to succeed.
			Client->Access.WriteDismiss();		// The 'ReadBegin()' from another thread will now succeed.
			qRGnr();
		}

		Socket = Client->Socket;
		IP.Append( Client->IP );
		Client->Access.ReadEnd();
	}

	return Socket;
}

namespace {
	void GetHead_(
		void *UP,
		str::dString &Head )
	{
		TSHeadSearch_( *(const str::wString *)UP, Head );	// 'UP' contains the token.
	}
}


qGCTOR( dmopool )
{
	MutexHandler_ = mtx::Create();
	Tokens_.Init();
	Heads_.Init();
	Clients_.Init();
	sclxdhtml::SetHeadFunction( GetHead_ );
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

