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

// LiST CRaTe

#ifndef LSTCRT__INC
# define LSTCRT__INC

# define LSTCRT_NAME		"LSTCRT"

# if defined( E_DEBUG ) && !defined( LSTCRT_NODBG )
#  define LSTCRT_DBG
# endif

# include "err.h"

# include "lstctn.h"

namespace lstcrt {
	using namespace lstctn;
}

# define qLMCRATEd( type, row )	qLMCONTAINERd( type, row )
# define qLMCRATEw( type, row ) qLMCONTAINERw( type, row )

# define qLMCRATEdl( type )		qLMCRATEd( type, sdr::sRow )
# define qLMCRATEwl( type )		qLMCRATEw( type, sdr::sRow )

# define qLCRATEd( type, row )  qLCONTAINERd( type, row )
# define qLCRATEw( type, row )	qLCONTAINERw( type, row )

# define qLCRATEdl( type )		qLCRATEd( type, sdr::sRow )
# define qLCRATEwl( type )		qLCRATEw( type, sdr::sRow )


#endif
