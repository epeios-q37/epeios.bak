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

# if 0	// Oboslete ?
	enum command {
		cHelp,
		cVersion,
		cLicense,
		c_amount
	};
# endif

	struct oddities__ {
# ifdef CPE_WIN
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		PWSTR pCmdLine;
		int nCmdShow;
		LPWSTR *argv;
# else
		ntvstr::char__ **argv;
# endif
		int argc;
	};

	// A définir par l'utilisateur.
	int SCLTOOLMain(
		const str::string_ &Command,
		const oddities__ &Oddities );

	inline sclrgstry::registry_ &GetRegistry( void );

	rgstry::level__ GetRegistryConfigurationLevel( void );
	rgstry::level__ GetRegistryProjectLevel( void );
	rgstry::level__ GetRegistrySetupLevel( void );
	rgstry::level__ GetRegistryArgumentsLevel( void );

	inline void SetValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry )
	{
		return sclrgstry::SetValue(GetRegistry(), Value, Entry );
	}

	inline void SetValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
		return sclrgstry::SetValue(GetRegistry(), Path, Value, Error );
	}

	inline bso::bool__ BGetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value )
	{
		return sclrgstry::BGetValue( GetRegistry(), Entry, Value );
	}

	inline bso::bool__ GetValues(
		const rgstry::tentry__ &Entry,
		str::strings_ &Values )
	{
		return sclrgstry::GetValues( GetRegistry(), Entry, Values );
	}

	inline const str::string_ &OGetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value,
		bso::bool__ *Missing = NULL )
	{
		return sclrgstry::OGetValue( GetRegistry(), Entry, Value, Missing );
	}

	inline const char *OGetValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer,
		bso::bool__ *Missing = NULL )
	{
		return sclrgstry::OGetValue( GetRegistry(), Entry, Buffer, Missing );
	}

	inline const str::string_ &MGetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value )
	{
		return sclrgstry::MGetValue( GetRegistry(), Entry, Value );
	}

	inline const char *MGetValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer )
	{
		return sclrgstry::MGetValue( GetRegistry(), Entry, Buffer );
	}

	inline bso::bool__ BGetBoolean(
		const rgstry::tentry__ &Entry,
		bso::bool__ DefaultValue = false )
	{
		return sclrgstry::BGetBoolean( GetRegistry(), Entry, DefaultValue );
	}

	inline bso::bool__ MGetBoolean( const rgstry::tentry___ &Entry )
	{
		return sclrgstry::MGetBoolean( GetRegistry(), Entry );
	}

# define SCLTOOL__UN( type, name, limit )\
	inline type MGet##name(\
		const rgstry::tentry__ &Entry,\
		type Limit = limit )\
	{\
		return sclrgstry::MGet##name( GetRegistry(), Entry, Limit );\
	}\
	inline type OGet##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit = limit )\
	{\
		return sclrgstry::OGet##name( GetRegistry(), Entry, DefaultValue, Limit );\
	}

	SCLTOOL__UN( bso::uint__, UInt, BSO_UINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLRGSTRY__UN( bso::u64__, U64, BSO_U64_MAX )
# endif
	SCLTOOL__UN( bso::u32__, U32, BSO_U32_MAX )
	SCLTOOL__UN( bso::u16__, U16, BSO_U16_MAX )
	SCLTOOL__UN( bso::u8__, U8, BSO_U8_MAX )

# define SCLTOOL__SN( type, name, min, max )\
	inline type MGet##name(\
		const rgstry::tentry__ &Entry,\
		type Min = min,\
		type Max = max )\
	{\
		return sclrgstry::MGet##name( GetRegistry(), Entry, Min, Max );\
	}\
	inline type OGet##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min = min,\
		type Max = max )\
	{\
		return sclrgstry::OGet##name( GetRegistry(), Entry, DefaultValue, Min, Max );\
	}

	SCLTOOL__SN( bso::sint__, SInt, BSO_SINT_MIN, BSO_SINT_MAX )
# ifdef BSO__64BITS_ENABLED
	SCLRGSTRY__SN( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
	SCLTOOL__SN( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
	SCLTOOL__SN( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
	SCLTOOL__SN( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

		// To define function retrieving mandatory registry value.
# define SCLTOOL_MV( name, entry )\
	inline const char *name(\
		TOL_CBUFFER___ &Buffer )\
	{\
		return scltool::MGetValue( entry, Buffer );\
	}\
	inline const str::string_ &name( str::string_ &Value )\
	{\
		return scltool::MGetValue( entry, Value );\
	}

		// To define function retrieving optional registry value.
# define SCLTOOL_OV( name, entry )\
	inline const char *name(\
		TOL_CBUFFER___ &Buffer,\
		bso::bool__ *Missing = NULL )\
	{\
		return scltool::OGetValue( entry, Buffer, Missing );\
	}\
	inline const str::string_ &name(\
		str::string_ &Value,\
		bso::bool__ *Missing = NULL )\
	{\
		return scltool::OGetValue( entry, Value, Missing );\
	}

	extern bso::bool__ IgnoreUnknownArguments;	// A 'false' par défaut, mais peut être modifié par l'utilisateur (p. ex. dans l'utilisation de 'CEF').
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
