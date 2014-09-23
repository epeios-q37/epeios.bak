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
	class agent___ {
	private:
		xhtcllbk::upstream_callback__ *_Callback;
		xhtcllbk::upstream_callback__ &_C( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		xhtcllbk::event_handlers _Handlers;
	public:
		void reset( bso::bool__ P = true )
		{
			_Callback = NULL;
			_Handlers.reset( P );
		}
		E_CDTOR( agent___ );
		void Init( xhtcllbk::upstream_callback__ &Callback )
		{
			_Callback = &Callback;
			_Handlers.Init();
		}
		const xhtcllbk::event_handlers_ &Handlers( void ) const
		{
			return _Handlers;
		}
		void RemoveEventHandlers( void )
		{
			_Handlers.Init();
		}
		void AddEventHandler(
			const char *EventName,
			xhtcllbk::event_handler__ &Handler )
		{
			if ( !_Handlers.Add( EventName, Handler ) )
				ERRFwk();
		}
		const char *ExecuteJavascript(
			const char *Script,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().ExecuteJavascript( Script, Buffer );
		}
		void ExecuteJavascript( const char *Script );
		const char *Get(
			const char *Id,
			const char *Name,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().Get( Id, Name, Buffer );
		}
		const char *GetFileContent(
			const char *Id,
			TOL_CBUFFER___ &Buffer )
		{
			if ( str::string( Get(Id, "files.length", Buffer ) ).ToUInt() == 0 )
				return NULL;

			return _C().GetFileContent( Id, Buffer );
		}
		void Set(
			const char *Id,
			const char *Name,
			const char *Value );
		void Remove(
			const char *Id,
			const char *Name )
		{
			_C().Remove( Id, Name );
		}
		void SetChildren(
			const char *Id,
			const char *XML,
			const char *XSL )
		{
			_C().SetChildren( Id, XML, XSL );
		}
		void SetChildren(
			const char *Id,
			const str::string_ &XML,
			const str::string_ &XSL );
		void SetChildren(
			const str::string_ &Id,
			const str::string_ &XML,
			const str::string_ &XSL );
		void Show(
			const char *Id,
			bso::bool__ Value = true )
		{
			if ( Value  )
				Remove( Id, "hidden" );
			else
				Set( Id, "hidden", "hidden" );
		}
		void Hide(
			const char *Id,
			bso::bool__ Value = true )
		{
			Show( Id, !Value );
		}
		// Retournent la valeur de l'item ('OPTION') actuellement sélectionné du 'SELECT' d'identifiant 'Id'.
		const char *GetSelectValue(
			const char *Id,
			TOL_CBUFFER___ &Buffer );	
		const str::string_ &GetSelectValue(
			const char *Id,
			str::string_ &Buffer );
		void Alert( const str::string_ &Message );
		void Alert( const char *Message )
		{
			Alert( str::string( Message ) );
		}
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
