/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

	'XDHq' is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'XDHq' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhqxdh.h"

#include "common.h"
#include "registry.h"
#include "session.h"

#include "logq.h"
#include "idsq.h"

#include "csdmnc.h"
#include "csdcmn.h"
#include "csdbns.h"

#include "sclm.h"

SCLI_DEF( xdhqxdh, PROGRAM_NAME, SOFTWARE_NAME );

using namespace xdhqxdh;

static const char *Launcher_ = NULL;

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
# define FUNCTION_SPEC
#endif

namespace {
	namespace {
		void Initialization_( xdhcdc::eMode Mode )
		{
		qRH;
			qCBUFFERh Buffer;
			str::wString HostService;
			str::wString Translation;
		qRB;
			HostService.Init();

			if ( sclm::OGetValue( registry::parameter::HostService, HostService ) && HostService.Amount() ) {
				Translation.Init();
				sclm::GetBaseTranslation("ConnectingTo", Translation, HostService);
				cio::COut << Translation << txf::commit << txf::nl;
				if ( session::Core.Init( HostService.Convert( Buffer ), 0, sck::NoTimeout ) ) {
					Translation.Init();
					sclm::GetBaseTranslation("ConnectedTo", Translation, HostService);
					cio::COut << Translation << txf::commit << txf::nl;
				} else
					sclm::ReportAndAbort("UnableToConnectTo", HostService);
			}

			faaspool::Initialize();
			session::LogDriver.Init( cio::COut );
		qRR;
		qRT;
		qRE;
		}

		xdhcdc::cSingle *FetchCallback_(void)
		{
			session::rSession *Callback = NULL;
		qRH;
		qRB;
			if ( ( Callback = new session::rSession ) == NULL )
				qRAlc();

			if ( !Callback->Init()) {
				delete Callback;
				Callback = NULL;
			}

			// WARNING ! In 'MultiUser' mode, 'UpstreamCallback' is not correctly set yet!
			/*	if ( Mode == xdhcmn::mMonoUser ) {
			        Callback->Launch( "", "" );
			    }
			*/
		qRR;
			if ( Callback != NULL )
				delete Callback;

			Callback = NULL;
		qRT;
		qRE;
			return Callback;
		}

		void DismissCallback_( xdhcdc::cSingle *Callback )
		{
			if ( Callback == NULL )
				qRGnr();

			delete Callback;
		}
	}

	typedef xdhcdc::cGlobal cDownstream_;

	class sDownstream
	: public cDownstream_
	{
	protected:
		virtual void XDHCDCInitialize(
			const xdhcdc::sData &Data,
			xdhcuc::cGlobal &Callback) override
		{
			if ( Launcher_ != NULL )
				qRFwk();

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclm::Initialize( Data.SCLRack(), Data.Localization(), xdhqxdh::Info );

				Initialization_( Data.Mode() );
				common::SetCallback(Callback);
			}
		}
		virtual void XDHCDCBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclm::GetBaseLanguage();

			if ( Language == NULL )
				qRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcdc::cSingle *XDHCDCFetchCallback(faaspool::sId) override
		{
			return FetchCallback_();
		}
		virtual void XDHCDCDismissCallback(xdhcdc::cSingle *Callback) override
		{
			return DismissCallback_(Callback);
		}
		const scli::sInfo &XDHCDCGetInfo( void ) override
		{
			return xdhqxdh::Info;
		}
		bso::sBool XDHCDCGetHead(
			const str::dString &Token,
			str::dString &Head,
			qRPN ) override
		{
			return faaspool::GetHead(Token, Head);
		}
	public:
		void reset( bso::bool__ P = true )
		{}
		E_CVDTOR( sDownstream );
		void Init( void )
		{}
	};
}

static inline void DoNothing_( void )
{}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XDHCDC_RETRIEVE_FUNCTION_NAME, xdhcdc::retrieve );

xdhcdc::cGlobal *XDHCDCRetrieve( void )
{
	sDownstream *Callback = NULL;
qRFH
qRFB
	Callback = new sDownstream;

	if ( Callback == NULL )
		qRAlc();

	Callback->Init();
qRFR
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
qRFT
qRFE(DoNothing_())
	return Callback;
}

