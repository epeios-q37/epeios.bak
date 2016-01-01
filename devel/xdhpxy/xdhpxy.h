/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef XDHPXY__INC
# define XDHPXY__INC

# define XDHPXY_NAME		"XDHPXY"

# if defined( E_DEBUG ) && !defined( XDHPXY_NODBG )
#  define XDHPXY_DBG
# endif

// X(SL/ DH(TML) ProXY

# include "xdhcbk.h"

# include "dlbrry.h"
# include "err.h"

namespace xdhpxy {
    class proxy___
    {
    private:
		dlbrry::dynamic_library___ _Library;
		Q37_MRMDF( xdhcbk::downstream_callback__, _C, _Callback );
	protected:
		virtual xdhcbk::upstream_callback__ &XDHPXYGetUpstreamCallback( void ) = 0;
    public:
        void reset( bso::bool__ P = true )
        {
			_Library.reset( P );
			_Callback = NULL;
        }
        E_CDTOR( proxy___ );
		bso::bool__ Init(
			const str::string_ &ModuleFileName,
			const char *Identification );
		xdhcbk::downstream_callback__ &Callback( void ) const
		{
			return _C();
		}
		const char *GetBaseLanguage( TOL_CBUFFER___ &Buffer ) const
		{
			return _C().BaseLanguage( Buffer );
		}
	};


}

#endif
