/*
	Header for the 'uif' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: uif.h,v 1.6 2012/11/14 16:06:39 csimon Exp $

#ifndef UIF__INC
#define UIF__INC

#define UIF_NAME		"UIF"

#define	UIF_VERSION	"$Revision: 1.6 $"

#define UIF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &UIFTutor;

#if defined( E_DEBUG ) && !defined( UIF_NODBG )
#define UIF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:39 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D User Interface Framework (based on wxWidgets). 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tol.h"

#include <wx/wx.h>

#include "wx/treectrl.h"

namespace uif {
#define UIF_CONNECT( class )\
		void Connect(\
			int eventType,\
			eObjectEventFunction func,\
			eObject *userData = (eObject *) NULL )\
		{\
			class::Connect( wxID_ANY, eventType, func, userData );\
		}

#define UIF_DISCONNECT( class )\
		bool Disconnect(\
			eEventType eventType = wxEVT_NULL,\
			eObjectEventFunction func = NULL,\
			eObject *userData = (eObject *) NULL )\
		{\
			return class::Disconnect( wxID_ANY, eventType, func, userData );\
		}

#define UIF_CONNECT_DISCONNECT( class )\
		UIF_CONNECT( class )\
		UIF_DISCONNECT( class )

#define UIF_T( name )	typedef wx##name	e##name

	UIF_T( Object );
	UIF_T( Window );
	UIF_T( WindowID );
	UIF_T( ObjectEventFunction );
	UIF_T( TreeItemData );
	UIF_T( TreeItemId );
	UIF_T( TreeEvent );
	UIF_T( String );
	UIF_T( App );
	UIF_T( CommandEvent );
	UIF_T( Event );
	UIF_T( EventType );
	UIF_T( Point );
	UIF_T( Size );
	UIF_T( MenuBar );
	UIF_T( ItemKind );


	E_TYPEDEF( int, menu_item_id__ );

	class eFrame
	: public wxFrame
	{
	public:
		eFrame(
			eWindow *parent,
			eWindowID id,
			const eString& title,
			const ePoint& pos = wxDefaultPosition,
			const eSize& size = wxDefaultSize,
			long style = wxDEFAULT_FRAME_STYLE,
			const eString& name = wxFrameNameStr)
		: wxFrame( parent, id, title, pos, size, style, name )
		{}
		UIF_CONNECT_DISCONNECT( wxFrame )
		void Connect(
			menu_item_id__ Id,
			eObjectEventFunction func,
			eObject *userData = (eObject *) NULL )
		{
			wxFrame::Connect( *Id, wxEVT_COMMAND_MENU_SELECTED, func, userData );
		}
		void Disconnect(
			menu_item_id__ Id,
			eObjectEventFunction func,
			eObject *userData = (eObject *) NULL )
		{
			wxFrame::Disconnect( *Id, wxEVT_COMMAND_MENU_SELECTED, func, userData );
		}
	};


	class eMenuItem;	// Predeclaration.

	class eMenu
	: public wxMenu
	{
	public:
		menu_item_id__ Append(
			const eString& text,
			const eString& help = wxEmptyString,
			eItemKind kind = wxITEM_NORMAL );
	};

	class eMenuItem
	: public wxMenuItem
	{
	private:
		menu_item_id__ Id_;
		static menu_item_id__ _GetId( void );
		static void _ReleaseId( menu_item_id__ Id );
	public:
		eMenuItem::eMenuItem(
			eMenu *parentMenu = (eMenu *)NULL,
            const eString& name = wxEmptyString,
            const eString& help = wxEmptyString,
            eItemKind kind = wxITEM_NORMAL,
            eMenu *subMenu = (eMenu *)NULL)
		: wxMenuItem( parentMenu, wxID_ANY,	name, help, kind, subMenu )
		{
			Id_ = _GetId();

			SetId( *Id_ );
		}
		virtual ~eMenuItem( void )
		{
			_ReleaseId( Id_ );
		}
		menu_item_id__ GetId( void ) const
		{
			return Id_;
		}
	};

	inline menu_item_id__ eMenu::Append(
		const eString& text,
		const eString& help,
		eItemKind kind )
	{
		eMenuItem *MenuItem = new eMenuItem( this, text, help, kind );

		DoAppend( MenuItem );

		return MenuItem->GetId();
	}

	class eTreeCtrl
	: public wxTreeCtrl
	{
	public:
		eTreeCtrl( eWindow* parent )
		: wxTreeCtrl( parent )
		{}
		UIF_CONNECT_DISCONNECT( wxTreeCtrl )
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
