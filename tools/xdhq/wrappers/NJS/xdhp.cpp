/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhp.h"

#include "registry.h"
#include "proxy.h"

#include "csdbns.h"
#include "csdcmn.h"
#include "csdmns.h"
#include "mtk.h"
#include "sclargmnt.h"

using namespace xdhp;

namespace {
	typedef sclnjs::cAsync cAsync_;

	lstbch::qLBUNCHwl( n4njs::cUCallback * ) Callbacks_;
	stsfsm::wAutomat Automat_;

	void Add_(
		const str::dString &Tag,
		n4njs::cUCallback *Callback )
	{
		if ( stsfsm::Add( Tag, *Callbacks_.Add( Callback ), Automat_ ) != stsfsm::UndefinedId )
			qRGnr();
	}

	void Add_(
		const str::dStrings &Tags,
		const n4njs::dCallbacks &Callbacks )
	{
		sdr::sRow Row = Tags.First();

		if ( Tags.Amount() != Callbacks.Amount() )
			qRGnr();

		while ( Row != qNIL ) {
			Add_( Tags( Row ), Callbacks( Row ) );

			Row = Tags.Next( Row );
		}
	}

	n4njs::cUCallback *Get_( const str::dString &Tag )
	{
		sdr::sRow Row = stsfsm::GetId( Tag, Automat_ );

		if ( Row != qNIL )
			return Callbacks_( Row );
		else
			return NULL;
	}

	void LaunchServer_(
		void *UP,
		mtk::gBlocker &Blocker )
	{
		if ( UP == NULL )
			qRFwk();

		csdmns::rServer &Server = *(csdmns::rServer *)UP;

		Blocker.Release();

		Server.Process();
	}
}

#if 0
namespace {
	qCDEF( char *, Id_, "_q37XDHRack" );

	class rXDHRack_ {
	private:
		xdh_cmn::rProcessing Processing_;
		csdmns::rServer Server_;
	public:
		xdh_cmn::rSharing Sharing;
		sclnjs::rCallback ConnectCallback;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Sharing, Processing_, Server_, ConnectCallback );
		}
		qCDTOR( rXDHRack_ );
		void Init(
			csdbns::sService Service,
			sclnjs::rCallback &ConnectCallback )
		{
			Sharing.Init();
			Processing_.Init( Sharing );
			Server_.Init( Service, Processing_ );
			this->ConnectCallback.Init();
			this->ConnectCallback = ConnectCallback;
			mtk::Launch( LaunchServer_, &Server_ );
		}
	} Rack_;
}
#else
namespace {
	qCDEF( char *, Id_, "_q37XDHRack" );

	namespace {
		typedef proxy::rData rPData_;
	}

	qENUM( Status_ ) {
		sNew,		// New connexion.
		sAction,	// Action to be handled.
		sPending,	// A response of an action has to be handled.
		s_amount,
		s_Undefined
	};

	class rData_
	: public rPData_
	{
	public:
		sclnjs::rCallback Callback;
		sclnjs::rObject XDH;	// User overloaded 'XDH' JS class.
		eStatus_ Status;
		void reset( bso::sBool P = true )
		{
			rPData_::reset( P );
			tol::reset( P, Callback, XDH );
			Status = s_Undefined;
		}
		qCVDTOR( rData_ );
		void Init( void )
		{
			rPData_::Init();

			tol::Init( Callback, XDH );
			Status = s_Undefined;
		}
	};

	sclnjs::rCallback ConnectCallback_;

	proxy::rSharing<rData_> Sharing_;

	namespace {
		typedef proxy::rProcessing<rData_> rPProcessing_;

		class rProcessing_
		: public rPProcessing_
		{
		protected:
			rData_ *PRXYNew( void ) override
			{
				rData_ *Data = new rData_;

				if ( Data == NULL )
					qRAlc();

				Data->Init();
				Data->Status = sNew;

				Sharing_.Write( Data );

				return Data;
			}
			void PRXYOnAction( rData_ *Data ) override
			{
				Data->Status = sAction;
				::Sharing_.Write( Data );
			}
			void PRXYOnPending( rData_ *Data ) override
			{
				Data->Status = sPending;
				Sharing_.Write( Data );
			}
		};
	}

