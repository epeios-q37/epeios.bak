/*
	Header for the 'sclgecko' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20012004 Claude SIMON.

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

//	$Id: sclgecko.h,v 1.9 2013/06/11 10:32:25 csimon Exp $

#ifndef SCLGECKO__INC
#define SCLGECKO__INC

#define SCLGECKO_NAME		"SCLGECKO"

#define	SCLGECKO_VERSION	"$Revision: 1.9 $"

#define SCLGECKO_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLGECKOTutor;

#if defined( E_DEBUG ) && !defined( SCLGECKO_NODBG )
# define SCLGECKO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/11 10:32:25 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe GECKO component 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"

# include "xulfui.h"
# include "sclxslui.h"

namespace sclgecko {
	using namespace sclxslui;

	// A définir par l'utilisateur.
	extern const char *SCLGECKOProgramName;

	// A définir par l'utilisateur.
	xulfui::steering_callback__ *SCLGECKOCreateSteering(
		const char *LauncherIdentification,
		const lcl::locale_ &Locale,
		const char *Language,
		void *UP );	// To overload !

	/*
	NOTA : La destruction du 'xulfui::steering_callback__' retourné ci-dessus, qui retourne en réalité un pointeur sur une classe surchargeant cette classe,
	est détruit par cette bibliothèque, le destructeur de 'xulfui::steering_callback__' étant virtuel.
	*/

	void SetElementsAttributesFromRegistry( nsxpcm::window__ &Window );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
