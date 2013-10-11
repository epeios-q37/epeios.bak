/*
	'trunk.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: trunk.h,v 1.6 2013/06/18 09:36:02 csimon Exp $

#ifndef TRUNK__INC
# define TRUNK__INC


# include "xulftk.h"
# include "nsxpcm.h"

# include "sclrgstry.h"
# include "scllocale.h"

# include "sktinf.h"
# include "frdsessn.h"
# include "ui.h"

# define PROGRAM_NAME				SKTINF_LC_AFFIX	"gck"
# define AFFILIATED_SOFTWARE_NAME	SKTINF_AFFILIATED_SOFTWARE_NAME
# define AFFILIATED_SOFTWARE_URL	SKTINF_AFFILIATED_SOFTWARE_URL
# define AUTHOR_NAME				SKTINF_AUTHOR_NAME
# define AUTHOR_CONTACT				SKTINF_AUTHOR_CONTACT
# define WEBSITE_URL				SKTINF_WEBSITE_URL
# define COPYRIGHT					COPYRIGHT_YEARS " " SKTINF_OWNER_NAME " (" SKTINF_OWNER_CONTACT ")"

namespace trunk {
	using namespace frdkernl;

	typedef xulftk::trunk___ _trunk__;

	class trunk___
	: public _trunk__
	{
	private:
		ui::ui___ _UI;
		frdsessn::session___ _Session;
		bso::bool__ _IsProjectModified( void ) const
		{
			/// To modify if there are other criterium for project modification.
			return Kernel().IsProjectModified();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_trunk__::reset( P );
			_UI.reset( P );
			_Session.reset( P );
		}
		E_CVDTOR( trunk___ );
		void Init(
			xulftk::user_callback__<trunk___>  &UserCallback,
			kernel___ &Kernel,
			const lcl::locale_ &Locale,
			const char *Language,
			geckoo::steering_callback__ &Steering )
		{
			reset();

			_Session.Init( Kernel );
			_trunk__::Init( PROGRAM_NAME, "chrome://" PROGRAM_NAME "/content/xsl/", _UI, Kernel, _Session,  UserCallback, Steering ),
			_UI.Init( *this );
		}
		void ReportAndAbort( const lcl::meaning_ &Meaning )
		{
		ERRProlog
			str::string Translation;
		ERRBegin
			Translation.Init();
			scllocale::GetTranslation( Meaning, Language(), Translation );

			UI().Alert( Translation );

			ERRAbort();
		ERRErr
		ERREnd
		ERREpilog
		}
		void ReportAndAbort( const char *Text )
		{
		ERRProlog
			str::string Translation;
		ERRBegin
			Translation.Init();
			scllocale::GetTranslation( Text, Language(), Translation );

			UI().Alert( Translation );

			ERRAbort();
		ERRErr
		ERREnd
		ERREpilog
		}
		void FinalizeGoToHome( void )
		{
			UI().Show( ui::dHome );
		}
		void FinalizeDefineSession( void )
		{
			UI().Show( ui::dSessionForm );
		}
		void FinalizeApplySession( void )
		{
			UI().Show( ui::dSessionView );
		}
		void FinalizeCancelSession( void )
		{
			UI().Show( ui::dHome );
		}
		void FinalizeDropSession( bso::bool__ SavingProject )
		{
			if ( SavingProject ) {
					// Put here the code which saves the project.
			}

			UI().Show( ui::dHome );
		}
		void FinalizeExit( void )
		{
		}
		bso::bool__ IsProjectModified( void )
		{
			return _IsProjectModified();
		}
		bso::bool__ IsProjectInProgress( void )
		{
			return Kernel().IsProjectInProgress();
		}
		const ui::ui___ &UI( void ) const
		{
			return _UI;
		}
		ui::ui___ &UI( void )
		{
			return _UI;
		}
	};

	typedef xulftk::user_callback__<trunk___> _user_callback__;

	class user_callback__
	: public _user_callback__
	{
	protected:
		virtual void XULFTKAboutText(
			const char *LauncherIdentification,
			const char *BuildInformations,
			str::string_ &Text );
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			URL = WEBSITE_URL;
		}
		virtual void XULFTKGoToHome( void )
		{
			_GoToHome();
			_T().FinalizeGoToHome();
		}
		virtual void XULFTKDefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &Criterions )
		{
			_DefineSession( ProjectFileName, Criterions );
			_T().FinalizeDefineSession();
		}
		virtual	void XULFTKApplySession( void )
		{
			_ApplySession( fblfrd::compatibility_informations__( SKTINF_LC_AFFIX, ESKETCH_API_VERSION ) );
			_T().FinalizeApplySession();
		}
		virtual	void XULFTKCancelSession( void )
		{
			_CancelSession();
			_T().FinalizeCancelSession();
		}
		virtual bso::bool__ XULFTKDropSession( void )
		{
			if ( _T().IsProjectModified() )
				if ( _DefendSession() ) {
					_T().FinalizeDropSession( true );
					_DropSession();
					return true;
				}  else
					return false;
			else {
				_T().FinalizeDropSession( false );
				_DropSession();
				return true;
			}
		}
		virtual bso::bool__ XULFTKExit( void )
		{
			if ( _T().IsProjectInProgress() ) {
				if ( DropSession() ) {
					_T().FinalizeExit();
					_Exit();
					reset();
				}
			} else {
				_T().FinalizeExit();
				_Exit();
				reset();
			}

			return true;
		}
	};
}

#endif