/*
	'err.cpp' by Claude SIMON (http://zeusw.org/).

	'err' is part of the Epeios framework.

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

#define ERR__COMPILATION

#include "err.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "fnm.h"

using namespace err;

#include "cio.h"

#ifdef ERR__THREAD_SAFE
#	include "mtx.h"
#endif

#include "tol.h"

// Utilis� durant la phase de chargement du programme.
static err::err___ FallbackError_( true );

err::err___ *err::ERRError = &FallbackError_;

#ifdef ERR__THREAD_SAFE
bool err::Concerned( void )
{
	return ( ERRError->ThreadID == tht::GetTID() );
}

void err::Unlock( void )
{
#if 0
#ifdef ERR_DBG	// Doesn't work (is ignored because there was necessary an error before ...)
	if ( !ERR.Error || !err::Concerned() )
		ERRu();
#endif
#endif
	ERRError->Type = err::t_None;

	mtx::Unlock( ERRError->Mutex );
}
#endif

const char *GetFileName_( const char *Path )
{
	const char *Repere;

	if ( ( ( Repere = strrchr( Path, '/' ) ) == NULL )
		&& ( ( Repere = strrchr( Path, '\\' ) ) == NULL ) )
		if ( ( Repere = strrchr( Path, ':' ) ) == NULL )
			Repere = Path;
		else
			Repere++;
	else
		Repere++;

	return Repere;
}

// Retourne une cha�ne ASCII contenant une br�ve description de ce qui est pass� en param�tre.
const char *err::Message(
	const char *Fichier,
	int Ligne,
	err::type Type,
	buffer__ &Buffer )
{
	tol::buffer__ TOLBuffer;

	strcpy( Buffer, tol::Date( TOLBuffer ) );

	strcat( Buffer, " " );

	strcat( Buffer, tol::Time( TOLBuffer ) );

	strcat( Buffer, " " );

	switch( Type ) {
	case err::tAllocation:
		strcat( Buffer, "ALC" );
		break;
	case err::tSystem:
		strcat( Buffer, "SYS" );
		break;
	case err::tVacant:
		strcat( Buffer, "VCT" );
		break;
	case err::tLimitation:
		strcat( Buffer, "LMT" );
		break;
	case err::tData:
		strcat( Buffer, "DTA" );
		break;
	case err::tFramework:
		strcat( Buffer, "FWK" );
		break;
	case err::tParameters:
		strcat( Buffer, "PRM" );
		break;
	case err::tForbidden:
		strcat( Buffer, "FBD" );
		break;
	case err::tLibrary:
		strcat( Buffer, "LBR" );
		break;
	case err::tChecker:
		strcat( Buffer, "CHK" );
		break;
	case t_Free:
		strcat( Buffer, "Free" );
		break;
	case t_Return:
		strcat( Buffer, "Return" );
		break;
	case t_Abort:
		strcat( Buffer, "Abort" );
		break;
	default:
		strcat( Buffer, "Unknown" );
		break;
	}

	strcat( Buffer, " error : " );
	strcat( Buffer, GetFileName_( Fichier ) );
	strcat( Buffer, "(" );
	sprintf( strchr( Buffer, 0 ), "%i", Ligne );
	strcat( Buffer, ")" );

	return Buffer;
}

void err___::reset( bool P )
{
	Line = 0;
	File = NULL;
	Type = err::t_Undefined;
# ifdef ERR__JMPUSE
	Jump = NULL;
# endif

#ifdef ERR__THREAD_SAFE
	if ( P )
		if ( Mutex != MTX_INVALID_HANDLER )
			mtx::Delete( Mutex );

	Mutex = MTX_INVALID_HANDLER;
	ThreadID = THT_UNDEFINED_THREAD_ID;
#endif
}

void err___::Init( void )
{
	reset();

#ifdef ERR__THREAD_SAFE
	Mutex = mtx::Create();
#endif

	Type = t_None;
}

// Handler par d�faut.
void err___::Set(
	const char *Fichier,
	int Ligne,
	err::type Type )
{
#ifdef ERR__THREAD_SAFE
	if ( ( !ERRHit() ) || !err::Concerned() )
	{
		mtx::Lock( Mutex );

		ThreadID = tht::GetTID();
	}
#endif
	if ( Fichier && !ERRHit() )
	{
		if ( Type != t_Undefined )
			this->Type = Type;
		this->File = Fichier;
		this->Line = Ligne;
	} else if ( !ERRHit() )
		ERRPrm();

#if 0
	this->Error = true;
#endif

/* Following lines are here to allowing the insertion of a breakpoint which
is not concerned by the 'ITN' error. Concenrs the whole software, and
not the 'ERR' library, thus the using of 'E_DEBUG' and not 'ERR_DBG'. */
#ifdef E_DEBUG
	if ( ERRFailure() )
		this->Type = ( this->Type == Type ? Type : this->Type );	// Silly, but is only goal is to allow the insertion of a breakpoint.
	else
		this->Type = ( this->Type == Type ? Type : this->Type );	// Silly too, because same goal.
#endif
}

void err___::SetAndLaunch(
	const char *Fichier,
	int Ligne,
	err::type Type )
{
	Set( Fichier, Ligne, Type );

	ERRT();
}

void err::Final( void )
{

	if ( ERRError->Type != err::t_Abort ) {
		buffer__ Buffer;

		const char *Message = err::Message( ERRError->File, ERRError->Line, ERRError->Type, Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

ERRProlog
ERRBegin
	if ( cio::IsInitialized() ) {
		if ( cio::Target() == cio::tConsole ) {
			cio::COut << txf::commit;
			cio::CErr << txf::nl << txf::tab;
		}

		cio::CErr << "{ " << Message << " }";

		if ( cio::Target() == cio::tConsole )
			cio::CErr << txf::nl;

		cio::CErr << txf::commit;
	} else
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	} else
		ERRRst();
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class errpersonnalization
{
public:
	errpersonnalization( void )
	{
		/* place here the actions concerning this library

		to be realized at the launching of the application  */
#if 0
#ifdef ERR__THREAD_SAFE
		Mutex = mtx::Create();
#endif
		ERR.Type = err::t_None;
#endif
	}
	~errpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
#if 0
#ifdef ERR__THREAD_SAFE
		if ( Mutex != MTX_INVALID_HANDLER )
			mtx::Delete( MutexHandler_ );
#endif
#endif
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static errpersonnalization Tutor;
