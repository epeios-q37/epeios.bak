/*
	'fnm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'fnm' header file ('fnm.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: fnm.cpp,v 1.34 2013/04/06 14:50:45 csimon Exp $

#define FNM__COMPILATION

#include "fnm.h"

class fnmtutor
: public ttr_tutor
{
public:
	fnmtutor( void )
	: ttr_tutor( FNM_NAME )
	{
#ifdef FNM_DBG
		Version = FNM_VERSION "\b\bD $";
#else
		Version = FNM_VERSION;
#endif
		Owner = FNM_OWNER;
		Date = "$Date: 2013/04/06 14:50:45 $";
	}
	virtual ~fnmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
	case fnm::tUnknow:
		return "Unknow";
	default:
		return "Undefined";
	}
}

fnm::type__ fnm::Type( const char *Nom )
{
	const char *Repere;

#ifdef FNM_DBG
		if ( Nom == NULL )
			ERRPrm();
#endif

	if ( *Nom == '\0' )
		return fnm::tEmpty;

	if ( ( Nom[strlen( Nom )-1] == ':' )
			|| ( Nom[strlen( Nom )-1] == '/' )
			|| ( Nom[strlen( Nom )-1] == '\\' ) )
		return fnm::tPath;

	Repere = strrchr( Nom, ':' );

	if ( Repere != NULL )
		Repere++;
	else
		Repere = Nom;


	if ( ( *Repere == '/' ) || ( *Repere == '\\' ) )
		return fnm::tAbsolute;
	else if ( ( strrchr( Repere, '/' ) != NULL ) || ( ( strrchr( Repere, '\\' ) != NULL ) ) )
		return fnm::tRelative;

	if ( Repere < strrchr( Nom, '.' ) )
		return fnm::tSuffixed;
	else
		return fnm::tFree;
}

const char *fnm::BuildFileName(
	const char *Rep,
	const char *Nom,
	const char *Ext,
	FNM__P )
{
ERRProlog
	size_t TailleRep = 0, TailleNom = 0, TailleExt = 0;
ERRBegin
	if ( Type( Nom ) == tAbsolute )
		Rep = NULL;

	if ( Rep )
		TailleRep = strlen( Rep );

	if ( Nom )
		TailleNom = strlen( Nom );

	if ( Ext )
		TailleExt = strlen( Ext );

	P.Calloc( TailleRep + TailleNom + TailleExt + 2 );

	if ( ( TailleNom == 0 ) && ( TailleExt == 0 ) ) {
		P.reset();
		ERRReturn;
	}

	if ( TailleRep != 0 ) {
		sprintf( P, "%s", Rep );

		if ( TailleNom != 0 ) {
			switch( Rep[TailleRep-1] ) {
			case ':':
			case '/':
			case '\\':
				break;
			default:
				strcat( P, FNM_DIRECTORY_SEPARATOR_STRING );
				break;
			}
		} else {
			while( --TailleRep &&
				   ( ( P[TailleRep] == '/' )
					 || ( P[TailleRep] == '\\' ) ) );

			if ( ( P[TailleRep] == ':' ) || ( TailleRep == 0 ) ) {
				P.reset();
				ERRReturn;
			} else 
				P[TailleRep+1] = 0;
		}
	} else if ( TailleNom == 0 ) {
		P.reset();
		ERRReturn;
	}

	switch ( Type( Nom ) ) {
	case fnm::tEmpty:
		strcat( P, Ext );
		break;
	case fnm::tSuffixed:
		strcat( P, Nom );
		break;
	case fnm::tRelative:
	case fnm::tAbsolute:
	case fnm::tFree:
		strcat( P, Nom );

		if ( TailleExt )
			strcat( P, Ext );

		break;
	case fnm::tPath:
		strcpy( P, Nom );
		break;
	case fnm::tUnknow:
		P.reset();
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
	P.reset();
ERREnd
ERREpilog
	return P;
}

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

const char *fnm::GetFileNameRoot(
	const char *Nom,
	FNM__P )
{
	const char *Repere = NULL;

	Repere = GetFileName( Nom );

	P.Malloc( strlen( Repere ) + 1 );

	strcpy( P, Repere );

	if ( strchr( P, '.' ) )
		*strchr( P, '.' ) = 0;

	return P;
}

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

const char *fnm::GetLocation(
	const char *Name,
	FNM__P )
{
	size_t L = GetFileName( Name ) - Name;

	if ( L != 0 ) {
		 P.Malloc( L + 1 );

		memcpy( P, Name, L );

		P[L] = 0;
	} else {
		P.Malloc( 1 );

		P[0] = 0;
	}

	return P;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class fnmpersonnalization
: public fnmtutor
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


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static fnmpersonnalization Tutor;

ttr_tutor &FNMTutor = Tutor;
