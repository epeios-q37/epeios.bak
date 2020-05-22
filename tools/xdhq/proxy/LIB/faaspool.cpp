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

# include "faaspool.h"

using namespace faaspool;

#include "prtcl.h"

#include "plugins.h"

#include "registry.h"

#include "csdbns.h"
#include "flx.h"
#include "lstbch.h"
#include "lstcrt.h"
#include "mtk.h"
#include "sclm.h"
#include "str.h"

namespace {
    xdhcuc::cGlobal *Callback_ = NULL;

    xdhcuc::cGlobal &C_(void)
    {
        if ( Callback_ == NULL )
            qRGnr();

        return *Callback_;
    }
}

void faaspool::SetCallback(xdhcuc::cGlobal &Callback)
{
    ::Callback_ = &Callback;
}

namespace {
	static qCDEF( char *, ProtocolId_, "7b4b6bea-2432-4584-950b-e595c9e391e1" );

	namespace registry_ {
		namespace parameter {
		}

		namespace definition {
            namespace {
                sclr::rEntry FaaS_("FaaS", sclr::Definitions);
            }

			sclr::rEntry Notification( "Notification", FaaS_ );
			sclr::rEntry URL( "URL", FaaS_ );
		}
	}

	qROW( FRow_ );	// Frontend 'Row'.

	typedef lstbch::qLBUNCHd( rShared *, sFRow_ ) dShareds_;
	qW( Shareds_ );

	qROW( BRow_ );	// Back-end Row.
}

namespace faaspool {
	class rBackend_
	{
	private:
		// Protects access to 'Shareds' and 'PendingDismiss_';
		mtx::rHandler Mutex_;
		bso::sBool PendingDismiss_;
		// Prevents destruction of 'Driver_' until no more client use it.
		tht::rBlocker Blocker_;
		xdhcuc::sRow TRow_; // Token row.
		void InvalidAll_( void )
		{
			sFRow_ Row = Shareds.First();

			while ( Row != qNIL ) {
				Shareds( Row )->Id = UndefinedId;

				Row = Shareds.Next( Row );
			}
		}
	public:
		sBRow_ Row;
		fdr::rRWDriver *Driver;
		wShareds_ Shareds;
		mtx::rHandler Access;
		tht::rBlocker Switch;
		bso::sBool GiveUp;
		str::wString IP;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Mutex_ != mtx::Undefined)
					mtx::Delete(Mutex_);

				if ( Access != mtx::Undefined )
					mtx::Delete( Access, true );

				if ( TRow_ != qNIL )
						C_().Remove(TRow_);

