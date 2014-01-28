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
	enum command {
		cHelp,
		cVersion,
		cLicense,
		c_amount
	};

	// A définir par l'utilisateur.
	extern const char *TargetName;

	const char *GetLanguage( void );

	void AddDefaultCommands( clnarg::description_ &Description );

	void OldPrintDefaultCommandDescriptions(
		const char *ProgramName,
		const clnarg::description_ &Description );

	void PrintDefaultCommandDescriptions( const char *ProgramName );

	const str::string_ &GetArgumentShortLong(
		const str::string_ &Id,
		str::string_ &ShortLong );

	inline const str::string_ &GetArgumentShortLong(
		const char *Id,
		str::string_ &ShortLong )
	{
		return GetArgumentShortLong( str::string( Id ), ShortLong );
	}

	const str::string_ &GetArgumentDescriptionTranslation(
		const str::string_ &Id,
		str::string_ &Translation );

	inline const str::string_ &GetArgumentDescriptionTranslation(
		const char *Id,
		str::string_ &Translation )
	{
		return GetArgumentDescriptionTranslation( str::string( Id ), Translation );
	}

	bso::int__ GetFreeArgumentsAmount( void );

	const str::string_ &GetFreeArgument(
		bso::uint__ Indice,
		str::string_ &Argument );

	void PutFreeArgumentTo(
		bso::uint__ Indice,
		const rgstry::tentry__ &Entry );

	void PrintCommandDescription(
		const char *CommandId,
		const char *OptionsAndFlags = NULL );

	void PrintFlagsAndOptionsHeader( void );

	void PrintFlagDescription( const char *FlagId );

	void PrintOptionDescription(
		const char *Label,
		const char *OptionId );

	void PrintArgumentsHeader( void );

	void PrintArgumentDescription(
		const char *Label,
		const char *ArgumentDescriptionLabel );


	// A définir par l'utilisateur.
	void Main(
		int argc,
		const char *argv[] );

	void LoadProject(
		const char *FileName,
		const char *Target );

	void LoadProject(
		const str::string_ &FileName,
		const char *Target );

	inline const rgstry::multi_level_registry_ &GetRegistry( void )
	{
		return sclrgstry::GetRegistry();
	}

	inline rgstry::level__ GetRegistryConfigurationLevel( void )
	{
		return sclrgstry::GetConfigurationLevel();
	}

	inline rgstry::level__ GetRegistryProjectLevel( void )
	{
		sclrgstry::GetProjectLevel();
	}

	rgstry::level__ GetRegistrySetupLevel( void );

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

	inline void ReportAndAbort( const lcl::meaning_ &Meaning )
	{
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	void ReportAndAbort( const char *Text );

	void ReportUnknownOptionErrorAndAbort( const char *Option );

	void ReportWrongNumberOfArgumentsErrorAndAbort( void );

	void ReportMissingCommandErrorAndAbort( void );


	const str::string_ &GetCommand( str::string_ &Command );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
