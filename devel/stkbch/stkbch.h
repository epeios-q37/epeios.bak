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
	using stkbse::vStack;

	using bch::cHook;

	template <typename item, typename row> class vBunchStack
	: public vStack< bch::bunch_< item, row >, item, row >
	{
	public:
		struct s
		: public vStack< bch::bunch_< item, row >, item, row >::s
		{};
		vBunchStack( s &S )
		: vStack< bch::bunch_< item, row >, item, row >( S )
		{}
		void reset( bso::fBool P = true )
		{
			vStack< bch::bunch_< item, row >, item, row >::reset( P );
		}
		void Init( void )
		{
			vStack< bch::bunch_< item, row >, item, row >::Init();
		}
		row Pop( item &Item )
		{
			return vStack< bch::bunch_< item, row >, item, row >::Pop( Item );
		}
		item Pop( void )
		{
			item Item;

			Pop( Item );

			return Item;
		}
		void Top( item &Item ) const
		{
			vStack< bch::bunch_< item, row >, item, row >::Top( Item );
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

# define qBSTACKv( item, row )	vBunchStack< item, row >
# define qBSTACKi( item, row )	iBunchStack< item, row >

# define qBSTACKvl( item )	qBSTACKv( item, stkbch::fRow )
# define qBSTACKil( item )	qBSTACKi( item, stkbch::fRow )

	using bch::rRH;

	using bch::rHF;
	using bch::rFH;

}

#endif
