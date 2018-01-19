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

#include "sclargmnt.h"

#include "csdmns.h"
#include "csdcmn.h"
#include "mtk.h"

using namespace xdhp;

namespace {

	class rComm_	// ...unication.
	{
	private:
		mtx::rHandler Read_, Write_;
		str::wString Language_;
		fdr::rRWDriver *Driver_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Read_ != mtx::UndefinedHandler )
					mtx::Delete( Read_ );

				if ( Write_ != mtx::UndefinedHandler )
					mtx::Delete( Write_ );
			}

			tol::reset( P, Language_, Driver_ );
			Read_ = Write_ = mtx::UndefinedHandler;
		}
		qCDTOR( rComm_ );
		void Init( const str::dString &Language )
		{
			reset();

			Read_ = mtx::Create();
			Write_ = mtx::Create();

			mtx::Lock( Read_ );

			Language_.Init( Language );
			Driver_ = NULL;
		}
		qRODISCLOSEr( str::dString, Language );
		void Write( fdr::rRWDriver *Driver )	// If 'Driver' == NULL, it only waits for dismissal.
		{
			mtx::Lock( Write_ );

			if ( ( Driver_ != NULL ) && ( Driver != NULL ) )
				qRGnr();

			Driver_ = Driver;

			if ( Driver == NULL )
				mtx::Unlock( Write_ );
			else
				mtx::Unlock( Read_ );
		}
		fdr::rRWDriver *Read( void )
		{
			mtx::Lock( Read_ );

			if ( Driver_ == NULL )
				qRGnr();

			return Driver_;
		}
		void Dismiss( void )
		{
			mtx::Unlock( Write_ );
		}
	};

	class rConn_	// ...exion.
	{
	private:
		mtx::rHandler Read_, Write_;
		rComm_ *Comm_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Read_ != mtx::UndefinedHandler )
					mtx::Delete( Read_ );

				if ( Write_ != mtx::UndefinedHandler )
					mtx::Delete( Write_ );
			}

			Read_ = Write_ = mtx::UndefinedHandler;
			Comm_ = NULL;
		}
		qCDTOR( rConn_ );
		void Init( void )
		{
			reset();

			Read_ = mtx::Create();
			Write_ = mtx::Create();

			mtx::Lock( Read_ );

			Comm_ = NULL;
		}
		void Write( rComm_ *Comm )
		{
			mtx::Lock( Write_ );

			if ( Comm_ != NULL )
				qRGnr();

			Comm_ = Comm;

			mtx::Unlock( Read_ );
		}
		rComm_ *Read( void )
		{
			rComm_ *Comm = NULL;

			mtx::Lock( Read_ );

			if ( Comm_ == NULL )
				qRGnr();

			Comm = Comm_;

			mtx::Unlock( Write_ );

			return Comm;
		}
	} Conn_;

	typedef csdmns::cProcessing cProcessing_;

	class sProcessing
	: public cProcessing_
	{
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *IODriver,
			const ntvstr::char__ *Origin ) override
		{
			rComm_ *Comm = NULL;
		qRH;
			flw::sDressedRWFlow<> Flow;
			str::wString Language;
		qRB;
			Flow.Init( *IODriver );
			Language.Init();
			server::Handshake( Flow, Language );

			Comm = new rComm_;

			if ( Comm == NULL )
				qRAlc();

			Comm->Init( Language );

			prtcl::PutAnswer( prtcl::aOK_1, Flow );
			Flow.Commit();

			Conn_.Write( Comm );
		qRR;
			if ( Comm != NULL )
				delete Comm;
		qRT;
		qRE;
			return Comm;
		}
		virtual csdscb::eAction CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override
		{
			if ( UP == NULL )
				qRGnr();

			rComm_ &Comm = *(rComm_ *)UP;

			Comm.Write( IODriver );

			Comm.Write( NULL );	// Wait for dismissal.

			return csdscb::aContinue;
		}
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override
		{
			if ( UP == NULL )
				qRGnr();

			delete (rComm_ *)UP;

			return true;
		}
	public:
		void Init( void )
		{}
	} Processing_;

	csdmns::rServer Server_;

	void Process_( void * )
	{
		Server_.Process();
	}
}

