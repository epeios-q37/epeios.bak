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

//	$Id: fblber.h,v 1.9 2013/04/15 10:50:49 csimon Exp $

#ifndef FBLBER__INC
#define FBLBER__INC

#define FBLBER_NAME		"FBLBER"

#define	FBLBER_VERSION	"$Revision: 1.9 $"

#define FBLBER_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( FBLBER_NODBG )
#define FBLBER_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:49 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Backend Embed Request 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fblbrq.h"

namespace fblber {
	using namespace fblbrq;

	class embedded_callbacks___
	: public callbacks__
	{
	protected:
		virtual const void *FBLBRQGet(
			sdr::row__ Row,
			cast__ Cast )
		{
			return Repository_( Row );
		}
		virtual void *FBLBRQPut(
			sdr::row__ Row,
			cast__ Cast )
		{
			return Repository_( Row );
		}
		flw::iflow__ &FBLBRQGetFlow( sdr::row__ Row )
		{
			return *(flw::iflow__ *)Repository_( Row );
		}
		virtual void FBLBRQPutFlow(
			sdr::row__ Row,
			flw::iflow__ &Flow )
		{
			*(flw::iflow__ **)Repository_( Repository_.Last() ) = &Flow;
		}
		virtual void FBLBRQPopIn(
			sdr::row__ CRow,
			flw::iflow__ &Flow,
			cast__ Cast );
		virtual void FBLBRQPopInEnd(
			sdr::row__ CRow,
			flw::iflow__ &Flow );
		virtual void FBLBRQPopOut(
			sdr::row__ CRow,
			flw::iflow__ &Flow,
			cast__ Cast );
		virtual void FBLBRQPush(
			bso::bool__,
			const casts_ &Casts,
			flw::oflow__ &Flow )
		{
			// Rien  faise.
		}
	private:
		bch::E_BUNCH( void *)Repository_;
	public:
		void reset( bso::bool__ P = true )
		{
			callbacks__::reset( P );
			Repository_.reset( P );
		}
		void Init( void )
		{
			Repository_.Init();
			callbacks__::Init();
		}
	};
}

/*$END$*/
#endif
