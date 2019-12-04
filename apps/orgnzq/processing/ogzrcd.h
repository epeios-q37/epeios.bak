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

#ifndef OGZRCD_INC_
# define OGZRCD_INC_

# ifdef XXX_DBG
#	define OGZRCD__DBG
# endif

# include "ogzbsc.h"

# include "lstcrt.h"

namespace ogzrcd {
	qROW( Row );
	qROWS( Row );

	typedef ogzbsc::dList<ogzbsc::sFRow> dFields;
	qW( Fields );

	typedef dFields dRecord;
	qW( Record );
}

// template parameters.
# define OGZRCD_TP	ogzrcd::dRecord, ogzrcd::sRow
namespace ogzrcd {
	typedef ogzbsc::cCommon<OGZRCD_TP> cRecord;

	typedef ogzbsc::mItems<OGZRCD_TP> mRecords_;

	class mRecords
	: public mRecords_
	{
	public:
		void Add(
			ogzbsc::sFRow Field,
			sRow Record ) const;
		bso::sBool Erase(	// Returns 'false' if 'Field' not owned by 'Record'.
			ogzbsc::sFRow Field,
			sRow Record,
			qRPD ) const;
		void Erase( sRow Record ) const
		{
			mRecords_::Erase( Record );
		}
		bso::sBool IsEmpty( sRow Record ) const;
	};

	typedef ogzbsc::rRegularCallback<lstcrt::qLCRATEw( dRecord, sRow ), OGZRCD_TP> rRegularCallback;
}

#endif
