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

#include "flf.h"
#include "fnm.h"

using namespace frdinstc;

#define C( name )	case v##name : return #name; break
 
const char *frdinstc::GetLabel( eView View )
{
	switch ( View ) {
	C( Records );
	C( Record );
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


void frdinstc::rUser::DumpFieldsColumns( xml::dWriter &Writer ) const
{
qRH
	fbltyp::wIds Ids;
	fbltyp::wIds Types;
	fbltyp::wId8s Numbers;
	str::wStrings Labels;
	str::wStrings Comments;
qRB
	Ids.Init();
	Types.Init();
	Numbers.Init();
	Labels.Init();
	Comments.Init();

	Core_.GetFieldsColumns( Ids, Types, Numbers, Labels, Comments );

	Dump_( Ids, Types, Numbers, Labels, Comments, Writer );
qRR
qRT
qRE
}

 

