/*
	Header for the 'frdfrd' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FRDFRD__INC
#define FRDFRD__INC

#define FRDFRD_NAME		"FRDFRD"

#define	FRDFRD_VERSION	"$Revision: 1.9 $"

#define FRDFRD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDFRDTutor;

#if defined( E_DEBUG ) && !defined( FRDFRD_NODBG )
#define FRDFRD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD FRontenD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "frdbse.h"

# include "fblfrd.h"

# include "csducl.h"

# include "err.h"
# include "flw.h"

namespace frdfrd {
	using fblfrd::compatibility_informations__;

	using fblfrd::incompatibility_informations_;
	using fblfrd::incompatibility_informations;

	typedef fblfrd::universal_frontend___ _frontend___;

	class frontend___
	: public _frontend___
	{
	private:
		csducl::universal_client_ioflow___ _Flow;
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend___::reset( P );
			_Flow.reset( P );
		}
		E_CVDTOR( frontend___ );
		bso::bool__ Init(
			const char *Language,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::universal_client_core &ClientCore,
			fblfrd::reporting_callbacks__ &ReportingCallbacks,
			incompatibility_informations_ &IncompatibilityInformations )
		{
			fblfrd::mode__ Mode = fblfrd::m_Undefined;

			_Flow.Init( ClientCore );

			switch ( ClientCore.GetType() ) {
			case csducl::tPlugin:
				Mode = fblfrd::mEmbedded;
				break;
			case csducl::tDaemon:
				Mode = fblfrd::mRemote;
				break;
			default:
				ERRFwk();
				break;
			}

			return _frontend___::Init( Language, CompatibilityInformations, _Flow, Mode, ReportingCallbacks, IncompatibilityInformations );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
