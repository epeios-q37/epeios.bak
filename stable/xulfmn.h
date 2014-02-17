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
	};

	namespace {
		typedef xulfbs::_t_window__<xulftk::trunk___> window__;
	}

	struct main__
	: public window__
	{
	protected:
		virtual const char *XULFBSGetDigest( xml::writer_ &Digest );
	public:
		widgets__ &Widgets;
		void reset( bso::bool__ P = true )
		{
			_window__::reset( P );
		}
		main__( widgets__ &Widgets )
		: Widgets( Widgets )
		{
			reset( false );
		}
		E_DTOR( main__ )
		void Init( xulftk::trunk___ &Trunk )
		{
			_window__::Init( Trunk );
		}
		void Register( nsIDOMWindow *Window );
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
