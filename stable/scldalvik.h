/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: scldalvik.h,v 1.3 2012/09/23 19:54:55 csimon Exp $

#ifndef SCLDALVIK__INC
#define SCLDALVIK__INC

#define SCLDALVIK_NAME		"SCLDALVIK"

#define	SCLDALVIK_VERSION	"$Revision: 1.3 $"

#define SCLDALVIK_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( SCLDALVIK_NODBG )
#define SCLDALVIK_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/09/23 19:54:55 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D SoCLe DALVIK 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "jni.h"
# include "dalvik.h"

namespace scldalvik {

	// A dfinir par l'utilisateur.
	dalvik::steering_callback___ *CreateSteering( void );

	// A dfinir par l'utilisateur.
	void DeleteSteering( dalvik::steering_callback___ *Steering );
}

/*$END$*/
#endif
