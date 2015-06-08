/*
	Header for the 'dbpbsc' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

#ifndef DBPBSC__INC
# define DBPBSC__INC

# include "bso.h"
# include "tol.h"
# include "str.h"

namespace dbpbsc {
	typedef bso::u8__ weight__;
#	define DEFAULT_WEIGHT	1

	typedef bso::uint__ counter__;
#define DBPBSC_COUNTER_MAX	BSO_UINT_MAX

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
		return BaseSearch<container, ctn::E_CITEMt( item, row ), row >( Label, Container );
	}

	template <typename container, typename item, typename row> inline row SearchInMono(
		const str::string_ &Label,
		const container &Container )
	{
		return BaseSearch<container, ctn::E_CMITEMt( item, row ), row >( Label, Container );
	}


};

#endif

