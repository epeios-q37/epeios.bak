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
	bch::qBUNCHwl( sFunction_ ) Functions_;
}

void sclznd::sRegistrar::Register( sFunction_ Function )
{
	Functions_.Append( Function );
}

ZEND_FUNCTION( Wrapper )
{
qRFH
	sArguments Arguments;
	long Index = 0;
	int num_varargs;
	zval ***varargs = NULL;
qRFB
	zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l*", &Index, &varargs, &num_varargs );

	if ( !Functions_.Exists( Index ) )
		qRGnr();

#ifdef ZTS
	Arguments.Init( tsrm_ls, num_varargs, varargs );
#else
	Arguments.Init( NULL, num_varargs, varargs );
#endif

	Functions_( Index )( Arguments );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRFR
qRFT
qRFE( sclmisc::ErrFinal() )
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

		Functions_.Init();
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