SCLZND_F( xdhp::Listen )
{
qRH;
	str::wString Arguments;
qRB;
	Arguments.Init();
	Caller.Get( Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Processing_.Init();
	Conn_.Init();

	Server_.Init( sclmisc::MGetU16( registry::parameter::Service ), Processing_ );

	mtk::RawLaunch( Process_, NULL );
qRR;
qRT;
qRE;
}

namespace {
	namespace {
		typedef fdr::rRWDressedDriver rRWDriver_;

		class rDriver_
		: public rRWDriver_
		{
		private:
			qRMV( rComm_, C_, Comm_ );
			qRMV( fdr::rRWDriver, D_, Driver_ );
		protected:
			virtual fdr::sSize FDRRead(
				fdr::sSize Maximum,
				fdr::sByte *Buffer ) override
			{
				if ( Driver_ == NULL ) {
					Driver_ = C_().Read();
				}

				D_().ITake( tht::GetTID() );
				return D_().Read( Maximum, Buffer, fdr::bNonBlocking );
			}
			virtual void FDRDismiss( bso::sBool Unlock ) override
			{
				D_().Dismiss( Unlock );
			}
			virtual tht::sTID FDRITake( fdr::sTID Owner ) override
			{
				return D_().ITake( Owner );
			}
			virtual fdr::sSize FDRWrite(
				const fdr::sByte *Buffer,
				fdr::sSize Maximum ) override
			{
				if ( Driver_ == NULL ) {
					Driver_ = C_().Read();
				}

				D_().OTake( tht::GetTID() );
				return D_().Write( Buffer, Maximum );
			}
			virtual void FDRCommit( bso::sBool Unlock ) override
			{
				D_().Commit( Unlock );
				C_().Dismiss();
				Driver_ = NULL;
			}
			virtual tht::sTID FDROTake( tht::sTID Owner ) override
			{
				return D_().OTake( Owner );
			}
		public:
			void reset( bso::sBool P = true )
			{
				rRWDriver_::reset( P );

				tol::reset( P, Comm_, Driver_ );
			}
			qCVDTOR( rDriver_ );
			void Init( rComm_ *Comm )
			{
				rRWDriver_::Init( fdr::ts_Default );

				Driver_ = NULL;
				Comm_ = Comm;
			}
			const str::dString &Language( void ) const
			{
				return C_().Language();
			}
		};
	}
	struct rRack_ {
	private:
		rDriver_ Driver_;
	public:
		flw::sDressedRWFlow<> Flow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Driver_, Flow );
		}
		qCDTOR( rRack_ );
		void Init( rComm_ *Comm )
		{
			Driver_.Init( Comm );
			Flow.Init( Driver_ );

		}
		const str::dString &Language( void ) const
		{
			return Driver_.Language();
		}
	};
}

SCLZND_F( xdhp::New )
{
qRH;
	rRack_ *Rack = NULL;
qRB;
	if ( (Rack = new rRack_) == NULL )
		qRAlc();

	Rack->Init( Conn_.Read() );

	Caller.SetReturnValue( (bso::sS64)Rack );
qRR;
	if ( Rack != NULL )
		delete Rack;
qRT;
qRE;
}

SCLZND_F( xdhp::Delete )
{
	bso::sS64 Long = 0;

	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	delete (rRack_ * )Long;
}

namespace {
	rRack_ &GetRack_( sclznd::sCaller &Caller )
	{
		bso::sS64 Long = 0;

		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		return *(rRack_ *)Long;
	}

	flw::sRWFlow &GetFlow_( sclznd::sCaller &Caller )
	{
		return GetRack_( Caller ).Flow;
	}
}

#define RACK	rRack_ &Rack = GetRack_( Caller )
#define FLOW	flw::sRWFlow &Flow = GetFlow_( Caller )

SCLZND_F( xdhp::GetAction )
{
qRH;
	str::wString Id, Action;
	str::wStrings Strings;
qRB;
	FLOW;

	tol::Init( Id, Action );
	server::GetAction( Flow, Id, Action );

	Strings.Init();

	Strings.Append( Id );
	Strings.Append( Action );

	Caller.SetReturnValue( Strings );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::Alert )
{
qRH;
	str::wString Message;
qRB;
	FLOW;

	Message.Init();
	Caller.Get( Message );

	server::Alert( Message, Flow );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::Confirm )
{
	bso::sBool Return = false;
qRH;
	str::wString Message, Response;
qRB;
	FLOW;

	Message.Init();
	Caller.Get( Message );

	Response.Init();
	server::Confirm( Message, Flow, Response );

	Caller.SetReturnValue( Response == "true" );
qRR;
qRT;
qRE;
}

