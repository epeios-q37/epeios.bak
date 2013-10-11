/*
	Header for the 'scllocale' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: scllocale.h,v 1.14 2013/05/22 17:40:11 csimon Exp $

#ifndef SCLLOCALE__INC
#define SCLLOCALE__INC

#define SCLLOCALE_NAME		"SCLLOCALE"

#define	SCLLOCALE_VERSION	"$Revision: 1.14 $"

#define SCLLOCALE_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLLOCALETutor;

#if defined( E_DEBUG ) && !defined( SCLLOCALE_NODBG )
#define SCLLOCALE_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.14 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/05/22 17:40:11 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe LOCALE 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace scllocale {

	const lcl::locale_ &GetLocale( void );

	inline const str::string_ &GetTranslation(
		const char *Text,
		const char *Language,
		str::string_ &Translation )
	{
		return GetLocale().GetTranslation( Text, Language, Translation );
	}

	inline const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		const char *Language,
		str::string_ &Translation )
	{
		return GetLocale().GetTranslation( Meaning, Language, Translation );
	}

	void Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath,
		utf::format__ Format );	// Chargement de la locale de base, propre au logiciel.

	lcl::level__ Push(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath,
		utf::format__ Format,
		rgstry::context___ &Context );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
