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

#include "slfhlead.h"

using namespace slfhlead;

namespace {
	csdmnc::rCore Core_;
	bso::sBool CoreIsInitialized_ = false;
}

bso::sBool slfhlead::InitializeCore(
		const char *HostService,
		bso::uint__ PingDelay,
		sck::duration__ Timeout,
		csdmnc::cLog *LogCallback)
{
		if ( CoreIsInitialized_ )
			qRFwk();

		if ( Core_.Init(HostService, PingDelay, Timeout, LogCallback) ) {
			CoreIsInitialized_ = true;
			return true;
		} else
			return false;
}

bso::sBool slfhlead::CoreIsInitialized(void)
{
	return CoreIsInitialized_;
}

csdmnc::rCore &slfhlead::Core(void) {
	if ( !CoreIsInitialized_ )
		qRGnr();

	return Core_;
}
