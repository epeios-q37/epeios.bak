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

// OrGaniZer DaTaBase

#ifndef OGZDTB__INC
# define OGZDTB__INC

# ifdef XXX_DBG
#	define OGZDTB__DBG
# endif

# include "ogzbsc.h"
# include "ogzmta.h"
# include "ogzetr.h"
# include "ogztyp.h"
# include "ogzclm.h"
# include "ogzfld.h"
# include "ogzrcd.h"
# include "ogzusr.h"

namespace ogzdtb {

	class cColumnFeatures
	{
	protected:
		virtual void OGZDTBColumnFeatures(
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dString &Label,
			const str::dString &Comment ) = 0;
	public:
		qCALLBACK( ColumnFeatures );
		void ColumnFeatures(
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			return OGZDTBColumnFeatures( Column, Type, Number, Label, Comment );
		}
	};

	class cFieldEntries
	{
	protected:
		virtual void OGZDTBFieldEntries(
			ogzbsc::sFRow Field,
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dStrings &Entries ) = 0;
	public:
		qCALLBACK( FieldEntries );
		void FieldEntries(
			ogzbsc::sFRow Field,
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dStrings &Entries )
		{
			return OGZDTBFieldEntries( Field, Column, Type, Number, Entries );
		}
	};

	class cRecordRetriever
	{
	protected:
		virtual void OGZDTBRetrieve(
			ogzbsc::sRRow Record,
			ogzbsc::dDatum &Entry )	= 0;
	public:
		qCALLBACK( RecordRetriever );
		void Retrieve(
			ogzbsc::sRRow Record,
			ogzbsc::dDatum &Entry )
		{
			return OGZDTBRetrieve( Record, Entry );
		}
	};

