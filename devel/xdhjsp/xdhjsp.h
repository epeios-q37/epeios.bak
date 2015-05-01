/*
	'xdhjsp.h' by Claude SIMON (http://zeusw.org/).

	'xdhjsp' is part of the Epeios framework.

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

#ifndef XDHJSP__INC
# define XDHJSP__INC

# define XDHJSP_NAME		"XDHJSP"

# if defined( E_DEBUG ) && !defined( XDHJSP_NODBG )
#  define XDHJSP_DBG
# endif

// X(SL) DH(TML) JavaScript Proxy

# include "xdhcbk.h"

# include "err.h"


namespace xdhjsp {

	E_ENUM( script ) {
		sDialogAlert,
		sDialogConfirm,
		sDocumentSetter,
		sChildrenSetter,
		sCastingDefiner,
		sCastingHandler,
		sPropertySetter,
		sPropertyGetter,
		sAttributeSetter,
		sAttributeGetter,
		sAttributeRemover,
		sWidgetContentRetriever,
		sContentSetter,
		sContentGetter,
		sWidgetFocusing,
		sFocusing,
		s_amount,
		s_Undefined,
	};

	const str::string_ &GetScript(
		script__ Script,
		str::string_ &Buffer );

	namespace script{
		using rgstry::entry___;

		extern entry___ DocumentSetter;
		extern entry___ ChildrenSetter;
		namespace casting {
			extern entry___ Definer;
			extern entry___ Handler;
		}
		namespace property {
			extern entry___ Setter;
			extern entry___ Getter;
		}
		namespace attribute {
			extern entry___ Setter;
			extern entry___ Getter;
			extern entry___ Remover;
		}
		namespace content {
			extern entry___ Setter;
			extern entry___ Getter;
		}
		extern entry___ Focusing;
		namespace dialog {
			extern entry___ Alert;
			extern entry___ Confirm;
		}
		namespace widget {
			extern entry___ Instantiation;
			extern entry___ ContentRetriever;
			extern entry___ Focusing;
		}
	}

	class callback__
	{
	protected:
		virtual void XDHJSPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XDHJSPGetScript(
			script__ Script,
			str::string_ &Buffer ) = 0;
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
			TOL_CBUFFER___ &Buffer )
		{
			XDHJSPExecute( Script, Buffer );

			return Buffer;
		}
		void Execute( const str::string_ &Script )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			Execute( Script,  Buffer );
		ERRErr
		ERREnd
		ERREpilog
		}
		const str::string_ &GetScript(
			script__ Script,
			str::string_ &Buffer )
		{
			XDHJSPGetScript( Script, Buffer );

			return Buffer;
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

	typedef xdhcbk::proxy_callback__ _proxy_callback__;

	class proxy_callback__
	: public _proxy_callback__
	{
	private:
		callback__ *_Callback;
		callback__ &_C( void ) const
		{
			if  ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
	protected:
		virtual void XDHCBKProcess(
			xdhcbk::function__ Function,
			...	) override; 
	public:
		void reset( bso::bool__ P = true )
		{
			_proxy_callback__::reset( P );
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
