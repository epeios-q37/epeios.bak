/*
	Header for the 'csdleo' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: csdleo.h,v 1.12 2013/04/09 08:03:07 csimon Exp $

#ifndef CSDLEO__INC
#define CSDLEO__INC

#define CSDLEO_NAME		"CSDLEO"

#define	CSDLEO_VERSION	"$Revision: 1.12 $"

#define CSDLEO_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CSDLEOTutor;

#if defined( E_DEBUG ) && !defined( CSDLEO_NODBG )
#define CSDLEO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.12 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 08:03:07 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Library Embedded Overlapping 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "rgstry.h"

#include "csdsuf.h"

# define CSDLEO_RETRIEVE_STEERING_FUNCTION_NAME		CSDLEORetrieveSteering
# define CSDLEO_RELEASE_STEERING_FUNCTION_NAME		CSDLEOReleaseSteering

/*
NOTA : version de la classe 'shared_data__', à mettre à jour à chaque fois que cette dernière est modifiée.
*/

# define CSDLEO_SHARED_DATA_VERSION_NUMBER	"3"

# define CSDLEO_SHARED_DATA_VERSION	CSDLEO_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

namespace csdleo {
	using namespace csdsuf;

	enum mode__ {
		mEmbedded,
		mRemote,
		m_amount,
		m_Undefined
	};

#pragma pack( push, 1)
	// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data_control__
	{
	public:
		const char *Version;	// Toujours en première position.
		bso::uint__ Control;	// Une valeur relative au contenu de la structure, à des fins de test primaire de compatibilité.
		void reset( bso::bool__ = true )
		{
			Version = NULL;
			Control = 0;
		}
		E_CDTOR( data_control__ );
		void Init( void )
		{
			Version = CSDLEO_SHARED_DATA_VERSION;
			Control = ControlComputing();
		}
		size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
		}
	};

	// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data__ {
	public:
		mode__ Mode;
		bso::bool__ DryRun;	// Si à 'true', le 'Server' n'est pas vraiment 'lancé' ; seul ces caractéristiques sont consultées (par 'getbkdapi', par exemple).
		void *UP;				// A la discrétion de l'utilisateur.
		fdr::oflow_driver_base___ *COut, *CErr;
		void reset( bso::bool__ = true )
		{
			COut = CErr = NULL;
			UP = NULL;
			DryRun = false;
		}
		E_CDTOR( data__ );
		data__(
			mode__ Mode,
			fdr::oflow_driver_base___ &COut,
			fdr::oflow_driver_base___ &CErr,
			bso::bool__ DryRun,
			void *UP = NULL )
		{
			Init( Mode, COut, CErr, DryRun, UP );
		}
		void Init(
			mode__ Mode,
			fdr::oflow_driver_base___ &COut,
			fdr::oflow_driver_base___ &CErr,
			bso::bool__ DryRun,
			void *UP = NULL )
		{
			this->Mode = Mode;
			this->COut = &COut;
			this->CErr = &CErr;
			this->DryRun = DryRun;
			this->UP = UP;
		}
	};

	// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	: public data_control__,
	  public data__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			data_control__::reset( P );
			data__::reset( P );
		}
		E_CDTOR( shared_data__ );
		void Init( data__ &Data )
		{
			data_control__::Init();
			data__::Init( Data.Mode, *Data.COut, *Data.CErr, Data.DryRun, Data.UP );
		}
	};
#pragma pack( pop )

	typedef csdleo::user_functions__ *(retrieve_steering)( shared_data__ * );
	typedef void (release_steering)( csdleo::user_functions__ * );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
