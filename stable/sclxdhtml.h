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

# include "crt.h"
# include "err.h"

# define SCLXDHTML_DEFAULT_SUFFIX "xdh"

namespace sclxdhtml {
	class dCast {
	public:
		str::dString
			Tag,
			Value;
		struct s {
			str::dString::s
				Tag,
				Value;
		} &S_;
		dCast( s &S )
		: S_( S ),
		  Tag( S.Tag ),
		  Value( S.Value )
		{}
		void reset( bool P = true )
		{
			tol::reset( P, Tag, Value );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Tag, Value );
		}
		dCast &operator =( const dCast &C )
		{
			Tag = C.Tag;
			Value = C.Value;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Tag, Value );
		}
		void Init(
			const str::dString &Tag,
			const str::dString &Value )
		{
			this->Tag.Init( Tag );
			this->Value.Init( Value );
		}
		void Init(
			const char *Tag,
			const char *Value )
		{
			Init( str::wString( Tag ), str::wString( Value ) );
		}
	};

	qW( Cast );

	typedef crt::qCRATEdl( dCast ) dCasts;
	qW( Casts );

	namespace registry {
		using rgstry::rEntry;

		namespace parameter {
			using namespace sclrgstry::parameter;
		}

		namespace definition {
			using namespace sclrgstry::definition;

			extern rEntry XSLLayoutFile;
			extern rEntry XSLCastingFile;
		}
	}

	const sclrgstry::registry_ &GetRegistry( void );

	const char *GetLauncher( void );

	template <typename session> class cAction {
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

	template <typename session> class action_handler_ {
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
		action_handler_ &operator =( const action_handler_ &AH )
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
			cAction<session> *Callback = _Get( str::string( Action ) );

			if ( Callback == NULL )
				qRFwk();	// An event has no associated action. Check the'.xsl' file.

			return Callback->Launch( Session, Id );
		}
	};

	E_AUTO1( action_handler );

	template <typename session> class cActionHelper {
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
			// Standardization.
		}
		E_CVDTOR( cActionHelper );
		void Init( void )
		{
			// Standardization.
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

	class rActionHelper {
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

	typedef xdhcmn::cSession cSession_;

	using xdhdws::sProxy;

	void Alert(
		const ntvstr::string___ &XML,
		const ntvstr::string___ &XSL,
		const ntvstr::string___ &Title,
		sProxy &Proxy,
		const char *Language );

	void Alert(
		const ntvstr::string___ &RawMessage,
		const char *Language,
		sProxy &Proxy );	// Translates 'Message'.

	void Alert(
		const ntvstr::string___ &Message,
		sProxy &Proxy,
		const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.

	bso::bool__ Confirm(
		const ntvstr::string___ &XML,
		const ntvstr::string___ &XSL,
		const ntvstr::string___ &Title,
		sProxy &Proxy,
		const char *Language );

	bso::bool__ Confirm(
		const ntvstr::string___ &RawMessage,
		const char *Language,
		sProxy &Proxy );

	bso::bool__ Confirm(
		const ntvstr::string___ &Message,
		sProxy &Proxy,
		const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.


	class reporting_callback__
	: public _reporting_callback__ {
	private:
		Q37_MRMDF( sProxy, P_, Proxy_ );
		Q37_MPMDF( const char, L_, Language_ );
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message ) override
		{
			if ( Reply == fblovl::rDisconnected )
				Alert( "SCLXHTML_Disconnected", L_(), P_() );
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
			sProxy &Proxy,
			const char *Language )
		{
			_reporting_callback__::Init();
			Proxy_ = &Proxy;
			Language_ = Language;
		}
	};

	void HandleError(
		sProxy &Proxy,
		const char *Language );

	// To indicate if the backend dedicated part in the login page should or not be visible.
	qENUM( BackendVisibility )
	{
		bvHide,
		bvShow,
		bv_amount,
		bv_Undefined
	};

	extern const char *RootTagId_;

	typedef void( *fSet )( xdhdws::sProxy &Proxy, const xdhdws::nstring___ &Id, const xdhdws::nstring___ &XML, const xdhdws::nstring___ &XSL );

	void SetElement_(
		const xdhdws::nstring___ &Id,
		fSet Set,
		const rgstry::rEntry &Filename,
		const char *Target,
		const sclrgstry::registry_ &Registry,
		const str::dString &XML,
		xdhdws::sProxy &Proxy,
		bso::char__ Marker );

	void SetContents_(
		const str::dStrings &Ids,
		const str::dStrings &Contents,
		xdhdws::sProxy &Proxy );

	void SetContent_(
		const str::dString &Id,
		const str::dString &Content,
		xdhdws::sProxy &Proxy );

	void SetCastsByIds_(
		const str::dStrings &Ids,
		const str::dStrings &Values,
		xdhdws::sProxy &Proxy );

	void SetCastsByTags_(
		const xdhdws::nstring___ &Id,
		const dCasts &Casts,
		xdhdws::sProxy &Proxy );

	void SetCastById_(
		const xdhdws::nstring___ &Id,
		const xdhdws::nstring___ &Value,
		xdhdws::sProxy &Proxy );

	void SetCastByTag_(
		const xdhdws::nstring___ &Id,
		const dCast &Cast,
		xdhdws::sProxy &Proxy );

	void SetCastByTag_(
		const xdhdws::nstring___ &Id,
		const str::dString &Tag,
		const str::dString &Value,
		xdhdws::sProxy &Proxy );

	inline void SetCastByTag_(
		const xdhdws::nstring___ &Id,
		const char *Tag,
		const char *Value,
		xdhdws::sProxy &Proxy )
	{
		return SetCastByTag_( Id, str::wString( Tag ), str::wString( Value ), Proxy );
	}

	template <typename session, typename rack> inline void SetElement_(
		const xdhdws::nstring___ &Id,
		const char *Target,
		const rgstry::rEntry &Filename,
		void( *Get )( session &Session, xml::dWriter &Writer ),
		fSet Set,
		const sclrgstry::registry_ &Registry,
		session &Session,
		bso::char__ Marker = '#' )
	{
	qRH;
		rack Rack;
	qRB;
		Rack.Init( Target, Session );

		Get( Session, Rack() );

		SetElement_( Id, Set, Filename, Target, Registry, Rack.Target(), Session, Marker );
	qRR;
	qRT;
	qRE;
	}

	template <typename session, typename dump> class rRack_
	{
	private:
		str::wString Target_;
		mutable flx::E_STRING_TOFLOW___ _Flow;
		xml::writer _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Target_, _Flow, _Writer );
		}
		E_CDTOR( rRack_ );
		void Init(
			const char *View,
			const char *Background,
			session &Session )
		{
			tol::buffer__ Buffer;
			xml::mark__ Mark = XML_UNDEFINED_MARK;

			Target_.Init();
			_Flow.Init( Target_ );
			_Writer.Init( _Flow, xml::oIndent, xml::e_Default );
			_Writer.PushTag( "XDHTML" );
			_Writer.PutAttribute( "View", View );
			_Writer.PutAttribute( "Background", Background );
			_Writer.PutAttribute( "Generator", sclmisc::SCLMISCTargetName );
			_Writer.PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );
			_Writer.PutAttribute( "OS", cpe::GetOSDigest() );
			Mark = _Writer.PushTag( "Corpus" );
			dump::Corpus( Session, _Writer );
			_Writer.PopTag( Mark );
			_Writer.PushTag( Background );
			dump::Common( Session, _Writer );
		}
		operator xml::writer_ &( )
		{
			return _Writer;
		}
		xml::writer_ &operator()( void )
		{
			return _Writer;
		}
		const str::dString &Target( void )
		{
			_Writer.reset();	// To close all tags.
			_Flow.Commit();

			return Target_;
		}
	};

	template <typename session, typename dump> class rLayoutRack
	: public rRack_<session, dump> {
	public:
		void Init(
			const char *View,
			session &Session )
		{
			rRack_<session, dump>::Init( View, "Layout", Session );
		}
	};

	inline void SetLayout_(
		xdhdws::sProxy &Proxy,
		const xdhcmn::nstring___ &Id,
		const xdhcmn::nstring___ &XML,
		const xdhcmn::nstring___ &XSL )
	{
		Proxy.SetLayout( Id, XML, XSL );
	}

	template <typename session, typename dump> inline void SetElementLayout(
		const xdhdws::nstring___ &Id,
		const char *Target,
		void( *Get )( session &Session, xml::dWriter &Writer ),
		const sclrgstry::registry_ &Registry,
		session &Session,
		bso::char__ Marker = '#' )
	{
		SetElement_<session, rLayoutRack<session, dump>>( Id, Target, registry::definition::XSLLayoutFile, Get, SetLayout_, Registry, Session, Marker );
	}

	template <typename session, typename dump> inline void SetDocumentLayout(
		const char *Target,
		void( *Get )( session &Session, xml::dWriter &Writer ),
		const sclrgstry::registry_ &Registry,
		session &Session,
		bso::char__ Marker = '#' )
	{
		SetElementLayout<session, dump>( RootTagId_, Target, Get, Registry, Session, Marker );
	}

	template <typename session> class rCore;

	// User put in 'instances' all his own objects, instantiating all with a 'new' (by overloading 'SCLXHTMLNew(...)'), a 'delete' will be made automatically when unloading the library.
	template <typename instances, typename frontend, typename page, page UndefinedPage, typename dump > class rSession
	: public cSession_,
	  public sProxy,
	  public instances,
	  public frontend
	{
	private:
		page Page_;	// Current page;
		reporting_callback__ _ReportingCallback;
		eBackendVisibility BackendVisibility_;
		qRMV( class rCore<rSession>, C_, Core_ );
	protected:
		bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override
		{
			return C_().Launch( *this, Id, Action );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			instances::reset( P );
			frontend::reset( P );
			Page_ = UndefinedPage;
			_ReportingCallback.reset( P );
			BackendVisibility_ = bv_Undefined;
			sProxy::reset();
			Core_ = NULL;
		}
		qCVDTOR( rSession )
			void Init(
				sclfrntnd::rKernel &Kernel,
				const char *Language,
				xdhcmn::cProxy *Callback,
				class rCore<rSession> &Core )
		{
			sProxy::Init( Callback );
			_ReportingCallback.Init( *this, Language );
			frontend::Init( Kernel, Language, _ReportingCallback );
			Page_ = UndefinedPage;
			// instances::Init( *this );	// Made on connection.
			BackendVisibility_ = bvShow;	// By default, the backend part of the login page is shown.
			Core_ = &Core;
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
			sclxdhtml::Alert( Message, *this, Language() );
		}
		void AlertT( const ntvstr::string___ &RawMessage )	// Translates 'RawMessage'.
		{
			sclxdhtml::Alert( RawMessage, Language(), *this );
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
		void SetElementLayout(
			const xdhdws::nstring___ &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::dWriter &Writer ),
			const sclrgstry::dRegistry &Registry )
		{
			sclxdhtml::SetElementLayout<rSession, dump>( Id, Target, Get, Registry, *this );
		}
		void SetElementLayout(
			const xdhdws::nstring___ &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::dWriter &Writer ) )
		{
			SetElementLayout( Id, Target, Get, frontend::Registry() );
		}
		inline void SetDocumentLayout(
			const char *Target,
			void( *Get )( rSession &Session, xml::dWriter &Writer ),
			const sclrgstry::dRegistry &Registry )
		{
			sclxdhtml::SetDocumentLayout<rSession, dump>( Target, Get, Registry, *this );
		}
		void SetDocumentLayout(
			const char *Target,
			void( *Get )( rSession &Session, xml::dWriter &Writer ) )
		{
			SetDocumentLayout( Target, Get, frontend::Registry() );
		}
		void SetContents(
			const str::dStrings &Ids,
			const str::dStrings &Contents )
		{
			sclxdhtml::SetContents_( Ids, Contents, *this );
		}
		void SetContent(
			const str::dString &Id,
			const str::dString &Content )
		{
			sclxdhtml::SetContent_( Id, Content, *this );
		}
		void SetContent(
			const char *Id,
			const str::dString &Content )
		{
			sclxdhtml::SetContent_( str::wString( Id ), Content, *this );
		}
		void SetElementCasts(
			const xdhdws::nstring___ &Id,
			const dCasts &Casts )
		{
			sclxdhtml::SetCasts_( Id, Casts, *this );
		}
		void SetElementCast(
			const xdhdws::nstring___ &Id,
			const dCast &Cast )
		{
			sclxdhtml::SetCasts_( Id, Cast, *this );
		}
		void SetElementCast(
			const xdhdws::nstring___ &Id,
			const str::dString &Tag,
			const str::dString &Value )
		{
			sclxdhtml::SetCasts_( Id, Tag, Value, *this );
		}
		void SetElementCast(
			const xdhdws::nstring___ &Id,
			const char *Tag,
			const char *Value )
		{
			sclxdhtml::SetCasts_( Id, Tag, Value, *this );
		}
		void SetDocumentCasts( const dCasts &Casts )
		{
			sclxdhtml::SetCasts_( RootTagId_, Casts, *this );
		}
		void SetDocumentCast( const dCast &Cast )
		{
			sclxdhtml::SetCasts_( RootTagId_, Cast, *this );
		}
		void SetDocumentCast(
			const str::dString &Tag,
			const str::dString &Value )
		{
			sclxdhtml::SetCast_( RootTagId_, Tag, Value, *this );
		}
		void SetDocumentCast(
			const char *Tag,
			const char *Value )
		{
			sclxdhtml::SetCast_( RootTagId_, Tag, Value, *this );
		}
	};

	template <typename session> class rCore {
	private:
		action_handler<session> _Handler;
		xdhcmn::eMode _Mode;
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
		E_CVDTOR( rCore )
			void Init(
				xdhcmn::eMode Mode,
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
				if ( !strcmp( Action, xdhcmn::RefreshActionLabel ) ) {
					_OnRefresh( Session );
				} else if ( _OnBeforeAction( Session, Id, Action ) ) {
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
		E_RODISCLOSE__( xdhcmn::eMode, Mode );
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

	inline void SetContents_(
		xdhdws::sProxy &Proxy,
		const xdhcmn::nstring___ &Ids,
		const xdhcmn::nstring___ &Contents )
	{
		Proxy.SetContents( Ids, Contents );
	}

	void SCLXDHTMLInitialization( xdhcmn::eMode Mode );	// To define by user.

	xdhcmn::cSession *SCLXDHTMLRetrieveCallback(
		const char *Language,
		xdhcmn::eMode Mode,
		xdhcmn::cProxy *ProxyCallback );	// To define by user.

	void SCLXDHTMLReleaseCallback( xdhcmn::cSession *Callback );	// To define by user.

	namespace prolog {
		static E_CDEF( char *, ProjectTypeId, "ProjectType" );
		static E_CDEF( char *, PredefinedProjectId, "PredefinedProject" );
		static E_CDEF( char *, RemoteProjectId, "RemoteProject" );

		void GetLayout(
			sclfrntnd::rFrontend &Frontend,
			xml::writer_ &Writer );

		void GetCasts(
			sProxy &Proxy,
			dCasts &Casts );

		void DisplaySelectedProjectFilename(
			sProxy &Proxy,
			const char *Id );

		sclmisc::project_type__ GetProjectFeatures(
			sProxy &Proxy,
			str::string_ &Feature );

		void LoadProject( sProxy &Proxy );
	}

	namespace login {
		static E_CDEF( char *, BackendTypeId, "BackendType" );
		static E_CDEF( char *, StraightBackendId, "StraightBackend" );
		static E_CDEF( char *, EmbeddedBackendId, "EmbeddedBackend" );
		static E_CDEF( char *, PredefinedBackendId, "PredefinedBackend" );

		const char *GetLabel( eBackendVisibility );

		sclfrntnd::eLogin GetLayout(
			sclfrntnd::rFrontend &Frontend,
			xml::writer_ &Writer );

		void GetCasts(
			sProxy &Proxy,
			eBackendVisibility Visibility,
			dCasts &Casts );

		void GetBackendFeatures(
			sProxy &Proxy,
			sclfrntnd::rFeatures &Features );

		void DisplaySelectedEmbeddedBackendFilename(
			sProxy &Proxy,
			const char *Id );
	}

}

#endif
