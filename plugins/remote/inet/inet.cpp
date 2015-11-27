/*
	'inet.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'inet' software.

    'inet' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'inet' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'inet'.  If not, see <http://www.gnu.org/licenses/>.
*/

// 'inet' remote plugin.

#include "registry.h"

#include "sclmisc.h"
#include "sclplugin.h"
#include "sclmisc.h"

#include "csdrcd.h"

#include "csdsnc.h"

#define PLUGIN_NAME	"inet"

typedef csdrcd::driver___ _plugin___;

typedef csdsnc::_driver___ _driver___;

class plugin___
: public _plugin___,
  public _driver___
{
private:
	csdsnc::core Core_;
protected:
	virtual fdr::size__ FDRWrite(
		const fdr::byte__ *Buffer,
		fdr::size__ Maximum ) override
	{
		return _driver___::FDRWrite( Buffer, Maximum );
	}
	virtual void FDRCommit( void ) override
	{
		return _driver___::FDRCommit();
	}
	virtual fdr::size__ FDRRead(
		fdr::size__ Maximum,
		fdr::byte__ *Buffer ) override
	{
		return _driver___::FDRRead( Maximum, Buffer );
	}
	virtual void FDRDismiss( void ) override
	{
		return _driver___::FDRDismiss();
	}
public:
	void reset( bso::bool__ P = true )
	{
		_plugin___::reset( P );
		_driver___::reset( P );
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

		_driver___::Init( Core_, fdr::ts_Default );

		return true;
	}
	void SCLPLUGINInitialize( void )
	{
	qRH
		str::string HostService;
		bso::uint__ PingDelay = 0;
		TOL_CBUFFER___ Buffer;
	qRB
		HostService.Init();

		sclmisc::MGetValue( registry::HostService, HostService );
		PingDelay = sclmisc::OGetUInt( registry::PingDelay, 0 );

		if ( !Init(HostService.Convert(Buffer), PingDelay) )
			sclmisc::ReportAndAbort( "UnableToConnectTo", HostService );
	qRR
	qRT
	qRE
	}
};

SCLPLUGIN_DEF( plugin___ );

const char *sclmisc::SCLMISCTargetName = PLUGIN_NAME;
