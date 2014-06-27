/*
	Header for the 'fblfep' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FBLFEP__INC
#define FBLFEP__INC

#define FBLFEP_NAME		"FBLFEP"

#define	FBLFEP_VERSION	"$Revision: 1.9 $"

#define FBLFEP_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLFEPTutor;

#if defined( E_DEBUG ) && !defined( FBLFEP_NODBG )
#define FBLFEP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Frontend Embedded Parameters 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "fblfph.h"

# include "err.h"
# include "flw.h"

namespace fblfep {
	typedef fblfph::callbacks__ _callbacks__;

	class embedded_callbacks__
	: public _callbacks__
	{
	protected:
		virtual void FBLFPHPreProcess( void )
		{
			// Nothing to do.
		}
		virtual void FBLFPHIn(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Channel )
		{
			flw::Put( Pointer, Channel );
		}
		virtual void FBLFPHOut(
			flw::ioflow__ &Channel,
			fblcst::cast__ Cast,
			void *Pointer )
		{
			flw::Put( Pointer, Channel );
		}
		virtual void FBLFPHFlowIn(
			bso::bool__ FirstCall,
			flw::iflow__ &Flow,
			flw::ioflow__ &Channel )
		{
			if ( FirstCall )
				flw::Put( &Flow, Channel );
		}
		virtual void FBLFPHFlowOut(
			flw::ioflow__ &Channel,
			flw::iflow__ *&Flow )
		{
			flw::Put( &Flow, Channel );
		}
		virtual void FBLFPHPostProcess( flw::ioflow__ & )
		{
			// Nothing to do.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_callbacks__::reset( P );
		}
		E_CVDTOR( embedded_callbacks__ );
		void Init( void )
		{
			_callbacks__::Init();
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
