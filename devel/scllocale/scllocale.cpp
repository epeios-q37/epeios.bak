/*
	'scllocale.cpp' by Claude SIMON (http://zeusw.org/).

	'scllocale' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define SCLLOCALE__COMPILATION

#include "scllocale.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "lcl.h"
#include "fnm.h"
#include "dir.h"
#include "cio.h"

#include "sclmisc.h"

using namespace scllocale;

using lcl::level__;

static lcl::locale Locale_;

static level__ SoftwareLevel_ = LCL_UNDEFINED_LEVEL;
static level__ ConfigurationLevel_ = LCL_UNDEFINED_LEVEL;
static level__ ProjectLevel_ = LCL_UNDEFINED_LEVEL;

const lcl::locale_ &scllocale::GetLocale( void )
{
	return Locale_;
}

static void GetLocaleFileParsingErrorMeaning_(
	const rgstry::context___ &Context,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.SetValue( SCLLOCALE_NAME "_LocaleFileParsingError" );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );
ERRErr
ERREnd
ERREpilog
}

static void Load_(
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	level__ &Level,
	const char *ErrorLabel )
{
ERRProlog
	rgstry::context___ Context;
ERRBegin
	if ( Level != LCL_UNDEFINED_LEVEL )
		if ( Locale_.Pop() != Level )
			ERRFwk();

	Context.Init();
	Level = Locale_.Push( Flow, xpp::criterions___( str::string( Directory ) ), RootPath, Context );

	if ( Level == LCL_UNDEFINED_LEVEL )
		sclmisc::ReportParsingErrorAndAbort( ErrorLabel, Context );
ERRErr
ERREnd
ERREpilog
}


void scllocale::LoadLocale(
	target__ Target,
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath )
{
	level__ *Level = NULL;
	const char *ErrorLabel = NULL;

	switch ( Target ) {
	case tSoftware:
		Level = &SoftwareLevel_;
		ErrorLabel = SCLLOCALE_NAME "_LocaleParsingError";
		break;
	case tConfiguration:
		Level = &ConfigurationLevel_;
		ErrorLabel = SCLLOCALE_NAME "_ConfigurationLocaleParsingError";
		break;
	case tProject:
		Level = &ProjectLevel_;
		ErrorLabel = SCLLOCALE_NAME "_ProjectLocaleParsingError";
		break;
	default:
		ERRFwk();
		break;
	}

	return Load_( Flow, Directory, RootPath, *Level, ErrorLabel );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scllocalepersonnalization
{
public:
	scllocalepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
		Locale_.Init();
}
	~scllocalepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static scllocalepersonnalization Tutor;
