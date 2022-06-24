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

// RaNDom

#ifndef RND_INC_
# define RND_INC_

# define RND_NAME		"RND"

# if defined( E_DEBUG ) && !defined( RND_NODBG )
#  define RND_DBG
# endif

# include "err.h"
# include "mtx.h"
# include "tol.h"

namespace rnd {
  qMIMICr(mtx::rHandle, hGuard);

	// Don't forget to call 'UnlockRandIfLocked()' when appropriate.
	void LockRand(hGuard &Guard);

	// Thread unsafe, to use after having called above function.
	inline int TURand(void)
	{
# undef rand
	  return rand();
# define rand TOL_RAND_MACRO_
	}

	/*f Initialize the random generator using the date & time.
	The used value is returned to be used with the following
	function to make the random generator always start with the same value. */
	inline unsigned int InitializeRandomGenerator( void )
	{
		unsigned int Seed = (unsigned int)time( NULL );

		srand( Seed );

		return Seed;
	}

	//f Initialize the random generator with 'Seed'.
	inline void InitializeRandomGenerator( unsigned int Seed )
	{
		srand( Seed );
	}

	int Rand(void);

	typedef char bUUID[37];

	const char *UUIDGen( bUUID &UUID );
}

#endif
