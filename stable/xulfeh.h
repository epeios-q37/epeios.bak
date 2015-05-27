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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef XULFEH__INC
#define XULFEH__INC

#define XULFEH_NAME		"XULFEH"

#define	XULFEH_VERSION	"$Revision: 1.9 $"

#define XULFEH_OWNER		"Claude SIMON"

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
		XULFBS__EH( exit__ );
		XULFBS__EH( about__ );
		XULFBS__EH( web_site__ );
		XULFBS__EH( debug__ );
		XULFBS__EH( new_project__ );
		XULFBS__EH( user_project__ );
		XULFBS__EH( project_type_selection__ );
		XULFBS__EH( user_project_selection__ );
		XULFBS__EH( apply__ );
		XULFBS__EH( close_project__ );
	}

	namespace sf { // Session form.
		XULFBS__EH( backend_type_selection__ );
		XULFBS__EH( embedded_backend_selection__ );
		XULFBS__EH( apply__ );
		XULFBS__EH( cancel__ );
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
			void Attach( nsIDOMWindow *Window );
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
			void Attach( nsIDOMWindow *Window );
		} SF;
		struct sv__ {
			void reset( bso::bool__ = true )
			{
				// Standardisation.
			}
			E_CDTOR( sv__ );
			void Init( xulftk::trunk___ & )
			{
				// Standardisation.
			}
			void Attach( nsIDOMWindow *Window )
			{
				// Standardisation.
			}
		} SV;
		void reset( bso::bool__ P = true )
		{
			M.reset(  P );
			SF.reset( P );
			SV.reset( P );
		}
		E_CDTOR( event_handlers__ )
		void Init( xulftk::trunk___ &Trunk )
		{
			M.Init( Trunk );
			SF.Init( Trunk );
			SV.Init( Trunk );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
