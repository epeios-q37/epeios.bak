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

# define CSDLEO_RETRIEVE_CALLBACK_FUNCTION_NAME		CSDLEORetrieveCallback
# define CSDLEO_RELEASE_CALLBACK_FUNCTION_NAME		CSDLEOReleaseCallback

/*
NOTA : version de la classe 'shared_data__', à mettre à jour à chaque fois que cette dernière est modifiée.
*/

# define CSDLEO_SHARED_DATA_VERSION_NUMBER	"6"

# define CSDLEO_SHARED_DATA_VERSION	CSDLEO_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

namespace csdleo {
	using namespace csdscb;

	enum mode__ {
		mEmbedded,
		mRemote,
		m_amount,
		m_Undefined
	};

	enum context__ {
		cIntrospection,	// Récupération de l'API.
		cRegular,		// fonctionnement normal
		c_amount,
		c_Undefined,
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
		context__ Context;
		const char *LibraryLocationAndName;
		err::err___ *ERRError;
		void *UP;				// A la discrétion de l'utilisateur.
		void reset( bso::bool__ = true )
		{
			Mode = m_Undefined;
			Context = c_Undefined;
			LibraryLocationAndName = NULL;
			ERRError = NULL;
			UP = NULL;
		}
		E_CDTOR( data__ );
		data__(
			mode__ Mode,
			const char *LibraryLocationAndName,
			err::err___ *ERRError,
			context__ Context,
			void *UP )
		{
			reset( false );
			Init( Mode, LibraryLocationAndName, ERRError, Context, UP );
		}
		void Init(
			mode__ Mode,
			const char *LibraryLocationAndName,
			err::err___ *ERRError,
			context__ Context,
			void *UP )
		{
			this->Mode = Mode;
			this->LibraryLocationAndName = LibraryLocationAndName;
			this->ERRError = ERRError;
			this->Context = Context;
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
			data__::Init( Data.Mode, Data.LibraryLocationAndName, Data.ERRError, Data.Context, Data.UP );
		}
	};
#pragma pack( pop )

	typedef csdleo::callback__ *(retrieve_callback)( shared_data__ * );
	typedef void (release_callback)( csdleo::callback__ * );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
