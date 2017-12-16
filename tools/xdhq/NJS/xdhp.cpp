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
#include "xdh_ups.h"

#include "csdbns.h"
#include "csdcmn.h"
#include "csdmxs.h"
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
}

namespace {
	qCDEF( char *, Id_, "_q37Rack" );

	class rRack_ {
	private:
		xdh_ups::rProcessing Processing_;
		csdmxs::rCallback Callback_;
		csdbns::rServer Server_;
	public:
		xdh_ups::rSharing Sharing;
		sclnjs::rCallback ConnectCallback;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Sharing, Processing_, Callback_, Server_, ConnectCallback );
		}
		qCDTOR( rRack_ );
		void Init(
			csdbns::sService Service,
			sclnjs::rCallback &ConnectCallback )
		{
			Sharing.Init();
			Processing_.Init( Sharing );
			Callback_.Init( Processing_ );
			Server_.Init( Service, Callback_ );
			ConnectCallback.Init();
			ConnectCallback = ConnectCallback;
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

namespace {
	void HandleConnection_( void )
	{
		xdh_ups::rData &Data = Rack_.Sharing.ReadJS();

		Rack_.ConnectCallback.ObjectLaunch( Data.XDH, Data.JS.Arguments );

		Data.XDH.Set( Id_, &Data );
	}
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

	Callback.reset( false );	// To avoid the destruction of contained items, as their are now managed by 'Listening'.

	HandleConnection_();

	while ( true ) {
		xdh_ups::rData &Data = Rack_.Sharing.ReadJS();
		sclnjs::rCallback *PendingCallback = &Data.JS.Callback;

		Data.JS.Callback.reset( false );

		if ( PendingCallback == NULL ) {
			Callback.Init();
			Callback.Assign( Get_( Data.JS.Action ) );
			Callback.ObjectLaunch( Data.XDH, Data.JS.Id );
		} else
			PendingCallback->VoidLaunch( Data.JS.Arguments );

		Data.Unlock();
	}
qRR
qRT
qRE
}

namespace layout_ {
	namespace {
		void Set_(
			const str::dString &Id,
			sclnjs::sCaller &Caller,
			xdh_ups::rData &Data )
		{
			Data.Server.Arguments.Init();

			treep::GetXML( Caller, Data.Server.Arguments.XML );

			Caller.GetArgument( Data.Server.Arguments.XSLFilename, Data.JS.Callback );
			Data.Server.Request = xdh_ups::server::rSetLayout;
		}
	}

	void Set(
		sclnjs::sCaller &Caller,
		xdh_ups::rData &Data )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		Caller.GetArgument( Id );

		Set_( Id, Caller, Data );
	qRR;
	qRT;
	qRE;
	}
}

namespace {
	xdh_ups::rData &GetData_( sclnjs::sCaller &Caller )
	{
		xdh_ups::rData *Data = NULL;
	qRH;
		sclnjs::rObject Object;
	qRB;
		Object.Init();
		Caller.GetArgument( Object );

		Data = (xdh_ups::rData * )Object.Get( Id_ );

		if ( Data == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Data;
	}
}

#define DATA	xdh_ups::rData &Data = GetData_( Caller )

SCLNJS_F( xdhp::SetLayout )
{
	DATA;
	layout_::Set( Caller, Data );
}

SCLNJS_F( xdhp::GetContents )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	RACK;

	Ids.Init();
	Caller.GetArgument( Ids );

	Contents.Init();
	server::GetContents( Ids, Rack.Flow, Contents );

	Caller.SetReturnValue( Contents );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::SetContents_ )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	RACK;

	tol::Init( Ids, Contents );
	Caller.GetArgument( Ids, Contents );

	server::SetContents_( Ids, Contents, Rack.Flow );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::SetWidgets )
{
qRH;
	str::wString Id;
qRB;
	RACK;

	Id.Init();
	Caller.GetArgument( Id );

	server::SetWidgets( Id, Rack.Flow );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::SetCasts )
{
qRH;
	str::wString Id;
	str::wStrings Tags, Values;
qRB;
	RACK;

	tol::Init( Id, Tags, Values );
	Caller.GetArgument( Id, Tags, Values );

	server::SetCasts( Id, Tags, Values, Rack.Flow );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::GetAttribute )
{
qRH;
	str::wString Id, Name, Value;
qRB;
	RACK;

	tol::Init( Id, Name );
	Caller.GetArgument( Id, Name );

	Value.Init();
	server::GetAttribute( Id, Name, Rack.Flow, Value );

	Caller.SetReturnValue( Value );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::SetAttribute )
{
qRH;
	str::wString Id, Name, Value;
qRB;
	RACK;

	tol::Init( Id, Name, Value );
	Caller.GetArgument( Id, Name, Value );

	server::SetAttribute( Id, Name, Value, Rack.Flow );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::GetProperty )
{
qRH;
	str::wString Id, Name, Value;
qRB;
	RACK;

	tol::Init( Id, Name );
	Caller.GetArgument( Id, Name );

	Value.Init();
	server::GetProperty( Id, Name, Rack.Flow, Value );

	Caller.SetReturnValue( Value );
qRR;
qRT;
qRE;
}

SCLNJS_F( xdhp::SetProperty )
{
qRH;
	str::wString Id, Name, Value;
qRB;
	RACK;

	tol::Init( Id, Name, Value );
	Caller.GetArgument( Id, Name, Value );

	Value.Init();
	server::SetProperty( Id, Name, Value, Rack.Flow );
qRR;
qRT;
qRE;
}

qGCTOR( xdhp )
{
	Automat_.Init();
	Callbacks_.Init();
}