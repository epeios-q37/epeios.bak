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

// SoCLe X(DHTML)

/*
NOTA: In this library, the token string is ALWAYS empty and the token row
is ALWAYS equal to 'qNIL'. Token related data are only useful in 'FaS' mode,
and mainly handled by the 'xdhqxdh' and 'faasq' utilities.
*/

#ifndef SCLX_INC_
# define SCLX_INC_

# define SCLX_NAME		"SCLX"

# if defined( E_DEBUG ) && !defined( SCLX_NODBG )
#  define SCLX_DBG
# endif

# include "sclf.h"
# include "sclr.h"

# include "err.h"
# include "fblfrd.h"
# include "rgstry.h"
# include "tol.h"
# include "xdhcdc.h"
# include "xdhcmn.h"
# include "xdhdws.h"

# define SCLX_DEFAULT_SUFFIX XDHDWS_DEFAULT_SUFFIX

namespace sclx {
	namespace faas {
		using namespace xdhdws::faas;
	}
	qCDEF( char, DefaultMarker, '#' );

	namespace registry {
		using rgstry::rEntry;

		namespace parameter {
			using namespace sclr::parameter;
		}

		namespace definition {
			using namespace sclr::definition;

			extern rEntry XMLFilesHandling;
			extern rEntry XSLFile;	// To style XML data tagged.
			extern rEntry HeadFile;	// For the head section of the HTML main page. One page only.
		}
	}

	const sclr::registry_ &GetRegistry( void );

	const char *GetLauncher( void );

	template <typename session> class cAction
	{
	protected:
		virtual void SCLXLaunch(
			session &Session,
			const char *Id,
			xdhcdc::eMode Mode ) = 0;
	public:
		qCALLBACK( Action );
		void Launch(
			session &Session,
			const char *Id,
			xdhcdc::eMode Mode )
		{
			return SCLXLaunch( Session, Id, Mode );
		}
	};

