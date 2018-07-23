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

//	$Id: sclerror.h,v 1.6 2013/07/18 19:46:25 csimon Exp $

#ifndef SCLERROR__INC
#define SCLERROR__INC

#define SCLERROR_NAME		"SCLERROR"

#define	SCLERROR_VERSION	"$Revision: 1.6 $"

#define SCLERROR_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( SCLERROR_NODBG )
#define SCLERROR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:25 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D SoCLe ERROR 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "cio.h"
# include "cpe.h"
# include "mtx.h"
# include "tht.h"

#include "scllocale.h"

namespace sclerror {

	typedef tht::thread_id__ tid__;

	E_ROW( row__ );

	class rError
	{
	public:
		mtx::handler___ Mutex = mtx::Undefined;
		lstbch::E_LBUNCHt( tid__, row__ ) TIds;
		ctn::E_CONTAINERt( lcl::meaning_, row__ ) Meanings;
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Mutex != mtx::Undefined )
					mtx::Delete( Mutex );

			Mutex = mtx::Undefined;
			TIds.reset( P );
			Meanings.reset( P );
		}
		E_CDTOR( rError );
		void Init( void )
		{
			if ( Mutex != mtx::Undefined )
				mtx::Delete( Mutex );

			Mutex = mtx::Create();

			TIds.Init();
			Meanings.Init();
		}
	};

	// A surcharger.
	extern rError *SCLERRORError;

	const lcl::meaning_ &GetMeaning( lcl::meaning_ &Meaning );

	bso::bool__ IsErrorPending( void );

	void ResetPendingError( void );

	void SetMeaning( const lcl::meaning_ &Meaning );

	bso::bool__ GetPendingErrorTranslation(
		const char *Language,
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default );
}

/*$END$*/
#endif
