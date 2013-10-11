/*
	Header for the 'xulfeh' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFEH__INC
#define XULFEH__INC

#define XULFEH_NAME		"XULFEH"

#define	XULFEH_VERSION	"$Revision: 1.9 $"

#define XULFEH_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFEHTutor;

#if defined( E_DEBUG ) && !defined( XULFEH_NODBG )
#define XULFEH_DBG
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
//D XUL Frontend Event Handlers 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"

namespace xulfeh {
	using namespace xulfbs;

	namespace m { // Main
		XULFBS_EH( exit__ );
		XULFBS_EH( about__ );
		XULFBS_EH( web_site__ );
		XULFBS_EH( debug__ );
		XULFBS_EH( new_project__ );
		XULFBS_EH( user_project__ );
		XULFBS_EH( project_type_selection__ );
		XULFBS_EH( user_project_selection__ );
		XULFBS_EH( apply__ );
		XULFBS_EH( close_project__ );
	}

	namespace sf { // Session form.
		XULFBS_EH( backend_type_selection__ );
		XULFBS_EH( embedded_backend_selection__ );
		XULFBS_EH( apply__ );
		XULFBS_EH( cancel__ );
	}

	class event_handlers__
	{
	public:
		struct m__ {
			m::new_project__ NewProject;
			m::user_project__ UserProject;
			m::close_project__ CloseProject;
			m::project_type_selection__ ProjectTypeSelection;
			m::user_project_selection__ UserProjectSelection;
			m::apply__ Apply;
			m::about__ About;
			m::web_site__ WebSite;
			m::debug__ Debug;
			m::exit__ Exit;
			void reset( bso::bool__ P = true )
			{
				NewProject.reset( P );
				UserProject.reset( P );
				CloseProject.reset( P );
				ProjectTypeSelection.reset( P );
				UserProjectSelection.reset( P );
				Apply.reset( P );
				About.reset( P );
				WebSite.reset( P );
				Debug.reset( P );
				Exit.reset( P );
			}
			E_CDTOR( m__ );
			void Init( xulftk::trunk___ &Trunk )
			{
				NewProject.Init( Trunk );
				UserProject.Init( Trunk );
				CloseProject.Init( Trunk );
				ProjectTypeSelection.Init( Trunk );
				UserProjectSelection.Init( Trunk );
				Apply.Init( Trunk );
				About.Init( Trunk );
				WebSite.Init( Trunk );
				Debug.Init( Trunk );
				Exit.Init( Trunk );
			}
		} M;
		struct sf__ {
			sf::backend_type_selection__ BackendTypeSelection;
			sf::embedded_backend_selection__ EmbeddedBackendSelection;
			sf::apply__ Apply;
			sf::cancel__ Cancel;
			void reset( bso::bool__ P = true )
			{
				BackendTypeSelection.reset( P );
				EmbeddedBackendSelection.reset( P );
				Apply.reset( P );
				Cancel.reset( P );
			}
			E_CDTOR( sf__ );
			void Init( xulftk::trunk___ &Trunk )
			{
				BackendTypeSelection.Init( Trunk );
				EmbeddedBackendSelection.Init( Trunk );
				Apply.Init( Trunk );
				Cancel.Init( Trunk );
			}
		} SF;
		void reset( bso::bool__ P = true )
		{
			M.reset(  P );
			SF.reset( P );
		}
		E_CDTOR( event_handlers__ )
		void Init( xulftk::trunk___ &Trunk )
		{
			M.Init( Trunk );
			SF.Init( Trunk );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
