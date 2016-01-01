/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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



//	$Id: frdrgy.cpp,v 1.24 2013/04/26 14:43:39 csimon Exp $

#define FRDRGY__COMPILATION

#include "frdrgy.h"

using namespace frdrgy;

# if 0

rgstry::entry___ frdrgy::Parameters( "Parameters" );
rgstry::entry___ frdrgy::Definitions( "Definitions" );
rgstry::entry___ frdrgy::Layouts( "Layouts" );

rgstry::entry___ frdrgy::DefaultProject( "DefaultProject", Parameters );
rgstry::entry___ frdrgy::Locale( "Locale", Parameters );

rgstry::entry___ frdrgy::PredefinedProjects( "PredefinedProjects", Parameters );
rgstry::entry___ frdrgy::DefaultPredefinedProject( "@Default", PredefinedProjects );
static rgstry::entry___ FreePredefinedProject_( "PredefinedProject", PredefinedProjects ); 
rgstry::entry___ frdrgy::PredefinedProjectId( "@id", FreePredefinedProject_ );
rgstry::entry___ frdrgy::PredefinedProject( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreePredefinedProject_);
rgstry::entry___ frdrgy::PredefinedProjectAlias( "@Alias", PredefinedProject );

rgstry::entry___ frdrgy::Backend( "Backend", Parameters );
rgstry::entry___ frdrgy::BackendAccessMode( "@AccessMode", Backend );
rgstry::entry___ frdrgy::BackendType( "@Type", Backend );
rgstry::entry___ frdrgy::BackendPingDelay( "PingDelay", Backend );

rgstry::entry___ frdrgy::Authentication( "Authentication", Parameters );
rgstry::entry___ frdrgy::AuthenticationCypherKey( "@CypherKey", Authentication );
rgstry::entry___ frdrgy::AuthenticationMode( "@Mode", Authentication );
rgstry::entry___ frdrgy::AuthenticationLogin( "Login", Authentication );
rgstry::entry___ frdrgy::AuthenticationPassword( "Password", Authentication );

rgstry::entry___ frdrgy::PredefinedBackends( "PredefinedBackends", Parameters );
rgstry::entry___ frdrgy::DefaultPredefinedBackend( "@Default", PredefinedBackends );
static rgstry::entry___ FreePredefinedBackend_( "PredefinedBackend", PredefinedBackends ); 
rgstry::entry___ frdrgy::PredefinedBackendId( "@id", FreePredefinedBackend_ );
rgstry::entry___ frdrgy::PredefinedBackend( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreePredefinedBackend_);
rgstry::entry___ frdrgy::PredefinedBackendAlias( "@Alias", PredefinedBackend );
rgstry::entry___ frdrgy::PredefinedBackendType( "@Type", PredefinedBackend );

rgstry::entry___ frdrgy::Internals( "Internals" );
rgstry::entry___ frdrgy::ProjectId( "ProjectId", Internals );




#define S		FRDRGY_PATH_SEPARATOR 
#define T		FRDRGY_PATH_TAG_STRING 
#define CTAG	FRDRGY_PATH_TAG_CHAR

#define PROFILES	FRDRGY_PROFILES_PATH
#define PROFILE		PROFILES "Profile[name=\"" T "\"]" S

const char *frdrgy::paths::profiles::FallbackProfile			= PROFILES "Fallback";
const char *frdrgy::paths::profiles::DefaultProfile				= PROFILES "Default";
const char *frdrgy::paths::profiles::UserProfile				= PROFILES "User";

static inline bso::bool__ GetFallbackProfileName_(
	const _registry_ &Registry,
	str::string_ &Name )
{
	return GetValue( paths::profiles::FallbackProfile, Registry, Name );
}

static inline bso::bool__ GetDefaultProfileName_(
	const _registry_ &Registry,
	str::string_ &Name )
{
	return GetValue( paths::profiles::DefaultProfile, Registry, Name );
}

static inline bso::bool__ GetUserProfileName_(
	const _registry_ &Registry,
	str::string_ &Name )
{
	return GetValue( paths::profiles::UserProfile, Registry, Name );
}

static bso::bool__ GetProfileValue_(
	const char *Path,
	const str::string_ &ProfileName,
	const _registry_ &Registry,
	str::string_ &Value )
{
	bso::bool__ Success = false;
qRH
str::string WorkPath;
qRB
	if ( ProfileName.Amount() == 0 )
		qRReturn;

	WorkPath.Init( Path );

	WorkPath.Replace( CTAG, 1, ProfileName );

	Success = Registry.GetValue( WorkPath, Value );
qRR
qRT
qRE
	return Success;
}

bso::bool__ frdrgy::GetProfileValue(
	const char *Path,
	const _registry_ &Registry,
	str::string_ &Value )
{
	bso::bool__ Success = false;
qRH
	str::string ProfileName;
qRB
	ProfileName.Init();

	if ( GetUserProfileName_( Registry, ProfileName ) )
		if ( Success = GetProfileValue_( Path, ProfileName, Registry, Value ) )
			qRReturn;

	ProfileName.Init();

	if ( GetDefaultProfileName_( Registry, ProfileName ) )
		if ( Success = GetProfileValue_( Path, ProfileName, Registry, Value ) )
			qRReturn;

	ProfileName.Init();

	if ( GetFallbackProfileName_( Registry, ProfileName ) )
		Success = GetProfileValue_( Path, ProfileName, Registry, Value );
qRR
qRT
qRE
	return Success;
}

bso::bool__ frdrgy::GetProfileUIntValue(
	const char *Path,
	const _registry_ &Registry,
	bso::uint__ &Id )
{
	bso::bool__ Success = false;
qRH
	str::string Value;
	sdr::row__ Error = qNIL;
qRB
	Value.Init();

	if ( GetProfileValue( Path, Registry, Value ) ) {
		Id = Value.ToUInt( &Error );

		Success = Error == qNIL;
	}
qRR
qRT
qRE
	return Success;
}

static void SetProfileValue_(
	const char *Path,
	const str::string_ &ProfileName,
	_registry_ &Registry,
	const str::string_ &Value )
{
qRH
str::string WorkPath;
qRB
	if ( ProfileName.Amount() == 0 )
		qRReturn;

	WorkPath.Init( Path );

	WorkPath.Replace( CTAG, 1, ProfileName );

	Registry.SetValue( WorkPath, Value );
qRR
qRT
qRE
}

void frdrgy::SetProfileValue(
	const char *Path,
	_registry_ &Registry,
	const str::string_ &Value )
{
	bso::bool__ Success = false;
qRH
	str::string ProfileName;
qRB
	ProfileName.Init();

	if ( !GetUserProfileName_( Registry, ProfileName ) )
		if ( !GetDefaultProfileName_( Registry, ProfileName ) )
			if ( !GetFallbackProfileName_( Registry, ProfileName ) )
				qRReturn;

	SetProfileValue_( Path, ProfileName, Registry, Value );
qRR
qRT
qRE
}

#endif

