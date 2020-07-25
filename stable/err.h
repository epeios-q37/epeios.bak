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

#ifndef ERR_INC_
# define ERR_INC_

# define ERR_NAME		"ERR"

# if defined( E_DEBUG ) && !defined( ERR_NODBG )
#  define ERR_DBG
# endif

// ERRor

/*
	NOTA : Quelque soit le mode dans lequel est compil� un ex�cutable (programme ou biblioth�que dynamique),
	ce module est TOUJOURS compil� en mode 'thread safe', du fait que, m�me si un ex�cutable est 'mono-threading',
	les biblioth�ques dynamiques auxquelles il peut �ventuellement recourir ('plugin', p. ex.) peuvent, elles,
	�tre 'multi-threading', et comme elles partagent le m�me objet 'error' ...
*/

# include <stdio.h>
#include <stdlib.h>

# ifdef ERR_JMPUSE
#  define ERR__JMPUSE
# endif

# ifdef ERR__JMPUSE
#  include <setjmp.h>
# endif

# include "cpe.h"
# include "thtsub.h"

// Pr�d�claration.
namespace mtx {
	struct _mutex__;
}

namespace err {
	typedef char buffer__[150];

	enum handling__ {
		hThrowException,	// Une erreur provoque une exception.
		hUserDefined,		// Le traitement de l'erreur est � la charge de l'utilisateur.
		h_amount,
		h_Undefined,
		h_Default = hThrowException	// Comportement par d�faut.
	};

	enum type {
		tAllocation,	// (qRAlc) Allocation failure.
		tSystem,		// (qRSys) System failure : a C/C++ standard or system function failed but should not, or returned an incoherent value.
		tVacant,		// (qRVct) Call to a vacant function (mainly because it is not implemented yet).
		tLimitation,	// (qRLmt) Limitation overflow.
		tFramework,		// (qRFwk) Misuse of the framework. should not be used outside the framework. Use 'tGeneric' instead.
		tForbidden,		// (qRFbd) Call to a forbidden function.
		tLibrary,		// (qRLbr) A C/C++ standard or system library returns an error.
		tChecker,		// (qRChk) A checking fails.
		tUnexpected,	// A library or module has an unexpected behavior.
		tGeneric,		// (qRGnr) Generic error. Like 'tFramework', but not in the framework.
		t_amount,
		t_None,			// No error.
		t_Free,			// (ERRFree) Not really an error. Allows the use or the error mechanism.
		t_Abort,		// Make the handling of 'ERRAbort()' easier.
		t_Undefined
	};


	class err___ {
	public:
		// line where the error occurs
		int Line;
		// file where the error occurs
		buffer__ File;
		// Type of error.
		err::type Type;
# ifdef ERR__JMPUSE
		// where to jump
		static jmp_buf *Jump;
# endif
		thtsub::sThreadID ThreadID;
		mtx::_mutex__ *Mutex;
		void reset( bool P = true );
		~err___( void )
		{
			reset( false );
		}
		err___( bool Initialize = false )
		{
			reset();

			if ( Initialize )
				Init();
		}
		void Init( void );
		void Set(
			const char *File = nullptr,
			int Line = 0,
			err::type Type = t_Undefined );
		void SetAndLaunch(
			const char *File = nullptr,
			int Line = 0,
			err::type Type = t_Undefined );
	};

	extern err___ *qRRor;

