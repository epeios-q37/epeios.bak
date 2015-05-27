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

//	$Id: xui.h,v 1.2 2010/07/24 20:00:56 csimon Exp $

#ifndef XUI__INC
#define XUI__INC

#define XUI_NAME		"XUI"

#define	XUI_VERSION	"$Revision: 1.2 $"

#define XUI_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XUI_NODBG )
#define XUI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/24 20:00:56 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL User Interface. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete ! Use 'XULUI' instead !"

#include "err.h"
#include "flw.h"
#include "nsxpcm.h"

namespace xui {

	template <typename ui> class bare_bridge__
	{
	private:
		ui *_UI;
	public:
		bare_bridge__( void )
		{
			_UI = NULL;
		}
		void Init( ui &UI )
		{
			_UI = &UI;
		}
		const ui &UI( void ) const
		{
			return *_UI;
		}
		ui &UI( void )
		{
			return *_UI;
		}
	};

	template <typename ui, typename widget> class _generic__
	: public bare_bridge__<ui>,
	  public widget
	{
	protected:
		void NSXPCMOnEvent( nsxpcm::event__ )
		{
			ERRu();
		}
	public:
		void Init( ui &UI )
		{
			bridge__::Init( UI );
			widget::Init();
		}
	};

#	define XUI__WN( widget, name )\
	template <typename ui> E_TTYPEDEF__( E_COVER2( _generic__<ui, nsxpcm::widget##__> ), name##__ )

#	define XUI__W( widget )	XUI__WN( widget, widget )

	XUI__W( textbox );
	XUI__W( radio );
	XUI__W( button );
	XUI__W( listbox );
	XUI__W( tree );
	XUI__W( deck );
	XUI__W( broadcaster );
	XUI__W( command );
	XUI__W( menu );
	XUI__W( menu_item );
	XUI__W( panel );
	XUI__WN( element, box );
	XUI__W( window );
	XUI__W( description );

	template <typename ui, typename widget> void Register(
		ui &UI,
		widget &Widget,
		nsIDOMDocument *Document,
		const char *Id,
		int Events )
	{
		Widget.Init( UI );
		nsxpcm::Register( Widget, Document, Id, Events );
	}

	template <typename ui, typename widget> void Register(
		ui &UI,
		widget &Widget,
		nsISupports *Supports,
		int Events )
	{
		Widget.Init( UI );
		nsxpcm::Register( Widget, Supports, Events );
	}

	template <typename ui> inline void Register(
		ui &UI,
		window__<ui> &Window,
		nsIDOMWindow *Element )
	{
		Register( UI, Window, Element, nsxpcm::efClose );
	}

	template <typename ui> inline void Register(
		ui &UI,
		broadcaster__<ui> &Broadcaster,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Broadcaster, Document, Id, nsxpcm::efNone );
	}

	template <typename ui> inline void Register(
		ui &UI,
		command__<ui> &Command,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Command, Document, Id, nsxpcm::efCommand );
	}

	template <typename ui> inline void Register(
		ui &UI,
		tree__<ui> &Tree,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Tree, Document, Id, nsxpcm::efSelect | nsxpcm::efDblClick );
	}

	template <typename ui> inline void Register(
		ui &UI,
		deck__<ui> &Deck,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Deck, Document, Id, nsxpcm::efNone );
	}

	template <typename ui> inline void Register(
		ui &UI,
		textbox__<ui> &Textbox,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Textbox, Document, Id, nsxpcm::efNone );
	}

	template <typename ui> inline void Register(
		ui &UI,
		button__<ui> &Button,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Button, Document, Id, nsxpcm::efCommand );
	}

	template <typename ui> inline void Register(
		ui &UI,
		description__<ui> &Description,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( UI, Description, Document, Id, nsxpcm::efNone );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
