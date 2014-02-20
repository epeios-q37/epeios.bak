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
	namespace {
		using namespace xulfbs;
	}

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
		void Attach( nsIDOMWindow *Window );
	};

	struct _session_form_core__
	: public _core__
	{
	private:
		frdkrn::backend_extended_type__ _BackendExtendedType;
	protected:
		void _Refresh( xml::writer_ &Digest );
	public:
		widgets__ &Widgets;
		void reset( bso::bool__ P = true )
		{
			_core__::reset( P );
			_BackendExtendedType = frdkrn::bxt_Undefined;
		}
		E_VDTOR( _session_form_core__ )
		_session_form_core__( widgets__ &Widgets )
		: Widgets( Widgets )
		{
			reset( false );
		}
		void Init( void )
		{
			_BackendExtendedType = frdkrn::bxt_Undefined;
			_core__::Init();
		}
		void Attach( nsIDOMWindow *Window );
		void SetBackendType( frdkrn::backend_extended_type__ Type )
		{
			_BackendExtendedType = Type;
		}
	};

	namespace {
		template <typename trunk> E_TTCLONE__( xulfbs::_page__<trunk>, page__ );
	}

	template <typename trunk> struct session_form__
	: public _session_form_core__,
	  public page__<trunk>
	{
	private:
		xulftk::trunk___ &_Trunk( void )
		{
			return page__<trunk>::Trunk();
		}
		nsIDOMWindow *_Window( void )
		{
			return page__<trunk>::Window();
		}
		nsIDOMDocument *_Document( void )
		{
			return page__<trunk>::Document();
		}
	protected:
		void Refresh( xml::writer_ &Digest )
		{
			_session_form_core__::_Refresh( Digest );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_session_form_core__::reset( P );
			page__<trunk>::reset( P );
		}
		E_VDTOR( session_form__ )
		session_form__( widgets__ &Widgets )
		: _session_form_core__( Widgets )
		{
			reset( false );
		}
		void Init( trunk &Trunk )
		{
			_session_form_core__::Init();
			page__<trunk>::Init( Trunk );
		}
		void Attach( nsIDOMWindow *Window )
		{
			_session_form_core__::Attach( Window );
			page__<trunk>::Attach( Window );
		}
		trunk &Trunk( void )
		{
			return page__<trunk>::Trunk();
		}
		nsIDOMWindow *Window( void )
		{
			return page__<trunk>::Window();
		}
		nsIDOMDocument *Document( void )
		{
			return page__<trunk>::Document();
		}
		void Refresh( void )
		{
			page__<trunk>::Refresh();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