	// If an error occurs, test if the current thread is concerned.
	bool Concerned( void );
	void Unlock( void );

# define ERRCommon( T )	err::qRRor->SetAndLaunch( __FILE__, __LINE__, T )

# define qRAlc()	ERRCommon( err::tAllocation )
# define qRSys()	ERRCommon( err::tSystem )
# define qRVct()	ERRCommon( err::tVacant )
# define qRLmt()	ERRCommon( err::tLimitation )
# define qRFwk()	ERRCommon( err::tFramework )
# define qRFbd()	ERRCommon( err::tForbidden )
# define qRLbr()	ERRCommon( err::tLibrary )
# define qRChk()	ERRCommon( err::tChecker )
# define qRUnx()	ERRCommon( err::tUnexpected )
# define qRGnr()	ERRCommon( err::tGeneric )



# define ERRRst()	{ err::Unlock(); }


# ifdef ERR__JMPUSE
//m Throw the handler.
#  define ERRT()		{longjmp( *err::qRRor->Jump, 1 );}
# else
#  define ERRT()		throw( *err::qRRor )
# endif

//d Error type.
# define ERRType	err::qRRor->Type

//d File in which the error was thrown.
# define ERRFile			err::qRRor->File

//d Line where the error was thrown.
# define ERRLine			err::qRRor->Line

# ifdef ERR__JMPUSE
#  define ERRGetJ()		err::FGetJ( *err::qRRor )
#  define ERRPutJ( J )	err::FSetJ( *err::qRRor, J )
# endif

# ifdef ERR__JMPUSE

// 'Head' : declarations.
#  define qRH	bso::bool__ ERRNoError = true; { jmp_buf ERRJmp, *ERROJmp = ERRGetJ();\
	ERRPutJ( &ERRJmp );

// 'Begin' : section under control.
#  define qRB	if ( !setjmp( ERRJmp ) ) {

// 'Error' : to execute if an error occurs.
#  define qRR		} else { ERRPutJ( ERROJmp ); ERRNoError = false; {

// 'Tail' : to execute, error or not.
#  define qRT		} }

#  define ERRCommonEnd	ERRPutJ( ERROJmp ); }

# else

#  define qRH	bso::bool__ ERRNoError = true; {
// pr�c�de les d�clarations
#  define qRB	try {
// pr�c�de les instructions proprement dites
#  define qRR		} catch ( err::err___ ) { ERRNoError = false; {
// pr�c�de les instructions � effectuer lors d'une erreur
#  define qRT		} }
// pr�c�de les instructions � ex�cuter, erreur ou pas
#  define ERRCommonEnd	}
// boucle la partie de traitement d'erreur

# endif

# define ERRTestEnd		if ( ERRHit() && !ERRNoError && err::Concerned() ) {\
							{

// 'End' : end of error handling bloc.
# define qRE					ERRCommonEnd ERRTestEnd } ERRT();  };
# define qRFE( action )			ERRCommonEnd ERRTestEnd } { action; if ( ERRHit() && err::Concerned() ) ERRRst(); } };
# define qRFH					qRH
# define qRFB					qRB
# define qRFR					qRR
# define qRFT					qRT

# ifdef ERR__JMPUSE
	inline jmp_buf *FGetJ( err___ &ERR_ )
	{
		return ERR_.Jump;
	}

	inline void FSetJ(
		err___ &ERR_,
		jmp_buf *Jump )
	{
		ERR_.Jump = Jump;
	}
# endif

	//f Return the error message which goes along the given parameters
	const char *Message(
		const char *File,
		int Line,
		err::type Type,
		buffer__ &Buffer );

# ifndef ERR_COMPILATION_
	inline const char *Message( buffer__ &Buffer )
	{
		return err::Message( ERRFile, ERRLine, ERRType, Buffer );
	}
# endif

# define ERRFailure()	( ERRType < err::t_amount )

# define ERRHit()	( ERRType != err::t_None )


// Similaire � un simple 'return', mais dans une section surveill� ('qRB'...'qRR'; un simple 'return' poserait probl�me dans une telle section).
# define qRReturn		ERRCommon( err::t_Return )

// Interruption de l'action en cours. Utilis� avec un gestionnaire d'interface �vennementielle, pour revenir rap�dement � la boucle d'attente.
# define qRAbort()		ERRCommon( err::t_Abort )

// Pour profiter du m�canisme de gestion d'erreur, sans qu'il n'y ai r�ellement une erreur dans le sens de cette biblioth�que.
# define qRFree()		ERRCommon( err::t_Free )
}

// Error handling parameter related macros.

// Definition with default value.
# define qRPD	qRPN = err::h_Default

// Naked defintion, without default value.
# define qRPN	err::handling__ qRP

// For testing
# define qRPT ( qRP == err::hThrowException )

// For reference.
# define qRP ErrHandling

// Parameter value for user handled error.
# define qRPU err::hUserDefined

#endif
