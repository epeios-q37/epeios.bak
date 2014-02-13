/*
	'xulfbs.h' by Claude SIMON (http://zeusw.org/).

	'xulfbs' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XULFBS__INC
# define XULFBS__INC

# define XULFBS_NAME		"XULFBS"

# if defined( E_DEBUG ) && !defined( XULFBS_NODBG )
#  define XULFBS_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XUL Frontend BaSe

# include "err.h"
# include "flw.h"

# include "geckoo.h"

// Predeclarations
namespace xulftk {
	class trunk___;
};

namespace xulfrd {
	class ui___;
}

namespace xulfbs {

	void _Report(
		xulftk::trunk___ &Trunk,
		const char *Message );

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
		trunk &Trunk( void ) const
		{
			if ( _Trunk == NULL )
				ERRFwk();

			return *_Trunk;
		}
	};

	typedef nsxpcm::event_handler__ _event_handler__;

	template <typename trunk> class event_handler__
	: public _event_handler__,
	  public _trunk_depot__<trunk>
	{
	protected:
		virtual void NSXPCMOnErr( const char *Message )
		{
			_Report( Trunk(), Message );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handler__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( event_handler__ );
		void Init( trunk &Trunk )
		{
			_event_handler__::Init();
			_trunk_depot__<trunk>::Init( Trunk );
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

# define XULFBS__WN( widget, name )\
	typedef nsxpcm::widget##__ name##__;

# define XULFBS__W( widget )	XULFBS__WN( widget, widget )

	XULFBS__WN( widget, box );
	XULFBS__W( button );
	XULFBS__W( checkbox );
	XULFBS__W( deck );
	XULFBS__W( description );
	XULFBS__W( listbox );
	XULFBS__W( menu );
	XULFBS__W( menuitem );
	XULFBS__W( menulist );
	// XULFBS__W( Page );	// Définit explicitement à cause de 'XULFBSRefresh(...)'.
	XULFBS__W( radio );
	XULFBS__W( radiogroup );
	XULFBS__W( tabs );
	XULFBS__W( tabpanels );
	XULFBS__W( textbox );
	XULFBS__W( tree );
	XULFBS__W( widget );
	// XULFBS__W( window );	// Définit explicitement à cause de 'XULWDGRefresh(...)'.

	class _refresh_callback__
	{
	protected:
		virtual const char *XULFBSRefresh( xml::writer_ &Digest ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( _refresh_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		const char *Refresh( xml::writer_ &Digest )
		{
			return XULFBSRefresh( Digest );
		}
	};

	class _wp_core__
	{
	private:
		nsIDOMDocument *_Document;
		nsIDOMElement *_Broadcasterset;
		_refresh_callback__ *_Callback;
	protected:
		virtual xulftk::trunk___ &_Trunk( void ) const = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Document = NULL;
			_Broadcasterset = NULL;
			_Callback = NULL;
		}
		E_CVDTOR( _wp_core__ );
		void Init( _refresh_callback__ &Callback )
		{
			_Document = NULL;
			_Broadcasterset = NULL;
			_Callback = &Callback;
		}
		void Attach( nsIDOMDocument *Document )
		{
			_Document = Document;
			_Broadcasterset = nsxpcm::GetElementById( Document, "bcsAvailability" );
		}
		void Refresh( void );
	};

	template <typename trunk> class refresh_callback__
	: public _refresh_callback__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_refresh_callback__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( refresh_callback__ );
		void Init( trunk &Trunk )
		{
			_refresh_callback__::Init();
			_trunk_depot__<trunk>::Init( Trunk );
		}
	};

	typedef nsxpcm::window__ _window__;

	template <typename trunk> class window__
	: public _window__,
	  public _wp_core__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_window__::reset( P );
			_wp_core__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( window__ );
		void Init(
			_refresh_callback__ &Callback,
			trunk &Trunk )
		{
			_window__::Init();
			_wp_core__::Init( Callback );
			_trunk_depot__<trunk>::Init( Trunk );
		}
		void Attach( nsIDOMWindow *Window )
		{
			_window__::Attach( nsxpcm::supports__( Window ) );
			_wp_core__::Attach( nsxpcm::GetDocument( Window )  );
		}
	};

	typedef nsxpcm::page__ _page__;

	template <typename trunk> class page__
	: public _page__,
	  public _wp_core__,
	  public _trunk_depot__<trunk>
	{
	protected:
		virtual xulftk::trunk___ &_Trunk( void ) const
		{
			return Trunk();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_page__::reset( P );
			_wp_core__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( page__ );
		void Init(
			_refresh_callback__ &Callback,
			trunk &Trunk )
		{
			_page__::Init();
			_wp_core__::Init( Callback );
			_trunk_depot__<trunk>::Init( Trunk );
		}
		void Attach( nsIDOMWindow *Window )
		{
			_window__::Attach( nsxpcm::supports__( Window ) );
			_wp_core__::Attach( nsxpcm::GetDocument( Window )  );
		}
	};


	using nsxpcm::autocomplete_textbox__;


# define XULFBS_EH( name )\
	class name\
	: public xulfbs::event_handler__<xulftk::trunk___>\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	};

	typedef nsxpcm::autocomplete_textbox_callback__ _autocomplete_textbox_callback__;
	typedef geckoo::pseudo_event_callback__ _pseudo_event_callback__;

	class autocomplete_textbox_callback__
	: public _autocomplete_textbox_callback__
	{
	private:
		void NSXPCMGetValue(
			const str::string_ &Pattern,
			bso::uint__ Index,
			str::string_ &Value )
		{
			XULFBSGetValue( Pattern, Index, Value );
		}
		void NSXPCMGetLabel(
			const str::string_ &Pattern,
			bso::uint__ Index,
			str::string_ &Label )
		{
			XULFBSGetLabel( Pattern, Index, Label );
		}
		void NSXPCMGetComment(
			const str::string_ &Pattern,
			bso::uint__ Index,
			str::string_ &Comment )
		{
			XULFBSGetComment( Pattern, Index, Comment );
		}
		bso::uint__ NSXPCMGetMatchingCount( const str::string_ &Pattern )
		{
			return XULFBSGetMatchingCount( Pattern );
		}
	protected:
		virtual void XULFBSGetValue(
			const str::string_ &Pattern,
			bso::uint__ Index,
			str::string_ &Value ) = 0;
		virtual void XULFBSGetLabel(
			const str::string_ &Pattern,
			bso::uint__ Index,
			str::string_ &Label )
		{
			GetValue( Pattern, Index, Label );
		}
		virtual void XULFBSGetComment(
			const str::string_ &Pattern,
			bso::uint__ Index,
			str::string_ &Comment )
		{
			ERRFwk();
		}
		virtual bso::uint__ XULFBSGetMatchingCount( const str::string_ &Pattern ) = 0;
		virtual void XULFBSOnTextEntered( nsIDOMElement *Element ) = 0;
		virtual void XULFBSOnTextReverted( nsIDOMElement *Element ) = 0;
	public:
		void OnTextEntered( nsIDOMElement *Element )
		{
			XULFBSOnTextEntered( Element );
		}
		void OnTextReverted( nsIDOMElement *Element )
		{
			XULFBSOnTextReverted( Element );
		}
	};

	typedef geckoo::pseudo_event_callback__ _pseudo_event_callback__;

	class _autocomplete_event_callback__
	: public _pseudo_event_callback__
	{
	private:
		autocomplete_textbox_callback__ *_Callback;
	protected:
		autocomplete_textbox_callback__ &_C( void )
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_pseudo_event_callback__::reset( P );

			_Callback = NULL;
		}
		E_CVDTOR( _autocomplete_event_callback__ )
		void Init( autocomplete_textbox_callback__ &Callback )
		{
			reset();

			_pseudo_event_callback__::Init();
			
			_Callback = &Callback;
		}
	};

	class _ontextentered_callback__
	: public _autocomplete_event_callback__
	{
	protected:
		virtual void GECKOOHandle( nsIDOMElement *Element )
		{
			_C().OnTextEntered( Element );
		}
	};

	class _ontextreverted_callback__
	: public _autocomplete_event_callback__
	{
	protected:
		virtual void GECKOOHandle( nsIDOMElement *Element )
		{
			_C().OnTextReverted( Element );
		}
	};

	inline void PushDigestRootTag(
		const char *Target,
		xml::writer_ &Digest )	// Ne pas oublier de faire un 'PopTag'...
	{
		Digest.PushTag( "Availabilities" );
		Digest.PutAttribute( "Target", Target );
	}

	inline void _PushDigestTag( xml::writer_ &Digest )	// Ne pas oublier de faire un 'PopTag'...
	{
		Digest.PushTag( "Epeios" );
	}
}

# define XULFBS_WIDGET( name )	using xulfbs::name##__;

/* Manque 'autocomplete_textbox__', car surchargé dans autre bibliothèque.*/
# define XULFBS_ALMOST_ALL_WIDGETS( target )\
	XULFBS_WIDGET( box )\
	XULFBS_WIDGET( button )\
	XULFBS_WIDGET( checkbox )\
	XULFBS_WIDGET( deck )\
	XULFBS_WIDGET( description )\
	XULFBS_WIDGET( listbox )\
	XULFBS_WIDGET( menu )\
	XULFBS_WIDGET( menulist )\
	XULFBS_WIDGET( menuitem )\
	XULFBS_WIDGET( page )\
	XULFBS_WIDGET( radio )\
	XULFBS_WIDGET( radiogroup )\
	XULFBS_WIDGET( tabs )\
	XULFBS_WIDGET( tabpanels )\
	XULFBS_WIDGET( textbox )\
	XULFBS_WIDGET( tree )\
	XULFBS_WIDGET( widget )\
	XULFBS_WIDGET( window )



				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
