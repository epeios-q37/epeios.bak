/*
	Header for the 'dlbrry' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: dlbrry.h,v 1.8 2013/04/09 08:03:08 csimon Exp $

#ifndef DLBRRY__INC
#define DLBRRY__INC

#define DLBRRY_NAME		"DLBRRY"

#define	DLBRRY_VERSION	"$Revision: 1.8 $"

#define DLBRRY_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &DLBRRYTutor;

#if defined( E_DEBUG ) && !defined( DLBRRY_NODBG )
#define DLBRRY_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.8 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 08:03:08 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Dynamic LiBRaRY 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "tol.h"

# define DLBRRY_UNDEFINED	NULL

namespace dlbrry {
	typedef void *library_handler__;

	class dynamic_library___
	{
	private:
		library_handler__  _LibraryHandler;
		bso::bool__ _LoadLibrary( const char *LibraryName );
		bso::bool__ _UnloadLibrary( void  );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _LibraryHandler != DLBRRY_UNDEFINED ) {
					if ( !_UnloadLibrary() )
						ERRSys();
				}
			}

			_LibraryHandler = NULL;
		}
		dynamic_library___( void )
		{
			reset( false );
		}
		~dynamic_library___( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *LibraryName,
			err::handling__ ERRHandling = err::h_Default )
		{
			reset();

			if ( _LoadLibrary( LibraryName ) )
				return true;

			if ( ERRHandling != err::hUserDefined )
				ERRSys();

			return false;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _LibraryHandler != DLBRRY_UNDEFINED;
		}
		void *GetFunction( const char *FunctionName );
	};

	template <typename function> function GetFunction(
		const char *FunctionName,
		dynamic_library___ &Library )
	{
		if ( !Library.IsInitialized() )
			ERRFwk();

		return (function)Library.GetFunction( FunctionName );
	}



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
