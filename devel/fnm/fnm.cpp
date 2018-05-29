/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define FNM__COMPILATION

#include "fnm.h"

#include "err.h"
#include "tol.h"
#include "fil.h"
#include "str.h"

using namespace fnm;

const char *fnm::GetLabel( fnm::type__ Type )
{
	switch ( Type ) {
	case fnm::tEmpty:
		return "Empty";
	case fnm::tAbsolute:
		return "Absolute";
	case fnm::tRelative:
		return "Relative";
	case fnm::tSuffixed:
		return "Suffixed";
	case fnm::tFree:
		return "Free";
	case fnm::tDirectory:
		return "Directory";
	case fnm::t_Undefined:
		return "Unknow";
	default:
		qRFwk();
	}

	return NULL;	// To avoid a 'warnin'.
}

#ifdef FNM__POSIX
# define strlen_	strlen 
# define strchr_	strchr
# define strrchr_	strrchr
# define strcat_	strcat
# define strcpy_	strcpy
# define SLASH	"/"
#elif defined( FNM__WIN )
# define strlen_	wcslen
# define strchr_	wcschr
# define strrchr_	wcsrchr
# define strcat_	wcscat
# define strcpy_	wcscpy
# define SLASH	L"/"
#else
# error
#endif

inline static fnm::type__ Type_( const nchar__ *Path )
{
	const nchar__ *Repere = NULL;

	if ( (Path == NULL ) || ( *Path == 0 ) )
		return fnm::tEmpty;

	if ( ( Path[strlen_( Path ) - 1] == ':' )
		 || ( Path[strlen_( Path ) - 1] == '/' )
		 || ( Path[strlen_( Path ) - 1] == '\\' ) )
		 return fnm::tDirectory;

	Repere = strrchr_( Path, ':' );

	if ( Repere != NULL )
		Repere++;
	else
		Repere = Path;


	if ( ( *Repere == '/' ) || ( *Repere == '\\' ) )
		return fnm::tAbsolute;
	else if ( ( strrchr_( Repere, '/' ) != NULL ) || ( ( strrchr_( Repere, '\\' ) != NULL ) ) )
		return fnm::tRelative;

	if ( Repere < strrchr_( Path, '.' ) )
		return fnm::tSuffixed;
	else 
		return fnm::tFree;
}

fnm::type__ fnm::Type( const name___ &Path )
{
	return Type_( Path.Internal() );
}

static const name___ &BuildPath_(
	const nchar__ *Dir,
	const nchar__ *Path,
	const nchar__ *Ext,
	name___ &Result )
{
qRH
	size_t DirSize = 0, PathSize = 0, ExtSize = 0;
qRB
	ncore___ &Core = Result.ExposedInternal();

	if ( Type_( Path ) == tAbsolute )
		Dir = NULL;

	if ( Dir )
		DirSize = strlen_( Dir );

	if ( Path )
		PathSize = strlen_( Path );

	if ( Ext )
		ExtSize = strlen_( Ext );

	Core.Calloc( DirSize + PathSize + ExtSize + 2 );

	if ( ( PathSize == 0 ) && ( ExtSize == 0 ) )
		qRReturn;

	if ( DirSize != 0 ) {
		strcpy_( Core, Dir );

		if ( PathSize != 0 ) {
			switch( Dir[DirSize-1] ) {
			case ':':
			case '/':
			case '\\':
				break;
			default:
				strcat_( Core, SLASH );
				break;
			}
		} else {
			while( --DirSize &&
				   ( ( Core[DirSize] == '/' )
					 || ( Core[DirSize] == '\\' ) ) );

			if ( ( Core[DirSize] == ':' ) || ( DirSize == 0 ) )
				qRReturn;
			else 
				Core[DirSize+1] = 0;
		}
	} else if ( PathSize == 0 )
		qRReturn;

	switch ( Type_( Path ) ) {
	case fnm::tEmpty:
		strcat_( Core, Ext );
		break;
	case fnm::tSuffixed:
	case fnm::tRelative:
	case fnm::tAbsolute:
	case fnm::tFree:
		strcat_( Core, Path );

		if ( ExtSize )
			strcat_( Core, Ext );

		break;
	case fnm::tDirectory:
		strcpy_( Core, Result );
		break;
	default:
		qRFwk();
		break;
	}
qRR
	Result.Init();
qRT
qRE
	return Result;
}

const name___ &fnm::BuildPath(
	const name___ &Dir,
	const name___ &Path,
	const name___ &Ext,
	name___ &Result )
{
	return BuildPath_( Dir.Internal(), Path.Internal(), Ext.Internal(), Result );
}


const nchar__ *fnm::GetBasename( const nchar__ *Filename )
{
	const nchar__ *Repere;

#ifdef FNM_DBG
	if ( *Filename == 0 )
		qRFwk();
#endif

	if ( ( ( Repere = strrchr_( Filename, '/' ) ) == NULL )
		 && ( ( Repere = strrchr_( Filename, '\\' ) ) == NULL ) )
		 if ( ( Repere = strrchr_( Filename, ':' ) ) == NULL )
			 Repere = Filename;
		else
			Repere++;
	else
		Repere++;

	return Repere;
}

const nchar__ *fnm::OldGetExtension( const nchar__ *Name )
{
	return strrchr_( GetBasename( Name ), '.' );
}

const name___ &fnm::Normalize( name___ &Path )
{
	nchar__ *R = Path.Internal();

	R = strchr_( Path.Internal(), '\\' );

	while( R != NULL ) {
		*R = '/';
		R = strchr_( R+1, '\\' );
	}

	return Path;
}

const nchar__ * fnm::NewGetExtension( const nchar__ * Name )
{
	return strrchr_( GetBasename( Name ), '.' ) + 1;
}

const name___ &fnm::GetLocation(
	const nchar__ *Filename,
	name___ &Location )
{
	size_t L = GetBasename( Filename ) - Filename;

	if ( L != 0 ) {
		 Location.ExposedInternal().Malloc( L + 1 );

		memcpy( Location.Internal(), Filename, L * sizeof( nchar__) );

		Location.ExposedInternal()[L] = 0;
	} else
		Location.Init();

	return Location;
}

const name___ &fnm::GetAffix(
	const nchar__ *Filename,
	name___ &Affix )
{
	const nchar__ *Repere = NULL;

	Repere = GetBasename( Filename );

	Affix.ExposedInternal().Malloc( strlen_( Repere ) + 1 );

	strcpy_( Affix.Internal(), Repere );

	if ( strchr_( Affix.Internal(), '.' ) )
		*strchr_( Affix.Internal(), '.' ) = 0;

	return Affix;
}

