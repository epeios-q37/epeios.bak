/*
	Header for the 'bkdrac' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: bkdrac.h,v 1.5 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDRAC__INC
#define BKDRAC__INC

#define BKDRAC_NAME		"BKDRAC"

#define	BKDRAC_VERSION	"$Revision: 1.5 $"

#define BKDRAC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BKDRAC_NODBG )
#define BKDRAC_DBG
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
//D BacKenD Remote ACcess 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdacc.h"
#include "bch.h"

#define BKDRAC_OUT_PARAMETERS_AMOUNT_MAX		20	// Le nombre maximum de paramtres en sortie.

namespace bkdrac {

	struct datum__ {
		bkdcst::cast Cast;
		void *Pointer;
		datum__( 
			bkdcst::cast Cast = bkdcst::c_Unknow,
			void *Pointer = NULL )
		{
			this->Cast = Cast;
			this->Pointer= Pointer;
		}
	};

	class backend_remote_access_base___
	{
	public:
		bch::E_BUNCH__( datum__, BKDRAC_OUT_PARAMETERS_AMOUNT_MAX ) Data;
		void reset( bso::bool__ = true )
		{
			// Standardization.
		}
		void Init( void )
		{
			Data.Init();
		}
		void In(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow );
		void Out(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer );
		void PostProcess( flw::ioflow__ &Flow );
	};

	typedef bkdacc::parameters_handling_functions__ _parameters_handling_functions__;

	class remote_parameters_handling_functions__
	: public _parameters_handling_functions__
	{
	private:
		backend_remote_access_base___ _Base;
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
		void BKDACCPostProcess( flw::ioflow__ &Flow )
		{
			_Base.PostProcess( Flow );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Base.reset( P );
			_parameters_handling_functions__::reset( P );
		}
		remote_parameters_handling_functions__( void )
		{
			reset( false );
		}
		virtual ~remote_parameters_handling_functions__( void )
		{
			reset();
		}
		void Init( flw::ioflow__ &Flow )
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
