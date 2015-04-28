/*
	Header for the 'bkdrrm' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

//	$Id: bkdrrm.h,v 1.2 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDRRM__INC
#define BKDRRM__INC

#define BKDRRM_NAME		"BKDRRM"

#define	BKDRRM_VERSION	"$Revision: 1.2 $"

#define BKDRRM_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BKDRRM_NODBG )
#define BKDRRM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD Remote Request Manager. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdrqm.h"

namespace bkdrrm {
	using namespace bkdrqm;

	class remote_request_manager
	: public request_manager__
	{
	protected:
		virtual const void *BKDRQMGet(
			epeios::row__ Row,
			cast__ Cast )
		{
			return Repository_( Row );
		}
		virtual void *BKDRQMPut(
			epeios::row__ Row,
			cast__ Cast )
		{
			return Repository_( Row );
		}
		virtual void BKDRQMPop(
			flw::iflow__ &Flow,
			const casts_ &Casts )
		{
			Repository_.Init();
			_CreateAll( Flow, Casts );

			if ( Flow.Get() != bkdcst::cEnd )
				ERRb();
		}
		virtual void BKDRQMPush(
			flw::oflow__ &Flow,
			const casts_ &Casts );
	private:
		bch::E_BUNCH( void *)Repository_;
		void *_Create(
			flw::iflow__ &Flow,
			cast Cast );
		void *_CreateAndGet(
			flw::iflow__ &Flow,
			cast Cast );
		void _Delete(
			void *P,
			cast Cast );
		void _CreateAll(
			flw::iflow__ &Flow,
			const casts_ &Casts );
		void _DeleteAll( const casts_ &Casts );
		void _Push(
			flw::oflow__ &Flow,
			void *P,
			cast Cast );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_DeleteAll( GetCasts() );

			request_manager__::reset( P );
			Repository_.reset( P );
		}
		remote_request_manager( void ) 
		{
			reset( false );
		}
		virtual ~remote_request_manager( void ) 
		{
			reset();
		}
		void Init( flw::ioflow__ &Flow )
		{
			reset();

			Repository_.Init();
			request_manager__::Init( Flow );
		}
	};

	typedef remote_request_manager	remote_request_manager_;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
