/*
	Header for the 'geckof' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: geckof.h,v 1.10 2013/04/09 17:57:31 csimon Exp $

#ifndef GECKOF__INC
#define GECKOF__INC

#define GECKOF_NAME		"GECKOF"

#define	GECKOF_VERSION	"$Revision: 1.10 $"

#define GECKOF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &GECKOFTutor;

#if defined( E_DEBUG ) && !defined( GECKOF_NODBG )
#define GECKOF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D GECKO Frontend 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "dlbrry.h"

# include "geckoo.h"

# define GECKOF_AUTOCOMPLETE_COMPONENT_NAME	"epeios_autocomplete_component;1"

namespace geckof {
	class gecko_wrapper___
	{
	private:
		dlbrry::dynamic_library___ _Library;
		geckoo::steering_callback__ *_Steering;
		bso::bool__ _CreateSteering( geckoo::shared_data__ *Data );
		bso::bool__ _DeleteSteering( void );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( _Library.IsInitialized() ) {
				if ( _Steering != NULL )
					if ( !_DeleteSteering() ) {
						_Library.reset();
						ERRSys();
					}

			}

			_Library.reset( P );
			_Steering = NULL;
		}
		gecko_wrapper___( void )
		{
			reset( false );
		}
		~gecko_wrapper___( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *LibraryName,
			geckoo::shared_data__ *SharedData,
			err::handling__ ERRHandling = err::h_Default )
		{
			reset();

			if ( _Library.Init( LibraryName, ERRHandling ) )
				if ( _CreateSteering( SharedData ) )
					return true;

			if ( ERRHandling != err::hUserDefined )
				ERRSys();

			return false;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Steering != NULL;
		}
		geckoo::steering_callback__ &GetSteering( void ) const
		{
			if ( !IsInitialized() )
				ERRFwk();

			return *_Steering;
		}
	};

	geckoo::steering_callback__ *RetrieveSteering(
		const char *LibraryName,
		err::handling__ ERRHandling = err::h_Default );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
