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

#include "xdh_ups.h"

#include "proxy.h"

using namespace xdh_ups;

namespace send_ {
	namespace {
		void Alert_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::alert::S( Arguments.Message, Flow );
		}
		void Confirm_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::confirm::S( Arguments.Message, Flow );
		}
		void SetLayout_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::layout::set::S( Arguments.Id, Arguments.XML, Arguments.XSLFilename, Arguments.Language, Flow );
		}
		void GetContents_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::contents::get::S( Arguments.Ids, Flow );
		}
		void SetContents_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::contents::set::S( Arguments.Ids, Arguments.Contents, Flow );
		}
		void DressWidgets_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::widgets::dress::S( Arguments.Id, Flow );
		}
		void AddClasses_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::classes::add::S( Arguments.Ids, Arguments.Classes, Flow );
		}
		void RemoveClasses_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::classes::remove::S( Arguments.Ids, Arguments.Classes, Flow );
		}
		void ToggleClasses_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::classes::toggle::S( Arguments.Ids, Arguments.Classes, Flow );
		}
		void EnableElements_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::elements::enable::S( Arguments.Ids, Flow );
		}
		void DisableElements_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::elements::disable::S( Arguments.Ids, Flow );
		}
		void GetAttribute_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::attribute::get::S( Arguments.Id, Arguments.Name, Flow );
		}
		void SetAttribute_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::attribute::set::S( Arguments.Id, Arguments.Name, Arguments.Value, Flow );
		}
		void GetProperty_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::property::get::S( Arguments.Id, Arguments.Name, Flow );
		}
		void SetProperty_(
			flw::sWFlow &Flow,
			const rArguments &Arguments )
		{
			proxy::property::set::S( Arguments.Id, Arguments.Name, Arguments.Value, Flow );
		}
	}
}

#define H( name )\
	case r##name:\
		send_::name##_( Flow, Arguments );\
		break

bso::sBool xdh_ups::Send(
	eRequest Request,
	flw::sWFlow &Flow,
	const rArguments &Arguments )
{
	switch ( Request ) {
	case r_Undefined:
		return false;
		break;
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

#undef H


namespace recv_ {
	namespace {
		void Alert_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::alert::R( Flow );
		}

		void Confirm_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::confirm::R( Flow, Return.StringToSet() );
		}

		void SetLayout_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::layout::set::R( Flow );
		}

		void GetContents_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::contents::get::R( Flow, Return.StringsToSet() );
		}

		void SetContents_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::contents::set::R( Flow );
		}

		void DressWidgets_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::widgets::dress::R( Flow );
		}

		void AddClasses_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::classes::add::R( Flow );
		}

		void RemoveClasses_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::classes::remove::R( Flow );
		}

		void ToggleClasses_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::classes::toggle::R( Flow );
		}

		void EnableElements_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::elements::enable::R( Flow );
		}

		void DisableElements_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::elements::disable::R( Flow );
		}

		void GetAttribute_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::attribute::get::R( Flow, Return.StringToSet() );
		}

		void SetAttribute_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::attribute::set::R( Flow );
		}

		void GetProperty_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::property::get::R( Flow, Return.StringToSet() );
		}
		void SetProperty_(
			flw::sRFlow &Flow,
			rReturn &Return )
		{
			proxy::property::set::R( Flow );
		}
	}
}

#define H( name )\
	case r##name:\
		recv_::name##_( Flow, Return );\
		break

bso::sBool xdh_ups::Recv(
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

