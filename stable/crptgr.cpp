/*
	'crptgr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'crptgr' header file ('crptgr.h').
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



//	$Id: crptgr.cpp,v 1.8 2013/04/19 19:28:00 csimon Exp $

#define CRPTGR__COMPILATION

#include "crptgr.h"

class crptgrtutor
: public ttr_tutor
{
public:
	crptgrtutor( void )
	: ttr_tutor( CRPTGR_NAME )
	{
#ifdef CRPTGR_DBG
		Version = CRPTGR_VERSION "\b\bD $";
#else
		Version = CRPTGR_VERSION;
#endif
		Owner = CRPTGR_OWNER;
		Date = "$Date: 2013/04/19 19:28:00 $";
	}
	virtual ~crptgrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"

using namespace crptgr;

const str::string_ &crptgr::Encrypt(
	const str::string_ &Plain,
	const str::string_ &Key,
	str::string_ &Crypted )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	encrypt_oflow___ EFlow;
	sdr::row__ Row = E_NIL;
ERRBegin
	SFlow.Init( Crypted );
	EFlow.Init( SFlow, Key );

	Row = Plain.First();

	while( Row != E_NIL ) {
		EFlow.Put( Plain( Row ) );

		Row = Plain.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return Crypted;
}

const str::string_ &crptgr::Decrypt(
	const str::string_ &Crypted,
	const str::string_ &Key,
	str::string_ &Plain )
{
ERRProlog
	flx::E_STRING_IFLOW__ SFlow;
	decrypt_iflow___ DFlow;
ERRBegin
	SFlow.Init( Crypted );
	DFlow.Init( SFlow, Key );

	while ( DFlow.AmountRed() != Crypted.Amount() )
		Plain.Append( DFlow.Get() );
ERRErr
ERREnd
ERREpilog
	return Plain;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class crptgrpersonnalization
: public crptgrtutor
{
public:
	crptgrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~crptgrpersonnalization( void )
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

static crptgrpersonnalization Tutor;

ttr_tutor &CRPTGRTutor = Tutor;
