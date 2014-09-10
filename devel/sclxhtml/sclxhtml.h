/*
	'sclxhtml.h' by Claude SIMON (http://zeusw.org/).

	'sclxhtml' is part of the Epeios framework.

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

#ifndef SCLXHTML__INC
# define SCLXHTML__INC

# define SCLXHTML_NAME		"SCLXHTML"

# if defined( E_DEBUG ) && !defined( SCLXHTML_NODBG )
#  define SCLXHTML_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe XHTML

# include "xhtagent.h"

# include "err.h"
# include "flw.h"
# include "rgstry.h"
# include "scllocale.h"

namespace sclxhtml {
	typedef xhtmlcbk::action_callback__ _action_callback__;

	template <typename callback > class action__
	: public _action_callback__
	{
	private:
		callback *_Callback;
	public:
		void reset( bso::bool__ P = true )
		{
			_action_callback__::reset( P );
			_Callback = NULL;
		}
		E_CVDTOR( action__ );
		void Init(
			const char *Name,
			callback &Callback )
		{
			_action_callback__::Init();
			_Callback = &Callback;

			C().A().AddAction( Name, *this );
		}
		callback &C( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		xhtagent::agent___ &A( void ) const
		{
			return C().A();
		}
	};

	// L'utilisateur met dans la classe mère ses propres objets et l'instancie par un 'new', et il est assuré qu'un 'delete' sera fait une fois la bibliothèque déchargée.
	class callback__
	{
	private:
		xhtagent::agent___ *_Agent;
	protected:
		virtual void SCLXHTMLStart(
			xhtagent::agent___ &Agent,
			str::string_ &XML,
			str::string_ &XSL ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Agent = NULL;
		}
		E_CVDTOR( callback__ )
		void Init( xhtagent::agent___ &Agent )
		{
			_Agent = &Agent;
		}
		void Start( void );
		xhtagent::agent___ &A( void ) const
		{
			if ( _Agent == NULL )
				ERRFwk();

			return *_Agent;
		}
	};

	void Load(
		const rgstry::entry___ &FileName,
		str::string_ &String );

	void LoadAndTranslateTags(
		const rgstry::entry___ &FileName,
		const char *LAnguage,
		str::string_ &String,
		char Marker = scllocale::DefaultMarker );

	callback__ *SCLXHTMLRetrieveCallback( xhtagent::agent___ &Agent );	// A surcharger.

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
