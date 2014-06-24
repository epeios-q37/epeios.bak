/*
	Header for the 'fblbur' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: fblbur.h,v 1.6 2013/04/09 17:57:29 csimon Exp $

#ifndef FBLBUR__INC
#define FBLBUR__INC

#define FBLBUR_NAME		"FBLBUR"

#define	FBLBUR_VERSION	"$Revision: 1.6 $"

#define FBLBUR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBURTutor;

#if defined( E_DEBUG ) && !defined( FBLBUR_NODBG )
#define FBLBUR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:29 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Backend Universal Request 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fblbrr.h"
# include "fblber.h"
# include "fblbrq.h"

namespace fblbur {

	using namespace fblovl;

	typedef fblbrq::request__ _request__;

	class universal_request___
	: public _request__
	{
	private:
		fblber::embedded_callbacks___ _Embedded;
		fblbrr::remote_callbacks___ _Remote;
	public:
		void reset( bso::bool__ P = true )
		{
			_request__::reset( P );

			_Embedded.reset( P );
			_Remote.reset( P );
		}
		universal_request___( void )
		{
			reset( false );
		}
		~universal_request___( void )
		{
			reset();
		}
		void Init(
			mode__ Mode,
			flw::ioflow__ &Flow )
		{
			reset();

			switch ( Mode ) {
			case mEmbedded:
				_Embedded.Init();
				_request__::Init( _Embedded, Flow );
				break;
			case mRemote:
				_Remote.Init();
				_request__::Init( _Remote, Flow );
				break;
			default:
				ERRPrm();
				break;
			}
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
