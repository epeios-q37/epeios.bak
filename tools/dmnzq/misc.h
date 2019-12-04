/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of dmnzq.

    dmnzq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dmnzq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dmnzq.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef MISC_INC_
# define MISC_INC_

# include "csdscb.h"

# define MISC_SLOT_PLUGIN_TARGET	"Slot"
# define MISC_SLOT_PLUGIN_VERSION	"3"

namespace misc {
	typedef csdscb::cProcessing sModule;	// Module to 'daemonize'.

	static E_CDEF( char *, SlotPluginTarget, MISC_SLOT_PLUGIN_TARGET );
	static E_CDEF( char *, SlotPluginVersion, MISC_SLOT_PLUGIN_VERSION );

	using csdscb::sTimeout;		// Timeout duration in seconds.
	using csdscb::NoTimeout;

	class cHandler
	{
	protected:
		virtual void MISCHandle(
			sModule &Module,
			sTimeout Timeout ) = 0;
	public:
		void Handle(
			sModule &Module,
			sTimeout Timeout )
		{
			return MISCHandle( Module, Timeout );
		}
		static const char *Label( void );
	};
}

#endif
