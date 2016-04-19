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

// 'straight' remote plugin.

#include "registry.h"

#include "rpstraight.h"

#include "sclmisc.h"
#include "sclplugin.h"
#include "sclmisc.h"

#include "csdrcd.h"

#include "csdmnc.h"

#define PLUGIN_NAME	"straight"

typedef csdrcd::driver___ _plugin___;


class plugin___
: public _plugin___
{
private:
	csdmnc::rCore Core_;
	csdmnc::rClientIOFlow Flow_;
	bso::sBool Connected_;
	void HandleLostConnection_( void )
	{
		Connected_ = false;
		ERRRst();
	}
protected:
	virtual fdr::size__ FDRWrite(
		const fdr::byte__ *Buffer,
		fdr::size__ Maximum ) override
	{
		fdr::size__ Size = 0;

		if ( !Connected_ )
			return 0;
	qRH
	qRB
		Size = Flow_.WriteUpTo( Buffer, Maximum );
	qRR
		HandleLostConnection_();
		Size = 0;
	qRT
	qRE
		return Size;
	}
	virtual void FDRCommit( void ) override
	{
		if ( !Connected_ )
			return;
	qRH
	qRB
		Flow_.Commit();
	qRR
		HandleLostConnection_();
	qRT
	qRE
		return;
	}
	virtual fdr::size__ FDRRead(
		fdr::size__ Maximum,
		fdr::byte__ *Buffer ) override
	{
		if ( !Connected_ )
			return 0;

		fdr::size__ Size = 0;
	qRH
	qRB
		Size = Flow_.ReadUpTo( Maximum, Buffer );
	qRR
		HandleLostConnection_();
		Size = 0;
	qRT
	qRE
		return Size;
	}
	virtual void FDRDismiss( void ) override
	{
		if ( !Connected_ )
			return;
	qRH
	qRB
		Flow_.Dismiss();
	qRR
		HandleLostConnection_();
	qRT
	qRE
		return;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_plugin___::reset( P );
		Flow_.reset( P );
		Core_.reset( P );
		Connected_ = false;
	}
	E_CVDTOR( plugin___ );
	bso::bool__ Init(
		const char *HostService,
		bso::uint__ PingDelay,
		sck::duration__ TimeOut )
	{
		_plugin___::Init();

		if ( !Core_.Init( HostService, PingDelay, TimeOut ) )
			return false;

		Flow_.Init( Core_ );

		Connected_ = true;

		return true;
	}
	bso::sBool SCLPLUGINInitialize( plgncore::sAbstract *BaseAbstract )
	{
		bso::sBool Success = false;
	qRH
		str::string HostService;
		bso::uint__ PingDelay = 0;
		sck::duration__ TimeOut = sck::NoTimeOut;
		TOL_CBUFFER___ Buffer;
		rpstraight::rAbstract *Abstract = (rpstraight::rAbstract *)BaseAbstract;
	qRB
		plgn::Test( BaseAbstract, rpstraight::Identifier );

		HostService.Init();

		sclmisc::MGetValue( registry::parameter::HostService, HostService );
		PingDelay = sclmisc::OGetUInt( registry::parameter::PingDelay, 0 );
		TimeOut = sclmisc::OGetU16( registry::parameter::TimeOut, sck::NoTimeOut );

		if ( !Init( HostService.Convert(Buffer), PingDelay, TimeOut ) )
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

SCLPLUGIN_DEF( plugin___ );

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

qGCTOR(straight)
{
	if ( strcmp( rpstraight::Identifier, IDENTIFIER ) )
		qRChk();
}

