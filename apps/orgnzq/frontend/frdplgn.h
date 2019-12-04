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

#ifndef FRDPLGN_INC_
# define FRDPLGN_INC_

# include "frdcllbck.h"

# include "plgn.h"

# include "tol.h"

# define FRDPLGN_XML_TARGET		"XML"
# define FRDPLGN_XML_VERSION	"1"

namespace frdplgn {
	extern const char *XMLTarget;
	extern const char *XMLVersion;

	typedef frdcllbck::cXML sXML_;

	class sXML
	: public sXML_
	{
	public:
		void reset( bso::bool__ P = true )
		{
		}
		E_CVDTOR( sXML );
		void Init( void )
		{
		}
	public:
		static const char *Label( void );
	};

	typedef plgn::rRetrievers<sXML> rXMLRetrievers;
}

#endif
