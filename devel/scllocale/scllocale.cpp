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

#include "sclerror.h"

using namespace scllocale;

static lcl::locale Locale_;

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

void scllocale::Load(
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	utf::format__ Format )
{
ERRProlog
	lcl::meaning ErrorMeaning;
	rgstry::context___ Context;
	lcl::level__ Level = LCL_UNDEFINED_LEVEL;
ERRBegin
	Context.Init();
	Level = Push( Flow, Directory, RootPath, Format, Context );

	if ( Level == LCL_UNDEFINED_LEVEL ) {
		ErrorMeaning.Init();
		GetLocaleFileParsingErrorMeaning_( Context, ErrorMeaning );
		sclerror::SetMeaning( ErrorMeaning );
		ERRAbort();
	} else if ( Level != 0 )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
}

lcl::level__ scllocale::Push(
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	utf::format__ Format,
	rgstry::context___ &Context )
{
	lcl::level__ Level = LCL_UNDEFINED_LEVEL;
ERRProlog
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	XFlow.Init( Flow, Format );

	Level = Locale_.Push( XFlow, xpp::criterions___( str::string( Directory ) ), RootPath,  Context );
ERRErr
ERREnd
ERREpilog
	return Level;
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
