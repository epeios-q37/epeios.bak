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

// DATA.

#ifndef DATA__INC
# define DATA__INC

# include "data_d.h"
# include "data_r.h"

#define DATA_ALL		BSO_UINT_MAX
#define DATA_UNDEFINED	DATA_ALL

namespace data {
	using data_c::dData;
	using data_c::wData;

	using data_d::sId;
	using data_d::All;
	using data_d::Undefined;
	using data_d::Display;

	using data_r::Retrieve;

	inline void Initialize( void )
	{
		data_c::Initialize();
		data_d::Initialize();
		data_r::Initialize();
	}
}

#endif
