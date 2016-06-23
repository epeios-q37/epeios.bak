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

#include "common.h"


#include "wrpcolumn.h"
#include "wrpfield.h"

#include "registry.h"
#include "dir.h"
#include "fnm.h"
#include "ogzinf.h"
#include "sclmisc.h"

using namespace wrpunbound;

using common::rStuff;

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &BaseBackend,\
		fblbkd::rRequest &Request )

DEC( Login )
{
qRH
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
	ogzusr::sRow UserRow = qNIL;
	bso::sBool NEw = false;
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
	void GetNumbers_(
		fbltyp::dId8s &Ids,
		fbltyp::dStrings &Labels )
	{
		int Number = 0;

		while ( Number < ogzclm::n_amount )
		{
			Ids.Append( Number );
			Labels.Append( str::wString( ogzclm::GetLabel( (ogzclm::eNumber)Number ) ) );

			Number++;
		}
	}
}

DEC( GetNumbers )
{
qRH
	fbltyp::wId8s Ids;
	fbltyp::wStrings Labels;
qRB
	Ids.Init();
	Labels.Init();

	GetNumbers_( Ids, Labels );

	Request.Id8sOut() = Ids;
	Request.StringsOut() = Labels;
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

namespace {
	class sColumnFeaturesCallback
	: public ogzdtb::cColumnFeatures
	{
	private:
		fbltyp::dIds
			&Columns_,
			&Types_;
		fbltyp::dId8s &Numbers_;
		str::dStrings
			&Labels_,
			&Comments_;
	protected:
		virtual void OGZDTBColumnFeatures(
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dString &Label,
			const str::dString &Comment ) override
		{
			Columns_.Append( *Column );
			Types_.Append( *Type );
			Numbers_.Append( Number );
			Labels_.Append( Label );
			Comments_.Append( Comment );
		}
	public:
		sColumnFeaturesCallback( fblbrq::rRequest &Request )
		: Columns_( Request.IdsOut() ),
		  Types_( Request.IdsOut() ),
		  Numbers_( Request.Id8sOut() ),
		  Labels_( Request.StringsOut() ),
		  Comments_( Request.StringsOut() )
		{}
	};

	void GetColumns_(
		ogzusr::sRow User,
		ogzbsc::sRRow Record,
		const ogzdtb::mDatabase &Database,
		fblbrq::rRequest &Request )
	{
		sColumnFeaturesCallback Callback( Request );

		if ( !Database.GetColumnsFeatures( User, Record, Callback, qRPU ) )
			REPORT( NoSuchField );
	}
}

DEC( GetRecordColumns )
{
qRH
qRB
	USER;
	DATABASE;

	const ogzbsc::sRRow &Record = *Request.IdIn();

	GetColumns_( User, Record, Database, Request );
qRR
qRT
qRE
}

namespace {
	class sFieldEntriesCallback
	: public ogzdtb::cFieldEntries
	{
	private:
		fbltyp::dIds
			&Fields_,
			&Columns_,
			&Types_;
		fbltyp::dStringsSet &EntriesSet_;
	protected:
		virtual void OGZDTBFieldEntries(
			ogzbsc::sFRow Field,
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dStrings &Entries ) override
		{
			Fields_.Append( *Field );
			Columns_.Append( *Column );
			Types_.Append( *Type );
			EntriesSet_.Append( Entries );

		}
	public:
		sFieldEntriesCallback( fblbrq::rRequest &Request )
		: Fields_( Request.IdsOut() ),
	      Columns_( Request.IdsOut() ),
		  Types_( Request.IdsOut() ),
		  EntriesSet_( Request.StringsSetOut() )
		{}
	};

	void GetRecordFields_(
		ogzusr::sRow User,
		const ogzdtb::mDatabase &Database,
		ogzbsc::sRRow Record,
		fblbrq::rRequest &Request )
	{
		sFieldEntriesCallback Callback( Request );

		Database.GetEntries( User, Record, Callback );
	}
}

DEC( GetRecordFields )
{
qRH
qRB
	STUFF;
	DATABASE;

	GetRecordFields_( Stuff.User(), Database, *Request.IdIn(), Request );
qRR
qRT
qRE
}

DEC( CreateRecord )
{
	USER;
	DATABASE;

	Request.IdOut() = *Database.NewRecord( User );
}

DEC( CreateField )
{
	USER;
	BACKEND;
	DATABASE;

	ogzbsc::sFRow FieldRow = qNIL;
	ogzbsc::sRRow Record = *Request.IdIn();
	const ogzclm::rColumnBuffer &Column = Backend.Object<wrpcolumn::dColumn>( Request.ObjectIn() )();
	const wrpfield::dField &Field = Backend.Object<wrpfield::dField>( Request.ObjectIn() );

	if ( ( FieldRow = Database.Create( User, Column, Record, Field, qRPU ) ) == qNIL )
		REPORT( NoSuchRecord );

	Request.IdOut() = *FieldRow;
}

DEC( UpdateField )
{
	USER;
	BACKEND;
	DATABASE;

	ogzbsc::sFRow FieldRow = *Request.IdIn();
	const wrpfield::dField &Field = Backend.Object<wrpfield::dField>( Request.ObjectIn() );
	bso::sBool FieldErased = false, RecordErased = false;

	if ( FieldRow == qNIL )
		qRGnr();

	if ( Field.Amount() == 0 ) {
		ogzbsc::sRRow Record = Database.GetRecord( User, FieldRow, qRPU );

		if ( Record == qNIL )
			REPORT( NoSuchField );	// Above method really returns 'qNIL' if the field doesn't exists.

		Database.Erase( User, FieldRow );

		FieldErased = true;

		RecordErased = Database.EraseIfEmpty( User, Record );
	} else if ( !Database.Update( User, FieldRow, Field, qRPU ) )
			REPORT( NoSuchField );

	Request.BooleanOut() = FieldErased;
	Request.BooleanOut() = RecordErased;
}

namespace {
	typedef ogzdtb::cRecordRetriever cRecordRetriever_;

	class sRecordRetriever
	: public cRecordRetriever_
	{
	private:
		fbltyp::dIds &Rows_;
		fbltyp::dStrings &Entries_;
	protected:
		void OGZDTBRetrieve(
			ogzbsc::sRRow Record,
			ogzbsc::dDatum &Entry )	override
		{
			if ( Rows_.Append( *Record ) != Entries_.Append( Entry ) )
				qRGnr();
		}
	public:
		sRecordRetriever(
			fbltyp::dIds &Rows,
			fbltyp::dStrings &Entries )
		: Rows_( Rows ),
		  Entries_( Entries )
		{}
	};
}

DEC( GetRecords )
{
qRH
qRB
	STUFF;
	DATABASE;

	fbltyp::dIds &Rows = Request.IdsOut();
	fbltyp::dStrings &Entries = Request.StringsOut();

	sRecordRetriever Callback( Rows, Entries );

	Database.GetRecordsFirstEntry( Stuff.User(), Callback );
qRR
qRT
qRE
}

DEC( MoveField )
{
	USER;
	DATABASE;

	ogzbsc::sRRow Record = *Request.IdIn();
	ogzbsc::sFRow Source = *Request.IdIn(), Target = *Request.IdIn();

	Database.MoveField( User, Record, Source, Target );
}

#define D( name )	OGZINF_UC_SHORT #name, ::name

void wrpunbound::Inform( fblbkd::backend___ &Backend )
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
		fblbkd::cEnd );

	Backend.Add( D( GetTypes ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids.
			fblbkd::cStrings,	// Labels.
			fblbkd::cStrings,	// Plugin ids.
		fblbkd::cEnd );

	Backend.Add( D( GetRecordColumns ),
			fblbkd::cId,		// Id of the record .
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids of the columns,
			fblbkd::cIds,		// Types.
			fblbkd::cId8s,		// Numbers.
			fblbkd::cStrings,	// Labels.
			fblbkd::cStrings,	// Comments.
		fblbkd::cEnd );

	Backend.Add( D( GetRecordFields ),
			fblbkd::cId,			// Record.
		fblbkd::cEnd,
			fblbkd::cIds,			// Fields.
			fblbkd::cIds,			// The column for each fields.
			fblbkd::cIds,			// The type of each field. More convenient to be here due to use of plugins for the types.
			fblbkd::cStringsSet,	// The entries for each field.
		fblbkd::cEnd );

	Backend.Add( D( CreateRecord ),
		fblbkd::cEnd,
			fblbkd::cId,	// The created Record.
		fblbkd::cEnd );

	Backend.Add( D( CreateField ),
			fblbkd::cId,		// Record in which to create the new field.
			fblbkd::cObject,	// Column object.
			fblbkd::cObject,	// Field buffer object to update with.
		fblbkd::cEnd,
			fblbkd::cId,	// The created field.
		fblbkd::cEnd );

	Backend.Add( D( UpdateField ),
			fblbkd::cId,		// Id of the field to update.
			fblbkd::cObject,	// Field buffer object to update with,
		fblbkd::cEnd,
			fblbkd::cBoolean,	// 'true' if the field was erased, because it was empty.
			fblbkd::cBoolean,	// 'true' if the record was erased, because the field was empty and the last one.
		fblbkd::cEnd );

	Backend.Add( D( GetRecords ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids of the record.
			fblbkd::cStrings,	// First entry of each record.
		fblbkd::cEnd );

	Backend.Add( D( MoveField ),
			fblbkd::cId,		// Id of the record owning the fields to move.
			fblbkd::cId,		// Id of yhe field to move.
			fblbkd::cId,		// Id of the field which place is taken by the source field.
		fblbkd::cEnd,
		fblbkd::cEnd );
}

