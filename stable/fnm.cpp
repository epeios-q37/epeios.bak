/*
	'fnm.cpp' by Claude SIMON (http://zeusw.org/).

	'fnm' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define FNM__COMPILATION

#include "fnm.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "err.h"
#include "tol.h"
#include "fil.h"

using namespace fnm;

const char *fnm::Description( fnm::type__ Type )
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
	case fnm::tPath:
		return "Path";
	case fnm::t_Undefined:
		return "Unknow";
	default:
		ERRFwk();
	}

	return NULL;	// Pour �viter un 'warning'.
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

name___ &name___::operator =( const name___ &N )
{
	bso::size__ Size = strlen_( N._Core );

	_Core.Malloc( Size );

	if ( Size != 0 )
		memcpy( _Core, N._Core, Size * sizeof( _base__ ) );
	else
		*_Core = 0;

	return *this;
}


inline static fnm::type__ Type_( const _base__ *Name )
{
	const _base__ *Repere = NULL;

	if ( *Name == 0 )
		return fnm::tEmpty;

	if ( ( Name[strlen_( Name )-1] == ':' )
			|| ( Name[strlen_( Name )-1] == '/' )
			|| ( Name[strlen_( Name )-1] == '\\' ) )
		return fnm::tPath;

	Repere = strrchr_( Name, ':' );

	if ( Repere != NULL )
		Repere++;
	else
		Repere = Name;


	if ( ( *Repere == '/' ) || ( *Repere == '\\' ) )
		return fnm::tAbsolute;
	else if ( ( strrchr_( Repere, '/' ) != NULL ) || ( ( strrchr_( Repere, '\\' ) != NULL ) ) )
		return fnm::tRelative;

	if ( Repere < strrchr_( Name, '.' ) )
		return fnm::tSuffixed;
	else 
		return fnm::tFree;
}

fnm::type__ fnm::Type( const name___ &Name )
{
	return Type_( Name.Core() );
}

static const name___ &BuildFileName_(
	const _base__ *Dir,
	const _base__ *Affix,
	const _base__ *Ext,
	name___ &Name )
{
ERRProlog
	size_t DirSize = 0, AffixSize = 0, ExtSize = 0;
ERRBegin
	_core___ &Core = Name.Core();

	if ( Type_( Affix ) == tAbsolute )
		Dir = NULL;

	if ( Dir )
		DirSize = strlen_( Dir );

	if ( Affix )
		AffixSize = strlen_( Affix );

	if ( Ext )
		ExtSize = strlen_( Ext );

	Core.Calloc( DirSize + AffixSize + ExtSize + 2 );

	if ( ( AffixSize == 0 ) && ( ExtSize == 0 ) )
		ERRReturn;

	if ( DirSize != 0 ) {
		strcpy_( Core, Dir );

		if ( AffixSize != 0 ) {
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
				ERRReturn;
			else 
				Core[DirSize+1] = 0;
		}
	} else if ( AffixSize == 0 )
		ERRReturn;

	switch ( Type_( Affix ) ) {
	case fnm::tEmpty:
		strcat_( Core, Ext );
		break;
	case fnm::tSuffixed:
		strcat_( Core, Affix );
		break;
	case fnm::tRelative:
	case fnm::tAbsolute:
	case fnm::tFree:
		strcat_( Core, Affix );

		if ( ExtSize )
			strcat_( Core, Ext );

		break;
	case fnm::tPath:
		strcpy_( Core, Affix );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
	Name.Init();
ERREnd
ERREpilog
	return Name;
}

const name___ &fnm::BuildFileName(
	const name___ &Dir,
	const name___ &Affix,
	const name___ &Ext,
	name___ &Name )
{
	return BuildFileName_( Dir.Core(), Affix.Core(), Ext.Core(), Name );
}

static inline const name___ &Set_(
	const _base__ *Core,
	name___ &Name )
{
	if ( Core == NULL )
		ERRFwk();

	Name.Init();

	if ( *Core != 0 ) {
		Name.Core().Malloc( strlen_( Core ) + 1 );
		strcpy_( Name.Core(), Core );
	}

	return Name;
}

const _base__ *GetFileName_( const _base__ *Name )
{
	const _base__ *Repere;

#ifdef FNM_DBG
	if ( *Name == 0 )
		ERRPrm();
#endif

	if ( ( ( Repere = strrchr_( Name, '/' ) ) == NULL )
		&& ( ( Repere = strrchr_( Name, '\\' ) ) == NULL ) )
		if ( ( Repere = strrchr_( Name, ':' ) ) == NULL )
			Repere = Name;
		else
			Repere++;
	else
		Repere++;

	return Repere;
}

const name___ &fnm::GetFileName(
	const name___ &Name,
	name___ &Affix )
{
	return Set_( GetFileName_( Name.Core() ), Affix );
}

const _base__ *GetExtension_( const _base__ *Name )
{
	return strrchr_( GetFileName_( Name ), '.' );
}

const name___ &fnm::GetExtension(
	const name___ &Name,
	name___ &Extension )
{
	return Set_( GetExtension_( Name.Core() ), Extension );
}

# if 0 // Obsolete ?
#ifndef CPE__MT
const char *fnm::file_name_manager::MakeFileName_(
	const char *Nom,
	int Occurence )
{
	size_t i;
	static char Resultat[9];

	strcpy( Resultat, "00000000" );

	Resultat[7] = (char)( '0' + Occurence );

	i = strlen( Nom );

	while( i-- )
	{
		size_t j = i%7;

		Resultat[j] += Nom[i];

		Resultat[j] &= 0x7f;

		Resultat[j] %= (char)36;

		if ( Resultat[j] > 9 )
			Resultat[j] += (char)7;

		Resultat[j] += '0';
	}

	return Resultat;
}

const char *fnm::file_name_manager::SearchFileName(
	const char *Repertoire,
	const char *Parametres,
	const char *Extension,
	FNM__P )
{
	const char *Nom = NULL;
ERRProlog
	int Occurence;
ERRBegin
	for ( Occurence = 0; Occurence <= 36; Occurence++ )
	{
		Nom = BuildFileName(
			Repertoire,
			MakeFileName_( Parametres, Occurence ),
			Extension,
			P );

		if ( !fil::FileExists( Nom ) || FNMMatch( Nom ) )
			break;

		free( (void *)Nom );
		Nom = NULL;
	}
ERRErr
	P.reset();

	Nom = NULL;
ERREnd
ERREpilog
	return Nom;
}
#endif
#endif

#if 0
const char *fnm::CorrectLocation(
	const char *Location,
	FNM__P )
{
	char *R = NULL;

#ifdef FNM_DBG
		if ( Location == NULL )
			ERRPrm();
#endif

	P.Malloc( strlen( Location ) + 1 );

	strcpy( P, Location );

	R = strpbrk( P, "\\/" );

	while( R != NULL ) {
		*R = FNM_DIRECTORY_SEPARATOR_CHARACTER;
		R = strpbrk( R+1, "\\/" );
	}

	return P;
}
#endif

const name___ &fnm::GetAffix(
	const name___ &Name,
	name___ &Affix )
{
	const _base__ *Repere = NULL;

	Repere = GetFileName_( Name.Core() );

	Affix.Core().Malloc( strlen_( Repere ) + 1 );

	strcpy_( Affix.Core(), Repere );

	if ( strchr_( Affix.Core(), '.' ) )
		*strchr_( Affix.Core(), '.' ) = 0;

	return Affix;
}


const name___ &fnm::GetLocation(
	const name___ &Name,
	name___ &Location )
{
	const _core___ &Core = Name.Core();
	size_t L = GetFileName_( Core ) - Core;

	if ( L != 0 ) {
		 Location.Core().Malloc( L + 1 );

		memcpy( Location.Core(), Core, L * sizeof( _base__) );

		Location.Core()[L] = 0;
	} else
		Location.Init();

	return Location;
}

/* Although in theory this class is inaccessible to the different modules,

it is necessary to personalize it, or certain compiler would not work properly */

class fnmpersonnalization
{
public:
	fnmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fnmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static fnmpersonnalization Tutor;

