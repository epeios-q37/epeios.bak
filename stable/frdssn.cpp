/*
	'frdssn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'frdssn' header file ('frdssn.h').
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



//	$Id: xxx.cpp,v 1.9 2012/11/14 16:06:23 csimon Exp $

#define FRDSSN__COMPILATION

#include "frdssn.h"

class frdssntutor
: public ttr_tutor
{
public:
	frdssntutor( void )
	: ttr_tutor( FRDSSN_NAME )
	{
#ifdef FRDSSN_DBG
		Version = FRDSSN_VERSION "\b\bD $";
#else
		Version = FRDSSN_VERSION;
#endif
		Owner = FRDSSN_OWNER;
		Date = "$Date: 2012/11/14 16:06:23 $";
	}
	virtual ~frdssntutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace frdssn;

recap__ frdssn::session___::FillSessionRegistry(
	xtf::extended_text_iflow__ &SettingsXFlow,
	const xpp::criterions___ &Criterions,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;
ERRProlog
	TOL_CBUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	_Test();

	switch ( _Registry.FillSession( SettingsXFlow, Criterions, FRDKRN_SESSION_LABEL, ErrorSet.Context ) ) {
	case rgstry::sOK:
		break;
	case rgstry::sUnableToFindRootPath:
		ERRFwk();
		break;
	default:
		Recap = rSetupParsingError;
		ERRReturn;
		break;
	}

	Recap = r_OK;
ERRErr
ERREnd
ERREpilog
	return Recap;
}

status__ frdssn::session___::FillSessionRegistry(
	xtf::extended_text_iflow__ &SettingsXFlow,
	const xpp::criterions___ &Criterions )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Recap = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Recap = FillSessionRegistry( SettingsXFlow, Criterions, ErrorSet ) ) != r_OK ) {
		_Meaning.Init();
		frdkrn::GetMeaning( Recap, ErrorSet, _Meaning );
		Status = sWarning;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class frdssnpersonnalization
: public frdssntutor
{
public:
	frdssnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~frdssnpersonnalization( void )
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

static frdssnpersonnalization Tutor;

ttr_tutor &FRDSSNTutor = Tutor;
