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

	typedef ogzfld::sXFields sFields_;

	class rRecordBuffer
	: public sFields_
	{
	private:
		sRow Row_;	// Row of the source record. If qNIL, it's a new record.
		ogzdta::rRegularDataCallback DataCallback_;
		ogzclm::rRegularColumnCallback ColumnCallback_;
		ogzfld::rRegularFieldCallback FieldCallback_;
		ogzdta::sData Data_;
	public:
		void reset( bso::sBool P = true )
		{
			Row_ = qNIL;
			Data_.reset( P );
			sFields_::reset( P );
			FieldCallback_.reset( P );
			ColumnCallback_.reset( P );
			DataCallback_.reset( P );
		}
		qCDTOR( rRecordBuffer );
		void Init( ogztyp::sRow TextType )
		{
			DataCallback_.Init();
			ColumnCallback_.Init();
			FieldCallback_.Init();
			Data_.Init( DataCallback_ );
			sFields_::Init( Data_, ColumnCallback_, TextType, FieldCallback_ );
		}
		ogzdta::sRow UpdateDatum(
			ogzdta::sRow DatumRow,	// if == 'qNIL', an entry is created.
			ogzfld::sRow FieldRow,
			const str::dString &Datum );
		qRODISCLOSEr( sRow, Row );
		qRODISCLOSEr( ogzdta::sData, Data );
	};
}


#endif
