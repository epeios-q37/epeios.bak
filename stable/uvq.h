/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// From libuv

#ifndef UVQ_INC_
# define UVQ_INC_

# define UVQ_NAME		"UVQ"

# if defined( E_DEBUG ) && !defined( UVQ_NODBG )
#  define UVQ_DBG
# endif

# include "err.h"
# include "tol.h"

namespace uvq {

	qENUM( Behavior ) {
		bRelaunch,	// Relaunches 'UVWork'.
		bExitAndDelete,	// Exits and calls 'delete' on the object, which have to be instantiated with 'new'.
		bExitOnly,	// Exits only.
		b_amount,
		b_Undefined
	};

	class cASync {
	protected:
		// Note to 'v8' user : you can NOT access any of the 'v8' data from this method.
		virtual void UVQWork( void ) = 0;
		// Note to 'v8' user : you CAN access any of the 'v8' data from this method.
		virtual eBehavior UVQAfter( void ) = 0;
	public:
		qCALLBACK( ASync );
		void Work( void )
		{
			return UVQWork();
		}
		eBehavior After( void )
		{
			return UVQAfter();
		}
	};

	void Launch( cASync &Callbacks );

	bso::sUInt AmountPending( void );
}

#endif
