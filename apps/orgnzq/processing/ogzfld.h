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
		qRMV( ogzclm::sXColumns, C_, Columns_ );
		ogzclm::sRow GetColumn_( sRow Row ) const;
		sRow Create_( ogzclm::sRow Column );
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
			Columns_ = NULL;
		}
		qCVDTOR( sXFields );
		void Init(
			cField &Callback,
			ogzclm::sXColumns &Columns )
		{
			Core_.Init( Callback );
			Columns_ = &Columns;
		}
		ogztyp::sRow GetType( sRow Row ) const
		{
			return C_().GetType( GetColumn_( Row ) );
		}
		sRow Create(
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			return Create_( C_().Create( Type, Number, Label, Comment ) );
		}
		void Recall(
			sRow Row,
			dField &Field ) const
		{
			Core_.Recall( Row, Field );
		}
		void GetColumn(
			sRow Row,
			ogzclm::sColumn &Column ) const
		{
			C_().Recall( GetColumn_( Row ), Column );
		}
		qRODISCLOSEs( sFields, Core );
	};


	typedef ogzcbs::rRegularDynamicCallback<OGZFLD_TP> rRegularFieldCallback;

	typedef ogzcbs::dList<sRow,sLRow> dFieldList;
	qW( FieldList );

	class rFieldBuffer
	: public wField
	{
	private:
		sRow Row_;	// Row of the source field. if  == 'qNIL', new field.
		ogzclm::rColumn Column_;
	public:
		void reset( bso::sBool P = true )
		{
			Row_ = qNIL;
			Column_.reset( P );
		}
		qCDTOR( rFieldBuffer );
		void Init( sRow Row )
		{
			Row_ = Row;
			wField::Init();
			Column_.Init();
		}
		void Set(
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			Column_.Init( Type, Number, Label, Comment );
		}
};
}

#endif
