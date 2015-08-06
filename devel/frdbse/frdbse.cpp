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

#define FRDBSE__COMPILATION

#include "frdbse.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "stsfsm.h"

using namespace frdbse;

#define C( name ) case bt##name: return #name; break

const char *frdbse::GetLabel( backend_type__ BackendType )
{
	switch ( BackendType ) {
	C( None );
	C( Remote );
	C( Embedded );
	C( Predefined );
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

static stsfsm::automat BackendAutomat_;

static void FillBackendAutomat_( void )
{
	BackendAutomat_.Init();
	stsfsm::Fill( BackendAutomat_, bt_amount, GetLabel );
}

backend_type__ frdbse::GetBackendType( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, BackendAutomat_, bt_Undefined, bt_amount );
}

static void GetAuthorText_(
	const char *Name,
	const char *Contact,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( FRDBSE_NAME "_AuthorText" );
	Meaning.AddTag( Name );
	Meaning.AddTag( Contact );

	Locale.GetTranslation( Meaning, Language, Text );
qRR
qRT
qRE
}

static void GetAffiliationText_(
	const char *Name,
	const char *URL,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( FRDBSE_NAME "_AffiliatedSoftwareText" );
	Meaning.AddTag( Name );
	Meaning.AddTag( URL );

	Locale.GetTranslation( Meaning, Language, Text );
qRR
qRT
qRE
}

Q37_GCTOR( frdbse )
{
	FillBackendAutomat_();
}

