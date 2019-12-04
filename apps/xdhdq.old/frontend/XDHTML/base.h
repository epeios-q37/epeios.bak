/*
	'base.h' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BASE_INC_
# define BASE_INC_

# include "sclxdhtml.h"

# include "xddfbc.h"
# include "xddinf.h"

# include "frdfrntnd.h"

# include "xdhdws.h"

# define BASE_NAME				XDDINF_LC_AFFIX	SCLXDHTML_DEFAULT_SUFFIX
# define BASE_AUTHOR_NAME		XDDINF_AUTHOR_NAME
# define BASE_AUTHOR_CONTACT	XDDINF_AUTHOR_CONTACT
# define BASE_OWNER_NAME		XDDINF_OWNER_NAME
# define BASE_OWNER_CONTACT		XDDINF_OWNER_CONTACT
# define BASE_COPYRIGHT_YEARS	COPYRIGHT_YEARS
# define BASE_COPYRIGHT			BASE_COPYRIGHT_YEARS " " BASE_OWNER_NAME " (" BASE_OWNER_CONTACT ")"
# define BASE_SOFTWARE_NAME		XDDINF_SOFTWARE_NAME
# define BASE_SOFTWARE_VERSION	XDDINF_SOFTWARE_VERSION
# define BASE_SOFTWARE_DETAILS	XDDINF_SOFTWARE_NAME " V" XDDINF_SOFTWARE_VERSION
# define BASE_SOFTWARE_URL		XDDINF_SOFTWARE_URL


/***** Macros dealing with actions ****/
// Definition.
# define BASE_ACD( type )\
	class type##__\
	: public base::action_callback__\
	{\
	protected:\
		virtual void SCLXDHTMLLaunch(\
			core::session___ &Session,\
			const char *Id ) override;\
	}

// Utilisation.
# define BASE_ACU( type, name )\
	type##__  name

// 'reset(...)'.
# define BASE_ACR( name )\
	name.reset( P )

// Initialisation.
# define BASE_ACI( name )\
	name.Init( #name );\
	base::AddActionCallback( #name, name )

# define BASE_AC( name )\
	void name::SCLXDHTMLLaunch(\
		core::session___ &Session,\
		const char *Id )
/**********/

// Prdeclaration.

namespace core {
	class session___;
}

namespace base {
	E_CDEF( char *, Name, BASE_NAME );

	typedef sclxdhtml::action_callback__<core::session___> action_callback__;

	void AddActionCallback(
		const char *Name,
		action_callback__ &Callback );

	class action_helper_callback__
	: public sclxdhtml::action_helper_callback__<core::session___>
	{
	protected:
		virtual bso::bool__ SCLXHTMLOnBeforeAction(
			core::session___ &Session,
			const char *Id,
			const char *Action ) override;
		virtual bso::bool__ SCLXHTMLOnClose( core::session___ &Session ) override;
	};

	typedef xdhdws::corpus_callback__ _corpus_callback__;

	class corpus_callback__
	: public _corpus_callback__
	{
	private:
		Q37_MRMDF( frdfrntnd::frontend___, _F,  _Frontend );
	protected:
		virtual void XDHDWSDump( xml::writer_ &Writer ) override
		{
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_corpus_callback__::reset( P );
			_Frontend = NULL;
		}
		E_CVDTOR( corpus_callback__ );
		void Init( frdfrntnd::frontend___ &Frontend )
		{
			_corpus_callback__::Init();
			_Frontend = &Frontend;
		}
	};

	XDHDWS_RACKS( Name );

	template <typename rack> class _rack___
	: public rack
	{
	private:
		corpus_callback__ _Callback;
	public:
		void reset( bso::bool__ P = true )
		{
			rack::reset( P );
			_Callback.reset( P );
		}
		E_CDTOR( _rack___ );
		void Init(
			const char *View,
			str::string_ &Target,
			frdfrntnd::frontend___ &Frontend )
		{
			_Callback.Init( Frontend );
			rack::Init( View, Target, _Callback );
		}
	};

	typedef _rack___<_content_rack___> content_rack___;
	typedef _rack___<_context_rack___> context_rack___;
}

#endif