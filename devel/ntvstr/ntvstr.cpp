/*
	'ntvstr.cpp' by Claude SIMON (http://zeusw.org/).

	'ntvstr' is part of the Epeios framework.

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

#define NTVSTR__COMPILATION

#include "ntvstr.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "str.h"

using namespace ntvstr;

#ifdef CPE_POSIX
#  include <langinfo.h>
#endif

#ifdef NTVSTR__MBS
# define strlen_	strlen 
# define strchr_	strchr
# define strrchr_	strrchr
# define strcat_	strcat
# define strcpy_	strcpy
# define SLASH	"/"
#elif defined( NTVSTR__WCS )
# define strlen_	wcslen
# define strchr_	wcschr
# define strrchr_	wcsrchr
# define strcat_	wcscat
# define strcpy_	wcscpy
# define SLASH	L"/"
#else
# error
#endif

#ifndef NTVSTR__MBS
void ntvstr::string___::Init( const bso::char__ *String )
{
	if ( String == NULL ) {
		_Core.reset();
		return;
	}

	Init();

	// Ncessaire pour les architectures 64 bits (sous Windwos 'int' a une taille de 32 bits, et 'size_t', 64), certaines fonctions Windows prenant un 'int'.
	int CoreExtent = ( _Core.Extent() > INT_MAX ? INT_MAX : (int)_Core.Extent() );

	if ( CoreExtent == 0 )
		_Core.Malloc( CoreExtent = MultiByteToWideChar( CP_UTF8, 0, String, -1, NULL, 0 ) );

	if ( !MultiByteToWideChar( CP_UTF8, 0, String, -1, _Core, CoreExtent )  ) {
		if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			ERRLbr();

		_Core.Malloc( CoreExtent = MultiByteToWideChar( CP_UTF8, 0, String, -1, NULL, 0 ) );

		if ( !MultiByteToWideChar( CP_UTF8, 0, String, -1, _Core, CoreExtent )  )
			ERRLbr();
	}
}
#endif

void ntvstr::string___::Init( const char__ *String )
{
	if ( String == NULL ) {
		_Core.reset();
		return;
	}

	bso::size__ Size = strlen_( String );

	_Core.Malloc( Size + 1 );

	if ( Size != 0 )
		strcpy_( _Core, String );
	else
		*_Core = 0;
}

void ntvstr::string___::Init( const str::string_ &String )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Init( String.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

const bso::char__ *ntvstr::string___::_Convert(
	unsigned int CodePage,
	TOL_CBUFFER___ &Buffer ) const
{
	if ( _Core == NULL ) {
		Buffer.reset();
		return Buffer;
	}

# ifdef NTVSTR__WCS
	// Ncessaire pour les architectures 64 bits (sous Windows, 'int' a une taille de 32 bits, et 'size_t', 64), certaines fonctions Windows prenant un 'int'.
	int BufferExtent = ( Buffer.Extent() > INT_MAX ? INT_MAX : (int)Buffer.Extent() );

	if ( BufferExtent == 0 )
		Buffer.Malloc( BufferExtent = WideCharToMultiByte( CodePage, 0, _Core, -1, NULL, 0, NULL, NULL ) );

	if ( !WideCharToMultiByte( CodePage, 0, _Core, -1, Buffer, BufferExtent, NULL, NULL )  ) {
		if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			ERRLbr();

		Buffer.Malloc( BufferExtent = WideCharToMultiByte( CodePage, 0, _Core, -1, NULL, 0, NULL, NULL ) );

		if ( !WideCharToMultiByte( CodePage, 0, _Core, -1, Buffer, BufferExtent, NULL, NULL ) )
			ERRLbr();
	}
# elif defined( NTVSTR__MBS )
	bso::size__ Size = strlen( _Core );

	Buffer.Malloc( Size + 1 );

	strcpy_( Buffer, _Core );
# else
#  error
# endif
	return Buffer;
}

const str::string_ &ntvstr::string___::_Convert(
	unsigned int CodePage,
	str::string_ &SBuffer ) const
{
ERRProlog
	TOL_CBUFFER___ CBuffer;
ERRBegin
	SBuffer.Append( _Convert( CodePage, CBuffer ) );
ERRErr
ERREnd
ERREpilog
	return SBuffer;
}
