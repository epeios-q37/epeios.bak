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

// OrGaniZer CoLuMn

#ifndef OGZCLM__INC
# define OGZCLM__INC

# ifdef XXX_DBG
#	define OGZCLM__DBG
# endif

# include "ogzbsc.h"
# include "ags.h"
# include "ogztyp.h"
# include "ogzdta.h"

namespace ogzclm {

	qENUM( Number ) {
		nMono,
		nMulti,
		n_amount,
		n_Undefined,
	};

	const char *GetLabel( eNumber Number );

	inline bso::sBool Exists( eNumber Number )
	{
		return Number < n_amount;
	}


	qENUM( Type ) {
		tExplicit,	// Explicit column definition.
		tDeduced,	// Deduced (from a model) column defintion.
		t_amount,
		t_Undefined
	};

	// Deduced column row. Refers to a column row from a model.
	qROW( DRow );

	class sColumn
	{
	private:
		ogztyp::sRow Type_;
		eNumber Number_;
		ogzdta::sRow
			Label_,
			Comment_;
	public:
		void reset( bso::sBool = true )
		{
			Type_ = qNIL;
			Number_ = n_Undefined;
			Label_ = Comment_ = qNIL;
		}
		E_CDTOR( sColumn );
		void Init(
			ogztyp::sRow Type = qNIL,
			eNumber Number = n_Undefined,
			ogzdta::sRow Label = qNIL,
			ogzdta::sRow Comment = qNIL )
		{
			Type_ = Type;
			Number_ = Number;
			Label_ = Label;
			Comment_ = Comment;
		}
		qRODISCLOSEs( ogztyp::sRow, Type );
		qRODISCLOSEs( eNumber, Number );
		qRODISCLOSEs( ogzdta::sRow, Label );
		qRODISCLOSEs( ogzdta::sRow, Comment );

	};

	typedef ogzbsc::sCRow sRow;

	typedef ogzcbs::dList<sRow> dRows;
	qW( Rows );

	// template parameters.
# define OGZCLM_TP	ogzclm::sColumn, ogzclm::sRow

	typedef ogzcbs::cStatic<OGZCLM_TP> cColumn;

	typedef ogzcbs::sStaticItems<OGZCLM_TP> sColumns;

	class sXColumns
	{
	private:
		sColumns Core_;
		ogztyp::sRow TextType_;
		qRMV( ogzdta::sData, D_, Data_ );
		ogzdta::sRow Create_( const str::dString &Text )
		{
			ogzdta::sRow Row = qNIL;

			if ( Text.Amount() != 0 ) {
				Row = D_().New( TextType_ );
				D_().Store( Row, TextType_, Text );
			}

			return Row;
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
			TextType_ = qNIL;
			Data_ = NULL;
		}
		qCVDTOR( sXColumns );
		void Init(
			cColumn &Callback,
			ogztyp::sRow TextType,
			ogzdta::sData &Data )
		{
			Core_.Init( Callback );
			TextType_ = TextType;
			Data_ = &Data;
		}
		qRODISCLOSEs( sColumns, Core );
		qRODISCLOSEs( ogztyp::sRow, TextType );
		ogztyp::sRow GetType( sRow Row );
		sRow Create(
			ogztyp::sRow Type,
			ogzclm::eNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			sRow Row = qNIL;	
			sColumn Column;

			Column.Init( Type, Number, Create_( Label ), Create_( Comment )  );

			Row = Core_.New();

			Core_.Store( Column, Row );

			return Row;
		}
		void Recall(
			sRow Row,
			sColumn &Column ) const
		{
			Core_.Recall( Row, Column );
		}
		void GetColumnsList( dRows &Rows )
		{
			Core_.GetList( 0, 0, Rows );
		}
	};

	typedef ogzcbs::rRegularStaticCallback<OGZCLM_TP> rRegularColumnCallback;
}

#endif
