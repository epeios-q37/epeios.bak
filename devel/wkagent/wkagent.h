/*
	'wkagent.h' by Claude SIMON (http://zeusw.org/).

	'wkagent' is part of the Epeios framework.

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

#ifndef WKAGENT__INC
# define WKAGENT__INC

# define WKAGENT_NAME		"WKAGENT"

# if defined( E_DEBUG ) && !defined( WKAGENT_NODBG )
#  define WKAGENT_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// WebKit AGENT

# include "err.h"
# include "flw.h"

#include "wkcllbck.h"

namespace wkagent {
	class agent___ {
	private:
		wkcllbck::upstream_callback__ *_Callback;
		wkcllbck::upstream_callback__ &_C( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		wkcllbck::actions _Actions;
	public:
		void reset( bso::bool__ P = true )
		{
			_Callback = NULL;
			_Actions.reset( P );
		}
		E_CDTOR( agent___ );
		void Init( wkcllbck::upstream_callback__ &Callback )
		{
			_Callback = &Callback;
			_Actions.Init();
		}
		const wkcllbck::actions_ &Actions( void ) const
		{
			return _Actions;
		}
		void EmptyActions( void )
		{
			_Actions.Init();
		}
		void AddAction(
			const char *Name,
			wkcllbck::action_callback__ &Callback )
		{
			if ( !_Actions.Add( Name, Callback ) )
				ERRFwk();
		}
		const char *ExecuteJavascript(
			const char *Script,
			TOL_CBUFFER___ &Buffer )
		{
			return _C().ExecuteJavascript( Script, Buffer );
		}
		void Set(
			const char *Id,
			const char *Name,
			const char *Value )
		{
			_C().Set( Id, Name, Value );
		}
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
		const char *GetSelectValue(
			const char *Id,
			TOL_CBUFFER___ &Buffer );	// Retourne la valeur de l'item ('OPTION') actuellement sélectionné du 'SELECT' d'identifiant 'Id'.
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
