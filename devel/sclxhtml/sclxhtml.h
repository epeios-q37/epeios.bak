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

	const char *GetLauncher( void );

	namespace {
		class _event_callback__
		{
		protected:
			virtual void SCLXHTMLHandle( const char *Id ) = 0;
		public:
			void reset( bso::bool__ = true )
			{
				// Standardisation.
			}
			E_CVDTOR( _event_callback__ );
			void Init( void )
			{
				// Standardisation.
			}
			void Handle( const char *Id )
			{
				SCLXHTMLHandle( Id );
			}
		};

		E_ROW( crow__ );	// (event) handler row;

		typedef bch::E_BUNCHt_( _event_callback__ *, crow__ ) _event_callbacks_;

		class event_handler_
		{
		private:
			_event_callback__ *_Get( const str::string_ &Name ) const
			{
				crow__ Row = stsfsm::GetId( Name, Automat );

				if ( Row == E_NIL )
					return NULL;

				return Callbacks( Row );
			}
		public:
			struct s {
				stsfsm::automat_::s Automat;
				_event_callbacks_::s Callbacks;
			};
			stsfsm::automat_ Automat;
			_event_callbacks_ Callbacks;
			event_handler_( s &S )
			: Automat( S.Automat ),
			  Callbacks( S.Callbacks )
			{}
			void reset( bso::bool__ P = true )
			{
				Automat.reset( P );
				Callbacks.reset( P );
			}
			void plug( ags::E_ASTORAGE_ &AS )
			{
				Automat.plug( AS );
				Callbacks.plug( AS );
			}
			event_handler_ &operator =(const event_handler_ &EM)
			{
				Automat = EM.Automat;
				Callbacks = EM.Callbacks;

				return *this;
			}
			void Init( void )
			{
				Automat.Init();
				Callbacks.Init();
			}
			bso::bool__ Add(
				const char *Name,
				_event_callback__ &Callback )
			{
				return stsfsm::Add( Name, *Callbacks.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
			}
			void Handle(
				const char *Id,
				const char *Event )
			{
				_event_callback__ *Callback = _Get( str::string(  Event ) );

				if ( Callback == NULL )
					ERRFwk();

				Callback->Handle( Id );
			}
		};

		E_AUTO( event_handler );
	}

	template <typename session> class event_callback__
	: public _event_callback__
	{
	private:
		session *_Session;
	protected:
# if 0
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
# endif
	public:
		void reset( bso::bool__ P = true )
		{
			_event_callback__::reset( P );
			_Session = NULL;
		}
		E_CVDTOR( event_callback__ );
		void Init(
			const char *EventName,
			session &Session )
		{
			_event_callback__::Init();
			_Session = &Session;

			this->Session().AddEventCallback( EventName, *this );
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

# if 0
	namespace {
		typedef frdkrn::reporting_callback__ _reporting_callback__;
	}

	class reporting_callback__
	: public _reporting_callback__
	{
	private:
		xhtagent::agent_core___ *_Agent;
		xhtagent::agent_core___ &_A( void ) const
		{
			if ( _Agent == NULL )
				ERRFwk();

			return *_Agent;
		}
	protected:
		virtual void FRDKRNReport( const str::string_ &Message ) override
		{
			_A().Alert( Message );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_reporting_callback__::reset( P );
			_Agent = NULL;
		}
		E_CVDTOR( reporting_callback__ );
		void Init(
			frdkrn::kernel___ &Kernel,
			xhtagent::agent_core___ &Agent )
		{
			_Agent = &Agent;
			_reporting_callback__::Init( Kernel );
		}
	};

	frdkrn::kernel___ *SCLXHTMLNewKernel( void );	// A surcharger.

	session_core___ *SCLXHTMLNewSession(
		xhtcllbk::token__ Token,
		xhtcllbk::upstream_callback__ &Callback );	// A surcharger.

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

# endif


	namespace {
		typedef xhtcllbk::session_callback__ _session_callback__;
	}

	class session_callback___
	: public _session_callback__
	{
	private:
		event_handler _Handler;
	protected:
		virtual void XHTCLLBKHandle(
			const char *Id,
			const char *Event ) override
		{
			_Handler.Handle( Id, Event );
		}
		virtual xhtagent::agent_core___ &_A( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_session_callback__::reset( P );
			_Handler.reset( P );
		}
		E_CVDTOR( session_callback___ );
		void Init( void )
		{
			_Handler.Init();
			_session_callback__::Init();
		}
		void ResetEventHandler( void )
		{
			_Handler.Init();
		}
		void AddEventCallback(
			const char *Event,
			_event_callback__ &Callback )
		{
			_Handler.Add( Event, Callback );
		}
		xhtagent::agent_core___ &AgentCore( void )
		{
			return _A();
		}
	};

	namespace {
		typedef frdssn::session___ _session___;

		typedef frdkrn::reporting_callback__ _reporting_callback__;

		template <typename agent> class reporting_callback__
		: public _reporting_callback__
		{
		private:
			agent *_Agent;
			agent &_A( void )
			{
				if ( _Agent == NULL )
					ERRFwk();

				return *_Agent;
			}
		protected:
			virtual void FRDKRNReport( const str::string_ &Message ) override
			{
				_A().Alert( Message );
			}
		public:
			void reset( bso::bool__ P = true )
			{
				_reporting_callback__::reset( P );
				_Agent = NULL;
			}
			E_CVDTOR( reporting_callback__ );
			void Init(
				frdkrn::kernel___ &Kernel,
				agent &Agent )
			{
				_reporting_callback__::Init( Kernel );
				_Agent = &Agent;
			}
		};
	}

	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assur� qu'un 'delete' sera fait une fois la biblioth�que d�charg�e.
	template <typename agent, typename instances, typename kernel, typename frame, frame UndefinedFrame > class session___
	: public session_callback___,
	  public agent,
	  public instances,
	  public _session___
	{
	private:
		kernel _Kernel;
		frame _Frame;	// Current frame;
		reporting_callback__<agent> _ReportingCallback;
	protected:
		virtual void FRDSSNOpen( const char *Language ) override
		{
			instances::Init( _Kernel );
		}
		virtual void FRDSSNClose( void ) override
		{
			instances::reset();
		}
		virtual void SCLXHTMLRefresh( frame Frame  ) = 0;
		virtual xhtagent::agent_core___ &_A( void ) override
		{
			return *this;
		}
		void SwitchTo( frame Frame )
		{
			ResetEventHandler();
			_Frame = Frame;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			session_callback___::reset( P );
			agent::reset( P );
			instances::reset( P );
			_session___::reset( P );
			_Kernel.reset( P );
			_Frame= UndefinedFrame;
			_ReportingCallback.reset( P );
		}
		E_CVDTOR( session___ )
		void Init(
			const char *Launcher,
			xhtcllbk::upstream_callback__ &Callback )
		{
			_ReportingCallback.Init( _Kernel, *this );
			_Kernel.Init( sclxhtml::GetRegistry(), scllocale::GetLocale(), sclmisc::GetLanguage(), _ReportingCallback, Launcher );
			session_callback___::Init();
			agent::Init( Callback );
//			instances::Init( _Kernel );	// Lanc� lors de l'ouverture de la session (vord 'FRDSSNOpen(...)').
			_session___::Init( _Kernel );
			_Frame = UndefinedFrame;
		}
		void Refresh( void )
		{
			if ( _Frame == UndefinedFrame )
				ERRFwk();
			SCLXHTMLRefresh( _Frame );
		}
		kernel &Kernel( void )
		{
			return _Kernel;
		}
	};

	void SCLXHTMLOnLoad( void );	// A surcharger. Lanc� lorsque la biblioth�que est charg�e.

	session_callback___ *SCLXHTMLNew( xhtcllbk::upstream_callback__ &Callback );

	void SCLXHTMLOnUnload( void );	// A surcharger. Lanc� lorsque la biblioth�que est d�charg�e.

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
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
