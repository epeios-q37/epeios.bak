/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'prxyq'.

    'prxyq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'prxyq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'prxyq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "csdbns.h"
#include "lstbch.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"prxyq"
# define NAME_LC			"prxyq"
# define NAME_UC			"PRXYQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	


namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	typedef fdr::ioflow_driver___<> _flow_driver___;

	typedef flw::standalone_ioflow__<> _flow__;

	class flow___
	: public _flow_driver___,
	  public _flow__
	{
	private:
		flx::size_embedded_iflow___ IFlow_;
		Q37_MRMDF( flw::oflow__, OF_, OFlow_ );
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum) override
		{
			return OF_().WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void ) override
		{
			OF_().Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return IFlow_.ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void ) override
		{
			IFlow_.Dismiss();
		}
public:
		void reset( bso::bool__ P = true )
		{
			_flow__::reset( P );
			_flow_driver___::reset( P );
			IFlow_.reset( P );
			OFlow_ = NULL;
		}
		E_CVDTOR( flow___ );
		void Init( void )
		{
			reset();

			_flow_driver___::Init( fdr::ts_Default );
			_flow__::Init( *this );

			// Other initializaiton will be made by other methods.
		}
		void SetIn( flw::iflow__ &Flow )
		{
			IFlow_.Init( Flow );
		}
		void SetOut( flw::oflow__ &Flow )
		{
			if ( OFlow_ != NULL )
				qRGnr();

			OFlow_ = &Flow;
		}
	};

	class proxy___
	{
	private:
		str::string Id_;
		flow___ Flow1_, Flow2_;
		mtx::handler___ Mutex_;
		void Process_(
			flw::iflow__ &IFlow,
			flw::oflow__ &OFlow )
		{
			fdr::byte__ Buffer[1024];
			fdr::size__ Size = 0;

			while ( ( Size = IFlow.ReadUpTo( sizeof( Buffer ), Buffer ) ) != 0) {
				while ( Size )
					Size -= OFlow.WriteUpTo( Buffer, Size );
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Id_.Init();
			Flow1_.reset( P );
			Flow2_.reset( P );

			if ( P )
				if ( Mutex_ != mtx::UndefinedHandler )
					mtx::Delete( Mutex_ );

			Mutex_ = mtx::UndefinedHandler;
		}
		E_CDTOR( proxy___ );
		void Init(
			const str::string_ &Id,
			flw::ioflow__ &Flow )
		{
			reset();

			Id_.Init( Id );
			Flow1_.Init();
			Flow2_.Init();

			Flow1_.SetIn( Flow );
			Flow2_.SetOut( Flow );

			Mutex_ = mtx::Create();

			mtx::Lock( Mutex_ );
			mtx::Lock( Mutex_ );	// Unlocked by 'Plug(...)'.
			mtx::Unlock( Mutex_ );

			Flow.Put( 0 );	// To report that the flow is plugged.

			Process_( Flow1_, Flow2_ );
		}
		void Plug( flw::ioflow__ &Flow )
		{
			Flow1_.SetOut( Flow );
			Flow2_.SetIn( Flow );

			mtx::Unlock( Mutex_ );

			Flow.Put( 0 );	// To report that the flow is plugged.

			Process_( Flow2_, Flow1_ );
		}
		const str::string_ &Id( void ) const
		{
			return Id_;
		}
	};
	

	typedef csdbns::callback__ _callback__;

	qROW( row__ );

	typedef lstbch::E_LBUNCHt_( proxy___ *, row__ ) proxies_;
	E_AUTO( proxies );

	typedef bch::E_BUNCH_( row__ ) singles_;
	E_AUTO( singles );

	proxies Proxies_;
	singles Singles_;

	mtx::handler___ Mutex_ = mtx::UndefinedHandler;

	void DeleteProxies_( void )
	{
		row__ Row = Proxies_.First();

		while ( Row != qNIL ) {
			delete Proxies_( Row );

			Row = Proxies_.Next( Row );
		}
	}

	void GetId_(
		flw::iflow__ &Flow,
		str::string_ &Id )
	{
		bso::char__ C = 0;

		while ( ( C = Flow.Get() ) != 0 )
			Id.Append( C );
	}

	row__ Search_( const str::string_ &Id )
	{
		row__ PRow = qNIL;
	qRH
		sdr::row__ Row = qNIL;
	qRB
		mtx::Lock( Mutex_ );

		Row = Singles_.First();

		while ( ( Row != qNIL) && ( Proxies_( Singles_( Row ) )->Id() != Id ) )
			Row = Singles_.Next( Row );

		if ( Row != qNIL ) {
			PRow = Singles_( Row );
			Singles_.Remove( Row );
		}
	qRR
	qRT
		mtx::Unlock( Mutex_ );
	qRE
		return PRow;
	}

	void Create_(
		const str::string_ &Id,
		flw::ioflow__ &Flow )
	{
	qRH
		proxy___ *Proxy = NULL;
	qRB
		mtx::Lock( Mutex_ );

		Proxy = new proxy___;

		if ( Proxy == NULL )
			qRAlc();

		Singles_.Append( Proxies_.Add( Proxy ) );

		mtx::Unlock( Mutex_ );

		Proxy->Init( Id, Flow );	// Blocking until deconnection.
	qRR
		if ( Proxy != NULL )
			delete Proxy;
	qRT
		if ( mtx::IsLocked( Mutex_ ) )
			mtx::Unlock( Mutex_ );
	qRE
	}

	void Plug_(
		row__ Row,
		flw::ioflow__ &Flow )
	{
	qRH
		proxy___ * Proxy = NULL;
	qRB
		mtx::Lock( Mutex_ );

		Proxy = Proxies_( Row );

		mtx::Unlock( Mutex_ );

		Proxy->Plug( Flow );	// Blocking until deconnection.
	qRR
	qRT
		if ( mtx::IsLocked( Mutex_ ) )
			mtx::Unlock( Mutex_ );
	qRE
	}


	class callback__
	: public _callback__
	{
	private:
	protected:
		void *CSDSCBPreProcess( const ntvstr::char__ *Origin ) override
		{
			return NULL;
		}
		csdscb::action__ CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP ) override
		{
		qRH
			str::string Id;
			row__ Row = qNIL;
		qRB
			Id.Init();
			GetId_( Flow, Id );

			Row = Search_( Id );

			if ( Row == qNIL ) 
				Create_( Id, Flow );
			else
				Plug_( Row, Flow );
		qRR
		qRT
		qRE
			return csdscb::aStop;
		}
		virtual void CSDSCBPostProcess( void *UP ) override
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			_callback__::Init();
		}
	};

	void Process_( void )
	{
	qRH
		csdbns::server___ Server;
		callback__ Callback;
	qRB
		Server.Init(sclmisc::MGetU16( registry::Service ), Callback );
	qRR
	qRT
	qRE
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()
		

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;

Q37_GCTOR( prxyq )
{
	Proxies_.Init();
	Singles_.Init();

	Mutex_ = mtx::Create();
}

Q37_GDTOR( prxyq )
{
	DeleteProxies_();

	if ( Mutex_ != mtx::UndefinedHandler )
		mtx::Delete( Mutex_ );
}

