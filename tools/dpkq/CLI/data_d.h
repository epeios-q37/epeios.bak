/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

// DATA Displaying

#ifndef DATA_D_INC_
# define DATA_D_INC_

# include "data_c.h"

# include "dpkctx.h"

#define DATA_D_ALL			BSO_UINT_MAX
#define DATA_D_UNDEFINED	DATA_D_ALL

namespace data_d {
	typedef bso::sUInt		sId;

	qCDEF( sId, All, bso::UIntMax );
	qCDEF( sId, Undefined, All );

	void Initialize( void );

	sId Display(
		sId Id,
		const data_c::dData &Data,
		const str::string_ &XSLFileName,
		bso::uint__ SessionMaxDuration,
		str::string_ &Label,
		str::string_ &TableLabel,
		dpkctx::context_ &Context,
		const str::string_ &OutputFileName );
}

#endif
