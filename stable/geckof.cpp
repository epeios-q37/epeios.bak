/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define GECKOF__COMPILATION

#include "geckof.h"

using namespace geckof;

extern "C" typedef geckoo::create_steering create_steering;

bso::bool__ geckof::gecko_wrapper___::_CreateSteering( geckoo::shared_data__ *Data )
{
	create_steering *CreateSteering = dlbrry::GetFunction<create_steering *>( E_STRING( GECKOO_CREATE_STEERING_FUNCTION_NAME ), _Library );

	if ( CreateSteering == NULL )
		return false;

	if ( _Steering != NULL )
		qRFwk();

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
		qRFwk();

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
qRH
	dlbrry::dynamic_library___ Library;
	retrieve_steering *RetrieveSteering = NULL;
qRB

	if ( Library.Init( LibraryName ) ) {
		RetrieveSteering = dlbrry::GetFunction<retrieve_steering *>( E_STRING( GECKOO_RETRIEVE_STEERING_FUNCTION_NAME ), Library );

		if ( RetrieveSteering != NULL )
			Steering = RetrieveSteering();
	}

	if ( ( Steering == NULL ) && ( ERRHandling != err::hUserDefined ) )
		qRSys();
qRR
qRT
qRE
	return Steering;
}

