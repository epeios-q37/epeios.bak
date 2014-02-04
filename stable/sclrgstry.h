/*
	'sclrgstry.h' by Claude SIMON (http://zeusw.org/).

	'sclrgstry' is part of the Epeios framework.

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

#ifndef SCLRGSTRY__INC
#define SCLRGSTRY__INC

#define SCLRGSTRY_NAME		"SCLRGSTRY"

#if defined( E_DEBUG ) && !defined( SCLRGSTRY_NODBG )
#define SCLRGSTRY_DBG
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe ReGiSTRY

# include "err.h"
# include "flw.h"

# include "rgstry.h"

namespace sclrgstry {

	using rgstry::tags_;
	using rgstry::tags;

//	extern const lcl::rack__ *LocaleRack;	// A définir.

	rgstry::multi_level_registry_ &GetRegistry( void );

	rgstry::level__ GetConfigurationLevel( void );
	rgstry::level__ GetProjectLevel( void );

	extern rgstry::entry___ Parameters;
	extern rgstry::entry___ Definitions;

	extern rgstry::entry___ Language;

	extern rgstry::entry___ Locale;	// Du fichier de configuration uniquement.

	using rgstry::value;
	using rgstry::value_;

	void LoadConfiguration(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	tol::report__ LoadProject(
		const char *FileName,
		const char *Target,
		rgstry::context___ &Context );

	bso::bool__ IsReady( void );

	void ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry );

	bso::bool__ GetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	void SetValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry );

	void SetValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL );

	bso::bool__ GetValues(
		const rgstry::tentry__ &Entry,
		str::strings_ &Values );

	const str::string_ &GetOptionalValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value,
		bso::bool__ *Missing = NULL );

	const char *GetOptionalValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer,
		bso::bool__ *Missing = NULL );

	const str::string_ &GetMandatoryValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	const char *GetMandatoryValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer );

	bso::bool__ GetBoolean(
		const rgstry::tentry__ &Entry,
		bso::bool__ DefaultValue = false );

	bso::bool__ GetMandatoryBoolean( const rgstry::tentry___ &Entry );

# define SCLRGSTRY__UN( type, name, limit )\
	type GetMandatory##name(\
	const rgstry::tentry__ &Entry,\
	type Limit = limit );\
	type Get##name(\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Limit = limit );

	SCLRGSTRY__UN( bso::uint__, UInt, BSO_UINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLRGSTRY__UN( bso::u64__, U64, BSO_U64_MAX )
# endif
	SCLRGSTRY__UN( bso::u32__, U32, BSO_U32_MAX )
	SCLRGSTRY__UN( bso::u16__, U16, BSO_U16_MAX )
	SCLRGSTRY__UN( bso::u8__, U8, BSO_U8_MAX )

# define SCLRGSTRY__SN( type, name, min, max )\
	type GetMandatory##name(\
	const rgstry::tentry__ &Entry,\
	type Min = min,\
	type Max = max );\
	type Get##name(\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Min = min,\
	type Max = max );

	SCLRGSTRY__SN( bso::sint__, SInt, BSO_SINT_MIN, BSO_SINT_MAX )
# ifdef BSO__64BITS_ENABLED
	SCLRGSTRY__SN( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
	SCLRGSTRY__SN( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
	SCLRGSTRY__SN( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
	SCLRGSTRY__SN( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

		// To define function retrieving mandatory registry value.
# define SCLRGSTRY_MV( name, entry )\
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
# define SCLRGSTRY_OV( name, entry )\
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
