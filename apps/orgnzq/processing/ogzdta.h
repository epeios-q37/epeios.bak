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

// OrGaniZer DaTuM

#ifndef OGZDTA__INC
# define OGZDTA__INC

# ifdef XXX_DBG
#	define OGZDTA__DBG
# endif

# include "ogzbsc.h"

# include "bch.h"
# include "lstctn.h"

namespace ogzdta {
	using ogzbsc::fSize;

	using ogzbsc::fDRow;
	using ogzbsc::fTRow;
	using ogzbsc::fByte;
	using ogzbsc::vDatum;
	using ogzbsc::iDatum;

	typedef lstctn::fHook fCallback;

	using lstctn::Plug;

	using lstctn::fCore;

	typedef lstctn::qLMCONTAINERi( vDatum, ogzdta::fDRow ) iData_;

	class rData
	: public iData_
	{
	public:
		void Init( fCallback &Callback )
		{
			if ( !Plug( *this, Callback ) )
				iData_::Init();
		}
	};

	typedef lstctn::rRH rRegularCallback;

	typedef bch::qBUNCHvl( fDRow ) vDatumList;
	qW( DatumList );
}


#endif
