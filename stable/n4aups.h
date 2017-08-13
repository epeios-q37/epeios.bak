/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// Native For All UPStream

#ifndef N4AUPS_INC_
# define N4AUPS_INC_

# define N4AUPS_NAME		"N4AUPS"

# if defined( E_DEBUG ) && !defined( N4AUPS_NODBG )
#  define N4AUPS_DBG
# endif

# include "err.h"
# include "n4a.h"

namespace n4aups {
	void SetLauncher( n4a::cLauncher *Launcher );

	n4a::cLauncher &GetLauncher( void );

	bso::sBool GetLauncherInfo( str::dString &Info );

	void DeleteLauncher( void );

	bso::sBool Register(
		const fnm::rName &ComponentFilename,
		sclmisc::sRack &Rack,
		qRPD );

	void *GetFunction( sdr::sRow Row );
}

#endif
