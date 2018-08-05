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
	mtx::rHandler Handler_ = mtx::Undefined;
	qROW( Row );
	crt::qMCRATEw( str::dString, sRow ) Tokens_;
	crt::qMCRATEw( bch::qBUNCHdl( sck::sSocket ), sRow ) Sockets_;
	csdbns::rListener Listener_;

	sRow Search_( const str::dString &Token )
	{
		if ( !mtx::IsLocked( Handler_ ) )
			qRGnr();

		sRow Row = Tokens_.First();

		while ( ( Row != qNIL ) && (Tokens_( Row ) != Token) )
			Row = Tokens_.Next( Row );

		return Row;
	}

	sRow Create_( const str::dString &Token )
	{
		sRow Row = qNIL;
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.Init( Handler_) ;
		Mutex.Lock();

		if ( Search_( Token ) != qNIL )
			qRGnr();

		Row = Tokens_.Append( Token );

		if ( Sockets_.New() != Row )
			qRGnr();

		Sockets_( Row ).Init();
	qRR;
	qRT;
	qRE;
		return Row;
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
		sRow Row = qNIL;
		mtx::rMutex Mutex;
	qRFB;
		Blocker.Release();

		Flow.Init( Socket, false, sck::NoTimeout );

		Token.Init();
		Get_( Flow, Token );

		Mutex.Init( Handler_ );

		if ( Token.Amount() == 0 ) {
			Token.Append( tol::UUIDGen( UUID ) );

			Row = Create_( Token );
		} else {
			Mutex.Lock();

			Row = Search_( Token );

			Mutex.Unlock();
		}

		if ( Row == qNIL )
			Token.Init();
		else {
			Mutex.Lock();

			Sockets_( Row ).Push( Socket );

			Mutex.Unlock();
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

sck::sSocket dmopool::GetConnexion( const str::dString &Token )
{
	sck::sSocket Socket = sck::Undefined;
qRH;
	mtx::rMutex Mutex;
	sRow Row = qNIL;
qRB;
	Mutex.Init( Handler_ );
	Mutex.Lock();

	Row = Search_( Token );

	if ( Row == qNIL )
		qRGnr();

	while ( !Sockets_( Row ).Amount() ) {
		Mutex.Unlock();

		tht::Defer();

		Mutex.Lock();
	}

	Socket = Sockets_( Row ).Pop();
qRR;
qRT;
qRE;
	return Socket;
}

qGCTOR( dmopool )
{
	Handler_ = mtx::Create();
	Tokens_.Init();
	Sockets_.Init();
}

qGDTOR( dmopool )
{
	if ( Handler_ != mtx::Undefined )
		mtx::Delete( Handler_, true );
}

