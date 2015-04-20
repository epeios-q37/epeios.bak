/*
	'scltool.h' by Claude SIMON (http://zeusw.org/).

	'scltool' is part of the Epeios framework.

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

#ifndef SCLTOOL__INC
# define SCLTOOL__INC

# define SCLTOOL_NAME		"SCLTOOL"

# if defined( E_DEBUG ) && !defined( SCLTOOL_NODBG )
#  define SCLTOOL_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe TOOL

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "clnarg.h"

# include "sclmisc.h"
# include "scllocale.h"
# include "sclerror.h"
# include "sclrgstry.h"

namespace scltool {

# if 0	// Obsolete ?
	enum command {
		cHelp,
		cVersion,
		cLicense,
		c_amount
	};
# endif
	// Facilite la gestion des fichiers de sorties optionel (si nom de fichier fourni, donne un flux texte vers ce fichier, sinon le flux texte de la sortie standard).
	class text_oflow_rack___
	{
	private:
		fnm::name___ _FileName;
		bso::bool__ _BackedUp;
		flf::file_oflow___ _Flow;
		txf::text_oflow__ _TFlow;
	public:
		void reset( bso::bool__ P = true )
		{
			_BackedUp = false;
			_FileName.reset( P );
			_TFlow.reset( P );
			_Flow.reset( P );
		}
		E_CDTOR( text_oflow_rack___ );
		txf::text_oflow__ &Init( const fnm::name___ &FileName );
		void HandleError( void );	// A appeler  partir de 'ERRErr'.
	};

	struct oddities__ {
# ifdef CPE_WIN
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
#  ifdef CPE_MSVC
		PWSTR pCmdLine;
#  elif defined( CPE_MINGW )
		LPSTR lpCmdLine;
#  else
#   error
#  endif
		int nCmdShow;
		LPWSTR *argv;
# else
		ntvstr::char__ **argv;
# endif
		int argc;
	};

	// A dfinir par l'utilisateur.
	int SCLTOOLMain(
		const str::string_ &Command,
		const oddities__ &Oddities );

	extern bso::bool__ IgnoreUnknownArguments;	// A 'false' par dfaut, mais peut tre modifi par l'utilisateur (p. ex. dans l'utilisation de 'CEF').
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
