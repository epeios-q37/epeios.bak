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

// XDH UPStream.
// Stuff related to the server part.

#ifndef XDH_UPS_INC_
# define XDH_UPS_INC_

# include "csdscb.h"
# include "sclnjs.h"
# include "xdh_dws.h"

namespace xdh_ups {
	qENUM( Request )
	{
		rAlert,
		rConfirm,
		rSetLayout,
		rGetContents,
		rSetContents,
		rDressWidgets,
		rSetCastsByIds,
		rSetCastsByTags,
		rGetAttribute,
		rSetAttribute,
		rGetProperty,
		rSetProperty,
		r_amount,
		r_Undefined
	};

	struct rArguments
	{
	public:
		str::wString Message, Id, XML, XSLFilename, Language, Name, Value;
		str::wStrings Ids, Contents, Tags, Values;
		void reset( bso::sBool P = true )
		{
			tol::reset( Message, Id, XML, XSLFilename, Language, Name, Value, Ids, Contents, Tags, Values );
		}
		qCDTOR( rArguments );
		void Init( void )
		{
			tol::Init( Message, Id, XML, XSLFilename, Language, Name, Value, Ids, Contents, Tags, Values );
		}
	};

	struct rServer
	{
	public:
		eRequest Request;
		rArguments Arguments;
		void reset( bso::sBool P = true )
		{
			Request = r_Undefined;
			Arguments.reset( P );
		}
		qCDTOR( rServer );
		void Init( void )
		{
			Request = r_Undefined;
			Arguments.Init();
		}
	};

	bso::sBool Send(
		flw::sWFlow &Flow,
		rServer & Server );

	bso::sBool Recv(
		eRequest Id,
		flw::sRFlow &Flow,
		xdh_dws::rArguments &Arguments );
}

#endif
