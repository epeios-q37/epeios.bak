/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define BTS_COMPILATION_

#include "bts.h"

using namespace bts;

#if 0
_row__ _binary_tree_seeker__::NodeWithoutLeft_( _row__ Node )
{
	while( Tree_->HasLeft( Node ) )
		Node = *Tree_->Left( Node );

	return Node;
}

_row__ _binary_tree_seeker__::First_( _row__ Node )
{
	Node = NodeWithoutLeft_( Node );

	while( Tree_->HasRight( Node ) )
		Node = NodeWithoutLeft_( *Tree_->Right( Node ) );

	return Node;
}
#endif