				InvalidAll_();
			}

			Mutex_ = mtx::Undefined;
			PendingDismiss_ = false;
			Blocker_.reset(P);
			TRow_ = qNIL;
			Row = qNIL;
			Driver = NULL;
			Shareds.reset( P );
			Access = mtx::Undefined;
			Switch.reset( P );
			tol::reset(P, IP);
			GiveUp = false;	// If at 'true', the client is deemed to be disconnected.
		}
		qCDTOR( rBackend_ );
		void Init(
			sBRow_ Row,
			xdhcuc::sRow TRow,
			fdr::rRWDriver &Driver,
			const str::dString &IP )
		{
			reset();

			Mutex_ = mtx::Create();
			PendingDismiss_ = false;
			Blocker_.Init();
			this->Row = Row;
			TRow_ = TRow;
			this->Driver = &Driver;
			Shareds.Init();
			Access = mtx::Create();
			Switch.Init();
			this->IP.Init( IP );
			GiveUp = false;
		}
		bso::sBool Set(rShared &Shared)
		{
			sFRow_ Row = qNIL;
		qRH
		qRB
			Row = Shareds.New();

			if ( *Row > Max )
				qRGnr();

			Shareds.Store( &Shared, Row );

			Shared.Id = (sId)*Row;
			Shared.Driver = Driver;
			Shared.Switch = &Switch;
		qRR
			if ( Row != qNIL )
				Shareds.Remove(Row);

			Row = qNIL;
		qRT
		qRE
			return Row != qNIL;
		}
		void Release(sId Id)
		{
		qRH
			mtx::rMutex Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);

			Shareds.Remove((sFRow_)Id);

			if (PendingDismiss_)
				if ( Shareds.Amount() == 0)
					Blocker_.Unblock();
		qRR
		qRT
		qRE
		}
		void WaitUntilNoMoreClient(void)
		{
		qRH
			mtx::rMutex Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);

			if ( Shareds.Amount() ) {
				PendingDismiss_ = true;
				Mutex.Unlock();
				Blocker_.Wait();
			}
		qRR
		qRT
		qRE
		}
	};

	mtx::rHandler MutexHandler_ = mtx::Undefined;
	lstcrt::qLMCRATEw( str::dString, sBRow_ ) Tokens_;
	lstcrt::qLMCRATEw( str::dString, sBRow_ ) Heads_;
	lstbch::qLBUNCHw( rBackend_ *, sBRow_ ) Backends_;
	csdbns::rListener Listener_;

	// NOTA : TU : Thread Unsafe ; TS : Thread Safe.

	sBRow_ TUGetBackendRow_( const str::dString &Token )
	{
		if ( !mtx::IsLocked( MutexHandler_ ) )
			qRGnr();

		sBRow_ Row = Tokens_.First();

		while ( (Row != qNIL) && ( Tokens_( Row ) != Token) )
			Row = Tokens_.Next( Row );

		return Row;
	}

	rBackend_ *TUGetBackend_( const str::dString &Token )
	{
		sBRow_ Row = TUGetBackendRow_( Token );

		if ( Row != qNIL )
			return Backends_( Row );
		else
			return NULL;
	}

	rBackend_ *TSGetBackend_( const str::dString &Token )
	{
		rBackend_ *Backend = NULL;
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.InitAndLock( MutexHandler_ );

		Backend = TUGetBackend_( Token );
	qRR;
	qRT;
	qRE;
		return Backend;
	}

	bso::sBool TUGetHead_(
		const str::dString &Token,
		str::dString &Head )
	{
		sBRow_ Row = TUGetBackendRow_( Token );

		if ( Row != qNIL ) {
			Heads_.Recall( Row, Head );
			return true;
        } else
            return false;
	}

	bso::sBool TSGetHead_(
		const str::dString &Token,
		str::dString &Head )
	{
        bso::sBool Found = false;
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.InitAndLock( MutexHandler_ );

		Found = TUGetHead_( Token, Head );
	qRR;
	qRT;
	qRE;
		return Found;
	}

	void Remove_(sBRow_ Row)
	{
	qRH;
		mtx::rMutex Mutex;
	qRB;
		Mutex.Init( MutexHandler_ );

		if ( !Backends_.Exists( Row ) || !Tokens_.Exists( Row ) || !Heads_.Exists( Row ) )
			qRGnr();

		Backends_.Remove( Row );
		Tokens_.Remove( Row );
		Heads_.Remove( Row );
	qRR;
	qRT;
	qRE;
	}

	rBackend_ *Create_(
		fdr::rRWDriver &Driver,
		const str::dString &IP,
		const str::dString &Token,
		const str::dString &Head )
	{
		rBackend_ *Backend = NULL;
	qRH;
		mtx::rMutex Mutex;
		sBRow_ Row = qNIL;
	qRB;
		Mutex.InitAndLock( MutexHandler_) ;

		Row = TUGetBackendRow_( Token );

		if ( Row == qNIL ) {
			Row = Tokens_.New();

			if ( Row != Backends_.New() )
				qRGnr();

			if ( Row != Heads_.New() )
				qRGnr();

			if ( (Backend = new rBackend_) == NULL )
				qRAlc();

			Backend->Init( Row, C_().Create(Token), Driver, IP );

			Tokens_.Store( Token, Row );

			Backends_.Store( Backend, Row );

			Heads_.Store( Head, Row );
		}
	qRR;
		if ( Backend != NULL )
			delete Backend;
	qRT;
	qRE;
		return Backend;
	}
}

namespace {
	void Get_(
		flw::rRFlow &Flow,
		str::dString &String )
	{
		prtcl::Get( Flow, String );
	}

	void Put_(
		const str::dString &String,
		flw::rWFlow &Flow )
	{
		prtcl::Put( String, Flow );
	}

	void Put_(
		const char *String,
		flw::rWFlow &Flow )
	{
		prtcl::Put( String, Flow );
	}

	namespace token_ {
		plgn::rRetriever<plugins::cToken> PluginRetriever_;

		namespace {
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
                    else if ( (Raw.Amount() > 1) && (Raw( 0 ) == '&') )
                        Normalized.Remove( Normalized.First() );
                    else
                        return plugins::sBad;

