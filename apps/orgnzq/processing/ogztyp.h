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

// OrGaniZer field TYPe

#ifndef OGZTYP__INC
# define OGZTYP__INC

# ifdef XXX_DBG
#	define OGZTYP__DBG
# endif

# include "ogzbsc.h"
# include "ogzcbs.h"

# include "lstbch.h"
# include "str.h"

namespace ogztyp {

	using ogzbsc::fRRow;
	using ogzbsc::fURow;
	using ogzbsc::fTRow;

	class fCallback
	{
	protected:
		virtual const char *OGZTYPGetLabel( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			//	Standardization.
		}
		qCVDTOR( fCallback );
		void Init( void )
		{
			//	Standardization.
		}
		const char *GetLabel( void )
		{
			return OGZTYPGetLabel();
		}
	};

	class fType
	{
	private:
		qRVM( fCallback, C_, Callback_ );
	public:
		qCDTOR( fType );
		void reset( bso::bool__ = true )
		{
			Callback_ = NULL;
		}
		void Init( fCallback &Callback )
		{
			Callback_ = &Callback;
		}
		const char *GetLabel( void ) const
		{
			return C_().GetLabel();
		}
	};

	typedef lstbch::qLBUNCHv( fType, fTRow ) vTypes;
	qW( Types );
}


#endif