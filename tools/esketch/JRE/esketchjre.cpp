/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of esketch.

	esketch is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	esketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with esketch. If not, see <http://www.gnu.org/licenses/>.
*/

#include "jre.old.h"

#include "sclmisc.h"
#include "scljre.old.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

# define NAME_MC			"eSketchJRE"
# define NAME_LC			"esketchjre"
# define NAME_UC			"ESKETCHJRE"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"

SCLJRE_DEF( eSketch );

namespace {
	jobject getText_(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		jre::sString JString;
	qRH
		str::wString Text;
	qRB
		Text.Init();
		sclmisc::GetBaseTranslation( "TheText", Text );

		JString.Init( Text );
	qRR
	qRT
	qRE
		return JString;
	}
}

void scljre::SCLJRERegister( sRegistrar &Registrar )
{
	Registrar.Register( getText_  );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
const char *scljre::SCLJREProductVersion = VERSION;
