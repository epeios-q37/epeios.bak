/*
	'dbpals' module by Claude SIMON ((http://zeusw.org/epeios/contact.html))
	Part of the 'erpck' tool.
	Copyright (C) 2010 Claude SIMON.
*/

// $Id: dbpals.cpp,v 1.3 2013/06/18 09:36:04 csimon Exp $

#include "dbpals.h"

using namespace dbpals;

trow__ dbpals::SearchTable(
	const str::string_ &Label,
	const table_aliases_ &Aliases )
{
	ctn::E_CMITEM( table_alias_ ) Alias;
	sdr::row__ Row = Aliases.First();

	Alias.Init( Aliases );

	if ( ( Row != qNIL ) && ( Alias( Row ).Label != Label ) )
		Row = Aliases.Next( Row );

	if ( Row != qNIL )
		return Alias( Row ).TableRow();
	else
		return qNIL;
}
