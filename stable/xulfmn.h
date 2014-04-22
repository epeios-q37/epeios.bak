/*
	Header for the 'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: xulfmn.h,v 1.15 2012/10/23 16:15:57 csimon Exp $

#ifndef XULFMN__INC
#define XULFMN__INC

#define XULFMN_NAME		"XULFMN"

#define	XULFMN_VERSION	"$Revision: 1.15 $"

#define XULFMN_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFMNTutor;

#if defined( E_DEBUG ) && !defined( XULFMN_NODBG )
#define XULFMN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.15 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/10/23 16:15:57 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend MaiN 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfbs.h"

# define XULFMN_AFFIX	"Main"

namespace xulfmn {
	using namespace xulfbs;

	typedef nsxpcm::ui_core__ _ui_core__;

	class widgets__
	{
	public:
		menu__ mnuPredefinedProject;
		deck__ dckMain;
		deck__ dckProjectType;
		menulist__ mnlProjectType;
		menulist__ mnlPredefinedProjectList;
		textbox__ txbUserProjectLocation;
		widget__
			vewHome,
			vewSessionForm,
			vewSessionView;
		void reset( bso::bool__ P = true )
		{
			mnuPredefinedProject.reset( P );
			dckMain.reset( P );
			dckProjectType.reset( P );
			mnlProjectType.reset( P );
			mnlPredefinedProjectList.reset( P );
			txbUserProjectLocation.reset( P );
			vewHome.reset( P );
			vewSessionForm.reset( P );
			vewSessionView.reset( P );
		}
		E_CDTOR( widgets__ )
		void Init( void )
		{
			mnuPredefinedProject.Init();
			dckMain.Init();
			dckProjectType.Init();
			mnlProjectType.Init();
			mnlPredefinedProjectList.Init();
			txbUserProjectLocation.Init();
			vewHome.Init();
			vewSessionForm.Init();
			vewSessionView.Init();
		}
		void Attach( nsIDOMWindow *Window );
	};

	struct _main_core__
	: public _core__
	{
	protected:
		void _Refresh( xml::writer_ &Digest );
	public:
		widgets__ Widgets;
		void reset( bso::bool__ P = true )
		{
			_core__::reset( P );
			Widgets.reset( P );
		}
		E_CDTOR( _main_core__ )
		void Init( void )
		{
			_core__::Init();
			Widgets.Init();
		}
		void Attach( nsIDOMWindow *Window );
		widget__ &vewHome( void )
		{
			return Widgets.vewHome;
		}
		widget__ &vewSessionForm( void )
		{
			return Widgets.vewSessionForm;
		}
		widget__ &vewSessionView( void )
		{
			return Widgets.vewSessionView;
		}
		deck__ &dckMain( void )
		{
			return Widgets.dckMain;
		}
	};
	
	template <typename trunk> E_TTCLONE__( xulfbs::_window__<trunk>, window__ );

	template <typename trunk> struct main__
	: public _main_core__,
	  public window__<trunk>
	{
	private:
		xulftk::trunk___ &_Trunk( void )
		{
			return window__<trunk>::Trunk();
		}
		nsIDOMWindow *_Window( void )
		{
			return window__<trunk>::Window();
		}
		nsIDOMDocument *_Document( void )
		{
			return window__<trunk>::Document();
		}
	protected:
		void Refresh( xml::writer_ &Digest )
		{
			_main_core__::_Refresh( Digest );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_main_core__::reset( P );
			window__<trunk>::reset( P );
		}
		E_CDTOR( main__ )
		void Init( trunk &Trunk )
		{
			_main_core__::Init();
			window__<trunk>::Init( Trunk );
		}
		trunk &Trunk( void )
		{
			return window__<trunk>::Trunk();
		}
		nsIDOMWindow *Window( void )
		{
			return window__<trunk>::Window();
		}
		nsIDOMDocument *Document( void )
		{
			return window__<trunk>::Document();
		}
		void Attach( nsIDOMWindow *Window )
		{
			_main_core__::Attach( Window );
			window__<trunk>::Attach( Window );
		}
		void Refresh( void )
		{
			window__<trunk>::Refresh();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
