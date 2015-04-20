/*
	Header for the 'csdscm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: csdscm.h,v 1.10 2012/11/14 16:06:26 csimon Exp $

#ifndef CSDSCM__INC
#define CSDSCM__INC

#define CSDSCM_NAME		"CSDSCM"

#define	CSDSCM_VERSION	"$Revision: 1.10 $"

#define CSDSCM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDSCMTutor;

#if defined( E_DEBUG ) && !defined( CSDSCM_NODBG )
#define CSDSCM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Server CoMmon 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'CSDLEO'.

#include "err.h"
#include "flw.h"

namespace csdscm {
	enum action__ {
		aContinue,
		aStop,
		a_amount,
		a_Undefined
	};

	class user_functions__ {
	protected:
		virtual void *CSDPreProcess( const char *Origin ) = 0;
		virtual action__ CSDProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void CSDPostProcess( void *UP ) = 0;
		virtual void CSDExit( void ) = 0;	// Appel lorsque l'on quitte l'application
												// (facilite la mise en oeuvre en tant que service Windows).
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		user_functions__( void )
		{
			reset( false );
		}
		~user_functions__( void )
		{
			reset();
		}
		void *PreProcess( const char *Origin )
		{
			return CSDPreProcess( Origin );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return CSDProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			CSDPostProcess( UP );
		}
		void Exit( void )
		{
			CSDExit();
		}
		void Init( void )
		{
			// Standadisation.
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
