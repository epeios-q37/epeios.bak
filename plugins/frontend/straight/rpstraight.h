/*
	Copyright (C) 2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'remote' software.

    'remote' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'remote' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'remote'.  If not, see <http://www.gnu.org/licenses/>.
*/

// Exposed data from the remote plugin 'Straight'.

#ifndef RPSTRAIGHT_INC_
# define RPSTRAIGHT_INC_

# include "bso.h"
# include "tol.h"
# include "str.h"
# include "plgncore.h"

namespace rpstraight {
	typedef plgncore::sAbstract sAbstract_;

	static qCDEF( char *, Identifier, "2c95d4ed-2c57-4399-9c86-4115883be7cc" );	// From 'Project.xml'. Don't use the 'IDENTIFIER' macro, as it is used to copile other binaries!!!

	class rAbstract
	: public sAbstract_
	{
	protected:
		virtual const char *PLGNCOREIdentifier( void ) override
		{
			return rpstraight::Identifier;
		}
	public:
		str::wString HostService;
		void reset( bso::sBool P = true )
		{
			sAbstract_::reset( P );
			HostService.reset( P );
		}
		qCDTOR( rAbstract );
		void Init( void )
		{
			sAbstract_::Init();
			HostService.Init();
		}
	};
}

#endif
