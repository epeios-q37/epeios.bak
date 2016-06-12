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

DEC( CreateRecord )
{
qRH
qRB
	STUFF;
	DATABASE;

	Request.IdOut() = *Database.NewRecord( Stuff.User() );
qRR
qRT
qRE
}

DEC( CreateField )
{
qRH
qRB
	STUFF;
	DATABASE;

	const ogzbsc::sRRow &Record = *Request.IdIn();
	const ogzclm::rColumnBuffer &Column = Backend.Object<wrpcolumn::dColumn>( Request.ObjectIn() )();

	Request.IdOut() = *Database.NewField( Column, Record, Stuff.User() );
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
		ogzbsc::sRRow Record,
		ogzusr::sRow User,
		const ogzdtb::mDatabase &Database,
		fblbrq::rRequest &Request )
	{
		sColumnFeaturesCallback Callback( Request );

		Database.GetColumnsFeatures( Record, User, Callback );
	}
}

DEC( GetRecordColumns )
{
qRH
qRB
	STUFF;
	DATABASE;

	const ogzbsc::sRRow &Record = *Request.IdIn();

	GetColumns_( Record, Stuff.User(), Database, Request );
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
		const ogzdtb::mDatabase &Database,
		ogzbsc::sRRow Record,
		ogzusr::sRow User,
		fblbrq::rRequest &Request )
	{
		sFieldEntriesCallback Callback( Request );

		Database.GetEntries( Record, User, Callback );
	}
}

DEC( GetRecordFields )
{
qRH
qRB
	STUFF;
	DATABASE;

	GetRecordFields_( Database, *Request.IdIn(), Stuff.User(), Request );
qRR
qRT
qRE
}

DEC( UpdateField )
{
	STUFF;
	DATABASE;

	ogzbsc::sFRow Field = *Request.IdIn();
	const fbltyp::sObject &FieldBuffer = Request.ObjectIn();

	if ( Field == qNIL )
		qRGnr();

	if ( !Database.UpdateField( Field, Stuff.User(), Backend.Object<wrpfield::dField>( FieldBuffer ), qRPU ) )
		REPORT( NoSuchField );
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

	Backend.Add( D( CreateRecord ),
		fblbkd::cEnd,
			fblbkd::cId,	// Id of the new.
		fblbkd::cEnd );

	Backend.Add( D( CreateField ),
			fblbkd::cId,		// Id of the record .
			fblbkd::cObject,	// Id of the column object.
		fblbkd::cEnd,
			fblbkd::cId,		// Id of the created field.
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
			fblbkd::cId,			// Id of the record .
		fblbkd::cEnd,
			fblbkd::cIds,			// Ids of the fields.
			fblbkd::cIds,			// The column for each fields.
			fblbkd::cIds,			// The type of each field. More convenient to be here due to use of plugins for the types.
			fblbkd::cStringsSet,	// The entries for each field.
		fblbkd::cEnd );

	Backend.Add(D( UpdateField ),
			fblbkd::cId,	// ID of the field to update.
			fblbkd::cObject,	// Field buffer object to update with,
		fblbkd::cEnd,
		fblbkd::cEnd );
}

