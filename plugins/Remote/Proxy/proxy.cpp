/*
	Copyright (C) 2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'remote' software.

    'remote' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'remote' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'remote'.  If not, see <http://www.gnu.org/licenses/>.
*/

// 'proxy' remote plugin.

#include "registry.h"

#include "rpproxy.h"

#include "sclmisc.h"
#include "sclplugin.h"
#include "sclmisc.h"

#include "csdrcd.h"
#include "csdmxc.h"

#include "prxy.h"

#define PLUGIN_NAME	"proxy"

typedef csdmxc::cCallback cCallback_;

class rCallback
: public cCallback_
{
private:
	qCBUFFERr HostService_, Identifier_;
	prxy::rFlow *FlowAsPointer_( void *UP ) const
	{
		if ( UP == NULL )
			qRFwk();

		return (prxy::rFlow *)UP;
	}
	prxy::rFlow &F_( void *UP ) const
	{
		return *FlowAsPointer_( UP );
	}
protected:
	virtual void *CSDMXCNew( void ) override
	{
		prxy::rFlow *Flow = NULL;
	qRH
		lcl::meaning Meaning;
	qRB
		Flow = new prxy::rFlow;

		if ( Flow == NULL )
			qRAlc();

		Meaning.Init();

		if ( !Flow->Init( HostService_, Identifier_, prxybase::tClient, sck::NoTimeout, Meaning ) )
			sclmisc::ReportAndAbort( Meaning );
	qRR
	qRT
	qRE
		return Flow;
	}
	virtual csdmxc::fFlow &CSDMXCExtractFlow( void *UP ) override
	{
		return F_( UP );
	}
	virtual void CSDMXCRelease( void *UP ) override
	{
		delete FlowAsPointer_( UP );
	}
	virtual time_t CSDMXCEpochTimeStamp( void *UP ) override
	{
		return F_( UP ).EpochTimeStamp();
	}
public:
	void reset( bso::sBool = true )
	{
	}
	qCVDTOR( rCallback );
	void Init(
		const str::string_ &HostService,
		const str::string_ &Identifier )
	{
		HostService.Convert( HostService_ );
		Identifier.Convert( Identifier_ );
	}
};

typedef csdrcd::cDriver cPlugin_;
using csdrcd::rDriver;

class rPlugin
: public cPlugin_
{
private:
	rCallback Callback_;
	csdmxc::rCore Core_;
protected:
	virtual rDriver *CSDRCCNew( void ) override
	{
		csdmxc::_driver___ *Driver = new csdmxc::_driver___;

		if ( Driver == NULL )
			qRAlc();

		Driver->Init( Core_, fdr::ts_Default );

		return Driver;
	}
	virtual void CSDRCCDelete( fdr::ioflow_driver_base___ *Driver ) override
	{
		delete Driver;
	}
public:
	void reset( bso::bool__ P = true )
	{
		Core_.reset( P );
		Callback_.reset( P );
	}
	qCVDTOR( rPlugin );
	bso::bool__ Init(
		const char *HostService,
		const char *Identifier )
	{
		Callback_.Init( str::string( HostService ), str::string( Identifier ) );

		if ( !Core_.Init( Callback_ ) )
			return false;

		return true;
	}
	bso::sBool SCLPLUGINInitialize( plgncore::sAbstract *BaseAbstract )
	{
		bso::sBool Success = false;
	qRH
		str::wString HostService, Identifier;
		qCBUFFERr HostServiceBuffer, IdentifierBuffer;
		rpproxy::rAbstract *Abstract = (rpproxy::rAbstract *)BaseAbstract;
	qRB
		plgn::Test( BaseAbstract, rpproxy::Identifier );

		HostService.Init();
		sclmisc::MGetValue( registry::HostService, HostService );

		Identifier.Init();
		sclmisc::MGetValue( registry::Identifier, Identifier );

		if ( !Init( HostService.Convert( HostServiceBuffer ), Identifier.Convert( IdentifierBuffer ) ) )
			switch ( plgn::ErrorReporting( Abstract ) ) {
			case plgn::rhInternally:
				sclmisc::ReportAndAbort( "UnableToConnectTo", HostService );
				break;
			case plgn::rhDetailed:
				Abstract->HostService = HostService;
			case plgn::rhBasic:
				qRReturn;
				break;
			default:
				qRGnr();
				break;
			}
		
		Success = true;
	qRR
	qRT
	qRE
		return Success;
	}
};

SCLPLUGIN_DEF( rPlugin );

const char *sclmisc::SCLMISCTargetName = PLUGIN_NAME;

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

qGCTOR( proxy )
{
	if ( strcmp( rpproxy::Identifier, IDENTIFIER ) )
		qRChk();
}

