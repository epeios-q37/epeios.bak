/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BASE__INC
# define BASE__INC

# include "sclxdhtml.h"

# include "ogzfbc.h"
# include "ogzinf.h"

# include "frdfrntnd.h"

# include "xdhdws.h"

# define BASE_NAME				OGZINF_LC_AFFIX	SCLXDHTML_DEFAULT_SUFFIX
# define BASE_AUTHOR_NAME		OGZINF_AUTHOR_NAME
# define BASE_AUTHOR_CONTACT	OGZINF_AUTHOR_CONTACT
# define BASE_OWNER_NAME		OGZINF_OWNER_NAME
# define BASE_OWNER_CONTACT		OGZINF_OWNER_CONTACT
# define BASE_COPYRIGHT_YEARS	COPYRIGHT_YEARS
# define BASE_COPYRIGHT			BASE_COPYRIGHT_YEARS " " BASE_OWNER_NAME " (" BASE_OWNER_CONTACT ")"
# define BASE_SOFTWARE_NAME		OGZINF_SOFTWARE_NAME
# define BASE_SOFTWARE_VERSION	OGZINF_SOFTWARE_VERSION
# define BASE_SOFTWARE_DETAILS	OGZINF_SOFTWARE_NAME " V" OGZINF_SOFTWARE_VERSION
# define BASE_SOFTWARE_URL		OGZINF_SOFTWARE_URL


/***** Macros dealing with actions ****/
// Definition.
# define BASE_ACD( type )\
	class f##type##\
	: public base::action_callback__\
	{\
	protected:\
		virtual void SCLXDHTMLLaunch(\
			core::rSession &Session,\
			const char *Id ) override;\
	}

// Utilisation.
# define BASE_ACU( name )\
	f##name  name

// 'reset(...)'.
# define BASE_ACR( name )\
	name.reset( P )

// Initialisation.
# define BASE_ACI( name )\
	name.Init( #name );\
	base::AddActionCallback( #name, name )

# define BASE_AC( name )\
	void name::SCLXDHTMLLaunch(\
		core::rSession &Session,\
		const char *Id )
/**********/

// Prdeclaration.

namespace core {
	class rSession;
}

namespace base {
	E_CDEF( char *, Name, BASE_NAME );

	typedef sclxdhtml::action_callback__<core::rSession> action_callback__;

	void AddActionCallback(
		const char *Name,
		action_callback__ &Callback );

	class action_helper_callback__
	: public sclxdhtml::action_helper_callback__<core::rSession>
	{
	protected:
		virtual bso::bool__ SCLXHTMLOnBeforeAction(
			core::rSession &Session,
			const char *Id,
			const char *Action ) override;
		virtual bso::bool__ SCLXHTMLOnClose( core::rSession &Session ) override;
	};

	typedef xdhdws::corpus_callback__ _corpus_callback__;

	class corpus_callback__
	: public _corpus_callback__
	{
	private:
		qRVM( frdfrntnd::rFrontend, F_,  Frontend_ );
	protected:
		virtual void XDHDWSDump( xml::writer_ &Writer ) override
		{
			if ( F_().IsConnected() )
				F_().DumpCorpus( Writer );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_corpus_callback__::reset( P );
			Frontend_ = NULL;
		}
		E_CVDTOR( corpus_callback__ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			_corpus_callback__::Init();
			Frontend_ = &Frontend;
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
			frdfrntnd::rFrontend &Frontend )
		{
			_Callback.Init( Frontend );
			rack::Init( View, Target, _Callback );
		}
	};

	typedef _rack___<_content_rack___> content_rack___;
	typedef _rack___<_context_rack___> context_rack___;


	void AddAllowedActionsOnWhenNotConnectedToBackend( const char *Action );

	void AddAllowedActionsOnWhenNotConnectedToBackend(
		const action_callback__  *FirstCallback,
		... /* other callbacks */ );	// Last must be 'NULL' !
}

#endif