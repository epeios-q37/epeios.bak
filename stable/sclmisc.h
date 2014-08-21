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
# include "rgstry.h"
# include "scllocale.h"
# include "sclerror.h"
# include "sclrgstry.h"

namespace sclmisc {

	extern const char *SCLMISCTargetName;	// A définir par l'utilisateur.

	const char *GetLanguage( void );

	inline const str::string_ &GetTranslation(
		const char *Text,
		str::string_ &Translation )
	{
		return scllocale::GetTranslation( Text, GetLanguage(), Translation );
	}

	inline const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		str::string_ &Translation )
	{
		return scllocale::GetTranslation( Meaning, GetLanguage(), Translation );
	}

	inline bso::bool__ GetSCLPendingError(
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default )
	{
		return sclerror::GetPendingError( GetLanguage(), Translation, ErrHandling );
	}

	void ReportAndAbort( const lcl::meaning_ &Meaning );

	void ReportAndAbort( const char *Text );

	void ReportAndAbort(
		const char *Text,
		const str::string_ &Tag );	// Pour simplifier le cas où le message a un seul 'tag'. Pour plus d'un tag, utiliser la version avec un 'lcl::meaning_'.

	void ReportParsingErrorAndAbort(
		const char *ErrorLabel,
		const rgstry::context___ &Context );

	void Initialize(
		xtf::extended_text_iflow__ &LocaleFlow,
		const char *LocaleDirectory,
		xtf::extended_text_iflow__ &RegistryFlow,
		const char *RegistryDirectory );

	void Initialize( const fnm::name___ &SuggestedDirectory );

	void EraseProjectRegistry( void );

	void LoadProject(
		const fnm::name___ &FileName,
		str::string_ &Id );

	using fil::GetBackupFileName;

	void CreateBackupFile(
		const fnm::name___ &FileName,
		fil::backup_mode__ Mode = fil::bm_Default );

	void RecoverBackupFile( const fnm::name___ &FileName );

	void ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName );

	// Facilite la gestion des fichiers de sorties optionel (si nom de fichier fourni, donne un flux texte vers ce fichier, sinon le flux texte de la sortie standard).
	class text_oflow_rack___ {
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
		void HandleError( void );	// A appeler à partir de 'ERRErr'.
	};

	void Load(
		const fnm::name___ &FileName,
		str::string_ &String );

	void Load(
		const rgstry::entry___ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