                    return plugins::sOK;
                }
            public:
                void reset(bso::sBool = true ) {}
                qCVDTOR( sToken_ );
                void Init( void ) {}
            } DefaultHandler_;
        }

		plugins::cToken &GetPlugin( void )
		{
			PluginRetriever_.Init();

			if ( sclm::Plug( plugins::TokenPluginTarget, NULL, PluginRetriever_, qRPU ) )
				return PluginRetriever_.Plugin();
			else {
				DefaultHandler_.Init();
				return DefaultHandler_;
			}
		}
	}

	void Notify_(
		const char *Message,
		flw::rWFlow &Flow )
	{
	qRH;
		str::wString Notification;
	qRB;
		Notification.Init( Message );

		if ( Notification.IsEmpty() )
			sclm::OGetValue( registry_::definition::Notification, Notification );

		prtcl::Put( Notification, Flow );
	qRR;
	qRT;
	qRE;
	}

	void Handshake_( fdr::rRWDriver &Driver )
	{
	qRH;
		flw::rDressedRWFlow<> Flow;
	qRB;
		Flow.Init( Driver );

		switch ( csdcmn::GetProtocolVersion( ProtocolId_, Flow ) ) {
		case 0:
			Put_( "", Flow );
			Notify_( NULL, Flow );
			Flow.Commit();
			break;
		case csdcmn::UndefinedVersion:
			Put_( "\nIncompatible FaaS protocol! Please update your software.\n", Flow );
			Flow.Commit();
			qRGnr();
		default:
			Put_( "\nUnknown FaaS protocol version!\n", Flow );
			Flow.Commit();
			qRGnr();
			break;
		}
	qRR;
	qRT;
	qRE;
	}

	const str::dString &BuildURL_(
        const str::dString &Address,
        const str::dString &RawService,
        const str::dString &Token,
        str::dString &URL )
    {
    qRH
        str::wString TaggedURL, Service;
        tagsbs::tvalues Tags;
    qRB
        TaggedURL.Init();
        sclm::MGetValue(registry_::definition::URL, TaggedURL);

        if ( RawService.Amount()) {
            Service.Init(":");
            Service.Append(RawService);
        } else
            Service.Init();

        Tags.Init();

        Tags.Append("Address", Address, "Service", Service, "Token", Token);

        if ( !tagsbs::SubstituteLongTags(TaggedURL, Tags, URL) )
            qRFwk();
    qRR
    qRE
    qRT
        return URL;
    }

	rBackend_ *CreateBackend_(
		fdr::rRWDriver &Driver,
		const str::dString &IP )
	{
		rBackend_ *Backend = NULL;
	qRH;
		flw::rDressedRWFlow<> Flow;
		str::wString Token, Head, Address, ErrorMessageLabel, ErrorMessage, URL;
		plugins::eStatus Status = plugins::s_Undefined;
	qRB;
		Flow.Init( Driver );

		Token.Init();
		Get_( Flow, Token );

		switch ( Status = token_::GetPlugin().Handle(Token) ) {
		case plugins::sOK:
			tol::Init(Head, Address);
			Get_(Flow, Head);
			Get_(Flow, Address);    // Address to which the toolkit has connected.

			if ( (Backend = Create_( Driver, IP, Token, Head )) == NULL ) {
				ErrorMessage.Init();
				sclm::GetBaseTranslation( "TokenAlreadyInUse", ErrorMessage, Token );
				Token.Init();	// To report backend that there is an error.
			}
			break;
		default:
			Token.Init();	// To report an error (see below).
			ErrorMessageLabel.Init( "PLUGINS_" );
			ErrorMessageLabel.Append( plugins::GetLabel( Status ) );
			ErrorMessage.Init();
			sclm::GetBaseTranslation( ErrorMessageLabel, ErrorMessage );
			break;
		}

		Put_( Token, Flow );

		if ( Backend == NULL )
			Put_( ErrorMessage, Flow );
        else {
            URL.Init();
            Put_(BuildURL_(Address,str::wString(),Token,URL), Flow);
        }
	qRR;
		if ( Backend != NULL )
			delete Backend;

		Backend = NULL;
	qRT;
	qRE;
		return Backend;
	}

	void HandleSwitching_(
		fdr::rRWDriver &Driver,
		const dShareds_ &Shareds,
		tht::rBlocker &Blocker )
	{
	qRH;
		flw::rDressedRFlow<> Flow;
		sId Id = UndefinedId;
		bso::sBool IsError = false;
	qRB;
		Flow.Init( Driver );

		while ( true ) {
			Id = UndefinedId;

			if ( Flow.EndOfFlow() )
				break;

			Id = GetId( Flow, &IsError );

			if ( IsError )
				qRGnr();

			if ( !Shareds.Exists( Id ) ) {
				Id = UndefinedId;
				qRGnr();
			}

			Shareds( Id )->Read.Unblock();

			Blocker.Wait();	// Waits until all data in flow red.
		}
	qRR;
	qRT;
	qRE;
	}

	struct gConnectionData_
	{
		sck::sSocket Socket = sck::Undefined;
		const char *IP = NULL;
	};

	void NewConnexion_(
		gConnectionData_ &Data,
		mtk::gBlocker &Blocker )
	{
	qRH;
		sck::sSocket Socket = sck::Undefined;
		str::wString IP;
		mtx::rMutex Mutex;
		sck::rRWDriver Driver;
		rBackend_ *Backend = NULL;
	qRB;
		Socket = Data.Socket;
		IP.Init( Data.IP );

		Blocker.Release();

		Driver.Init( Socket, false, fdr::ts_Default );

		Handshake_( Driver );

		if ( ( Backend = CreateBackend_( Driver, IP ) ) != NULL )
			HandleSwitching_( Driver, Backend->Shareds, Backend->Switch );	// Doesn't return until disconnection or error.
	qRR;
	qRT;
		if ( Backend != NULL ) {
			Backend->WaitUntilNoMoreClient();
			Remove_( Backend->Row );
			delete Backend;

			Backend = NULL;
		}

		Driver.reset();	// Otherwise it will be done after the destruction of the socket, hence the commit will fail.

		if ( Socket != sck::Undefined ) {
			sck::Close( Socket, err::hUserDefined );	// An error occurring during closing is ignored.
			Socket = sck::Undefined;
		}
	qRE;
	}

	void NewConnexionRoutine_(
		gConnectionData_ &Data,
		mtk::gBlocker &Blocker )
	{
	qRFH;
	qRFB;
		NewConnexion_( Data, Blocker );
	qRFR;
	qRFT;
	qRFE(sclm::ErrorDefaultHandling());
	}

	void ListeningRoutine_( void * )
	{
	qRFH;
		gConnectionData_ Data;
	qRFB;
		while ( true ) {
			Data.Socket = Listener_.GetConnection( Data.IP );

			mtk::Launch(NewConnexionRoutine_, Data);
		}
	qRFR;
	qRFT;
	qRFE(sclm::ErrorDefaultHandling());
	}
}

