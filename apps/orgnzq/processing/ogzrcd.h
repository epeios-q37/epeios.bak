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

// OrGaniZer ReCorD

#ifndef OGZRCD__INC
# define OGZRCD__INC

# ifdef XXX_DBG
#	define OGZRCD__DBG
# endif

# include "ogzbsc.h"
# include "ogzcbs.h"
# include "ogzdta.h"
# include "ogzfld.h"

namespace ogzrcd {
	using ogzbsc::sFRow;

	typedef ogzfld::dFieldList dRecord;
	qW( Record );

	typedef ogzbsc::sRRow sRow;

	qROW( IRow );	// Item row.
	qROW( LRow );	// List row.

// Template parameters.
# define OGZRCD_TP	ogzrcd::dRecord,ogzrcd::wRecord,ogzrcd::sRow,ogzrcd::sFRow,ogzfld::sLRow,ogzrcd::sLRow

	typedef ogzcbs::cDynamic<OGZRCD_TP> cRecord;

	typedef ogzcbs::sDynamicItems<OGZRCD_TP> sRecords;

	typedef ogzcbs::rRegularDynamicCallback<OGZRCD_TP> rRegularRecordCallback;

	template <typename callback, typename items> class rCommon_
	: public items
	{
	protected:
		callback Callback_;
	public:
		void reset( bso::sBool P = true )
		{
			items::reset( P );
			Callback_.reset( P );
		}
		E_CDTOR( rCommon_ );
		void Init( void )
		{
			Callback_.Init();
			items::Init( Callback_ );
		}
	};

	typedef ogzclm::sXColumns sColumns_;

	class sColumns
	: public rCommon_<ogzclm::rRegularColumnCallback, sColumns_ >
	{
	public:
		void Init(
			ogztyp::sRow TextType,
			ogzdta::sData &Data )
		{
			Callback_.Init();
			sColumns_::Init( Callback_, TextType, Data );
		}
	};

	typedef ogzfld::sXFields sFields_;

	class sFields
	: public rCommon_<ogzfld::rRegularFieldCallback, sFields_ >
	{
	public:
		void Init( ogzclm::sXColumns &Columns )
		{
			Callback_.Init();
			sFields_::Init( Callback_, Columns );
		}
	};

	class rRecordBuffer
	: public wRecord
	{
	private:
		sRow Row_;	// Row of the source record. If qNIL, it's a new record.
		rCommon_<ogzdta::rRegularDataCallback, ogzdta::sData> Data_;
		sColumns Columns_;
		sFields Fields_;
	public:
		void reset( bso::sBool P = true )
		{
			Row_ = qNIL;
			Data_.reset( P );
			Columns_.reset( P );
			Fields_.reset( P );
		}
		qCDTOR( rRecordBuffer );
		void Init( ogztyp::sRow TextType )
		{
			wRecord::Init();
			Row_ = qNIL;
			Data_.Init();
			Columns_.Init( TextType, Data_ );
			Fields_.Init( Columns_ );
		}
		ogzdta::sRow UpdateDatum(
			ogzdta::sRow DatumRow,	// if == 'qNIL', an entry is created.
			ogzfld::sRow FieldRow,
			const str::dString &Datum );
		qRODISCLOSEr( sRow, Row );
		const ogzdta::sData &Data( void ) const
		{
			return Data_;
		}
		const ogzclm::sColumns &Columns( void ) const
		{
			return Columns_.Core();
		}
		const ogzfld::sFields &Fields( void ) const
		{
			return Fields_.Core();
		}
		void GetColumn(
			ogzfld::sLRow Row,
			ogzclm::sColumn &Column ) const
		{
			Fields_.GetColumn( Get( Row ), Column );
		}
		void GetColumnContent(
			ogzfld::sLRow Row,
			ogztyp::sRow &Type,
			ogzclm::eNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const;
	};
}


#endif
