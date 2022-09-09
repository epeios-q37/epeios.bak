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

// SoCLe Session

#ifndef SCLS_INC_
# define SCLS_INC_

# define SCLS_NAME		"SCLS"

# if defined( E_DEBUG ) && !defined( SCLS_NODBG )
#  define SCLS_DBG
# endif

# include "sclf.h"
# include "sclx.h"

# include "xdhcdc.h"

# include "err.h"

namespace scls {
 	typedef xdhcdc::cSingle cDownstream_;

	template <typename session> class rCore;

  // To indicate if the backend dedicated part in the login page should or not be visible.
  qENUM( BackendVisibility )
  {
    bvHide,
    bvShow,
    bv_amount,
    bv_Undefined
  };


	// User put in 'instances' all his own objects, instantiating all with a 'new' (by overloading 'SCLXHTMLNew(...)'), a 'delete' will be made automatically when unloading the library.
	template <typename instances, typename frontend, typename page, page UndefinedPage, typename dump> class rSession
	: public cDownstream_,
		public sclx::sProxy,
		public instances,
		public frontend
	{
	private:
		sclx::eXSLFileHandling XSLFileHandling_;
		qCRMV( scli::sInfo, I_, Info_ );
		qRMV( sclf::rKernel, K_, Kernel_ );
		page Page_;	// Current page;
		sclx::sReporting Reporting_;
		qRMV( class rCore<rSession>, C_, Core_ );
		eBackendVisibility BackendVisibility_;
	protected:
		bso::sBool XDHCDCInitialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token) override// If empty, SlfH session, else token used for the FaaS session.
		{
			if ( Token.Amount() )
				qRFwk();    // Should never be launched in 'FaaS' mode.

			if ( !sProxy::Init(Callback, I_(), XSLFileHandling_) )
        return false;

			Reporting_.Init( *this, Language );
			frontend::Init( K_(), Language, Reporting_ );

			return true;
		}
		bso::bool__ XDHCDCHandle(
      const str::dString &Id,
      const str::dString &Action) override
		{
			bso::sBool Cont = false;
		qRH;
			qCBUFFERh BId, BAction;
		qRB;
			Cont = C_().Launch(*this, Id.Convert(BId), Action.Convert(BAction));
		qRR;
		qRT;
		qRE;
			return Cont;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			instances::reset( P );
			frontend::reset( P );
			XSLFileHandling_ = sclx::xfh_Undefined;
			tol::reset(P, Info_, Kernel_);
			Page_ = UndefinedPage;
			Reporting_.reset( P );
			sProxy::reset();
			Core_ = NULL;
			BackendVisibility_ = bv_Undefined;
		}
		qCVDTOR( rSession )
		void Init(
			sclf::rKernel &Kernel,
			class rCore<rSession> &Core,
			const scli::sInfo &Info,
			sclx::eXSLFileHandling XSLFileHandling = sclx::xfh_Default )
		{
			this->XSLFileHandling_ = XSLFileHandling;
			Info_ = &Info;
			Kernel_ = &Kernel;
			Page_ = UndefinedPage;
			// instances::Init( *this );	// Made on connection.
			Core_ = &Core;
			BackendVisibility_ = bvShow;	// By default, the backend part of the login page is shown.
		}
		qRWDISCLOSEr( eBackendVisibility, BackendVisibility );
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
			return GetHead_( frontend::Registry(), Content, sclx::DefaultMarker );
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
			return ConfirmT( str::wString(RawMessage));
		}
		bso::bool__ Confirm(
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Title )
		{
			return sProxy::Confirm( XML, XSL, Title, Language() );
		}
		qRODISCLOSEr( page, Page );
		template <typename chars> void Inner(
			const chars &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ),
			const sclr::dRegistry &Registry )
		{
			sProxy::HandleLayout_<rSession, sclx::rRack<rSession,dump>,chars>( "inner", Id, Target, Registry, Get, *this );
		}
		template <typename chars> void Inner(
			const chars &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ) )
		{
			Inner( Id, Target, Get, frontend::Registry() );
		}
		template <typename chars> void Last(
			const chars &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ),
			const sclr::dRegistry &Registry )
		{
			sProxy::HandleLayout_<rSession, sclx::rRack<rSession,dump>,chars>( "beforeend", Id, Target, Registry, Get, *this );
		}
		template <typename chars> void Last(
			const chars &Id,
			const char *Target,
			void( *Get )( rSession &Session, xml::rWriter &Writer ) )
		{
			Last( Id, Target, Get, frontend::Registry() );
		}
		template <typename chars> bool BGetValue(const chars &Id)
		{
			bso::sBool Return = false;
		qRH;
			str::wString Buffer;
		qRB;
			Buffer.Init();
			Return = GetValue(Id, Buffer) == "true";
		qRR;
		qRT;
		qRE;
			return Return;
		}
	};

	template <typename session> class rCore
	{
	private:
		sclx::action_handler<session> _Handler;
		xdhcdc::eMode Mode_;
		Q37_MRMDF( sclx::cActionHelper<session>, _AH, _ActionHelperCallback );
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
			sclx::cActionHelper<session> &ActionHelperCallback )
		{
			OnNewSession_ = OnNewSession.Name;
			_ActionHelperCallback = &ActionHelperCallback;
			Mode_ = Mode;
			_Handler.Init();
		}
		void AddActionCallback(
			const char *ActionName,
			sclx::cAction<session> &Callback )
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
				Session.SetAttribute( "", "xdh:onevents", "(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)" );
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

	namespace prolog {
		static qCDEFS(BorderId, "Border" );
		static qCDEFS(PresetId, "Preset" );
		static qCDEFS(SetupFormId, "SetupForm" );
		static qCDEFS(SetupId, "Setup" );
		static qCDEFS(ProjectFormId, "ProjectForm" );
		static qCDEFS(ProjectId, "Project" );

		void WriteLayout(
			sclf::rFrontend &Frontend,
			xml::rWriter &Writer );

		void HandlePresetSwitching( sclx::sProxy &Proxy );

		void DisplaySelectedProjectFilename(
			sclx::sProxy &Proxy,
			const str::dString &Id );

		inline void DisplaySelectedProjectFilename(
			sclx::sProxy &Proxy,
			const char *Id )
		{
			return DisplaySelectedProjectFilename(Proxy, str::wString(Id));
		}

		sclf::ePresetType GetPresetFeatures(
			sclx::sProxy &Proxy,
			str::string_ &Feature);

		bso::sBool LoadPreset( sclx::sProxy &Proxy );
	}

	namespace login {
		static qCDEFS(BackendId, "Backend");
		static qCDEFS(BackendTypeId, "BackendType" );
		// Ids of the forms for the parameters of the different backend types.
		static qCDEFS(PredefinedBackendId, "PredefinedBackend" );
		static qCDEFS(RemoteBackendId, "RemoteBackend" );
		static qCDEFS(ProxyfiedBackendId, "ProxyfiedBackend" );
		static qCDEFS(EmbeddedBackendId, "EmbeddedBackend" );

		const char *GetLabel( eBackendVisibility );

		sclf::eLogin WriteLayout(
			sclf::rFrontend &Frontend,
			xml::rWriter &Writer );

		void HandleBackendTypeSwitching(
      sclx::sProxy &Proxy,
      eBackendVisibility Visibility);

		template <typename session> inline void HandleBackendTypeSwitching(session &Session)
		{
		  return HandleBackendTypeSwitching(Session, Session.BackendVisibility());
		}

		void GetBackendFeatures(
			sclx::sProxy &Proxy,
			sclf::rBackendFeatures &Features );

		void DisplaySelectedEmbeddedBackendFilename(
			sclx::sProxy &Proxy,
			const str::dString &Id );

		inline void DisplaySelectedEmbeddedBackendFilename(
			sclx::sProxy &Proxy,
			const char *Id )
		{
			return DisplaySelectedEmbeddedBackendFilename(Proxy, str::wString(Id));
		}
	}
}

#endif
