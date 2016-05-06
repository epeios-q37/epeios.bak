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

// OrGaniZer FieLD

#ifndef OGZFLD__INC
# define OGZFLD__INC

# ifdef XXX_DBG
#	define OGZFLD__DBG
# endif

# include "ogzbsc.h"
# include "ogzcbs.h"
# include "ogzclm.h"
# include "ogzdta.h"

# include "bch.h"

namespace ogzfld {
	using ogzbsc::sDRow;
	using ogzbsc::sTRow;
	using ogzbsc::sCRow;

	typedef ogzbsc::sFRow sRow;

	using ogzdta::dDatumList;

	class dField
	: public dDatumList
	{
	public:
		struct s
		: public dDatumList::s
		{
			sCRow Column;
		}&S_;
		dField( s &S )
		: S_( S ),
		  dDatumList( S )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Column = qNIL;
			dDatumList::reset( P );
		}
		void plug( ogzdta::cHook &Hook )
		{
			dDatumList::plug( Hook );
		}
		void plug( qASd *AS )
		{
			dDatumList::plug( AS );
		}
		dField &operator =( const dField &F )
		{
			S_.Column = F.S_.Column;
			dDatumList::operator=( F );

			return *this;
		}
		void Init( ogzclm::sRow Column = qNIL )
		{
			S_.Column = Column;
			dDatumList::Init();
		}
		qRWDISCLOSEd( sCRow, Column );
	};

	qW( Field );

	qROW( IRow );	// Item row.
	qROW( LRow );	// List row.

	// template parameters.
# define OGZFLD_TP	ogzfld::dField, ogzfld::wField, ogzfld::sRow, ogzfld::sDRow, sdr::sRow, ogzfld::sLRow

	typedef ogzcbs::cDynamic<OGZFLD_TP> cField;

	typedef ogzcbs::sDynamicItems<OGZFLD_TP> sFields;

	typedef ogzcbs::dList<ogzfld::sRow, ogzfld::sLRow> dRows;
	qW( Rows );

	class sXFields
	{
	private:
		sFields Core_;
		ogzclm::sXColumns Columns_;
		qRMV( ogzdta::sData, D_, Data_ );
		sRow Create_( ogzclm::sRow Column );
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
			Columns_.reset( P );
			Data_ = NULL;
		}
		qCVDTOR( sXFields );
		void Init(
			ogzdta::sData &Data,
			ogzclm::cColumn &ColumnCallback,
			ogztyp::sRow TextType,
			cField &FieldCallback )
		{
			Data_ = &Data;
			Columns_.Init( ColumnCallback, TextType, Data );
			Core_.Init( FieldCallback );
		}
		sRow Create( const ogzclm::rColumnBuffer &Column )
		{
			return Create_( Columns_.Create( Column ) );
		}
		void GetFeatures(
			sRow Row,
			ogzclm::sRow &Column,
			str::dStrings &Entries ) const;
		qRODISCLOSEs( sFields, Core );
		qRODISCLOSEs( ogzclm::sXColumns, Columns );
	};


	typedef ogzcbs::rRegularDynamicCallback<OGZFLD_TP> rRegularFieldCallback;

	typedef ogzcbs::dList<sRow,sLRow> dFieldList;
	qW( FieldList );

	class rFieldBuffer
	: public wField
	{
	private:
		sRow Row_;	// Row of the source field. if  == 'qNIL', new field.
		ogzclm::sRow ColumnRow_;	// Row of the column.
	public:
		void reset( bso::sBool  = true )
		{
			Row_ = qNIL;
			ColumnRow_ = qNIL;
		}
		qCDTOR( rFieldBuffer );
		void Init(
			ogzclm::sRow ColumnRow = qNIL,
			sRow Row = qNIL )
		{
			Row_ = Row;
			wField::Init();
			ColumnRow_ = ColumnRow;
		}
	};
}

#endif
