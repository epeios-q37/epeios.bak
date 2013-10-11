/*
	Header for the 'lgg' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2000, 2002-2004 Claude SIMON.

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

//	$Id: lgg.h,v 1.18 2012/11/14 16:06:30 csimon Exp $

#ifndef LGG__INC
#define LGG__INC

#define LGG_NAME		"LGG"

#define	LGG_VERSION	"$Revision: 1.18 $"

#define LGG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &LGGTutor;

#if defined( E_DEBUG ) && !defined( LGG_NODBG )
#define LGG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.18 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:30 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D LanGuaGes 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"

#error "Obsolete ! Use 'LCL' library instead !";

namespace lgg {
	//e Languages.
	enum language__ {
		//i Not really a language : for testing purpose or internal gestion.
		lRaw,
		//i English.
		lEnglish,
		//i French.
		lFrench,
		//i German
		lGerman,
		//i Spanish,
		lSpanish,
		lDutch,
		lPortuguese,
		//i Amount of languages.
		l_amount,
		//i undefined
		l_undefined
	};

	extern const char *LanguageNames[lgg::l_amount];
	extern const char *LanguageLabels[lgg::l_amount];

	//f Return the ID of language named 'Name'.
	language__ GetLanguageIDNamed( const char *Name );

	//f Return the ID of language with label 'Label'.
	language__ GetLanguageIDWithLabel( const char *Label );

	inline bso::bool__ Test( language__ Language ) {
		return ( Language < l_amount );
	}

	inline const char *GetLanguageLabel( language__ Language )
	{
#ifdef LGG_DBG
		if ( !Test( Language ) )
			ERRu();
#endif
		return LanguageLabels[Language];
	}

	inline const char *GetLanguageName( language__ Language )
	{
#ifdef LGG_DBG
		if ( !Test( Language ) )
			ERRu();
#endif

		return LanguageNames[Language];
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
