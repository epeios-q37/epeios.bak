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

#include "server.h"

#include "csdcmn.h"
#include "csdbns.h"

#include "sclargmnt.h"
#include "sclmisc.h"

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

namespace action_ {
	qCDEF( char *, Id, "_q37ActionRack" );

	class rRack {
	private:
	public:
		sck::rRWFlow Flow;
		sclnjs::rObject Object;
		qCBUFFERr Language;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow, Language );
		}
		qCDTOR( rRack );
		void Init(
			sck::sSocket Socket,
			sclnjs::rCallback &Callback )
		{
		qRH;
			str::wString Language;
		qRB;
			Flow.Init( Socket, true );

			Language.Init();
			server::Handshake( Flow, Language );

			Language.Convert( this->Language );

			Object.Init();
			Callback.ObjectLaunch( Object );

			Object.Set( Id, this );
		qRR;
		qRT;
		qRE;
		}
		void GetAction(
			str::dString &Id,
			str::dString &Action )
		{
			server::GetAction( Flow, Id, Action );
		}
	};

	class rAsyncCallback
	: public rRack,
	  public cAsync_
	{
	private:
		str::wString Id_, Action_;
	protected:
		void SCLNJSWork( void ) override
		{
			tol::Init( Id_, Action_ );
			rRack::GetAction( Id_, Action_ );
		}
		sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			n4njs::cUCallback *Callback = Get_( Action_ );
			
			if ( Callback != NULL )
				sclnjs::VoidLaunch( *Callback, Object, Id_ );

			// TODO : report not found callback.

			return sclnjs::bRelaunch;
		}
	};
}

namespace listening_ {
	class rRack {
	private:
		qRMV( sclnjs::rCallback, C_, Callback_ );
		csdbns::rListener Listener_;
		sck::sSocket Socket_;
		qCBUFFERr Language_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Callback_ != NULL )
					delete Callback_;
			} 

			tol::reset( P, Listener_, Callback_, Language_ );
			Socket_ = sck::Undefined;
		}
		qCDTOR( rRack );
		void Init(
			csdbns::sService Service,
			sclnjs::rCallback *Callback )
		{
			Callback_ = Callback;
			Listener_.Init( Service );
			Socket_ = sck::Undefined;
		}
		void GetConnection( void )
		{
			const char *IP = NULL;
			Socket_ = Listener_.GetConnection( IP );
		}
		void HandleConnection( void )
		{
		qRH;
			action_::rAsyncCallback *Action = NULL;
		qRB;
			Action = new action_::rAsyncCallback;

			if ( Action == NULL )
				qRAlc();

			Action->Init( Socket_, C_() );

			sclnjs::Launch( *Action );
		qRR;
			if ( Action != NULL )
				delete Action;
		qRT;
		qRE;
		}
	};

	class rAsyncCallback
	: public rRack,
	  public cAsync_
	{
	protected:
		void SCLNJSWork( void ) override
		{
			rRack::GetConnection();
		}
		sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			rRack::HandleConnection();

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
	sclnjs::rCallback *Callback = NULL;
	str::wString Arguments;
	listening_::rAsyncCallback *Listening = NULL;
qRB;
	Callback = new sclnjs::rCallback;

	if ( Callback == NULL )
		qRGnr();

	tol::Init( *Callback, Arguments );
	Caller.GetArgument( *Callback, Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Listening = new listening_::rAsyncCallback;

	if ( Listening == NULL )
		qRAlc();

	Listening->Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );
	sclnjs::Launch( *Listening );
qRR
	if ( Listening != NULL )
		delete Listening;

	if ( Callback != NULL )
		delete Callback;
qRT
qRE
}

namespace {
	void SetElement_(
		server::fSet Set,
		const str::dString &Id,
		sclnjs::sCaller &Caller,
		const char *Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString XML, XSLFilename;
	qRB;
		tol::Init( XML, XSLFilename );

		treep::GetXML( Caller, XML );
		Caller.GetArgument( XSLFilename );

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

	action_::rRack &GetActionRack_( sclnjs::sCaller &Caller )
	{
		action_::rRack *Rack = NULL;
	qRH;
		sclnjs::rObject Object;
	qRB;
		Object.Init();
		Caller.GetArgument( Object );

		Rack = (action_::rRack * )Object.Get( action_::Id );

		if ( Rack == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Rack;
	}
}

#define RACK	action_::rRack &Rack = GetActionRack_( Caller )

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

SCLNJS_F( xdhp::SetContents )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	RACK;

	tol::Init( Ids, Contents );
	Caller.GetArgument( Ids, Contents );

	server::SetContents( Ids, Contents, Rack.Flow );
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

qGCTOR( xdhp )
{
	Automat_.Init();
	Callbacks_.Init();
}