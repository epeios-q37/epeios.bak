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
# include "ogzdta.h"
# include "ogztyp.h"
# include "ogzclm.h"
# include "ogzfld.h"
# include "ogzrcd.h"
# include "ogzusr.h"

namespace ogzdtb {
	class mDatabase
	{
	private:
		ogzrcd::sRow GetRawRecordRow_(
			ogzbsc::sRRow Record,
			ogzusr::sRow User ) const
		{
			ogzrcd::sRow RawRecordRow = Users.GetRaw( Record, User );

			if ( RawRecordRow == qNIL )
				qRGnr();

			return RawRecordRow;;
		}
		void GetDatum_(
			ogzdta::sRow Row,
			ogztyp::sRow Type,
			str::dString &Datum) const
		{
			if ( Row != qNIL )
				Data.Recall( Row, Type, Datum );
		}
		void GetColumnFeatures_(
			ogzclm::sRow Column,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const;
		ogztyp::sRow GetEntries_(
			const ogzfld::dField &Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries ) const;
		ogztyp::sRow GetEntries_(
			ogzfld::sRow Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries	) const;
		ogztyp::sRow GetEntries_(
			ogzbsc::sFRow Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries	) const
		{
			return GetEntries_( Users.GetRaw( Field, User ), User, Entries );
		}
	public:
		ogzdta::mData Data;
		ogzclm::mColumns Columns;
		ogzfld::mFields Fields;
		ogzrcd::mRecords Records;
		ogzusr::mUsers Users;
		void reset( bso::bool__ P = true )
		{
			Data.reset( P );
			Columns.reset( P );
			Fields.reset( P );
			Records.reset( P );
			Users.reset( P );
		}
		qCDTOR( mDatabase );
		void Init(
			const ogztyp::dTypes &Types,
			ogzdta::cData &DTACallback,
			ogzclm::cColumn &CLMCallback,
			ogzfld::cField &FLDCallback,
			ogzrcd::cRecord &RCDCallback,
			ogzusr::cUser &USRCallback )
		{
			Data.Init( DTACallback );
			Columns.Init( CLMCallback );
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
		void GetFieldColumnFeatures(
			ogzfld::sRow Field,
			ogzclm::sRow &Row,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			Row = Fields.GetColumn( Field );

			GetColumnFeatures_( Row, Type, Number, Label, Comment );
		}
		ogztyp::sRow GetFieldEntries(
			ogzbsc::sFRow Field,
			ogzusr::sRow User,
			ogzbsc::dData &Entries	) const
		{
			return GetEntries_( Field, User, Entries );
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
		M( ogzdta, Data );
		M( ogzclm, Column );
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
