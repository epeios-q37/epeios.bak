/*
	'xhtagent.h' by Claude SIMON (http://zeusw.org/).

	'xhtagent' is part of the Epeios framework.

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

#ifndef XHTAGENT__INC
# define XHTAGENT__INC

# define XHTAGENT_NAME		"XHTAGENT"

# if defined( E_DEBUG ) && !defined( XHTAGENT_NODBG )
#  define XHTAGENT_DBG
# endif

/******************************************************************************/
/* do not modify anything above this limit */
/*			  unless specified			 */
/*******************************************/

// XHT(ML) AGENT

# include "xhtcllbk.h"

# include "err.h"
# include "flw.h"

namespace xhtagent {

	typedef ntvstr::string___ nstring___;

	class agent___ {
	private:
		xhtcllbk::upstream_callback__ *_Callback;
		xhtcllbk::upstream_callback__ &_C( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Callback = NULL;
		}
		E_CDTOR( agent___ );
		void Init( xhtcllbk::upstream_callback__ &Callback )
		{
			_Callback = &Callback;
		}
		const char *GetLanguage( TOL_CBUFFER___ &Buffer )
		{
			return _C().GetLanguage( Buffer );
		}
		void ExecuteJavascript( const nstring___ &Script )
		{
			return _C().ExecuteJavascript( Script );
		}
		const char *GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetAttribute( Id, Name, Buffer );
		}
		const str::string_ &GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			str::string_ &Value )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			Value.Append( GetAttribute( Id, Name, Buffer ) );
		ERRErr
		ERREnd
		ERREpilog
			return Value;
		}
		void SetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			_C().SetAttribute( Id, Name, Value );
		}
		const char *GetResult(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetResult( Id, Buffer );
		}
		const str::string_ &GetResult(
			const nstring___ &Id,
			str::string_ &Value )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			Value.Append( GetResult( Id, Buffer ) );
		ERRErr
		ERREnd
		ERREpilog
			return Value;
		}
		void RemoveAttribute(
			const nstring___ &Id,
			const nstring___ &Name )
		{
			_C().RemoveAttribute( Id, Name );
		}
		void SetString(
			const nstring___ &Id,
			const nstring___ &Name,
			const str::string_ &Value );
		const char *GetString(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetProperty( Id, Name, Buffer );
		}
		void SetContent(
			const nstring___ &Id,
			const str::string_ &Value )
		{
			_C().SetContent( Id, Value );
		}
		const char *GetContent(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetContent( Id, Buffer );
		}
		const str::string_ &GetContent(
			const nstring___ &Id,
			str::string_ &Value )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			Value.Append( GetContent( Id, Buffer ) );
		ERRErr
		ERREnd
		ERREpilog
			return Value;
		}
		void Focus( const nstring___ &Id )
		{
			return _C().Focus( Id );
		}
		void SetChildren(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			_C().SetChildren( Id, XML, XSL );
		}
		void SetDocument(
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			SetChildren( (const char *)NULL, XML, XSL );
		}
		void SetCasting(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			_C().SetCasting( Id, XML, XSL );
		}
		void SetDocumentCasting(
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			_C().SetCasting( (const char *)NULL, XML, XSL );
		}
		void Show(
			const nstring___ &Id,
			bso::bool__ Value = true )
		{
			if ( Value  )
				RemoveAttribute( Id, "hidden" );
			else
				SetAttribute( Id, "hidden", "hidden" );
		}
		void Hide(
			const nstring___ &Id,
			bso::bool__ Value = true )
		{
			Show( Id, !Value );
		}
		void Alert(
			const str::string_ &XML,
			const str::string_ &XSL,
			const str::string_ &Title = str::string() );
		void Alert( const str::string_ &Message );
		void Alert( const char *Message )
		{
			Alert( str::string( Message ) );
		}
		bso::bool__ Confirm(
			const str::string_ &XML,
			const str::string_ &XSL,
			const str::string_ &Title = str::string() );
		bso::bool__ Confirm( const str::string_ &Message );
		bso::bool__ Confirm( const char *Message )
		{
			return Confirm( str::string( Message ) );
		}
		void Log( const str::string_ &Message );
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
