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

// SoCLe MISCellaneous

#ifndef SCLMISC__INC
# define SCLMISC__INC

# define SCLMISC_NAME		"SCLMISC"

# if defined( E_DEBUG ) && !defined( SCLMISC_NODBG )
#  define SCLMISC_DBG
# endif

# include "err.h"
# include "flw.h"
# include "utf.h"
# include "rgstry.h"
# include "scllocale.h"
# include "sclerror.h"
# include "scli.h"
# include "sclrgstry.h"

# include "plgn.h"

/***************/
/***** OLD *****/
/***************/

namespace sclmisc {

	bso::bool__ IsInitialized( void );

	const str::dString &GetBinPath( void );

	void SetBaseLanguage( const str::dString &Language );	// Set the base language.

	void RefreshBaseLanguage( void );	// Set the base language with the content of the registry.

	const char *GetBaseLanguage( void );	// Admin. language ; could also be the user language under some circumstances.

	template <typename source> inline const str::string_ &GetBaseTranslation(
		const source &Source,
		str::dString &Translation )
	{
		return scllocale::GetTranslation( Source, GetBaseLanguage(), Translation );
	}

	template <typename source, typename ... tags> inline const str::string_ &GetBaseTranslation(
		const source &Source,
		str::dString &Translation,
		const tags &... Tags )
	{
		return scllocale::GetTranslation( Source, GetBaseLanguage(), Translation, Tags... );
	}

	inline bso::bool__ GetSCLBasePendingErrorTranslation(
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default )
	{
		return sclerror::GetPendingErrorTranslation( GetBaseLanguage(), Translation, ErrHandling );
	}

	bso::sBool DisplaySCLBasePendingError( txf::sWFlow &Flow = cio::CErr );

	// To use as 'action' parameter for the 'qRFE(...)' macro to display the file/line of an error.
	void ErrFinal( void );

	// Simplifies the use of below variadics.
	void ReportAndAbort_( const lcl::meaning_ &Meaning );

	inline void ReportAndAbort( const lcl::meaning_ &Meaning )
	{
		ReportAndAbort_( Meaning );
	}

	inline void ReportAndAbort( const lcl::meaning &Meaning )
	{
		ReportAndAbort_( Meaning );
	}

	void ReportAndAbort( const char *Text );

	// Variadics handling.
	namespace report_and_abort_ {
		inline void Add( const lcl::meaning_ &Meaning )
		{
			// Nothing to do.
		}

		template <typename tag> inline void Add(
			lcl::meaning_ &Meaning,
			const tag &Tag )
		{
			Meaning.AddTag( Tag );
		}

		template <typename tag, typename... tags> inline void Add(
			lcl::meaning_ &Meaning,
			const tag &Tag,
			const tags&... Tags )
		{
			Add( Meaning, Tag );

			Add( Meaning, Tags... );
		}
	}

	template <typename text, typename... tags> inline void ReportAndAbort(
		const text &Text,
		const tags&... Tags )
	{
	qRH
		lcl::wMeaning Meaning;
	qRB
		Meaning.Init();

		Meaning.SetValue( Text );

		report_and_abort_::Add( Meaning, Tags... );

		ReportAndAbort( Meaning );
	qRR
	qRT
	qRE
	}

	void ReportParsingErrorAndAbort(
		const char *ErrorLabel,
		const rgstry::context___ &Context );

	struct sRack {
	private:
		template <typename t> inline void Test_( t *T )
		{
			if ( T != NULL )
				qRFwk();
		}
		void Test_( void )
		{
			Test_( qRRor );
			Test_( SCLError );
			Test_( CIO );
			Test_( Locale);
		}
	public:
		err::err___ *qRRor;
		sclerror::rError *SCLError;
		const cio::set__ *CIO;
		scllocale::rRack *Locale;
		void reset( bso::sBool = true )
		{
			qRRor = NULL;
			SCLError = NULL;
			CIO = NULL;
			Locale = NULL;
		}
		qCDTOR( sRack );
		void Init( void )
		{
			Test_();

			this->qRRor = err::qRRor;
			this->SCLError = sclerror::SCLERRORError;
			this->CIO = &cio::GetCurrentSet();
			this->Locale = &scllocale::GetRack();
		}
		void Init(
			err::err___ &qRRor,
			sclerror::rError &SCLError,
			const cio::set__ &CIO,
			scllocale::rRack &Locale )
		{
			Test_();

			this->qRRor = &qRRor;
			this->SCLError = &SCLError;
			this->CIO = &CIO;
			this->Locale = &Locale;
		}
	};

