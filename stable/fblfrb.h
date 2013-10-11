/*
	Header for the 'fblfrb' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: fblfrb.h,v 1.2 2011/09/12 15:56:42 csimon Exp $

#ifndef FBLFRB__INC
#define FBLFRB__INC

#define FBLFRB_NAME		"FBLFRB"

#define	FBLFRB_VERSION	"$Revision: 1.2 $"

#define FBLFRB_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLFRBTutor;

#if defined( E_DEBUG ) && !defined( FBLFRB_NODBG )
#define FBLFRB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/09/12 15:56:42 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Frontend for Remote Backend 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBRFDP' instead !"

# include "err.h"
# include "flw.h"
 #include "bch.h"

# include "fblcst.h"
# include "fblfrd.h"

# define FBLFRB__OUT_PARAMETERS_AMOUNT_MAX		20	// Le nombre maximum de paramètres en sortie.

namespace fblfrb {

	struct datum__ {
		fblcst::cast Cast;
		void *Pointer;
		datum__( 
			fblcst::cast Cast = fblcst::c_Unknow,
			void *Pointer = NULL )
		{
			this->Cast = Cast;
			this->Pointer= Pointer;
		}
	};

	class backend_remote_access_base___
	{
	public:
		bch::E_BUNCH__( datum__, FBLFRB__OUT_PARAMETERS_AMOUNT_MAX ) Data;
		void reset( bso::bool__ = true )
		{
			// Standardization.
		}
		void Init( void )
		{
			Data.Init();
		}
		void PreProcess( void )
		{
			Data.Init();
		}
		void In(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow );
		void Out(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer );
		void PostProcess( flw::ioflow__ &Flow );
	};

	typedef fblfrd::parameters_handling_functions__ _parameters_handling_functions__;

	class remote_parameters_handling_functions__
	: public _parameters_handling_functions__
	{
	private:
		backend_remote_access_base___ _Base;
	protected:
		void FBLFRDPreProcess( void )
		{
			_Base.PreProcess();
		}
		virtual void FBLFRDIn(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			_Base.In( Cast, Pointer, Flow );
		}
		virtual void FBLFRDOut(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer )
		{
			_Base.Out( Flow, Cast, Pointer );
		}
		void FBLFRDPostProcess( flw::ioflow__ &Flow )
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
