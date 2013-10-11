/*
	Header for the 'tsf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: tsf.h,v 1.4 2012/11/14 16:06:38 csimon Exp $

#ifndef TSF__INC
#define TSF__INC

#define TSF_NAME		"TSF"

#define	TSF_VERSION	"$Revision: 1.4 $"

#define TSF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &TSFTutor;

#if defined( E_DEBUG ) && !defined( TSF_NODBG )
#define TSF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:38 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Thread-Safe Flow. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mtx.h"

namespace tsf {

	class thread_safe_oflow__
	: public flw::oflow__
	{
	private:
		flw::oflow__ *_Flow;
		mtx::mutex_handler__ _Mutex;
		bso::bool__ _Owner;
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
			bool Synchronization )
		{
			if ( !_Owner ) {
				mtx::Lock( _Mutex );
				_Owner = true;
			}

			return _Flow->WriteRelay( Buffer, Wanted, Minimum, Synchronization );
		}
		virtual void FLWSynchronizing( void )
		{
			if ( _Owner ) {
				_Owner = false;
				mtx::Unlock( _Mutex );
			}
		}

	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Flow != NULL )
					Synchronize();
			}

			_Flow = NULL;
			_Mutex = MTX_INVALID_HANDLER;
			_Owner = false;
		}
		void Init(
			flw::oflow__ &Flow,
			mtx::mutex_handler__ Mutex )
		{
			reset();

			_Flow = &Flow;
			_Mutex = Mutex;
		}
	};



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