	void Initialize(
		const sRack &Rack,
		const rgstry::entry__ &Configuration );

	void Initialize(
		const sRack &Rack,
		xtf::extended_text_iflow__ &LocaleFlow,
		const char *LocaleDirectory,
		xtf::extended_text_iflow__ &RegistryFlow,
		const char *RegistryDirectory,
		const fnm::name___ &BinPath,
		const scli::sInfo &Info );

	void Initialize(
		const sRack &Rack,
		const fnm::name___ &BinPath,
		const scli::sInfo &Info,
		qRPD );

	// Counter-part of 'Initialize'.
	void Quit( const scli::sInfo &Info );

	// Store the content of the 'lasting' registry level as application data.
	void StoreLastingRegistry( const scli::sInfo &Info );

	// Deletes the file which contains the lasting registry.
	void DumpLastingRegistryFile(
		txf::sWFlow &Flow,
		const scli::sInfo &Info );

	// Deletes the file which contains the lasting registry.
	void DeleteLastingRegistryFile( const scli::sInfo &Info );

	void DumpRegistriesAndOrLocalesIfRequired( void );

	void EraseProjectRegistry( void );

	qENUM( ProjectType ) {
		ptNew,			// Empty project.
		ptPredefined,	// Use of a project defined in the 'Definitions' section in the configuration file.
		ptRemote,		// Project stored in a file.
		ptEmbedded,		// The content of the project is embedded in the its declaration (not implemented yet)
		pt_amount,
		pt_Undefined
	};

	const char *GetLabel( eProjectType ProjectType );

	eProjectType GetProjectType( const str::string_ &Pattern );

	void LoadProject(
		flw::iflow__ &Flow,
		const fnm::name___ &Directory,
		const scli::sInfo &Info,
		str::string_ &Id );

	void LoadProject(
		const fnm::name___ &FileName,
		const scli::sInfo &Info,
		str::string_ &Id );

	void LoadProject(
		eProjectType ProjectType,
		const str::string_ &ProjectFeature,
		const scli::sInfo &Info );

	void LoadProject( const scli::sInfo &Info );	// Load project, if applies, following configuration file indications.

	using fil::GetBackupFilename;

	void CreateBackupFile(
		const fnm::name___ &FileName,
		fil::backup_mode__ Mode = fil::bm_Default );

	void RecoverBackupFile( const fnm::name___ &FileName );

	void ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName );

	void Load(
		const fnm::name___ &FileName,
		str::string_ &Content );

	void LoadAndTranslateTags(
		const fnm::name___ &FileName,
		const char *Language,
		str::string_ &Content,
		char Marker = scllocale::DefaultMarker );

	bso::sBool Load(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		sclrgstry::eNeedness Needness,
		str::string_ &Content );
