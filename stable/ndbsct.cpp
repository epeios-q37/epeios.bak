/*
	'ndbsct' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbsct' header file ('ndbsct.h').
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



//	$Id: ndbsct.cpp,v 1.20 2013/04/11 19:36:51 csimon Exp $

#define NDBSCT__COMPILATION

#include "ndbsct.h"

class ndbscttutor
: public ttr_tutor
{
public:
	ndbscttutor( void )
	: ttr_tutor( NDBSCT_NAME )
	{
#ifdef NDBSCT_DBG
		Version = NDBSCT_VERSION "\b\bD $";
#else
		Version = NDBSCT_VERSION;
#endif
		Owner = NDBSCT_OWNER;
		Date = "$Date: 2013/04/11 19:36:51 $";
	}
	virtual ~ndbscttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ndbsct;

#define LIST_FILE_NAME_EXTENSION	".edl"
#define CONTENT_FILE_NAME_EXTENSION		".edc"

void ndbsct::static_content_atomized_file_manager___::Init(
	const str::string_ &BaseFileName,
	fil::mode__ Mode,
	fls::id__ ID )
{
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	reset();

	_Mode = Mode;
	_BaseFileName.Init( BaseFileName );

	FileName.Init( BaseFileName );
	FileName.Append( CONTENT_FILE_NAME_EXTENSION );

	_MemoryFileManager.Init( FileName.Convert( FileNameBuffer ), Mode, true, ID );


	FileName.Init( BaseFileName );
	FileName.Append( LIST_FILE_NAME_EXTENSION );

	_ListFileManager.Init( FileName.Convert( FileNameBuffer ), Mode, true );
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbsctpersonnalization
: public ndbscttutor
{
public:
	ndbsctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbsctpersonnalization( void )
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

static ndbsctpersonnalization Tutor;

ttr_tutor &NDBSCTTutor = Tutor;
