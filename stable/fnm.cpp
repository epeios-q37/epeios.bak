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
#include "str.h"

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

	return NULL;	// Pour éviter un 'warning'.
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

	_Core.Malloc( Size + 1 );

	if ( Size != 0 )
		strcpy_( _Core, N._Core );
	else
		*_Core = 0;

	return *this;
}

void fnm::name___::Init( const bso::char__ *Name )
{
	if ( Name == NULL )
		Name = "";

	Init();
# ifdef FNM__WIN
	if ( _Core.Size() > INT_MAX )
		ERRFwk();

	// Nécessaire pour les architctures 64 bits (sous Windwos 'int' a une taille de 32 bits, et 'size_t', 64), certaines focntions Windows prentant un 'int'.
	int CoreSize = (int)_Core.Size();

	if ( CoreSize == 0 )
		_Core.Malloc( CoreSize = MultiByteToWideChar( CP_UTF8, 0, Name, -1, NULL, 0 ) );

	if ( !MultiByteToWideChar( CP_UTF8, 0, Name, -1, _Core, CoreSize )  ) {
		if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			ERRLbr();

		_Core.Malloc( CoreSize = MultiByteToWideChar( CP_UTF8, 0, Name, -1, NULL, 0 ) );

		if ( !MultiByteToWideChar( CP_UTF8, 0, Name, -1, _Core, CoreSize )  )
			ERRLbr();
	}
# elif defined( FNM__POSIX )
	bso::size__ Size = strlen_( Name );

	_Core.Malloc( Size + 1 );

	strcpy_( _Core, Name );
# else
#  error
# endif
}


void fnm::name___::Init( const str::string_ &Name )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Init( Name.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

const bso::char__ *fnm::name___::UTF8( TOL_CBUFFER___ &Buffer ) const
{
# ifdef FNM__WIN
	if ( Buffer.Size() > INT_MAX )
		ERRLmt();

	// Nécessaire pour les architctures 64 bits (sous Windwos 'int' a une taille de 32 bits, et 'size_t', 64), certaines focntions Windows prentant un 'int'.
	int BufferSize = (int)Buffer.Size();

	if ( BufferSize == 0 )
		Buffer.Malloc( BufferSize = WideCharToMultiByte( CP_UTF8, 0, _Core, -1, NULL, 0, NULL, NULL ) );

	if ( !WideCharToMultiByte( CP_UTF8, 0, _Core, -1, Buffer, BufferSize, NULL, NULL )  ) {
		if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			ERRLbr();

		Buffer.Malloc( BufferSize = WideCharToMultiByte( CP_UTF8, 0, _Core, -1, NULL, 0, NULL, NULL ) );

		if ( !WideCharToMultiByte( CP_UTF8, 0, _Core, -1, Buffer, BufferSize, NULL, NULL ) )
			ERRLbr();
	}
# elif defined( FNM__POSIX )
	bso::size__ Size = strlen( _Core );

	Buffer.Malloc( Size + 1 );

	strcpy_( Buffer, _Core );
# else
#  error
# endif
	return Buffer;
}

const str::string_ &fnm::name___::UTF8( str::string_ &SBuffer ) const
{
ERRProlog
	TOL_CBUFFER___ CBuffer;
ERRBegin
	SBuffer.Append( UTF8( CBuffer ) );
ERRErr
ERREnd
ERREpilog
	return SBuffer;
}



inline static fnm::type__ Type_( const base__ *Name )
{
	const base__ *Repere = NULL;

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

const name___ &fnm::BuildFileName(
	const base__ *Dir,
	const base__ *Affix,
	const base__ *Ext,
	name___ &Name )
{
ERRProlog
	size_t DirSize = 0, AffixSize = 0, ExtSize = 0;
ERRBegin
	core___ &Core = Name.Core();

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

const name___ &fnm::_Set(
	const base__ *Core,
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

const base__ *fnm::GetFileName( const base__ *Name )
{
	const base__ *Repere;

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

const base__ *fnm::GetExtension( const base__ *Name )
{
	return strrchr_( GetFileName( Name ), '.' );
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

const name___ &fnm::CorrectLocation( name___ &Location )
{
	base__ *R = Location.Core();

	R = strchr_( Location.Core(), '\\' );

	while( R != NULL ) {
		*R = '/';
		R = strchr_( R+1, '\\' );
	}

	return Location;
}

const name___ &fnm::GetLocation(
	const base__ *Name,
	name___ &Location )
{
	size_t L = GetFileName( Name ) - Name;

	if ( L != 0 ) {
		 Location.Core().Malloc( L + 1 );

		memcpy( Location.Core(), Name, L * sizeof( base__) );

		Location.Core()[L] = 0;
	} else
		Location.Init();

	return Location;
}

const name___ &fnm::GetAffix(
	const base__ *Name,
	name___ &Affix )
{
	const base__ *Repere = NULL;

	Repere = GetFileName( Name );

	Affix.Core().Malloc( strlen_( Repere ) + 1 );

	strcpy_( Affix.Core(), Repere );

	if ( strchr_( Affix.Core(), '.' ) )
		*strchr_( Affix.Core(), '.' ) = 0;

	return Affix;
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

