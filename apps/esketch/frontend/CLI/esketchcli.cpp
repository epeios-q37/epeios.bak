/*
	Copyright (C) 2018 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'esketchcli'.

    'esketchcli' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'esketchcli' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'esketchcli'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "esketchcli.h"

#include "registry.h"

#include "frdfrntnd.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( esketchcli, NAME_LC, SKTINF_MC );

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
		class rRack_ {
		private:
			frdfrntnd::rKernel Kernel_;
			sclfrntnd::sReportingCallback ReportingCallback_;
			void Connect_( void )
			{
			qRH
				fblfrd::wIncompatibilityInformations IncompatibilityInformations;
			qRB
				IncompatibilityInformations.Init();
				if ( !Frontend.Connect( fblfrd::compatibility_informations__( SKTINF_LC, ESKETCH_API_VERSION ), IncompatibilityInformations ) )
					qRGnr();

			qRR
			qRE
			qRT
			}
		public:
			frdfrntnd::rFrontend Frontend;
			frdfrntnd::rMyObject Object;
			void reset( bso::sBool P = true )
			{
				tol::reset( P, ReportingCallback_, Kernel_, Frontend, Object  );
			}
			qCDTOR( rRack_ );
			void Init( void )
			{
			qRH
				sclfrntnd::rFeatures Features;
			qRB
				Features.Init();
				sclfrntnd::GuessBackendFeatures( Features );

				Kernel_.Init( Features, plgn::EmptyAbstracts );
				ReportingCallback_.Init();
				Frontend.Init( Kernel_, sclmisc::GetBaseLanguage(), ReportingCallback_ );
				Connect_();
				Object.Init( Frontend );
			qRR
			qRT
			qRE
			}
			void GetKernelFeatures(
				str::dString &Plugin,
				str::dString &PluginParameters )
			{
				return Kernel_.GetFeatures( Plugin, PluginParameters );
			}
		};
	}

	void AboutBackend_( void )
	{
	qRH
		str::wString ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
		rRack_ Rack;
	qRB
		Rack.Init();

		tol::Init( ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );
		Rack.GetKernelFeatures( Plugin, PluginParameters );
		Rack.Frontend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

		cio::COut << Plugin << " '" << PluginParameters << "' : " << txf::nl << BackendInformations << txf::nl;
	qRR
	qRT
	qRE
	}


	void Test_( void )
	{
	qRH
		rRack_ Rack;
	qRB
		Rack.Init();

		Rack.Object.TestMessage();
	qRR
	qRE
	qRT
	}

	void ToUpper_( void )
	{
	qRH
		rRack_ Rack;
		str::wString Pattern;
	qRB
		Rack.Init();

		Pattern.Init();
		sclmisc::MGetValue( registry::parameter::Pattern, Pattern );
		Rack.Object.ToUpper( Pattern );

		cio::COut << Pattern << txf::nl;
	qRR
	qRE
	qRT
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return esketchcli::Info;
}

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
	C( AboutBackend );
	C( Test );
	C( ToUpper );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}
