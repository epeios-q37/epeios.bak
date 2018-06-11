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

// STacK BunCH

#ifndef STKBCH__INC
# define STKBCH__INC

# define STKBCH_NAME		"STKBCH"

# if defined( E_DEBUG ) && !defined( STKBCH_NODBG )
#  define STKBCH_DBG
# endif

# include "stkbse.h"
# include "bch.h"

namespace stkbch {
	using stkbse::dStack;

	using bch::sHook;

	template <typename item, typename row> class dBunchStack
	: public dStack< bch::bunch_< item, row >, item, row >
	{
	public:
		struct s
		: public dStack< bch::bunch_< item, row >, item, row >::s
		{};
		dBunchStack( s &S )
		: dStack< bch::bunch_< item, row >, item, row >( S )
		{}
		void reset( bso::sBool P = true )
		{
			dStack< bch::bunch_< item, row >, item, row >::reset( P );
		}
		void Init( void )
		{
			dStack< bch::bunch_< item, row >, item, row >::Init();
		}
		row Pop( item &Item )
		{
			return dStack< bch::bunch_< item, row >, item, row >::Pop( Item );
		}
		item Pop( void )
		{
			item Item;

			Pop( Item );

			return Item;
		}
		void Top( item &Item ) const
		{
			dStack< bch::bunch_< item, row >, item, row >::Top( Item );
		}
		item Top( void ) const
		{
			item Item;

			Top( Item );

			return Item;
		}
	};

	qW2( BunchStack )

	qROW( Row );

# define qBSTACKd( item, row )	dBunchStack< item, row >
# define qBSTACKw( item, row )	wBunchStack< item, row >

# define qBSTACKdl( item )	qBSTACKd( item, stkbch::sRow )
# define qBSTACKwl( item )	qBSTACKw( item, stkbch::sRow )

	using bch::rRH;

	using bch::rHF;
	using bch::rFH;

}

#endif
