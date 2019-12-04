/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDLEO_INC_
# define CSDLEO_INC_

# define CSDLEO_NAME		"CSDLEO"

# if defined( E_DEBUG ) && !defined( CSDLEO_NODBG )
#  define CSDLEO_DBG
# endif

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
NOTA : version de la classe 'shared_data__', � mettre � jour � chaque fois que cette derni�re est modifi�e.
*/

# define CSDLEO_SHARED_DATA_VERSION_NUMBER	"8"

# define CSDLEO_SHARED_DATA_VERSION	CSDLEO_SHARED_DATA_VERSION_NUMBER " " CPE_AL

namespace csdleo {
	
	enum mode__ {
		mEmbedded,
		mRemote,
		m_amount,
		m_Undefined
	};

	enum context__ {
		cIntrospection,	// R�cup�ration de l'API.
		cRegular,		// fonctionnement normal
		c_amount,
		c_Undefined,
	};

#pragma pack( push, 1)
	// NOTA : Si modifi�, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data_control__
	{
	public:
		const char *Version;	// Toujours en premi�re position.
		bso::uint__ Control;	// Une valeur relative au contenu de la structure, � des fins de test primaire de compatibilit�.
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

	// NOTA : Si modifi�, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data__ {
	public:
		context__ Context;
		mode__ Mode;
		const char *LibraryLocationAndName;
		void *UP;				// A la discr�tion de l'utilisateur.
		void reset( bso::bool__ = true )
		{
			Context = c_Undefined;
			Mode = m_Undefined;
			LibraryLocationAndName = NULL;
			UP = NULL;
		}
		E_CDTOR( data__ );
		data__(
			context__ Context,
			const char *LibraryLocationAndName,
			void *UP,
			mode__ Mode = mEmbedded )
		{
			reset( false );
			Init( Context, LibraryLocationAndName, UP, Mode );
		}
		void Init(
			context__ Context,
			const char *LibraryLocationAndName,
			void *UP,
			mode__ Mode = mEmbedded )
		{
			this->Context = Context;
			this->Mode = Mode;
			this->LibraryLocationAndName = LibraryLocationAndName;
			this->UP = UP;
		}
	};

	// NOTA : Si modifi�, modifier 'CSDLEO_SHARED_DATA_VERSION' !
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
			data__::Init( Data.Context, Data.LibraryLocationAndName, Data.UP, Data.Mode );
		}
	};
#pragma pack( pop )

	class callback__
	{
	protected:
		virtual void CSDLEOInitialize( const shared_data__ *Data ) = 0;
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
		void Initialize( const shared_data__ *Data )
		{
			CSDLEOInitialize( Data );
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

#endif
