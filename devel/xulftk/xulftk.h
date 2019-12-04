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

#ifndef XULFTK_INC_
# define XULFTK_INC_

# define XULFTK_NAME		"XULFTK"

# if defined( E_DEBUG ) && !defined( XULFTK_NODBG )
#  define XULFTK_DBG
# endif

// XUL Frontend TrunK

# include "err.h"
# include "flw.h"

# include "nsxpcm.h"
# include "frdkrn.h"

# include "geckob.h"

# include "xulfui.h"
# include "frdssn.h"

namespace xulftk {
	using frdkrn::status__;

	typedef frdkrn::reporting_functions__ _reporting_functions__;

	const str::string_ &BuildAboutText(
		const char *LauncherIdentification,
		const char *BuildInformations,
		const char *AppName,
		const char *Version,
		const char *AuthorName,
		const char *AuthorContact,
		const char *Copyright,
		const char *SoftwareDetails,
		const char *SoftwareURL,
		xulftk::trunk___ &Trunk,
		str::string_ &Text );


	class reporting_functions__
	: public _reporting_functions__
	{
	private:
		trunk___ *_Trunk;
	protected:
		virtual void FRDKRNReport( const str::string_ &Message );
		virtual void FRDKRNNotify( const str::string_ &Message );
	public:
		void reset( bso::bool__ P = true )
		{
			_reporting_functions__::reset( P );
			_Trunk = NULL;
		}
		E_CVDTOR( reporting_functions__ );
		void Init( trunk___ &Trunk );
	};

