/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef SCLXDHTML__INC
# define SCLXDHTML__INC

# define SCLXDHTML_NAME		"SCLXDHTML"

# if defined( E_DEBUG ) && !defined( SCLXDHTML_NODBG )
#  define SCLXDHTML_DBG
# endif

// SoCLe X(SL)/DH(TML)

# include "xdhdws.h"

# include "sclfrntnd.h"

# include "sclrgstry.h"
# include "sclmisc.h"

# include "err.h"

# define SCLXDHTML_DEFAULT_SUFFIX "xdh"

namespace sclxdhtml {
	const sclrgstry::registry_ &GetRegistry( void );

	const char *GetLauncher( void );

	template <typename session> class cAction
	{
	protected:
		virtual void SCLXLaunch(
			session &Session,
			const char *Id ) = 0;
	public:
		qCALLBACK( Action );
		void Launch(
			session &Session,
			const char *Id )
		{
			return SCLXLaunch( Session, Id );
		}
	};

	E_ROW( crow__ );	// callback row;

	template <typename session> E_TTCLONE_( bch::E_BUNCHt_( cAction<session> *, crow__ ), action_callbacks_ );

	template <typename session> class action_handler_
	{
	private:
		cAction<session> *_Get( const str::string_ &Action ) const
		{
			crow__ Row = stsfsm::GetId( Action, Automat );

			if ( Row == qNIL )
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
		void plug( qASd *AS )
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
			cAction<session> &Callback )
		{
			return stsfsm::Add( Name, *Callbacks.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
		}
		void Launch(
			session &Session,
			const char *Id,
			const char *Action )
		{
			cAction<session> *Callback = _Get( str::string(  Action ) );

			if ( Callback == NULL )
				qRFwk();	// L'action affecte  un vnement n'existe pas. Contrler le fichier '.xsl'.

			return Callback->Launch( Session, Id );
		}
	};

	E_AUTO1( action_handler );

	template <typename session> class cActionHelper
	{
	protected:
		virtual bso::bool__ SCLXOnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action ) = 0;
		virtual void SCLXOnRefresh( session &Session ) = 0;
		virtual bso::bool__ SCLXOnClose( session &Session ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( cActionHelper );
		void Init( void )
		{
			// Standardisation.
		}
		bso::bool__ OnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action )
		{
			return SCLXOnBeforeAction( Session, Id, Action );
		}
		void OnRefresh( session &Session )
		{
			return SCLXOnRefresh( Session );
		}
		bso::bool__ OnClose( session &Session )
		{
			return SCLXOnClose( Session );
		}
	};

	class rActionHelper
	{
	private:
		stsfsm::wAutomat Automat_;
	public:
		void reset( bso::sBool P = true )
		{
			Automat_.reset( P );
		}
		qCDTOR( rActionHelper );
		void Init( void )
		{
			Automat_.Init();
		}
		void Add( const char *Action )
		{
			if ( stsfsm::Add( Action, 0, Automat_ ) != stsfsm::UndefinedId )
				qRGnr();
		}
		template <typename action> void Add( const action &Action )
		{
			if ( stsfsm::Add( action::Name, 0, Automat_ ) != stsfsm::UndefinedId )
				qRGnr();
		}
		template <typename action, typename ... args> void Add(
			const action &First,
			const args &... Others )
		{
			Add( First );
			Add( Others... );
		}
		bso::sBool Search( const char *Action )
		{
			return stsfsm::GetId( str::string( Action ), Automat_ ) == 0;
		}
	};

	typedef fblfrd::reporting_callback__ _reporting_callback__;

	typedef xdhcmn::session_callback__ _session_callback__;

	using xdhdws::proxy__;

	void Alert(
		const ntvstr::string___ &XML,
		const ntvstr::string___ &XSL,
		const ntvstr::string___ &Title,
		proxy__ &Proxy,
		const char *Language );

	void Alert(
		const ntvstr::string___ &RawMessage,
		const char *Language,
		proxy__ &Proxy );	// Translates 'Message'.

