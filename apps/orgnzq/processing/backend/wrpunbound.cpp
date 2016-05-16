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

#include "wrpunbound.h"

#include "wrpcolumn.h"

#include "registry.h"
#include "dir.h"
#include "fnm.h"
#include "ogzinf.h"
#include "sclmisc.h"

using namespace wrpunbound;

#define REPORT( message ) sclmisc::ReportAndAbort( message )

using common::rStuff;

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &,\
		fblbkd::untyped_module &,\
		fblbkd::index__,\
		fblbkd::command__,\
		fblbkd::rRequest &Request,\
		bso::bool__ &,\
		void *UP )

DEC( Login )
{
qRH
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
	ogzusr::sRow UserRow = qNIL;
qRB
	STUFF;
	AUTHENTICATION;

	const str::dString &Username = Request.StringIn();
	const str::dString &Password = Request.StringIn();

	UserRow = Authentication.Authenticate( Username, Password );

	if ( UserRow != qNIL )
		Stuff.SetUser( UserRow );

	Request.BooleanOut() = UserRow != qNIL;
qRR 
qRT
qRE
}

namespace {
	const str::dString &GetWording_( ogzclm::eNumber Number )
	{
		static str::wString Wording;
	qRH
		str::wString RawWording;
	qRB
		RawWording.Init();
		RawWording.Append( ogzclm::GetLabel( Number ) );
		RawWording.Append( "NumberWording" );

		Wording.Init();
		sclmisc::GetBaseTranslation( RawWording, Wording );
	qRR
	qRT
	qRE
		return Wording;
	}

	void GetNumbers_(
		fbltyp::dId8s &Ids,
		fbltyp::dStrings &Labels,
		fbltyp::dStrings &Wordings )
	{
		int Number = 0;

		while ( Number < ogzclm::n_amount )
		{
			Ids.Append( Number );
			Labels.Append( str::wString( ogzclm::GetLabel( (ogzclm::eNumber)Number ) ) );
			Wordings.Append( GetWording_( (ogzclm::eNumber)Number ) );

			Number++;
		}
	}
}

DEC( GetNumbers )
{
qRH
	fbltyp::wId8s Ids;
	fbltyp::wStrings Labels, Wordings;
qRB
	Ids.Init();
	Labels.Init();
	Wordings.Init();

	GetNumbers_( Ids, Labels, Wordings );

	Request.Id8sOut() = Ids;
	Request.StringsOut() = Labels;
	Request.StringsOut() = Wordings;
qRR 
qRT
qRE
}

namespace {
	void GetType_(
		const ogztyp::sType &Type,
		fbltyp::strings_ &Labels,
		fbltyp::strings_ &PluginIds )
	{
		Labels.Add( str::wString( Type.GetLabel() ) );
		PluginIds.Append(str::wString( Type.GetIdentifier() ) );
	}

	void GetTypes_(
		const ogztyp::dTypes &Types,
		fbltyp::ids_ &Ids,
		fbltyp::strings_ &Labels,
		fbltyp::strings_ &PluginIds )
	{
		ogztyp::sRow Row = Types.First();

		while ( Row != qNIL ) {
			GetType_( Types( Row ), Labels, PluginIds );

			Ids.Add( *Row );

			Row = Types.Next( Row );
		}

	}
}

DEC( GetTypes )
{
qRH
qRB
	fbltyp::dIds &Ids = Request.IdsOut();
	fbltyp::dStrings &Labels = Request.StringsOut();
	fbltyp::dStrings &PluginIds = Request.StringsOut();

	GetTypes_( common::GetTypes(), Ids, Labels, PluginIds );
qRR 
qRT
qRE
}

DEC( DefineRecord )
{
qRH
qRB
	STUFF;
	DATABASE;

	if ( Request.IdIn() != fbltyp::UndefinedId )
		qRVct();

	Request.IdOut() = *Database.NewRecord( Stuff.User() );
qRR
qRT
qRE
}

DEC( CreateField )
{
qRH
qRB
	BACKEND;
	STUFF;
	DATABASE;

	const ogzusr::sRRow &Record = *Request.IdIn();
	const fbltyp::sObject &Column = Request.ObjectIn();

	Request.IdOut() = *Database.NewField( Stuff.User(), Record, Backend.Object<wrpcolumn::dColumn>( Column )() );
qRR
qRT
qRE
}

namespace {
	void GetColumnFeatures_(
		ogzfld::sRow Field,
		const ogzdtb::mDatabase &Database,
		ogzclm::sRow &Column,
		ogztyp::sRow &Type,
		ogzclm::eNumber &Number,
		str::dString &Label,
		str::dString &Comment )
	{
		Database.GetFieldColumnFeatures( Field, Column, Type, Number, Label, Comment );
	}

