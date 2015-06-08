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

#ifndef DHTJSBASED__INC
# define DHTJSBASED__INC

# define DHTJSBASED_NAME		"DHTJSBASED"

# if defined( E_DEBUG ) && !defined( DHTJSBASED_NODBG )
#  define DHTJSBASED_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// DHT(ML) JavaScript BASED

# include "xdhcbk.h"

# include "err.h"
# include "flw.h"

namespace dhtjsbased {

	E_ENUM( script ) {
		sDialogAlert,
		sDialogConfirm,
		sDocumentSetter,
		sChildrenSetter,
		sCastingDefiner,
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

	class callback__
	{
	protected:
		virtual void DHTJSBASEDExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void DHTJSBASEDGetScript(
			script__ Script,
			str::string_ &Buffer ) = 0;
		virtual void DHTJSBASEDGetTranslation(
			const char *Message,
			str::string_ &Buffer ) = 0;
		virtual void DHTJSBASEDGetRootTagId( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void DHTJSBASEDGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void DHTJSBASEDGetResultAttributeName( TOL_CBUFFER___ &Buffer ) = 0;
		virtual void DHTJSBASEDHandleExtensions( const xdhcbk::nstring___ &Id ) = 0;
		virtual void DHTJSBASEDHandleCastings( const xdhcbk::nstring___ &Id ) = 0;
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
			DHTJSBASEDExecute( Script, Buffer );

			return Buffer;
		}
		void Execute( const str::string_ &Script )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Execute( Script,  Buffer );
		qRR
		qRT
		qRE
		}
		const str::string_ &GetScript(
			script__ Script,
			str::string_ &Buffer )
		{
			DHTJSBASEDGetScript( Script, Buffer );

			return Buffer;
		}
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Buffer )
		{
			DHTJSBASEDGetTranslation( Message, Buffer );

			return Buffer;
		}
		const char *GetRootTagId( TOL_CBUFFER___ &Buffer )
		{
			DHTJSBASEDGetRootTagId( Buffer );

			return Buffer;
		}
		const char *GetWidgetAttributeName( TOL_CBUFFER___ &Buffer )
		{
			DHTJSBASEDGetWidgetAttributeName( Buffer );

			return Buffer;
		}
		const char *GetResultAttributeName( TOL_CBUFFER___ &Buffer )
		{
			DHTJSBASEDGetResultAttributeName( Buffer );

			return Buffer;
		}
		void HandleExtensions( const xdhcbk::nstring___ &Id )
		{
			DHTJSBASEDHandleExtensions( Id );
		}
		void HandleCastings( const xdhcbk::nstring___ &Id )
		{
			DHTJSBASEDHandleCastings( Id );
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
				qRFwk();

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

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
