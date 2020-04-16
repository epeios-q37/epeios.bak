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

// SoCLe Registry

#ifndef SCLR_INC_
# define SCLR_INC_

# define SCLR_NAME		"SCLR"

# if defined( E_DEBUG ) && !defined( SCLR_NODBG )
#  define SCLR_DBG
# endif

# include "err.h"
# include "rgstry.h"

namespace sclr {

	using rgstry::tags_;
	using rgstry::tags;
	using rgstry::rEntry;

	typedef rgstry::multi_layer_registry_ registry_;

	extern const char *ParametersTag;

#if 0
	registry_ &GetRegistry( void );
# endif

	registry_ &GetRWCommonRegistry( void );
	const registry_ &GetCommonRegistry( void );

	tht::rLocker &GetCommonRegistryLocker( void );

	qENUM( Layer ) {
		lMain,
		lLasting,	// Lasting data set by application.
		lProject,
		lSetup,
		lArguments,
		lRuntime,	// Data set by application, but which are lost when quitting (contrary of the data of 'Lasting', which are persistent.
		l_amount,
		l_Highest = lRuntime,
		l_Default = l_Highest,
		l_Undefined
	};

	const char *GetLabel( eLayer Layer );

	rgstry::layer__ GetRawLayer( eLayer Layer );

	extern rgstry::entry___ Parameters;
	extern rgstry::entry___ Definitions;
	extern rgstry::entry___ Locale;
	extern rgstry::entry___ Arguments;
	extern rgstry::entry___ Debug;

	namespace parameter {
		extern rgstry::entry___ Language;

		extern rgstry::entry___ Project;

		namespace project{
			extern rgstry::entry___ Type;
			extern rgstry::entry___ Feature;
		}

		// 'Parameters/Plugins/Plugin[target="..."]'.
		extern rgstry::entry___ TargetedPlugin;

		namespace targeted_plugin {
			extern rgstry::entry___ Id;
		}

		// 'Parameters/Plugins/Set[target="..."]/Plugin'.
		extern rgstry::entry___ LoosePluginItem;

		namespace loose_plugin_item {
			extern rgstry::entry___ Id;
		}

		// 'Parameters/Plugins/Set[target="..."]/Plugin[Id="..."'.
		extern rgstry::entry___ PluginItem;

		namespace debug {
			extern rgstry::rEntry DumpRegistries;
			extern rgstry::rEntry DumpLocales;
		}
	}

	namespace definition {
		extern rgstry::entry___ DefaultProjectId;

		extern rgstry::entry___ Project;

		namespace project {
			extern rgstry::entry___ Id;
		}

		extern rgstry::entry___ TaggedProject;

		extern rgstry::entry___ Plugin;

		namespace plugin {
			extern rgstry::entry___ Filename;
			extern rgstry::entry___ Configuration;
			extern rgstry::entry___ Locale;
		};
	}

	using rgstry::value;
	using rgstry::value_;

	const char *GetLanguage(
		const registry_ &Registry,
		TOL_CBUFFER___ &Buffer );

	inline void BuildRootPath(
		const char *Subject,
		const char *Target,
		str::string_ &Path )
	{
	/*	Path.Append( Subject );
		Path.Append( "s/" );
	*/	Path.Append( Subject );
		Path.Append( "[target=\"" );
		Path.Append( Target );
		Path.Append( "\"]" );
	}

	void Erase( eLayer Layer );

	void Reset( eLayer Layer );

	void Reset(
		eLayer Layer,
		const rgstry::entry__ &Entry );

	void Load(
		eLayer Layer,
		xtf::extended_text_iflow__ &Flow,
		const fnm::name___ &Directory,
		const char *RootPath );

	void Load(
		eLayer Layer,
		flw::rRFlow &Flow,
		const fnm::name___ &Directory,
		const char *RootPath );

	void Load(
		eLayer Layer,
		const fnm::name___ &Filename,
		const char *Target,
		str::string_ &Id );

	void LoadProject(
		flw::iflow__ &Flow,
		const char *Target,
		const fnm::name___ &Directory,
		str::string_ &Id );

	void LoadProject(
		const fnm::name___ &Filename,
		const char *Target,
		str::string_ &Id );

	void FillWithSetupOfId(
		registry_ &Registry,
		rgstry::layer__ layer,
		const str::string_ &Id );

	void FillWithGivenSetup(
		registry_ &Registry,
		rgstry::layer__ Layer );	// Fill the indicated setup if one is given.

	void ReportIfNoParametersSetupId( void );

