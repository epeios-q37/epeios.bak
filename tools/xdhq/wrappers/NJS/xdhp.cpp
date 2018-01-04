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
#include "treep.h"
#include "xdh_cmn.h"

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

	class rXDHRack_
	: public sclnjs::cAsync
	{
	private:
		xdh_cmn::rProcessing Processing_;
		csdmns::rServer Server_;
		qRMV( xdh_cmn::rData, D_, Data_ );
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
		xdh_cmn::rSharing Sharing;
		sclnjs::rCallback ConnectCallback;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Sharing, Processing_, Server_, ConnectCallback );
			Data_ = NULL;
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
			Data_ = NULL;
		}
		void WaitForData( void )
		{
			if ( Data_ != NULL )
				qRGnr();

			Data_ = &Sharing.ReadJS();
		}
		void HandleData( void )
		{
		qRH;
			sclnjs::rCallback Callback;
		qRB;
			xdh_cmn::rData &Data = D_();
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
				ConnectCallback.ObjectLaunch( Data.XDH, Data.JS.Arguments.Core() );
				Data.XDH.Set( Id_, &Data );
			}

			Data.Unlock();
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
	str::wString Arguments;
	sclnjs::rCallback Callback;
qRB;
	tol::Init( Callback, Arguments );
	Caller.GetArgument( Callback, Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Rack_.Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );

	Callback.reset( false );	// To avoid the destruction of contained items, as their are now managed by the rack.

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
	xdh_cmn::rData &GetData_( sclnjs::sCaller &Caller )
	{
		xdh_cmn::rData *Data = NULL;
	qRH;
		sclnjs::rObject Object;
	qRB;
		Object.Init();
		Caller.GetArgument( Object );

		Data = (xdh_cmn::rData * )Object.Get( Id_ );

		if ( Data == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Data;
	}
}

#define DATA\
	xdh_cmn::rData &Data = GetData_( Caller );\
	xdh_ups::rServer &Server = Data.Server;\
	xdh_dws::rJS &JS = Data.JS;\
	xdh_ups::rArguments &Arguments = Server.Arguments;\
	Arguments.Init();

SCLNJS_F( xdhp::Alert )
{
	DATA;

	Caller.GetArgument( Arguments.Message, JS.Callback );

	Server.Request = xdh_ups::rAlert;
}

SCLNJS_F( xdhp::Confirm )
{
	DATA;

	Caller.GetArgument( Arguments.Message, JS.Callback );

	Server.Request = xdh_ups::rConfirm;
}



SCLNJS_F( xdhp::SetLayout )
{
	DATA;

	Caller.GetArgument( Arguments.Id );

	treep::GetXML( Caller, Arguments.XML );

	Caller.GetArgument( Server.Arguments.XSLFilename, JS.Callback );
	Arguments.Language = Data.Language;
	Server.Request = xdh_ups::rSetLayout;
}

SCLNJS_F( xdhp::GetContents )
{
	DATA;

	Caller.GetArgument( Arguments.Ids, JS.Callback );
	Server.Request = xdh_ups::rGetContents;
}

SCLNJS_F( xdhp::SetContents )
{
	DATA;

	Caller.GetArgument( Arguments.Ids, Arguments.Contents, JS.Callback );
	Server.Request = xdh_ups::rSetContents;
}

SCLNJS_F( xdhp::DressWidgets )
{
	DATA;

	Caller.GetArgument( Arguments.Id, JS.Callback );
	Server.Request = xdh_ups::rDressWidgets;
}

SCLNJS_F( xdhp::SetCastsByIds )
{
	DATA;

	Caller.GetArgument( Arguments.Ids, Arguments.Values, JS.Callback );
	Server.Request = xdh_ups::rSetCastsByIds;
}

SCLNJS_F( xdhp::SetCastsByTags )
{
	DATA;

	Caller.GetArgument( Arguments.Id, Arguments.Tags, Arguments.Values, JS.Callback );
	Server.Request = xdh_ups::rSetCastsByTags;
}

SCLNJS_F( xdhp::GetAttribute )
{
	DATA;

	Caller.GetArgument( Arguments.Id, Arguments.Name, JS.Callback );
	Server.Request = xdh_ups::rGetAttribute;
}

SCLNJS_F( xdhp::SetAttribute )
{
	DATA;

	Caller.GetArgument( Arguments.Id, Arguments.Name, Arguments.Value, JS.Callback );
	Server.Request = xdh_ups::rSetAttribute;
}

SCLNJS_F( xdhp::GetProperty )
{
	DATA;

	Caller.GetArgument( Arguments.Id, Arguments.Name, JS.Callback );
	Server.Request = xdh_ups::rGetProperty;
}

SCLNJS_F( xdhp::SetProperty )
{
	DATA;

	Caller.GetArgument( Arguments.Id, Arguments.Name, Arguments.Value, JS.Callback );
	Server.Request = xdh_ups::rSetProperty;
}

qGCTOR( xdhp )
{
	Automat_.Init();
	Callbacks_.Init();
}