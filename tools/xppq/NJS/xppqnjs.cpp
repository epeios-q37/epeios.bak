/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

# include "xppqnjs.h"

#include "parser.h"
#include "registry.h"
#include "stream.h"

#include "sclm.h"
#include "scln4a.h"

void sclnjs::SCLNJSInfo( txf::sWFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build: " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	void ReturnArgument_( sclnjs::sCaller &Caller )
	{
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.GetArgument( Input );

		Text.Init();
		sclm::GetBaseTranslation( "Argument", Text, Input );

		Caller.SetReturnValue( Text );
	qRR
	qRT
	qRE
	}
}

namespace console_ {
	SCLNJS_F( OnData )
	{
	qRH
		sclnjs::rObject This;
		sclnjs::rBuffer Chunk;
	qRB
		tol::Init( This, Chunk );
		Caller.GetArgument( This, Chunk );

		cio::COut << Chunk;
	qRR
	qRT
	qRE
	}

	SCLNJS_F( OnEnd )
	{
	qRH
		sclnjs::rObject This;
	qRB
		tol::Init( This );
		Caller.GetArgument( This );

		cio::COut << txf::nl << "END!!!" << txf::commit;
	qRR
	qRT
	qRE
	}
}

namespace stream_ {
	SCLNJS_F( Transform )
	{
	qRH
		sclnjs::rBuffer In;
		str::wString Out;
	qRB
		tol::Init(In, Out);
		Caller.GetArgument(In);

		In.ToString(Out);

		str::ToUpper(Out);

		Caller.SetReturnValue(Out);
	qRR
	qRT
	qRE
	}
}

const scli::sInfo &sclnjs::SCLNJSRegister( sclnjs::sRegistrar &Registrar )
{
	static scli::sInfo Info(NAME_LC, NAME_MC, "q37.info");

	Registrar.Register(ReturnArgument_ );
	Registrar.Register(10, parser::OnData, parser::OnEnd, parser::Parse);
	Registrar.Register(20, stream::Set, stream::Transform, stream::Flush, stream::GetError);
	Registrar.Register(30, console_::OnData, console_::OnEnd);
	Registrar.Register(40, stream_::Transform);

	return Info;
}
