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

#include "server.h"

using namespace xdh_ups;

namespace send_ {
	namespace {
		void Alert_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::alert::S( Arguments.Message, Flow );
		}
		void Confirm_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::confirm::S( Arguments.Message, Flow );
		}
		void SetLayout_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::layout::set::S( Arguments.Id, Arguments.XML, Arguments.XSLFilename, Arguments.Language, Flow );
		}
		void GetContents_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::contents::get::S( Arguments.Ids, Flow );
		}
		void SetContents_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::contents::set::S( Arguments.Ids, Arguments.Contents, Flow );
		}
		void DressWidgets_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::widgets::dress::S( Arguments.Id, Flow );
		}
		void SetCastsByIds_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::casts_by_ids::set::S( Arguments.Ids, Arguments.Values, Flow );
		}
		void SetCastsByTags_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::casts_by_tags::set::S( Arguments.Id, Arguments.Tags, Arguments.Values, Flow );
		}
		void GetAttribute_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::attribute::get::S( Arguments.Id, Arguments.Name, Flow );
		}
		void SetAttribute_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::attribute::set::S( Arguments.Id, Arguments.Name, Arguments.Value, Flow );
		}
		void GetProperty_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::property::get::S( Arguments.Id, Arguments.Name, Flow );
		}
		void SetProperty_(
			flw::sWFlow &Flow,
			rArguments &Arguments )
		{
			server::property::set::S( Arguments.Id, Arguments.Name, Arguments.Value, Flow );
		}
	}
}

#define H( name )\
	case r##name:\
		send_::name##_( Flow, Server.Arguments );\
		break

bso::sBool xdh_ups::Send(
	flw::sWFlow &Flow,
	rServer &Server )
{
	switch ( Server.Request ) {
	case r_Undefined:
		return false;
		break;
	H( Alert );
	H( Confirm );
	H( SetLayout );
	H( GetContents );
	H( SetContents );
	H( DressWidgets );
	H( SetCastsByIds );
	H( SetCastsByTags );
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
			xdh_dws::rArguments &Arguments )
		{
			server::alert::R( Flow );
		}

		void Confirm_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::confirm::R( Flow, Arguments.Response() );
		}

		void SetLayout_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::layout::set::R( Flow );
		}

		void GetContents_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::contents::get::R( Flow, Arguments.Contents() );
		}

		void SetContents_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::contents::set::R( Flow );
		}

		void DressWidgets_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::widgets::dress::R( Flow );
		}

		void SetCastsByIds_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::casts_by_ids::set::R( Flow );
		}

		void SetCastsByTags_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::casts_by_tags::set::R( Flow );
		}

		void GetAttribute_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::attribute::get::R( Flow, Arguments.Value() );
		}

		void SetAttribute_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::attribute::set::R( Flow );
		}

		void GetProperty_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::property::get::R( Flow, Arguments.Value() );
		}
		void SetProperty_(
			flw::sRFlow &Flow,
			xdh_dws::rArguments &Arguments )
		{
			server::property::set::R( Flow );
		}
	}
}

#define H( name )\
	case r##name:\
		recv_::name##_( Flow, Arguments );\
		break

bso::sBool xdh_ups::Recv(
	eRequest Id,
	flw::sRFlow &Flow,
	xdh_dws::rArguments &Arguments )
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
		H( SetCastsByIds );
		H( SetCastsByTags );
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

