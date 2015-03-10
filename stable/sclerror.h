/*
	Header for the 'sclerror' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: sclerror.h,v 1.6 2013/07/18 19:46:25 csimon Exp $

#ifndef SCLERROR__INC
#define SCLERROR__INC

#define SCLERROR_NAME		"SCLERROR"

#define	SCLERROR_VERSION	"$Revision: 1.6 $"

#define SCLERROR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLERRORTutor;

#if defined( E_DEBUG ) && !defined( SCLERROR_NODBG )
#define SCLERROR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:25 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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

#include "scllocale.h"

namespace sclerror {

	namespace {
		typedef tht::thread_id__ _tid__;

		E_ROW( _row__ );
	}

	class error___
	{
	public:
		mtx::handler___ Mutex = mtx::UndefinedHandler;
		lstbch::E_LBUNCHt( _tid__, _row__ ) TIds;
		ctn::E_CONTAINERt( lcl::meaning_, _row__ ) Meanings;
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Mutex != mtx::UndefinedHandler )
					mtx::Delete( Mutex );

			Mutex = mtx::UndefinedHandler;
			TIds.reset( P );
			Meanings.reset( P );
		}
		E_CDTOR( error___ );
		void Init( void )
		{
			if ( Mutex != mtx::UndefinedHandler )
				mtx::Delete( Mutex );

			Mutex = mtx::Create();

			TIds.Init();
			Meanings.Init();
		}
	};

	// A surcharger.
	extern error___ *SCLERRORError;

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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
