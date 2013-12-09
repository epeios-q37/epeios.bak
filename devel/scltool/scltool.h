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

	void PrintDefaultCommandDescriptions(
		const char *ProgramName,
		const clnarg::description_ &Description );

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

	const rgstry::multi_level_registry_ &GetRegistry( void );

	rgstry::level__ GetRegistryConfigurationLevel( void );
	rgstry::level__ GetRegistryProjectLevel( void );
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

	bso::bool__ GetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	bso::bool__ GetValues(
		const rgstry::tentry__ &Entry,
		str::strings_ &Values );

	const str::string_ &GetOptionalValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value,
		bso::bool__ *Missing = NULL );

	const char *GetOptionalValue(
		const rgstry::tentry__ &Entry,
		STR_BUFFER___ &Buffer,
		bso::bool__ *Missing = NULL );

	const str::string_ &GetMandatoryValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	const char *GetMandatoryValue(
		const rgstry::tentry__ &Entry,
		STR_BUFFER___ &Buffer );

# define SCLTOOL__UN( type, name, limit )\
	type GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Limit = limit );\
	type Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit = limit );

	SCLTOOL__UN( bso::uint__, UInt, BSO_UINT_MAX )
# ifdef BSO__64BITS_ENABLED
	SCLTOOL__UN( bso::u64__, U64, BSO_U64_MAX )
# endif
	SCLTOOL__UN( bso::u32__, U32, BSO_U32_MAX )
	SCLTOOL__UN( bso::u16__, U16, BSO_U16_MAX )
	SCLTOOL__UN( bso::u8__, U8, BSO_U8_MAX )

# define SCLTOOL__SN( type, name, min, max )\
	type GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Min = min,\
		type Max = max );\
	type Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min = min,\
		type Max = max );

	SCLTOOL__SN( bso::sint__, SInt, BSO_SINT_MIN, BSO_SINT_MAX )
# ifdef BSO__64BITS_ENABLED
	SCLTOOL__SN( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
	SCLTOOL__SN( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
	SCLTOOL__SN( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
	SCLTOOL__SN( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

// To define function retrieving mandatory registry value.
# define SCLTOOL_MV( name, entry )\
	inline const char *name(\
		STR_BUFFER___ &Buffer )\
	{\
		return GetMandatoryValue( entry, Buffer );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value )\
	{\
		return GetMandatoryValue( entry, Value );\
	}

// To define function retrieving optional registry value.
# define SCLTOOL_OV( name, entry )\
	inline const char *name(\
		STR_BUFFER___ &Buffer,\
		bso::bool__ *Missing = NULL )\
	{\
		return GetOptionalValue( entry, Buffer, Missing );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value,\
		bso::bool__ *Missing = NULL )\
	{\
		return GetOptionalValue( entry, Value, Missing );\
	}

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
