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

#ifdef NTVSTR__POSIX
# define strlen_	strlen 
# define strchr_	strchr
# define strrchr_	strrchr
# define strcat_	strcat
# define strcpy_	strcpy
# define SLASH	"/"
#elif defined( NTVSTR__WIN )
# define strlen_	wcslen
# define strchr_	wcschr
# define strrchr_	wcsrchr
# define strcat_	wcscat
# define strcpy_	wcscpy
# define SLASH	L"/"
#else
# error
#endif

nstring___ &ntvstr::nstring___::operator =( const nstring___ &NS )
{
	bso::size__ Size = ( NS._Core == NULL ? 0 : strlen_( NS._Core ) );

	_Core.Malloc( Size + 1 );

	if ( Size != 0 )
		strcpy_( _Core, NS._Core );
	else
		*_Core = 0;

	return *this;
}

void ntvstr::nstring___::Init( const bso::char__ *String )
{
	if ( String == NULL )
		String = "";

	Init();
# ifdef NTVSTR__WIN
	if ( _Core.Size() > INT_MAX )
		ERRFwk();

	// Nécessaire pour les architctures 64 bits (sous Windwos 'int' a une taille de 32 bits, et 'size_t', 64), certaines focntions Windows prentant un 'int'.
	int CoreSize = (int)_Core.Size();

	if ( CoreSize == 0 )
		_Core.Malloc( CoreSize = MultiByteToWideChar( CP_UTF8, 0, String, -1, NULL, 0 ) );

	if ( !MultiByteToWideChar( CP_UTF8, 0, String, -1, _Core, CoreSize )  ) {
		if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			ERRLbr();

		_Core.Malloc( CoreSize = MultiByteToWideChar( CP_UTF8, 0, String, -1, NULL, 0 ) );

		if ( !MultiByteToWideChar( CP_UTF8, 0, String, -1, _Core, CoreSize )  )
			ERRLbr();
	}
# elif defined( NTVSTR__POSIX )
	bso::size__ Size = strlen_( String );

	_Core.Malloc( Size + 1 );

	strcpy_( _Core, String );
# else
#  error
# endif
}


void ntvstr::nstring___::Init( const str::string_ &String )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Init( String.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

const bso::char__ *ntvstr::nstring___::_Convert(
	unsigned int CodePage,
	TOL_CBUFFER___ &Buffer ) const
{
# ifdef NTVSTR__WIN
	if ( Buffer.Size() > INT_MAX )
		ERRLmt();

	// Nécessaire pour les architctures 64 bits (sous Windwos 'int' a une taille de 32 bits, et 'size_t', 64), certaines focntions Windows prentant un 'int'.
	int BufferSize = (int)Buffer.Size();

	if ( BufferSize == 0 )
		Buffer.Malloc( BufferSize = WideCharToMultiByte( CodePage, 0, _Core, -1, NULL, 0, NULL, NULL ) );

	if ( !WideCharToMultiByte( CodePage, 0, _Core, -1, Buffer, BufferSize, NULL, NULL )  ) {
		if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			ERRLbr();

		Buffer.Malloc( BufferSize = WideCharToMultiByte( CodePage, 0, _Core, -1, NULL, 0, NULL, NULL ) );

		if ( !WideCharToMultiByte( CodePage, 0, _Core, -1, Buffer, BufferSize, NULL, NULL ) )
			ERRLbr();
	}
# elif defined( NTVSTR__POSIX )
	bso::size__ Size = strlen( _Core );

	Buffer.Malloc( Size + 1 );

	strcpy_( Buffer, _Core );
# else
#  error
# endif
	return Buffer;
}

const str::string_ &ntvstr::nstring___::_Convert(
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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ntvstrpersonnalization
{
public:
	ntvstrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ntvstrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static ntvstrpersonnalization Tutor;