	class mDatabase
	{
	private:
		ogzetr::sRow GetRawEntryRow_(
			ogzusr::sRow User,
			ogzbsc::sERow Row ) const
		{
			return Users.GetRaw( User, Row );
		}
		ogzclm::sRow GetRawColumnRow_(
			ogzusr::sRow User,
			ogzbsc::sCRow Row ) const
		{
			return Users.GetRaw( User, Row );
		}
		ogzfld::sRow GetRawFieldRow_(
			ogzusr::sRow User,
			ogzbsc::sFRow Row ) const
		{
			return Users.GetRaw( User, Row );
		}
		ogzrcd::sRow GetRawRecordRow_(
			ogzusr::sRow User,
			ogzbsc::sRRow Record ) const
		{
			return Users.GetRaw( User, Record );
		}
		void GetMeta_(
			ogzmta::sRow Row,
			str::dString &Meta ) const
		{
			if ( Row != qNIL )
				Metas.Recall( Row, Meta );
		}
		void GetDatum_(
			ogzetr::sRow Row,
			ogztyp::sRow Type,
			str::dString &Datum) const
		{
			if ( Row != qNIL )
				Entries.Recall( Row, Type, Datum );
		}
		void GetColumnFeatures_(
			ogzclm::sRow Column,
			ogztyp::sRow *Type,
			ogzclm::eNumber *Number,
			str::dString *Label,
			str::dString *Comment ) const;
		void GetColumnFeatures_(
			ogzclm::sRow Column,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			return GetColumnFeatures_( Column, &Type, &Number, &Label, &Comment );
		}
		ogztyp::sRow GetType_(
			ogzusr::sRow User,
			const ogzfld::dField &Field ) const;
		void GetColumnFeatures_(
			ogzusr::sRow User,
			const ogzfld::dRows &Fields,
			cColumnFeatures &Callback ) const;
		void GetColumnFeatures_(
			ogzusr::sRow User,
			const ogzrcd::dFields &Fields,
			cColumnFeatures &Callback ) const;
		void GetEntries_(
			ogzusr::sRow User,
			const ogzfld::dField &Field,
			ogzbsc::dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number ) const;
		bso::sBool GetEntries_(
			ogzusr::sRow User,
			ogzfld::sRow Field,
			ogzbsc::dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			qRPC ) const;
		void GetEntries_(
			ogzusr::sRow User,
			const ogzrcd::dFields &RegularFields,
			const ogzfld::dRows &RawFields,
			cFieldEntries &Callback ) const;
		void GetEntries_(
			ogzusr::sRow User,
			const ogzrcd::dFields &Fields,
			cFieldEntries &Callback ) const;
		void Delete_(
			ogzusr::sRow User,
			const ogzfld::dEntries &Entries );
		void Append_(
			ogzusr::sRow User,
			const ogzbsc::dDatum &Datum,
			ogztyp::sRow Type,
			ogzfld::dField &Field );
		void Append_(
			ogzusr::sRow User,
			const ogzbsc::dData &Data,
			ogzfld::dField &Field );
		void GetFirstEntry_(
			ogzusr::sRow User,
			const ogzfld::dField &Field,
			ogzbsc::dDatum &Entry ) const;
		void GetFirstEntry_(
			ogzusr::sRow User,
			ogzbsc::sFRow,
			ogzbsc::dDatum &Entry ) const;
		void GetFirstEntry_(
			ogzusr::sRow User,
			ogzrcd::sRow Record,
			ogzbsc::dDatum &Entry ) const;
		void GetRecords_(
			ogzusr::sRow User,
			const ogzusr::dRecords &Records,
			cRecordRetriever &Callback ) const;
	public:
		ogzmta::mMetas Metas;
		ogzclm::mColumns Columns;
		ogzetr::mEntries Entries;
		ogzfld::mFields Fields;
		ogzrcd::mRecords Records;
		ogzusr::mUsers Users;
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Metas, Columns, Entries, Fields, Records, Users );
		}
		qCDTOR( mDatabase );
		void Init(
			const ogztyp::dTypes &Types,
			ogzmta::cMeta &MTACallback,
			ogzclm::cColumn &CLMCallback,
			ogzetr::cEntry &ETRCallback,
			ogzfld::cField &FLDCallback,
			ogzrcd::cRecord &RCDCallback,
			ogzusr::cUser &USRCallback )
		{
			Metas.Init( MTACallback );
			Columns.Init( CLMCallback );
			Entries.Init( ETRCallback );
			Fields.Init( FLDCallback );
			Records.Init( RCDCallback );
			Users.Init( USRCallback );
		}
		ogzbsc::sRRow NewRecord( ogzusr::sRow User )
		{
			return Users.Add( User, Records.New() );
		}
		ogzbsc::sFRow NewField(
			ogzusr::sRow User,
			const ogzclm::rColumnBuffer &Column,
			ogzbsc::sRRow Record );
		void GetColumnsFeatures(
			ogzusr::sRow User,
			ogzbsc::sRRow Record,
			cColumnFeatures &Callback ) const;
		bso::sBool GetEntries(
			ogzusr::sRow User,
			ogzbsc::sFRow Field,
			ogzbsc::dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			qRPC ) const
		{
			return GetEntries_( User, GetRawFieldRow_( User, Field ), Entries, Type, Number, qRP );
		}
		void GetEntries(
			ogzusr::sRow User,
			ogzbsc::sRRow Record,
			cFieldEntries &Callback	) const;
		bso::sBool UpdateField(	// return 'false' if field doesn't exists.
			ogzusr::sRow User,
			ogzbsc::sFRow Field,
			ogzbsc::dData &Entries,
			qRPC );
		void GetRecords(
			ogzusr::sRow User,
			cRecordRetriever &Callback ) const;
	};

# ifdef M
#  define OGZDTB_M_	M
#  #undef M
# endif

# define M( module, name )\
	protected:\
		virtual module::c##name &OGZDTBGet##name##Callback( void ) = 0;\
	public:\
		module::c##name &Get##name##Callback( void )\
		{\
			return OGZDTBGet##name##Callback();\
		}

	class cDatabase
	{
	protected:
		M( ogzmta, Meta );
		M( ogzclm, Column );
		M( ogzetr, Entry );
		M( ogzfld, Field );
		M( ogzrcd, Record );
		M( ogzusr, User );
	public:
		qCALLBACK( Database );
	};

# ifdef OGZDTB_M_
#  define M OGZDTB_M_
#  undef OGZDTB_M_
# else
#  undef M
# endif

}

#endif
