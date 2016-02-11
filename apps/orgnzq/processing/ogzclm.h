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

namespace ogzclm {

	qENUM( Number ) {
		nMono,
		nMulti,
		n_amount,
		n_Undefined,
	};

	qENUM( Type ) {
		tExplicit,	// Explicit column definition.
		tDeduced,	// Deduced (from a model) column defintion.
		t_amount,
		t_Undefined
	};

	// Deduced column row. Refers to a column row from a model.
	qROW( DCRow );

	class fColumn
	{
	public:
		ogztyp::fTRow Type;
		eNumber Number;
		void reset( bso::fBool = true )
		{
			Type = qNIL;
			Number = n_Undefined;
		}
		E_CDTOR( fColumn );
		void Init( void )
		{
			Type = qNIL;
			Number = n_Undefined;
		}
	};

	// Column row.
	qROW( CRow );

	// template parameters.
# define OGZCLM_TP	ogzclm::fColumn, ogzclm::fCRow

	typedef ogzcbs::fSCallback<OGZCLM_TP> fCallback;

	typedef ogzcbs::fSItems<OGZCLM_TP> fColumns;

	typedef ogzcbs::rSRegularCallback<OGZCLM_TP> rRegularCallback;
}

#endif
