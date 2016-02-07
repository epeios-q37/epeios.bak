/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// PrOXY BASE

#ifndef PRXYBASE__INC
# define PRXYBASE__INC

# define PRXYBASE_NAME		"PRXYBASE"

# if defined( E_DEBUG ) && !defined( PRXYBASE_NODBG )
#  define PRXYBASE_DBG
# endif

# include "tol.h"
# include "flw.h"
# include "str.h"

# include "err.h"

namespace prxybase {
	qENUM( Type ) {
		tClient,
		tServer,
		t_amount,
		t_Undefined
	};

	inline eType GetType( flw::iflow__ &Flow )
	{
		return (eType)Flow.Get();
	}

	inline void PutType(
		eType Type,
		flw::oflow__ &Flow )
	{
		Flow.Put( Type );
	}

	inline void PutId(
		const char *Identifier,
		flw::oflow__ &Flow )
	{
		Flow.Write( Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.
	};

	inline const str::vString &GetId(
		flw::iflow__ &Flow,
		str::vString &Id )
	{
		bso::char__ C = 0;

		while ( ( C = Flow.Get() ) != 0 )
			Id.Append( C );

		return Id;
	}
}

#endif
