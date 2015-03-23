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

	class core_action_callback__
	{
	private:
		const char *_Name;
	protected:
		virtual bso::bool__ SCLXHTMLLaunch( const char *Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Name = NULL;
		}
		E_CVDTOR( core_action_callback__ );
		void Init( const char *Name )
		{
			_Name = Name;
		}
		bso::bool__ Launch( const char *Id )
		{
			return SCLXHTMLLaunch( Id );
		}
		const char *Name( void ) const
		{
			if ( _Name == NULL )
				ERRFwk();

			return _Name;
		}
	};

	namespace {
		class _action_helper_callback__
		{
		protected:
			virtual bso::bool__ SCLXHTMLOnBeforeAction(
				const char *Id,
				const char *Action ) = 0;
			virtual bso::bool__ SCLXHTMLOnClose( void ) = 0;
		public:
			void reset( bso::bool__ = true )
			{
				// Standardisation.
			}
			E_CVDTOR( _action_helper_callback__ );
			void Init( void )
			{
				// Standardisation.
			}
			bso::bool__ OnBeforeAction(
				const char *Id,
				const char *Action )
			{
				return SCLXHTMLOnBeforeAction( Id, Action );
			}
			bso::bool__ OnClose( void )
			{
				return SCLXHTMLOnClose();
			}
		};

		E_ROW( crow__ );	// callback row;

		typedef bch::E_BUNCHt_( core_action_callback__ *, crow__ ) core_action_callbacks_;

		class action_handler_
		{
		private:
			core_action_callback__ *_Get( const str::string_ &Action ) const
			{
				crow__ Row = stsfsm::GetId( Action, Automat );

				if ( Row == E_NIL )
					return NULL;

				return Callbacks( Row );
			}
		public:
			struct s {
				stsfsm::automat_::s Automat;
				core_action_callbacks_::s Callbacks;
			};
			stsfsm::automat_ Automat;
			core_action_callbacks_ Callbacks;
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
				core_action_callback__ &Callback )
			{
				return stsfsm::Add( Name, *Callbacks.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
			}
			bso::bool__ Launch(
				const char *Id,
				const char *Action )
			{
				core_action_callback__ *Callback = _Get( str::string(  Action ) );

				if ( Callback == NULL )
					ERRFwk();	// L'action affectée à un évènement n'existe pas. Contrôler le fichier '.xsl'.

				return Callback->Launch( Id );
			}
		};

		E_AUTO( action_handler );
	}

	template <typename session> class action_callback__
	: public core_action_callback__
	{
	private:
		session *_Session;
	public:
		void reset( bso::bool__ P = true )
		{
			core_action_callback__::reset( P );
			_Session = NULL;
		}
		E_CVDTOR( action_callback__ );
		void Init(
			const char *ActionName,
			session &Session )
		{
			core_action_callback__::Init( ActionName );
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

	template <typename session> class action_helper_callback__
	: public _action_helper_callback__
	{
	private:
		session *_Session;
	public:
		void reset( bso::bool__ P = true )
		{
			_action_helper_callback__::reset( P );
			_Session = NULL;
		}
		E_CVDTOR( action_helper_callback__ );
		void Init( session &Session )
		{
			_action_helper_callback__::Init();
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
		typedef frdssn::session___ _session___;

		typedef frdkrn::reporting_callback__ _reporting_callback__;

		typedef xhtcllbk::session_callback__ _session_callback__;

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
			virtual void FBLFRDReport(
				fblovl::reply__ Reply,
				const char *Message ) override
//			virtual void FRDKRNReport( const str::string_ &Message ) override
			{
				_A().RawAlert( Message );
			}
		public:
			void reset( bso::bool__ P = true )
			{
				_reporting_callback__::reset( P );
				_Agent = NULL;
			}
			E_CVDTOR( reporting_callback__ );
			void Init(
//				const char *Language,
				_agent___ &Agent )
			{
//				_reporting_callback__::Init( scllocale::GetLocale(), Language );
				_reporting_callback__::Init();
				_Agent = &Agent;
			}
		};
	}

	void HandleError(
		xhtagent::agent___ &Agent,
		const char *Language );


	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assuré qu'un 'delete' sera fait une fois la bibliothèque déchargée.
	template <typename instances, typename kernel, typename page, page UndefinedPage > class session___
	: public _session_callback__,
	  public _agent___,
	  public instances,
	  public _session___
	{
	private:
		kernel _Kernel;
		action_handler _Handler;
		_action_helper_callback__ *_Callback;
		page _Page;	// Current page;
		const char *_Launcher;
		reporting_callback__ _ReportingCallback;
		_action_helper_callback__ &_C( void )
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		bso::bool__ _OnBeforeAction(
			const char *Id,
			const char *Action )
		{
			return _C().OnBeforeAction( Id, Action );
		}
		bso::bool__ _OnClose( void )
		{
			return _C().OnClose();
		}
	protected:
		virtual void FRDSSNOpen( const char *Language ) override
		{
//			_ReportingCallback.Init( Language, *this );
			_ReportingCallback.Init( *this );
			instances::Init( _Kernel );
			_session___::Registry().SetValue(sclrgstry::Language, str::string( Language ) );
		}
		virtual void FRDSSNClose( void ) override
		{
			instances::reset();
			_Kernel.Close();
		}
		virtual const char *FRDSSNLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			return sclrgstry::GetLanguage_( _session___::Registry(), Buffer );
		}
		virtual void SCLXHTMLRefresh( page Page  ) = 0;
		virtual bso::bool__ XHTCLLBKLaunch(
			const char *Id,
			const char *Action ) override	// Retourne 'true' si l'action a été correctement traitée (et que la propagation de l'évènement à l'orgine de cette action doit être arrêtée).
		{
			bso::bool__ Success = false;
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			if ( _OnBeforeAction( Id, Action ) )
				if ( !strcmp( Action, xhtcllbk::CloseActionLabel ) )
					Success = _OnClose();	// Dans ce cas, si 'Success' est à 'false', la fermeture de l'application est suspendue.
				else
					Success = _Handler.Launch( Id, Action );
		ERRErr
			HandleError( *this, Language( Buffer ) );
		ERREnd
		ERREpilog
			return Success;
		}
		virtual const char *XHTCLLBKLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			return _session___::Language( Buffer );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_session_callback__::reset( P );
			_agent___::reset( P );
			instances::reset( P );
			_session___::reset( P );
			_Kernel.reset( P );
			_Handler.reset( P );
			_Callback = NULL;
			_Page = UndefinedPage;
			_Launcher = NULL;
			_ReportingCallback.reset( P );
		}
		E_CVDTOR( session___ )
		void Init(
			const char *Launcher,
			xhtcllbk::proxy_callback__ &Callback,
			_action_helper_callback__ &ActionHelperCallback )
		{
			_agent___::Init( Callback );
			_Kernel.Init( _ReportingCallback );
			_Handler.Init();
			_session___::Init( _Kernel, sclmisc::GetRegistry() );
			_session_callback__::Init();
			_Callback = &ActionHelperCallback;
			_Page = UndefinedPage;
			_Launcher = Launcher;

			// Le reste est initialisé lors de l'ouverure de session (voir 'FRDSSNOpen()'.
		}
		void AddActionCallback(
			const char *ActionName,
			core_action_callback__ &Callback )
		{
			_Handler.Add( ActionName, Callback );
		}
		const char *Language( TOL_CBUFFER___ &Buffer )
		{
			return _session___::Language( Buffer );	// Pour résoudre l'ambiguité.
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
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Translation )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			scllocale::GetTranslation( Message, Language( Buffer ), Translation );
		ERRErr
		ERREnd
		ERREpilog
			return Translation;
		}
		bso::bool__ Confirm(
			const str::string_ &XML,
			const str::string_ &XSL,
			const str::string_ &Title = str::string() )
		{
			return _agent___::Confirm( XML, XSL, Title );
		}
		bso::bool__ Confirm( const char *Message )
		{
			bso::bool__ OK = false;
		ERRProlog
			str::string Translation;
		ERRBegin
			Translation.Init();

			OK = RawConfirm( GetTranslation( Message, Translation ) );
		ERRErr
		ERREnd
		ERREpilog
			return OK;
		}
		void Alert(
			const str::string_ &XML,
			const str::string_ &XSL,
			const str::string_ &Title = str::string() )
		{
			return _agent___::Alert( XML, XSL, Title );
		}
		void Alert( const char *Message )
		{
		ERRProlog
			str::string Translation;
		ERRBegin
			Translation.Init();

			RawConfirm( GetTranslation( Message, Translation ) );
		ERRErr
		ERREnd
		ERREpilog
		}
	};

	xhtcllbk::session_callback__ *SCLXHTMLNew( xhtcllbk::proxy_callback__ &Callback );

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
