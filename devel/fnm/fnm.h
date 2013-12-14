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
# include "bch.h"

# if defined( CPE_POSIX )
#  define FNM__POSIX
# elif defined( CPE_WIN )
#  define FNM__WIN
# else
#  error "Unknown target !"
# endif

namespace fnm
{
# ifdef FNM__POSIX
	typedef bso::char__ base__;
# elif defined( FNM__WIN )
	typedef wchar_t _base__;
# endif

	typedef tol::E_BUFFER___( _base__ ) _core___;

	class name___
	{
	private:
		_core___ _Core;
	public:
		void reset( bso::bool__ P = true )
		{
			_Core.reset( P );
		}
		E_CVDTOR( name___ );
		name___ &operator =( const name___ &N );
		void Init( void )
		{
			// _Core.Init();	// C'st un buffer, donc pas d'initialisation.
			_Core.Malloc( 1 );
			*_Core = 0;
		}
		const _core___ &Core( void ) const
		{
			return _Core;
		}
		_core___ &Core( void )
		{
			return _Core;
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
	fnm::type__ Type( const name___ &FileName );


#if 0	// Obsolete ?
	#define FNM_BUFFER___	tol::E_BUFFER___( char )
	#define FNM__P	FNM_BUFFER___ &P


	/*f Correct location, i. e. remplaces '\' or '/' with correct directory separator depending on OS. */
	const name___ &CorrectLocation( name___ &Name );
#endif

	//f Description of the 'Type' type.
	const char *Description( fnm::type__ Type );

	/*f Make file name with 'Name', 'Directory' as default
	directory, and 'Extension' as defaut extension; 
	IMPORTANT: the returned pointer MUST be freed with 'free()'.*/
	const name___ &BuildFileName(
		const name___ &Rep,
		const name___ &Nom,
		const name___ &Ext,
		name___ &Name );

	/*f Return the name of the file named 'Name', without its localization. */
	inline const name___ &GetFileName(
		const name___ &LocalizedName,
		name___ &Name );

	const name___ &GetExtension(
		const name___ &Name,
		name___ &Extension );


	// Return a string contaiinig the location only.
	const name___ &GetLocation(
		const name___ &Name,
		name___ &Location );

	//f Return the file name of 'Name' without localization and extension.
	const name___ &GetAffix(
		const name___ &Name,
		name___ &Affix );

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
