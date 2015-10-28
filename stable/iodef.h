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

//	$Id: iodef.h,v 1.5 2012/11/14 16:06:30 csimon Exp $

#ifndef IODEF__INC
#define IODEF__INC

#define IODEF_NAME		"IODEF"

#define	IODEF_VERSION	"$Revision: 1.5 $"

#define IODEF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( IODEF_NODBG )
#define IODEF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:30 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Input/Output DEFinitions. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete"

#include "cpe.h"

namespace iodef {
	//e Open mode.
	enum mode__ {
		//i Unknow.
		mUnknow,
		//i Ecrase le contenu du fichier.
		mRemove,
		//i Ajoute  la fin du fichier.
		mAppend,
		//i Ouvre le fichier ne lecture/criture.
		mReadWrite,
		//i Ouvre le fichier en lecture seule.
		mReadOnly,
		//i Amount of mode.
		m_amount
	};
 	//e Status.
	enum status__ {
		//i Failure.
		sFailure = 0,
		//i Success.
		sSuccess,
		//i Unknow,
		s_Unknow,
		//i Amount of status.
		s_amount
	};
}

/*$END$*/
#endif
