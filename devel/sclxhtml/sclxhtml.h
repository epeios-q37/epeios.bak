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
# include "frdssn.h"

namespace sclxhtml {
	const sclrgstry::registry_ &GetRegistry( void );

	typedef xhtcllbk::event_handler__ _event_handler__;

	template <typename callback, typename agent> class event_handler__
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
					Agent().Alert( Message );
					sclerror::ResetPendingError();
				} else
					Agent().Alert("?");
				break;
			case err::t_Free:
			case err::t_Return:
				Agent().Alert( "???" );
				break;
			default:
				Agent().Alert( err::Message( Buffer ) );
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

			Agent().AddEventHandler( EventName, *this );
		}
		callback &Callback( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		agent &Agent( void ) const
		{
			return Callback().Agent();
		}
	};

	class callback_core___
	{
	protected:
		virtual void SCLXHTMLStart( void ) = 0;
		virtual xhtagent::agent_core___ &_A( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			// Standadisation.
		}
		E_CVDTOR( callback_core___ )
		void Init( frdkrn::kernel___ &Kernel )
		{
			// Standardisation.
		}
		void Start( void );
		xhtagent::agent_core___ &AgentCore( void )
		{
			return _A();
		}
	};

	// L'utilisateur met dans la classe mère ses propres objets et l'instancie par un 'new', et il est assuré qu'un 'delete' sera fait une fois la bibliothèque déchargée.
	template <typename agent, typename kernel> class callback___
	: public callback_core___,
	  public agent
	{
	protected:
		virtual xhtagent::agent_core___ &_A( void ) override
		{
			return *this;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			callback_core___::reset( P );
			agent::reset( P );
		}
		E_CVDTOR( callback___ )
		void Init(
			xhtcllbk::token__ Token,
			kernel &Kernel,
			xhtcllbk::upstream_callback__ &UCallback )
		{
			callback_core___::Init( Kernel );
			agent::Init( Token, UCallback, Kernel );
		}
		void Start( void );
		agent &Agent( void )
		{
			return *this;
		}
	};

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String,
		bso::char__ Marker = '#' )
	{
		sclmisc::LoadXMLAndTranslateTags( FileName, Registry, String, Marker );
	}

	void MainSubmission(
		const sclrgstry::registry_ &Registry,
		xhtagent::agent_core___ &Agent );

	callback_core___ *SCLXHTMLRetrieveCallback(
		xhtcllbk::token__ Token,
		xhtcllbk::upstream_callback__ &UCallback );	// A surcharger.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
