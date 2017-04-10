/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define SCLZND_COMPILATION_

#include "sclznd.h"

#include "sclerror.h"
#include "scllocale.h"
#include "sclmisc.h"

#include "bch.h"
#include "str.h"
#include "cio.h"

using namespace sclznd;

namespace {
	bch::qBUNCHwl( sCallback_ ) Callbacks_;
}

void sclznd::sRegistrar::Register( sCallback_ Callback )
{
	Callbacks_.Append( Callback );
}

namespace {
	void Infos_( str::dString &Infos )
	{
	qRFH
		flx::rStringOFlow SFlow;
		txf::sOFlow TFlow;
	qRFB
		Infos.Init();
		SFlow.Init( Infos );
		TFlow.Init( SFlow );

		TFlow << sclmisc::SCLMISCProductName << " v" << SCLZNDProductVersion << " - Zend v" << ZEND_VERSION << txf::nl
			<< txf::pad << "Build : " __DATE__ " " __TIME__ " (" <<  cpe::GetDescription() << ')';
	qRFR
	qRFT
	qRFE( sclmisc::ErrFinal() )
	}
}

void sclznd::Infos_( zval *Return )
{
qRFH
	str::wString Infos;
	qCBUFFERr Buffer;
qRFB
	Infos.Init();
	::Infos_( Infos );

	ZVAL_STRING( Return ,Infos.Convert( Buffer ), 1 );
qRFR
qRFT
qRFE( sclmisc::ErrFinal() )
}


void sclznd::Wrapper_(
	long Index,
	int num_varargs,
	zval ***varargs
	TSRMLS_DC )
{
qRFH
	sArguments Arguments;
qRFB
	if ( !Callbacks_.Exists( Index ) )
		qRGnr();

#ifdef ZTS
	Arguments.Init( tsrm_ls, num_varargs, varargs );
#else
	Arguments.Init( NULL, num_varargs, varargs );
#endif

	Callbacks_( Index )( Arguments );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRFR
qRFT
qRFE( sclmisc::ErrFinal() )
}

ZEND_FUNCTION(Wrapper);


namespace {

	zend_module_entry ModuleEntry = {
	#if ZEND_MODULE_API_NO >= 20010901
		STANDARD_MODULE_HEADER,
	#endif
		sclmisc::SCLMISCProductName,
		SCLZNDFunctions,
		NULL, // name of the MINIT function or NULL if not applicable
		NULL, // name of the MSHUTDOWN function or NULL if not applicable
		NULL, // name of the RINIT function or NULL if not applicable
		NULL, // name of the RSHUTDOWN function or NULL if not applicable
		NULL, // name of the MINFO function or NULL if not applicable
	#if ZEND_MODULE_API_NO >= 20010901
		SCLZNDProductVersion,
	#endif
		STANDARD_MODULE_PROPERTIES
	};
}
 
// the following code creates an entry for the module and registers it with Zend.
 
extern "C" {
	ZEND_DLEXPORT zend_module_entry *get_module(void) { return &ModuleEntry; }\
}


namespace {
	namespace {
		err::err___ Error_;
		sclerror::rError SCLError_;
		scllocale::rRack Locale_;
		sclmisc::sRack Rack_;
	}

	void Initialize_( void )
	{
	qRFH
		str::wString Location;
	qRFB
		sRegistrar Registrar;

		Error_.Init();
		SCLError_.Init();
		Locale_.Init();

		cio::Initialize( cio::GetConsoleSet() );
		Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

		Location.Init();
		// TODO : Find a way to fill 'Location' with the path of the binary.

		sclmisc::Initialize( Rack_, Location, qRPU );

		Registrar.Init();

		Callbacks_.Init();
		sclznd::SCLZNDRegister( Registrar );
	qRFR
	qRFT
	qRFE( sclmisc::ErrFinal() )
	}
}

qGCTOR( sclznd )
{
	Initialize_();
}



