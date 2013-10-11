/*
	Header for the 'csddlc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: csddlc.h,v 1.13 2012/11/14 16:06:26 csimon Exp $

#ifndef CSDDLC__INC
#define CSDDLC__INC

#define CSDDLC_NAME		"CSDDLC"

#define	CSDDLC_VERSION	"$Revision: 1.13 $"

#define CSDDLC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDDLCTutor;

#if defined( E_DEBUG ) && !defined( CSDDLC_NODBG )
#define CSDDLC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Dynamic Library Client 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete ! User 'CSDLEC' instead."

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "csdebd.h"

namespace csddlc {

	class dynamic_library_client_core
	{
	private:
		csdscm::user_functions__ *_UserFunctions;
		void *_LibraryHandler;
	public:
		void reset( bso::bool__ P = true );
		dynamic_library_client_core( void )
		{
			reset( false );
		}
		~dynamic_library_client_core( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *LibraryName,
			void *UP );
		bso::bool__ IsInitialized( void ) const
		{
			return _UserFunctions != NULL;
		}
		csdscm::user_functions__ &GetSteering( void ) const
		{
			if ( !IsInitialized() )
				ERRu();

			return *_UserFunctions;
		}
};

	typedef csdebd::embed_client_server_ioflow___ _embed_client_server_ioflow___;

	
	class dynamic_library_client_ioflow___
	: public _embed_client_server_ioflow___
	{
	public:
		void Init( dynamic_library_client_core &Core )
		{
			_embed_client_server_ioflow___::Init( Core.GetSteering() );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
