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

#define XMLELM__COMPILATION

#include "xmlelm.h"

using namespace xmlelm;

erow__ xmlelm::Search(
	const name_ &Name,
	type__ Type,
	const element_tree_ &ElementTree,
	erow__ Parent )
{
	ctn::E_CMITEMt( name_, erow__ ) N;
	erow__ Row = ElementTree.FirstChild( Parent );

	N.Init( ElementTree );

	while ( ( Row != qNIL )
		    && ( ( ElementTree.GetOddity( Row ) != Type )
			     || ( N( Row ) != Name ) ) )
		Row = ElementTree.NextSibling( Row );

	return Row;
}


/*
static bso::bool__ WriteXMLAttributes_(
	const element_tree_ &ElementTree,
	erow__ Parent,
	txf::text_oflow__ &Flow )
{
	ctn::E_CMITEMt( name_, erow__ ) Name;
	bso::bool__ HasChild = false;
	erow__ Row = ElementTree.First( Parent );

	Name.Init( ElementTree );

	while( Row != qNIL ) {
		if ( ElementTree.GetOddity( Row ) == tAttribute )
			Flow << ' ' << Name( Row ) << "=\"\"";
		else
			HasChild = true;

		Row = ElementTree.Next( Row );
	}

	return HasChild;
}


static void WriteXMLWithEmptyTag_(
	const element_tree_ &ElementTree,
	erow__ Root,
	txf::text_oflow__ &Flow )
{
	dtr::browser__<erow__> Browser;
	ctn::E_CMITEMt( name_, erow__ ) Name;
	bso::bool__ NoRegularChild = false;
	bso::bool__ RootHasChild = false;

	Name.Init( ElementTree );

	Flow << "<" << Name( Root );

	if ( !( RootHasChild = WriteXMLAttributes_( ElementTree, Root, Flow ) ) )
			Flow << '/';

	Flow << '>';

	Browser.Init( Root );

	while ( ElementTree.Browse( Browser ) != qNIL ) {
		switch( Browser.Kinship() ) {
		case dtr::kParent:
			if ( !NoRegularChild )
				Flow << "</" << Name( Browser.Position() ) << ">";
			NoRegularChild = false;
			break;
		case dtr::kSibling:
			if ( ElementTree.GetOddity( Browser.Position() ) != tAttribute ) {
				Flow << "<" << Name( Browser.Position() );
				if ( !WriteXMLAttributes_( ElementTree, Browser.Position(), Flow ) )
					Flow << '/';
				Flow << '>';
				NoRegularChild = false;
			}
			break;
		case dtr::kChild:
			NoRegularChild = true;
			if ( ElementTree.GetOddity( Browser.Position() ) != tAttribute ) {
				Flow << "<" << Name( Browser.Position() );
				if ( !WriteXMLAttributes_( ElementTree, Browser.Position(), Flow ) )
					Flow << '/';
				Flow << '>';
				NoRegularChild = false;
			}
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( RootHasChild )
		Flow << "</" << Name( Root ) << '>';
}

static void WriteXMLNoEmptyTag_(
	const tags_ &Tags,
	txf::text_oflow__ &Flow )
{
	dtr::browser__<tag_row__> Browser;
	tag_row__ Precedent = qNIL;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	bso::bool__ NoRegularChild = true;
	bso::bool__ JustParent = false;

	Tag.Init( Tags );

	Browser.Init( Tags.GetRoot() );

	do {
		switch( Browser.Status() ) {
		case dtr::bParent:
		case dtr::bLast:
				if ( !JustParent && !Tag( Precedent ).IsAttribute() )
					Flow << "</" << Tag( Precedent ).Name << '>';
				Flow << "</" << Tag( Browser.Position() ).Name << '>';
				JustParent = true;
			break;
		case dtr::bBrother:
			if ( !Tag( Browser.Position() ).IsAttribute() ) {
				if ( !Tag( Precedent ).IsAttribute() && !JustParent )
					Flow << "</" << Tag( Precedent ).Name << '>';
				Flow << "<" << Tag( Browser.Position() ).Name;
				WriteXMLAttributes_( Tags, Browser.Position(), Flow );
				Flow << '>';
				NoRegularChild = false;
				JustParent = false;
			}
			break;
		case dtr::bChild:
			if ( !Tag( Browser.Position() ).IsAttribute() ) {
				Flow << "<" << Tag( Browser.Position() ).Name;
				WriteXMLAttributes_( Tags, Browser.Position(), Flow );
				Flow << '>';
				JustParent = false;
			}
			break;
		case dtr::bFirst:
			Flow << '<' << Tag( Browser.Position() ).Name;
			if ( WriteXMLAttributes_( Tags, Browser.Position(), Flow ) )
				Flow << '>';
			else
				Flow << "></" << Tag( Browser.Position() ).Name << '>';
			JustParent = false;
			break;
		default:
			ERRc();
			break;
		}

		Precedent = Browser.Position();
	} while( Tags.Browse( Browser ) != qNIL );
}

void xmltag::WriteXML(
	const tags_ &Tags,
	txf::text_oflow__ &Flow,
	mode Mode )
{
	switch ( Mode ) {
	case mWithEmptyTag:
		WriteXMLWithEmptyTag_( Tags, Flow );
		break;
	case mNoEmptyTag:
		WriteXMLNoEmptyTag_( Tags, Flow );
		break;
	default:
		ERRu();
		break;
	}
}
*/

void xmlelm::Merge(
	const element_tree_ &Source,
	erow__ SRoot,
	element_tree_ &Target,
	erow__ TRoot,
	element_map_ &ElementMap )
{
qRH
	erow__ TargetRow, IntermediateRow, SourceRow;
	ctn::E_CMITEMt( name_, erow__ ) Name;
	dtr::browser__<erow__> Browser;
qRB
	Name.Init( Source );

	ElementMap.Init();

	ElementMap.Allocate( Source.Amount() );

	TargetRow = TRoot;

	Browser.Init( SRoot );

	while( ( SourceRow = Browser.Position() ) != qNIL ) {
		switch( Browser.Kinship() ) {
		case dtr::kChild:
			if ( ( IntermediateRow = Search( Name( SourceRow ), Source.GetOddity( SourceRow ), Target, TargetRow ) ) == qNIL )
				IntermediateRow = Target.BecomeLastChild( Name( SourceRow ), Source.GetOddity( SourceRow ), TargetRow );
			ElementMap.Store( IntermediateRow, SourceRow );
			TargetRow = IntermediateRow;
			break;
		case dtr::kSibling:
			if ( ( IntermediateRow = Search( Name( SourceRow ), Source.GetOddity( SourceRow ), Target, Target.Parent( TargetRow ) ) ) == qNIL )
				IntermediateRow = Target.BecomeNextSibling( Name( SourceRow ), Source.GetOddity( SourceRow ), TargetRow );
			ElementMap.Store( IntermediateRow, SourceRow );
			TargetRow = IntermediateRow;
			break;
		case dtr::kParent:
			TargetRow = Target.Parent( TargetRow );
			break;
		default:
			qRFwk();
			break;
		}

		Source.Browse( Browser );
	}
qRR
qRT
qRE
}

