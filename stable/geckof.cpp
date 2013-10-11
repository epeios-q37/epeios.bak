/*
	'geckof' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'geckof' header file ('geckof.h').
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



//	$Id: geckof.cpp,v 1.10 2013/04/09 17:57:31 csimon Exp $

#define GECKOF__COMPILATION

#include "geckof.h"

class geckoftutor
: public ttr_tutor
{
public:
	geckoftutor( void )
	: ttr_tutor( GECKOF_NAME )
	{
#ifdef GECKOF_DBG
		Version = GECKOF_VERSION "\b\bD $";
#else
		Version = GECKOF_VERSION;
#endif
		Owner = GECKOF_OWNER;
		Date = "$Date: 2013/04/09 17:57:31 $";
	}
	virtual ~geckoftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace geckof;

extern "C" typedef geckoo::create_steering create_steering;

bso::bool__ geckof::gecko_wrapper___::_CreateSteering( geckoo::shared_data__ *Data )
{
	create_steering *CreateSteering = dlbrry::GetFunction<create_steering *>( E_STRING( GECKOO_CREATE_STEERING_FUNCTION_NAME ), _Library );

	if ( CreateSteering == NULL )
		return false;

	if ( _Steering != NULL )
		ERRFwk();

	if ( ( _Steering = CreateSteering( Data ) ) == NULL )
		return false;

	return true;
}

extern "C" typedef geckoo::delete_steering delete_steering;

bso::bool__ geckof::gecko_wrapper___::_DeleteSteering( void )
{
	delete_steering *DeleteSteering = dlbrry::GetFunction<delete_steering *>( E_STRING( GECKOO_DELETE_STEERING_FUNCTION_NAME ), _Library );

	if ( DeleteSteering == NULL )
		return false;

	if ( _Steering == NULL )
		ERRFwk();

	DeleteSteering( _Steering );

	_Steering = NULL;

	return true;
}

extern "C" typedef geckoo::retrieve_steering retrieve_steering;

geckoo::steering_callback__ *geckof::RetrieveSteering(
	const char *LibraryName,
	err::handling__ ERRHandling )
{
	geckoo::steering_callback__ *Steering = NULL;
ERRProlog
	dlbrry::dynamic_library___ Library;
	retrieve_steering *RetrieveSteering = NULL;
ERRBegin

	if ( Library.Init( LibraryName ) ) {
		RetrieveSteering = dlbrry::GetFunction<retrieve_steering *>( E_STRING( GECKOO_RETRIEVE_STEERING_FUNCTION_NAME ), Library );

		if ( RetrieveSteering != NULL )
			Steering = RetrieveSteering();
	}

	if ( ( Steering == NULL ) && ( ERRHandling != err::hUserDefined ) )
		ERRSys();
ERRErr
ERREnd
ERREpilog
	return Steering;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class geckofpersonnalization
: public geckoftutor
{
public:
	geckofpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~geckofpersonnalization( void )
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

static geckofpersonnalization Tutor;

ttr_tutor &GECKOFTutor = Tutor;
