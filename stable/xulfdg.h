/*
	Header for the 'xulfdg' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xulfdg.h,v 1.11 2013/06/11 10:32:26 csimon Exp $

#ifndef XULFDG__INC
#define XULFDG__INC

#define XULFDG_NAME		"XULFDG"

#define	XULFDG_VERSION	"$Revision: 1.11 $"

#define XULFDG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFDGTutor;

#if defined( E_DEBUG ) && !defined( XULFDG_NODBG )
#define XULFDG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/11 10:32:26 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
	typedef xulfbs::window__<xulftk::trunk___> _window__;

	XULFBS__EH( jsconsole_eh__ );
	XULFBS__EH( dominspector_eh__ );
	XULFBS__EH( frontend_error_eh__ );
	XULFBS__EH( backend_error_eh__ );

	struct debug_dialog__
	: public _window__
	{
	protected:
		virtual const char *XULFBSGetDigest( xml::writer_ &Digest );
	public:
		struct event_handlers__ {
			jsconsole_eh__ ehJSConsole;
			dominspector_eh__ ehDOMInspector;
			frontend_error_eh__ ehFrontendError;
			backend_error_eh__ ehBackendError;
		} EventHandlers;
		void reset( bso::bool__ P = true )
		{
			_window__::reset( P );
		}
		void Init( xulftk::trunk___ &Trunk )
		{
			_window__::Init( Trunk );
		}
		void Register( nsIDOMWindow *Window );
//		void ExtractSelectedDatabase( void );
		void Close( void )
		{
			_window__::Close();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
