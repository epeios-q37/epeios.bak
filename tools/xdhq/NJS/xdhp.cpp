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

namespace {
	class rListeningRack_ {
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
		qCDTOR( rListeningRack_ );
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
		qRB;
			ActionCallback = new rActionAsyncCallback_;

			if ( ActionCallback == NULL )
				qRAlc();

			ActionCallback->Init( Socket_ );

			sclnjs::Launch( *ActionCallback );

			C_().Launch();
		qRR;
			if ( ActionCallback != NULL )
				delete ActionCallback;
		qRT;
		qRE;
		}
	};

	class rListeningAsyncCallback_
	: public rListeningRack_,
	  public cAsync_
	{
	protected:
		void SCLNJSWork( void ) override
		{
			rListeningRack_::GetConnection();
		}
		sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			rListeningRack_::HandleConnection();

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
	rListeningAsyncCallback_ *ListeningCallback = NULL;
qRB;
	Callback = new sclnjs::rCallback;

	if ( Callback == NULL )
		qRGnr();

	tol::Init( *Callback, Arguments );
	Caller.GetArgument( *Callback, Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	ListeningCallback = new rListeningAsyncCallback_;

	if ( ListeningCallback == NULL )
		qRAlc();

	ListeningCallback->Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );
	sclnjs::Launch( *ListeningCallback );
qRR
	if ( ListeningCallback != NULL )
		delete ListeningCallback;

	if ( Callback != NULL )
		delete Callback;
qRT
qRE
}

namespace {
	class rActionRack_ {
	private:
		sck::rRWFlow Flow_;
		qCBUFFERr Language_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow_, Language_ );
		}
		qCDTOR( rActionRack_ );
		void Init( sck::sSocket Socket )
		{
			qRH;
			str::wString Language;
			qRB;
			Flow_.Init( Socket, true );

			Language.Init();
			server::Handshake( Flow_, Language );

			Language.Convert( Language_ );
			qRR;
			qRT;
			qRE;
		}
		void GetAction(
			str::dString &Id,
			str::dString &Action )
		{
			server::GetAction( Flow_, Id, Action );
		}
	};

	typedef sclnjs::cAsync cAsync_;

	class rActionAsyncCallback_
		: public rActionRack_,
		public cAsync_ {
	private:
		str::wString Id_, Action_;
	protected:
		void SCLNJSWork( void ) override
		{
			tol::Init( Id_, Action_ );
			rActionRack_::GetAction( Id_, Action_ );
		}
		sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			Get_( Action_ )->Launch( Id_ );

			return sclnjs::bRelaunch;
		}
	};
}



SCLNJS_F( xdhp::New )
{}


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
}


SCLNJS_F( SetLayout )
{
	SetElement_( server::SetElementLayout, Caller, "en", )
}


qGCTOR( xdhp )
{
	Automat_.Init();
	Callbacks_.Init();
}