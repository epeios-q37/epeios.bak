/*
	Header for the 'sclmisc' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: sclmisc.h,v 1.7 2013/07/18 19:46:22 csimon Exp $

#ifndef SCLMISC__INC
#define SCLMISC__INC

#define SCLMISC_NAME		"SCLMISC"

#define	SCLMISC_VERSION	"$Revision: 1.7 $"

#define SCLMISC_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLMISCTutor;

#if defined( E_DEBUG ) && !defined( SCLMISC_NODBG )
#define SCLMISC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:22 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe MISCellaneous 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "utf.h"

namespace sclmisc {

	void Initialize(
		flw::iflow__ &LocaleFlow,
		utf::format__ LocaleFormat,
		flw::iflow__ &RegistryFlow,
		utf::format__ RegistryFormat,
		const char *Target,
		const char *LocaleDirectory,
		const char *RegistryDirectory );

	void Initialize(
		const char *Target,
		const char *SuggestedDirectory );

	using fil::GetBackupFileName;

	void CreateBackupFile(
		const char *FileName,
		fil::backup_mode__ Mode = fil::bm_Default );

	void RecoverBackupFile( const char *FileName );

	void ReportFileOpeningErrorAndAbort( const char *FileName );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
