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

#include "registry.h"
#include "session.h"

#include "prtcl.h"

#include "logq.h"
#include "idsq.h"

#include "csdmnc.h"
#include "csdcmn.h"
#include "csdbns.h"

#include "sclmisc.h"
#include "sclxdhtml.h"

SCLI_DEF( xdhqxdh, PROGRAM_NAME, SOFTWARE_NAME );

using namespace xdhqxdh;

static const char *Launcher_ = NULL;

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
# define FUNCTION_SPEC
#endif

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'scllocale.cpp', d'o le 'namespace'.
namespace {
    namespace {
        void Initialization_( xdhcmn::eMode Mode )
        {
        qRH;
            qCBUFFERr Buffer;
            str::wString HostService;
        qRB;
            HostService.Init();

            if ( sclmisc::OGetValue( registry::parameter::HostService, HostService ) )
                session::Core.Init( HostService.Convert( Buffer ), 0, sck::NoTimeout );

            faaspool::Initialize();
            session::LogDriver.Init( cio::COut );
        qRR;
        qRT;
        qRE;
        }

        xdhcmn::cSession *RetrieveSession_(void)
        {
            session::rSession *Session = NULL;
        qRH;
        qRB;
            if ( ( Session = new session::rSession ) == NULL )
                qRAlc();

            if ( !Session->Init() ) {
                delete Session;
                Session = NULL;
            }

            // WARNING ! In 'MultiUser' mode, 'UpstreamCallback' is not correctly set yet!
        /*	if ( Mode == xdhcmn::mMonoUser ) {
                Session->Launch( "", "" );
            }
        */
        qRR;
            if ( Session != NULL )
                delete Session;

            Session = NULL;
        qRT;
        qRE;
            return Session;
        }

        void ReleaseSession_( xdhcmn::cSession *Session )
        {
            if ( Session == NULL )
                qRGnr();

            delete Session;
        }
    }

	typedef xdhcmn::cDownstream cDownstream_;

	class sDownstream
	: public cDownstream_
	{
	protected:
		virtual void XDHCMNInitialize( const xdhcmn::shared_data__ &Data ) override
		{
			if ( Launcher_ != NULL )
				qRFwk();

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.SCLRack(), Data.Localization(), xdhqxdh::Info );

				Initialization_( Data.Mode() );
			}
		}
		virtual void XDHCMNBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclmisc::GetBaseLanguage();

			if ( Language == NULL )
				qRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcmn::cSession *XDHCMNRetrieveSession(void) override
		{
			return RetrieveSession_();
		}
		virtual void XDHCMNReleaseSession( xdhcmn::cSession *Session ) override
		{
			return ReleaseSession_( Session );
		}
		const scli::sInfo &XDHCMNGetInfo( void ) override
		{
			return xdhqxdh::Info;
		}
		bso::sBool XDHCMNGetHead(
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

DEF( XDHCMN_RETRIEVE_FUNCTION_NAME, xdhcmn::retrieve );

xdhcmn::cDownstream *XDHCMNRetrieve( void )
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

