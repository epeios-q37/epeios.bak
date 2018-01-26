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

#include "prxy_send.h"

using namespace prxy_send;

using namespace prxy_cmn;

namespace {
	void Alert_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		alert::S( Arguments.Message, Flow );
	}
	void Confirm_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		confirm::S( Arguments.Message, Flow );
	}
	void SetLayout_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		layout::set::S( Arguments.Id, Arguments.XML, Arguments.XSLFilename, Arguments.Language, Flow );
	}
	void GetContents_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		contents::get::S( Arguments.Ids, Flow );
	}
	void SetContents_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		contents::set::S( Arguments.Ids, Arguments.Contents, Flow );
	}
	void DressWidgets_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		widgets::dress::S( Arguments.Id, Flow );
	}
	void AddClasses_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		classes::add::S( Arguments.Ids, Arguments.Classes, Flow );
	}
	void RemoveClasses_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		classes::remove::S( Arguments.Ids, Arguments.Classes, Flow );
	}
	void ToggleClasses_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		classes::toggle::S( Arguments.Ids, Arguments.Classes, Flow );
	}
	void EnableElements_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		elements::enable::S( Arguments.Ids, Flow );
	}
	void DisableElements_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		elements::disable::S( Arguments.Ids, Flow );
	}
	void GetAttribute_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		attribute::get::S( Arguments.Id, Arguments.Name, Flow );
	}
	void SetAttribute_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		attribute::set::S( Arguments.Id, Arguments.Name, Arguments.Value, Flow );
	}
	void GetProperty_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		property::get::S( Arguments.Id, Arguments.Name, Flow );
	}
	void SetProperty_(
		flw::sWFlow &Flow,
		const rArguments &Arguments )
	{
		property::set::S( Arguments.Id, Arguments.Name, Arguments.Value, Flow );
	}
}

#define H( name )\
	case r##name:\
		name##_( Flow, Arguments );\
		break

void prxy_send::Send(
	eRequest Request,
	flw::sWFlow &Flow,
	const rArguments &Arguments )
{
	switch ( Request ) {
	case r_Undefined:
		qRGnr();
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
}

#undef H