	class _user_callback__
	{
	private:
		trunk___ *_Trunk;
		const char *_LauncherIdentification;	// Identification du lanceur du composant (normalement 'egeckcom' avec sa version).
	protected:
		trunk___ &_T( void )
		{
			if ( _Trunk == NULL )
				qRFwk();

			return *_Trunk;
		}
		void _GoToHome( void )
		{}	// Standadisation.
		void _DefineSession( const str::string_ &ProjectId );
# if 0
		void _DefineSession(
			const str::string_ &FileName,
			const xpp::criterions___ &Criterions );
# endif
		void _ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations );
		void _CancelSession( void );
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appel� par la red�fintion de 'XULFTKDropSession()' lorsque projet modifi�.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		// Ferme l'application. Normalement appel� par la red�finition de 'XULFTKExit()'.
		void _Exit( void );
		virtual void XULFTKAboutText(
			const char *LauncherIdentification,
			const char *BuildInformations,
			str::string_ &Text )
		{
			qRFwk();	// Si pas surcharg�, alors 'xulfmn::about_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			qRFwk();	// Si pas surcharg�, alors 'xulfmn::web_site_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKGoToHome( void ) = 0;
		virtual void XULFTKDefineSession( const fnm::name___ &ProjectFileName )	// Si la 'Size()' == 0, alors cr�ation d'un nouveau projet, sinon chargement du projet en question.
		{
			qRFwk();	//	Si pas surcharg�, alors 'xulfsf::open_project_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
# if 0	// Old
		virtual void XULFTKDefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			qRFwk();	//	Si pas surcharg�, alors 'xulfsf::open_project_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
# endif
		virtual void XULFTKApplySession( void )
		{
			qRFwk();	//	Si pas surcharg�, alors 'xulfsf::apply_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKCancelSession( void )
		{
			qRFwk();	//	Si pas surcharg�, alors 'xulfsf::cancel_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual bso::bool__ XULFTKDropSession( void )	// Retourne 'true' si la session aeffectivement �t� ferm�e, 'false' sinon.
		{
			qRFwk();	//	Si pas surcharg�, alors 'xulfmn::close_project_eh__::NSXPCMOnEvent()' doit �tre red�fini.

			return false;	// Pour �viter un 'warning'.
		}
		virtual bso::bool__ XULFTKExit( void )
		{
			qRFwk();	//	Si pas surcharg�, alors 'xulfmn::exit_eh__::NSXPCMOnEvent()' doit �tre red�fini.

			return true;	// Pour �viter un 'warning'.
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Trunk = NULL;
			_LauncherIdentification = NULL;
		}
		E_CVDTOR( _user_callback__ );
		void Init(
			const char *LauncherIdentification,	// Contenu NON dupliqu� !
			trunk___ &Trunk )
		{
			_Trunk = &Trunk;
			_LauncherIdentification = LauncherIdentification;
		}
		void AboutText( str::string_ &Informations )
		{
			XULFTKAboutText( _LauncherIdentification, cpe::GetDescription(), Informations );
		}
		void SiteURL( str::string_ &URL )
		{
			XULFTKSiteURL( URL );
		}
		void GoToHome( void )
		{
			XULFTKGoToHome();
		}
		void DefineSession( const fnm::name___ &ProjectFileName )
		{
			XULFTKDefineSession( ProjectFileName );
		}
# if 0	// Old
		void DefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			XULFTKDefineSession( ProjectFileName, XMLPreprocessorCriterions );
		}
# endif
		void ApplySession( void )
		{
			XULFTKApplySession();
		}
		void CancelSession( void )
		{
			XULFTKCancelSession();
		}
		bso::bool__ DropSession( void )	// Retourne 'true' si la session aeffectivement �t� ferm�e, 'false' sinon.
		{
			return XULFTKDropSession();
		}
		bso::bool__ Exit( void )
		{
			return XULFTKExit();
		}
	};

	template <typename trunk> class user_callback__
	: public _user_callback__
	{
	protected:
		trunk &_T( void )
		{
			return *(trunk *)&_user_callback__::_T();	// On peut caster, parce que 'Init(...)' garantit que l'objet est du bon type.
		}
	public:
		void Init(
			const char *LauncherIdentification,
			trunk &Trunk )
		{
			_user_callback__::Init( LauncherIdentification, Trunk );
		}
	};

	class trunk___
	{
	private:
		xulfui::ui___ *_UI;
		frdkrn::kernel___ *_Kernel;
		frdssn::session___ *_Session;
		_user_callback__ *_UserCallback;
		geckoo::steering_callback__  *_Steering;
		const char *_TargetName;
		const char *_DefaultXSLRootPath;
		reporting_functions__ _DefaultReportingFunctions;
		_user_callback__ &_UF( void )
		{
			if ( _UserCallback == NULL )
				qRFwk();

			return *_UserCallback;
		}
	protected:
		void Handle_( frdkrn::status__ Status );
		void _DefineSession( const str::string_ &ProjectId );	// 'registry' qui contient la configuration de l'application.
# if 0 // Old
		void _DefineSession(
			const str::string_ &ProjectFileName,	// Si non vide, contient le nom du fichier projet avec lequel pr�remplir le 'SessionForm'.
			const xpp::criterions___ &Criterions );	// 'registry' qui contient la configuration de l'application.
# endif
		// Normalement appel�e par la red�fintion de 'XULFTKApplySession()'. Charge le projet correspondant au fichier 'FileName'.
		void _ApplySession(	const frdkrn::compatibility_informations__ &CompatibilityInformations );
		// Normalement appel�e par la red�fintion de 'XULFTKCacnelSession()', m�me si ne fait rien (standardisation).
		void _CancelSession( void )
		{}
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appel� par la red�fintion de 'XULFTKDropSession()' lorsque projet modifi�.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		// Ferme l'application. Normalement appel� par la red�finition de 'XULFTKExit()'.
		void _Exit( void )
		{
			UI().Main().Window()->Close();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_UI = NULL;
			_Kernel = NULL;
			_Session =  NULL;
			_UserCallback = NULL;
			_Steering = NULL;
			_DefaultXSLRootPath = NULL;
			_DefaultReportingFunctions.reset( P );
//			_TrunkFunctions.reset();
		}
		trunk___( void )
		{
			reset( false );
		}
		~trunk___( void )
		{
			reset();
		}
		status__ Init(
			const char *TargetName,	// ATTENTION : pointeur copi�, contenu NON dupliqu�.
			const char *DefaultXSLRootPath,	// ATTENTION : pointeur copi�, contenu NON dupliqu�.
			xulfui::ui___ &UI,
			frdkrn::kernel___ &Kernel,
			frdssn::session___ &Session,
			_user_callback__ &UserCallback,
			geckoo::steering_callback__ &Steering )
		{
			_UI = &UI;
			_Kernel = &Kernel;
			_Session = &Session;
			_TargetName = TargetName;
			_UserCallback = &UserCallback;
			_Steering = &Steering;
			_DefaultXSLRootPath = DefaultXSLRootPath;
			_DefaultReportingFunctions.Init( *this );

			return frdkrn::sOK;
		}
		void XULWDGReport( const char *Message )	// Pseudo m�thode virtuelle.
		{
			_UI->LogAndPrompt( Message );
		}
		_reporting_functions__ &DefaultReportingFunctions( void )
		{
			return _DefaultReportingFunctions;
		}
		void ExposeSteering( void )
		{
			geckob::SetSteering( *_Steering );
		}
		const xulfui::ui___ &UI( void ) const
		{
#ifdef XULFTK_DBG
			if ( _UI == NULL )
				qRFwk();
#endif
			return *_UI;
		}
		xulfui::ui___ &UI( void )
		{
#ifdef XULFTK_DBG
			if ( _UI == NULL )
				qRFwk();
#endif
			return *_UI;
		}
		const frdkrn::kernel___ &Kernel( void ) const
		{
			if ( _Kernel == NULL )
				qRFwk();

			return *_Kernel;
		}
		frdkrn::kernel___ &Kernel( void )
		{
			if ( _Kernel == NULL )
				qRFwk();

			return *_Kernel;
		}
		const frdssn::session___ &Session( void ) const
		{
#ifdef XULFTK_DBG
			if ( _Session == NULL )
				qRFwk();
#endif
			return *_Session;
		}
		frdssn::session___ &Session( void )
		{
#ifdef XULFTK_DBG
			if ( _Session == NULL )
				qRFwk();
#endif
			return *_Session;
		}
		const frdrgy::registry_ &Registry( void ) const
		{
			return Session().Registry();
		}
		frdrgy::registry_ &Registry( void )
		{
			return Session().Registry();
		}
		const char *Language( void ) const
		{
			return Session().Language();
		}
		void RefreshUI( void )
		{
			UI().Refresh();
		}
		void About( void )
		{
		qRH
			str::string Text;
		qRB
			Text.Init();

			_UF().AboutText( Text ),

			UI().Alert( Text );
		qRR
		qRT
		qRE
		}
		void FinalizeLaunching( void );
# if 0 //Old
		bso::bool__ FinalizeLaunching( void )
		{
			bso::bool__ Defined = false;
		qRH
			str::string DefaultProjectFileName;
		qRB
			DefaultProjectFileName.Init();

			if ( Kernel().Registry().GetValue( frdrgy::DefaultProject, DefaultProjectFileName ) ) {
				DefineSession( DefaultProjectFileName, xpp::criterions___() );
				Defined = true;
			} else
				GoToHome();

			RefreshUI();

		qRR
		qRT
		qRE
			return Defined;
		}
# endif
		void BrowseWEBSite( void )
		{
		qRH
			str::string URL;
		qRB
			URL.Init();

			_UF().SiteURL( URL );

			nsxpcm::LaunchURI( URL );
		qRR
		qRT
		qRE
		}
		void GoToHome( void )
		{
			_UF().GoToHome();
		}
		bso::bool__ DefineSession( const fnm::name___ &ProjectFileName )
		{
			if ( _UF().DropSession() ) {
				_UF().DefineSession( ProjectFileName );
				return true;
			} else
				return false;
		}
# if 0	// Old.
		bso::bool__ DefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &Criterions )
		{
			if ( _UF().DropSession() ) {
				_UF().DefineSession( ProjectFileName, Criterions );
				return true;
			} else
				return false;
		}
# endif
		void ApplySession( void )
		{
			_UF().ApplySession();
			RefreshUI();
		}
		void CancelSession( void )
		{
			_UF().CancelSession();
			RefreshUI();
		}
		bso::bool__ DropSession( void )
		{
			if ( _UF().DropSession() ) {
				RefreshUI();
				return true;
			} else
				return false;
		}
		bso::bool__ Exit( void )
		{
			bso::bool__ Confirmation = false;
		qRH
			str::string Translation;
		qRB
			Translation.Init();
			Kernel().Locale().GetTranslation( XULFTK_NAME "_ExitConfirmation", Kernel().Language(), Translation );

			if ( Confirmation = nsxpcm::Confirm( UI().Main().Window(), Translation ) )
				Confirmation = _UF().Exit();
		qRR
		qRT
		qRE
			return Confirmation;
		}
		const char *DefaultXSLRootPath( void ) const
		{
			if ( _DefaultXSLRootPath == NULL )
				qRFwk();

			return _DefaultXSLRootPath;
		}
		const str::string_ &BuildXSLFileName(
			const char *XSLFileNameAffix,
			str::string_ &FileName );
		const str::string_ &BuildXSLDigestFileName(
			const char *XSLFileNameAffix,
			str::string_ &FileName );
		friend class _user_callback__;
	};

	inline void _user_callback__::_DefineSession( const str::string_ &ProjectId )
	{
		_T()._DefineSession( ProjectId );
	}
# if 0	// Old
	inline void _user_callback__::_DefineSession(
		const str::string_ &ProjectFileName,
		const xpp::criterions___ &Criterions )
	{
		_T()._DefineSession( ProjectFileName, Criterions );
	}
# endif

	inline void _user_callback__::_ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations )
	{
		_T()._ApplySession( CompatibilityInformations );
	}

	inline void _user_callback__::_CancelSession( void )
	{
		_T()._CancelSession();
	}

	inline bso::bool__ _user_callback__::_DefendSession( void )
	{
		return _T()._DefendSession();
	}

	inline void _user_callback__::_DropSession( void )
	{
		_T()._DropSession();
	}

	inline void _user_callback__::_Exit( void )
	{
		_T()._Exit();
	}

}

#endif
