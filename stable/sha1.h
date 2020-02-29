/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// SHA-1

#ifndef SHA1_INC_
# define SHA1_INC_

# define SHA1_NAME		"SHA1"

# if defined( E_DEBUG ) && !defined( SHA1_NODBG )
#  define SHA1_DBG
# endif

# include "err.h"
# include "flw.h"

namespace sha1 {
    void Compute(
        flw::rRFlow &Flow,
        str::dString &Hash );

    void Compute(
        const str::dString &Message,
        str::dString &Hash );

}

#endif
