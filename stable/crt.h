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

// CRaTe

#ifndef CRT_INC_
# define CRT_INC_

# define CRT_NAME		"CRT"

# if defined( E_DEBUG ) && !defined( CRT_NODBG )
#  define CRT_DBG
# endif

# include "err.h"

# include "ctn.h"

# include "tol.h"

namespace crt {
	using ctn::const_mono_item;
	using ctn::const_poly_item;
	using ctn::volatile_mono_item;
	using ctn::volatile_poly_item;

	template <typename type, typename row> qTCLONEd( ctn::mono_container_<qCOVER2(type,row)>, dMonoCrate );
	qW2( MonoCrate );

	template <typename type, typename row> qTCLONEd( ctn::poly_container_<qCOVER2(type,row)>, dPolyCrate );
	qW2( PolyCrate );

	using ctn::sHooks;

	using ctn::Search;

	template <typename row, typename crate> inline row NewAndInit_( crate &Crate )
	{
		row Row = Crate.New();

		Crate( Row ).Init();

		Crate.Flush();

		return Row;
	}

	template <typename row, typename item> inline row NewAndInit( ctn::mono_container_<item, row> &Crate )
	{
		return NewAndInit_<row>( Crate );
	}

	template <typename row, typename item> inline row NewAndInit( ctn::poly_container_<item, row> &Crate )
	{
		return NewAndInit_<row>( Crate );
	}
}

# define qMCRATEd( Type, r )	dMonoCrate< Type, r >
# define qMCRATEw( Type, r )	wMonoCrate< Type, r >

# define qMCRATEdl( Type )		qMCRATEd( Type, sdr::row__ )
# define qMCRATEwl( Type )		qMCRATEw( Type, sdr::row__ )


# define qCRATEd( Type, r )		dPolyCrate< Type, r >
# define qCRATEw( Type, r )		wPolyCrate< Type, r >

# define qCRATEdl( Type )		qCRATEd( Type, sdr::row__ )
# define qCRATEwl( Type )		qCRATEw( Type, sdr::row__ )

#endif
