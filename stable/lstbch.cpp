/*
	'lstbch' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'lstbch' header file ('lstbch.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: lstbch.cpp,v 1.46 2013/04/06 14:50:46 csimon Exp $

#define LSTBCH__COMPILATION

#include "lstbch.h"

class lstbchtutor
: public ttr_tutor
{
public:
	lstbchtutor( void )
	: ttr_tutor( LSTBCH_NAME )
	{
#ifdef LSTBCH_DBG
		Version = LSTBCH_VERSION "\b\bD $";
#else
		Version = LSTBCH_VERSION;
#endif
		Owner = LSTBCH_OWNER;
		Date = "$Date: 2013/04/06 14:50:46 $";
	}
	virtual ~lstbchtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace lstbch;

void lstbch::hook_filenames___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
ERRProlog
	fnm::name___ Bunch, List;
ERRBegin
	Bunch.Init();
	fnm::BuildPath( Path, Basename, "b", Bunch );

	List.Init();
	fnm::BuildPath( Path, Basename, "l", List );

	this->Bunch.Init( NULL, Bunch );
	this->List.Init( NULL, List );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lstbchpersonnalization
: public lstbchtutor
{
public:
	lstbchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lstbchpersonnalization( void )
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

static lstbchpersonnalization Tutor;

ttr_tutor &LSTBCHTutor = Tutor;
