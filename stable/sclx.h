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

# include "sclr.h"

# include "err.h"
# include "fblfrd.h"
# include "rgstry.h"
# include "tol.h"
# include "xdhcdc.h"
# include "xdhcmn.h"
# include "xdhdws.h"
# include "xdhutl.h"

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
			extern rEntry _HeadFile;	// For the head section of the HTML main page. One page only.
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
		cAction<session> *Get_( const str::string_ &Action ) const
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
			cAction<session> *Callback = Get_( str::string( Action ) );

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
		template <typename s, typename ...args> void Fill_(
			str::dStrings &Values,
			const s &Value,
			const args &...Args )
		{
			Fill_(Values, Value);
			Fill_(Values, Args...);
		}
		template <typename ...args> void Process_(
			const char *ScriptName,
			str::dString *Result,
			const args &...Args )
		{
		qRH
			str::wStrings Values;
		qRB
			Values.Init();
			Fill_(Values, Args...);
			Core_.Process(ScriptName, Values, Result);
		qRR
		qRE
		qRT
		}
		template <typename ...args> const str::dString &Process_(
			const char *ScriptName,
			str::dString &Buffer,
			const args &...Args )
		{
			Process_(ScriptName, &Buffer, Args...);

			return Buffer;
		}
		template <typename ...args> const char *Process_(
			const char *ScriptName,
			qCBUFFERh &Buffer,
			const args &...Args )
		{
		qRH
			str::wString Return;
		qRB
			Return.Init();
			Process_(ScriptName, &Return, Args...);
			Return.Convert(Buffer);
		qRR
		qRE
		qRT
			return Buffer;
		}
		template <typename ...args> const str::dString &Process_(
			const char *TaggedScript,
			const char *TagList,
			str::dString &Buffer,
			const args &...Args )
		{
			Process_(TaggedScript, TagList, &Buffer, Args...);

			return Buffer;
		}
		template <typename ...args> const char *Process_(
			const char *TaggedScript,
			const char *TagList,
			qCBUFFERh &Buffer,
			const args &...Args )
		{
		qRH
			str::wString Return;
		qRB
			Return.Init();
			Process_(TaggedScript, TagList, &Return, Args...);
			Return.Convert(Buffer);
		qRR
		qRE
		qRT
			return Buffer;
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
		void HandleClasses_(
			const char *Variant,
			const str::dStrings &Ids,
			const str::dStrings &Classes)
		{
			Process_("HandleClasses_1", NULL, Variant, Ids, Classes);
		}
		template <typename c1, typename c2> void GetValue_(
			const c1 &Id,
			c2 &Buffer )
		{
		qRH;
			str::wStrings Values;
		qRB;
			Values.Init();
			GetValues(str::wStrings(Id), Values);
			str::Recall(Values, Buffer);
		qRR;
		qRT;
		qRE;
		}
	protected:
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
		void HandleLayout_(
      const str::dString &Variant,
      const str::dString &Id,
      const str::dString &XML,
      const str::dString &XSL)
    {
    qRH;
      str::wString Dummy;
    qRB;
      Dummy.Init();

      Process_("HandleLayout_1", &Dummy, Variant, Id, XML, XSL); // The primitive returns a string, but which is not handled by the user.
    qRR;
    qRT;
    qRE;
    }
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, Info_ );
			XSLFileHandling_ = xfh_Undefined;
		}
		qCDTOR( sProxy );
		bso::sBool Init(
			xdhcuc::cSingle &Callback,
			const scli::sInfo &Info,
			eXSLFileHandling XSLFileHandling)
		{
			if ( !Core_.Init(Callback) )
        return false;

			Info_ = &Info;
			XSLFileHandling_ = XSLFileHandling;

			if ( tol::IsDev() )
        DebugLog();

			return true;
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
		template <typename ...args> void Execute(
			const char *TaggedScript,
			const char *TagList,
			str::dString *Result,
			const args &...Args )
		{
			Process_(TaggedScript, TagList, Result, Args...);
		}
		void DebugLog(bso::sBool Switch = true)
		{
			Process_("DebugLog_1", NULL, Switch ? "true" : "false");
		}
		template <typename s> void Log( const s &Message )
		{
			Process_("Log_1", NULL, Message);
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
    void Inner(
      const str::dString &Id,
      const str::dString &XML,
      const str::dString &XSL = str::Empty)
    {
      return HandleLayout_(str::wString("inner"), Id, XML, XSL);
    }
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
		void GetValues(
			const str::dStrings &Ids,
			str::dStrings &Values );
		template <typename c> const str::dString &GetValue(
			const c &Id,
			str::dString &Buffer )
		{
			GetValue_(Id, Buffer);

			return Buffer;
		}
		template <typename c> const char *GetValue(
			const c &Id,
			qCBUFFERh &Buffer )
		{
			GetValue_(Id, Buffer);

			return Buffer;
		}
		void SetValues(
			const str::dStrings &Ids,
			const str::dStrings &Values )
		{
			Process_("SetValues_1", NULL, Ids, Values);
		}
		template <typename c1, typename c2> void SetValue(
			const c1 &Id,
			const c2 &Value )
		{
			return SetValues(str::wStrings(Id),str::wStrings(Value));
		}
		template <typename chars> void Focus( const chars &Id )
		{
				Process_("Focus_1", NULL, Id);
		}
		template <typename chars> const char *Parent(
			const chars &Id,
			qCBUFFERh &Value )
		{
			Process_("Parent_1", Value, Id);

			return Value;
		}
		const char *FirstChild(
			const str::dString &Id,
			qCBUFFERh &Value )
		{
			qRLmt();
			return Value;
		}
		template <typename chars> const char *LastChild(
			const chars &Id,
			qCBUFFERh &Value )
		{
			Process_("LastChild_1", Value, Id);

			return Value;
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

	void Broadcast(
		const str::dString &Action,
		const str::dString &Id);

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

	xdhcdc::cSingle *SCLXFetchCallback();	// To define by user.

	void SCLXDismissCallback( xdhcdc::cSingle *Callback );	// To define by user.
}

#endif
