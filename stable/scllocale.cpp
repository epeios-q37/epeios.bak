/*
	'scllocale' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scllocale' header file ('scllocale.h').
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: scllocale.cpp,v 1.14 2013/05/22 17:40:11 csimon Exp $

#define SCLLOCALE__COMPILATION

#include "scllocale.h"

class scllocaletutor
: public ttr_tutor
{
public:
	scllocaletutor( void )
	: ttr_tutor( SCLLOCALE_NAME )
	{
#ifdef SCLLOCALE_DBG
		Version = SCLLOCALE_VERSION "\b\bD $";
#else
		Version = SCLLOCALE_VERSION;
#endif
		Owner = SCLLOCALE_OWNER;
		Date = "$Date: 2013/05/22 17:40:11 $";
	}
	virtual ~scllocaletutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

static void ReportLocaleFileParsingError_(
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
		ReportLocaleFileParsingError_( Context, ErrorMeaning );
		sclerror::SetMeaning( ErrorMeaning );
		ERRExit( EXIT_FAILURE );
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
: public scllocaletutor
{
public:
	scllocalepersonnalization( void )
	{
		Locale_.Init();
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scllocalepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static scllocalepersonnalization Tutor;

ttr_tutor &SCLLOCALETutor = Tutor;
