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

//	$Id: xulfsf.h,v 1.13 2012/08/05 18:31:02 csimon Exp $

#ifndef XULFSF__INC
#define XULFSF__INC

#define XULFSF_NAME		"XULFSF"

#define	XULFSF_VERSION	"$Revision: 1.13 $"

#define XULFSF_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULFSF_NODBG )
#define XULFSF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/08/05 18:31:02 $

/* End of automatic documentation generation part. */

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
		widgets__ Widgets;
		void reset( bso::bool__ P = true )
		{
			_core__::reset( P );
			Widgets.reset( P );
			_BackendExtendedType = frdkrn::bxt_Undefined;
		}
		E_CDTOR( _session_form_core__ )
		void Init( void )
		{
			_BackendExtendedType = frdkrn::bxt_Undefined;
			_core__::Init();
			Widgets.Init();
		}
		void Attach( nsIDOMWindow *Window );
		void SetBackendType( frdkrn::backend_extended_type__ Type )
		{
			_BackendExtendedType = Type;
		}
	};

	template <typename trunk> E_TTCLONE__( xulfbs::_page__<trunk>, page__ );

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
		E_CDTOR( session_form__ )
		void Init( trunk &Trunk )
		{
			_session_form_core__::Init();
			page__<trunk>::Init( Trunk );
		}
		void Attach( nsIDOMWindow *Window )
		{
			_session_form_core__::Attach( Window );
			page__<trunk>::Attach( Window );
			Widgets.Attach( Window );
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
#endif
