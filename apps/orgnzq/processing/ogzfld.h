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

#ifndef OGZFLD_INC_
# define OGZFLD_INC_

# ifdef XXX_DBG
#	define OGZFLD__DBG
# endif

# include "ogzbsc.h"

# include "bch.h"
# include "lstcrt.h"

namespace ogzfld {
	qROW( Row );
	qROWS( Row );

	typedef ogzbsc::dList<ogzbsc::sCRow> dColumns;
	qW( Columns );

	typedef ogzbsc::dList<ogzbsc::sERow> dEntries;
	qW( Entries );

	class dField
	: public dEntries
	{
	public:
		struct s
		: public dEntries::s
		{
			ogzbsc::sCRow Column;
			ogzbsc::sRRow Record;
		}&S_;
		dField( s &S )
		: S_( S ),
		  dEntries( S )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Column = qNIL;
			S_.Record = qNIL;

			dEntries::reset( P );
		}
		void plug( qASd *AS )
		{
			dEntries::plug( AS );
		}
		dField &operator =( const dField &F )
		{
			S_.Column = F.S_.Column;
			S_.Record = F.S_.Record;

			dEntries::operator=( F );

			return *this;
		}
		void Init( void )
		{
			S_.Column = qNIL;
			S_.Record = qNIL;

			dEntries::Init();
		}
		void Init(
			ogzbsc::sCRow Column,
			ogzbsc::sRRow Record )
		{
			S_.Column = Column;
			S_.Record = Record;

			dEntries::Init();
		}
		void RemoveEntries( void )
		{
			dEntries::Init();
		}
		qRWDISCLOSEd( ogzbsc::sCRow, Column );
		qRWDISCLOSEd( ogzbsc::sRRow, Record );
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
			ogzbsc::sRRow Record,
			sRow Field = qNIL ) const;
		ogzbsc::sRRow GetRecord( sRow Field ) const;
	};

	typedef ogzbsc::rRegularCallback<lstcrt::qLCRATEw( dField, sRow ), OGZFLD_TP> rRegularCallback;
}

#endif
