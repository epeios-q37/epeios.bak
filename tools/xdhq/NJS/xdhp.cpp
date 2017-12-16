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
		xdh_ups::rSharing UpstreamSharing_;
		xdh_ups::rProcessing Processing_;
		csdmxs::rCallback Callback_;
		csdbns::rServer Server_;
		qCBUFFERr Language_;
		sclnjs::rCallback ConnectCallback_;
		sclnjs::rCallback *PendingCallback_;
		sclnjs::dArguments *PendingCallbackArguments_;
		sclnjs::rObject XDH_;	// The by the user overloaded 'XDH' JS class.
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, UpstreamSharing_, Processing_, Callback_, Server_, Language_, ConnectCallback_, PendingCallback_ );
		}
		qCDTOR( rRack_ );
		void Init(
			csdbns::sService Service,
			sclnjs::rCallback &ConnectCallback )
		{
			UpstreamSharing_.Init();
			Processing_.Init( UpstreamSharing_ );
			Callback_.Init( Processing_ );
			Server_.Init( Service, Callback_ );
			ConnectCallback_.Init();
			ConnectCallback_ = ConnectCallback;
			PendingCallback_ = NULL;
		}
		void PrepareCalling( void )
		{
			xdh_ups::js::rJS &JS = UpstreamSharing_.GetData().JS;

			if ( JS.Callback != NULL )
				PendingCallback_ = JS.Callback;

			PendingCallbackArguments_ = &JS.Arguments;
		}
		void LaunchCalling( void )
		{
			if ( PendingCallback_ == NULL ) {// First call, on a fresh connection; we use the 'ConnectCallback_'.
				XDH_.Init();
				ConnectCallback_.ObjectLaunch( XDH_, *PendingCallbackArguments_ );
				XDH_.Set( Id_, this );
			} else {
				PendingCallback_->VoidLaunch( *PendingCallbackArguments_ );	// As the callback are asynchronous, they never returns an value.
			}
		}
	};

	class rAsyncCallback_
	: public rRack_,
	  public cAsync_
	{
	protected:
		void SCLNJSWork( void ) override
		{
			rRack_::PrepareCalling();
		}
		sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			rRack_::LaunchCalling();

			return sclnjs::bRelaunch;
		}
	};
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
	rAsyncCallback_ *ARack = NULL;
qRB;
	tol::Init( Callback, Arguments );
	Caller.GetArgument( Callback, Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	ARack = new rAsyncCallback_;

	if ( ARack == NULL )
		qRAlc();

	ARack->Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );

	Callback.reset( false );	// To avoid the destruction of contained items, as their are now managed by 'Listening'.
	sclnjs::Launch( *ARack );
qRR
	if ( ARack != NULL )
		delete ARack;
qRT
qRE
}

namespace layout_ {
	namespace {
		void Set_ (
			const str::dString &Id,
			sclnjs::sCaller &Caller,
			const str::dString,
			flw::sWFlow &Flow )
		{
		qRH;
			str::wString XML, XSLFilename;
			sclnjs::rCallback Callback;
		qRB;
			XML.Init();
			treep::GetXML( Caller, XML );

			tol::Init( XSLFilename, Callback );
			Caller.GetArgument( XSLFilename, Callback );

			Set( Id, XML, XSLFilename, Language, Flow );
		qRR;
		qRT;
		qRE;
		}

	void SetElement_(
		server::fSet Set,
		sclnjs::sCaller &Caller,
		const char *Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		Caller.GetArgument( Id );

		SetElement_( Set, Id, Caller, Language, Flow );
	qRR;
	qRT;
	qRE;
	}

	rRack_ &GetRack_( sclnjs::sCaller &Caller )
	{
		rRack_ *Rack = NULL;
	qRH;
		sclnjs::rObject Object;
	qRB;
		Object.Init();
		Caller.GetArgument( Object );

		Rack = (rRack_ * )Object.Get( Id_ );

		if ( Rack == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Rack;
	}
}

#define RACK	rRack_ &Rack = GetRack_( Caller )

SCLNJS_F( xdhp::SetLayout )
{
	RACK;
	SetElement_( server::SetLayout, Caller, Rack.Language, Rack.Flow );
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