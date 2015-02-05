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
		class _action_callback__
		{
		protected:
			virtual bso::bool__ SCLXHTMLLaunch( const char *Id ) = 0;
		public:
			void reset( bso::bool__ = true )
			{
				// Standardisation.
			}
			E_CVDTOR( _action_callback__ );
			void Init( void )
			{
				// Standardisation.
			}
			bso::bool__ Launch( const char *Id )
			{
				return SCLXHTMLLaunch( Id );
			}
		};

		class _prelaunch_callback__
		{
		protected:
			virtual bso::bool__ SCLXHTMLPreLaunch(
				const char *Id,
				const char *Action ) = 0;
		public:
			void reset( bso::bool__ = true )
			{
				// Standardisation.
			}
			E_CVDTOR( _prelaunch_callback__ );
			void Init( void )
			{
				// Standardisation.
			}
			bso::bool__ PreLaunch(
				const char *Id,
				const char *Action )
			{
				return SCLXHTMLPreLaunch( Id, Action );
			}
		};

		E_ROW( crow__ );	// callback row;

		typedef bch::E_BUNCHt_( _action_callback__ *, crow__ ) _action_callbacks_;

		class action_handler_
		{
		private:
			_action_callback__ *_Get( const str::string_ &Action ) const
			{
				crow__ Row = stsfsm::GetId( Action, Automat );

				if ( Row == E_NIL )
					return NULL;

				return Callbacks( Row );
			}
		public:
			struct s {
				stsfsm::automat_::s Automat;
				_action_callbacks_::s Callbacks;
			};
			stsfsm::automat_ Automat;
			_action_callbacks_ Callbacks;
			action_handler_( s &S )
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
			action_handler_ &operator =(const action_handler_ &AH )
			{
				Automat = AH.Automat;
				Callbacks = AH.Callbacks;

				return *this;
			}
			void Init( void )
			{
				Automat.Init();
				Callbacks.Init();
			}
			bso::bool__ Add(
				const char *Name,
				_action_callback__ &Callback )
			{
				return stsfsm::Add( Name, *Callbacks.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
			}
			bso::bool__ Launch(
				const char *Id,
				const char *Action )
			{
				_action_callback__ *Callback = _Get( str::string(  Action ) );

				if ( Callback == NULL )
					ERRFwk();	// L'action affectée à un évènement n'existe pas. Contrôler le fichier '.xsl'.

				return Callback->Launch( Id );
			}
		};

		E_AUTO( action_handler );
	}

	template <typename session> class action_callback__
	: public _action_callback__
	{
	private:
		session *_Session;
	public:
		void reset( bso::bool__ P = true )
		{
			_action_callback__::reset( P );
			_Session = NULL;
		}
		E_CVDTOR( action_callback__ );
		void Init(
			const char *ActionName,
			session &Session )
		{
			_action_callback__::Init();
			_Session = &Session;

			this->Session().AddActionCallback( ActionName, *this );
		}
		session &Session( void ) const
		{
			if ( _Session == NULL )
				ERRFwk();

			return *_Session;
		}
	};

	template <typename session> class prelaunch_callback__
	: public _prelaunch_callback__
	{
	private:
		session *_Session;
	public:
		void reset( bso::bool__ P = true )
		{
			_prelaunch_callback__::reset( P );
			_Session = NULL;
		}
		E_CVDTOR( prelaunch_callback__ );
		void Init( session &Session )
		{
			_prelaunch_callback__::Init();
			_Session = &Session;
		}
		session &Session( void ) const
		{
			if ( _Session == NULL )
				ERRFwk();

			return *_Session;
		}
	};

	namespace {
		typedef xhtcllbk::session_callback__ _session_callback__;
	}

	class session_callback___
	: public _session_callback__
	{
	private:
		action_handler _Handler;
		const char *_Language;
		const char *_L( void ) const
		{
			if ( _Language == NULL )
				ERRFwk();

			return _Language;
		}
		_prelaunch_callback__ *_Callback;
		_prelaunch_callback__ &_C( void )
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		bso::bool__ _PreLaunch(
			const char *Id,
			const char *Action )
		{
			return _C().PreLaunch(Id, Action);
		}
	protected:
		virtual bso::bool__ XHTCLLBKLaunch(
			const char *Id,
			const char *Action ) override;	// Retourne 'true' si l'action a été correctement traitée (et que la propagation de l'évènement à l'orgine de cette action doit être arrêtée).
		virtual xhtagent::agent___ &_A( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_session_callback__::reset( P );
			_Handler.reset( P );
			_Callback = NULL;
			_Language = NULL;
		}
		E_CVDTOR( session_callback___ );
		void Init( _prelaunch_callback__ &Callback )
		{
			_Handler.Init();
			_session_callback__::Init();
			_Callback = &Callback;
			_Language = NULL;
		}
		void SetLanguage( const char *Language )
		{
			_Language = Language;
		}
		void AddActionCallback(
			const char *ActionName,
			_action_callback__ &Callback )
		{
			_Handler.Add( ActionName, Callback );
		}
		xhtagent::agent___ &AgentCore( void )
		{
			return _A();
		}
	};

	namespace {
		typedef frdssn::session___ _session___;

		typedef frdkrn::reporting_callback__ _reporting_callback__;

		typedef xhtagent::agent___ _agent___;

		class reporting_callback__
		: public _reporting_callback__
		{
		private:
			_agent___ *_Agent;
			_agent___ &_A( void )
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
				const char *Language,
				_agent___ &Agent )
			{
				_reporting_callback__::Init( scllocale::GetLocale(), Language );
				_Agent = &Agent;
			}
		};
	}

	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assuré qu'un 'delete' sera fait une fois la bibliothèque déchargée.
	template <typename instances, typename kernel, typename page, page UndefinedPage > class session___
	: public session_callback___,
	  public _agent___,
	  public instances,
	  public _session___
	{
	private:
		kernel _Kernel;
		page _Page;	// Current page;
		const char *_Launcher;
		reporting_callback__ _ReportingCallback;
	protected:
		virtual void FRDSSNOpen( const char *Language ) override
		{
			_ReportingCallback.Init( Language, *this );
			session_callback___::SetLanguage( Language );
			instances::Init( _Kernel );
			_session___::Registry().SetValue(sclrgstry::Language, str::string( Language ) );
		}
		virtual void FRDSSNClose( void ) override
		{
			instances::reset();
		}
		virtual const char *FRDSSNLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			if ( _session___::IsOpen() )
				return sclrgstry::GetLanguage_(_session___::Registry(), Buffer );
			else
				return _agent___::GetLanguage( Buffer );
		}
		virtual void SCLXHTMLRefresh( page Page  ) = 0;
		virtual xhtagent::agent___ &_A( void ) override
		{
			return *this;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			session_callback___::reset( P );
			_agent___::reset( P );
			instances::reset( P );
			_session___::reset( P );
			_Kernel.reset( P );
			_Page= UndefinedPage;
			_Launcher = NULL;
			_ReportingCallback.reset( P );
		}
		E_CVDTOR( session___ )
		void Init(
			const char *Launcher,
			xhtcllbk::upstream_callback__ &Callback,
			_prelaunch_callback__ &PreLaunchCallback )
		{
			_agent___::Init( Callback );
			_Kernel.Init( _ReportingCallback );
			_session___::Init( _Kernel );
			session_callback___::Init( PreLaunchCallback );
			_Page = UndefinedPage;
			_Launcher = Launcher;

			// Le reste est initialisé lors de l'ouverure de session (voir 'FRDSSNOpen()'.
		}
		void Refresh( void )
		{
			if ( _Page == UndefinedPage )
				ERRFwk();
			else
				SCLXHTMLRefresh( _Page );
		}
		void SwitchTo( page Page = UndefinedPage )
		{
			if ( Page != UndefinedPage )
				_Page = Page;
			else
				ERRFwk();
		}
		kernel &Kernel( void )
		{
			return _Kernel;
		}
		const char *Launcher( void ) const
		{
			if ( _Launcher == NULL )
				ERRFwk();

			return _Launcher;
		}
	};

	void SCLXHTMLOnLoad( void );	// A surcharger. Lancé lorsque la bibliothèque est chargée.

	session_callback___ *SCLXHTMLNew( xhtcllbk::upstream_callback__ &Callback );

	void SCLXHTMLOnUnload( void );	// A surcharger. Lancé lorsque la bibliothèque est déchargée.

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String,
		bso::char__ Marker = '#' )
	{
		sclmisc::LoadXMLAndTranslateTags( FileName, Registry, String, Marker );
	}

	void LoadProject( xhtagent::agent___ &Agent );

	void LaunchProject(
		const char *Language,
		frdkrn::kernel___ &Kernel,
		frdssn::session___ &Session,
		xhtagent::agent___ &Agent,
		const frdkrn::compatibility_informations__ &CompatibilityInformations );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
