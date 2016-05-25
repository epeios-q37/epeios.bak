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
	STUFF;
	DATABASE;

	const ogzbsc::sRRow &Record = *Request.IdIn();
	const fbltyp::sObject &Column = Request.ObjectIn();

	Request.IdOut() = *Database.NewField( Backend.Object<wrpcolumn::dColumn>( Column )(), Record, Stuff.User() );
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

		Database.GetColumnFeatures( Record, User, Callback );
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

	Backend.Add( D( GetRecordFields ),
			fblbkd::cId,			// Id of the record .
		fblbkd::cEnd,
			fblbkd::cIds,			// Ids of the fields.
			fblbkd::cIds,			// The column for each fields.
			fblbkd::cIds,			// The type of each field. More convenient to be here due to use of plugins for the types.
			fblbkd::cStringsSet,	// The entries for each field.
		fblbkd::cEnd );
}

