/*
	Header for the 'gui' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
