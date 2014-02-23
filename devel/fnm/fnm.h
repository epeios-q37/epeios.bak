/*
	'fnm.h' by Claude SIMON (http://zeusw.org/).

	'fnm' is part of the Epeios framework.

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

#ifndef FNM__INC
# define FNM__INC

# define FNM_NAME		"FNM"

# if defined( E_DEBUG ) && !defined( FNM_NODBG )
#  define FNM_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// File Name Manager

# include "cpe.h"
# include "tol.h"
# include "strng.h"

# if defined( CPE_POSIX )
#  define FNM__POSIX
# elif defined( CPE_WIN )
#  define FNM__WIN
# else
#  error "Unknown target !"
# endif

// Pour éviter d'avoir à inclure 'str.h'
namespace str {
	class string_;
}

namespace fnm
{
# ifdef FNM__POSIX
	typedef bso::char__ base__;
# elif defined( FNM__WIN )
	typedef wchar_t base__;
# endif

	typedef tol::E_BUFFER___( base__ ) core___;

	typedef tol::E_BUFFER___( bso::char__ ) buffer___;

	class name___
	{
	private:
		core___ _Core;
	public:
		void reset( bso::bool__ P = true )
		{
			_Core.reset( P );
		}
		E_CDTOR( name___ );
		name___( const char *Name )
		{
			reset( false );

			Init( Name );
		}
		name___( const str::string_ &Name )
		{
			reset( false );
			Init( Name );
		}
		name___ &operator =( const name___ &N );
		void Init( void )
		{
			// _Core.Init();	// C'est un buffer, donc pas d'initialisation.
			_Core.Malloc( 1 );
			*_Core = 0;
		}
		void Init( const bso::char__ *Name );
		void Init( const str::string_ &Name );
		void Init( const name___ &Name )
		{
			Init();

			operator =( Name );
		}
		void Forget( void )
		{
			_Core.Forget();
		}
		const bso::char__ *UTF8( TOL_CBUFFER___ &Buffer ) const;
		const str::string_ &UTF8( str::string_ &Buffer ) const;
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
# ifdef FNM__WIN
			return wcslen( _Core );
# elif defined( FNM__POSIX )
			return strlen( _Core );
# else
#  error
# endif
		}
	};


	//e Different type of file name.
	enum type__ {
		// ""
		tEmpty = 1,
		// "x:\...\nom.suf" or "\...\nom.suf" etc..
		tAbsolute,
		// "x:nom.suf" or "../nom.suf" etc..
		tRelative,
		// "nom.suf"
		tSuffixed,
		// "nom"
		tFree,
		// "d:" or "d:\...\directory\" etc.
		tPath,
		t_amount,
		t_Undefined
	};

	//f Type of the file name 'FileName'.
	type__ Type( const name___ &FileName );


#if 0	// Obsolete ?
	#define FNM_BUFFER___	tol::E_BUFFER___( char )
	#define FNM__P	FNM_BUFFER___ &P

#endif

	/* Remplace '\' par '/'. */
	const name___ &CorrectLocation( name___ &Name );

	const char *Description( type__ Type );

	const name___ &BuildFileName(
		const base__ *Dir,
		const base__ *Affix,
		const base__ *Ext,
		name___ &Name );

	inline const name___ &BuildFileName(
		const name___ &Dir,
		const name___ &Affix,
		const name___ &Ext,
		name___ &Name )
	{
		return BuildFileName( Dir.Core(), Affix.Core(), Ext.Core(), Name );
	}

	const base__ *GetFileName( const base__ *LocalizedName );

	const name___ &_Set(
		const base__ *Core,
		name___ &Name );

	inline const name___ &GetFileName(
		const name___ &LocalizedName,
		name___ &Name )
	{
		return _Set( GetFileName( LocalizedName.Core() ), Name );
	}

	const base__ *GetExtension( const base__ *Name );

	inline const name___ &GetExtension(
		const name___ &Name,
		name___ &Extension )
	{
		return _Set( GetExtension( Name.Core() ), Extension );
	}

	const name___ &GetLocation(
		const base__ *Name,
		name___ &Location );

	inline const name___ &GetLocation(
		const name___ &Name,
		name___ &Location )
	{
		return GetLocation( Name.Core(), Location );
	}

	const name___ &GetAffix(
		const base__ *Base,
		name___ &Affix );

	inline const name___ &GetAffix(
		const name___ &Name,
		name___ &Affix )
	{
		return GetAffix( Name.Core(), Affix );
	}

#if 0	// Obsolete ?

	/************************************************/
	/* GESTION DE LA GENERATION D'UN NOM DE FICHIER */
	/************************************************/


	//c Manage 8 character long file name.
	class file_name_manager
	//: public utl_PU
	{
	private:
#ifndef CPE__MT
		/* Return a 8 characters long file name using the 'Base' string (any length)
		with 'Occurence' (>=0 <=36) the present occurence of a 'Base' based file name */
		const char *MakeFileName_(
			const char *base,
			int Occurence = 0 );
#endif
	protected:
		//v Must return 'true' if the file named 'Name' is the searched file..
		virtual bool FNMMatch(	const char *Name ) = 0;
	public:
		//f Initialization.
		void Init( void )
		{
			// for standadization reason.
		}
		/*f Return a 8 characters file name based on the 'Base' string (any lentgh)
		in 'Directory' with 'Extension' as extension. Use 'TOLFileExists' to define
		if this file already exists. If 'NULL' is returned, then no file can be
		generated, because all occurence are already used. */
#ifndef CPE__MT
		const char *SearchFileName(
			const char *Directory,
			const char *Base,
			const char *Extension,
			FNM__P );
#endif
	};
#endif
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const fnm::name___ &Name );


#if 0 // Obsolete ?

#ifdef FNM__WIN
#	define FNM_DIRECTORY_SEPARATOR_STRING	"\\"
#	define FNM_DIRECTORY_SEPARATOR_CHARACTER	'\\'
#elif defined( FNM__POSIX )
#	define FNM_DIRECTORY_SEPARATOR_STRING	"/"
#	define FNM_DIRECTORY_SEPARATOR_CHARACTER	'/'
#else
#	error
#endif

#endif

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
