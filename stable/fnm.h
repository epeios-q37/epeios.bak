/*
	Header for the 'fnm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: fnm.h,v 1.34 2013/04/06 14:50:45 csimon Exp $

#ifndef FNM__INC
#define FNM__INC

#define FNM_NAME		"FNM"

#define	FNM_VERSION	"$Revision: 1.34 $"

#define FNM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FNMTutor;

#if defined( E_DEBUG ) && !defined( FNM_NODBG )
#define FNM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.34 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 14:50:45 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D File Name Manager 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D FileName. Handle file names.

# include "cpe.h"
# include "tol.h"
# include "strng.h"

#if defined( CPE_POSIX )
#	define FNM__POSIX
#elif defined( CPE_WIN )
#	define FNM__WIN
#else
#	error "Unknown target !"
#endif

namespace fnm
{
	//e Different type of file name.
	enum type__ {
		//i "" or NULL
		tEmpty = 1,
		//i "x:\...\nom.suf" or "\...\nom.suf" etc..
		tAbsolute,
		//i "x:nom.suf" or "../nom.suf" etc..
		tRelative,
		//i "nom.suf"
		tSuffixed,
		//i "nom"
		tFree,
		//i "d:" or "d:\...\directory\" etc.
		tPath,
		//i unknow or not initialized.
		tUnknow
	};

	//f Type of the file name 'FileName'.
	fnm::type__ Type( const char *FileName );

#define FNM_BUFFER___	tol::E_FPOINTER___( char )
#define FNM__P	FNM_BUFFER___ &P

	/*f Correct location, i. e. remplaces '\' or '/' with correct directory separator depending on OS. */
	const char *CorrectLocation(
		const char *Location,
		FNM__P );


	//f Description of the 'Type' type.
	const char *Description( fnm::type__ Type );

	/*f Make file name with 'Name', 'Directory' as default
	directory, and 'Extension' as defaut extension; 
	IMPORTANT: the returned pointer MUST be freed with 'free()'.*/
	const char *BuildFileName(
		const char *Rep,
		const char *Nom,
		const char *Ext,
		FNM__P );

	/*f Return the name of the file named 'Name', without its localization. */
	inline const char *GetFileName( const char *Name )
	{
		const char *Repere;

#ifdef FNM_DBG
		if ( Name == NULL )
			ERRPrm();
#endif

		if ( ( ( Repere = strrchr( Name, '/' ) ) == NULL )
			&& ( ( Repere = strrchr( Name, '\\' ) ) == NULL ) )
			if ( ( Repere = strrchr( Name, ':' ) ) == NULL )
				Repere = Name;
			else
				Repere++;
		else
			Repere++;

		return Repere;
	}

	inline const char *GetExtension( const char *Name )
	{
		return strrchr( GetFileName( Name ), '.' );
	}


	// Return a string contaiinig the location only.
	const char *GetLocation(
		const char *Name,
		FNM__P );

	//f Return the file name of 'Name' without localization and extension.
	const char *GetFileNameRoot(
		const char *Nom,
		FNM__P );

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
}

#ifdef FNM__WIN
#	define FNM_DIRECTORY_SEPARATOR_STRING	"\\"
#	define FNM_DIRECTORY_SEPARATOR_CHARACTER	'\\'
#elif defined( FNM__POSIX )
#	define FNM_DIRECTORY_SEPARATOR_STRING	"/"
#	define FNM_DIRECTORY_SEPARATOR_CHARACTER	'/'
#else
#	error
#endif



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
