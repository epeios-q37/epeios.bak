/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "frdinstc.h"

#include "frdcllbck.h"

#include "flf.h"
#include "fnm.h"

using namespace frdinstc;

#define C( name )	case t##name : return #name; break
 
const char *frdinstc::GetLabel( eTarget Target )
{
	switch ( Target ) {
	C( Column );
	C( Field );
	C( Record );
	C( Records );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}
 
#undef C

namespace {
	void Dump_(
		const fbltyp::dIds &Ids,
		const fbltyp::dIds &Types,
		const fbltyp::dId8s &Numbers,
		const str::dStrings &Labels,
		const str::dStrings &Comments,
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Ids.First();

		Writer.PushTag( "Columns" );
		xml::PutAttribute("Amount", Ids.Amount(), Writer );

		while ( Row != qNIL ) {
			Writer.PushTag( "Column" );

			xml::PutAttribute( "id", *Ids( Row ), Writer );
			xml::PutAttribute( "Type", *Types( Row ), Writer );
			xml::PutAttribute( "Number", *Numbers( Row ), Writer );
			Writer.PutValue( Labels( Row ), "Label"  );
			Writer.PutValue( Comments( Row ), "Comment" );

			Writer.PopTag();

			Row = Ids.Next( Row );
		}

		Writer.PopTag();
	}
}

void frdinstc::rUser::DumpCurrentRecordColumns( xml::dWriter &Writer ) const
{
qRH
	fbltyp::wIds Columns;
	fbltyp::wIds Types;
	fbltyp::wId8s Numbers;
	str::wStrings Labels;
	str::wStrings Comments;
qRB
	if ( Record_ == UndefinedRecord )
		qRGnr();

	Columns.Init();
	Types.Init();
	Numbers.Init();
	Labels.Init();
	Comments.Init();
	Core_.GetRecordColumns( Record_, Columns, Types, Numbers, Labels, Comments );

	Dump_( Columns, Types, Numbers, Labels, Comments, Writer );
qRR
qRT
qRE
}

namespace {
	void Dump_(
		const fbltyp::dStrings &RawEntries,
		frdcllbck::cXML &Callback,
		xml::dWriter &Writer )
	{
	qRH
		sdr::sRow Row = qNIL;
		str::wString Entry;
	qRB
		Row = RawEntries.First();

		Writer.PushTag( "Entries" );

		Writer.PutAttribute( "Amount", RawEntries.Amount() );

		while ( Row != qNIL ){
			Entry.Init();
			Callback.ToXML( RawEntries( Row ), Entry );
			Writer.PutValue( Entry, "Entry" );

			Row = RawEntries.Next( Row );
		}

		Writer.PopTag();
	qRR
	qRT
	qRE
	}

	void Dump_(
		const fbltyp::dIds &Ids,
		const fbltyp::dIds &Columns,
		const fbltyp::dStringsSet &EntriesSet,
		const fbltyp::dIds &Types,
		sField Current,
		const frdmisc::dXTypes &XTypes,
		xml::dWriter &Writer )
	{
	qRH
		sdr::sRow Row = qNIL;
	qRB
		Row = Ids.First();

		Writer.PushTag( "Fields" );
		xml::PutAttribute("Amount", Ids.Amount(), Writer );

		if ( Current != UndefinedField )
			Writer.PutAttribute("Current", **Current );

		while ( Row != qNIL ) {
			Writer.PushTag( "Field" );

			xml::PutAttribute( "id", *Ids( Row ), Writer );
			xml::PutAttribute( "Column", *Columns( Row ), Writer );

			Dump_( EntriesSet( Row ), XTypes( *Types( Row ) ).Callback(), Writer );

			Writer.PopTag();

			Row = Ids.Next( Row );
		}

		Writer.PopTag();

	qRR
	qRT
	qRE
	}
}

void frdinstc::rUser::DumpCurrentRecordFields( xml::dWriter &Writer ) const
{
qRH
	fbltyp::wIds Fields, Columns, Types;
	fbltyp::wStringsSet EntriesSet;
qRB
	if ( Record_ == UndefinedRecord )
		qRGnr();

	Fields.Init();
	Columns.Init();
	EntriesSet.Init();
	Types.Init();
	Core_.GetRecordFields( Record_, Fields, Columns, EntriesSet, Types );

	Dump_( Fields, Columns, EntriesSet, Types, Field_, Core_.Types(), Writer );
qRR
qRT
qRE
}

void frdinstc::rUser::DumpColumnBuffer( xml::dWriter &Writer ) const
{
qRH
	sType Type = UndefinedType;
	sNumber Number = UndefinedNumber;
	str::wString Label, Comment;
qRB
	Label.Init();
	Comment.Init();

	Core_.GetColumn( Type, Number, Label, Comment );

	Writer.PushTag("Column" );

	Writer.PutAttribute( "Type", **Type, **UndefinedType  );
	Writer.PutAttribute( "Number" , **Number, **UndefinedNumber );
	Writer.PutValue( Label, "Label" );
	Writer.PutValue( Comment, "Comment" );

	Writer.PopTag();
qRR
qRT
qRE
}

void frdinstc::rUser::DumpFieldBuffer( xml::dWriter &Writer ) const
{
	Writer.PushTag( "FieldBuffer" );

	Writer.PopTag();
}

void frdinstc::rUser::DumpFieldBufferCurrentField( xml::dWriter &Writer ) const
{
qRH
	sType Type = UndefinedType;
	sNumber Number = UndefinedNumber;
	wEntrysI1S Entries;
qRB
	Entries.Init();

	Core_.GetCurrentField( Type, Number, Entries );

	Writer.PushTag( "Field" );

	Writer.PutAttribute( "Type", **Type );
	Writer.PutAttribute( "Number", **Number );

	Writer.PushTag("Entries");
	Writer.PutAttribute( "Selected", **Entry_, **UndefinedEntry );

	sclfrntnd::Dump( Entries, "Entry", Writer );

	Writer.PopTag();

	Writer.PopTag();
qRR
qRT
qRE
}

void frdinstc::rUser::DumpRecords( xml::dWriter &Writer ) const
{
qRH
	wDigestsI1S Digests;
qRB
	Digests.Init();
	Core_.GetRecords( Digests );

	sclfrntnd::Dump( Digests, "Records", "Record", Writer );
qRR
qRE
qRT
}


