/*
	Copyright (C) 2016 Claude SIMON (http://q37.info/contact/).

	This file is part of the embedded plugin.

    The embedded plugin is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The embedded plugin is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with the embedded plugin.  If not, see <http://www.gnu.org/licenses/>.
*/

// 'embedded' plugin.

#include "embedded.h"

#include "registry.h"

#include "rpembedded.h"

#include "sclmisc.h"
#include "sclplugin.h"
#include "sclmisc.h"

#include "fblovl.h"
#include "csdlec.h"

SCLI_DEF( embedded, NAME_LC, NAME_MC );

const scli::sInfo &sclplugin::SCLPLUGINInfo( void )
{
	return embedded::Info;
}

typedef fblovl::cDriver cPlugin_;

class rPlugin
: public cPlugin_
{
private:
	csdlec::library_embedded_client_core__ Core_;
	bso::sBool Connected_;
	void HandleLostConnection_( void )
	{
		Connected_ = false;
		ERRRst();
	}
protected:
	virtual fdr::ioflow_driver_base___ *CSDRCCNew( void ) override
	{
		csdlec::rDriver *Driver = new csdlec::rDriver;

		if ( Driver == NULL )
			qRAlc();

		Driver->Init( Core_.GetCallback() );

		return Driver;
	}
	virtual void CSDRCCDelete( fdr::ioflow_driver_base___ *Driver ) override
	{
		delete Driver;
	}
	virtual fblovl::eMode FBLOVLMode( void ) override
	{
		return fblovl::mReferenced;
	}
public:
	void reset( bso::bool__ P = true )
	{
		Core_.reset( P );
		Connected_ = false;
	}
	qCVDTOR( rPlugin );
	bso::bool__ Init(
		const char *BackendFilename,
		csdleo::context__ Context )
	{
		bso::sBool Success = false;
	qRH
		csdlec::library_data__ Data;
		sclmisc::sRack SCLRack;
	qRB
		SCLRack.Init();
		Data.Init( Context, BackendFilename, &SCLRack );

		if ( Core_.Init( BackendFilename, Data, qRPU ) ) {
            Success = true;
            Connected_ = true;
        }
	qRR
	qRT
	qRE
		return Success;
	}
	bso::sBool SCLPLUGINInitialize( plgncore::sAbstract *BaseAbstract )
	{
		bso::sBool Success = false;
	qRH
		str::string BackendFilename;
		TOL_CBUFFER___ Buffer;
		rpembedded::rAbstract *Abstract = (rpembedded::rAbstract *)BaseAbstract;
	qRB
		plgn::Test( BaseAbstract, rpembedded::Identifier );

		BackendFilename.Init();

		sclmisc::MGetValue( registry::parameter::BackendFilename, BackendFilename );

		if ( !Init( BackendFilename.Convert(Buffer), csdleo::cRegular ) )
			switch ( plgn::ErrorReporting( Abstract ) ) {
			case plgn::rhInternally:
				sclmisc::ReportAndAbort( "UnableToLoad", BackendFilename );
				break;
			case plgn::rhDetailed:
				Abstract->BackendFilename = BackendFilename;
			case plgn::rhBasic:
				break;
			default:
				qRGnr();
				break;
			}
		else
            Success = true;
	qRR
	qRT
	qRE
		return Success;
	}
};

SCLPLUGIN_DEF( rPlugin );

void sclplugin::SCLPLUGINPluginIdentifier( str::dString &Identifier )
{
	Identifier.Append( IDENTIFIER );
}

void sclplugin::SCLPLUGINPluginDetails( str::dString &Details )
{
	Details.Append( PLUGIN_NAME " V" VERSION " - Build " __DATE__ " " __TIME__ " (" );
	Details.Append( cpe::GetDescription() );
	Details.Append( ')' );
}

qGCTOR(embedded)
{
	if ( strcmp( rpembedded::Identifier, IDENTIFIER ) )
		qRChk();
}

