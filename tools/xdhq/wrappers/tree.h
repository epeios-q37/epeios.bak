/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

// TREE 

#ifndef TREE_INC_
# define TREE_INC_

# include "flx.h"
# include "xml.h"

namespace tree {
	typedef xml::wWriter rWriter_;

	struct rRack
	: public rWriter_
	{
	private:
		flx::rStringTOFlow Flow_;
		str::wString XML_;
	public:
		void reset( bso::sBool P = true )
		{
			rWriter_::reset( P );
			tol::reset( P, XML_, Flow_ );
		}
		qCDTOR( rRack );
		void Init( const char *Generator )
		{
			tol::bDateAndTime Buffer;
			XML_.Init();
			Flow_.Init( XML_ );
			rWriter_::Init( Flow_, xml::lCompact, xml::e_Default );

			PushTag( "XDHTML" );
			PutAttribute( "Generator", Generator );
			PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );
			PutAttribute( "OS", cpe::GetOSDigest() );
		}
		const str::dString &GetXML( str::dString &XML )
		{
			rWriter_::reset();
			Flow_.reset();
			XML = XML_;
			XML_.reset();

			return XML;
		}
	};
}

#endif
