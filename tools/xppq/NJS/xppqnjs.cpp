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
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
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
	namespace {
		qCDEF(char *, TargetId_, "_q37Target");
		qCDEF(char *, TargetEODId_, "_q37TargetEOD");
		qCDEF(char *, TargetChunkId_, "_q37TargetChunk");

		typedef str::wString rChunk_;

		inline sclnjs::rObject &GetTarget_(sclnjs::rObject &Source)
		{
		  return Source.Get<sclnjs::rObject>(TargetId_);
		}

		inline bso::sBool &GetEOD_(sclnjs::rObject &Target)
		{
      return Target.Get<bso::sBool>(TargetEODId_);
		}

		inline rChunk_ &GetChunk_(sclnjs::rObject &Target)
		{
		  return Target.Get<rChunk_>(TargetChunkId_);
		}
	}

	SCLNJS_F( OnData )
	{
	qRH
		sclnjs::rObject This;  // Source.
		sclnjs::rBuffer Chunk;
	qRB
		tol::Init( This, Chunk );
		Caller.GetArgument( This, Chunk );

		sclnjs::rObject &Target = GetTarget_(This);

    CPq;
		Chunk.ToString(GetChunk_(Target));
		CPq;
	qRR
	qRT
	qRE
	}

	SCLNJS_F( OnEnd )
	{
	qRH
		sclnjs::rObject This;  // Source.
	qRB
		tol::Init( This );
		Caller.GetArgument( This );

		CPq;
		sclnjs::rObject &Target = GetTarget_(This);
		GetEOD_(Target) = true;
		CPq;
	qRR
	qRT
	qRE
	}

	SCLNJS_F( Read )
	{
	qRH
		sclnjs::rObject This;  // Target
	qRB
		tol::Init( This );
		Caller.GetArgument( This );

    rChunk_ &Chunk = GetChunk_(This);

    CPq;
    if ( !GetEOD_(This) ) {
      Caller.SetReturnValue(Chunk);
      CPq;
    } else {
      Caller.SetReturnValueAsNull();
      CPq;
    }
    CPq;

    Chunk.Init();
	qRR
	qRT
	qRE
	}

	SCLNJS_F( Set )
	{
	qRH
		sclnjs::rObject Source, *This = NULL;
    bso::sBool *EOD = NULL;
    rChunk_ *Chunk = NULL;
	qRB
		This = qNEW(sclnjs::rObject);
    EOD = qNEW(bso::sBool);
    Chunk = qNEW(rChunk_);

		tol::Init(Source, *This, *Chunk);
		*EOD = false;

		Caller.GetArgument( Source, *This );

		Source.Set(TargetId_, This);
		This->Set(TargetEODId_, EOD);
		This->Set(TargetChunkId_, Chunk);
	qRR
    qDELETE(This);
    qDELETE(EOD);
    qDELETE(Chunk);
	qRT
	qRE
	}
}

namespace dummy_ {
	SCLNJS_F( Read )
	{
	qRH
		static bool First = true;
		str::wString Chunk;
	qRB
		if ( First ) {
      First = false;
		  CPq;
		  Chunk.Init("Coucou");
      Caller.SetReturnValue(Chunk);
		  CPq;
		} else {
		  CPq;
		  Caller.SetReturnValueAsNull();
      CPq;
		}
	qRR
	qRT
	qRE
	}

	SCLNJS_F( Set )
	{
	qRH
		sclnjs::rObject *This = NULL;
	qRB
		This = qNEW(sclnjs::rObject);

		tol::Init(*This);
	qRR
    qDELETE(This);
	qRT
	qRE
	}
}

const scli::sInfo &sclnjs::SCLNJSRegister( sclnjs::sRegistrar &Registrar )
{
	static scli::sInfo Info(NAME_LC, NAME_MC, "q37.info");

	Registrar.Register( ReturnArgument_ );                                          // 0
	Registrar.Register( parser::OnData, parser::OnEnd, parser::Parse );             // 1 … 3
	Registrar.Register( stream::upstream::OnData, stream::upstream::OnEnd, stream::downstream::Read, stream::_Set ); // 4 … 7
	Registrar.Register( console_::OnData, console_::OnEnd );                        // 8 … 9
	Registrar.Register( stream_::OnData, stream_::OnEnd, stream_::Read, stream_::Set );            // 10 … 13
	Registrar.Register(dummy_::Read, dummy_::Set);  // 14 … 15

	return Info;
}
