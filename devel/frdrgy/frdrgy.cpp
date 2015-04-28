/*
	'frdrgy' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'frdrgy' header file ('frdrgy.h').
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
ERRProlog
str::string WorkPath;
ERRBegin
	if ( ProfileName.Amount() == 0 )
		ERRReturn;

	WorkPath.Init( Path );

	WorkPath.Replace( CTAG, 1, ProfileName );

	Success = Registry.GetValue( WorkPath, Value );
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ frdrgy::GetProfileValue(
	const char *Path,
	const _registry_ &Registry,
	str::string_ &Value )
{
	bso::bool__ Success = false;
ERRProlog
	str::string ProfileName;
ERRBegin
	ProfileName.Init();

	if ( GetUserProfileName_( Registry, ProfileName ) )
		if ( Success = GetProfileValue_( Path, ProfileName, Registry, Value ) )
			ERRReturn;

	ProfileName.Init();

	if ( GetDefaultProfileName_( Registry, ProfileName ) )
		if ( Success = GetProfileValue_( Path, ProfileName, Registry, Value ) )
			ERRReturn;

	ProfileName.Init();

	if ( GetFallbackProfileName_( Registry, ProfileName ) )
		Success = GetProfileValue_( Path, ProfileName, Registry, Value );
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ frdrgy::GetProfileUIntValue(
	const char *Path,
	const _registry_ &Registry,
	bso::uint__ &Id )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Value;
	sdr::row__ Error = E_NIL;
ERRBegin
	Value.Init();

	if ( GetProfileValue( Path, Registry, Value ) ) {
		Id = Value.ToUInt( &Error );

		Success = Error == E_NIL;
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

static void SetProfileValue_(
	const char *Path,
	const str::string_ &ProfileName,
	_registry_ &Registry,
	const str::string_ &Value )
{
ERRProlog
str::string WorkPath;
ERRBegin
	if ( ProfileName.Amount() == 0 )
		ERRReturn;

	WorkPath.Init( Path );

	WorkPath.Replace( CTAG, 1, ProfileName );

	Registry.SetValue( WorkPath, Value );
ERRErr
ERREnd
ERREpilog
}

void frdrgy::SetProfileValue(
	const char *Path,
	_registry_ &Registry,
	const str::string_ &Value )
{
	bso::bool__ Success = false;
ERRProlog
	str::string ProfileName;
ERRBegin
	ProfileName.Init();

	if ( !GetUserProfileName_( Registry, ProfileName ) )
		if ( !GetDefaultProfileName_( Registry, ProfileName ) )
			if ( !GetFallbackProfileName_( Registry, ProfileName ) )
				ERRReturn;

	SetProfileValue_( Path, ProfileName, Registry, Value );
ERRErr
ERREnd
ERREpilog
}

#endif

