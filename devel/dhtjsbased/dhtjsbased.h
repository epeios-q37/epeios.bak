/*
	'dhtjsbased.h' by Claude SIMON (http://zeusw.org/).

	'dhtjsbased' is part of the Epeios framework.

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

# include "xhtcllbk.h"

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
		s_amount,
		s_Undefined,
	};

	class callback__
	{
	protected:
		virtual void DHTJSBASEDExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual const str::string_ &DHTJSBASEDGetScript(
			script__ Script,
			str::string_ &Buffer ) = 0;
		virtual str::string_ &DHTJSBASEDGetTranslation(
			const char *Message,
			str::string_ &Buffer ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standadization;
		}
		E_CVDTOR( callback__);
		void Init( void )
		{
			// Standadization;
		}
		void Execute(
			const str::string_ &Script,
			TOL_CBUFFER___ &Buffer )
		{
			DHTJSBASEDExecute( Script, Buffer );
		}
		void Execute( const str::string_ &Script )
		{
			DHTJSBASEDExecute( Script,  *(TOL_CBUFFER___ *)NULL );
		}
		const str::string_ &GetScript(
			script__ Script,
			str::string_ &Buffer )
		{
			return DHTJSBASEDGetScript( Script, Buffer );
		}
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Buffer )
		{
			return DHTJSBASEDGetTranslation( Message, Buffer );
		}
	};

	typedef xhtcllbk::proxy_callback__ _proxy_callback__;

	class proxy_callback__
	: public _proxy_callback__
	{
	protected:
		virtual void XHTCLLBKProcess(
			xhtcllbk::function__ Function,
			...	) override; 
		virtual void XHTCLLBKGetLanguage(
			TOL_CBUFFER___ &Buffer ) override;
		virtual void XHTCLLBKExecuteJavascript(
			const xhtcllbk::nchar__ *Script,
			TOL_CBUFFER___ &Buffer ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			_proxy_callback__::reset( P );
		}
		E_CVDTOR( proxy_callback__);
		void Init( void )
		{
			_proxy_callback__::Init();
		}
	};

	
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
