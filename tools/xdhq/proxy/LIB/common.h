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

#ifndef COMMON_INC_
# define COMMON_INC_

# include "xdhcuc.h"

# include "csdmnc.h"
# include "logq.h"

namespace common {
	namespace faas {
		using namespace xdhcmn::faas;
	}

	bso::sBool InitializeCore(
			const char *HostService,
			bso::uint__ PingDelay,
			sck::duration__ Timeout,
			csdmnc::cLog *LogCallback = NULL );

	bso::sBool CoreIsInitialized(void);
	csdmnc::rCore &Core(void);

	extern logq::rFDriver<> LogDriver;

	void SetCallback(xdhcuc::cGlobal &Callback);

	xdhcuc::cGlobal &GetCallback(void);
}

#endif