	void Alert(
		const ntvstr::string___ &Message,
		proxy__ &Proxy,
		const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.

	bso::bool__ Confirm(
		const ntvstr::string___ &XML,
		const ntvstr::string___ &XSL,
		const ntvstr::string___ &Title,
		proxy__ &Proxy,
		const char *Language );

	bso::bool__ Confirm(
		const ntvstr::string___ &RawMessage,
		const char *Language,
		proxy__ &Proxy );

	bso::bool__ Confirm(
		const ntvstr::string___ &Message,
		proxy__ &Proxy,
		const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.


	class reporting_callback__
	: public _reporting_callback__
	{
	private:
		Q37_MRMDF( proxy__, P_, Proxy_ );
		Q37_MPMDF( const char, L_, Language_ );
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message ) override
		{
			if ( Reply == fblovl::rDisconnected )
				Alert("SCLXHTML_Disconnected", L_(), P_() );
			else {
//				sclmisc::ReportAndAbort( Message );
				Alert( Message, P_(), L_() );
				qRAbort();
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_reporting_callback__::reset( P );
			Proxy_ = NULL;
		}
		E_CVDTOR( reporting_callback__ );
		void Init(
			proxy__ &Proxy,
			const char *Language )
		{
			_reporting_callback__::Init();
			Proxy_ = &Proxy;
			Language_ = Language;
		}
	};

	void HandleError(
		proxy__ &Proxy,
		const char *Language );

	// To indicate if the backend dedicated part in the login page should or not be visible.
	qENUM( BackendVisibility ) {
		bvHide,	
		bvShow,
		bv_amount,
		bv_Undefined
	};


	// User put in 'instances' all his own objects, instanciating all with a 'new' (by overloading 'SCLXHTMLNew(...)'), a 'delete' will be made automatically when unloading thie library.
	template <typename instances, typename frontend, typename page, page UndefinedPage > class rSession
	: public _session_callback__,
	  public proxy__,
	  public instances,
	  public frontend
	{
	private:
		page Page_;	// Current page;
		reporting_callback__ _ReportingCallback;
		eBackendVisibility BackendVisibility_;
	public:
		void reset( bso::bool__ P = true )
		{
			_session_callback__::reset( P );
			instances::reset( P );
			frontend::reset( P );
			Page_ = UndefinedPage;
			_ReportingCallback.reset( P );
			BackendVisibility_ = bv_Undefined;
		}
		qCVDTOR( rSession )
		void Init(
			sclfrntnd::rKernel &Kernel,
			const char *Language,
			xdhcmn::proxy_callback__ *Callback )
		{
			proxy__::Init( Callback );
			_ReportingCallback.Init( *this, Language );
			frontend::Init( Kernel, Language, _ReportingCallback );
			_session_callback__::Init();
			Page_ = UndefinedPage;
			// instances::Init( *this );	// Made on connection.
			BackendVisibility_ = bvShow;	// By default, the backend part of the login page is shown.
		}
		bso::bool__ Connect(
			const fblfrd::compatibility_informations__ &CompatibilityInformations,
			fblfrd::incompatibility_informations_ &IncompatibilityInformations )
		{
			if ( !frontend::Connect( CompatibilityInformations, IncompatibilityInformations ) )
				return false;

			// if ( frontend::IsConnected() )	// It's to each subcomponent to decide what to do when not connected.
			instances::Init( *this );
			
			return true;
		}
		void Disconnect( void )
		{
			instances::reset();

			frontend::Disconnect();
		}
		const char *Language( void )
		{
			return frontend::Language();
		}
		void SwitchTo( page Page = UndefinedPage )
		{
			if ( Page != UndefinedPage )
				Page_ = Page;
			else
				qRFwk();
		}
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Translation )
		{
			return scllocale::GetTranslation( Message, Language(), Translation );
		}
		void AlertU( const ntvstr::string___ &Message )	// Displays 'Message' as is.
		{
			sclxdhtml::Alert ( Message, *this, Language() );
		}
		void AlertT( const ntvstr::string___ &RawMessage )	// Translates 'RawMessage'.
		{
			sclxdhtml::Alert ( RawMessage, Language(), *this );
		}
		void Alert(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title )
		{
			sclxdhtml::Alert( XML, XSL, Title, *this, Language() );
		}
		template <typename i> void Alert( i I )
		{
			bso::bInt Buffer;

			AlertU( bso::Convert( I, Buffer ) ); 
		}
		bso::bool__ ConfirmU( const ntvstr::string___ &Message )	// Displays 'Message' as is.
		{
			return sclxdhtml::Confirm( Message, *this, Language() );
		}
		bso::bool__ ConfirmT( const ntvstr::string___ &RawMessage )	// Translates 'RawMessage'.
		{
			return sclxdhtml::Confirm( RawMessage, Language(), *this );
		}
		bso::bool__ Confirm(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title )
		{
			return sclxdhtml::Confirm( XML, XSL, Title, this, Language() );
		}
		qRWDISCLOSEr( eBackendVisibility, BackendVisibility );
		qRODISCLOSEr( page, Page );
	};

	template <typename session> class core___
	{
	private:
		action_handler<session> _Handler;
		xdhcmn::mode__ _Mode;
		Q37_MRMDF( cActionHelper<session>, _AH, _ActionHelperCallback );
		bso::bool__ _OnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action )
		{
			return _AH().OnBeforeAction( Session, Id, Action );
		}
		void _OnRefresh( session &Session )
		{
			return _AH().OnRefresh( Session );
		}
		bso::bool__ _OnClose( session &Session )
		{
			return _AH().OnClose( Session );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Handler.reset( P );
			_Mode = xdhcmn::m_Undefined;
			_ActionHelperCallback = NULL;
		}
		E_CVDTOR( core___ )
		void Init(
			xdhcmn::mode__ Mode,
			cActionHelper<session> &ActionHelperCallback )
		{
			_ActionHelperCallback = &ActionHelperCallback;
			_Mode = Mode;
			_Handler.Init();
		}
		void AddActionCallback(
			const char *ActionName,
			cAction<session> &Callback )
		{
			_Handler.Add( ActionName, Callback );
		}
		bso::bool__ Launch(
			session &Session,
			const char *Id,
			const char *Action )
		{
			bso::bool__ Success = true;
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			if ( !strcmp( Action, xdhcmn::RefreshActionLabel) ) {
				_OnRefresh( Session );
			} else if ( _OnBeforeAction( Session, Id, Action) ) {
				if ( !strcmp( Action, xdhcmn::CloseActionLabel ) )
					Success = _OnClose( Session );	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
				else
# if 0
					Success = _Handler.Launch( Session, Id, Action );
# else
					_Handler.Launch( Session, Id, Action );
# endif
			}
		qRR
			HandleError( Session, Session.Language() );
		qRT
		qRE
			return Success;
		}
		E_RODISCLOSE__( xdhcmn::mode__, Mode );
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

	/*
	void LoadProject( proxy__ &Proxy );

	void LaunchProject(
		proxy__ &Proxy ,
		sclfrntnd::kernel___ &Kernel );
		*/

	void SCLXDHTMLInitialization( xdhcmn::mode__ Mode );	// To define by user.

	xdhcmn::session_callback__ *SCLXDHTMLRetrieveCallback(
		const char *Language,
		xdhcmn::proxy_callback__ *ProxyCallback );	// To define by user.

	void SCLXDHTMLReleaseCallback( xdhcmn::session_callback__ *Callback );	// To define by user.

	namespace prolog {
		E_CDEF( char *, ProjectTypeId, "ProjectType" );
		E_CDEF( char *, PredefinedProjectId, "PredefinedProject" );
		E_CDEF( char *, RemoteProjectId, "RemoteProject" );

		void GetContent(
			sclfrntnd::rFrontend &Frontend,
			xml::writer_ &Writer );

		void GetContext(
			proxy__ &Proxy,
			xml::writer_ &Writer );

		void DisplaySelectedProjectFilename(
			proxy__ &Proxy,
			const char *Id );

		sclmisc::project_type__ GetProjectFeatures(
			proxy__ &Proxy,
			str::string_ &Feature );

		void LoadProject( proxy__ &Proxy );
	}

	namespace login {
		E_CDEF( char *, BackendTypeId, "BackendType" );
		E_CDEF( char *, StraightBackendId, "StraightBackend" );
		E_CDEF( char *, EmbeddedBackendId, "EmbeddedBackend" );
		E_CDEF( char *, PredefinedBackendId, "PredefinedBackend" );

		const char *GetLabel( eBackendVisibility );

		sclfrntnd::eLogin GetContent(
			sclfrntnd::rFrontend &Frontend,
			xml::writer_ &Writer );

		void GetContext(
			proxy__ &Proxy,
			eBackendVisibility Visibility,
			xml::writer_ &Writer );

		void GetBackendFeatures(
			proxy__ &Proxy,
			sclfrntnd::rFeatures &Features );

		void DisplaySelectedEmbeddedBackendFilename(
			proxy__ &Proxy,
			const char *Id );
	}

}

#endif
