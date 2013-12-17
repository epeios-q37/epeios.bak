/*
	'sclmisc.h' by Claude SIMON (http://zeusw.org/).

	'sclmisc' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCLMISC__INC
# define SCLMISC__INC

# define SCLMISC_NAME		"SCLMISC"

# if defined( E_DEBUG ) && !defined( SCLMISC_NODBG )
#  define SCLMISC_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe MISCellaneous

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
		const fnm::name___ &FileName,
		fil::backup_mode__ Mode = fil::bm_Default );

	void RecoverBackupFile( const fnm::name___ &FileName );

	void ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
