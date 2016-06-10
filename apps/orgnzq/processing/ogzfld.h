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
# include "ogzclm.h"

# include "bch.h"

namespace ogzfld {
	qROW( Row );
	qROWS( Row );

	typedef ogzbsc::dList<ogzbsc::sCRow> dColumns;

	typedef ogzbsc::dList<ogzbsc::sDRow> dData;

	class dField
	: public dData
	{
	public:
		struct s
		: public dData::s
		{
			ogzbsc::sCRow Column;
		}&S_;
		dField( s &S )
		: S_( S ),
		  dData( S )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Column = qNIL;
			dData::reset( P );
		}
		void plug( qASd *AS )
		{
			dData::plug( AS );
		}
		dField &operator =( const dField &F )
		{
			S_.Column = F.S_.Column;
			dData::operator=( F );

			return *this;
		}
		void Init( ogzbsc::sCRow Column = qNIL )
		{
			S_.Column = Column;
			dData::Init();
		}
		void RemoveEntries( void )
		{
			dData::Init();
		}
		qRWDISCLOSEd( ogzbsc::sCRow, Column );
	};

	qW( Field );
}

// template parameters.
# define OGZFLD_TP	ogzfld::dField, ogzfld::sRow
namespace ogzfld {
	typedef ogzbsc::cCommon<OGZFLD_TP> cField;

	typedef ogzbsc::mItems<OGZFLD_TP> mFields_;

	class mFields
	: public mFields_
	{
	public:
		ogzbsc::sCRow GetColumn( sRow Field ) const;
		sRow New(
			ogzbsc::sCRow Column,
			sRow Field = qNIL ) const;
	};

	typedef ogzbsc::rRegularCallback<lstctn::qLCONTAINERw( dField, sRow ), OGZFLD_TP> rRegularCallback;
}

#endif
