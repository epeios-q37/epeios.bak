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
# include "sclmisc.h"
# include "xhtfmn.h"

namespace sclxhtml {
	typedef xhtcllbk::event_handler__ _event_handler__;

	template <typename callback > class event_handler__
	: public _event_handler__
	{
	private:
		callback *_Callback;
	protected:
		virtual void SCLXHTMLHandle( void ) = 0;
		virtual void XHTCLLBKHandle( void )
		{
		ERRProlog
			str::string Message;
			err::buffer__ Buffer;
		ERRBegin
			SCLXHTMLHandle();
		ERRErr
			switch ( ERRType ) {
			case err::t_Abort:
				Message.Init();
				if ( sclerror::GetPendingError(sclmisc::GetLanguage(), Message) ) {
					A().Alert( Message );
					sclerror::ResetPendingError();
				} else
					A().Alert("?");
				break;
			case err::t_Free:
			case err::t_Return:
				A().Alert( "???" );
				break;
			default:
				A().Alert( err::Message( Buffer ) );
				break;
			}

			ERRRst();
		ERREnd
		ERREpilog
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handler__::reset( P );
			_Callback = NULL;
		}
		E_CVDTOR( event_handler__ );
		void Init(
			const char *EventName,
			callback &Callback )
		{
			_event_handler__::Init();
			_Callback = &Callback;

			C().A().AddEventHandler( EventName, *this );
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
		virtual void SCLXHTMLStart( xhtagent::agent___ &Agent ) = 0;
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
		const rgstry::tentry__ &FileName,
		str::string_ &String );

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		str::string_ &String,
		bso::char__ Marker = '#' )
	{
		sclmisc::LoadXMLAndTranslateTags( FileName, sclrgstry::GetRegistry(), sclmisc::GetLanguage(), String, Marker );
	}

	void MainSubmission(
		xhtagent::agent___ &Agent,
		xml::writer_ &Writer );

	callback__ *SCLXHTMLRetrieveCallback( xhtagent::agent___ &Agent );	// A surcharger.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
