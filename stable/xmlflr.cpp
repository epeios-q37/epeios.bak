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

#define XMLFLR__COMPILATION

#include "xmlflr.h"


using namespace xmlflr;

static bso::bool__ WriteAttributes_(
	const xmldoc::document_ &Document,
	vrow__ Row,
	filler__ &Filler )
{
	bso::bool__ HasChild = false;
qRH
	ctn::E_CMITEMt( value_, vrow__ ) Value;
	ctn::E_CMITEMt( name_, erow__ ) Name;
qRB
	Value.Init( Document.Data );
	Name.Init( Document.Structure );

	Row = Document.Data.FirstChild( Row );

	HasChild = ( Row != qNIL );

	while( Row != qNIL ) {
		if ( Document.Structure.GetOddity( Document.Data.GetOddity( Row ) ) == xmlelm::tAttribute )
			Filler.PutAttribute( Name( Document.Data.GetOddity( Row ) ), Value( Row ) );
		Row = Document.Data.NextSibling( Row );
	}
qRR
qRT
qRE
	return HasChild;
}

void xmlflr::filler__::Put(
	const xmldoc::document_ &Document,
	vrow__ Root )
{
qRH
	dtr::browser__<vrow__> Browser;
	ctn::E_CMITEMt( value_, vrow__ ) Value;
	ctn::E_CMITEMt( name_, erow__ ) Name;
	str::string Result;
qRB
	Value.Init( Document.Data );
	Name.Init( Document.Structure );

	PushTag( Name( Document.Data.GetOddity( Root ) ) );
	PutValue( Value( Root ) );

	Browser.Init( Root );
	Document.Data.Browse( Browser );

	while( Browser.Position() != qNIL ) {
		switch( Browser.Kinship() ) {
		case dtr::kChild:
		case dtr::kSibling:
			if ( Document.Structure.GetOddity( Document.Data.GetOddity( Browser.Position() ) ) != xmlelm::tAttribute ) {
				PushTag( Name( Document.Data.GetOddity( Browser.Position() ) ) );
				PutValue( Value( Browser.Position() ) );
				if ( WriteAttributes_( Document, Browser.Position(), *this ) )
					break;
			} else
				break;
		case dtr::kParent:
			PopTag();
			break;
		default:
			qRFwk();
			break;
		}

		Document.Data.Browse( Browser );
	}

	PopTag();
qRR
qRT
qRE
}

