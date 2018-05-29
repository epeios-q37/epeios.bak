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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FBLFEP__INC
#define FBLFEP__INC

#define FBLFEP_NAME		"FBLFEP"

#define	FBLFEP_VERSION	"$Revision: 1.9 $"

#define FBLFEP_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( FBLFEP_NODBG )
#define FBLFEP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

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
#endif
