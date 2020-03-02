/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

    XDHWebQ is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHWebQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XDWMAIN_INC_
# define XDWMAIN_INC_

# include "xdwrgstry.h"

# include "xdhujp.h"
# include "xdhups.h"

namespace xdwmain {
    class rSession; // Predeclaration.

    using xdhups::rAgent;

    typedef xdhujp::cJS cJS_;

	class sJS
	: public cJS_
	{
	private:
		Q37_MRMDF( rSession, S_, Session_ );
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			str::dString &ReturnValue ) override;
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( xdwrgstry::custom_item::attribute_name::Widget, Buffer);
		}
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( xdwrgstry::custom_item::attribute_name::Result, Buffer );
		}
		/*
		virtual void XDHJSPHandleExtensions( const xdhcbk::nstring___ &Id ) override;
		virtual void XDHJSPHandleCastings( const xdhcbk::nstring___ &Id ) override;
		*/
	public:
		void reset( bso::bool__ P = true )
		{
			Session_ = NULL;
		}
		E_CVDTOR( sJS );
		void Init( rSession &Session )
		{
			Session_ = &Session;
		}
	};


	class rSession
	{
	private:
		sJS _JSCallback;
		xdhups::sSession Session_;
	public:
		void reset( bso::bool__ P = true )
		{
			_JSCallback.reset( P );
			Session_.reset( P );
		}
		E_CDTOR( rSession );
		void Init( xdhcmn::cSession &SessionCallback )
		{
			_JSCallback.Init( *this );
			Session_.Init( &SessionCallback );
		}
		bso::sBool Initialize(
			xdhcmn::cUpstream *Callback,
			const char *Language,
			const str::dString &Token )	// If empty, PROD session, else token used for the DEMO session.
		{
			return Session_.Initialize(Callback, Language, Token);
		}
		cJS_ &JSCallback( void )
		{
			return _JSCallback;
		}
		xdhcmn::cSession *SessionCallback( void ) const
		{
			return Session_.Callback();
		}
	};
}

#endif
