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

#ifndef FNM_INC_
# define FNM_INC_

# define FNM_NAME		"FNM"

# if defined( E_DEBUG ) && !defined( FNM_NODBG )
#  define FNM_DBG
# endif

// File Name Manager

/*
	Significations des diff�rents termes :
	- 'path' pour, de mani�re g�n�rique, un fichier ou un r�pertoire,
	- 'directory' (diminutif : 'dir') pour un r�pertoire,
	- 'filename' pour un nom de fichier, avec ou sans sa localisation,
	- 'basename' pour un nom de fichier sans sa localisation, mais avec son �ventuellle extension,
	- 'location' la localisation (la liste des parents) d'un fichier,
	- 'affix' le 'basename' d'un fichier sans son extension.
*/

# include "cpe.h"
# include "tol.h"
# include "strng.h"
# include "ntvstr.h"

# if defined( CPE_S_POSIX )
#  define FNM__POSIX
# elif defined( CPE_S_WIN )
#  define FNM__WIN
# else
#  error "Unknown target !"
# endif

/**************/
/**** OLD *****/
/**************/

namespace fnm {
	typedef ntvstr::char__ nchar__;
	typedef ntvstr::core___ ncore___;
	typedef ntvstr::string___ name___;

	// Les diff�rents type d'un chemin ('path').
	enum type__
	{
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
		tDirectory,
		t_amount,
		t_Undefined
	};

	// Type d'un chemin.
	type__ Type( const name___ &Path );

	/* Remplace '\' par '/'. */
	const name___ &Normalize( name___ &Path );

	const char *GetLabel( type__ Type );

	const name___ &BuildPath(
		const name___ &Dir,
		const name___ &Path,
		const name___ &Ext,
		name___ &Result );

	const nchar__ *GetBasename( const nchar__ *Filename );

	inline const name___ &GetBasename(
		const name___ &Filename,
		name___ &Basename )
	{
		return Basename = GetBasename( Filename.Internal() );
	}

	// Extension WITH heading '.'.
	const nchar__ *OldGetExtension( const nchar__ *Name );

	// Extension WITHOUT heading '.'.
	const nchar__ *NewGetExtension( const nchar__ *Name );

	inline const name___ &OldGetExtension(
		const name___ &Name,
		name___ &Extension )
	{
		return Extension = OldGetExtension( Name.Internal() );
	}

	inline const name___ &NewGetExtension(
		const name___ &Name,
		name___ &Extension )
	{
		return Extension = NewGetExtension( Name.Internal() );
	}

	const name___ &GetLocation(
		const nchar__ *Filename,
		name___ &Location );

	inline const name___ &GetLocation(
		const name___ &Filename,
		name___ &Location )
	{
		return GetLocation( Filename.Internal(), Location );
	}

	const name___ &GetAffix(
		const nchar__ *Filename,
		name___ &Affix );

	inline const name___ &GetAffix(
		const name___ &Filename,
		name___ &Affix )
	{
		return GetAffix( Filename.Internal(), Affix );
	}
}

/**************/
/**** NEW *****/
/**************/

namespace fnm {
	typedef name___ rName;

	using ntvstr::Empty;
}


#endif
