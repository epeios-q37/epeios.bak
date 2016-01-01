/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

//	$Id: tsf.h,v 1.4 2012/11/14 16:06:38 csimon Exp $

#ifndef TSF__INC
#define TSF__INC

#define TSF_NAME		"TSF"

#define	TSF_VERSION	"$Revision: 1.4 $"

#define TSF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( TSF_NODBG )
#define TSF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:38 $

/* End of automatic documentation generation part. */

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
#endif
