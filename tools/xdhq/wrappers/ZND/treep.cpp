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

// TREE Primitives/

#include "treep.h"

#include "tree.h"
#include "xdhqznd.h"

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
			tree::rRack::Init( xdhqznd::Info.Target() );
		}
	};
}

SCLZND_F( treep::New )
{
qRH;
	rTreeRack_ *Rack = NULL;
qRB;
	Rack = new rTreeRack_;

	if ( Rack == NULL )
		qRAlc();

	Rack->Init();

	Caller.SetReturnValue( (bso::sS64)Rack );
qRR;
qRT;
qRE;
}

SCLZND_F( treep::Delete )
{
	bso::sS64 Long = 0;

	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	delete (rTreeRack_ * )Long;
}

namespace {
	rTreeRack_ &GetRack_( sclznd::sCaller &Caller )
	{
		bso::sS64 Long = 0;

		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		return *(rTreeRack_ *)Long;
	}
}

#define RACK rTreeRack_ &Rack = GetRack_( Caller )

SCLZND_F( treep::PushTag )
{
qRH;
	str::wString Tag;
qRB;
	RACK;

	Tag.Init();
	Caller.Get( Tag );

	Rack.PushTag( Tag );
qRR;
qRT;
qRE;
}

SCLZND_F( treep::PopTag )
{
	RACK;

	Rack.PopTag();
}

SCLZND_F( treep::PutValue )
{
qRH;
	str::wString Value;
qRB;
	RACK;

	Value.Init();
	Caller.Get( Value );

	Rack.PutValue( Value );
qRR;
qRT;
qRE;
}

SCLZND_F( treep::PutAttribute )
{
qRH;
	str::wString Name, Value;
qRB;
	RACK;

	tol::Init( Name, Value );

	Caller.Get( Name );
	Caller.Get( Value );

	Rack.PutAttribute( Name, Value );
qRR;
qRT;
qRE;
}

const str::dString &treep::GetXML(
	sclznd::sCaller &Caller,
	str::dString &XML )
{
	RACK;

	return Rack.GetXML( XML );
}