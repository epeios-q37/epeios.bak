/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef DPKBSC__INC
# define DPKBSC__INC

# include "bso.h"
# include "tol.h"
# include "str.h"

namespace dpkbsc {
	typedef bso::u8__ weight__;
#	define DEFAULT_WEIGHT	1

	typedef bso::uint__ counter__;
#define DPKBSC_COUNTER_MAX	BSO_UINT_MAX

	E_ROW( rrow__ );	// 'record row'.
	typedef bch::E_BUNCH_( rrow__ ) rrows_;
	E_AUTO( rrows );

	E_ROW( trow__ );	// 'table row'.

	template <typename container, typename item, typename row> inline row BaseSearch(
		const str::string_ &Label,
		const container &Container )
	{
		item Item;
		row Row = Container.First();

		Item.Init( Container );

		while ( ( Row != qNIL ) && ( Item( Row ).Label != Label ) )
			Row = Container.Next( Row );

		return Row;
	}

	template <typename container, typename item, typename row> inline row SearchInMulti(
		const str::string_ &Label,
		const container &Container )
	{
		return BaseSearch<container, ctn::qCITEMs( item, row ), row >( Label, Container );
	}

	template <typename container, typename item, typename row> inline row SearchInMono(
		const str::string_ &Label,
		const container &Container )
	{
		return BaseSearch<container, ctn::qCMITEMs( item, row ), row >( Label, Container );
	}
};

#endif

