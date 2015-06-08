/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define GECKOO__COMPILATION

#include "geckoo.h"

using namespace geckoo;

const char *geckoo::shared_data__::Version = GECKOO_OVERLAPPING_VERSION;
bso::uint__ geckoo::shared_data__::Control = shared_data__::ControlComputing();

void geckoo::AddPseudoEventHandler(
	nsIDOMNode *Node,
	const char *PseudoEventName,
	pseudo_event_callback__ &Handler )
{
qRH
	str::string Value;
	bso::pointer_buffer__ Buffer;
qRB
	Value.Init( "handlePseudoEvent( this, '" );

	Value.Append( bso::Convert( &Handler, Buffer ) );

	Value.Append( "');" );

	nsxpcm::SetAttribute( Node, PseudoEventName, Value );
qRR
qRT
qRE
}