	class rXDHRack_
	: public sclnjs::cAsync
	{
	private:
		rProcessing_ Processing_;
		csdmns::rServer Server_;
		qRMV( rData_, D_, Data_ );
		void SetCallbackArguments_(
			const proxy::rReturn &Return,
			sclnjs::dArguments &Arguments )
		{
			switch ( Return.GetType() ) {
			case prxy_recv::tString:
				Arguments.Add( Return.GetString() );
				break;
			case prxy_recv::tStrings:
				Arguments.Add( Return.GetStrings() );
				break;
			case prxy_recv::t_Undefined:
				break;
			default:
				qRGnr();
				break;
			}
		}
	protected:
		virtual void UVQWork( void ) override
		{
			WaitForData();
		}
		virtual sclnjs::eBehavior UVQAfter( void ) override
		{
			HandleData();

			return sclnjs::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Processing_, Server_ );
			Data_ = NULL;
		}
		qCDTOR( rXDHRack_ );
		void Init(
			csdbns::sService Service,
			const str::dString &NewSessionAction )
		{
			Processing_.Init( NewSessionAction );
			Server_.Init( Service, Processing_ );
			mtk::Launch( LaunchServer_, &Server_ );
			Data_ = NULL;
		}
		void WaitForData( void )
		{
			if ( Data_ != NULL )
				qRGnr();

			Data_ = (rData_ *)Sharing_.Read();
		}
		void HandleData( void )
		{
		qRH;
			sclnjs::rCallback Callback;
			sclnjs::wArguments Arguments;
		qRB;
			rData_ &Data = D_();
			Callback.Init();
			Callback = Data.Callback;
			Data.Callback.reset( false );

			switch ( Data.Status ) {
			case sNew:
				Data.Recv.ReadBegin();
				ConnectCallback_.ObjectLaunch( Data.XDH );
				Data.XDH.Set( Id_, &Data );
				Data.Recv.ReadEnd();
				break;
			case sAction:
				Callback.Init();
				Data.Recv.ReadBegin();
				Callback.Assign( Get_( Data.Recv.Action ) );
				Callback.VoidLaunch( Data.XDH, Data.Recv.Id );
				Data.Recv.ReadEnd();
				if ( !Data.IsTherePendingRequest() )
					Data.Sent.WriteDismiss();
				Callback.reset( false );
				break;
			case sPending:
				Arguments.Init();
				Data.Recv.ReadBegin();
				SetCallbackArguments_( Data.Recv.Return, Arguments );
				Data.Recv.ReadEnd();
				Callback.VoidLaunch( Arguments );
				if ( !Data.IsTherePendingRequest() )
					Data.Sent.WriteDismiss();
				Callback.reset( false );
				break;
			default:
				qRGnr();
				break;
			}

			Data_ = NULL;
		qRR;
		qRT;
		qRE;
		}
	} Rack_;
}
#endif


SCLNJS_F( xdhp::Register )
{
qRH;
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;
	sclnjs::rStrings Tags;
	sclnjs::rCallbacks Callbacks;
qRB;
	tol::Init( Tags, Callbacks );
	Caller.GetArgument( Tags, Callbacks );

	Add_( Tags.Get(), Callbacks.Get() );

	Callbacks.reset( false );	// So the stored callbacks survive.
qRR
qRT
qRE
}


#if 0
SCLNJS_F( xdhp::Listen )
{
qRH;
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;
	sclnjs::rCallback Callback;
	str::wString Arguments;
qRB;
	tol::Init( Callback, Arguments );
	Caller.GetArgument( Callback, Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Rack_.Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );

	Callback.reset( false );	// To avoid the destruction of contained items, as their are now managed by the rack.

	while ( true ) {
		xdh_cmn::rData &Data = Rack_.Sharing.ReadJS();
		Callback.Init();
		Callback = Data.JS.Callback;
		Data.JS.Callback.reset( false );

		if ( Callback.HasAssignation() ) {	// There is a pending callback.
			Callback.VoidLaunch( Data.JS.Arguments.Core() );
			Callback.reset( false );
		} else if ( Data.JS.Action.Amount() != 0 ) {	// No pending callback, but an action has to be handled launched.
			Callback.Init();
			Callback.Assign( Get_( Data.JS.Action ) );
			Callback.VoidLaunch( Data.XDH, Data.JS.Id );
			Callback.reset( false );
			tol::Init( Data.JS.Id, Data.JS.Action );
		} else {	// A new connection was open.
			Rack_.ConnectCallback.ObjectLaunch( Data.XDH, Data.JS.Arguments.Core() );
			Data.XDH.Set( Id_, &Data );
		}

		Data.Unlock();
	}
qRR;
qRT;
qRE;
}
#else
SCLNJS_F( xdhp::Listen )
{
qRH;
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;
	str::wString Arguments, Action;
qRB;
	tol::Init( Arguments, Action );
	Caller.GetArgument( ConnectCallback_, Arguments, Action );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Rack_.Init( sclmisc::MGetU16( registry::parameter::Service ), Action );

#if 0
	while ( true ) {
		Rack_.WaitForData();
		Rack_.HandleData();
	}
#else
	sclnjs::Launch( Rack_ );
#endif
qRR;
qRT;
qRE;
}
#endif

namespace {
	rData_ &GetData_( sclnjs::sCaller &Caller )
	{
		rData_ *Data = NULL;
	qRH;
		sclnjs::rObject Object;
	qRB;
		Object.Init();
		Caller.GetArgument( Object );

		Data = (rData_ * )Object.Get( Id_ );

		if ( Data == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Data;
	}
}

SCLNJS_F( xdhp::Launch )
{
qRH;
	rData_ &Data = GetData_( Caller );
	proxy::rNewArguments &Arguments = Data.Sent.NewArguments;
	int Amount = 0;
	int &RawType = Amount;
	str::wString String;
	str::wStrings Strings;
qRB;
	Data.Sent.WriteBegin();

	Data.Request = prxy_cmn::rNew;
	Arguments.Init();

	Caller.GetArgument( Arguments.Command );

	Caller.GetArgument( RawType );

	if ( RawType >= prxy_recv::t_amount )
		qRGnr();

	Data.ReturnType = (prxy_recv::eType)RawType;

	Caller.GetArgument( Amount );

	while ( Amount-- ) {
		String.Init();
		Caller.GetArgument( String );
		Arguments.Strings.Append( String );
	}

	Caller.GetArgument( Amount );

	while ( Amount-- ) {
		Strings.Init();
		Caller.GetArgument( Strings );
		Arguments.Arrays.Append( Strings );
	}

	Caller.GetArgument( Data.Callback );

	Data.Sent.WriteEnd();
qRR;
qRT;
qRE;
}

qGCTOR( xdhp )
{
	tol::Init( Automat_, Callbacks_, ConnectCallback_, Sharing_ );
}