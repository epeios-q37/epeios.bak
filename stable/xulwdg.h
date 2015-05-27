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

#ifndef XULWDG__INC
#define XULWDG__INC

#define XULWDG_NAME		"XULWDG"

#define	XULWDG_VERSION	"$Revision: 1.23 $"

#define XULWDG_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULWDG_NODBG )
#define XULWDG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.23 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/01/11 17:35:04 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL WiDGets 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "nsxpcm.h"

#if 0
// Sensitive widget (reacts to events).
#define XULWDG_SWIDGET( widget, name )\
	class name\
	: public widget\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	}

// Unsensitive widget (reacts to no event).
#define XULWDG_UWIDGET( widget, name )\
	class name\
	: public widget\
	{}
#endif

namespace xulwdg {

	template <typename trunk> class _trunk_depot__
	{
	private:
		 trunk *_Trunk;
	public:
		void reset( bso::bool__ P = true )
		{
			_Trunk = NULL;
		}
		E_CVDTOR( _trunk_depot__ );
		void Init( trunk &Trunk )
		{
			_Trunk = &Trunk;
		}
		const trunk &Trunk( void ) const
		{
			if ( _Trunk == NULL )
				ERRFwk();

			return *_Trunk;
		}
		trunk &Trunk( void )
		{
			if ( _Trunk == NULL )
				ERRFwk();

			return *_Trunk;
		}
	};


# define XULWDG__WN( widget, name )\
	typedef nsxpcm::widget##__ name##__;
//	template <typename trunk> E_TTCLONE__( _widget__< E_COVER2( trunk, nsxpcm::widget##__ )>, name##__ );


# define XULWDG__W( widget )	XULWDG__WN( widget, widget )

	XULWDG__WN( widget, box );
	XULWDG__W( button );
	XULWDG__W( checkbox );
	XULWDG__W( deck );
	XULWDG__W( description );
	XULWDG__W( listbox );
	XULWDG__W( menu );
	XULWDG__W( menuitem );
	XULWDG__W( menulist );
	// XULWDG__W( Page );	// Dfinit explicitement  cause de 'XULWDGRefresh(...)'.
	XULWDG__W( radio );
	XULWDG__W( radiogroup );
	XULWDG__W( tabs );
	XULWDG__W( tabpanels );
	XULWDG__W( textbox );
	//	XULWDG__W( tree );	// Dfinit explictiement ci-dessous  cause de 'Init(...)' de 'nsxpcm::tree__'.
	XULWDG__W( widget );
	// XULWDG__W( window );	// Dfinit explicitement  cause de 'XULWDGRefresh(...)'.

	typedef nsxpcm::tree__ _tree__;

	template <typename trunk> class tree__
	: public _tree__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_tree__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( tree__ );
		void Init(
			nsxpcm::tree_view_callback__ &Callback,
			trunk &Trunk,
			const nsxpcm::supports__ &Supports )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_tree__::Init( Callback, Supports );
		}
	};

	class _wp_core__
	{
	protected:
		virtual void XULWDGRefresh(
			nsIDOMWindow *&Window,
			nsIDOMElement *&Broadcasters,
			xml::writer_ &Digest,
			str::string_ &XSLFileName ) = 0;
	public:
		void reset( bso::bool__ = true )
		{}
		E_CVDTOR( _wp_core__ );
		void Init( void )
		{
			// Standardisation.
		}
		void Refresh( void );
	};

	template <typename trunk, typename wp> class _wp__
	: public wp,
	  public _wp_core__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			wp::reset( P );
			_wp_core__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( _wp__ );
		void Init( trunk &Trunk )
		{
			wp::Init();
			_wp_core__::Init();
			_trunk_depot__<trunk>::Init( Trunk );
		}
	};

	template <typename trunk> E_TTCLONE__( _wp__<E_COVER2( trunk, nsxpcm::window__ )>, window__);
	template <typename trunk> E_TTCLONE__( _wp__<E_COVER2( trunk, nsxpcm::page__ )>, page__);

	typedef nsxpcm::autocomplete_textbox__ _autocomplete_textbox__;

	template <typename trunk> class autocomplete_textbox__
	: public _autocomplete_textbox__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_autocomplete_textbox__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( autocomplete_textbox__ );
		void Init(
			nsxpcm::autocomplete_textbox_callback__ &Callback,
			trunk &Trunk,
			const nsxpcm::supports__ &Supports )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_autocomplete_textbox__::Init( Callback, Supports );
		}
	};

	typedef nsxpcm::event_handler__ _event_handler__;

	template <typename trunk> class event_handler__
	: public _event_handler__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handler__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( event_handler__ );
		void Init( trunk &Trunk )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_event_handler__::Init();
		}
		void Add(
			nsISupports *Supports,
			int Events )
		{
			_event_handler__::Add( Supports, Events);
		}
		void Add(
			nsIDOMWindow *Window,
			const str::string_ &Id,
			int Events )
		{
			Add( nsxpcm::GetElementById( nsxpcm::GetDocument( Window ), Id ), Events );
		}
		void Add(
			nsIDOMWindow *Window,
			const char *Id,
			int Events )
		{
			Add( Window, str::string( Id ), Events );
		}
	};

}

/*
# define XULWDG__EH( name )\
	template <typename target> E_TTCLONE__( event_handler__<E_COVER2( target )>, name##_eh__ );
*/

# define XULWDG_WIDGET( name, target )	typedef xulwdg::name##__<target> name##__;\

/* Manque 'autocomplete_textbox__', car surcharg dans autre bibliothque.*/
# define XULWDG_ALMOST_ALL_WIDGETS( target )\
	XULWDG_WIDGET( box, target )\
	XULWDG_WIDGET( button, target )\
	XULWDG_WIDGET( checkbox, target )\
	XULWDG_WIDGET( deck, target )\
	XULWDG_WIDGET( description, target )\
	XULWDG_WIDGET( listbox, target )\
	XULWDG_WIDGET( menu, target )\
	XULWDG_WIDGET( menulist, target )\
	XULWDG_WIDGET( menuitem, target )\
	XULWDG_WIDGET( page, target )\
	XULWDG_WIDGET( radio, target )\
	XULWDG_WIDGET( radiogroup, target )\
	XULWDG_WIDGET( tabs, target )\
	XULWDG_WIDGET( tabpanels, target )\
	XULWDG_WIDGET( textbox, target )\
	XULWDG_WIDGET( tree, target )\
	XULWDG_WIDGET( widget, target )\
	XULWDG_WIDGET( window, target )



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
