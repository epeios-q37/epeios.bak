/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PRXY_SEND_INC_
# define PRXY_SEND_INC_

# include "prxy_cmn.h"
# include "crt.h"

namespace prxy_send {
	struct rArguments
	{
	public:
		str::wString Script, Message, Id, XML, XSLFilename, Language, Name, Value;
		str::wStrings Ids, Contents, Tags, Values, Classes;
		void reset( bso::sBool P = true )
		{
			tol::reset( Script, Message, Id, XML, XSLFilename, Language, Name, Value, Ids, Contents, Tags, Values, Classes );
		}
		qCDTOR( rArguments );
		void Init( void )
		{
			tol::Init( Script, Message, Id, XML, XSLFilename, Language, Name, Value, Ids, Contents, Tags, Values, Classes );
		}
	};

	struct rNewArguments
	{
	public:
		str::wString Command;
		str::wStrings Strings;
		crt::qCRATEwl( str::dStrings ) Arrays;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Command, Strings, Arrays );
		}
		qCDTOR( rNewArguments );
		void Init( void )
		{
			tol::Init( Command, Strings, Arrays );
		}
	};

	void Send(
		prxy_cmn::eRequest Request,
		flw::sWFlow &Flow,
		const rArguments &Arguments,
		const rNewArguments &NewArguments );
}

#endif