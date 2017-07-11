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

#ifndef CONTEXT__INC
# define CONTEXT__INC

# include "dpkctx.h"

namespace context {
	void Retrieve(
		const str::string_ &FileName,
		dpkctx::context_ & Context );

	void Dump(
		const dpkctx::context_ &Context,
		const str::string_ &FileName );
}

#endif
