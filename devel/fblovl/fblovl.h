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

//D Frontend/Backend Layout OVerLapping 

#ifndef FBLOVL__INC
# define FBLOVL__INC

#define FBLOVL_NAME		"FBLOVL"

# if defined( E_DEBUG ) && !defined( FBLOVL_NODBG )
#  define FBLOVL_DBG
# endif

# define FBLOVL_PROTOCOL_VERSION	"13"

# include "tol.h"
# include "csdrcc.h"

namespace fblovl {

	qENUM( Mode ) {
		mNone,
		mReferenced,	// Data is transmitted by reference.
		mSerialized,		// Data is serialized.
		m_Amount,
		m_Undefined
	};

	typedef csdrcc::cDriver cDriver_;

	class cDriver
	: public cDriver_
	{
	protected:
		virtual eMode FBLOVLMode( void ) = 0;
	public:
		eMode Mode( void )
		{
			return FBLOVLMode();
		}
	};

	enum reply__ {
		rOK,
		rSoftwareError,	// Lorsque survient un 'ERR[x|X](...)'.
		rRequestError,	// Lorsque survient un problme lors de l'excution de la requte.
		rDisconnected,	// Connection was lost.
		r_amount,
		r_Undefined
	};

	const char *GetLabel( reply__ Reply );
}

#endif
