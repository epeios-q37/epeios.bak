/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parser.h"
#include "stream.h"

#include "sclmisc.h"

#include "cio.h"
#include "sclnjs.h"
#include "nodeq.h"
#include "xpp.h"
#include "mtk.h"

# define NAME_MC			"XPPqNJS"
# define NAME_LC			"xppqnjs"
# define NAME_UC			"XPPQNJS"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	
# define VERSION			"0.1.3"

void sclnjs::SCLNJSRegister( sclnjs::sRegistrar &Registrar )
{
	Registrar.Register( stream::Set );
	Registrar.Register( parser::Parse );
}

SCLNJS_MODULE( xppq );

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
const char *sclnjs::SCLNJSProductVersion = VERSION;
