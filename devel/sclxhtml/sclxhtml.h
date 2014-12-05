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

# include "sclfrntnd.h"

# include "err.h"
# include "flw.h"
# include "rgstry.h"
# include "scllocale.h"
# include "sclmisc.h"
# include "frdssn.h"

namespace sclxhtml {
	const sclrgstry::registry_ &GetRegistry( void );

	typedef xhtcllbk::event_handler__ _event_handler__;

	template <typename session> class event_handler__
	: public _event_handler__
	{
	private:
		session *_Session;
	protected:
		virtual void SCLXHTMLHandle( const char *Id ) = 0;
		virtual void XHTCLLBKHandle( const char *Id )
		{
		ERRProlog
			str::string Message;
			err::buffer__ Buffer;
		ERRBegin
			SCLXHTMLHandle( Id );
		ERRErr
			switch ( ERRType ) {
			case err::t_Abort:
				Message.Init();
				if ( sclerror::GetPendingError(sclmisc::GetLanguage(), Message) ) {
					Session().Alert( Message );
					sclerror::ResetPendingError();
				} else
					Session().Alert("?");
				break;
			case err::t_Free:
			case err::t_Return:
				Session().Alert( "???" );
				break;
			default:
				Session().Alert( err::Message( Buffer ) );
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
			_Session = NULL;
		}
		E_CVDTOR( event_handler__ );
		void Init(
			const char *EventName,
			session &Session )
		{
			_event_handler__::Init();
			_Session = &Session;

			this->Session().AddEventHandler( EventName, *this );
		}
		session &Session( void ) const
		{
			if ( _Session == NULL )
				ERRFwk();

			return *_Session;
		}
		/*
		agent &Agent( void ) const
		{
			return Callback().Agent();
		}
		*/
	};

	class session_core___
	{
	protected:
		virtual void SCLXHTMLStart( void ) = 0;
		virtual bso::bool__ SCLXHTMLOnClose( void ) = 0;
		virtual xhtagent::agent_core___ &_A( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardisation.
		}
		E_CVDTOR( session_core___ )
		void Init( void )
		{
			// Standardisation.
		}
		void Start( void )
		{
			SCLXHTMLStart();
		}
		bso::bool__ OnClose( void )
		{
			return SCLXHTMLOnClose();
		}
		xhtagent::agent_core___ &AgentCore( void )
		{
			return _A();
		}
	};

	// L'utilisateur met dans le type 'session' ses propres objets et instancie le tout par un 'new', et il est assuré qu'un 'delete' sera fait une fois la bibliothèque déchargée.
	template <typename agent, typename session, typename kernel, typename frame, frame UndefinedFrame > class session___
	: public session_core___,
	  public agent,
	  public session
	{
	private:
		frame _Frame;	// Current frame;
	protected:
		virtual void SCLXHTMLRefresh( frame Frame  ) = 0;
		virtual xhtagent::agent_core___ &_A( void ) override
		{
			return *this;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			session_core___::reset( P );
			agent::reset( P );
			_Frame= UndefinedFrame;
		}
		E_CVDTOR( session___ )
		void Init(
			xhtcllbk::token__ Token,
			kernel &Kernel,
			xhtcllbk::upstream_callback__ &UCallback )
		{
			session_core___::Init();
			agent::Init( Token, UCallback );
			session::Init( Kernel );
			_Frame = UndefinedFrame;
		}
		void SetEvents( frame Frame )
		{
			ResetEventManager();
			Handlers.Init( *this, Frame );
			_Frame = Frame;
		}
		void Refresh( void )
		{
			if ( _Frame == UndefinedFrame )
				ERRFwk();
			SCLXHTMLRefresh( _Frame );
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

	void MainSubmission( xhtagent::agent_core___ &Agent );

	void SessionFormSubmission(
		frdkrn::kernel___ &Kernel,
		frdssn::session___ &Session,
		xhtagent::agent_core___ &Agent,
		const frdkrn::compatibility_informations__ &CompatibilityInformations );

	void SCLXHTMLOnLoading( const char *LauncherIdentification );	// A surcharger. Lancé lorsque la bibliothèque est chargée.

	session_core___ *SCLXHTMLNewSession(
		xhtcllbk::token__ Token,
		xhtcllbk::upstream_callback__ &Callback );	// A surcharger.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
