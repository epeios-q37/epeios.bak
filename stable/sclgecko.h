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

//	$Id: sclgecko.h,v 1.9 2013/06/11 10:32:25 csimon Exp $

#ifndef SCLGECKO_INC_
#define SCLGECKO_INC_

#define SCLGECKO_NAME		"SCLGECKO"

#define	SCLGECKO_VERSION	"$Revision: 1.9 $"

#define SCLGECKO_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( SCLGECKO_NODBG )
# define SCLGECKO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/11 10:32:25 $

/* End of automatic documentation generation part. */

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

	// A dfinir par l'utilisateur.
	xulfui::steering_callback__ *SCLGECKOCreateSteering(
		const char *LauncherIdentification,
		const lcl::locale_ &Locale,
		const char *Language,
		void *UP );	// To overload !

	/*
	NOTA : La destruction du 'xulfui::steering_callback__' retourn ci-dessus, qui retourne en ralit un pointeur sur une classe surchargeant cette classe,
	est dtruit par cette bibliothque, le destructeur de 'xulfui::steering_callback__' tant virtuel.
	*/

	void SetElementsAttributesFromRegistry( nsxpcm::window__ &Window );
}

/*$END$*/
#endif