	// How the XSL file are handled.
	qENUM( XSLFileHandling )
	{
		xfhName,		// The name of the XSL file is sent (Atlas toolkit behavior).
		xfhContent,		// The content of the XSL file is sent.
		xfhRegistry,	// One of above depending of the content of the registry.
		xfh_amount,
		xfh_Undefined,
		xfh_Default = xfhRegistry,
	};

# define SCLX_ADec( session, name )\
	extern class s##name\
	: public sclx::cAction<session>\
	{\
	protected:\
		virtual void SCLXLaunch(\
			session &Session,\
			const char *Id,\
			xdhcdc::eMode Mode ) override;\
	public:\
		static const char *Name;\
	} name

# define SCLX_ADef( session, owner, name )\
	owner::s##name owner::name;\
	const char *owner::s##name::Name = #name;\
	void owner::s##name::SCLXLaunch(\
		session &Session,\
		const char *Id,\
		xdhcdc::eMode Mode )

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
			xdhcdc::eMode Mode )
		{
			cAction<session> *Callback = _Get( str::string( Action ) );

			if ( Callback == NULL )
				qRFwk();	// An event has no associated action. Check the'.xsl' file.

			return Callback->Launch( Session, Id, Mode );
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

	typedef fblfrd::cReporting cReporting_;

	typedef xdhcdc::cSingle cDownstream_;

	class sProxy
	{
	private:
		xdhdws::sProxy Core_;
		qRMV( const scli::sInfo, I_, Info_ );
		eXSLFileHandling XSLFileHandling_;
		void Fill_(
			str::dStrings &Values,
			const str::dString &Value )
		{
			Values.Append(Value);
		}
		void Fill_(
			str::dStrings &Values,
			const str::wString &Value ) // This variant with 'str::wStrind' is needed for the variadics to work.
		{
			Values.Append(Value);
		}
		void Fill_(
			str::dStrings &Values,
			const char *Value )
		{
			Values.Append(str::wString(Value));
		}
		void Fill_(
			str::dStrings &Values,
			const str::dStrings &SplittedValues );
		template <class s,class... Args> void Fill_(
			str::dStrings &Values,
			const s &Value,
			const Args &...args )
		{
			Fill_(Values, Value);
			Fill_(Values, args...);
		}
		template <class ...Args> void Process_(
			const char *ScriptName,
			str::dString *Result,
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
		void Alert_(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title,
			const char *Language );
		void Alert_(
			const str::dString &Message,
			const str::dString &Title,
			const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
			const char *CloseTextLanguage );
		bso::bool__ Confirm_(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title,
			const char *Language )
		{
			qRLmt();
			// Reactivate in '.cpp'.
			return false;
		}
		bso::sBool Confirm_(
			const str::dString &Message,
			const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
			const char *CloseTextLanguage );
		void HandleLayout_(
			const char *Variant,
			const str::dString &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclr::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker);
		void HandleLayout_(
			const char *Variant,
			const char *Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclr::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker)
		{
			return HandleLayout_(Variant, str::wString(Id), XSLFilename, Target, Registry, XML, Marker);
		}
		template <typename session, typename rack, typename chars> void HandleLayout_(
			const char *Variant,
			const chars &Id,
			const char *Target,
			const sclr::registry_ &Registry,
			void( *Get )(session &Session, xml::rWriter &Writer),
			session &Session,
			bso::char__ Marker = DefaultMarker )
		{
		qRH;
			rack Rack;
		qRB;
			Rack.Init( Target, Session, I_() );

			Get( Session, Rack() );

			HandleLayout_( Variant, Id, registry::definition::XSLFile, Target, Registry, Rack.Target(), Marker );
		qRR;
		qRT;
		qRE;
		}
		void HandleClasses_(
			const char *Variant,
			const str::dStrings &Ids,
			const str::dStrings &Classes)
		{
			Process_("HandleClasses_1", NULL, Variant, Ids, Classes);
		}
	protected:
		template <typename chars> void PrependLayout_(
			const chars &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclr::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker)
		{
			return HandleLayout_("Prepend", Id, XSLFilename, Target, Registry, XML, Marker);
		}
		template <typename chars> void SetLayout_(
			const chars &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclr::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker)
		{
			return HandleLayout_("Set", Id, XSLFilename, Target, Registry, XML, Marker);
		}
		template <typename chars> void AppendLayout_(
			const chars &Id,
			const rgstry::rEntry &XSLFilename,
			const char *Target,
			const sclr::registry_ &Registry,
			const str::dString &XML,
			bso::char__ Marker)
		{
			return HandleLayout_("Append", Id, XSLFilename, Target, Registry, XML, Marker);
		}
		template <typename session, typename rack, typename chars> void PrependLayout_(
			const chars &Id,
			const char *Target,
			const sclr::registry_ &Registry,
			void( *Get )(session &Session, xml::rWriter &Writer),
			session &Session,
			bso::char__ Marker = DefaultMarker )
		{
			return HandleLayout_<session,rack,chars>("Prepend", Id, Target, Registry, Get, Session, Marker);
		}
		template <typename session, typename rack, typename chars> void SetLayout_(
			const chars &Id,
			const char *Target,
			const sclr::registry_ &Registry,
			void( *Get )(session &Session, xml::rWriter &Writer),
			session &Session,
			bso::char__ Marker = DefaultMarker )
		{
			return HandleLayout_<session,rack,chars>("Set", Id, Target, Registry, Get, Session, Marker);
		}
		template <typename session, typename rack, typename chars> void AppendLayout_(
			const chars &Id,
			const char *Target,
			const sclr::registry_ &Registry,
			void( *Get )(session &Session, xml::rWriter &Writer),
			session &Session,
			bso::char__ Marker = DefaultMarker )
		{
			return HandleLayout_<session,rack,chars>("Append", Id, Target, Registry, Get, Session, Marker);
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, Info_ );
			XSLFileHandling_ = xfh_Undefined;
		}
		qCDTOR( sProxy );
		void Init(
			xdhcuc::cSingle &Callback,
			const scli::sInfo &Info,
			eXSLFileHandling XSLFileHandling )
		{
			Core_.Init(Callback, str::Empty);
			Info_ = &Info;
			XSLFileHandling_ = XSLFileHandling;
		}
		const scli::sInfo &Info( void ) const
		{
			return I_();
		}
		void Execute(
			const str::dString &Script,
			str::dString &Result )
		{
			Process_("Execute_1", &Result, Script);
		}
		void Execute( const str::dString &Script )
		{
			Process_("Execute_1", NULL, Script);
		}
		void Execute(
			const char *Script,
			str::dString &Result )
		{
			return Execute(str::wString(Script), Result);
		}
		void Execute(const char *Script)
		{
			return Execute(str::wString(Script));
		}
		void Log( const str::dString &Message )
		{
			qRLmt();
		}
		// The basic alert, without use of 'JQuery' based widget.
		void AlertB( const str::dString & Message );
		void Alert(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title,
			const char *Language );
		void AlertT(
			const str::dString &RawMessage,
			const str::dString &RawTitle,
			const char *Language );	// Translates 'Message'.
		void AlertU(
			const str::dString &Message,
			const str::dString &Title,
			const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.
		bso::bool__ Confirm(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title,
			const char *Language )
		{
			return Confirm_( XML, XSL, Title, Language );
		}
		bso::bool__ ConfirmT(
			const str::dString &RawMessage,
			const char *Language );
		bso::bool__ ConfirmT(
			const char *RawMessage,
			const char *Language )
		{
			return ConfirmT(str::wString(RawMessage), Language);
		}
		bso::bool__ ConfirmU(
			const str::dString &Message,
			const char *Language );	// Displays 'Message' as is. 'Language' is used for the closing text message.
		template <typename s, typename t, typename u> void SetAttribute(
			const s &Id,
			const t &Name,
			const u &Value )
		{
			Process_("SetAttribute_1", NULL, Id, Name, Value);
		}
		const char *GetAttribute(
			const str::dString &Id,
			const str::dString &Name,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value();
		}
		const str::dString &GetAttribute(
			const str::dString &Id,
			const str::dString &Name,
			str::dString &Value )
		{
			qRLmt();
			return Value;
		}
		void RemoveAttribute(
			const str::dString &Id,
			const str::dString &Name )
		{
			qRLmt();
		}
		void SetContents(
			const str::dStrings &Ids,
			const str::dStrings &Contents )
		{
			Process_("SetContents_1", NULL, Ids, Contents);
		}
		template <typename c1, typename c2> void SetContent(
			const c1 &Id,
			const c2 &Content )
		{
			return SetContents(str::wStrings(Id),str::wStrings(Content));
		}
		void GetContents(
			const str::dStrings &Ids,
			str::dStrings &Contents );
		const str::dString &GetContent(
			const str::dString &Id,
			str::dString &Content );
		const str::dString &GetContent(
			const char *Id,
			str::dString &Content )
		{
			return GetContent(str::wString(Id), Content);
		}
		void SetTimeout(
			const str::dString &Delay,
			const str::dString &Action );
		const char *Parent(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value;
		}
		const char *FirstChild(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value;
		}
		const char *LastChild(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value();
		}
		const char *PreviousSibling(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value;
		}
		const char *NextSibling(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value;
		}
		void InsertChild(
			const str::dString &Child,
			const str::dString &Id )
		{
			qRLmt();
		}
		void AppendChild(
			const str::dString &Child,
			const str::dString &Id )
		{
			qRLmt();
		}
		void InsertBefore(
			const str::dString &Sibling,
			const str::dString &Id )
		{
			qRLmt();
		}
		void InsertAfter(
			const str::dString &Sibling,
			const str::dString &Id )
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
			const str::dStrings &Classes )
		{
			return HandleClasses_("Add", Ids, Classes);
		}
		template <typename c1, typename c2> void AddClass(
			const c1 &Id,
			const c2 &Class )
		{
			return AddClasses(str::wStrings(Id), str::wStrings(Class));
		}
		void RemoveClasses(
			const str::dStrings &Ids,
			const str::dStrings &Classes )
		{
			return HandleClasses_("Remove", Ids, Classes);
		}
		template <typename c1, typename c2> void RemoveClass(
			const c1 &Id,
			const c2 &Class )
		{
			return RemoveClasses(str::wStrings(Id), str::wStrings(Class));
		}
		void ToggleClasses(
			const str::dStrings &Ids,
			const str::dStrings &Classes )
		{
			return HandleClasses_("Toggle", Ids, Classes);
		}
		template <typename c1, typename c2> void ToggleClass(
			const c1 &Id,
			const c2 &Class )
		{
			return ToggleClasses(str::wStrings(Id), str::wStrings(Class));
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
		void DressWidgets( const str::dString &Id )
		{
			qRLmt();
		}
		const char *Dummy(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value;
		}
	};

	class sReporting
	: public cReporting_
	{
	private:
		Q37_MRMDF( sProxy, P_, Proxy_ );
		Q37_MPMDF( const char, L_, Language_ );
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const str::dString &Message ) override
		{
			if ( Reply == fblovl::rDisconnected )
				P_().AlertT( str::wString("SCLXHTML_Disconnected"), str::wString(), L_() );
			else {
				//				sclmisc::ReportAndAbort( Message );
				P_().AlertU( Message, str::wString(), L_() );
				qRAbort();
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Proxy_ = NULL;
		}
		E_CVDTOR( sReporting );
		void Init(
			sProxy &Proxy,
			const char *Language )
		{
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

		void BroadcastAction(
			const char *Action,
			const char *Id);

	template <typename session> class rCore;

	// User put in 'instances' all his own objects, instantiating all with a 'new' (by overloading 'SCLXHTMLNew(...)'), a 'delete' will be made automatically when unloading the library.
	template <typename instances, typename frontend, typename page, page UndefinedPage, typename dump> class rSession
	: public cDownstream_,
		public sProxy,
		public instances,
		public frontend
	{
	private:
		eXSLFileHandling XSLFileHandling_;
		qCRMV( scli::sInfo, I_, Info_ );
		qRMV( sclf::rKernel, K_, Kernel_ );
		page Page_;	// Current page;
		sReporting Reporting_;
		eBackendVisibility BackendVisibility_;
		qRMV( class rCore<rSession>, C_, Core_ );
		faas::sId Id_;
	protected:
		bso::sBool XDHCDCInitialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token)	override // If empty, PROD session, else token used for the DEMO session.
		{
			if ( Token.Amount() )
				qRFwk();    // Should never be maunched in 'DEMO' mode.

			sProxy::Init( Callback, I_(), XSLFileHandling_ );
			Reporting_.Init( *this, Language );
			frontend::Init( K_(), Language, Reporting_ );

			return true;
		}
		bso::bool__ XDHCDCLaunch(
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
			Id_ = faas::UndefinedId;
		}
		qCVDTOR( rSession )
		void Init(
			sclf::rKernel &Kernel,
			class rCore<rSession> &Core,
			const scli::sInfo &Info,
			faas::sId Id,
			eXSLFileHandling XSLFileHandling = xfh_Default )
		{
			this->XSLFileHandling_ = XSLFileHandling;
			Info_ = &Info;
			Kernel_ = &Kernel;
			Page_ = UndefinedPage;
			// instances::Init( *this );	// Made on connection.
			BackendVisibility_ = bvShow;	// By default, the backend part of the login page is shown.
			Core_ = &Core;
			Id_ = Id;
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
			return scll::GetTranslation( Message, Language(), Translation );
		}
		bso::sBool GetHead( str::dString &Content )
		{
			return GetHead_( frontend::Registry(), Content, DefaultMarker );
		}
		void AlertU( const str::dString &Message )	// Displays 'Message' as is.
		{
			sProxy::AlertU( Message, Language() );
		}
		void AlertU( const char *Message )	// Displays 'Message' as is.
		{
			AlertU( str::wString(Message));
		}
		void AlertT(
			const str::dString &RawMessage,
			const str::dString &RawTitle)	// Translates 'RawMessage'.
		{
			sProxy::AlertT( RawMessage, RawTitle, Language() );
		}
		void AlertT(
			const char *RawMessage,
			const char *RawTitle)	// Translates 'RawMessage'.
		{
			AlertT(str::wString(RawMessage),str::wString(RawTitle));
		}
		void Alert(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title )
		{
			sProxy::Alert( XML, XSL, Title, Language() );
		}
		template <typename i> void Alert( i I )
		{
			bso::pInt Buffer;

			AlertU( bso::Convert( I, Buffer ) );
		}
		bso::bool__ ConfirmU( const str::dString &Message )	// Displays 'Message' as is.
		{
			return sProxy::ConfirmU( Message, Language() );
		}
		bso::bool__ ConfirmT( const str::dString &RawMessage )	// Translates 'RawMessage'.
		{
			return sProxy::ConfirmT( RawMessage, Language() );
		}
		bso::bool__ ConfirmT( const char *RawMessage )	// Translates 'RawMessage'.
		{
			return ConfirmT( str::wString(RawMessage));
		}
		bso::bool__ Confirm(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title )
		{
			return sProxy::Confirm( XML, XSL, Title, Language() );
		}
		qRWDISCLOSEr( eBackendVisibility, BackendVisibility );
		qRODISCLOSEr( page, Page );
		template <typename chars> void SetElementLayout(
			const chars &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ),
			const sclr::dRegistry &Registry )
		{
			sProxy::SetLayout_<rSession, rRack<rSession,dump>,chars>( Id, Target, Registry, Get, *this );
		}
		template <typename chars> void SetElementLayout(
			const chars &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ) )
		{
			SetElementLayout( Id, Target, Get, frontend::Registry() );
		}
		inline void SetDocumentLayout(
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ),
			const sclr::dRegistry &Registry )
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

	template <typename session> class rCore
	{
	private:
		action_handler<session> _Handler;
		xdhcdc::eMode Mode_;
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
			Mode_ = xdhcdc::m_Undefined;
			_ActionHelperCallback = NULL;
			OnNewSession_ = NULL;
		}
		E_CVDTOR( rCore )
		template <typename action> void Init(
			xdhcdc::eMode Mode,
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

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		bso::char__ Marker = DefaultMarker )
	{
		return sclm::LoadXMLAndTranslateTags( FileName, Registry, Content, 0, Marker );
	}

	const scli::sInfo &SCLXInfo( void );	// To define by user.
	void SCLXInitialization( xdhcdc::eMode Mode );	// To define by user.

	xdhcdc::cSingle *SCLXFetchCallback(faas::sId Id);	// To define by user.

	void SCLXDismissCallback( xdhcdc::cSingle *Callback );	// To define by user.

	namespace prolog {
		static qCDEFS(BorderId, "Border" );
		static qCDEFS(ProjectTypeId, "ProjectType" );
		static qCDEFS(PredefinedProjectFormId, "PredefinedProjectForm" );
		static qCDEFS(PredefinedProjectId, "PredefinedProject" );
		static qCDEFS(RemoteProjectFormId, "RemoteProjectForm" );
		static qCDEFS(RemoteProjectId, "RemoteProject" );

		void GetLayout(
			sclf::rFrontend &Frontend,
			xml::rWriter &Writer );

		void HandleProjectTypeSwitching( sProxy &Proxy );

		void DisplaySelectedProjectFilename(
			sProxy &Proxy,
			const str::dString &Id );

		inline void DisplaySelectedProjectFilename(
			sProxy &Proxy,
			const char *Id )
		{
			return DisplaySelectedProjectFilename(Proxy, str::wString(Id));
		}

		sclm::eProjectType GetProjectFeatures(
			sProxy &Proxy,
			str::string_ &Feature );

		void LoadProject( sProxy &Proxy );
	}

	namespace login {
		static qCDEFS(BackendTypeId, "BackendType" );
		// Ids of the forms for the parameters of the different backend types.
		static qCDEFS(PredefinedBackendId, "PredefinedBackend" );
		static qCDEFS(RemoteBackendId, "RemoteBackend" );
		static qCDEFS(ProxyfiedBackendId, "ProxyfiedBackend" );
		static qCDEFS(EmbeddedBackendId, "EmbeddedBackend" );

		const char *GetLabel( eBackendVisibility );

		sclf::eLogin GetLayout(
			sclf::rFrontend &Frontend,
			xml::rWriter &Writer );

		void HandleBackendTypeSwitching( sProxy &Proxy );

		void GetBackendFeatures(
			sProxy &Proxy,
			sclf::rFeatures &Features );

		void DisplaySelectedEmbeddedBackendFilename(
			sProxy &Proxy,
			const str::dString &Id );

		inline void DisplaySelectedEmbeddedBackendFilename(
			sProxy &Proxy,
			const char *Id )
		{
			return DisplaySelectedEmbeddedBackendFilename(Proxy, str::wString(Id));
		}
	}
}

#endif
