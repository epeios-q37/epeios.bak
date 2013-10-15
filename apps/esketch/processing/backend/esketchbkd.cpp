/*
	'esketchbkd.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: esketchbkd.cpp,v 1.10 2009/09/23 17:47:42 csimon Exp $

#include "sclbacknd.h"

#include "registry.h"
#include "sktfbc.h"
#include "sktinf.h"

#include "fdr.h"

#include "fblbkd.h"

#include "wrpunbound.h"
#include "wrpexample.h"

#define BACKEND_NAME	SKTINF_LC_AFFIX	"bkd"
#define COPYRIGHT		COPYRIGHT_YEARS " " SKTINF_OWNER_NAME " (" SKTINF_OWNER_CONTACT ")"	
#define API_VERSION		"1"

const char *sclbacknd::TargetName = BACKEND_NAME;

typedef fblbkd::backend___	_backend___;

class backend___
: public _backend___
{
private:
	FBLBKD_RAM_MODULE( wrpexample::myobject_ ) MyObject;
public:
	void Init(
		fblbur::mode__ Mode,
		const lcl::locale_ &Locale,
		const char *ClientOrigin )
	{
		_backend___::Init(
			Mode,
			API_VERSION,
			ClientOrigin,
			SKTINF_LC_AFFIX,
			Locale,
			BACKEND_NAME " " VERSION,
			COPYRIGHT,
			SKTINF_MC_AFFIX " " SKTINF_VERSION );

		wrpunbound::Inform( *this );

		MyObject.Init();
		Add( MyObject );
	}
};

typedef fblbkd::text_log_functions__<> log_functions__;

typedef sclbacknd::data___<backend___> data___;

void *sclbacknd::New( 
	fblbur::mode__ Mode,
	const lcl::locale_ &Locale,
	const char *Origin )
{
	::data___ *Data = NULL;
ERRProlog
ERRBegin
	if ( ( Data = new ::data___ ) == NULL )
		ERRAlc();

	Data->Init( Mode, Locale, Origin );
ERRErr
	if ( Data != NULL )
		delete Data;

	Data = NULL;
ERREnd
ERREpilog
	return Data;
}
