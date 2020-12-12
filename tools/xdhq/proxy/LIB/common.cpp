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

#include "common.h"

using namespace common;

namespace {
	csdmnc::rCore Core_;
	bso::sBool CoreIsInitialized_ = false;
}

bso::sBool common::InitializeCore(
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

bso::sBool common::CoreIsInitialized(void)
{
	return CoreIsInitialized_;
}

csdmnc::rCore &common::Core(void) {
	if ( !CoreIsInitialized_ )
		qRGnr();

	return Core_;
}

logq::rFDriver<> common::LogDriver;

namespace {
	xdhcuc::cGlobal *Callback_ = NULL;

	xdhcuc::cGlobal &C_(void)
	{
		if ( Callback_ == NULL )
			qRGnr();

		return *Callback_;
	}
}

void common::SetCallback(xdhcuc::cGlobal &Callback)
{
	::Callback_ = &Callback;
}

xdhcuc::cGlobal &common::GetCallback(void)
{
	return C_();
}
