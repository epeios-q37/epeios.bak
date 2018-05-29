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

#define ERR__COMPILATION

#include "err.h"

using namespace err;

#include "mtx.h"
#include "tol.h"
#include "tht.h"
#include "fnm.h"
#include "cio.h"

// Utilis durant la phase de chargement du programme.
static err::err___ FallbackError_( true );

err::err___ *err::qRRor = &FallbackError_;

#ifdef ERR__JMPUSE
jmp_buf *err::err___::Jump;
#endif

bool err::Concerned( void )
{
	return ( qRRor->ThreadID == tht::GetTID() );
}

void err::Unlock( void )
{
#if 0
#ifdef ERR_DBG	// Doesn't work (is ignored because there was necessary an error before ...)
	if ( !ERR.Error || !err::Concerned() )
		ERRu();
#endif
#endif
	qRRor->Type = err::t_None;

	mtx::Unlock( qRRor->Mutex );
}

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

// Retourne une chane ASCII contenant une brve description de ce qui est pass en paramtre.
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
	case err::tFramework:
		strcat( Buffer, "FWK" );
		break;
	case err::tForbidden:
		strcat( Buffer, "FBD" );
		break;
	case err::tGeneric:
		strcat( Buffer, "GNR" );
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
	File[0] = 0;
	Type = err::t_Undefined;
# ifdef ERR__JMPUSE
	Jump = NULL;
# endif

	if ( P )
		if ( Mutex != MTX_INVALID_HANDLER )
			mtx::Delete( Mutex );

	Mutex = MTX_INVALID_HANDLER;
	ThreadID = THT_UNDEFINED_THREAD_ID;
}

void err___::Init( void )
{
	reset();

	Mutex = mtx::Create();

	Type = t_None;
}

// Handler par dfaut.
void err___::Set(
	const char *Fichier,
	int Ligne,
	err::type Type )
{
	if ( ERRHit() && ( Type == err::t_Return ) ) {
		cio::CErr << __LOC__ " : Using 'qRReturn' when a error already in progress is actually not handled correctly !!!" << txf::nl << txf::commit;
		abort();
	}

	if ( ( !ERRHit() ) || !err::Concerned() )
	{
		if ( Mutex == NULL ) {
			// In this situation, there will be a stack overflow. So we launch a SEGFAULT to facilitate the debugging.
			*(volatile int *)NULL = 0;
		}

		mtx::Lock( Mutex );

		ThreadID = tht::GetTID();
	}

	if ( Fichier && !ERRHit() )
	{
		if ( Type != t_Undefined )
			this->Type = Type;
		if ( strlen( Fichier ) >= sizeof( this->File ) )
			qRLmt();
		strcpy( this->File, Fichier );
		this->Line = Ligne;
	} else if ( !ERRHit() )
		qRFwk();

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
