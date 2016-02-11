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
	using ogzbsc::fDRow;
	using ogzbsc::fTRow;
	using ogzbsc::fFRow;
	using ogzclm::fCRow;

	using ogzdta::vDatumList;

	class vField
	: public vDatumList
	{
	public:
		struct s
		: public vDatumList::s
		{
			fCRow Column;
		}&S_;
		vField( s &S )
		: S_( S ),
		  vDatumList( S )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Column = qNIL;
			vDatumList::reset( P );
		}
		void plug( qSDf &SD )
		{
			vDatumList::plug( SD );
		}
		void plug( qASv &AS )
		{
			vDatumList::plug( AS );
		}
		vField &operator =( const vField &F )
		{
			S_.Column = F.S_.Column;
			vDatumList::operator=( F );

			return *this;
		}
		void Init( void )
		{
			S_.Column = qNIL;
			vDatumList::Init();
		}
		qRODISCLOSEv( fCRow, Column );
	};

	qW( Field );

	// template parameters.
# define OGZFLD_TP	ogzfld::vField, ogzfld::iField, ogzfld::fFRow, ogzfld::fDRow, sdr::fRow

	typedef ogzcbs::fDCallback<OGZFLD_TP> fCallback;

	typedef ogzcbs::fDItems<OGZFLD_TP> fFields;

	typedef ogzcbs::rDRegularCallback<OGZFLD_TP> rRegularCallback;

	typedef bch::qBUNCHvl( fFRow ) vFieldList;
	qW( FieldList );
}

#endif
