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

	class mDatabase
	{
	private:
		ogzclm::sRow GetRawColumnRow_(
			ogzbsc::sCRow Row,
			ogzusr::sRow User ) const
		{
			return Users.GetRaw( Row, User );
		}
		ogzfld::sRow GetRawFieldRow_(
			ogzbsc::sFRow Row,
			ogzusr::sRow User ) const
		{
			return Users.GetRaw( Row, User );
		}
		ogzrcd::sRow GetRawRecordRow_(
			ogzbsc::sRRow Record,
			ogzusr::sRow User ) const
		{
			return Users.GetRaw( Record, User );
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
			const ogzfld::dField &Field,
			ogzusr::sRow User) const;
		void GetColumnFeatures_(
			const ogzfld::dRows &Fields,
			ogzusr::sRow User,
			cColumnFeatures &Callback ) const;
		void GetColumnFeatures_(
			const ogzrcd::dFields &Fields,
			ogzusr::sRow User,
			cColumnFeatures &Callback ) const;
		void GetEntries_(
			const ogzfld::dField &Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number ) const;
		bso::sBool GetEntries_(
			ogzfld::sRow Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			qRPC ) const;
		void GetEntries_(
			const ogzrcd::dFields &RegularFields,
			const ogzfld::dRows &RawFields,
			ogzusr::sRow User,
			cFieldEntries &Callback ) const;
		void GetEntries_(
			const ogzrcd::dFields &Fields,
			ogzusr::sRow User,
			cFieldEntries &Callback ) const;
		void Delete_(
			const ogzfld::dEntries &Entries,
			ogzusr::sRow User );
		void Append_(
			const ogzbsc::dDatum &Datum,
			ogztyp::sRow Type,
			ogzusr::sRow User,
			ogzfld::dField &Field );
		void Append_(
			const ogzbsc::dData &Data,
			ogzusr::sRow User,
			ogzfld::dField &Field );
	public:
		ogzmta::mMetas Metas;
		ogzclm::mColumns Columns;
		ogzetr::mEntries Entries;
		ogzfld::mFields Fields;
		ogzrcd::mRecords Records;
		ogzusr::mUsers Users;
		void reset( bso::bool__ P = true )
		{
			Metas.reset( P );
			Columns.reset( P );
			Entries.reset( P );
			Fields.reset( P );
			Records.reset( P );
			Users.reset( P );
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
			return Users.Add( Records.New(), User );
		}
		ogzbsc::sFRow NewField(
			const ogzclm::rColumnBuffer &Column,
			ogzbsc::sRRow Record,
			ogzusr::sRow User );
		void GetColumnsFeatures(
			ogzbsc::sRRow Record,
			ogzusr::sRow User,
			cColumnFeatures &Callback ) const;
		bso::sBool GetEntries(
			ogzbsc::sFRow Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			qRPC ) const
		{
			return GetEntries_( GetRawFieldRow_( Field, User ), User, Entries, Type, Number, qRP );
		}
		void GetEntries(
			ogzbsc::sRRow Record,
			ogzusr::sRow User,
			cFieldEntries &Callback	) const;
		bso::sBool UpdateField(	// return 'false' if field doesn't exists.
			ogzbsc::sFRow Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries,
			qRPC );
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
