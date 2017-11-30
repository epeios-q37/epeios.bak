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

#include "txf.h"
#include "xml.h"
#include "flx.h"

using namespace treep;

namespace {
	typedef xml::wWriter rWriter_;

	struct rRack_
	: public rWriter_
	{
	private:
		flx::rStringTOFlow Flow_;
		str::wString XML_;
	public:
		void reset( bso::sBool P = true )
		{
			rWriter_::reset( P );
			tol::reset( P, XML_, Flow_ );
		}
		qCDTOR( rRack_ );
		void Init( const str::dString &Background )
		{
			tol::bDateAndTime Buffer;
			XML_.Init();
			Flow_.Init( XML_ );
			rWriter_::Init( Flow_, xml::lCompact, xml::e_Default );

			PushTag( "XDHTML" );
			PutAttribute( "Background", Background );
			PutAttribute( "Generator", sclmisc::SCLMISCTargetName );
			PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );
			PutAttribute( "OS", cpe::GetOSDigest() );
			PushTag( Background );
		}
		const str::dString &GetXML( str::dString &XML )
		{
			rWriter_::reset();
			Flow_.reset();
			XML = XML_;
			XML_.reset();
			
			return XML;
		}
	};
}

SCLJRE_F( treep::New )
{
	scljre::sJObject Return = NULL;
qRH;
	str::wString Background;
qRB;
	Background.Init();
	Caller.Get( Background );

	Return = scljre::CreateUO<rRack_>( Background );
qRR;
qRT;
qRE;
	return Return;
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

