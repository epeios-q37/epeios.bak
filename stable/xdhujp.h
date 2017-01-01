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

#ifndef XDHUJP__INC
# define XDHUJP__INC

# define XDHUJP_NAME		"XDHUJP"

# if defined( E_DEBUG ) && !defined( XDHUJP_NODBG )
#  define XDHUJP_DBG
# endif

// X(SL)/DH(TML) Upstream Javascript-related Proxy

# include "xdhujs.h"

# include "str.h"
# include "err.h"

namespace xdhujp {
	class callback__
	{
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Buffer ) = 0;
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
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
			XDHUJPExecute( Script, Buffer );

			if ( Buffer != NULL )
				return *Buffer;
			else
				return NULL;
		}
		void Execute( const str::string_ &Script )
		{
			Execute( Script,  NULL );
		}
		const char *GetWidgetAttributeName( TOL_CBUFFER___ &Buffer )
		{
			XDHUJPGetWidgetAttributeName( Buffer );

			return Buffer;
		}
		const char *GetResultAttributeName( TOL_CBUFFER___ &Buffer )
		{
			XDHUJPGetResultAttributeName( Buffer );

			return Buffer;
		}
	};

	const char *Execute(
		callback__ &Callback,
		xdhujs::script_name__ Script,
		TOL_CBUFFER___ *Buffer,
		... );

	typedef xdhcmn::proxy_callback__ _proxy_callback__;

	class proxy_callback__
	: public _proxy_callback__
	{
	private:
		Q37_MRMDF( callback__, C_, Callback_ );
	protected:
		virtual void XDHCMNProcess(
			xdhcmn::function__ Function,
			TOL_CBUFFER___ *Result,
			va_list List ) override; 
	public:
		void reset( bso::bool__ P = true )
		{
			_proxy_callback__::reset( P );
			Callback_ = NULL;
		}
		E_CVDTOR( proxy_callback__);
		void Init( callback__ &Callback )
		{
			_proxy_callback__::Init();
			Callback_ = &Callback;
		}
	};

	

}

#endif
