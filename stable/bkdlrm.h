/*
	Header for the 'bkdlrm' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

//	$Id: bkdlrm.h,v 1.2 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDLRM__INC
#define BKDLRM__INC

#define BKDLRM_NAME		"BKDLRM"

#define	BKDLRM_VERSION	"$Revision: 1.2 $"

#define BKDLRM_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BKDLRMTutor;

#if defined( E_DEBUG ) && !defined( BKDLRM_NODBG )
#define BKDLRM_DBG
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
//D BacKenD Local Request Manager. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdrqm.h"

namespace bkdlrm {

	using namespace bkdrqm;

	class local_request_manager
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
			_GetAll( Flow, Casts );
		}
		virtual void BKDRQMPush(
			flw::oflow__ &Flow,
			const casts_ &Casts )
		{
			// Rien  faire ...
		}
	private:
		bch::E_BUNCH( void *)Repository_;
		void _GetAll(
			flw::iflow__ &Flow,
			const casts_ &Casts );
	public:
		void reset( bso::bool__ P = true )
		{
			request_manager__::reset( P );
			Repository_.reset( P );
		}
		void Init( flw::ioflow__ &Flow )
		{
			Repository_.Init();
			request_manager__::Init( Flow );
		}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