	void FillWithContent(
		registry_ &Registry,
		rgstry::layer__ Layer,
		const str::dString &BinPath,
		const str::string_ &Content );

# if 0
	void FillRegistryWithSetup(
		registry_ &Registry,
		layer__ Layer,	// 'Layer' de le 'setup registry'.
		const str::string_ &SetupId );	// Remplit la section 'Parameters' de la 'registry' avec le contenu du 'Setup' d'identifiant 'SetupId'.

	inline void FillRegistryWithSetup(
		registry_ &Registry,
		layer__ Layer )	// Remplit la section 'Parameters' de la 'registry' avec le contenu du 'Setup' par dÃ©faut ('Parameters/@DefaultSetup').
	{
		FillRegistryWithSetup( Registry, Layer, str::string() );
	}
# endif

	void ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry );

	void AddValue(
		registry_ &Registry,
		const str::string_ &Value,
		const rgstry::tentry__ &Entry );

	void AddValue(
		registry_ &Registry,
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL );

	void SetValue(
		registry_ &Registry,
		const str::string_ &Value,
		const rgstry::tentry__ &Entry );

	void SetValue(
		registry_ &Registry,
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL );

	bso::bool__ GetValues(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		str::dStrings &Values );

	bso::bool__ OGetValue(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	const char *OGetValue(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer );	// Returns NULL when entry missing.

	const str::string_ &MGetValue(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		str::string_ &Value );

	const char *MGetValue(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer );

	inline bso::bool__ BGetValue(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		str::string_ &Value,
		qRPN )
    {
        if ( !OGetValue(Registry, Entry, Value)) {
            if ( qRPT )
                qRFwk();
            else
                return false;
        }

        return true;
    }

    bso::bool__ BGetBoolean(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		bso::bool__ DefaultValue,
		qRPN );

    inline bso::bool__ OGetBoolean(
		const registry_ &Registry,
		const rgstry::tentry__ &Entry,
		bso::bool__ DefaultValue = false )
    {
        return BGetBoolean(Registry, Entry, DefaultValue, qRPU);
    }

	bso::bool__ MGetBoolean(
		const registry_ &Registry,
		const rgstry::tentry___ &Entry );

# define SCLR_UN_( type, name, limit )\
	type MGet##name(\
		const registry_ &Registry,\
		const rgstry::tentry__ &Entry,\
		type Limit = limit );\
	type OGet##name(\
		const registry_ &Registry,\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit = limit );

	SCLR_UN_( bso::uint__, UInt, BSO_UINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLRG_UN_( bso::u64__, U64, BSO_U64_MAX )
# endif
	SCLR_UN_( bso::u32__, U32, BSO_U32_MAX )
	SCLR_UN_( bso::u16__, U16, BSO_U16_MAX )
	SCLR_UN_( bso::u8__, U8, BSO_U8_MAX )

# define SCLR_SN_( type, name, min, max )\
	type MGet##name(\
		const registry_ &Registry,\
		const rgstry::tentry__ &Entry,\
		type Min = min,\
		type Max = max );\
	type OGet##name(\
		const registry_ &Registry,\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min = min,\
		type Max = max );

	SCLR_SN_( bso::sint__, SInt, BSO_SINT_MIN, BSO_SINT_MAX )
# ifdef BSO__64BITS_ENABLED
	SCLR_SN_( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
	SCLR_SN_( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
	SCLR_SN_( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
	SCLR_SN_( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

		// To define function retrieving mandatory registry value.
# define SCLR_MV( name, entry )\
		const registry_ &Registry,\
		inline const char *name(\
		TOL_CBUFFER___ &Buffer )\
	{\
		return MGetValue( Registry, entry, Buffer );\
	}\
	inline const str::string_ &name(\
		const registry_ &Registry,\
		str::string_ &Value )\
	{\
		return MGetValue( Registry, entry, Value );\
	}

		// To define function retrieving optional registry value.
# define SCLR_OV( name, entry )\
	inline const char *name(\
		const registry_ &Registry,\
		TOL_CBUFFER___ &Buffer,\
		bso::bool__ *Missing = NULL )\
	{\
		return OGetValue( Registry, entry, Buffer, Missing );\
	}\
	inline const str::string_ &name(\
		const registry_ &Registry,\
		str::string_ &Value,\
		bso::bool__ *Missing = NULL )\
	{\
		return OGetValue( Registry, entry, Value, Missing );\
	}
}

/***************/
/***** NEW *****/
/***************/

namespace sclr {
	typedef registry_ dRegistry;
	qW( Registry );

	typedef rgstry::entry___ rEntry;}

#endif
