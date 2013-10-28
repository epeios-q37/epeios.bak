/*
	'csdleo.h' by Claude SIMON (http://zeusw.org/).

	'csdleo' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CSDLEO__INC
# define CSDLEO__INC

# define CSDLEO_NAME		"CSDLEO"

# if defined( E_DEBUG ) && !defined( CSDLEO_NODBG )
#  define CSDLEO_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Client-Server Devices Library Embedded Overlapping

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "rgstry.h"

# include "csdscb.h"

# define CSDLEO_RETRIEVE_STEERING_FUNCTION_NAME		CSDLEORetrieveSteering
# define CSDLEO_RELEASE_STEERING_FUNCTION_NAME		CSDLEOReleaseSteering

/*
NOTA : version de la classe 'shared_data__', à mettre à jour à chaque fois que cette dernière est modifiée.
*/

# define CSDLEO_SHARED_DATA_VERSION_NUMBER	"3"

# define CSDLEO_SHARED_DATA_VERSION	CSDLEO_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

namespace csdleo {
	using namespace csdscb;

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

	typedef csdleo::callback__ *(retrieve_steering)( shared_data__ * );
	typedef void (release_steering)( csdleo::callback__ * );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