/*
	void Load(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &Content )
	{
		if ( !Load( FileName, Registry, sclrgstry::nMandatory, Content ) )
			qRGnr();
	}
*/
	bso::sBool LoadAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &Content,
		sclrgstry::eNeedness Needness,
		char Marker = scllocale::DefaultMarker );

	inline void LoadAndTranslateTags(
		const rgstry::tentry__ &Filename,
		const sclrgstry::registry_ &Registry,
		str::string_ &Content,
		char Marker = scllocale::DefaultMarker )
	{
		if ( !LoadAndTranslateTags( Filename, Registry, Content, sclrgstry::nMandatory, Marker ) )
			qRFwk();
	}

	void LoadXMLAndTranslateTags(
		const fnm::rName &Filename,
		const char *Language,
		str::string_ &Content,
		xml::sLevel Level,
		char Marker = scllocale::DefaultMarker );

	bso::sBool LoadXMLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &Content,
		sclrgstry::eNeedness Needness,
		xml::sLevel Level,
		char Marker = scllocale::DefaultMarker );

	inline void LoadXMLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &Content,
		xml::sLevel Level,
		char Marker = scllocale::DefaultMarker )
	{
		if ( !LoadXMLAndTranslateTags( FileName, Registry, Content, sclrgstry::nMandatory, Level, Marker ) )
			qRFwk();
	}

	sclrgstry::registry_ &GetRWRegistry( void );
	const sclrgstry::registry_ &GetRegistry( void );

	typedef tht::rLockerHandler LockerHandler_;

	class rLocker_
	: public LockerHandler_
	{
	public:
		void reset( bso::sBool P = true )
		{
			LockerHandler_::reset( P );
		}
		qCDTOR( rLocker_ );
		void Init( void )
		{
			LockerHandler_::Init( sclrgstry::GetCommonRegistryLocker() );
		}
	};

	inline rgstry::level__ GetRegistryRawLevel( sclrgstry::eLevel Level )
	{
		return sclrgstry::GetRawLevel( Level );
	}

	inline void FillSetupRegistry( const str::string_ &Id )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclrgstry::FillWithSetupOfId( GetRWRegistry(), GetRawLevel( sclrgstry::lSetup ), Id );
	qRR
	qRT
	qRE
	}

	inline void FillSetupRegistry( void )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclrgstry::FillWithGivenSetup( GetRWRegistry(), GetRawLevel( sclrgstry::lSetup ) );
	qRR
	qRT
	qRE
	}

	inline void AddValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclrgstry::AddValue( GetRWRegistry(), Value, Entry );
	qRR
	qRT
	qRE
	}

	inline void AddValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclrgstry::AddValue( GetRWRegistry(), Path, Value, Error );
	qRR
	qRT
	qRE
	}

	inline void AddLastingValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		GetRWRegistry().AddValue( sclrgstry::lLasting, Path, Value, Error );
	qRR
	qRT
	qRE
	}

	inline void AddLastingValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry,
		sdr::row__ *Error = NULL )
	{
	qRH
		str::wString Path;
	qRB
		Path.Init();
		AddLastingValue( Entry.GetPath( Path ), Value, Error );
	qRR
	qRT
	qRE
	}

	inline void SetValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclrgstry::SetValue( GetRWRegistry(), Value, Entry );
	qRR
	qRT
	qRE
	}

	inline void SetValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclrgstry::SetValue( GetRWRegistry(), Path, Value, Error );
	qRR
	qRT
	qRE
	}

	inline void SetLastingValue(
		const str::string_ &Path,
		const str::string_ &Value,
		sdr::row__ *Error = NULL )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		GetRWRegistry().SetValue( sclrgstry::lLasting, Path, Value, Error );
	qRR
	qRT
	qRE
	}

	inline void SetLastingValue(
		const str::string_ &Value,
		const rgstry::tentry__ &Entry,
		sdr::row__ *Error = NULL )
	{
	qRH
		str::wString Path;
	qRB
		Path.Init();
		SetLastingValue(Entry.GetPath( Path ), Value, Error );
	qRR
	qRT
	qRE
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
		qRH\
			sclmisc::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			Result = sclmisc::MGetValue( entry, Buffer );\
		qRR\
		qRT\
		qRE\
			return Buffer;\
		}\
	inline const str::string_ &name( str::string_ &Value )\
		{\
		qRH\
			sclmisc::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			result = sclmisc::MGetValue( entry, Value );\
		qRR\
		qRT\
		qRE\
			return Value;\
		}

		// To define function retrieving optional registry value.
