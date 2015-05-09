/*
	'sclxdhtml.h' by Claude SIMON (http://zeusw.org/).

	'sclxdhtml' is part of the Epeios framework.

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

#ifndef SCLXDHTML__INC
# define SCLXDHTML__INC

# define SCLXDHTML_NAME		"SCLXDHTML"

# if defined( E_DEBUG ) && !defined( SCLXDHTML_NODBG )
#  define SCLXDHTML_DBG
# endif

// SoCLe X(SL)/DH(TML)

# include "xdhdws.h"

# include "frdssn.h"

# include "sclrgstry.h"
# include "sclmisc.h"

# include "err.h"

namespace sclxdhtml {
	const sclrgstry::registry_ &GetRegistry( void );

	const char *GetLauncher( void );

	template <typename session> class action_callback__
	{
	private:
		const char *_Name;
	protected:
		virtual bso::bool__ SCLXDHTMLLaunch(
			session &Session,
			const char *Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Name = NULL;
		}
		E_CVDTOR( action_callback__ );
		void Init( const char *Name )
		{
			_Name = Name;
		}
		bso::bool__ Launch(
			session &Session,
			const char *Id )
		{
			return SCLXDHTMLLaunch( Session, Id );
		}
		const char *Name( void ) const
		{
			if ( _Name == NULL )
				ERRFwk();

			return _Name;
		}
	};

	namespace {
		template <typename session> class _action_helper_callback__
		{
		protected:
			virtual bso::bool__ SCLXHTMLOnBeforeAction(
				session &Session,
				const char *Id,
				const char *Action ) = 0;
			virtual bso::bool__ SCLXHTMLOnClose( session &Session ) = 0;
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
				session &Session,
				const char *Id,
				const char *Action )
			{
				return SCLXHTMLOnBeforeAction( Session, Id, Action );
			}
			bso::bool__ OnClose( session &Session )
			{
				return SCLXHTMLOnClose( Session );
			}
		};

		E_ROW( crow__ );	// callback row;

		template <typename session> E_TTCLONE_( bch::E_BUNCHt_( action_callback__<session> *, crow__ ), action_callbacks_ );

		template <typename session> class action_handler_
		{
		private:
			action_callback__<session> *_Get( const str::string_ &Action ) const
			{
				crow__ Row = stsfsm::GetId( Action, Automat );

				if ( Row == E_NIL )
					return NULL;

				return Callbacks( Row );
			}
		public:
			struct s {
				stsfsm::automat_::s Automat;
				typename action_callbacks_<session>::s Callbacks;
			};
			stsfsm::automat_ Automat;
			action_callbacks_<session> Callbacks;
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
				action_callback__<session> &Callback )
			{
				return stsfsm::Add( Name, *Callbacks.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
			}
			bso::bool__ Launch(
				session &Session,
				const char *Id,
				const char *Action )
			{
				action_callback__<session> *Callback = _Get( str::string(  Action ) );

				if ( Callback == NULL )
					ERRFwk();	// L'action affecte  un vnement n'existe pas. Contrler le fichier '.xsl'.

				return Callback->Launch( Session, Id );
			}
		};

		E_AUTO1( action_handler );
	}

	template <typename session> class action_helper_callback__
	: public _action_helper_callback__<session>
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

		typedef xdhcbk::session_callback__ _session_callback__;

		using xdhdws::proxy__;

		class reporting_callback__
		: public _reporting_callback__
		{
		private:
			Q37_MRMDF( proxy__, _P, _Proxy );
		protected:
			virtual void FBLFRDReport(
				fblovl::reply__ Reply,
				const char *Message ) override
			{
				_P().Alert( Message );
			}
		public:
			void reset( bso::bool__ P = true )
			{
				_reporting_callback__::reset( P );
				_Proxy = NULL;
			}
			E_CVDTOR( reporting_callback__ );
			void Init( proxy__ &Proxy )
			{
				_reporting_callback__::Init();
				_Proxy = &Proxy;
			}
		};
	}

	void HandleError(
		proxy__ &Proxy,
		const char *Language );

	/*********** 1 *****************/

	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assur qu'un 'delete' sera fait une fois la bibliothque dcharge.
	template <typename core, typename instances, typename kernel, typename page, page UndefinedPage > class session___
	: public _session_callback__,
	  public proxy__,
	  public instances,
	  public _session___
	{
	private:
		Q37_MRMDF(core, _C, _Core );
		kernel _Kernel;
		page _Page;	// Current page;
		reporting_callback__ _ReportingCallback;
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
		virtual void SCLXDHTMLRefresh( page Page ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_session_callback__::reset( P );
			instances::reset( P );
			_session___::reset( P );
			_Kernel.reset( P );
			_Page = UndefinedPage;
			_ReportingCallback.reset( P );
		}
		E_CVDTOR( session___ )
		void Init( xdhcbk::proxy_callback__ *Callback )
		{
			proxy__::Init( Callback );
			_Kernel.Init( _ReportingCallback );
			_session___::Init( _Kernel, sclmisc::GetRegistry() );
			_session_callback__::Init();
			_Page = UndefinedPage;

			// Le reste est initialis lors de l'ouverure de session (voir 'FRDSSNOpen()'.
		}
		const char *Language( TOL_CBUFFER___ &Buffer )
		{
			return _session___::Language( Buffer );	// Pour rsoudre l'ambiguit.
		}
		void Refresh( void )
		{
			if ( _Page == UndefinedPage )
				ERRFwk();
			else
				SCLXDHTMLRefresh( _Page );
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
	};

	/********** 2 ******************/

	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assur qu'un 'delete' sera fait une fois la bibliothque dcharge.
	template <typename session> class core___
	{
	private:
		action_handler<session> _Handler;
		Q37_MRMDF( _action_helper_callback__<session>, _AH, _ActionHelperCallback );
		reporting_callback__ _ReportingCallback;
		bso::bool__ _OnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action )
		{
			return _AH().OnBeforeAction( Session, Id, Action );
		}
		bso::bool__ _OnClose( session &Session )
		{
			return _AH().OnClose( Session );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Handler.reset( P );
			_ActionHelperCallback = NULL;
			_ReportingCallback.reset( P );
		}
		E_CVDTOR( core___ )
		void Init(
			xdhcbk::proxy_callback__ &Callback,
			_action_helper_callback__<session> &ActionHelperCallback )
		{
			_Kernel.Init( _ReportingCallback );
			_ActionHelperCallback = &ActionHelperCallback;
			_Handler.Init();
		}
		void AddActionCallback(
			const char *ActionName,
			action_callback__<session> &Callback )
		{
			_Handler.Add( ActionName, Callback );
		}
		bso::bool__ Launch(
			session &Session,
			const char *Id,
			const char *Action )
		{
			bso::bool__ Success = false;
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			if ( _OnBeforeAction( Session, Id, Action ) )
				if ( !strcmp( Action, xdhcbk::CloseActionLabel ) )
					Success = _OnClose( Session );	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
				else
					Success = _Handler.Launch( Session, Id, Action );
		ERRErr
			HandleError( Session, Session.Language( Buffer ) );
		ERREnd
		ERREpilog
			return Success;
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
	};


	/*********************************/

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String,
		bso::char__ Marker = '#' )
	{
		sclmisc::LoadXMLAndTranslateTags( FileName, Registry, String, Marker );
	}

	void LoadProject( proxy__ &Proxy );

	void LaunchProject(
		const char *Language,
		frdkrn::kernel___ &Kernel,
		frdssn::session___ &Session,
		proxy__ &Proxy,
		const frdkrn::compatibility_informations__ &CompatibilityInformations );

	xdhcbk::session_callback__ *SCLXDHTMLNew( xdhcbk::upstream_callback__ &Callback );	// To override.

}

#endif
