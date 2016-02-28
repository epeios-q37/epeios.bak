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

// STacK ConTaiNer

#ifndef STKCTN__INC
# define STKCTN__INC

# define STKCTN_NAME		"STKCTN"

# if defined( E_DEBUG ) && !defined( STKCTN_NODBG )
#  define STKCTN_DBG
# endif

# include "stkbse.h"
# include "ctn.h"

namespace stkctn {
	using stkbse::vStack;

	template <typename structure, typename item, typename row> class vContainerStack
	: public vStack< structure, item, row >
	{
	public:
		struct s
		: public vStack< structure, item, row >::s
		{};
		vContainerStack( s &S )
		: vStack< structure, item, row >( S )
		{}
		void reset( bso::fBool P = true )
		{
			vStack< structure, item, row >::reset( P );
		}
		void Init( void )
		{
			vStack< structure, item, row >::Init();
		}
	};

	qW3( ContainerStack );

	using ctn::rRH;

	using ctn::rHF;
	using ctn::rFH;

	qROW( Row );

# define qCSTACKv( item, row )	vContainerStack< ctn::E_CONTAINERt_( item, row ), item, row >
# define qCSTACKi( item, row )	iContainerStack< ctn::E_CONTAINERt_( item, row ), item, row >

# define qCSTACKvl( item )		qCSTACKv( item, stkctn::fRow )
# define qCSTACKil( item )		qCSTACKi( item, stkctn::fRow )

# define qMCSTACKv( item, row )	vContainerStack< ctn::E_MCONTAINERt_( item, row ), item, row >
# define qMCSTACKi( item, row )	iContainerStack< ctn::E_MCONTAINERt_( item, row ), item, row >

# define qMCSTACKvl( item )		qMCSTACKv( item, stkctn::fRow )
# define qMCSTACKil( item )		qMCSTACKi( item, stkctn::fRow )


}

#endif
