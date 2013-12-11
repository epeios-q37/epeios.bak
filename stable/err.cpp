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

err_ err::ERR;

#include "cio.h"

#ifdef ERR__THREAD_SAFE
#	include "mtx.h"
#	include "mtk.h"
static mtx::mutex_handler__ MutexHandler_ = MTX_INVALID_HANDLER;
static mtk::thread_id__ ThreadID_;
#endif

#include "tol.h"

/* fin du pré-sous-module */
int err_::Line = 0;
const char *err_::File = NULL;
#ifdef ERR__JMPUSE
jmp_buf *err_::Jump = NULL;
#endif
err::type err_::Type = err::t_None;

#ifdef ERR__THREAD_SAFE
bool err::Concerned( void )
{
	return ( ThreadID_ == mtk::GetTID() );
}

void err::Unlock( void )
{
#if 0
#ifdef ERR_DBG	// Doesn't work (is ignored because there was necessary an error before ...)
	if ( !ERR.Error || !err::Concerned() )
		ERRu();
#endif
#endif
	ERR.Type = err::t_None;

	mtx::Unlock( MutexHandler_ );
}
#endif

// Retourne une chaîne ASCII contenant une brève description de ce qui est passé en paramètre.
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
	strcat( Buffer, fnm::GetFileName( Fichier ) );
	strcat( Buffer, "(" );
	sprintf( strchr( Buffer, 0 ), "%i", Ligne );
	strcat( Buffer, ")" );

	return Buffer;
}


// Handler par défaut.
void err_::Handler(
	const char *Fichier,
	int Ligne,
	err::type Type )
{
#ifdef ERR__THREAD_SAFE
	if ( ( !ERRFailure() ) || !err::Concerned() )
	{
		mtx::Lock( MutexHandler_ );

		ThreadID_ = mtk::GetTID();
	}
#endif
	if ( Fichier && !ERRHit() )
	{
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

	Unlock();

	ERRT();
}
	/* handler de traitement d'erreur; 'Fichier' contient le nom du fichier,
	'Ligne' le numéro de ligne, 'Type', le type de l'erreur */


void err::Final( void )
{

	if ( ERR.Type != err::t_Abort ) {
		buffer__ Buffer;

		const char *Message = err::Message( ERR.File, ERR.Line, ERR.Type, Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

ERRProlog
# ifdef CPE_MT
	cio::cout___ COut;
	cio::cerr___ CErr;
# else
	static txf::text_oflow__ &COut = cio::COut;
	static txf::text_oflow__ &CErr = cio::CErr;
# endif
ERRBegin
	if ( cio::IsInitialized() ) {
# ifdef CPE_MT
		COut.Init();
		CErr.Init();
# endif
		if ( cio::Target() == cio::tConsole ) {
			COut << txf::commit;
			CErr << txf::nl << txf::tab;
		}

		CErr << "{ " << Message << " }";

		if ( cio::Target() == cio::tConsole )
			CErr << txf::nl;

		CErr << txf::commit;
	}
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
#ifdef ERR__THREAD_SAFE
		MutexHandler_ = mtx::Create();
#endif
		ERR.Type = err::t_None;
	}
	~errpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
#ifdef ERR__THREAD_SAFE
		if ( MutexHandler_ != MTX_INVALID_HANDLER )
			mtx::Delete( MutexHandler_ );
#endif
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static errpersonnalization Tutor;
