/*
	Header for the 'fblfup' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef FBLFUP__INC
#define FBLFUP__INC

#define FBLFUP_NAME		"FBLFUP"

#define	FBLFUP_VERSION	"$Revision: 1.9 $"

#define FBLFUP_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLFUPTutor;

#if defined( E_DEBUG ) && !defined( FBLFUP_NODBG )
#define FBLFUP_DBG
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
//D Frontend/Backend Layout Frontend Universal Parameters 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "fblfep.h"
# include "fblfrp.h"
# include "fblfph.h"
# include "fblovl.h"

# include "err.h"
# include "flw.h"

namespace fblfup {
	using namespace fblovl;

	typedef fblfph::parameters_handling_functions__ _parameters_handling_functions__;

	class universal_parameters__
	: public _parameters_handling_functions__
	{
	private:
		fblfep::embedded_parameters_base__ _Embedded;
		fblfrp::remote_parameters_base__ _Remote;
		mode__ _Mode;
	protected:
		virtual void FBLFPHPreProcess( void )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PreProcess();
				break;
			case mRemote:
				_Remote.PreProcess();
				break;
			default:
				ERRFwk();
				break;
			}
		}
		virtual void FBLFPHIn(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.In( Cast, Pointer, Flow );
				break;
			case mRemote:
				_Remote.In( Cast, Pointer, Flow );
				break;
			default:
				ERRFwk();
				break;
			}
		}
		virtual void FBLFPHOut(
			flw::ioflow__ &Flow,
			fblcst::cast__ Cast,
			void *Pointer )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.Out( Flow, Cast, Pointer );
				break;
			case mRemote:
				_Remote.Out( Flow, Cast, Pointer );
				break;
			default:
				ERRFwk();
				break;
			}
		}
		virtual void FBLFPHPostProcess( flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PostProcess( Flow );
				break;
			case mRemote:
				_Remote.PostProcess( Flow );
				break;
			default:
				ERRFwk();
				break;
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Embedded.reset( P );
			_Remote.reset( P );
			_parameters_handling_functions__::reset( P );
			_Mode = m_Undefined;
		}
		E_CVDTOR( universal_parameters__ );
		void Init( mode__ Mode )
		{
			reset();

			switch ( Mode ) {
			case mEmbedded:
				_Embedded.Init();
				break;
			case mRemote:
				_Remote.Init();
				break;
			default:
				ERRFwk();
				break;
			}

			_Mode = Mode;

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