	void GetColumns_(
		const ogzrcd::dFields &Fields,
		fbltyp::dIds &Columns,
		fbltyp::dIds &Types,
		fbltyp::dId8s &Numbers,
		str::dStrings &Labels,
		str::dStrings &Comments )
	{
	qRH
		ogzrcd::sFRow Row = qNIL;
		ogzclm::sRow Column = qNIL;
		ogztyp::sRow Type = qNIL;
		ogzclm::eNumber Number = ogzclm::n_Undefined;
		str::wString Label, Comment;
	qRB
		DATABASE;

		Row = Fields.First();

		while ( Row != qNIL ) {
			Label.Init();
			Comment.Init();

			GetColumnFeatures_( Fields( Row ), Database, Column, Type, Number, Label, Comment );

			if ( Columns.Search( *Column ) == qNIL ) {
				Columns.Append( *Column );
				Types.Append( *Type );
				Numbers.Append( Number );
				Labels.Append( Label );
				Comments.Append( Comment );
			}

			Row = Fields.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

DEC( GetRecordColumns )
{
qRH
	ogzrcd::wFields Fields;
qRB
	STUFF;
	DATABASE;

	const ogzusr::sRRow &Record = *Request.IdIn();

	Fields.Init();
	Database.GetRawFieldRows( Stuff.User(), Record, Fields );

	fbltyp::dIds &Columns = Request.IdsOut();
	fbltyp::dIds &Types = Request.IdsOut();
	fbltyp::dId8s &Numbers = Request.Id8sOut();
	str::dStrings &Labels = Request.StringsOut();
	str::dStrings &Comments = Request.StringsOut();

	GetColumns_( Fields, Columns, Types, Numbers, Labels, Comments );
qRR
qRT
qRE
}

namespace {
	void GetFieldFeatures_(
		ogzfld::sRow Field,
		const ogzdtb::mDatabase &Database,
		ogzclm::sRow &Column,
		ogztyp::sRow &Type,
		str::dStrings &Entries )
	{
		Type = Database.GetFieldEntries( Field, Column, Entries );
	}

	void GetColumns_(
		const ogzrcd::dFields &Fields,
		fbltyp::dIds &FieldIds,
		fbltyp::dIds &Columns,
		fbltyp::dStringsSet &EntriesSet,
		fbltyp::dIds &Types )
	{
	qRH
		ogzrcd::sFRow Row = qNIL;
		ogzclm::sRow Column = qNIL;
		ogztyp::sRow Type = qNIL;
		str::wStrings Entries;
	qRB
		DATABASE;

		Row = Fields.First();

		while ( Row != qNIL ) {
			Label.Init();
			Comment.Init();

			GetColumnFeatures_( Fields( Row ), Database, Column, Type, Number, Label, Comment );

			if ( Columns.Search( *Column ) == qNIL ) {
				Columns.Append( *Column );
				Types.Append( *Type );
				Numbers.Append( Number );
				Labels.Append( Label );
				Comments.Append( Comment );
			}

			Row = Fields.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

DEC( GetRecordFields )
{
qRH
	ogzrcd::wFields Fields;
qRB
	STUFF;
	DATABASE;

	const ogzusr::sRRow &Record = *Request.IdIn();

	Fields.Init();
	Database.GetRawFieldRows( Stuff.User(), Record, Fields );

	fbltyp::dIds &Fields = Request.IdsOut();
	fbltyp::dIds &Columns = Request.IdsOut();
	fbltyp::dStringsSet &EntriesSet = Request.StringsSetOut();
	fbltyp::dIds &Types = Request.IdsOut();
qRR
qRT
qRE
}

#define D( name )	OGZINF_UC_SHORT #name, ::name

void wrpunbound::Inform(
	fblbkd::backend___ &Backend,
	rStuff &Stuff )
{
	Backend.Add( D( Login ),
			fblbkd::cString,	// Username.
			fblbkd::cString,	// Password.
		fblbkd::cEnd,
			fblbkd::cBoolean,	// Success.
		fblbkd::cEnd );

	Backend.Add( D( GetNumbers ),
		fblbkd::cEnd,
			fblbkd::cId8s,		// Ids.
			fblbkd::cStrings,	// Labels,
			fblbkd::cStrings,	// Wordings,
		fblbkd::cEnd );

	Backend.Add( D( GetTypes ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids.
			fblbkd::cStrings,	// Labels.
			fblbkd::cStrings,	// Plugin ids.
		fblbkd::cEnd );

	Backend.Add( D( DefineRecord ),
			fblbkd::cId,	// Id ot the record to define. New one if undefined.
		fblbkd::cEnd,
			fblbkd::cId,	// Id ot the record to be defiend.
		fblbkd::cEnd );

	Backend.Add( D( CreateField ),
			fblbkd::cId,	// Id of the record .
			fblbkd::cObject,	// Id of the column object.
		fblbkd::cEnd,
			fblbkd::cId,	// Id of the created field.
		fblbkd::cEnd );

	Backend.Add( D( GetRecordColumns ),
			fblbkd::cId,	// Id of the record .
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids of the columns,
			fblbkd::cIds,		// Types.
			fblbkd::cId8s,		// Numbers.
			fblbkd::cStrings,	// Labels.
			fblbkd::cStrings,	// Comments.
		fblbkd::cEnd );

	Module.Add( D( GetRecordFields ),
			fblbkd::cId,			// Id of the record .
		fblbkd::cEnd,
			fblbkd::cIds,			// Ids of the fields.
			fblbkd::cIds,			// The column for each fields.
			fblbkd::cStringsSet,	// The entries for each field.
			fblbkd::cIds,			// The type of each field. More convenient to be here due to use of plugins for the types.
		fblbkd::cEnd );
}

