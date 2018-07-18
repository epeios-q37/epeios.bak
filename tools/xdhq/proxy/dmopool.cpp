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

#include "bch.h"
#include "crt.h"
#include "csdbns.h"
#include "flx.h"
#include "mtk.h"
#include "str.h"

namespace {
	mtx::rHandler Mutex_ = mtx::UndefinedHandler;
	qROW( Row );
	crt::qMCRATEw( str::dString, sRow ) Tokens_;
	crt::qMCRATEw( bch::qBUNCHdl( sck::sSocket ), sRow ) Sockets_;
	csdbns::rListener Listener_;

	sRow Search_( const str::dString &Token )
	{
		mtx::Lock( Mutex_ );

		sRow Row = Tokens_.First();

		while ( ( Row != qNIL ) && (Tokens_( Row ) != Token) )
			Row = Tokens_.Next( Row );


		mtx::Unlock( Mutex_ );

		return Row;
	}

	sRow Create_( const str::dString &Token )
	{
		sRow Row = qNIL;

		if ( Search_( Token ) != qNIL )
			qRGnr();

		Row = Tokens_.Append( Token );

		if ( Sockets_.New() != Row )
			qRGnr();

		Sockets_( Row ).Init();

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
	qRH
		sck::sSocket Socket = (sck::sSocket)UP;
		bso::sBool Locked = false;
		str::wString Token;
		sck::rRWFlow Flow;
		tol::bUUID UUID;
		sRow Row = qNIL;
	qRB;
		Blocker.Release();

		Flow.Init( Socket, false, sck::NoTimeout );

		Token.Init();
		Get_( Flow, Token );

		if ( Token.Amount() == 0 ) {
			Token.Append( tol::UUIDGen( UUID ) );
//			Token.Append( "coucou" );

			Row = Create_( Token );
		} else if ( Token == "xdhq_desktop" )
			Row = Create_( Token );
		else
			Row = Search_( Token );

		if ( Row == qNIL )
			Token.Init();
		else {
			mtx::Lock( Mutex_ );
			Locked = true;

			Sockets_( Row ).Push( Socket );

			mtx::Unlock( Mutex_ );
			Locked = false;
		}

		Put_( Token, Flow );
	qRR;
	qRT;
		if( Locked )
			mtx::Unlock( Mutex_ );
	qRE;
	}

	void ListeningRoutine_( void * )
	{
		sck::sSocket Socket = sck::Undefined;
		const char *IP;

		while ( true ) {
			Socket = sck::Undefined;

			Socket = Listener_.GetConnection( IP );

			mtk::Launch( NewConnexionRoutine_, (void *)Socket );
		}
	}

	void Init_( void )
	{
		Mutex_ = mtx::Create();
		Tokens_.Init();
		Sockets_.Init();
		Listener_.Init( 51000 );

		mtk::RawLaunch( ListeningRoutine_, NULL );
	}
}

sck::sSocket dmopool::GetConnexion( const str::dString &Token )
{
	sck::sSocket Socket = sck::Undefined;
	sRow Row = qNIL;

	Row = Search_( Token );

	if ( Row == qNIL )
		qRGnr();

	mtx::Lock( Mutex_ );

	Socket = Sockets_( Row ).Pop();

	mtx::Unlock( Mutex_ );

	return Socket;
}

qGCTOR( dmopool )
{
	Init_();
}

