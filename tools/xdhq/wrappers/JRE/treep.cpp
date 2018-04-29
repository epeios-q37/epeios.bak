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
#include "xdhqjre.h"

#include "txf.h"
#include "xml.h"
#include "flx.h"

using namespace treep;

namespace {
	struct rRack_
	: public tree::rRack
	{
	public:
		void Init( void )
		{
			tree::rRack::Init( NAME_MC );
		}
	};
}

SCLJRE_F( treep::New )
{
	return scljre::CreateUO<rRack_>( Env );
}

SCLJRE_F( treep::Delete )
{
	return scljre::DeleteUO<rRack_>( Caller );
}

namespace {
	rRack_ &GetRack_( scljre::sCaller &Caller )
	{
		return scljre::GetUO<rRack_>( Caller );
	}
}

SCLJRE_F( treep::PushTag )
{
qRH;
	str::wString Tag;
qRB;
	rRack_ &Rack = GetRack_( Caller );

	Tag.Init();
	Caller.Get( Tag );

	Rack.PushTag( Tag );
qRR;
qRT;
qRE;
	return scljre::Null();
}

SCLJRE_F( treep::PopTag )
{
	GetRack_( Caller ).PopTag();

	return scljre::Null();
}

SCLJRE_F( treep::PutValue )
{
qRH;
	str::wString Value;
qRB;
	rRack_ &Rack = GetRack_( Caller );

	Value.Init();
	Caller.Get( Value );

	Rack.PutValue( Value );
qRR;
qRT;
qRE;
	return scljre::Null();
}

SCLJRE_F( treep::PutAttribute )
{
qRH;
	str::wString Name, Value;
qRB;
	rRack_ &Rack = GetRack_( Caller );

	tol::Init( Name, Value );

	Caller.Get( Name );
	Caller.Get( Value );

	Rack.PutAttribute( Name, Value );
qRR;
qRT;
qRE;
	return scljre::Null();
}

const str::dString &treep::GetXML(
	scljre::sCaller &Caller,
	str::dString &XML )
{
	return GetRack_( Caller ).GetXML( XML );
}

