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

//	$Id: xului.h,v 1.9 2010/10/26 14:59:53 csimon Exp $

#ifndef XULUI__INC
#define XULUI__INC

#define XULUI_NAME		"XULUI"

#define	XULUI_VERSION	"$Revision: 1.9 $"

#define XULUI_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULUI_NODBG )
#define XULUI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/10/26 14:59:53 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D XUL User Interface. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obolete : Use 'XULWDG'."

#include "err.h"
#include "flw.h"
#include "nsxpcm.h"

namespace xului {

	template <typename target> class bare_bridge__
	{
	private:
		target *_Target;
	public:
		bare_bridge__( void )
		{
			_Target = NULL;
		}
		void Init( target &Target )
		{
			_Target = &Target;
		}
		const target &Target( void ) const
		{
			return *_Target;
		}
		target &Target( void )
		{
			return *_Target;
		}
	};

	template <typename target, typename widget> class _generic__
	: public bare_bridge__<target>,
	  public widget
	{
	protected:
		void NSXPCMOnEvent( nsxpcm::event__ )
		{
			ERRu();
		}
	public:
		void Init( target &Target )
		{
			bridge__::Init( Target );
			widget::Init();
		}
	};

#	define XULUI__WN( widget, name, events )\
	template <typename target> E_TTYPEDEF__( E_COVER2( _generic__<target, nsxpcm::widget##__> ), name##__ );\
	template <typename target> inline void Register(\
		target &Target,\
		name##__<target> &Widget,\
		nsISupports *Element,\
		nsIDOMWindow *Window )\
	{\
		Register( Target, Widget, Element, Window, events );\
	}\
	template <typename target> inline void Register(\
		target &Target,\
		name##__<target> &Widget,\
		nsIDOMWindow *Window,\
		const char *Id )\
	{\
		Register( Target, Widget, Window, Id, events );\
	}

#	define XULUI__W( widget, events )	XULUI__WN( widget, widget, events )

	XULUI__W( textbox, nsxpcm::ef_None );
	XULUI__W( radio, nsxpcm::efCommand );
	XULUI__W( button, nsxpcm::efCommand );
	XULUI__W( listbox, nsxpcm::efCommand );
	XULUI__W( tree, nsxpcm::efSelect | nsxpcm::efDblClick );
	XULUI__W( deck, nsxpcm::ef_None );
	XULUI__W( broadcast, nsxpcm::ef_None );
	XULUI__W( command, nsxpcm::efCommand );
	XULUI__W( menu, nsxpcm::ef_None );
	XULUI__W( menu_item, nsxpcm::efCommand );
	XULUI__W( panel, nsxpcm::ef_None );
	XULUI__WN( widget, box, nsxpcm::ef_None );
//	XULUI__W( document, nsxpcm::efClose );
	XULUI__W( window, nsxpcm::efClose );
	XULUI__W( description, nsxpcm::ef_None );

	template <typename target, typename widget> void Register(
		target &Target,
		widget &Widget,
		nsIDOMWindow *Window,
		const char *Id,
		int Events )
	{
		Widget.Init( Target );
		nsxpcm::Register( Widget, Window, Id, Events );
	}

	template <typename target, typename widget> void Register(
		target &Target,
		widget &Widget,
		nsISupports *Supports,
		nsIDOMWindow *Window,
		int Events )
	{
		Widget.Init( Target );
		nsxpcm::Register( Widget, Supports, Window, Events );
	}
}

/*$END$*/
#endif
