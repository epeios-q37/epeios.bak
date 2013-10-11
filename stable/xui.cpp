/*
	'xui' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xui' header file ('xui.h').
	Copyright (C) 2009-2004 Claude SIMON.

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



//	$Id: xui.cpp,v 1.2 2012/11/14 16:06:40 csimon Exp $

#define XUI__COMPILATION

#include "xui.h"

class xuitutor
: public ttr_tutor
{
public:
	xuitutor( void )
	: ttr_tutor( XUI_NAME )
	{
#ifdef XUI_DBG
		Version = XUI_VERSION "\b\bD $";
#else
		Version = XUI_VERSION;
#endif
		Owner = XUI_OWNER;
		Date = "$Date: 2012/11/14 16:06:40 $";
	}
	virtual ~xuitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xui;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xuipersonnalization
: public xuitutor
{
public:
	xuipersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xuipersonnalization( void )
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

static xuipersonnalization Tutor;

ttr_tutor &XUITutor = Tutor;
