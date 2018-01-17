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

#include "treep.h"

#include "tree.h"

#include "txf.h"
#include "xml.h"
#include "flx.h"

using namespace treep;

namespace {
	struct rTreeRack_
	: public tree::rRack
	{
	public:
		void Init( void )
		{
			tree::rRack::Init( sclmisc::SCLMISCTargetName );
		}
	};
}
namespace {
	typedef sclnjs::rObject rObject_;

	qCDEF( char *, Id_, "_q37TreeRack" );
}

SCLNJS_F( treep::New )
{
qRH;
	rObject_ Object;
	rTreeRack_ *Rack = NULL;
qRB;
	tol::Init( Object );
	Caller.GetArgument( Object );

	Rack = new rTreeRack_;

	if ( Rack == NULL )
		qRAlc();

	Rack->Init();

	Object.Set( Id_, Rack );
qRR;
	if ( Rack != NULL )
		delete Rack;
qRT;
qRE;
}

SCLNJS_F( treep::Delete )
{
	// To complete !!!
}

namespace {
	rTreeRack_ &GetRack_( sclnjs::sCaller &Caller )
	{
		rTreeRack_ *Rack = NULL;
	qRH;
		rObject_ Object;
	qRB;
		tol::Init( Object );
		Caller.GetArgument( Object );

		Rack = (rTreeRack_ *)Object.Get( Id_ );

		if ( Rack == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return *Rack;
	}
}

SCLNJS_F( treep::PushTag )
{
qRH;
	str::wString Tag;
qRB;
	rTreeRack_ &Rack = GetRack_( Caller );

	Tag.Init();
	Caller.GetArgument( Tag );

	Rack.PushTag( Tag );
qRR;
qRT;
qRE;
}

SCLNJS_F( treep::PopTag )
{
	GetRack_( Caller ).PopTag();
}

SCLNJS_F( treep::PutValue )
{
qRH;
	str::wString Value;
qRB;
	rTreeRack_ &Rack = GetRack_( Caller );

	Value.Init();
	Caller.GetArgument( Value );

	Rack.PutValue( Value );
qRR;
qRT;
qRE;
}

SCLNJS_F( treep::PutAttribute )
{
qRH;
	str::wString Name, Value;
qRB;
	rTreeRack_ &Rack = GetRack_( Caller );

	tol::Init( Name, Value );

	Caller.GetArgument( Name, Value );

	Rack.PutAttribute( Name, Value );
qRR;
qRT;
qRE;
}

const str::dString &treep::GetXML(
	sclnjs::sCaller &Caller,
	str::dString &XML )
{
	return GetRack_( Caller ).GetXML( XML );
}