void faaspool::Initialize( void )
{
qRH;
	csdbns::sService Service = csdbns::Undefined;
qRB;
	if ( (Service = sclm::OGetU16( registry::parameter::faas::Service, csdbns::Undefined ) ) != csdbns::Undefined ) {
		Listener_.Init( Service );

		mtk::RawLaunch(ListeningRoutine_, NULL, true);
	}
qRR;
qRT;
qRE;
}

bso::sBool faaspool::GetConnection_(
	const str::dString &Token,
	str::dString &IP,
	rShared &Shared,
	rBackend_ *&Backend)
{
qRH;
	mtx::rMutex Mutex;
	flw::rDressedWFlow<> Flow;
qRB;
	if ( Backend != NULL)
		qRGnr();

	Backend = TSGetBackend_( Token );

	if ( Backend != NULL ) {
		Mutex.Init( Backend->Access );

		Mutex.Lock();

		if ( Backend->Set(Shared) ) {
			Flow.Init( *Backend->Driver );
			IP.Append( Backend->IP );
			PutId( CreationId, Flow );	// To signal to the back-end a new connection.
			PutId( Shared.Id, Flow );	// The id of the new front-end.
			Flow.Commit();
		}  else
			Backend = NULL;
	}
qRR;
qRT;
qRE;
	return Backend != NULL;
}

void faaspool::rRWDriver::Release_(void)
{
qRH
qRB
	fdr::rWDriver &Driver = D_();

	PutId(ClosingId, Driver);
	PutId(Shared_.Id, Driver);
	Driver.Commit(true, err::h_Default);
qRR
	ERRRst();
qRT
	B_().Release(Shared_.Id);
	Shared_.Id = UndefinedId;
	Backend_ = NULL;
qRE
}

bso::sBool faaspool::GetHead(
    const str::dString &Token,
    str::dString &Head )
{
    return TSGetHead_(Token,Head);	// 'UP' contains the token.
}

qGCTOR(faaspool)
{
	MutexHandler_ = mtx::Create();
	Tokens_.Init();
	Heads_.Init();
	Backends_.Init();
}

qGDTOR(faaspool)
{
	if ( MutexHandler_ != mtx::Undefined )
		mtx::Delete( MutexHandler_, true );

	sBRow_ Row = Backends_.First();

	while ( Row != qNIL ) {
		delete Backends_( Row );

		Row = Backends_.Next( Row );
	}
}