namespace {
	void SetLayout_(
		const str::dString &Id,
		sclznd::sCaller &Caller,
		const str::dString &Language,
		flw::sRWFlow &Flow )
	{
	qRH;
		str::wString XML, XSLFilename;
	qRB;
		tol::Init( XML, XSLFilename );

		treep::GetXML( Caller, XML );
		Caller.Get( XSLFilename );

		server::layout::Set( Id, XML, XSLFilename, Language, Flow );
	qRR;
	qRT;
	qRE;
	}

	void SetLayout_(
		sclznd::sCaller &Caller,
		const str::dString &Language,
		flw::sRWFlow &Flow )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		Caller.Get( Id );

		SetLayout_( Id, Caller, Language, Flow );
	qRR;
	qRT;
	qRE;
	}
}

SCLZND_F( xdhp::SetLayout )
{
	RACK;

	SetLayout_( Caller, Rack.Language(), Rack.Flow );
}

SCLZND_F( xdhp::GetContents )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	FLOW;

	Ids.Init();
	Caller.Get( Ids );

	Contents.Init();
	server::contents::Get( Ids, Flow, Contents );

	Caller.SetReturnValue( Contents );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::SetContents )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	FLOW;

	tol::Init( Ids, Contents );
	Caller.Get( Ids, Contents );

	server::contents::Set( Ids, Contents, Flow );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::DressWidgets )
{
qRH;
	str::wString Id;
qRB;
	FLOW;

	tol::Init( Id );
	Caller.Get( Id );

	server::widgets::Dress( Id, Flow );
qRR;
qRT;
qRE;
}

namespace {
	void HandleClasses_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dStrings &Ids,
			const str::dStrings &Classes,
			flw::sRWFlow &Flow) )
	{
	qRH;
		str::wStrings Ids, Classes;
	qRB;
		FLOW;

		tol::Init( Ids, Classes );
		Caller.Get( Ids, Classes );

		Function( Ids, Classes, Flow );
	qRR;
	qRT;
	qRE;
	}
}

SCLZND_F( xdhp::AddClasses )
{
	HandleClasses_( Caller, server::classes::Add );
}

SCLZND_F( xdhp::RemoveClasses )
{
	HandleClasses_( Caller, server::classes::Remove );
}

SCLZND_F( xdhp::ToggleClasses )
{
	HandleClasses_( Caller, server::classes::Toggle );
}

namespace {
	void HandleElements_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dStrings &Ids,
			flw::sRWFlow &Flow) )
	{
	qRH;
		str::wStrings Ids;
	qRB;
		FLOW;

		tol::Init( Ids );
		Caller.Get( Ids );

		Function( Ids, Flow );
	qRR;
	qRT;
	qRE;
	}
}

SCLZND_F( xdhp::EnableElements )
{
	HandleElements_( Caller, server::elements::Enable );
}

SCLZND_F( xdhp::DisableElements )
{
	HandleElements_( Caller, server::elements::Disable );
}

namespace {
	void Get_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value ) )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		FLOW;

		tol::Init( Id, Name );
		Caller.Get( Id, Name );

		tol::Init( Value );
		Function( Id, Name, Flow, Value );

		Caller.SetReturnValue( Value );
	qRR;
	qRT;
	qRE;
	}

	void Set_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dString &Id,
			const str::dString &Name,
			const str::dString &Value,
			flw::sRWFlow &Flow ) )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		FLOW;

		tol::Init( Id, Name, Value );
		Caller.Get( Id, Name, Value );

		Function( Id, Name, Value, Flow );
	qRR;
	qRT;
	qRE;
	}

}

SCLZND_F( xdhp::GetAttribute )
{
	Get_( Caller, server::attribute::Get );
}

SCLZND_F( xdhp::SetAttribute )
{
	Set_( Caller, server::attribute::Set );
}

SCLZND_F( xdhp::GetProperty )
{
	Get_( Caller, server::property::Get );
}

SCLZND_F( xdhp::SetProperty )
{
	Set_( Caller, server::property::Set );
}


