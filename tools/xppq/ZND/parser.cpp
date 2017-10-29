/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XPPq.

    XPPq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XPPq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XPPq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "parser.h"

#include "xml.h"

using namespace parser;

using sclznd::sCaller;

namespace {
	class rRack_ {
	private:
		sclznd::rStreamDriver Stream_;
		flw::sDressedIFlow<> SFlow_;
		xtf::extended_text_iflow__ SXFlow_;
	public:
		xml::rParser Parser;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Stream_, SFlow_, SXFlow_, Parser );
		}
		qCDTOR( rRack_ );
		void Init( sclznd::sCaller &Caller )
		{
			Stream_.Init( Caller );
			SFlow_.Init( Stream_ );
			SXFlow_.Init( SFlow_, utf::f_Default );
			Parser.Init( SXFlow_, xml::eh_Default );
		}
	};
}

SCLZND_F( parser::New )
{
qRH;
	rRack_ *Rack = NULL;
qRB;
	if ( ( Rack = new rRack_ ) == NULL )
		qRAlc();

	Rack->Init( Caller );

	Caller.SetReturnValue( (bso::sS64)Rack );
qRR;
	if ( Rack != NULL )
		delete Rack;
qRT;
qRE;
}

SCLZND_F( parser::Parse )
{
qRH;
	bso::sS64 Long = 0;
	lcl::wMeaning Meaning;
	lcl::locale Locale;
	str::wString Error;
	bso::sS64 Token = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	rRack_ &Rack = *(rRack_ *)Long;
	xml::rParser &Parser = Rack.Parser;

	switch ( Parser.Parse( xml::tfObvious ) ) {
	case xml::t_Error:
		Meaning.Init();
		xml::GetMeaning( Parser.GetStatus(), Parser.Flow().Position(), Meaning );
		Locale.Init();
		Error.Init();
		Locale.GetTranslation( Meaning, "", Error );
		sclznd::Throw( Error );
		break;
	case xml::t_Processed:
		break;
	default:
		break;
	}

	// If modified, modify also PHP source file.
	switch ( Parser.Token() ) {
	case xml::t_Error:
		break;
	case xml::t_Processed:
		Token = 0;
		break;
	case xml::tStartTag:
		Token = 1;
		break;
	case xml::tAttribute:
		Token = 2;
		break;
	case xml::tValue:
		Token = 3;
		break;
	case xml::tEndTag:
		Token = 4;
		break;
	default:
		qRGnr();
		break;
	}

	Caller.SetReturnValue( Token );
qRR;
qRT;
qRE;
}

SCLZND_F( parser::TagName )
{
qRH;
	bso::sS64 Long = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	rRack_ &Rack = *(rRack_ *)Long;
	xml::rParser &Parser = Rack.Parser;

	Caller.SetReturnValue( Rack.Parser.TagName() );
qRR;
qRT;
qRE;
}

SCLZND_F( parser::AttributeName )
{
qRH;
	bso::sS64 Long = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	rRack_ &Rack = *(rRack_ *)Long;
	xml::rParser &Parser = Rack.Parser;

	Caller.SetReturnValue( Rack.Parser.AttributeName() );
qRR;
qRT;
qRE;
}

SCLZND_F( parser::Value )
{
qRH;
	bso::sS64 Long = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	rRack_ &Rack = *(rRack_ *)Long;
	xml::rParser &Parser = Rack.Parser;

	Caller.SetReturnValue( Rack.Parser.Value() );
qRR;
qRT;
qRE;
}

void parser::Delete( sclznd::sCaller & Caller )
{
qRH;
	bso::sS64 Long = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	delete (rRack_ * )Long;
qRR;
qRT;
qRE;
}
