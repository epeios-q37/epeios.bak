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

#ifndef XDHJSP__INC
# define XDHJSP__INC

# define XDHJSP_NAME		"XDHJSP"

# if defined( E_DEBUG ) && !defined( XDHJSP_NODBG )
#  define XDHJSP_DBG
# endif

// X(SL)/DH(TML) JavaScript-related Proxy

# include "xdhjst.h"

# include "xdhcbk.h"

# include "err.h"


namespace xdhjsp {
	class callback__
	{
	protected:
		virtual void XDHJSPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Buffer ) = 0;
		virtual void XDHJSPGetTranslation(
			const char *Message,
			str::string_ &Buffer ) = 0;
		virtual void XDHJSPGetRootTagId( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHJSPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHJSPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHJSPHandleExtensions( const xdhcbk::nstring___ &Id ) = 0;
		virtual void XDHJSPHandleCastings( const xdhcbk::nstring___ &Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardization;
		}
		E_CVDTOR( callback__);
		void Init( void )
		{
			// Standardization;
		}
		const char *Execute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Buffer )
		{
			XDHJSPExecute( Script, Buffer );

			if ( Buffer != NULL )
				return *Buffer;
			else
				return NULL;
		}
		void Execute( const str::string_ &Script )
		{
			Execute( Script,  NULL );
		}
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Buffer )
		{
			XDHJSPGetTranslation( Message, Buffer );

			return Buffer;
		}
		const char *GetRootTagId( TOL_CBUFFER___ &Buffer )
		{
			XDHJSPGetRootTagId( Buffer );

			return Buffer;
		}
		const char *GetWidgetAttributeName( TOL_CBUFFER___ &Buffer )
		{
			XDHJSPGetWidgetAttributeName( Buffer );

			return Buffer;
		}
		const char *GetResultAttributeName( TOL_CBUFFER___ &Buffer )
		{
			XDHJSPGetResultAttributeName( Buffer );

			return Buffer;
		}
		void HandleExtensions( const xdhcbk::nstring___ &Id )
		{
			XDHJSPHandleExtensions( Id );
		}
		void HandleCastings( const xdhcbk::nstring___ &Id )
		{
			XDHJSPHandleCastings( Id );
		}
	};

	const char *Execute(
		callback__ &Callback,
		xdhjst::script_name__ Script,
		TOL_CBUFFER___ *Buffer,
		... );

	typedef xdhcbk::proxy_callback__ _proxy_callback__;

	class proxy_callback__
	: public _proxy_callback__
	{
	private:
		Q37_MRMDF( callback__, _C, _Callback );
	protected:
		virtual void XDHCBKProcess(
			xdhcbk::function__ Function,
			TOL_CBUFFER___ *Result,
			va_list List ) override; 
	public:
		void reset( bso::bool__ P = true )
		{
			_proxy_callback__::reset( P );
			_Callback = NULL;
		}
		E_CVDTOR( proxy_callback__);
		void Init( callback__ &Callback )
		{
			_proxy_callback__::Init();
			_Callback = &Callback;
		}
	};

	

}

#endif
