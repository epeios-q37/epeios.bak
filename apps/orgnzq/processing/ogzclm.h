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

#ifndef OGZCLM_INC_
# define OGZCLM_INC_

# ifdef XXX_DBG
#	define OGZCLM__DBG
# endif

# include "ogzbsc.h"
# include "ags.h"
# include "ogztyp.h"

namespace ogzclm {
	typedef ogzbsc::dList<ogzbsc::sMRow> dMetas;

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

	qROW( Row );
	qROWS( Row );

	class sColumnCore_
	{
	private:
		ogztyp::sRow Type_;
		eNumber Number_;
	public:
		void reset( bso::sBool = true )
		{
			Type_ = qNIL;
			Number_ = n_Undefined;
		}
		E_CDTOR( sColumnCore_ );
		void Init(
			ogztyp::sRow Type = qNIL,
			eNumber Number = n_Undefined )
		{
			Type_ = Type;
			Number_ = Number;
		}
		qRODISCLOSEs( ogztyp::sRow, Type );
		qRODISCLOSEs( eNumber, Number );

	};

	class sColumn
	: public sColumnCore_
	{
	private:
		ogzbsc::sMRow
			Label_,
			Comment_;
	public:
		void reset( bso::sBool P = true )
		{
			sColumnCore_::reset( P );
			Label_ = Comment_ = qNIL;
		}
		E_CDTOR( sColumn );
		void Init(
			ogztyp::sRow Type = qNIL,
			eNumber Number = n_Undefined,
			ogzbsc::sMRow Label = qNIL,
			ogzbsc::sMRow Comment = qNIL )
		{
			sColumnCore_::Init( Type, Number );
			Label_ = Label;
			Comment_ = Comment;
		}
		qRODISCLOSEs( ogzbsc::sMRow, Label );
		qRODISCLOSEs( ogzbsc::sMRow, Comment );
	};
}

// template parameters.
# define OGZCLM_TP	ogzclm::sColumn, ogzclm::sRow

namespace ogzclm {
	typedef ogzbsc::cCommon<OGZCLM_TP> cColumn;

	typedef ogzbsc::mItems<OGZCLM_TP> mColumns_;

	class mColumns
	: public mColumns_
	{
	public:
		sRow New(
			ogztyp::sRow Type,
			eNumber Number,
			ogzbsc::sMRow Label,
			ogzbsc::sMRow Comment,
			sRow Column = qNIL ) const;
		bso::sBool GetTypeAndNumber(
			sRow Column,
			ogztyp::sRow &Type,
			eNumber &Number,
			qRPD ) const;
	};


	typedef ogzbsc::rRegularCallback<lstbch::qLBUNCHw( sColumn, sRow ), OGZCLM_TP> rRegularCallback;

	class rColumnBuffer
	: public sColumnCore_
	{
	private:
		str::wString
			Label_,
			Comment_;
	public:
		void reset( bso::sBool P = true )
		{
			sColumnCore_::reset( P );
			Label_.reset( P );
			Comment_.reset( P );
		}
		E_CDTOR( rColumnBuffer );
		void Init(
			ogztyp::sRow Type = qNIL,
			eNumber Number = n_Undefined,
			const str::dString &Label = str::wString(),
			const str::dString &Comment = str::wString() )
		{
			sColumnCore_::Init( Type, Number );
			Label_.Init( Label );
			Comment_.Init( Comment );
		}
		qRODISCLOSEr( str::dString, Label );
		qRODISCLOSEr( str::dString, Comment );

	};
}

#endif
