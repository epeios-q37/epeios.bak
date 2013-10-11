/*
	Header for the 'sclrgstry' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: sclrgstry.h,v 1.29 2013/06/18 09:36:04 csimon Exp $

#ifndef SCLRGSTRY__INC
#define SCLRGSTRY__INC

#define SCLRGSTRY_NAME		"SCLRGSTRY"

#define	SCLRGSTRY_VERSION	"$Revision: 1.29 $"

#define SCLRGSTRY_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLRGSTRYTutor;

#if defined( E_DEBUG ) && !defined( SCLRGSTRY_NODBG )
#define SCLRGSTRY_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.29 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/18 09:36:04 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe ReGiSTRY 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "rgstry.h"

# define SCLREGSITRY_TAG_MARKER_STRING	"%"
# define SCLREGSITRY_TAG_MARKER_CHAR	'%'

namespace sclrgstry {

	using rgstry::tags_;
	using rgstry::tags;

//	extern const lcl::rack__ *LocaleRack;	// A définir.

	const rgstry::registry_ &GetRegistry( void );

	rgstry::row__ GetRoot( void );

	extern rgstry::entry___ Parameters;

	extern rgstry::entry___ Language;

	extern rgstry::entry___ Locale;	// Du fichier de configuration uniquement.

	using rgstry::value;
	using rgstry::value_;

	void Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	bso::bool__ IsReady( void );

	void ReportBadOrNoValueForEntryError( const rgstry::tentry__ &Entry );


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
