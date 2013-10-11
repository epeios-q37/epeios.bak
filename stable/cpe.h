/*
	Header for the 'cpe' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: cpe.h,v 1.52 2013/07/18 19:46:24 csimon Exp $

#ifndef CPE__INC
#define CPE__INC

#define CPE_NAME		"CPE"

#define	CPE_VERSION	"$Revision: 1.52 $"

#define CPE_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CPETutor;

#if defined( E_DEBUG ) && !defined( CPE_NODBG )
#define CPE_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.52 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/07/18 19:46:24 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D ComPilation Enviroment 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//#include "err.h"
//#include "flo.h"


# undef CPE_MSVC	// Environnement Microsoft Visual C++.
# define CPE_MSVC_LABEL		"MSVC"

# undef CPE_XCODE		// Environnement Xcode (MacOS, iOS).
# define CPE_XCODE_LABEL	"Xcode"

# undef CPE_CYGWIN	// Environnement CYGWIN.
# define CPE_CYGWIN_LABEL	"Cygwin"

# undef CPE_MINGW	// Environnement MINGW.
# define CPE_MINGW_LABEL	"MinGW"

# undef CPE_LINUX	// Environnement Linux
# define CPE_LINUX_LABEL	"Linux"

# undef CPE_ANDROID	// Environnement Android.
# define CPE_ANDROID_LABEL	"Android"


# undef CPE_WIN		// Windows système base API
# undef CPE_POSIX	// Posix système base API.

# undef CPE_VC		// Compilateur VC++
# undef CPE_VC6		// Compilateur VC++ V6
# undef CPE_VC8		// Compilateur VC++ V8 (2005)
# undef CPE_VC9		// Compilateur VC++ V9 (2008)

# undef CPE_GCC		// Compilateur GNU C++.
# undef CPE_GCC3	// Compilateur GNU C++ V3.x

# undef CPE_MT			// Cible multitâche.
# undef CPE_CONSOLE		// Cible console.
# undef CPE_LIBRARY		// Cible bibliothèque (DLL, .so, ...)

# undef CPE_JAVA	// Cible composant java en code natif.
# undef CPE_GECKO	// Cible composant Gecko.

# undef CPE_INT		// Contient la taille (en bits) d'un entier naturel.

# undef CPE_INT64	// Taille naturelle d'un entier est de 64 bits.
# undef CPE_INT32	// Taille naturelle d'un entier est de 32 bits.


# undef CPE_X86		// Processeur IA-32 compatibles.
# define CPE_X86_LABEL	"IA-32"

# undef CPE_X64		// Processeur AMD 64 bits et compatibles.
# define CPE_X64_LABEL	"x86-64"

# undef CPE_ARM		// Processeur ARM.
# define CPE_ARM_LABEL "ARM"


# ifdef E_GECKO
#  define CPE_GECKO
#  define CPE_LIBRARY
# endif

# ifdef E_JAVA
#  define CPE_JAVA
#  define CPE_LIBRARY
# endif

# ifdef _MSC_VER
#  define CPE_MSVC
#  define CPE_WIN
#  define CPE_VC
#  if defined( _M_IX86 )
#   define CPE_X86
#  elif defined( _M_X64 )
#   define CPE_X64
#  else
#   error "Unable to guess the architecture."
#  endif
#  ifdef _MT
#   ifndef E_ST
#    define CPE_MT
#   endif
#  elif defined( E_MT )
#   error "'E_MT' is defined, but compiler options does not allow multitasking features."
#  endif
# endif

# ifdef __APPLE__
#  define CPE_XCODE
#  define CPE_GCC
#  define CPE_POSIX
# endif

# ifdef __GNUC__
#  define CPE_GCC
#  ifdef __CYGWIN__
#   define CPE_CYGWIN
#   define CPE_POSIX
#  elif defined( __MINGW32__ )
#   define CPE_MINGW
#   define CPE_WIN
#  elif defined( __linux__ )
#   define CPE_LINUX
#   define CPE_POSIX
#  endif
#  ifdef __ANDROID__
#   define CPE_ANDROID
#  endif
# endif

# ifndef CPE_MT
#  ifdef E_MT 
#   define CPE_MT
#  endif
# endif

# ifndef E_USE_NO_WORKAROUNDS
#  define CPE_USE_WORKAROUNDS
# endif	

# ifdef E_LIBRARY
#  define CPE_LIBRARY
# elif defined( CPE_MSVC )
#  if defined( _USRDLL ) || defined( _WINDLL )
#   define CPE_LIBRARY
#  endif
# endif

# ifdef E_GUI
#  define CPE_GUI
# elif !defined( CPE_LIBRARY )
#  define CPE_CONSOLE
# endif

# ifndef E_DISABLE_WARNINNG_SUPPRESSION
#  define CPE_WARNING_SUPPRESSION_ENABLED
# endif

# ifdef CPE_VC
#  ifdef CPE_WARNING_SUPPRESSION_ENABLED
#   pragma warning( disable: 4786 )	// 'The identifier string exceeded the maximum allowable length and was truncated.'.
#  endif
#  if _MSC_VER == 1200
#   define CPE_VC6
#  elif _MSC_VER == 1400
#   define CPE_VC8
#  elif _MSC_VER == 1500
#   define CPE_VC9
#  endif
# endif

# ifdef CPE_GCC
#  if __GNUC__ == 3
#   define CPE_GCC3
#  endif
#  if defined( __i386__ )
#   define CPE_X86
#  elif defined( __x86_64__ )
#   define CPE_X64
#  elif defined( __ARM_EABI__ )
#   define CPE_ARM
#  else
#   error "Unable to guess architecture."
#  endif
# endif

# ifdef CPE_JAVA
#  ifdef CPE_CYGWIN
#   error "JNI doesn't work with 'Cygwin' genuine compiler. Use 'MinGW' compiler instead."
#  endif
# endif


# ifdef CPE_X86
#  ifdef CPE_INT
#  error "'CPE_INT' should not be already defined."
#  endif
#  define CPE_INT	32
# endif

# ifdef CPE_X64
#  ifdef CPE_INT
#  error "'CPE_INT' should not be already defined."
#  endif
#  define CPE_INT	64
# endif

# ifdef CPE_ARM
#  ifdef CPE_INT
#  error "'CPE_INT' should not be already defined."
#  endif
#  define CPE_INT	32
# endif

# ifndef CPE_INT
#  error "Missing 'CPE_INT' definition."
# endif

# ifdef E_INT32
#  undef CPE_INT
#  define CPE_INT	32
# endif
 
# ifdef E_INT64
#  undef CPE_INT
#  define CPE_INT	64
# endif

# if CPE_INT == 32
#  define CPE_INT32
# elif ( CPE_INT == 64 )
#  define CPE_INT64
# elif defined( CPE_INT )
#  error "Wrong value for 'CPE_INT'."
# else
#  error "'CPE_INT' not defined"
# endif
 

# if defined( CPE_MSVC )
#  define CPE_ENVIROMENT_LABEL	CPE_MSVC_LABEL
# elif defined( CPE_XCODE )
#  define CPE_ENVIROMENT_LABEL	CPE_XCODE_LABEL
# elif defined( CPE_CYGWIN )
#  define CPE_ENVIROMENT_LABEL	CPE_CYGWIN_LABEL
# elif defined( CPE_MINGW )
#  define CPE_ENVIROMENT_LABEL	CPE_MINGW_LABEL
# elif defined( CPE_LINUX )
#  define CPE_ENVIROMENT_LABEL	CPE_LINUX_LABEL
# elif defined( CPE_ANDROID )
#  define CPE_ENVIROMENT_LABEL	CPE_ANDROID_LABEL
# else
#  error "Undefined enviroment !"
# endif

# if defined( CPE_X86 )
#  define CPE_ARCHITECTURE_LABEL	CPE_X86_LABEL
# elif defined( CPE_X64 )
#  define CPE_ARCHITECTURE_LABEL	CPE_X64_LABEL
# elif defined( CPE_ARM )
#  define CPE_ARCHITECTURE_LABEL	CPE_ARM_LABEL
# else
#  error "Undefined architecture !"
# endif


namespace cpe {

	const char *GetDescription( void );

}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