# define SCLMISC_OV( name, entry )\
	inline const char *name( TOL_CBUFFER___ &Buffer )\
		{\
		qRH\
			sclmisc::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			Result = sclmisc::OGetValue( entry, Buffer );\
		qRR\
		qRT\
		qRE\
			return Buffer;\
		}\
	inline bso::bool__ name( str::string_ &Value )\
		{\
			bso::sBool Result= false;\
		qRH\
			sclmisc::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			Result = sclmisc::OGetValue( entry, Value );\
		qRR\
		qRT\
		qRE\
			return Result;\
		}

	// To facilitate optional text output file handling. if no file name is given, then the standard output is given. Also creates a backup file, which can be restored.
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
		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
		txf::text_oflow__ &Init( const fnm::name___ &FileName );
		void HandleError( void );	// A appeler  partir de 'qRR'.
		bso::sBool IsFile( void ) const
		{
			return _BackedUp;
		}
	};

	void DumpRegistries(
		const str::string_ &RawList,
		txf::text_oflow__ &Flow );

	void DumpLocales(
		const str::string_ &RawList,
		txf::text_oflow__ &Flow );

	// Retrieve plugin features with given target AND id.
	const str::string_ &GetPluginFeatures(
		const char *Target,
		const str::dString &Id,
		str::string_ &Filename,
		rgstry::entry__ &Configuration,
		rgstry::entry__ &Locale );

	// Retrieve a plugin with given target. Its id is retrieved from the registry.
	const str::string_ &GetPluginFeatures(
		const char *Target,
		str::string_ &Filename,
		rgstry::entry__ &Configuration,
		rgstry::entry__ &Locale,
		str::string_ &Arguments );

	void HandleLocale_(
		const rgstry::entry__ &Entry,
		const str::string_ &Filename );

	// Retrieve a plugin with given target AND Id.
	template <typename retriever> inline void Plug(
		const char *Target,
		const str::dString &Id,
		const str::dString &Arguments,	// Useless if 'Id' empty.
		const char *Identifier,
		retriever &PluginRetriever )
	{
	qRH
		str::string Filename, TrueArguments;
		rgstry::entry__ Configuration, Locale;
	qRB
		Filename.Init();
		TrueArguments.Init();
		Configuration.Init();
		Locale.Init();

		if ( Id.Amount() != 0 ) {
			GetPluginFeatures( Target, Id, Filename, Configuration, Locale );
			TrueArguments = Arguments;
		} else
			GetPluginFeatures( Target, Filename, Configuration, Locale, TrueArguments );

		HandleLocale_( Locale, Filename );

		PluginRetriever.Initialize( Filename, Identifier, Configuration, TrueArguments, plgn::EmptyAbstracts );
	qRR
	qRT
	qRE
	}

	// Retrieve a plugin with given target. Its id is retrieved from the registry.
	template <typename retriever> inline void Plug(
		const char *Target,
		const char *Identifier,
		retriever &PluginRetriever )
	{
		Plug( Target, str::wString(), str::wString(), Identifier, PluginRetriever );
	}

	void Plug_(
		const char *Target,
		const char *Label,
		const char *Identifier,
		plgn::dRetrievers &Retrievers );

	template <typename plugin> inline void Plug(
		const char *Target,
		const char *Identifier,
		plgn::rRetrievers<plugin> &Retrievers )
	{
		return Plug_(Target, plugin::Label(), Identifier, Retrievers );
	}
}

/***************/
/***** NEW *****/
/***************/

namespace sclmisc {
	typedef text_oflow_rack___ rTextOFlowRack;

	// To facilitate optional text output file handling. if no file name is given, then the standard output is given. Also creates a backup file, which can be restored.
	class rODriverRack
	{
	private:
		fnm::name___ Filename_;
		flf::rODriver Driver_;
		bso::bool__ BackedUp_;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Driver_, Filename_, BackedUp_ );
		}
		E_CDTOR( rODriverRack );
		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
		fdr::rWDriver &Init( const fnm::name___ &FileName );
		void HandleError( void );	// A appeler  partir de 'qRR'.
		bso::sBool IsFile( void ) const
		{
			return BackedUp_;
		}
	};

	class rIDriverRack
	{
	private:
		fnm::name___ Filename_;
		flf::rIDriver Driver_; 
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Driver_, Filename_ );
		}
		E_CDTOR( rIDriverRack );
		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
		fdr::rRDriver &Init( const fnm::name___ &FileName );
		void HandleError( void )	// To call from' qRR'.
		{
			// Does nothing. For simetry with 'rODriverRack'.
		}
		bso::sBool IsFile( void ) const
		{
			return !Filename_.IsEmpty();
		}
	};

	void ExitOnSignal( void );
}


#endif
