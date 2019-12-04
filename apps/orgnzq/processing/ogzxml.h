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

// OrGaniZer XML

#ifndef OGZXML_INC_
# define OGZXML_INC_

# ifdef XXX_DBG
#	define OGZXML__DBG
# endif

# include "ogzbsc.h"
# include "ogzdtb.h"

# include "xml.h"

namespace ogzxml {
	typedef bso::sU8 sVersion;

	qCDEF( sVersion, RecordLastVersion, 0 );

	qENUM( RecordTag ) {
		rtRecord,
		rtFields,
		rtField,
		rtData,
		rtDatum,
		rt_amount,
		rt_Undefined 
	};

	const char *GetLabel(
		eRecordTag Tag,
		sVersion Version = RecordLastVersion );

	qENUM( RecordAttribute ) {
		raId,
		raAmount,
		raType,
		raNumber,
		ra_amount,
		ra_Undefined 
	};

	const char *GetLabel(
		eRecordAttribute Attribute,
		sVersion Version = RecordLastVersion );
}

#endif
