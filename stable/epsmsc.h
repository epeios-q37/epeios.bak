/*
	Header for the 'epsmsc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: epsmsc.h,v 1.24 2012/12/13 11:36:01 csimon Exp $

#ifndef EPSMSC__INC
#define EPSMSC__INC

#define EPSMSC_NAME		"EPSMSC"

#define	EPSMSC_VERSION	"$Revision: 1.24 $"

#define EPSMSC_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &EPSMSCTutor;

#if defined( E_DEBUG ) && !defined( EPSMSC_NODBG )
#define EPSMSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.24 $
//C Claude SIMON
//R $Date: 2012/12/13 11:36:01 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D EPeioS MiSC 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "cio.h"

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
	void PrintLicense( txf::text_oflow__ &Flow = cio::COut );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
