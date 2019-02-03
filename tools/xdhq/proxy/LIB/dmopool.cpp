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

#include "plugins.h"

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
	static qCDEF( char *, ProtocolId_, "877c913f-62df-40a1-bf5d-4bb5e66a6dd9" );

	namespace registry_ {
		namespace parameter {
			sclrgstry::rEntry Notification( "DemoNotification", sclrgstry::Parameters );
		}
	}

	class rClient_
	{
	public:
		sId FreeId;	// First available id.
		sck::sSocket Socket;
		tht::rReadWrite Access;
		bso::sBool GiveUp;
		str::wString IP;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Socket != sck::Undefined )
					sck::Close( Socket, qRPU );
			}

			FreeId = Undefined;
			Socket = sck::Undefined;
			Access.reset( P );
			tol::reset( P, IP );
			GiveUp = false;	// If at 'true', the client is deemed to be disconnected.
		}
		qCDTOR( rClient_ );
		void Init( void )
		{
			reset();

			FreeId = 0;
			Access.Init();
			tol::Init( IP );
			GiveUp = false;
		}
		sXSocket NewXSocket( void )
		{
			sXSocket XSocket;

			if ( Socket == sck::Undefined )
				qRGnr();

			if ( FreeId == Max )
				qRLmt();

			XSocket.Init( FreeId++, Socket );

			return XSocket;
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

	void Put_(
		const char *String,
		flw::sWFlow &Flow )
	{
		prtcl::Put( String, Flow );
	}

	struct sData_
	{
		sck::sSocket Socket = sck::Undefined;
		const char *IP = NULL;
	};

	namespace token_ {
		plgn::rRetriever<plugins::cToken> PluginRetriever_;

		class sToken_
		: public plugins::cToken
		{
		protected:
			plugins::eStatus PLUGINSHandle(
				const str::dString &Raw,
				str::dString &Normalized ) override
			{
				tol::bUUID UUID;

				Normalized = Raw;

				if ( Raw.Amount() == 0 )
					Normalized.Append( tol::UUIDGen( UUID ) );

				return plugins::sOK;
			}
		public:
			void reset(bso::sBool = true ) {}
			qCVDTOR( sToken_ );
			void Init( void ) {}
		} DefaultHandler_;

		plugins::cToken &GetPlugin( void )
		{
			PluginRetriever_.Init();

			if ( sclmisc::Plug( plugins::TokenPluginTarget, NULL, PluginRetriever_, qRPU ) )
				return PluginRetriever_.Plugin();
			else {
				DefaultHandler_.Init();
				return DefaultHandler_;
			}
		}
	}

	void Notify_(
		const char *Message,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString Notification;
	qRB;
		Notification.Init( Message );

		if ( Notification.IsEmpty() )
			sclmisc::OGetValue( registry_::parameter::Notification, Notification );

		prtcl::Put( Notification, Flow );
	qRR;
	qRT;
	qRE;
	}


	void NewConnexionRoutine_(
		sData_ &Data,
		mtk::gBlocker &Blocker )
	{
	qRFH;
		sck::sSocket Socket = sck::Undefined;
		str::wString IP;
		str::wString Token, Head, ErrorMessageLabel, ErrorMessage;
		sck::rRWFlow Flow;
		rClient_ *Client = NULL;
		mtx::rMutex Mutex;
		plugins::eStatus Status = plugins::s_Undefined;
	qRFB;
		Socket = Data.Socket;
		IP.Init( Data.IP );

		Blocker.Release();

		ErrorMessage.Init();

		Flow.Init( Socket, false, sck::NoTimeout );

		switch ( csdcmn::GetProtocolVersion( ProtocolId_, Flow ) ) {
		case 0:
			Put_( "", Flow );
			Notify_( NULL, Flow );
			Flow.Commit();
			break;
		case csdcmn::UndefinedVersion:
			Put_( "Unknown demo protocol !!!", Flow );
			Flow.Commit();
			qRGnr();
		default:
			Put_( "Unknown demo version !!!", Flow );
			Flow.Commit();
			qRGnr();
			break;
		}

		Token.Init();
		Get_( Flow, Token );

		switch ( Status = token_::GetPlugin().Handle( Token ) ) {
		case plugins::sOK:
			Head.Init();
			Get_( Flow, Head );

			Client = Create_( Token, Head );
			break;
		default:
			Token.Init();
			ErrorMessageLabel.Init( "PLUGINS_" );
			ErrorMessageLabel.Append( plugins::GetLabel( Status ) );
			sclmisc::GetBaseTranslation( ErrorMessageLabel, ErrorMessage );
			break;
		}


		if ( Client != NULL ) {
			Client->Access.WriteBegin();
			Client->Socket = Socket;
			Client->IP.Init( IP );
			Client->Access.WriteEnd();
		}

		Put_( Token, Flow );

		if ( Token.Amount() == 0 )
			Put_( ErrorMessage, Flow );
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

sXSocket dmopool::GetConnection(
	const str::dString &Token,
	str::dString &IP )
{
	sXSocket XSocket;
	rClient_ *Client = TSClientSearch_( Token );

	XSocket.Init();

	if ( Client != NULL ) {
		if ( !Client->Access.ReadBegin( 1000 ) ) {	// Give 1 second to the client to respond.
			Client->GiveUp = true;				// No available connections within 1 second, tells other to give up.
			Client->Access.WriteDismiss();		// The 'ReadBegin()' from another thread will now succeed.
			qRGnr();
		}

		if ( Client->GiveUp ) {	// 'ReadBegin()' succeeded, but we were instructed to give up.
			Client->Access.ReadEnd();	// For the following 'WriteDismiss()' to succeed.
			Client->Access.WriteDismiss();		// The 'ReadBegin()' from another thread will now succeed.
			qRGnr();
		}

		XSocket = Client->NewXSocket();
		IP.Append( Client->IP );
		Client->Access.ReadEnd();
	}

	return XSocket;
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

