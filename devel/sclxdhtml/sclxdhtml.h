/*
Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef SCLXDHTML_INC_
# define SCLXDHTML_INC_

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

# define SCLXDHTML_DEFAULT_SUFFIX XDHDWS_DEFAULT_SUFFIX

namespace sclxdhtml {

	qCDEF( char, DefaultMarker, '#' );

	namespace registry {
		using rgstry::rEntry;

		namespace parameter {
			using namespace sclrgstry::parameter;
		}

		namespace definition {
			using namespace sclrgstry::definition;

			extern rEntry XMLFilesHandling;
			extern rEntry XSLFile;	// To style XML data tagged).
			extern rEntry HeadFile;	// For the head section of the HTML main page. One page only.
		}
	}

	const sclrgstry::registry_ &GetRegistry( void );

	const char *GetLauncher( void );

	using xdhdws::fHead;

    using xdhdws::SetHeadFunction;

	template <typename session> class cAction {
	protected:
		virtual void SCLXLaunch(
			session &Session,
			const char *Id,
			xdhcmn::eMode Mode ) = 0;
	public:
		qCALLBACK( Action );
		void Launch(
			session &Session,
			const char *Id,
			xdhcmn::eMode Mode )
		{
			return SCLXLaunch( Session, Id, Mode );
		}
	};

	// How the XSL file are handled.
	qENUM( XSLFileHandling ) {
		xfhName,		// The name of the XSL file is sent (Atlas toolkit behavior).
		xfhContent,		// The content of the XSL file is sent.
		xfhRegistry,	// One of above depending of the content of the registry.
		xfh_amount,
		xfh_Undefined,
		xfh_Default = xfhRegistry,
	};

# define SCLX_ACD( session, name )\
	extern class s##name\
	: public sclxdhtml::cAction<session>\
	{\
	protected:\
		virtual void SCLXLaunch(\
			session &Session,\
			const char *Id,\
			xdhcmn::eMode Mode ) override;\
	public:\
		static const char *Name;\
	} name

# define SCLX_AC( session, owner, name )\
	owner::s##name owner::name;\
	const char *owner::s##name::Name = #name;\
	void owner::s##name::SCLXLaunch(\
		session &Session,\
		const char *Id,\
		xdhcmn::eMode Mode )

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
			const char *Action,
			xdhcmn::eMode Mode )
		{
			cAction<session> *Callback = _Get( str::string( Action ) );

			if ( Callback == NULL )
				qRFwk();	// An event has no associated action. Check the'.xsl' file.

			return Callback->Launch( Session, Id, Mode );
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

	typedef fblfrd::reporting_callback__ sReporting_;

	typedef xdhcmn::cSession cSession_;

	class sProxy
	{
	private:
		xdhdws::sProxy Core_;
		qRMV( const scli::sInfo, I_, Info_ );
		eXSLFileHandling XSLFileHandling_;
		void Alert_(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title,
			const char *Language )
			{
                qRLmt();
                // Reactivae in '.cpp'.
			}
		void Alert_(
			const ntvstr::string___ &Message,
			const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
			const char *CloseTextLanguage );
		bso::bool__ Confirm_(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title,
			const char *Language )
			{
                qRLmt();
                // Reactivate in '.cpp'.
                return false;
			}
		bso::sBool Confirm_(
			const ntvstr::string___ &Message,
			const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
			const char *CloseTextLanguage );
	protected:
		void PrependLayout_(
			const xdhdws::nstring___ &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclrgstry::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker);
		void SetLayout_(
			const xdhdws::nstring___ &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclrgstry::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker);
		void AppendLayout_(
			const xdhdws::nstring___ &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclrgstry::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker);
		template <typename session, typename rack> void SetLayout_(
			const xdhdws::nstring___ &Id,
			const char *Target,
			const sclrgstry::registry_ &Registry,
			void( *Get )(session &Session, xml::rWriter &Writer),
			session &Session,
			bso::char__ Marker = DefaultMarker )
		{
		qRH;
			rack Rack;
		qRB;
			Rack.Init( Target, Session, I_() );

			Get( Session, Rack() );

			SetLayout_( Id, registry::definition::XSLFile, Target, Registry, Rack.Target(), Marker );
		qRR;
		qRT;
		qRE;
		}
		void Fill_(
            str::dStrings &Values,
            const str::dString &Value )
        {
            Values.Append(Value);
        }
        template <class... Args> void Fill_(
            str::dStrings &Values,
            const str::dString &Value,
            const Args &...args )
        {
            Fill_(Values, Value);
            Fill_(Values, args...);
        }
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, Info_ );
			XSLFileHandling_ = xfh_Undefined;
		}
		qCDTOR( sProxy );
		void Init(
            xdhcmn::cUpstream *Upstream,
			const scli::sInfo &Info,
			eXSLFileHandling XSLFileHandling )
		{
			Core_.Init( Upstream );
			Info_ = &Info;
			XSLFileHandling_ = XSLFileHandling;
		}
		const scli::sInfo &Info( void ) const
		{
			return I_();
		}
		template <class ...Args> void Process(
            const char *ScriptName,
            str::dString &Result,
            const Args &...args )
        {
        qRH
            str::wStrings Values;
        qRB
            Values.Init();
            Fill_(Values, args...);
            Core_.Process(ScriptName, Values, Result);
        qRR
        qRE
        qRT
        }
		void Execute(
			const str::dString &Script,
			str::dString &Result )
		{
            Process("Execute_1", Result, Script);
		}
		void Execute( const str::dString &Script )
		{
		qRH;
			str::wString Dummy;
		qRB;
            Dummy.Init();
			Execute( Script, Dummy );
		qRR;
		qRT;
		qRE;
		}
		void Log( const ntvstr::rString &Message )
		{
            qRLmt();
		}
		// The basic alert, without use of 'JQuery' based widget.
		void AlertB( const ntvstr::string___ & Message );
		void Alert(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title,
			const char *Language );
		void AlertT(
			const ntvstr::string___ &RawMessage,
			const char *Language );	// Translates 'Message'.
		void AlertU(
			const ntvstr::string___ &Message,
			const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.
		bso::bool__ Confirm(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title,
			const char *Language )
		{
			return Confirm_( XML, XSL, Title, Language );
		}
		bso::bool__ ConfirmT(
			const ntvstr::string___ &RawMessage,
			const char *Language );
		bso::bool__ ConfirmU(
			const ntvstr::string___ &Message,
			const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.
		void SetAttribute(
			const xdhcmn::rNString &Id,
			const xdhcmn::rNString &Name,
			const xdhcmn::rNString &Value )
		{
            qRLmt();
		}
		const char *GetAttribute(
			const xdhcmn::rNString &Id,
			const xdhcmn::rNString &Name,
			qCBUFFERr &Value )
		{
			qRLmt();
			return Value();
		}
		const str::dString &GetAttribute(
			const xdhcmn::rNString &Id,
			const xdhcmn::rNString &Name,
			str::dString &Value )
		{
            qRLmt();
            return Value;
		}
		void RemoveAttribute(
			const xdhcmn::rNString &Id,
			const xdhcmn::rNString &Name )
		{
            qRLmt();
		}
		void SetValue(
			const ntvstr::rString &Id,
			const ntvstr::rString &Value )
		{
            qRLmt();
		}
		const str::dString &GetValue(
			const ntvstr::rString &Id,
			str::dString &Value )
		{
            qRLmt();
            return Value;
		}
		const char *GetValue(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value;
		}
		const str::dString &GetResult(
			const ntvstr::rString &Id,
			str::dString &Result )
		{
            qRLmt();
            return Result;
		}
		const char *GetResult(
			const ntvstr::rString &Id,
			qCBUFFERr &Result )
		{
            qRLmt();
            return Result;
		}
		void SetContents(
			const str::dStrings &Ids,
			const str::dStrings &Contents )
        {
            qRLmt();
            // Reactivate in '.cpp'.
        }
		void SetContent(
			const str::dString &Id,
			const str::dString &Content );
		void SetTimeout(
			const ntvstr::rString &Delay,
			const ntvstr::rString &Action );
		const char *Parent(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value;
		}
		const char *FirstChild(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value;
		}
		const char *LastChild(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value();
		}
		const char *PreviousSibling(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value;
		}
		const char *NextSibling(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value;
		}
		void InsertChild(
			const ntvstr::rString &Child,
			const ntvstr::rString &Id )
		{
            qRLmt();
		}
		void AppendChild(
			const ntvstr::rString &Child,
			const ntvstr::rString &Id )
		{
            qRLmt();
		}
		void InsertBefore(
			const ntvstr::rString &Sibling,
			const ntvstr::rString &Id )
		{
            qRLmt();
		}
		void InsertAfter(
			const ntvstr::rString &Sibling,
			const ntvstr::rString &Id )
		{
            qRLmt();
		}
		/*
		void InsertCSSRule(
			const str::dString &Rule,
			xdhcmn::sIndex Index );
		xdhcmn::sIndex AppendCSSRule( const str::dString &Rule );
		void RemoveCSSRule( xdhcmn::sIndex Index );
		*/
		void AddClasses(
			const str::dStrings &Ids,
			const str::dStrings &Classes );
		void AddClass(
			const str::dString &Id,
			const str::dString &Class );
		void AddClass(
			const char *Id,
			const char *Class )
		{
			AddClass( str::wString( Id ), str::wString( Class ) );
		}
		void RemoveClasses(
			const str::dStrings &Ids,
			const str::dStrings &Classes );
		void RemoveClass(
			const str::dString &Id,
			const str::dString &Class );
		void RemoveClass(
			const char *Id,
			const char *Class )
		{
			RemoveClass( str::wString( Id ), str::wString( Class ) );
		}
		void ToggleClasses(
			const str::dStrings &Idss,
			const str::dStrings &Classes );
		void ToggleClass(
			const str::dString &Id,
			const str::dString &Class );
		void ToggleClass(
			const char *Id,
			const char *Class )
		{
			ToggleClass( str::wString( Id ), str::wString( Class ) );
		}
		void EnableElements( const str::dStrings &Ids );
		void EnableElement( const str::dString &Id );
		void EnableElement( const char *Id )
		{
			EnableElement( str::wString( Id ) );
		}
		void DisableElements( const str::dStrings &Ids );
		void DisableElement( const str::dString &Id);
		void DisableElement( const char *Id )
		{
			DisableElement( str::wString( Id ) );
		}
		void DressWidgets( const ntvstr::rString &Id )
		{
            qRLmt();
		}
		const char *Dummy(
			const ntvstr::rString &Id,
			qCBUFFERr &Value )
		{
            qRLmt();
            return Value;
		}
	};

	class sReporting
	: public sReporting_ {
	private:
		Q37_MRMDF( sProxy, P_, Proxy_ );
		Q37_MPMDF( const char, L_, Language_ );
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message ) override
		{
			if ( Reply == fblovl::rDisconnected )
				P_().AlertT( "SCLXHTML_Disconnected", L_() );
			else {
				//				sclmisc::ReportAndAbort( Message );
				P_().AlertU( Message, L_() );
				qRAbort();
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			sReporting_::reset( P );
			Proxy_ = NULL;
		}
		E_CVDTOR( sReporting );
		void Init(
			sProxy &Proxy,
			const char *Language )
		{
			sReporting_::Init();
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

	template <typename session, typename dump> class rRack
	{
	private:
		str::wString Target_;
		mutable flx::E_STRING_TOFLOW___ _Flow;
		xml::rWriter _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Target_, _Flow, _Writer );
		}
		E_CDTOR( rRack );
		void Init(
			const char *View,
			session &Session,
			const scli::sInfo &Info )
		{
			tol::buffer__ Buffer;
			xml::sMark Mark = xml::Undefined;

			Target_.Init();
			_Flow.Init( Target_ );
			_Writer.Init( _Flow, xml::oIndent, xml::e_Default );
			_Writer.PushTag( "XDHTML" );
			_Writer.PutAttribute( "View", View );
			_Writer.PutAttribute( "Generator", Info.Target() );
			_Writer.PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );
			_Writer.PutAttribute( "OS", cpe::GetOSDigest() );
			Mark = _Writer.PushTag( "Corpus" );
			dump::Corpus( Session, _Writer );
			_Writer.PopTag( Mark );
			_Writer.PushTag( "Layout" );
			dump::Common( Session, _Writer );
		}
		operator xml::rWriter &()
		{
			return _Writer;
		}
		xml::rWriter &operator()( void )
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

	template <typename session> class rCore;

	// User put in 'instances' all his own objects, instantiating all with a 'new' (by overloading 'SCLXHTMLNew(...)'), a 'delete' will be made automatically when unloading the library.
	template <typename instances, typename frontend, typename page, page UndefinedPage, typename dump> class rSession
	: public cSession_,
	  public sProxy,
	  public instances,
	  public frontend
	{
	private:
		eXSLFileHandling XSLFileHandling_;
		qCRMV( scli::sInfo, I_, Info_ );
		qRMV( sclfrntnd::rKernel, K_, Kernel_ );
		page Page_;	// Current page;
		sReporting Reporting_;
		eBackendVisibility BackendVisibility_;
		qRMV( class rCore<rSession>, C_, Core_ );
	protected:
		bso::sBool XDHCMNInitialize(
			xdhcmn::cUpstream *Callback,
			const char *Language,
			const str::dString &Token )	override // If empty, PROD session, else token used for the DEMO session.
        {
            if ( Token.Amount() )
                qRFwk();    // Should never be maunched in 'DEMO' mode.

			sProxy::Init( Callback, I_(), XSLFileHandling_ );
			Reporting_.Init( *this, Language );
			frontend::Init( K_(), Language, Reporting_ );

			return true;
        }
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
			XSLFileHandling_ = xfh_Undefined;
			tol::reset(P, Info_, Kernel_);
			Page_ = UndefinedPage;
			Reporting_.reset( P );
			BackendVisibility_ = bv_Undefined;
			sProxy::reset();
			Core_ = NULL;
		}
		qCVDTOR( rSession )
		void Init(
			sclfrntnd::rKernel &Kernel,
			class rCore<rSession> &Core,
			const scli::sInfo &Info,
			eXSLFileHandling XSLFileHandling = xfh_Default )
		{
            this->XSLFileHandling_ = XSLFileHandling;
            Info_ = &Info;
            Kernel_ = &Kernel;
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
		bso::sBool GetHead( str::dString &Content )
		{
			return GetHead_( frontend::Registry(), Content, DefaultMarker );
		}
		void AlertU( const ntvstr::string___ &Message )	// Displays 'Message' as is.
		{
			sProxy::AlertU( Message, Language() );
		}
		void AlertT( const ntvstr::string___ &RawMessage )	// Translates 'RawMessage'.
		{
			sProxy::AlertT( RawMessage, Language() );
		}
		void Alert(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title )
		{
			sProxy::Alert( XML, XSL, Title, Language() );
		}
		template <typename i> void Alert( i I )
		{
			bso::bInt Buffer;

			AlertU( bso::Convert( I, Buffer ) );
		}
		bso::bool__ ConfirmU( const ntvstr::string___ &Message )	// Displays 'Message' as is.
		{
			return sProxy::ConfirmU( Message, Language() );
		}
		bso::bool__ ConfirmT( const ntvstr::string___ &RawMessage )	// Translates 'RawMessage'.
		{
			return sProxy::ConfirmT( RawMessage, Language() );
		}
		bso::bool__ Confirm(
			const ntvstr::string___ &XML,
			const ntvstr::string___ &XSL,
			const ntvstr::string___ &Title )
		{
			return sProxy::Confirm( XML, XSL, Title, Language() );
		}
		qRWDISCLOSEr( eBackendVisibility, BackendVisibility );
		qRODISCLOSEr( page, Page );
		void SetElementLayout(
			const xdhdws::nstring___ &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ),
			const sclrgstry::dRegistry &Registry )
		{
			sProxy::SetLayout_<rSession, rRack<rSession,dump>>( Id, Target, Registry, Get, *this );
		}
		void SetElementLayout(
			const xdhdws::nstring___ &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ) )
		{
			SetElementLayout( Id, Target, Get, frontend::Registry() );
		}
		inline void SetDocumentLayout(
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ),
			const sclrgstry::dRegistry &Registry )
		{
//			sProxy::HeadUp_( frontend::Registry(), DefaultMarker );
			SetElementLayout( "", Target, Get, Registry );
		}
		void SetDocumentLayout(
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ) )
		{
			SetDocumentLayout( Target, Get, frontend::Registry() );
		}
		void SetContents(
			const str::dStrings &Ids,
			const str::dStrings &Contents )
		{
			sProxy::SetContents( Ids, Contents );
		}
		void SetContent(
			const str::dString &Id,
			const str::dString &Content )
		{
			sProxy::SetContent( Id, Content );
		}
		void SetContent(
			const char *Id,
			const str::dString &Content )
		{
			SetContent( str::wString( Id ), Content );
		}
	};

	template <typename session> class rCore {
	private:
		action_handler<session> _Handler;
		xdhcmn::eMode Mode_;
		Q37_MRMDF( cActionHelper<session>, _AH, _ActionHelperCallback );
		qPMV( const char, ONS_, OnNewSession_ );	// Name of the action to call on new session.
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
			Mode_ = xdhcmn::m_Undefined;
			_ActionHelperCallback = NULL;
			OnNewSession_ = NULL;
		}
		E_CVDTOR( rCore )
		template <typename action> void Init(
			xdhcmn::eMode Mode,
			const action &OnNewSession,
			cActionHelper<session> &ActionHelperCallback )
		{
			OnNewSession_ = OnNewSession.Name;
			_ActionHelperCallback = &ActionHelperCallback;
			Mode_ = Mode;
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
		qRH;
			TOL_CBUFFER___ Buffer;
		qRB;
			if ( ( Action == NULL ) || ( *Action == 0 ) ) {
				Session.SetAttribute( "", "data-xdh-onevents", "(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)" );
				Action = ONS_();
			}

			 if ( !strcmp( Action, xdhcmn::RefreshActionLabel ) ) {
				_OnRefresh( Session );
			} else if ( _OnBeforeAction( Session, Id, Action ) ) {
				if ( !strcmp( Action, xdhcmn::CloseActionLabel ) )
					Success = _OnClose( Session );	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
				else
					_Handler.Launch( Session, Id, Action, Mode_ );
			}
		qRR;
			HandleError( Session, Session.Language() );
		qRT;
		qRE;
			return Success;
		}
	};


	/*********************************/

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &Content,
		bso::char__ Marker = DefaultMarker )
	{
		return sclmisc::LoadXMLAndTranslateTags( FileName, Registry, Content, 0, Marker );
	}

	inline void SetContents_(
		xdhdws::sProxy &Proxy,
		const xdhcmn::nstring___ &Ids,
		const xdhcmn::nstring___ &Contents )
	{
        qRLmt();
//		Proxy.SetContents( Ids, Contents );
	}

	const scli::sInfo &SCLXDHTMLInfo( void );	// To define by user.
	void SCLXDHTMLInitialization( xdhcmn::eMode Mode );	// To define by user.

	xdhcmn::cSession *SCLXDHTMLRetrieveCallback(	// To define by user.
		const char *Language,
		xdhcmn::eMode Mode,
		const str::dString &Token,	// If not empty, DEMO mode with connexion identified by 'Token',
									// otherwise PROD mode, with host/service retrieved from registry.
		xdhcmn::cUpstream *UpstreamCallback );

	void SCLXDHTMLReleaseCallback( xdhcmn::cSession *Callback );	// To define by user.

	namespace prolog {
		static E_CDEF( char *, BorderId, "Border" );
		static E_CDEF( char *, ProjectTypeId, "ProjectType" );
		static E_CDEF( char *, PredefinedProjectFormId, "PredefinedProjectForm" );
		static E_CDEF( char *, PredefinedProjectId, "PredefinedProject" );
		static E_CDEF( char *, RemoteProjectFormId, "RemoteProjectForm" );
		static E_CDEF( char *, RemoteProjectId, "RemoteProject" );

		void GetLayout(
			sclfrntnd::rFrontend &Frontend,
			xml::rWriter &Writer );

		void HandleProjectTypeSwitching( sProxy &Proxy );

		void DisplaySelectedProjectFilename(
			sProxy &Proxy,
			const char *Id );

		sclmisc::eProjectType GetProjectFeatures(
			sProxy &Proxy,
			str::string_ &Feature );

		void LoadProject( sProxy &Proxy );
	}

	namespace login {
		static E_CDEF( char *, BackendTypeId, "BackendType" );
		// Ids of the forms for the parameters of the different backend types.
		static E_CDEF( char *, PredefinedBackendId, "PredefinedBackend" );
		static E_CDEF( char *, RemoteBackendId, "RemoteBackend" );
		static E_CDEF( char *, ProxyfiedBackendId, "ProxyfiedBackend" );
		static E_CDEF( char *, EmbeddedBackendId, "EmbeddedBackend" );

		const char *GetLabel( eBackendVisibility );

		sclfrntnd::eLogin GetLayout(
			sclfrntnd::rFrontend &Frontend,
			xml::rWriter &Writer );

		void HandleBackendTypeSwitching( sProxy &Proxy );

		void GetBackendFeatures(
			sProxy &Proxy,
			sclfrntnd::rFeatures &Features );

		void DisplaySelectedEmbeddedBackendFilename(
			sProxy &Proxy,
			const char *Id );
	}

}

#endif
