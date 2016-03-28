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
protected:
	virtual fdr::size__ FDRWrite(
		const fdr::byte__ *Buffer,
		fdr::size__ Maximum ) override
	{
		return Flow_.WriteUpTo( Buffer, Maximum );
	}
	virtual void FDRCommit( void ) override
	{
		return Flow_.Commit();
	}
	virtual fdr::size__ FDRRead(
		fdr::size__ Maximum,
		fdr::byte__ *Buffer ) override
	{
		return Flow_.ReadUpTo( Maximum, Buffer );
	}
	virtual void FDRDismiss( void ) override
	{
		return Flow_.Dismiss();
	}
public:
	void reset( bso::bool__ P = true )
	{
		_plugin___::reset( P );
		Flow_.reset( P );
		Core_.reset( P );
	}
	E_CVDTOR( plugin___ );
	bso::bool__ Init(
		const char *HostService,
		bso::uint__ PingDelay )
	{
		_plugin___::Init();

		if ( !Core_.Init( HostService, PingDelay ) )
			return false;

		Flow_.Init( Core_ );

		return true;
	}
	bso::sBool SCLPLUGINInitialize( void *UP )
	{
		bso::sBool Success = false;
	qRH
		str::string HostService;
		bso::uint__ PingDelay = 0;
		TOL_CBUFFER___ Buffer;
		rpstraight::rData *Data = (rpstraight::rData *)UP;
	qRB
		HostService.Init();

		sclmisc::MGetValue( registry::HostService, HostService );
		PingDelay = sclmisc::OGetUInt( registry::PingDelay, 0 );

		if ( !Init(HostService.Convert(Buffer), PingDelay) )
			if ( Data == NULL  )
				sclmisc::ReportAndAbort( "UnableToConnectTo", HostService );
			else {
				if ( Data != plgn::NonNullUP ) {
					Data->HostService = HostService;
				}

				qRReturn;
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