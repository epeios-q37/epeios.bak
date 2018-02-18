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
	class cJS
	{
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Buffer ) = 0;
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
	public:
		qCALLBACK( JS )
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
		cJS &Callback,
		xdhujs::script_name__ Script,
		TOL_CBUFFER___ *Buffer,
		... );

	typedef xdhcmn::cProxy cProxy_;

	class sProxyCallback
	: public cProxy_
	{
	private:
		Q37_MRMDF( cJS, C_, Callback_ );
	protected:
		virtual void XDHCMNProcess(
			xdhcmn::function__ Function,
			TOL_CBUFFER___ *Result,
			va_list List ) override; 
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Callback_ );
		}
		E_CVDTOR( sProxyCallback );
		void Init( cJS &Callback )
		{
			Callback_ = &Callback;
		}
	};

	

}

#endif
