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

#ifndef CPE_INC_
# define CPE_INC_

# define CPE_NAME		"CPE"

# if defined( E_DEBUG ) && !defined( CPE_NODBG )
#  define CPE_DBG
# endif

// ComPilation Enviroment.

// Compiler-related macros.

# define CPE_CL_CLANG	"Clang"
# define CPE_CL_GCC		"GCC"
# define CPE_CL_MINGW	"MinGW"
# define CPE_CL_MSC		"MSC"

# define CPE_STRING_SUB_(x) #x
# define CPE_STRING_(x) CPE_STRING_SUB_(x)

# if defined(__clang__)
#  define CPE_C_CLANG
#  define CPE_CL	CPE_CL_CLANG
#  define CPE_VL	CPE_STRING_( __clang_major__ ) "." CPE_STRING_( __clang_minor__ ) "." CPE_STRING_( __clang_patchlevel__ )
# elif defined(__GNUC__) || defined(__GNUG__)
#  define CPE_C_GCC
#  ifdef __MINGW32__
#   define CPE_C_MINGW
#   define CPE_CL	CPE_CL_MINGW
#  elif defined __MINGW64__
#   define CPE_C_MINGW
#   define CPE_CL	CPE_CL_MINGW
#  else
#   define CPE_CL	CPE_CL_GCC
#  endif
#  define CPE_VL	__VERSION__
# elif defined(_MSC_VER)
#  define CPE_C_MSC
#  define CPE_CL	CPE_CL_MSC
#  define CPE_VL	CPE_STRING_( _MSC_VER )
# endif

# ifndef CPE_CL
#  error "Undefined compiler !"
# endif

// OS-related macros.
# define CPE_SL_ANDROID		"Android"
# define CPE_SL_GNULINUX	"GNU/Linux"
# define CPE_SL_FREEBSD	"FreeBSD"
# define CPE_SL_LINUX		"Linux"
# define CPE_SL_IOS			"iOS"
# define CPE_SL_OSX			"macOS"
# define CPE_SL_CYGWIN		"Cygwin"
# define CPE_SL_WIN32		"Win32"
# define CPE_SL_WIN64		"Win64"
# define CPE_SL_POSIX		"Posix"
# define CPE_SL_UNIX		"Unix"
# define CPE_SL_DARWIN		"Darwin"
# define CPE_SL_WIN			"Win"

# ifdef __linux__
#  define CPE_S_LINUX
#  ifdef __ANDROID__
#   define CPE_S_ANDROID
#   define CPE_SL	CPE_SL_ANDROID
#  elif defined( __gnu_linux__ )
#   define CPE_S_GNULINUX
#   define CPE_SL CPE_SL_GNULINUX
#  else
#   define CPE_SL CPE_SL_LINUX
#  endif
# endif

# ifdef __FreeBSD__
#  define CPE_SL CPE_SL_FREEBSD
#  define CPE_S_FREEBSD
# endif

# if defined(__APPLE__) && defined(__MACH__)
#  define CPE_S_DARWIN
#  include <TargetConditionals.h>
#  if TARGET_IPHONE_SIMULATOR == 1
#   define CPE_S_IOS
#   define CPE_SL	CPE_SL_IOS
#  elif TARGET_OS_IPHONE == 1
#   define CPE_S_IOS
#   define CPE_SL	CPE_SL_IOS
#  elif TARGET_OS_MAC == 1
#   define CPE_S_OSX
#   define CPE_SL	CPE_SL_OSX
#  endif
# endif

# if defined(__CYGWIN__) && !defined(_WIN32)
#  define CPE_S_CYGWIN
#  define CPE_SL	CPE_SL_CYGWIN
# endif

# if defined(_WIN64)
#  define CPE_S_WIN
#  define CPE_S_WIN64
#  define CPE_SL	CPE_SL_WIN64
# elif defined(_WIN32)
#  define CPE_S_WIN
#  define CPE_S_WIN32
#  define CPE_SL	CPE_SL_WIN32
# endif

# if !defined( _WIN32 ) && ( defined( __unix__ ) || defined( __unix ) || ( defined( __APPLE__ ) && defined( __MACH__ ) ) )
# define CPE_S_UNIX
# include <unistd.h>
# if defined(_POSIX_VERSION)
#  define CPE_S_POSIX
#  ifndef CPE_SL
#   define CPE_SL CPE_SL_POSIX
#  endif
# else
#  ifndef CPE_SL
#   define CPE_SL CPE_SL_UNIX
#  endif
# endif
#endif

# ifndef CPE_SL
#  error "Undefined OS !"
# endif

// Architecture-related macros.
# define CPE_TL_ARM32	"AArch32"
# define CPE_TL_ARM64	"AArch64"
# define CPE_TL_IA32	"IA-32"
# define CPE_TL_AMD64	"AMD64"
# define CPE_TL_MIPSEL	"MIPSel"

# ifdef __arm__
#  define CPE_A_ARM
#  define CPE_A_ARM32
#  define CPE_F_32BITS
#  define CPE_AL	CPE_TL_ARM32
# endif

# ifdef __aarch64__
#  define CPE_A_ARM
#  define CPE_A_ARM64
#  define CPE_F_64BITS
#  define CPE_AL	CPE_TL_ARM64
# endif

# ifdef __MIPSEL__
#  define CPE_A_MIPSEL
#  define CPE_F_64BITS
#  define CPE_AL	CPE_TL_MIPSEL
# endif


# if defined( __x86_64__ ) || defined( _M_X64 )
#  define CPE_A_AMD64
#  define CPE_F_64BITS
#  define CPE_AL	CPE_TL_AMD64
# elif defined( __i386 ) || defined( _M_IX86 )
#  define CPE_A_IA32
#  define CPE_F_32BITS
#  define CPE_AL	CPE_TL_IA32
# endif

# ifndef CPE_AL
#  error "Undefined architecture !"
# endif

// Target related macros.

# ifdef qST
#  define CPE_F_ST	// single threaded.
# else
#  define CPE_F_MT	// Multi-threaded.
# endif

# ifdef qLIBRARY
#  define CPE_F_LIBRARY
# elif defined( CPE_WIN )
#  if defined( _USRDLL ) || defined( _WINDLL )
#   define CPE_F_LIBRARY
#  endif
# endif

# define CPE_DESCRIPTION	CPE_SL ";" CPE_CL " " CPE_VL ";" CPE_AL

namespace cpe {

	inline const char *GetDescription( void )
	{
		return CPE_DESCRIPTION;
	}

	const char *GetOSDigest( void );
}

#endif
