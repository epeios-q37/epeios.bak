/*
	'lgg' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'lgg' header file ('lgg.h').
	Copyright (C) 2000, 2002-2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: lgg.cpp,v 1.18 2012/11/14 16:06:30 csimon Exp $

#define LGG__COMPILATION

#include "lgg.h"

class lggtutor
: public ttr_tutor
{
public:
	lggtutor( void )
	: ttr_tutor( LGG_NAME )
	{
#ifdef LGG_DBG
		Version = LGG_VERSION "\b\bD $";
#else
		Version = LGG_VERSION;
#endif
		Owner = LGG_OWNER;
		Date = "$Date: 2012/11/14 16:06:30 $";
	}
	virtual ~lggtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace lgg;

const char *lgg::LanguageNames[lgg::l_amount] = {
	"raw",
	"English",
	"Français",
	"Deutsch",
	"Español",
	"Nederlands",
	"Português"
 };

const char *lgg::LanguageLabels[lgg::l_amount] = {
	"xx",
	"en",
	"fr",
	"de",
	"es",
	"nl",
	"pt"
};

language__ lgg::GetLanguageIDNamed( const char *Name )
{
	int i = 0;

	while( ( i < l_amount ) && ( strcmp( Name, LanguageNames[i] ) ) )
		i++;

	if ( i >= l_amount )
		return l_undefined;
	else
		return (language__)i;
}

language__ lgg::GetLanguageIDWithLabel( const char *Name )
{
	int i = 0;

	while( ( i < l_amount ) && ( strcmp( Name, LanguageLabels[i] ) ) )
		i++;

	if ( i >= l_amount )
		return l_undefined;
	else
		return (language__)i;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lggpersonnalization
: public lggtutor
{
public:
	lggpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lggpersonnalization( void )
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

static lggpersonnalization Tutor;

ttr_tutor &LGGTutor = Tutor;
