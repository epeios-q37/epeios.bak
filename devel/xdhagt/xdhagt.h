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

#ifndef XDHAGT__INC
# define XDHAGT__INC

# define XDHAGT_NAME		"XDHAGT"

# if defined( E_DEBUG ) && !defined( XDHAGT_NODBG )
#  define XDHAGT_DBG
# endif

// X(SL)/DH(TML) AGenT

# include "xdhcbk.h"

# include "err.h"
# include "ntvstr.h"

namespace xdhagt {
	typedef ntvstr::string___ nstring___;

	class agent___ {
	private:
		Q37_MRMDF( xdhcbk::upstream_callback__, _C, _Callback );
		void *_UP;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _UP != NULL )
					_C().Delete( _UP );

			_Callback = NULL;
			_UP = NULL;
		}
		E_CDTOR( agent___ );
		void Init( xdhcbk::upstream_callback__ &Callback )
		{
			_Callback = &Callback;
			_UP = _C().New();
		}
# if 0
		const char *GetLanguage( TOL_CBUFFER___ &Buffer )
		{
			return _C().GetLanguage( Buffer );
		}
		void ExecuteJavascript( const nstring___ &Script )
		{
			return _C().ExecuteJavascript( Script );
		}
# endif
		const char *GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetAttribute( _UP, Id, Name, Buffer );
		}
		const str::string_ &GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			str::string_ &Value )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Value.Append( GetAttribute( Id, Name, Buffer ) );
		qRR
		qRT
		qRE
			return Value;
		}
		void SetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			_C().SetAttribute(_UP, Id, Name, Value );
		}
		const char *GetResult(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetResult( _UP, Id, Buffer );
		}
		const str::string_ &GetResult(
			const nstring___ &Id,
			str::string_ &Value )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Value.Append( GetResult( Id, Buffer ) );
		qRR
		qRT
		qRE
			return Value;
		}
		void RemoveAttribute(
			const nstring___ &Id,
			const nstring___ &Name )
		{
			_C().RemoveAttribute( _UP, Id, Name );
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
			return _C().GetProperty( _UP, Id, Name, Buffer );
		}
		void SetContent(
			const nstring___ &Id,
			const str::string_ &Value )
		{
			_C().SetContent( _UP, Id, Value );
		}
		const char *GetContent(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().GetContent( _UP, Id, Buffer );
		}
		const str::string_ &GetContent(
			const nstring___ &Id,
			str::string_ &Value )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Value.Append( GetContent( Id, Buffer ) );
		qRR
		qRT
		qRE
			return Value;
		}
		void Focus( const nstring___ &Id )
		{
			return _C().Focus( _UP, Id );
		}
		void SetChildren(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			_C().SetChildren( _UP, Id, XML, XSL );
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
			_C().SetCasting( _UP, Id, XML, XSL );
		}
		void SetDocumentCasting(
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			_C().SetCasting( _UP, (const char *)NULL, XML, XSL );
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
		// 'Raw...', parce qu'ils affichent le message tel quel, pour viter une confusion avec les versions affichant les messages traduits.
		void RawAlert( const str::string_ &Message );
		void RawAlert( const char *Message )
		{
			RawAlert( str::string( Message ) );
		}
		bso::bool__ Confirm(
			const str::string_ &XML,
			const str::string_ &XSL,
			const str::string_ &Title = str::string() );
		// 'Raw...', parce qu'ils affichent le message tel quel, pour viter une confusion avec les versions affichant les messages traduits.
		bso::bool__ RawConfirm( const str::string_ &Message );
		bso::bool__ RawConfirm( const char *Message )
		{
			return RawConfirm( str::string( Message ) );
		}
		void Log( const str::string_ &RawMessage );
	};
}

#endif
