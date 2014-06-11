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
	class embedded_parameters_base__
	{
	public:
		void reset( bso::bool__ = true )
		{
			// Standardization.
		}
		E_CVDTOR( embedded_parameters_base__ );
		void Init( void )
		{
			// Standardization.
		}
		void PreProcess( void )
		{
			// Nothing to do.
		}
		void In(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			Flow.Put( Cast );

			flw::Put( Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			fblcst::cast__ Cast,
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

	typedef fblfph::parameters_handling_functions__ _parameters_handling_functions__;

	struct embedded_parameters__
	: public _parameters_handling_functions__
	{
	private:
		embedded_parameters_base__ _Base;
	protected:
		virtual void FBLFRDPoreProcess( void )
		{
			_Base.PreProcess();
		}
		virtual void FBLFRDIn(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			_Base.In( Cast, Pointer, Flow );
		}
		virtual void FBLFRDOut(
			flw::ioflow__ &Flow,
			fblcst::cast__ Cast,
			void *Pointer )
		{
			_Base.Out( Flow, Cast, Pointer );
		}
		virtual void FBLFRDPostProcess( flw::ioflow__ &Flow )
		{
			_Base.PostProcess( Flow );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Base.reset( P );
			_parameters_handling_functions__::reset( P );
		}
		E_CVDTOR( embedded_parameters__ );
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
