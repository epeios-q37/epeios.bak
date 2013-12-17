/*
	'ndbbsc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbbsc' header file ('ndbbsc.h').
	Copyright (C) 2004 Claude SIMON.

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



//	$Id: ndbbsc.cpp,v 1.10 2013/04/15 10:50:52 csimon Exp $

#define NDBBSC__COMPILATION

#include "ndbbsc.h"

class ndbbsctutor
: public ttr_tutor
{
public:
	ndbbsctutor( void )
	: ttr_tutor( NDBBSC_NAME )
	{
#ifdef NDBBSC_DBG
		Version = NDBBSC_VERSION "\b\bD $";
#else
		Version = NDBBSC_VERSION;
#endif
		Owner = NDBBSC_OWNER;
		Date = "$Date: 2013/04/15 10:50:52 $";
	}
	virtual ~ndbbsctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ndbbsc;

void ndbbsc::DropFile(
	const str::string_ &RootFileName,
	const char *Extension )
{
ERRProlog
	str::string FileName;
	TOL_CBUFFER___ FileNameBuffer;
ERRBegin
	if ( RootFileName.Amount() != 0 ) {
		FileName.Init( RootFileName );
		FileName.Append( Extension );

		if ( fil::Exists( FileName.Convert( FileNameBuffer) ) )
			if ( remove( FileNameBuffer ) != 0 )
				ERRLbr();
	}
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbbscpersonnalization
: public ndbbsctutor
{
public:
	ndbbscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbbscpersonnalization( void )
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

static ndbbscpersonnalization Tutor;

ttr_tutor &NDBBSCTutor = Tutor;
