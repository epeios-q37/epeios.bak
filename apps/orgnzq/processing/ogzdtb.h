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

#ifndef OGZDTB_INC_
# define OGZDTB_INC_

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
	using ogzbsc::sCRow;	// Column row.
	using ogzbsc::sERow;	// Entry row.
	using ogzbsc::sFRow;	// Field row.
	using ogzbsc::sMRow;	// Meta row.
	using ogzbsc::sRRow;	// Record row.
	using ogzbsc::sURow;	// User row.

	using ogzbsc::dDatum;
	using ogzbsc::wDatum;

	using ogzbsc::dData;
	using ogzbsc::wData;

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
			sFRow Field,
			ogzclm::sRow Column,
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dStrings &Entries ) = 0;
	public:
		qCALLBACK( FieldEntries );
		void FieldEntries(
			sFRow Field,
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
			sRRow Record,
			dDatum &Entry )	= 0;
	public:
		qCALLBACK( RecordRetriever );
		void Retrieve(
			sRRow Record,
			dDatum &Entry )
		{
			return OGZDTBRetrieve( Record, Entry );
		}
	};

	class mDatabase
	{
	private:
		ogzmta::sRow GetRaw_(
			sURow User,
			sMRow Row,
			qRPD ) const
		{
			return Users.GetRaw( User, Row, qRP );
		}
		ogzetr::sRow GetRaw_(
			sURow User,
			sERow Row,
			qRPD ) const
		{
			return Users.GetRaw( User, Row, qRP );
		}
		ogzclm::sRow GetRaw_(
			sURow User,
			sCRow Row,
			qRPD ) const
		{
			return Users.GetRaw( User, Row, qRP );
		}
		ogzfld::sRow GetRaw_(
			sURow User,
			sFRow Row,
			qRPD ) const
		{
			return Users.GetRaw( User, Row, qRP );
		}
		ogzrcd::sRow GetRaw_(
			sURow User,
			sRRow Record,
			qRPD ) const
		{
			return Users.GetRaw( User, Record, qRP );
		}
		void GetMeta_(
			ogzmta::sRow Row,
			str::dString &Meta ) const
		{
			if ( Row != qNIL )
				Metas.Recall( Row, Meta );
		}
		void GetMeta_(
			sURow User,
			sMRow Row,
			str::dString &Meta ) const
		{
			if ( Row != qNIL )
				GetMeta_( GetRaw_( User, Row ), Meta  );
		}
		void GetDatum_(
			ogzetr::sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum) const
		{
			if ( Row != qNIL )
				Entries.Recall( Row, Type, Datum );
		}
		void GetColumnFeatures_(
			sURow User, 
			ogzclm::sRow Column,
			ogztyp::sRow *Type,
			ogzclm::eNumber *Number,
			str::dString *Label,
			str::dString *Comment ) const;
		void GetColumnFeatures_(
			sURow User, 
			ogzclm::sRow Column,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			return GetColumnFeatures_( User, Column, &Type, &Number, &Label, &Comment );
		}
		ogztyp::sRow GetType_(
			sURow User,
			const ogzfld::dField &Field ) const;
		void GetColumnFeatures_(
			sURow User,
			const ogzfld::dRows &Fields,
			cColumnFeatures &Callback ) const;
		void GetColumnFeatures_(
			sURow User,
			const ogzrcd::dFields &Fields,
			cColumnFeatures &Callback ) const;
		void GetEntries_(
			sURow User,
			const ogzfld::dField &Field,
			dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number ) const;
		void GetEntries_(
			sURow User,
			ogzfld::sRow Field,
			dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number ) const;
		void GetEntries_(
			sURow User,
			const ogzrcd::dFields &RegularFields,
			const ogzfld::dRows &RawFields,
			cFieldEntries &Callback ) const;
		void GetEntries_(
			sURow User,
			const ogzrcd::dFields &Fields,
			cFieldEntries &Callback ) const;
		void Delete_(
			sURow User,
			const ogzfld::dEntries &Entries ) const;
		void Append_(
			sURow User,
			const dDatum &Datum,
			ogztyp::sRow Type,
			ogzfld::dField &Field ) const;
		void Append_(
			sURow User,
			const dData &Data,
			ogzfld::dField &Field ) const;
		void GetFirstEntry_(
			sURow User,
			const ogzfld::dField &Field,
			dDatum &Entry ) const;
		void GetFirstEntry_(
			sURow User,
			sFRow,
			dDatum &Entry ) const;
		void GetFirstEntry_(
			sURow User,
			ogzrcd::sRow Record,
			dDatum &Entry ) const;
		void GetRecordsFirstEntry_(
			sURow User,
			const ogzusr::dRecords &Records,
			cRecordRetriever &Callback ) const;
		sFRow Create_(
			sURow User,
			const ogzclm::rColumnBuffer &Column,
			sRRow Record,
			qRPD ) const;
		bso::sBool Update_(	// Returns 'false' if field doesn't exists.
			ogzfld::sRow Field,
			dData &Entries,
			qRPN );
		void Erase_(
			ogzusr::sRow User,
			ogzfld::sRow Field ) const;
		void Erase_(
			ogzusr::sRow User,
			ogzclm::sRow Column ) const;
		void Erase_(
			ogzusr::sRow User,
			ogzmta::sRow Meta ) const
		{
			Metas.Delete( Meta );
		}
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
		sRRow NewRecord( sURow User )
		{
			return Users.Add( User, Records.New() );
		}
		bso::sBool GetColumnsFeatures(	// Returns 'false' if 'Record' does not exist.
			sURow User,
			sRRow Record,
			cColumnFeatures &Callback,
			qRPD ) const;
		bso::sBool GetEntries(
			sURow User,
			sFRow Field,
			dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			qRPD ) const
		{
			ogzfld::sRow RawField = GetRaw_( User, Field, qRP );

			if ( RawField == qNIL )
				return false;
			else {
				GetEntries_( User, RawField, Entries, Type, Number );
				return true;
			}
		}
		bso::sBool GetEntries(
			sURow User,
			sFRow Field,
			dData &Entries,
			qRPD ) const
		{
			ogztyp::sRow Type = qNIL;
			ogzclm::eNumber Number = ogzclm::n_Undefined;

			return GetEntries( User, Field, Entries, Type, Number, qRP );
		}
		bso::sBool GetEntries(	// Returns 'false' if 'Record' doesn't exists.
			sURow User,
			sRRow Record,
			cFieldEntries &Callback,
			qRPD ) const;
		void GetRecordsFirstEntry(
			sURow User,
			cRecordRetriever &Callback ) const;
		sRRow GetRecord(	// Returns 'qNIL' if field doesn't exists.
			sURow User,
			sFRow Field,
			qRPD  ) const;
		bso::sBool Erase(	// Returns 'false' if field does not exists.
			sURow User,
			sFRow Field,
			qRPD ) const
		{
			ogzfld::sRow RawField = GetRaw_( User, Field, qRP );

			if ( RawField == qNIL )
				return false;
			else {
				Records.Erase( Field, GetRaw_( User, GetRecord( User, Field ) ) );
				Erase_( User, RawField );

				return true;
			}
		}
		bso::sBool EraseIfEmpty(	// Returns 'true' if record erased.
			sURow User,
			sRRow Record ) const
		{
			ogzrcd::sRow RawRecord = GetRaw_( User, Record );

			if ( Records.IsEmpty( RawRecord ) ) {
				Records.Erase( GetRaw_( User, Record ) );
				Users.Erase( User, Record );
				return true;
			} else
				return false;
		}
		sFRow Create(	// Returns 'qNIL' if the record doesn't exist.
			sURow User,
			const ogzclm::rColumnBuffer &Column,
			sRRow Record,
			const dData &Entries,
			qRPD ) const;
		bso::sBool Update(	// Returns 'false' if 'Field' doesn't exists.
			sURow User,
			sFRow Field,
			const dData &Entries,
			qRPD ) const;
		void MoveField(
			sURow User,
			sRRow Record,
			sFRow Source,
			sFRow Target ) const;
		bso::sBool MetaExists(
			sURow User,
			ogzmta::eTarget Target,
			const str::dString &Pattern ) const
		{
			return Metas.Search( User, Target, Pattern ) != qNIL;
		}
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
