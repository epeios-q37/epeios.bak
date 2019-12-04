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

#ifndef CONTEXT_INC_
# define CONTEXT_INC_

# include "dpkctx.h"

namespace context {
	// File from which the context is retrieved is red from the registry.
	void Retrieve( dpkctx::context_ &Context );

	// File where the content is dumped is red from the registry.
	void Dump( const dpkctx::context_ &Context );
}

#endif
