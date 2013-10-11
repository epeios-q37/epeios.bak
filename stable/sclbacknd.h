/*
	Header for the 'sclbacknd' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

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

//	$Id: sclbacknd.h,v 1.4 2013/01/18 17:37:27 csimon Exp $

#ifndef SCLBACKND__INC
#define SCLBACKND__INC

#define SCLBACKND_NAME		"SCLBACKND"

#define	SCLBACKND_VERSION	"$Revision: 1.4 $"

#define SCLBACKND_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLBACKNDTutor;

#if defined( E_DEBUG ) && !defined( SCLBACKND_NODBG )
#define SCLBACKND_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/01/18 17:37:27 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe BACKEND 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "csdleo.h"

# include "fblbkd.h"

namespace sclbacknd {

	struct _data__ 
	{
		fblbkd::backend___ &Backend;
		fblbkd::text_log_functions__<> RequestLogFunctions;
		flx::void_oflow_driver___ VoidFlowDriver;
		_data__( fblbkd::backend___ &Backend )
		: Backend( Backend )
		{}
		void Init( void )
		{
			VoidFlowDriver.Init( fdr::tsDisabled );
			RequestLogFunctions.Init( VoidFlowDriver );
		}
	};

	template <typename backend>	struct data___
	: public _data__
	{
	public:
		backend Backend;
		data___( void )
		: _data__( Backend )
		{}
		void Init( 
			fblbur::mode__ Mode,
			const lcl::locale_ &Locale,
			const char *Origin )
		{
			_data__::Init();
			Backend.Init( Mode, Locale, Origin );
		}
	};

	// A définir par l'utilisateur.
	extern const char *TargetName;

	void *New(
		fblbur::mode__ Mode,
		const lcl::locale_ &Locale,
		const char *Origin );	// A Surcharger.
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
