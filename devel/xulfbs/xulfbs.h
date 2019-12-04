/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef XULFBS_INC_
# define XULFBS_INC_

# define XULFBS_NAME		"XULFBS"

# if defined( E_DEBUG ) && !defined( XULFBS_NODBG )
#  define XULFBS_DBG
# endif

// XUL Frontend BaSe

# include "err.h"
# include "flw.h"

# include "geckoo.h"

# include "xulwdg.h"

// Predeclarations
namespace xulftk {
	class trunk___;
};

namespace xulfrd {
	class ui___;
}

namespace xulfbs {

	class _core__ {
	protected:
		virtual xulftk::trunk___ &_Trunk( void ) = 0;
		virtual nsIDOMWindow *_Window( void ) = 0;
		virtual nsIDOMDocument *_Document( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( _core__ );
		void Init( void )
		{
			//Standardisation.
		}
		xulftk::trunk___ &Trunk( void )
		{
			return _Trunk();
		}
		nsIDOMWindow *Window( void )
		{
			return _Window();
		}
		nsIDOMDocument *Document( void )
		{
			return _Document();
		}
	};

	void _Report(
		xulftk::trunk___ &Trunk,
		const char *Message );

	template <typename trunk> E_TTCLONE__( xulwdg::event_handler__<trunk>, _event_handler__ );
	template <typename trunk> E_TTCLONE__( xulwdg::tree__<trunk>, _tree__ );

# define XULFBS__WN( widget, name )\
	typedef xulwdg::widget##__ name##__;

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
	// XULFBS__W( Page );	// D�finit explicitement � cause de 'XULFBSRefresh(...)'.
	XULFBS__W( radio );
	XULFBS__W( radiogroup );
	XULFBS__W( tabs );
	XULFBS__W( tabpanels );
	XULFBS__W( textbox );
	// XULFBS__W( tree );	// D�finit explicitement � cause de l'utilisation d'un 'trunk'.
	XULFBS__W( widget );
	// XULFBS__W( window );	// D�finit explicitement � cause de 'XULWDGRefresh(...)'.

	class _wp_core__
	{
	private:
		nsIDOMWindow *_Window;
		nsIDOMElement *_Broadcasters;
	protected:
		virtual const char *XULFBSRefresh( xml::writer_ &Digest ) = 0;	// Retourne l'affixe du fichier XSL.
		const char *Refresh(
			nsIDOMWindow *&Window,
			nsIDOMElement *&Broadcasters,
			xml::writer_ &Digest )
		{
			Digest.PushTag( "States" );

			Window = _Window;
			Broadcasters = _Broadcasters;

			const char *XSLFileNameAffix = XULFBSRefresh( Digest );

			Digest.PopTag();

			return XSLFileNameAffix;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Window = NULL;
			_Broadcasters = NULL;
		}
		E_CVDTOR( _wp_core__ );
		void Init( void )
		{
			_Window = NULL;
			_Broadcasters = NULL;
		}
		void Attach( nsIDOMWindow *Window )
		{
			_Window = Window;
			_Broadcasters = nsxpcm::GetElementById( nsxpcm::GetDocument( Window ), "bcsShapes" );
		}
	};

	template <typename trunk, typename wp> class _wp__
	: public wp,
	  public _wp_core__
	{
	protected:
		virtual void XULWDGRefresh(
			nsIDOMWindow *&Window,
			nsIDOMElement *&Broadcasters,
			xml::writer_ &Digest,
			str::string_ &XSLFileName )
		{
			wp::Trunk().BuildXSLFileName( _wp_core__::Refresh( Window, Broadcasters, Digest ), XSLFileName );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			wp::reset( P );
			_wp_core__::reset( P );
		}
		E_CVDTOR( _wp__ );
		void Init( trunk &Trunk )
		{
			wp::Init( Trunk );
			_wp_core__::Init();
		}
		void Attach( nsIDOMWindow *Window )
		{
			wp::Attach( nsxpcm::supports__( Window ) );
			_wp_core__::Attach( Window );
		}
		void Refresh( void )
		{
			wp::Refresh();
		}
	};

	template <typename trunk> E_TTCLONE__( _wp__<E_COVER2( trunk, xulwdg::window__<trunk>)>, _window__ );
	template <typename trunk> E_TTCLONE__( _wp__<E_COVER2( trunk, xulwdg::page__<trunk>)>, _page__ );


	using nsxpcm::autocomplete_textbox__;


# define XULFBS__EH( name )\
	class name\
	: public xulfbs::_event_handler__<xulftk::trunk___>\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
		virtual void NSXPCMOnErr( const char *Message )\
		{\
			xulfbs::_Report( Trunk(), Message );\
		}\
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
			qRFwk();
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
				qRFwk();

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

/*
	inline void PushDigestRootTag(
		const char *Target,
		xml::writer_ &Digest )	// Ne pas oublier de faire un 'PopTag'...
	{
		Digest.PushTag( "Availabilities" );
		Digest.PutAttribute( "Target", Target );
	}
*/
	inline void _PushDigestTag( xml::writer_ &Digest )	// Ne pas oublier de faire un 'PopTag'...
	{
		Digest.PushTag( "Epeios" );
	}
}

# define XULFBS_WIDGET( name )	using xulfbs::name##__;
# define XULFBS_TWIDGET( trunk, name )	typedef xulfbs::_##name##__<trunk> name##__;

/* Manque 'autocomplete_textbox__', car surcharg� dans autre biblioth�que.*/
# define XULFBS_ALMOST_ALL_WIDGETS( trunk )\
	XULFBS_WIDGET( box )\
	XULFBS_WIDGET( button )\
	XULFBS_WIDGET( checkbox )\
	XULFBS_WIDGET( deck )\
	XULFBS_WIDGET( description )\
	XULFBS_WIDGET( listbox )\
	XULFBS_WIDGET( menu )\
	XULFBS_WIDGET( menulist )\
	XULFBS_WIDGET( menuitem )\
	XULFBS_TWIDGET( trunk, page )\
	XULFBS_WIDGET( radio )\
	XULFBS_WIDGET( radiogroup )\
	XULFBS_WIDGET( tabs )\
	XULFBS_WIDGET( tabpanels )\
	XULFBS_WIDGET( textbox )\
	XULFBS_TWIDGET( trunk, tree )\
	XULFBS_WIDGET( widget )\
	XULFBS_TWIDGET( trunk, window )



#endif
