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

	lstbch::qLBUNCHwl( sclnjs::rCallback * ) Callbacks_;
	stsfsm::wAutomat Automat_;

	void Add_(
		const str::dString &Tag,
		sclnjs::rCallback *Callback )
	{
		if ( stsfsm::Add( Tag, *Callbacks_.Add( Callback ), Automat_ ) != stsfsm::UndefinedId )
			qRGnr();
	}

	sclnjs::rCallback *Get_( const str::dString &Tag )
	{
		sdr::sRow Row = stsfsm::GetId( Tag, Automat_ );

		if ( Row == qNIL )
			qRGnr();

		return Callbacks_( Row );
	}
}

namespace action_ {
	qCDEF( char *, Id, "_q37ActionRack" );

	class rRack {
	private:
		qCBUFFERr Language_;
	public:
		sck::rRWFlow Flow;
		sclnjs::rObject Object;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow, Language_ );
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

			Language.Convert( Language_ );

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
			Get_( Action_ )->VoidLaunch( Object,  Id_ );

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
	str::wString Tag;
	sclnjs::rCallback *Callback = NULL;
qRB;
	Callback = new sclnjs::rCallback;

	if ( Callback == NULL )
		qRGnr();

	tol::Init( Tag, *Callback );
	Caller.GetArgument( Tag, *Callback );

	Add_( Tag, Callback );
qRR
	if ( Callback != NULL )
		delete Callback;
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

	action_::rRack GetActionRack_( sclnjs::sCaller &Caller )
	{
		action_::rRack *Rack = NULL;
	qRH;
		sclnjs::rObject Object;
	qRB;
		Object.Init();
		Caller.GetArgument( Object );

		Rack = ( action_::rRack * )Object.Get( action_::Id );

		if ( Rack == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Rack;
	}
}

SCLNJS_F( xdhp::SetLayout )
{
	SetElement_( server::SetElementLayout, Caller, "en", GetActionRack_( Caller ).Flow );
}

qGCTOR( xdhp )
{
	Automat_.Init();
	Callbacks_.Init();
}