/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

	bso::bool__ IsInitialized( void );

	void RefreshBaseLanguage( void );	// Set the base langauge (returned by above function) with the content of the registry.

	const char *GetBaseLanguage( void );	// Language d'administration, pouvant être language utilsateur selon contexte.

	inline const str::string_ &GetBaseTranslation(
		const char *Text,
		str::string_ &Translation )
	{
		return scllocale::GetTranslation( Text, GetBaseLanguage(), Translation );
	}

	inline const str::string_ &GetBaseTranslation(
		const lcl::meaning_ &Meaning,
		str::string_ &Translation )
	{
		return scllocale::GetTranslation( Meaning, GetBaseLanguage(), Translation );
	}

	inline bso::bool__ GetSCLBasePendingErrorTranslation(
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default )
	{
		return sclerror::GetPendingErrorTranslation( GetBaseLanguage(), Translation, ErrHandling );
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
		err::err___ *qRRor,
		sclerror::error___ *SCLError,
		const cio::set__ &CIO,
		const rgstry::entry__ &Configuration,
		const rgstry::entry__ &Locale );

	void Initialize(
		err::err___ *qRRor,
		sclerror::error___ *SCLError,
		const cio::set__ &CIO,
		xtf::extended_text_iflow__ &LocaleFlow,
		const char *LocaleDirectory,
		xtf::extended_text_iflow__ &RegistryFlow,
		const char *RegistryDirectory );

	void Initialize(
		err::err___ *qRRor,
		sclerror::error___ *SCLError,
		const cio::set__ &CIO,
		const fnm::name___ &SuggestedDirectory );

	void EraseProjectRegistry( void );

	enum project_type__ {
		ptNew,			// Empty project.
		ptPredefined,	// Use of a project defined in the 'Definitions' section in the configuration file.
		ptRemote,		// Project stored in a file.
		ptEmbedded,		// The content of the project is embedded in the its declaration (not implemented yet)
		pt_amount,
		pt_Undefined
	};

	const char *GetLabel( project_type__ ProjectType );

	project_type__ GetProjectType( const str::string_ &Pattern );

	void LoadProject(
		flw::iflow__ &Flow,
		str::string_ &Id );

	void LoadProject(
		const fnm::name___ &FileName,
		str::string_ &Id );

	void LoadProject(
		project_type__ ProjectType,
		const str::string_ &ProjectFeature );

	void LoadProject( void );	// Load project, if applies, following configuration file indications.

	using fil::GetBackupFilename;

	void CreateBackupFile(
		const fnm::name___ &FileName,
		fil::backup_mode__ Mode = fil::bm_Default );

	void RecoverBackupFile( const fnm::name___ &FileName );

	void ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName );

	void Load(
		const fnm::name___ &FileName,
		str::string_ &String );

	void LoadAndTranslateTags(
		const fnm::name___ &FileName,
		const char *Language,
		str::string_ &String,
		char Marker = scllocale::DefaultMarker );

	void Load(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String );

	void LoadAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String,
		char Marker = scllocale::DefaultMarker );

	void LoadXMLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String,
		char Marker = scllocale::DefaultMarker );

	sclrgstry::registry_ &GetRegistry( void );

	rgstry::level__ GetRegistryConfigurationLevel( void );
	rgstry::level__ GetRegistryProjectLevel( void );
	rgstry::level__ GetRegistrySetupLevel( void );
	rgstry::level__ GetRegistryArgumentsLevel( void );

	inline void AddValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry )
	{
		return sclrgstry::AddValue( GetRegistry(), Value, Entry );
	}

	inline void AddValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
		return sclrgstry::AddValue( GetRegistry(), Path, Value, Error );
	}

	inline void SetValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry )
	{
		return sclrgstry::SetValue( GetRegistry(), Value, Entry );
	}

	inline void SetValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
		return sclrgstry::SetValue( GetRegistry(), Path, Value, Error );
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

	inline bso::bool__ OGetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value )
	{
		return sclrgstry::OGetValue( GetRegistry(), Entry, Value );
	}

	inline const char *OGetValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer )	// Returns NULL when entry missing.
	{
		return sclrgstry::OGetValue( GetRegistry(), Entry, Buffer );
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

# define SCLMISC__UN( type, name, limit )\
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

	SCLMISC__UN( bso::uint__, UInt, BSO_UINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLRGSTRY__UN( bso::u64__, U64, BSO_U64_MAX )
# endif
		SCLMISC__UN( bso::u32__, U32, BSO_U32_MAX )
		SCLMISC__UN( bso::u16__, U16, BSO_U16_MAX )
		SCLMISC__UN( bso::u8__, U8, BSO_U8_MAX )

# define SCLMISC__SN( type, name, min, max )\
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

		SCLMISC__SN( bso::sint__, SInt, BSO_SINT_MIN, BSO_SINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLRGSTRY__SN( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
		SCLMISC__SN( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
		SCLMISC__SN( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
		SCLMISC__SN( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

		// To define function retrieving mandatory registry value.
# define SCLMISC_MV( name, entry )\
	inline const char *name(\
		TOL_CBUFFER___ &Buffer )\
		{\
		return sclmisc::MGetValue( entry, Buffer );\
		}\
	inline const str::string_ &name( str::string_ &Value )\
		{\
		return sclmisc::MGetValue( entry, Value );\
		}

		// To define function retrieving optional registry value.
# define SCLMISC_OV( name, entry )\
	inline const char *name( TOL_CBUFFER___ &Buffer )\
		{\
		return sclmisc::OGetValue( entry, Buffer );\
		}\
	inline bso::bool__ name( str::string_ &Value )\
		{\
		return sclmisc::OGetValue( entry, Value );\
		}

	// Façon standard de récupèrer un plugin.
	const str::string_ &GetPluginFeatures(
		const char *Target,
		str::string_ &Filename,
		rgstry::entry__ &Configuration,
		rgstry::entry__ &Locale );

	template <typename retriever> inline void Plug(
		const char *Target,
		retriever &PluginRetriever )
	{
	qRH
		str::string Filename;
		rgstry::entry__ Configuration, Locale;
	qRB
		Filename.Init();
		Configuration.Init();
		Locale.Init();

		GetPluginFeatures( Target, Filename, Configuration, Locale );
		PluginRetriever.Init( Filename, Configuration, Locale );
	qRR
	qRT
	qRE
	}

}

			/********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
