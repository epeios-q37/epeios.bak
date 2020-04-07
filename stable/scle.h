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

// SoCLe Error

#ifndef SCLE_INC_
# define SCLE_INC_

# define SCLE_NAME		"SCLE"

# if defined( E_DEBUG ) && !defined( SCLE_NODBG )
#  define SCLE_DBG
# endif

# include "ctn.h"
# include "err.h"
# include "lcl.h"
# include "lstbch.h"
# include "sdr.h"
# include "tht.h"

namespace scle {
	typedef tht::thread_id__ tid__;

	E_ROW( row__ );

	class rError
	{
	public:
		mtx::handler___ Mutex = mtx::Undefined;
		lstbch::E_LBUNCHt( tid__, row__ ) TIds;
		ctn::E_CONTAINERt( lcl::meaning_, row__ ) Meanings;
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Mutex != mtx::Undefined )
					mtx::Delete( Mutex );

			Mutex = mtx::Undefined;
			TIds.reset( P );
			Meanings.reset( P );
		}
		E_CDTOR( rError );
		void Init( void )
		{
			if ( Mutex != mtx::Undefined )
				mtx::Delete( Mutex );

			Mutex = mtx::Create();

			TIds.Init();
			Meanings.Init();
		}
	};

	// A surcharger.
	extern rError *SCLEError;

	const lcl::meaning_ &GetMeaning( lcl::meaning_ &Meaning );

	bso::bool__ IsErrorPending( void );

	void ResetPendingError( void );

	void SetMeaning( const lcl::meaning_ &Meaning );

	bso::bool__ GetPendingErrorTranslation(
		const char *Language,
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default );

}

#endif
