/*
	'ntvstr.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef NTVSTR__INC
# define NTVSTR__INC

# define NTVSTR_NAME		"NTVSTR"

# if defined( E_DEBUG ) && !defined( NTVSTR_NODBG )
#  define NTVSTR_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// NaTiVe STRing

# include "err.h"
# include "flw.h"

# include "cpe.h"
# include "tol.h"
# include "strng.h"

# if defined( CPE_POSIX )
#  define NTVSTR__POSIX
# elif defined( CPE_WIN )
#  define NTVSTR__WIN
# else
#  error "Unknown target !"
# endif

// Pour éviter d'avoir à inclure 'str.h'
namespace str {
	class string_;
}


namespace ntvstr {
# ifdef NTVSTR__POSIX
	typedef bso::char__ base__;
	E_CDEF( unsigned int, UTF8, 0 );
	E_CDEF( unsigned int, Native, 0 );
# elif defined( NTVSTR__WIN )
	typedef wchar_t base__;
	E_CDEF( unsigned int, UTF8, CP_UTF8 );
	E_CDEF( unsigned int, Native, CP_ACP );
# else
#  error
# endif

	typedef tol::E_BUFFER___( base__ ) core___;

	typedef tol::E_BUFFER___( bso::char__ ) buffer___;

	// 'NAtive string' ; 'string' utilisant en interne l'encodage natif. LEs 'str::string_' passés en paramètres sont encodés en 'UTF-8'.
	class nstring___
	{
	private:
		core___ _Core;
		const bso::char__ *_Convert(
			unsigned int CodePage,
			TOL_CBUFFER___ &Buffer ) const;
		const str::string_ &_Convert(
			unsigned int CodePage,
			str::string_ &Buffer ) const;
	public:
		void reset( bso::bool__ P = true )
		{
			_Core.reset( P );
		}
		E_CDTOR( nstring___ );
		nstring___( const char *String )
		{
			reset( false );

			Init( String );
		}
		nstring___( const str::string_ &String )
		{
			reset( false );
			Init( String );
		}
		nstring___ &operator =( const nstring___ &NS );
		void Init( void )
		{
			// _Core.Init();	// C'est un buffer, donc pas d'initialisation.
			_Core.Malloc( 1 );
			*_Core = 0;
		}
		void Init( const bso::char__ *String );
		void Init( const str::string_ &String );
		void Init( const nstring___ &String )
		{
			Init();

			operator =( String );
		}
		void Forget( void )
		{
			_Core.Forget();
		}
		const bso::char__ *UTF8( TOL_CBUFFER___ &Buffer ) const
		{
			return _Convert( ntvstr::UTF8, Buffer );
		}
		const str::string_ &UTF8( str::string_ &Buffer ) const
		{
			return _Convert( ntvstr::UTF8, Buffer );
		}
		const bso::char__ *NativeEncoding( TOL_CBUFFER___ &Buffer ) const
		{
			return _Convert( ntvstr::Native, Buffer );
		}
		const str::string_ &NativeEncoding( str::string_ &Buffer ) const
		{
			return _Convert( ntvstr::Native, Buffer );
		}
		const core___ &Core( void ) const
		{
			return _Core;
		}
		core___ &Core( void )
		{
			return _Core;
		}
		bso::size__ Size( void ) const
		{
			if ( _Core == NULL )
				return 0;
			else
# ifdef NTVSTR__WIN
				return wcslen( _Core );
# elif defined( NTVSTR__POSIX )
				return strlen( _Core );
# else
#  error
# endif
		}
	};


	template <typename b> inline const bso::char__ *ToNativeEncoding(
		const str::string_ &String,
		b &Buffer )
	{
		return nstring___( String ).NativeEncoding( Buffer );
	}
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
