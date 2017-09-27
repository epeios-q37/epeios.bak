/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of rncq.

	rncq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	rncq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with rncq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "rncqjre.h"

#include "registry.h"

#include "rnctol.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "scljre.h"

void scljre::SCLJREInfo( txf::sOFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLJRE_F( ReturnArgument_ )
	{
		scljre::sJObject Return;
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.Get( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Return = scljre::String( Text );
	qRR
	qRT
	qRE
		return Return;
	}

	namespace {
		scljre::sJObject Evaluate_(
			scljre::sCaller &Caller,
			bso::sBool RPN )
		{
			scljre::sJObject Object = NULL;
		qRH
			str::wString Expression, Result;
		qRB
			tol::Init( Expression, Result );
			Caller.Get( Expression );

			if ( !rnctol::Evaluate( Expression, RPN, Result ) )
				sclmisc::GetBaseTranslation( "BadExpression", Result );

			Object = scljre::String( Result );
		qRR
		qRT
		qRE
			return Object;
		}
	}

	SCLJRE_F( EvaluateALG_ )
	{
		return Evaluate_( Caller, false );
	}

	SCLJRE_F( EvaluateRPN_ )
	{
		return Evaluate_( Caller, true );
	}
}

void scljre::SCLJRERegister( scljre::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( EvaluateALG_, EvaluateRPN_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
