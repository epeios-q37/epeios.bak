/*
	Header for the 'xulfsf' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xulfsf.h,v 1.13 2012/08/05 18:31:02 csimon Exp $

#ifndef XULFSF__INC
#define XULFSF__INC

#define XULFSF_NAME		"XULFSF"

#define	XULFSF_VERSION	"$Revision: 1.13 $"

#define XULFSF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFSFTutor;

#if defined( E_DEBUG ) && !defined( XULFSF_NODBG )
#define XULFSF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/08/05 18:31:02 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend Session Form 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"
# include "frdkrn.h"

# define XULFSF_AFFIX "SessionForm"

namespace xulfsf {
	using namespace xulfbs;

	typedef nsxpcm::ui_core__ _ui_core__;

	class widgets__ {
	public:
		menulist__ mnlBackendType;
		menuitem__
			mniNoBackend,
			mniPredefinedBackend,
			mniDaemonBackend,
			mniEmbeddedBackend;
		deck__ dckBackendType;
		menulist__ mnlPredefinedBackend;
		textbox__ txbDaemonBackend;
		textbox__ txbEmbeddedBackend;
		textbox__ txbLogin;
		textbox__ txbPassword;
		void reset( bso::bool__ P = true )
		{
			mnlBackendType.reset( P );
			mniNoBackend.reset( P );
			mniPredefinedBackend.reset( P );
			mniDaemonBackend.reset( P );
			mniEmbeddedBackend.reset( P );
			dckBackendType.reset( P );
			mnlPredefinedBackend.reset( P );
			txbDaemonBackend.reset( P );
			txbEmbeddedBackend.reset( P );
			txbLogin.reset( P );
			txbPassword.reset( P );
		}
		E_CVDTOR( widgets__ );
		void Init( void )
		{
			mnlBackendType.Init();
			mniNoBackend.Init();
			mniPredefinedBackend.Init();
			mniDaemonBackend.Init();
			mniEmbeddedBackend.Init();
			dckBackendType.Init();
			mnlPredefinedBackend.Init();
			txbDaemonBackend.Init();
			txbEmbeddedBackend.Init();
			txbLogin.Init();
			txbPassword.Init();
		}
	};

	struct session_form__
	: public _ui_core__
	{
	private:
		frdkrn::backend_extended_type__ _BackendExtendedType;
	public:
		widgets__ &Widgets;
		void reset( bso::bool__ P = true )
		{
			_BackendExtendedType = frdkrn::bxt_Undefined;
			_ui_core__::reset( P );
		}
		E_VDTOR( session_form__ )
		session_form__( widgets__ &Widgets )
		: Widgets( Widgets )
		{
			reset( false );
		}
		void Init( void )
		{
			_ui_core__::Init();
			_BackendExtendedType = frdkrn::bxt_Undefined;
		}
		void Register( nsIDOMWindow *Window );
		void SetBackendType( frdkrn::backend_extended_type__ Type )
		{
			_BackendExtendedType = Type;
		}
		void GetDigest( xml::writer_ &Digest );
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
