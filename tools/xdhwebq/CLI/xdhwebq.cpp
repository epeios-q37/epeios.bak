/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhwebq.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "xdwmain.h"

#include "csdbns.h"
#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "websck.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( xdhwebq, NAME_LC, NAME_MC );

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

namespace {
	namespace {
		typedef csdbns::cProcessing cProcessing_;

		qENUM( State_ ) {
            sProlog,
            sRegular,
            s_amount,
            s_Undefined
		};

		class sProcessing
		: public cProcessing_
		{
		private:
			qRMV( xdwmain::rAgent, A_, Agent_ );
			eState_ State_;
			str::wString Token_;
		protected:
			void *CSDSCBPreProcess(
				fdr::rRWDriver *Driver,
				const ntvstr::char__ *Origin ) override
			{
			qRH
                flw::rDressedRWFlow<> Flow;
                websck::wHeader Header;
			qRB
                Fields.Init();

                if ( websck::Handshake(*Driver, Fields) ) {
                    State_ = sRegular;
                } else if ( Header.FirstLine == "FaaS Prolog" ) {
                    State_ = sProlog;
                } else {
                    State_ = s_Undefined;
                }
			qRR
			qRT
			qRE
				return NULL;
			}
			csdscb::action__ CSDSCBProcess(
				fdr::rRWDriver *Driver,
				void *UP ) override
			{
			qRH
                xdwmain::rSession Session;
                str::wString Langage, Head;
                qCBUFFERr Buffer;
                flw::rDressedRWFlow<> Flow;
			qRB
                Flow.Init(*Driver);

                switch ( State_ ) {
                case sProlog:
                    Head.Init();
                    A_().Head(&Token_, Head);
                    Flow.Write(Head.Convert(Buffer), Head.Amount());
                    Flow.Commit();
                    break;
                case sRegular:
                     Session.Init(A_(), Langage.Convert(Buffer), Token_);
                     qRVct();
                     break;
                default:
                    qRGnr();
                    break;
                }
			qRR
			qRT
			qRE
				return csdscb::aStop;
			}
			virtual bso::sBool CSDSCBPostProcess( void *UP ) override
			{
				return true;
			}
		public:
			void reset( bso::bool__ P = true )
			{
				Agent_ = NULL;
				State_ = s_Undefined;
				Token_.reset(P);
			}
			E_CVDTOR( sProcessing );
			void Init( xdwmain::rAgent &Agent )
			{
				Agent_ = &Agent;
				State_ = s_Undefined;
				Token_.Init();
			}
		};

	}

	void Process_( void )
	{
	qRH
		xdwmain::rAgent Agent;
		str::wString Identification, ModuleFilename;
		TOL_CBUFFER___ IdentificationBuffer;
		csdbns::server___ Server;
		sProcessing Callback;
		qCBUFFERr Buffer;
	qRB
		Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
		Identification.Append( cpe::GetDescription() );

		ModuleFilename.Init();
		sclmisc::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

		Agent.Init( xdhcmn::mMultiUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( Buffer ) );

		Callback.Init( Agent );

		Server.Init( sclmisc::MGetU16( registry::parameter::Service ), Callback );
		Server.Process( NULL );
	qRR
	qRT
	qRE
	}
}

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return xdhwebq::Info;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
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
