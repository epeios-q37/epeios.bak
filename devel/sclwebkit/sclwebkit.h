/*
	'sclwebkit.h' by Claude SIMON (http://zeusw.org/).

	'sclwebkit' is part of the Epeios framework.

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

#ifndef SCLWEBKIT__INC
# define SCLWEBKIT__INC

# define SCLWEBKIT_NAME		"SCLWEBKIT"

# if defined( E_DEBUG ) && !defined( SCLWEBKIT_NODBG )
#  define SCLWEBKIT_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe WEBKIT

# include "err.h"
# include "flw.h"
# include "wkagent.h"
# include "sclrgstry.h"

namespace sclwebkit {
	typedef wkcllbck::action_callback__ action__;

	// L'utilisateur met dans la classe mère ses propres objets et l'instancie par un 'new', et il est assuré qu'un 'delete' sera fait une fois la bibliothèque déchargée.
	class callback__
	{
	private:
		action__ *_StartAction;	// Action à lancer au lancement du programme.
	public:
		void reset( bso::bool__ = true )
		{
			_StartAction = NULL;
		}
		E_CVDTOR( callback__ )
		void Init( action__ &StartAction )
		{
			_StartAction = &StartAction;
		}
		action__ &StartAction( void ) const
		{
			if ( _StartAction == NULL )
				ERRFwk();

			return *_StartAction;
		}
	};

	void Load(
		const rgstry::entry___ &FileName,
		str::string_ &String );

	callback__ *SCLWEBKITLaunch( wkagent::agent___ &Agent );	// A surcharger.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
