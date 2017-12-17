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

		if ( Data.JS.Callback.HasAssignation() ) {	// There is a pending action launched from callbac.
			Data.JS.Callback.VoidLaunch( Data.JS.Arguments );
			Data.JS.Callback.reset();
		} else if ( Data.JS.Action.Amount() != 0 ) {	// No pending query, but an action was launched.
			Callback.Init();
			Callback.Assign( Get_( Data.JS.Action ) );
			Callback.VoidLaunch( Data.XDH, Data.JS.Id );
		} else {	// A new connection was open.
			Rack_.ConnectCallback.ObjectLaunch( Data.XDH, Data.JS.Arguments );
			Data.XDH.Set( Id_, &Data );
		}

		Data.Unlock();
	}
qRR
qRT
qRE
}

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

	Caller.GetArgument( Arguments.Ids );
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

SCLNJS_F( xdhp::SetCasts )
{
	DATA;

	Caller.GetArgument( Arguments.Id, Arguments.Tags, Arguments.Values, JS.Callback );
	Server.Request = xdh_ups::rSetCasts;
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