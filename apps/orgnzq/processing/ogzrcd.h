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
	using ogzbsc::fFRow;
	using ogzbsc::fRRow;

	typedef ogzfld::vFieldList vRecord;
	qW( Record );

// Template parameters.
# define OGZRCD_TP	ogzrcd::vRecord, ogzrcd::iRecord, ogzrcd::fRRow, ogzrcd::fFRow, sdr::fRow

typedef ogzcbs::fDCallback<OGZRCD_TP> fCallback;

	typedef ogzcbs::fDItems<OGZRCD_TP> fRecords;

	typedef ogzcbs::rDRegularCallback<OGZRCD_TP> rRegularCallback;

	template <typename callback, typename items> class rCommon_
	: public items
	{
	private:
		callback Callback_;
	public:
		void reset( bso::fBool P = true )
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

	class rRecordBuffer
	: public iRecord
	{
	private:
		fRRow Id_;	// Row of the source record. If qNIL, it's a new record.
	public:
		rCommon_<ogzdta::rRegularCallback, ogzdta::fData> Data;
		rCommon_<ogzclm::rRegularCallback, ogzclm::fColumns> Columns;
		rCommon_<ogzfld::rRegularCallback, ogzfld::fFields> Fields;
		void reset( bso::fBool P = true )
		{
			Id_ = qNIL;
			Data.reset( P );
			Columns.reset( P );
			Fields.reset( P );
		}
		qCDTOR( rRecordBuffer );
		void Init( void )
		{
			Id_ = qNIL;
			Data.Init();
			Columns.Init();
			Fields.Init();
		}
		qRODISCLOSEr( fRRow, Id );
	};
}


#endif
