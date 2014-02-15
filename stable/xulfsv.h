/*
	Header for the 'xulfsv' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xulfsv.h,v 1.1 2011/12/16 16:19:54 csimon Exp $

#ifndef XULFSV__INC
#define XULFSV__INC

#define XULFSV_NAME		"XULFSV"

#define	XULFSV_VERSION	"$Revision: 1.1 $"

#define XULFSV_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFSVTutor;

#if defined( E_DEBUG ) && !defined( XULFSV_NODBG )
#define XULFSV_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/12/16 16:19:54 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend Session View 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "xulfbs.h"
# include "frdkrn.h"

# define XULFSV_AFFIX	"SessionView"

namespace xulfsv {
	typedef xulfbs::page__<xulftk::trunk___> _page__;

	// Standardisation.
	class widgets__
	{
	public:
		void reset( bso::bool__ = true )
		{}
		E_CVDTOR( widgets__ );
		void Init( void )
		{}
	};

	struct session_view__
	: public _page__
	{
	public:
		widgets__ &Widgets;
		void reset( bso::bool__ P = true )
		{
			_page__::reset( P );
		}
		E_VDTOR( session_view__ );
		session_view__( widgets__ &Widgets )
		: Widgets( Widgets )
		{}
		void Init( xulftk::trunk___ &Trunk )
		{
			_page__::Init( Trunk );
		}
		void Register( nsIDOMWindow *Window );
		void GetDigest( xml::writer_ &Digest );
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
