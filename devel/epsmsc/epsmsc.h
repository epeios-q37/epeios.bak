/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// EPeioS MiSC 

#ifndef EPSMSC__INC
#define EPSMSC__INC

#define EPSMSC_NAME		"EPSMSC"

#define	EPSMSC_VERSION	"$Revision: 1.24 $"

#define EPSMSC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( EPSMSC_NODBG )
#define EPSMSC_DBG
#endif

# include "cio.h"

# define EPSMSC_ORGANIZATION	"q37.info"

//d The epeios site URL.
# define EPSMSC_EPEIOS_URL	"http://q37.info/"

//d The gnu URL.
# define EPSMSC_GNU_URL	"http://www.gnu.org/"

# if 0
//d The hypertext link to the Epeios project.
#define EPSMSC_EPEIOS_LINK	"<A HREF=\"" EPSMSC_EPEIOS_URL "\">Epeios project</A>"
# endif

# define EPSMSC_APP_URL( name ) EPSMSC_EPEIOS_URL name "/"

//m The hypertext link to the software named 'Name'.
# define EPSMSC_APP_LINK( name )\
	"<A HREF=" EPSMSC_APP_URL( name ) "\" TARGET=\"_blank\">" name "</A>"

# if 0
//m The invitation to consult the help text for application named 'Name'.
# define EPSMSC_HELP_INVITATION( Name )	"Try '" Name " --help' for more informations." 
#endif

//d The author.
#define EPSMSC_AUTHOR_NAME	"Claude SIMON"

//d Author e-mail
#define EPSMSC_AUTHOR_CONTACT	EPSMSC_EPEIOS_URL "contact.html"

//d Author mail hypertext link.
#define EPSMSC_AUTHOR_LINK	"<A HREF=\"" EPSMSC_AUTHOR_CONTACT "\">" EPSMSC_AUTHOR_NAME "</A>"

# if 0
//d GNU (http://www.gnu.org) Text.
#define EPSMSC_GNU_PROJECT_AFFILIATION "\tThis program is part of the GNU project, released\n\tunder the aegis of GNU (" EPSMSC_GNU_URL ").\n\tIt uses Epeios (" EPSMSC_EPEIOS_URL ") technologies."

//d Epeios (http://q37.info/epeios/) text.
#define EPSMSC_EPEIOS_PROJECT_AFFILIATION "This program is part of the Epeios project (" EPSMSC_EPEIOS_URL ")." 

//d Epeios (http://q37.info/epeios/) framework.
#define EPSMSC_EPEIOS_FRAMEWORK_DEPENDENCE "Relies massively on the Epeios framework (" EPSMSC_EPEIOS_URL ")." 
# endif

//m Copyright text with yers 'Years' (string) for Epeios.
#define EPSMSC_COPYRIGHT( years )	"Copyright (c) " years " " EPSMSC_AUTHOR_NAME " (" EPSMSC_AUTHOR_CONTACT ")"

							
namespace epsmsc {
	//f Print information about GNU GPL license.
	void PrintLicense(
		const char *Name,
		txf::text_oflow__ &Flow = cio::COut );
}

/*$END$*/
#endif
