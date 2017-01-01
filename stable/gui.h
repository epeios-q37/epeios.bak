/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: gui.h,v 1.4 2012/11/14 16:06:29 csimon Exp $

#ifndef GUI__INC
#define GUI__INC

#define GUI_NAME		"GUI"

#define	GUI_VERSION	"$Revision: 1.4 $"

#define GUI_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( GUI_NODBG )
#define GUI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:29 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Graphical User Interface 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ids.h"

#include "wx/wx.h" 
#include "wx/treectrl.h"

namespace gui {

	class eTreeCtrl;

	typedef wxMenu			eMenu;
	typedef wxMenuBar		eMenuBar;
	typedef wxApp			eApp;
	typedef wxTreeEvent		eTreeEvent;
	typedef wxWindow		eWindow;
	typedef wxEvent			eEvent;
	typedef wxEventType		eEventType;
	typedef wxTreeItemData	eTreeItemData;
	typedef void (eTreeCtrl::*eTreeCtrlFunction)(eEvent&);
	typedef wxTreeItemId	eTreeItemId;
	typedef wxString			eString;
	typedef wxPoint			ePoint;
	typedef wxSize			eSize;
	typedef wxCommandEvent	eCommandEvent;

	ePoint &eDefaultPosition = *(ePoint *)&wxDefaultPosition;
	eSize &eDefaultSize = *(eSize *)&wxDefaultSize;

#define	eMessageBox	wxMessageBox;
#define eEVT_COMMAND_TREE_ITEM_COLLAPSING	wxEVT_COMMAND_TREE_ITEM_COLLAPSING
#define eEVT_COMMAND_TREE_ITEM_EXPANDING	wxEVT_COMMAND_TREE_ITEM_EXPANDING
	class eFrame
	: public wxFrame
	{
	private:
		ids::E_IDS_STORE( int ) Ids;
	public:
		eFrame( const eString& title,
               const ePoint& pos = eDefaultPosition,
               const eSize& size = eDefaultSize )
			   : wxFrame( NULL, wxID_ANY, title, pos, size )
		{}
	};

	class eTreeCtrl
	: public wxTreeCtrl
	{
	public:
		eTreeCtrl( eWindow* parent )
		: wxTreeCtrl( parent )
		{}
		void Connect( eEventType eventType,
					  eTreeCtrlFunction func )
		{
			wxTreeCtrl::Connect( wxID_ANY, eventType, (wxObjectEventFunction)func );
		}
	    bool Disconnect( eEventType eventType,
					  eTreeCtrlFunction func )
		{
			return wxTreeCtrl::Disconnect( wxID_ANY, eventType, (wxObjectEventFunction)func );
		}

	};
}

/*$END$*/
#endif
