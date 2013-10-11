/*
	'dbsbsc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'dbsbsc' header file ('dbsbsc.h').
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



//	$Id: dbsbsc.cpp,v 1.10 2009/11/12 18:25:54 csimon Exp $

#define DBSBSC__COMPILATION

#include "dbsbsc.h"

class dbsbsctutor
: public ttr_tutor
{
public:
	dbsbsctutor( void )
	: ttr_tutor( DBSBSC_NAME )
	{
#ifdef DBSBSC_DBG
		Version = DBSBSC_VERSION "\b\bD $";
#else
		Version = DBSBSC_VERSION;
#endif
		Owner = DBSBSC_OWNER;
		Date = "$Date: 2009/11/12 18:25:54 $";
	}
	virtual ~dbsbsctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dbsbsc;

void dbsbsc::DropFile(
	const str::string_ &RootFileName,
	const char *Extension )
{
ERRProlog
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	if ( RootFileName.Amount() != 0 ) {
		FileName.Init( RootFileName );
		FileName.Append( Extension );

		if ( fil::FileExists( FileName.Convert( FileNameBuffer) ) )
			if ( remove( FileNameBuffer ) != 0 )
				ERRd();
	}
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbsbscpersonnalization
: public dbsbsctutor
{
public:
	dbsbscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbsbscpersonnalization( void )
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

static dbsbscpersonnalization Tutor;

ttr_tutor &DBSBSCTutor = Tutor;
