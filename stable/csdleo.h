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
# include "cio.h"
# include "rgstry.h"

# include "csdscb.h"

# include <stdarg.h>

# define CSDLEO_RETRIEVE_CALLBACK_FUNCTION_NAME		CSDLEORetrieveCallback
// # define CSDLEO_RELEASE_CALLBACK_FUNCTION_NAME		CSDLEOReleaseCallback

/*
NOTA : version de la classe 'shared_data__',  mettre  jour  chaque fois que cette dernire est modifie.
*/

# define CSDLEO_SHARED_DATA_VERSION_NUMBER	"7"

# define CSDLEO_SHARED_DATA_VERSION	CSDLEO_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

namespace csdleo {
	
	enum mode__ {
		mEmbedded,
		mRemote,
		m_amount,
		m_Undefined
	};

	enum context__ {
		cIntrospection,	// Rcupration de l'API.
		cRegular,		// fonctionnement normal
		c_amount,
		c_Undefined,
	};

#pragma pack( push, 1)
	// NOTA : Si modifi, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data_control__
	{
	public:
		const char *Version;	// Toujours en premire position.
		bso::uint__ Control;	// Une valeur relative au contenu de la structure,  des fins de test primaire de compatibilit.
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

	// NOTA : Si modifi, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data__ {
	public:
		context__ Context;
		const char *LibraryLocationAndName;
		err::err___ *ERRError;
		const cio::set__ *CIO;
		void *UP;				// A la discrtion de l'utilisateur.
		void reset( bso::bool__ = true )
		{
			Context = c_Undefined;
			LibraryLocationAndName = NULL;
			ERRError = NULL;
			CIO = NULL;
			UP = NULL;
		}
		E_CDTOR( data__ );
		data__(
			context__ Context,
			const char *LibraryLocationAndName,
			err::err___ *ERRError,
			void *UP )
		{
			reset( false );
			Init( Context, LibraryLocationAndName, ERRError,UP );
		}
		void Init(
			context__ Context,
			const char *LibraryLocationAndName,
			err::err___ *ERRError,
			void *UP )
		{
			this->Context = Context;
			this->LibraryLocationAndName = LibraryLocationAndName;
			this->ERRError = ERRError;
			this->CIO = &cio::GetCurrentSet();
			this->UP = UP;
		}
	};

	// NOTA : Si modifi, modifier 'CSDLEO_SHARED_DATA_VERSION' !
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
			data__::Init( Data.Context, Data.LibraryLocationAndName, Data.ERRError, Data.UP );
		}
	};
#pragma pack( pop )

	class callback__
	{
	protected:
		virtual void CSDLEOInitialize(
			const shared_data__ *Data,
			... ) = 0;
		virtual csdscb::callback__ *CSDLEORetrieveCallback(
			csdleo::mode__ Mode,
			csdleo::context__ Context ) = 0;
		virtual void CSDLEOReleaseCallback( csdscb::callback__ *Callback ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			//Standardisaction.
		}
		void Initialize(
			const shared_data__ *Data,
			const ntvstr::char__ *FirstParameter = NULL,
			... /* Autres paramtres. Le dernier doit tre = 'NULL' */ )
		{
			va_list Parameters;
			va_start( Parameters, FirstParameter );

			CSDLEOInitialize( Data, FirstParameter, Parameters );	// 'FirstParameter' est inclus dans le '...' de la mthode appele.
			// Il n'existe en tant que paramtre de cette mthode que pour en faciliter la comprhension.
		}
		csdscb::callback__ *RetrieveCallback(
			csdleo::mode__ Mode,
			csdleo::context__ Context )
		{
			return CSDLEORetrieveCallback( Mode, Context );
		}
		void ReleaseCallback( csdscb::callback__ *Callback)
		{
			CSDLEOReleaseCallback( Callback );
		}
	};

	typedef callback__ &(retrieve_callback)( void );
//	typedef void (release_callback)( csdleo::callback__ * );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
