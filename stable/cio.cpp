/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define CIO_COMPILATION_

#include "cio.h"

#include "flx.h"

using namespace cio;

namespace {
	eType Type_ = t_Undefined;

	flx::void_idriver___ _VInDriver;
	flx::void_odriver___ _VOutDriver;
	flx::void_odriver___ _VErrDriver;

	iof::io_iflow_driver___ _SInDriver;
	iof::io_oflow_driver___ _SOutDriver;
	iof::io_oflow_driver___ _SErrDriver;

	set__
		_Current,
		_Console( _SInDriver, _SOutDriver, _SErrDriver, tTerminal ),
		_Void( _VInDriver, _VOutDriver, _VErrDriver, tVoid );
}

cif__ cio::CInF;
cof___ cio::COutF, cio::CErrF;

txf::text_iflow__ cio::CIn;
txf::text_oflow__ cio::COut, cio::CErr;

#if defined( CPE_S_WIN )
#	include <io.h>
#	include <fcntl.h>
#endif

#ifdef IOP__USE_LOWLEVEL_IO
iop::descriptor__ cio::CInDescriptor = 0, cio::COutDescriptor = 1, cio::CErrDescriptor = 2;
#elif defined( IOP__USE_STANDARD_IO )
iop::descriptor__ cio::cind = stdin, cio::coutd = stdout, cio::cerrd = stderr;
#else
#	error "Unkonw I/O enviroment !"
#endif

const set__ &cio::GetConsoleSet( void )
{
	return _Console;
}

const set__ &cio::GetVoidSet( void )
{
	return _Void;
}

const set__ &cio::GetCurrentSet( void )
{
	return _Current;
}

static void InitializeConsole_( void )
{
#if defined( CPE_S_WIN )
	if ( _fileno( stdin ) >= 0 ) {
		if ( _setmode( _fileno( stdin ), _O_BINARY ) == -1 )
			qRLbr();

		if ( _setmode( _fileno( stdout ), _O_BINARY ) == -1 )
			qRLbr();

		if ( _setmode( _fileno( stderr ), _O_BINARY ) == -1 )
			qRLbr();
	}
#endif
	_VInDriver.Init( fdr::ts_Default, flx::aAllowed );
	_VOutDriver.Init( fdr::ts_Default, flx::aAllowed );
	_VErrDriver.Init( fdr::ts_Default, flx::aAllowed );

	_SInDriver.Init( CInDescriptor, fdr::ts_Default );
	_SOutDriver.Init( COutDescriptor, fdr::ts_Default );
	_SErrDriver.Init( CErrDescriptor, fdr::ts_Default );
}

const set__ &cio::GetSet( eType Type )
{
	switch ( Type ) {
	case tTerminal:
		return GetConsoleSet();
		break;
	case tVoid:
		return GetVoidSet();
		break;
	case tCurrent:
		return GetCurrentSet();
		break;
	default:
		qRFwk();
		break;
	}

	return GetVoidSet();	// To avoid a warning.
}

void cio::Initialize( eType Type )
{
	switch ( Type ) {
	case tTerminal:
		CInF.Init( _SInDriver );
		COutF.Init( _SOutDriver );
		CErrF.Init( _SErrDriver );
		break;
	case tVoid:
		CInF.Init( _VInDriver );
		COutF.Init( _VOutDriver );
		CErrF.Init( _VErrDriver );
		break;
	case tUser:
		if ( !CInF.IsInitialized() )
			qRFwk();

		if ( !COutF.IsInitialized() )
			qRFwk();

		if ( !CErrF.IsInitialized() )
			qRFwk();

		break;
	default:
		qRFwk();
		break;
	}

	_Current.Init( CInF.RDriver(), COutF.WDriver(), CErrF.WDriver(), Type );

	cio::CIn.Init( CInF );
	cio::COut.Init( COutF );
	cio::CErr.Init( CErrF );

	::Type_ = Type;
}

void cio::Initialize( const set__ &Set )
{
	CInF.Init( Set.In() );
	COutF.Init( Set.Out() );
	CErrF.Init( Set.Err() );

	Initialize( Set.Type() );
}

eType cio::Type( void )
{
	return ::Type_;
}

fdr::rRDriver &cio::GetInDriver( void )
{
	return _SInDriver;
}

fdr::rWDriver &cio::GetOutDriver( void )
{
	return _SOutDriver;
}

fdr::rWDriver &cio::GetErrDriver( void )
{
	return _SErrDriver;
}

Q37_GCTOR( cio )
{
	InitializeConsole_();
}
