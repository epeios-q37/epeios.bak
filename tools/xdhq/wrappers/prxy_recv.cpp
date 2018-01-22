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

#include "prxy_recv.h"

using namespace prxy_recv;

using namespace prxy_cmn;

namespace {
	void Alert_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		alert::R( Flow );
	}

	void Confirm_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		confirm::R( Flow, Return.StringToSet() );
	}

	void SetLayout_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		layout::set::R( Flow );
	}

	void GetContents_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		contents::get::R( Flow, Return.StringsToSet() );
	}

	void SetContents_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		contents::set::R( Flow );
	}

	void DressWidgets_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		widgets::dress::R( Flow );
	}

	void AddClasses_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		classes::add::R( Flow );
	}

	void RemoveClasses_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		classes::remove::R( Flow );
	}

	void ToggleClasses_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		classes::toggle::R( Flow );
	}

	void EnableElements_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		elements::enable::R( Flow );
	}

	void DisableElements_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		elements::disable::R( Flow );
	}

	void GetAttribute_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		attribute::get::R( Flow, Return.StringToSet() );
	}

	void SetAttribute_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		attribute::set::R( Flow );
	}

	void GetProperty_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		property::get::R( Flow, Return.StringToSet() );
	}
	void SetProperty_(
		flw::sRFlow &Flow,
		rReturn &Return )
	{
		property::set::R( Flow );
	}
}

#define H( name )\
	case r##name:\
		name##_( Flow, Return );\
		break

bso::sBool prxy_recv::Recv(
	eRequest Id,
	flw::sRFlow &Flow,
	rReturn &Return )
{
	if ( Id == r_Undefined )	// A new event has be detected.
		return false;
	else {
		if ( prtcl::GetRequest( Flow ) != prtcl::rReady_1 )
			qRGnr();

		switch ( Id ) {
		H( Alert );
		H( Confirm );
		H( SetLayout );
		H( GetContents );
		H( SetContents );
		H( DressWidgets );
		H( AddClasses );
		H( RemoveClasses );
		H( ToggleClasses );
		H( EnableElements );
		H( DisableElements );
		H( GetAttribute );
		H( SetAttribute );
		H( GetProperty );
		H( SetProperty );
		default:
			qRGnr();
			break;
		}

		return true;
	}
}

#undef H

