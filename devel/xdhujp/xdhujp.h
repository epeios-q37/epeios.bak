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

#ifndef XDHUJP_INC_
# define XDHUJP_INC_

# define XDHUJP_NAME		"XDHUJP"

# if defined( E_DEBUG ) && !defined( XDHUJP_NODBG )
#  define XDHUJP_DBG
# endif

// X(SL)/DH(TML) Upstream Javascript-related Proxy

# error "Deprecated!"

# include "xdhujs.h"

# include "str.h"
# include "err.h"

namespace xdhujp {
	class cJS
	{
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			str::dString &ReturnValue ) = 0;
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
	public:
		qCALLBACK( JS )
		void Execute(
			const str::string_ &Script,
			str::dString &ReturnValue )
		{
            XDHUJPExecute( Script, ReturnValue );
		}
		void Execute( const str::string_ &Script )
		{
		qRH
            str::wString Dummy;
        qRB
            Dummy.Init();
			Execute( Script, Dummy );
        qRR
        qRE
        qRT
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

	void Execute(
		cJS &Callback,
		const char *ScriptName,
		const str::dStrings &Values,
		str::dString &ReturnValue );

	void Execute(
		cJS &Callback,
		const char *ScriptName,
		const str::dStrings &Values );

	typedef xdhcmn::cUpstream cUpstream_;

	// Was 'sProxyCallback'.
	class sUpstream
	: public cUpstream_
	{
	private:
		Q37_MRMDF( cJS, C_, Callback_ );
	protected:
		virtual void XDHCMNProcess(
			const char *ScriptName,
			const str::dStrings &Values,
			str::dString &ReturnValue ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Callback_ );
		}
		E_CVDTOR( sUpstream );
		void Init( cJS &Callback )
		{
			Callback_ = &Callback;
		}
	};

    // Fill the main page with the content of 'HTML'.
    // Used to report an errro.
    void SetMainPage(
    	cJS  &Callback,
        const str::dString &HTML);
}

#endif
