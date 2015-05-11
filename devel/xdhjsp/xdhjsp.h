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

	E_ENUM( script_name ) {
		snLog,
		snDialogAlert,
		snDialogConfirm,
		snAttributeSetter,
		snAttributeGetter,
		snAttributeRemover,
		snPropertySetter,
		snPropertyGetter,
		snDocumentSetter,
		snContentSetter,
		snContentGetter,
		snCastingDefiner,
		snCastingHandler,
		snChildrenSetter,
		snWidgetContentRetriever,
		snWidgetFocusing,
		snFocusing,
		sn_amount,
		sn_Undefined,
	};

	const str::string_ &GetTaggedScript(
		script_name__ ScriptName,
		str::string_ &Buffer );

	const str::string_ &GetScript(
		script_name__ ScriptName,
		str::string_ *Script,	// Was '&Script', but should not work due 'va_start(...)' restrictions concerning references (but it worked under MSVC).
		... );

	namespace registry {
		using rgstry::entry___;

		extern entry___ RootTagId;

		extern entry___ CustomItems;

		namespace custom_item {
			extern entry___ AttributeNames;

			namespace attribute_name {
				extern entry___ Widget;
				extern entry___ Result;
			}
		}

		namespace script {
			extern entry___ Log;
			extern entry___ DocumentSetter;
			extern entry___ ChildrenSetter;
			extern entry___ Focusing;
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
	}

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
		script_name__ Script,
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
