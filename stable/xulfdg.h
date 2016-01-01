/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

//	$Id: xulfdg.h,v 1.11 2013/06/11 10:32:26 csimon Exp $

#ifndef XULFDG__INC
#define XULFDG__INC

#define XULFDG_NAME		"XULFDG"

#define	XULFDG_VERSION	"$Revision: 1.11 $"

#define XULFDG_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULFDG_NODBG )
#define XULFDG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/11 10:32:26 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend DebuG 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"

# define XULFDG_AFFIX		"Debug"

# define XULFDG_WINDOW_ID	"dlg" XULFDG_AFFIX "_alpha1"

namespace xulfdg {
	XULFBS_TWIDGET( xulftk::trunk___, window );

	XULFBS__EH( jsconsole_eh__ );
	XULFBS__EH( dominspector_eh__ );
	XULFBS__EH( frontend_error_eh__ );
	XULFBS__EH( backend_error_eh__ );

	struct event_handlers__ {
	public:
		jsconsole_eh__ JSConsole;
		dominspector_eh__ DOMInspector;
		frontend_error_eh__ FrontendError;
		backend_error_eh__ BackendError;
		void reset( bso::bool__ P = true )
		{
			JSConsole.reset( P );
			DOMInspector.reset( P );
			FrontendError.reset( P );
			BackendError.reset( P );
		}
		E_CDTOR( event_handlers__ )
		void Init( xulftk::trunk___ &Trunk )
		{
			JSConsole.Init( Trunk );
			DOMInspector.Init( Trunk );
			FrontendError.Init( Trunk );
			BackendError.Init( Trunk );
		}
		void Attach( nsIDOMWindow *Window );
	};


	struct debug_dialog__
	: public window__
	{
	protected:
		virtual const char *XULFBSRefresh( xml::writer_ &Digest );
	public:
		event_handlers__ EventHandlers;
		void reset( bso::bool__ P = true )
		{
			window__::reset( P );
			EventHandlers.reset( P );
		}
		void Init( xulftk::trunk___ &Trunk )
		{
			window__::Init( Trunk );
			EventHandlers.Init( Trunk );
		}
		void Attach( nsIDOMWindow *Window );
		void Close( void )
		{
			window__::Close();
		}
	};

}

/*$END$*/
#endif
