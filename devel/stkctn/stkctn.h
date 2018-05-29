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
	using stkbse::dStack;

	template <typename structure, typename item, typename row> class dContainerStack
	: public dStack< structure, item, row >
	{
	public:
		struct s
		: public dStack< structure, item, row >::s
		{};
		dContainerStack( s &S )
		: dStack< structure, item, row >( S )
		{}
		void reset( bso::sBool P = true )
		{
			dStack< structure, item, row >::reset( P );
		}
		void Init( void )
		{
			dStack< structure, item, row >::Init();
		}
	};

	qW3( ContainerStack );

	using ctn::rRH;

	using ctn::rHF;
	using ctn::rFH;

	qROW( Row );

# define qCSTACKd( item, row )	dContainerStack< ctn::E_CONTAINERt_( item, row ), item, row >
# define qCSTACKw( item, row )	wContainerStack< ctn::E_CONTAINERt_( item, row ), item, row >

# define qCSTACKdl( item )		qCSTACKd( item, stkctn::sRow )
# define qCSTACKwl( item )		qCSTACKw( item, stkctn::sRow )

# define qMCSTACKd( item, row )	dContainerStack< ctn::E_MCONTAINERt_( item, row ), item, row >
# define qMCSTACKw( item, row )	wContainerStack< ctn::E_MCONTAINERt_( item, row ), item, row >

# define qMCSTACKdl( item )		qMCSTACKd( item, stkctn::sRow )
# define qMCSTACKwl( item )		qMCSTACKw( item, stkctn::sRow )


}

#endif
