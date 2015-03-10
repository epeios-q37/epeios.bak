/*
	'err.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef ERR__INC
# define ERR__INC

# define ERR_NAME		"ERR"

# if defined( E_DEBUG ) && !defined( ERR_NODBG )
#  define ERR_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// ERRor

/*
	NOTA : Quelque soit le mode dans lequel est compilé un exécutable (programme ou bibliothèque dynamique),
	ce module est TOUJOURS compilé en mode 'thread safe', du fait que, même si un exécutable est 'mono-threading',
	les bibliothèques dynamiques auxquelles il peut éventuellement recourir ('plugin', p. ex.) peuvent, elles,
	être 'multi-threading', et comme elles partagent le même objet 'error' ...
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

#  include "tht.h"
// Prédéclaration.
namespace mtx {
	struct _mutex__;
}

namespace err {
	typedef char buffer__[150];

	enum handling__ {
		hThrowException,	// Une erreur provoque une exception.
		hUserDefined,		// Le traitement de l'erreur est à la charge de l'utilisateur.
		h_amount,
		h_Undefined,
		h_Default = hThrowException	// Comportement par défaut.
	};

	enum type {
		tAllocation,	// (ERRAlc) Echec d'une allocation de RAM.
		tSystem,		// (ERRSys) Dysfonctionnemnt du système (échec d'une opération qui n'aurait pas dû échouer, ou valeur incohérente retournée par une fonction ).
		tVacant,		// (ERRVct) Appel à une fonctionnalité absente (à priori non encore implémentée).	
		tLimitation,	// (ERRLmt) Dépassement d'une limite.
		tData,			// (ERRDta) Incohérence dans le nombre ou le contenu d'un ensemble de données.
		tFramework,		// (ERRFwk) Incohérence dans l'utilsation faite du 'framework'.
		tParameters,	// (ERRPrm) Incohérence dans les paramètres passées à une méthode/fonction du 'framework'.
		tForbidden,		// (ERRFbd) Appel d'une fonctionnalité non autorisée.
		tLibrary,		// (ERRLbr) Une fonction d'une bibliothèque système a retournée une erreur.
		tChecker,		// (ERRChk) Un test de contrôle a échoué.
		t_amount,
		t_None,			// Signale l'absence d'erreur.
		t_Free,			// (ERRFree) Pas une erreur au sens propre. Permet de profiter du mécanisme de gestion d'erreur.
		t_Return,		// Facilite la gestion d'un 'ERRReturn'.
		t_Abort,		// Facilite la gestion d'un 'ERRAbort()'.
		t_Undefined
	};


	class err___ {
	public:
		// line where the error occurs
		int Line;
		// file where the error occurs
		const char *File;
		// Type of error.
		err::type Type;
# ifdef ERR__JMPUSE
		// where to jump
		static jmp_buf *Jump;
# endif
		tht::thread_id__ ThreadID;
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
			const char *File = NULL,
			int Line = 0,
			err::type Type = t_Undefined );
		void SetAndLaunch(
			const char *File = NULL,
			int Line = 0,
			err::type Type = t_Undefined );
	};

	extern err___ *ERRError;

	void Final( void );

	// If an error occurs, test if the current thread is concerned.
	bool Concerned( void );
	void Unlock( void );

# define ERRCommon( T )	err::ERRError->SetAndLaunch( __FILE__, __LINE__, T )

# define ERRAlc()	ERRCommon( err::tAllocation )
# define ERRSys()	ERRCommon( err::tSystem )
# define ERRVct()	ERRCommon( err::tVacant )
# define ERRLmt()	ERRCommon( err::tLimitation )
# define ERRDta()	ERRCommon( err::tData )
# define ERRFwk()	ERRCommon( err::tFramework )
# define ERRPrm()	ERRCommon( err::tParameters )
# define ERRFbd()	ERRCommon( err::tForbidden )
# define ERRLbr()	ERRCommon( err::tLibrary )
# define ERRChk()	ERRCommon( err::tChecker )



# define ERRRst()	{ err::Unlock(); }


# ifdef ERR__JMPUSE
//m Throw the handler.
#  define ERRT()		{longjmp( *err::ERRError->Jump, 1 );}
# else
#  define ERRT()		throw( *err::ERRError )
# endif

//d Error type.
# define ERRType	err::ERRError->Type

//d File in which the error was thrown.
# define ERRFile			err::ERRError->File

//d Line where the error was thrown.
# define ERRLine			err::ERRError->Line

# ifdef ERR__JMPUSE
#  define ERRGetJ()		err::FGetJ( *err::ERRError )
#  define ERRPutJ( J )	err::FSetJ( *err::ERRError, J )
# endif

# ifdef ERR__JMPUSE

//d Put the declaration after this.
#  define ERRProlog	bso::bool__ ERRNoError = true; { jmp_buf ERRJmp, *ERROJmp = ERRGetJ();\
	ERRPutJ( &ERRJmp );

//d Put the instructions to survey after this.
#  define ERRBegin	if ( !setjmp( ERRJmp ) ) {

//d Put the instruction to launch if an error occurs.
#  define ERRErr		} else { ERRPutJ( ERROJmp ); ERRNoError = false;

//d Put the instruction to launch, error or not.
#  define ERREnd		}

#  define ERRCommonEpilog	ERRPutJ( ERROJmp ); }

# else

#  define ERRProlog	bso::bool__ ERRNoError = true; {
// précède les déclarations
#  define ERRBegin	try {
// précède les instructions proprement dites
#  define ERRErr		} catch ( err::err___ ) { ERRNoError = false;
// précède les instructions à effectuer lors d'une erreur
#  define ERREnd		}
// précède les instructions à exécuter, erreur ou pas
#  define ERRCommonEpilog	}
// boucle la partie de traitement d'erreur

# endif

# define ERRTestEpilog	if ( ERRHit() && !ERRNoError && err::Concerned() ) {\
							if ( ERRType == err::t_Return ) {\
								ERRRst()

//d End of the error bloc.
# define ERREpilog	ERRCommonEpilog ERRTestEpilog } else ERRT();  };
# define ERRFEpilog	ERRCommonEpilog ERRTestEpilog } else err::Final(); };
# define ERRFProlog	ERRProlog
# define ERRFBegin	ERRBegin
# define ERRFErr		ERRErr
# define ERRFEnd		ERREnd

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

# ifndef ERR__COMPILATION
	inline const char *Message( buffer__ &Buffer )
	{
		return err::Message( ERRFile, ERRLine, ERRType, Buffer );
	}
# endif

# define ERRFailure()	( ERRType < err::t_amount )

# define ERRHit()	( ERRType != err::t_None )


// Similaire à un simple 'return', mais dans une section surveillé ('ERRBegin'...'ERRErr'; un simple 'return' poserait problème dans une telle section).
# define ERRReturn		ERRCommon( err::t_Return )

// Interruption de l'action en cours. Utilisé avec un gestionnaire d'interface évennementielle, pour revenir rapîdement à la boucle d'attente.
# define ERRAbort()		ERRCommon( err::t_Abort )

// Pour profiter du mécanisme de gestion d'erreur, sans qu'il n'y ai réellement une erreur dans le sens de cette bibliothèque.
# define ERRFree()		ERRCommon( err::t_Free )
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
