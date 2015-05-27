/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
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
NOTA : version de la classe 'shared_data__', à mettre à jour à chaque fois que cette dernière est modifiée.
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
		context__ Context;
		mode__ Mode;
		const char *LibraryLocationAndName;
		err::err___ *ERRError;
		const cio::set__ *CIO;
		void *UP;				// A la discrétion de l'utilisateur.
		void reset( bso::bool__ = true )
		{
			Context = c_Undefined;
			Mode = m_Undefined;
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
			void *UP,
			mode__ Mode = mEmbedded )
		{
			reset( false );
			Init( Context, LibraryLocationAndName, ERRError,UP, Mode );
		}
		void Init(
			context__ Context,
			const char *LibraryLocationAndName,
			err::err___ *ERRError,
			void *UP,
			mode__ Mode = mEmbedded )
		{
			this->Context = Context;
			this->Mode = Mode;
			this->LibraryLocationAndName = LibraryLocationAndName;
			this->ERRError = ERRError;
			this->CIO = &cio::GetCurrentSet();
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
			data__::Init( Data.Context, Data.LibraryLocationAndName, Data.ERRError, Data.UP, Data.Mode );
		}
	};
#pragma pack( pop )

	class callback__
	{
	protected:
		virtual void CSDLEOInitialize(
			const shared_data__ *Data,
			... ) = 0;
		// Returned callback is used to handle all connections !
		virtual csdscb::callback__ *CSDLEORetrieveCallback(
			csdleo::context__ Context,
			csdleo::mode__ Mode ) = 0;
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
			... /* Autres paramètres. Le dernier doit être = 'NULL' */ )
		{
			va_list Parameters;
			va_start( Parameters, FirstParameter );

			CSDLEOInitialize( Data, FirstParameter, Parameters );	// 'FirstParameter' est inclus dans le '...' de la méthode appelée.
			// Il n'existe en tant que paramètre de cette méthode que pour en faciliter la compréhension.
		}
		csdscb::callback__ *RetrieveCallback(
			csdleo::context__ Context,
			csdleo::mode__ Mode )
		{
			return CSDLEORetrieveCallback( Context, Mode );
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
