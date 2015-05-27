/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: bkdlac.h,v 1.5 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDLAC__INC
#define BKDLAC__INC

#define BKDLAC_NAME		"BKDLAC"

#define	BKDLAC_VERSION	"$Revision: 1.5 $"

#define BKDLAC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BKDLAC_NODBG )
#define BKDLAC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD Local ACcess 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdacc.h"

namespace bkdlac {

	class backend_local_access_base__
	{
	public:
		void reset( bso::bool__ = true )
		{
			// Standardization.
		}
		void Init( void )
		{
			// Standardization.
		}
		void In(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			Flow.Put( Cast );

			flw::Put( Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			Flow.Put( Cast );

			flw::Put( Pointer, Flow );
		}
		void PostProcess( flw::ioflow__ & )
		{
			// Nothing to do.
		}
	};

	typedef bkdacc::parameters_handling_functions__ _parameters_handling_functions__;

	struct local_parameters_handling_functions__
	: public _parameters_handling_functions__
	{
	private:
		backend_local_access_base__ _Base;
	protected:
		virtual void BKDACCIn(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			_Base.In( Cast, Pointer, Flow );
		}
		virtual void BKDACCOut(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			_Base.Out( Flow, Cast, Pointer );
		}
		virtual void BKDACCPostProcess( flw::ioflow__ &Flow )
		{
			_Base.PostProcess( Flow );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Base.reset( P );
			_parameters_handling_functions__::reset( P );
		}
		local_parameters_handling_functions__( void )
		{
			reset( false );
		}
		virtual ~local_parameters_handling_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			_Base.Init();
			_parameters_handling_functions__::Init();
		}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
