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
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// SOcLe Misc.

#ifndef SCLM_INC_
# define SCLM_INC_

# define SCLM_NAME		"SCLM"

# if defined( E_DEBUG ) && !defined( SCLM_NODBG )
#  define SCLM_DBG
# endif

# include "scle.h"
# include "scli.h"
# include "scll.h"
# include "sclr.h"

# include "bso.h"
# include "cio.h"
# include "err.h"
# include "plgn.h"
# include "str.h"

namespace sclm {
	bso::bool__ IsInitialized( void );

	const str::dString &GetBinPath( void );

	void SetBaseLanguage( const str::dString &Language );	// Set the base language.

	void RefreshBaseLanguage( void );	// Set the base language with the content of the registry.

	const char *GetBaseLanguage( void );	// Admin. language ; could also be the user language under some circumstances.

	template <typename source> inline const str::string_ &GetBaseTranslation(
		const source &Source,
		str::dString &Translation )
	{
		return scll::GetTranslation( Source, GetBaseLanguage(), Translation );
	}

	template <typename source, typename ... tags> inline const str::string_ &GetBaseTranslation(
		const source &Source,
		str::dString &Translation,
		const tags &... Tags )
	{
		return scll::GetTranslation( Source, GetBaseLanguage(), Translation, Tags... );
	}

	inline bso::bool__ GetSCLBasePendingErrorTranslation(
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default )
	{
		return scle::GetPendingErrorTranslation( GetBaseLanguage(), Translation, ErrHandling );
	}

	bso::sBool DisplaySCLBasePendingError( txf::sWFlow &Flow = cio::CErr );

	// To use as 'action' parameter for the 'qRFE(...)' macro to display the file/line of an error.
	void ErrorDefaultHandling(void);

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
		scle::rError *SCLError;
		const cio::set__ *CIO;
		scll::rRack *Locale;
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
			this->SCLError = scle::SCLEError;
			this->CIO = &cio::GetCurrentSet();
			this->Locale = &scll::GetRack();
		}
		void Init(
			err::err___ &qRRor,
			scle::rError &SCLError,
			const cio::set__ &CIO,
			scll::rRack &Locale )
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

	// Store the content of the 'lasting' registry layer as application data.
	void StoreLastingRegistry( const scli::sInfo &Info );

	// Deletes the file which contains the lasting registry.
	void DumpLastingRegistryFile(
		txf::sWFlow &Flow,
		const scli::sInfo &Info );

	// Deletes the file which contains the lasting registry.
	void DeleteLastingRegistryFile( const scli::sInfo &Info );

	void DumpRegistriesAndOrLocalesIfRequired( void );

	void EraseProjectRegistry( void );

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
		const fnm::name___ &FileName,
		const scli::sInfo &Info);

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
		char Marker = scll::DefaultMarker );

	bso::sBool Load(
		const rgstry::tentry__ &FileName,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		qRPD );

	bso::sBool LoadAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		char Marker,
		qRPN );

	inline void LoadAndTranslateTags(
		const rgstry::tentry__ &Filename,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		char Marker = scll::DefaultMarker)
	{
		LoadAndTranslateTags(Filename, Registry, Content, Marker, err::h_Default);
	}

	inline void LoadAndTranslateTags(
		const rgstry::tentry__ &Filename,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		qRPN)
	{
		LoadAndTranslateTags(Filename, Registry, Content, scll::DefaultMarker, qRP);
	}

	void LoadXMLAndTranslateTags(
		const fnm::rName &Filename,
		const char *Language,
		str::string_ &Content,
		xml::sLevel StartLevel,
		char Marker);

	inline void LoadXMLAndTranslateTags(
		const fnm::rName &Filename,
		const char *Language,
		str::string_ &Content,
		xml::sLevel StartLevel)
    {
        return LoadXMLAndTranslateTags(Filename, Language, Content, StartLevel, scll::DefaultMarker);
    }

	bso::sBool LoadXMLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		xml::sLevel StartLevel,
		char Marker,
		qRPN );

	inline void LoadXMLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		xml::sLevel StartLevel,
		char Marker = scll::DefaultMarker)
	{
		LoadXMLAndTranslateTags(FileName, Registry, Content, StartLevel, Marker, err::h_Default);
	}

	inline bso::sBool LoadXMLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclr::registry_ &Registry,
		str::string_ &Content,
		xml::sLevel StartLevel,
		qRPN )
	{
		return LoadXMLAndTranslateTags(FileName, Registry, Content, StartLevel, scll::DefaultMarker, qRP);
	}

	sclr::registry_ &GetRWRegistry( void );
	const sclr::registry_ &GetRegistry( void );

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
			LockerHandler_::Init( sclr::GetCommonRegistryLocker() );
		}
	};

	inline rgstry::layer__ GetRegistryRawLayer( sclr::eLayer Layer )
	{
		return sclr::GetRawLayer( Layer );
	}

	inline void FillSetupRegistry( const str::string_ &Id )
	{
	qRH
		rLocker_ Locker;
	qRB
		Locker.Init();
		sclr::FillWithSetupOfId( GetRWRegistry(), GetRawLayer( sclr::lSetup ), Id );
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
		sclr::FillWithGivenSetup( GetRWRegistry(), GetRawLayer( sclr::lSetup ) );
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
		sclr::AddValue( GetRWRegistry(), Value, Entry );
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
		sclr::AddValue( GetRWRegistry(), Path, Value, Error );
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
		GetRWRegistry().AddValue( sclr::lLasting, Path, Value, Error );
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
		sclr::SetValue( GetRWRegistry(), Value, Entry );
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
		sclr::SetValue( GetRWRegistry(), Path, Value, Error );
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
		GetRWRegistry().SetValue( sclr::lLasting, Path, Value, Error );
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
		str::string_ &Value,
		qRPN )
	{
		return sclr::BGetValue( GetRegistry(), Entry, Value, qRP );
	}

	inline bso::bool__ GetValues(
		const rgstry::tentry__ &Entry,
		str::dStrings &Values )
	{
		return sclr::GetValues( GetRegistry(), Entry, Values );
	}

	inline bso::bool__ OGetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value )
	{
		return sclr::OGetValue( GetRegistry(), Entry, Value );
	}

	inline const char *OGetValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer )	// Returns NULL when entry missing.
	{
		return sclr::OGetValue( GetRegistry(), Entry, Buffer );
	}

	inline const str::string_ &MGetValue(
		const rgstry::tentry__ &Entry,
		str::string_ &Value )
	{
		return sclr::MGetValue( GetRegistry(), Entry, Value );
	}

	inline const char *MGetValue(
		const rgstry::tentry__ &Entry,
		TOL_CBUFFER___ &Buffer )
	{
		return sclr::MGetValue( GetRegistry(), Entry, Buffer );
	}

	inline bso::sBool OGetChar(
    const rgstry::sTEntry &Entry,
    bso::sChar &Char)
  {
    return sclr::OGetChar(GetRegistry(), Entry, Char);
  }

	inline bso::sChar OGetChar(const rgstry::sTEntry &Entry)
  {
    return sclr::OGetChar(GetRegistry(), Entry);
  }

	inline void MGetChar(
    const rgstry::sTEntry &Entry,
    bso::sChar &Char)
  {
    return sclr::MGetChar(GetRegistry(), Entry, Char);
  }

	inline bso::sChar MGetChar(const rgstry::sTEntry &Entry)
  {
    return sclr::MGetChar(GetRegistry(), Entry);
  }

	inline bso::bool__ BGetBoolean(
		const rgstry::tentry__ &Entry,
		bso::sBool &Result,
		bso::bool__ DefaultValue,
		qRPN)
	{
		return sclr::BGetBoolean(GetRegistry(), Entry, Result, DefaultValue, qRP);
	}

	inline bso::sBool OGetBoolean(
		const rgstry::tentry__ &Entry,
		bso::bool__ DefaultValue)
	{
		return sclr::OGetBoolean(GetRegistry(), Entry, DefaultValue);
	}

	inline bso::bool__ MGetBoolean(const rgstry::tentry___ &Entry)
	{
		return sclr::MGetBoolean( GetRegistry(), Entry );
	}

# define SCLM_UN_( type, name, limit )\
	inline type MGet##name(\
		const rgstry::tentry__ &Entry,\
		type Limit = limit )\
		{\
			return sclr::MGet##name( GetRegistry(), Entry, Limit );\
		}\
	inline type OGet##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit = limit )\
		{\
			return sclr::OGet##name( GetRegistry(), Entry, DefaultValue, Limit );\
		}

		SCLM_UN_( bso::uint__, UInt, BSO_UINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLM_UN_( bso::u64__, U64, BSO_U64_MAX )
# endif
		SCLM_UN_( bso::u32__, U32, BSO_U32_MAX )
		SCLM_UN_( bso::u16__, U16, BSO_U16_MAX )
		SCLM_UN_( bso::u8__, U8, BSO_U8_MAX )

# define SCLM_SN_( type, name, min, max )\
	inline type MGet##name(\
		const rgstry::tentry__ &Entry,\
		type Min = min,\
		type Max = max )\
		{\
			return sclr::MGet##name( GetRegistry(), Entry, Min, Max );\
		}\
	inline type OGet##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min = min,\
		type Max = max )\
		{\
			return sclr::OGet##name( GetRegistry(), Entry, DefaultValue, Min, Max );\
		}

		SCLM_SN_( bso::sint__, SInt, BSO_SINT_MIN, BSO_SINT_MAX )
# ifdef BSO__64BITS_ENABLED
		SCLM_SN_( bso::s64__, S64, BSO_S64, BSO_S64_MAX )
#endif
		SCLM_SN_( bso::s32__, S32, BSO_S32_MIN, BSO_S32_MAX )
		SCLM_SN_( bso::s16__, S16, BSO_S16_MIN, BSO_S16_MAX )
		SCLM_SN_( bso::s8__, S8, BSO_S8_MIN, BSO_S8_MAX )

		// To define function retrieving mandatory registry value.
# define SCLM_MV( name, entry )\
	inline const char *name(\
		TOL_CBUFFER___ &Buffer )\
		{\
		qRH\
			sclm::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			Result = sclm::MGetValue( entry, Buffer );\
		qRR\
		qRT\
		qRE\
			return Buffer;\
		}\
	inline const str::string_ &name( str::string_ &Value )\
		{\
		qRH\
			sclm::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			result = sclm::MGetValue( entry, Value );\
		qRR\
		qRT\
		qRE\
			return Value;\
		}

		// To define function retrieving optional registry value.
# define SCLM_OV( name, entry )\
	inline const char *name( TOL_CBUFFER___ &Buffer )\
		{\
		qRH\
			sclm::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			Result = sclm::OGetValue( entry, Buffer );\
		qRR\
		qRT\
		qRE\
			return Buffer;\
		}\
	inline bso::bool__ name( str::string_ &Value )\
		{\
			bso::sBool Result= false;\
		qRH\
			sclm::rRegistryLocker Locker;\
		qRB\
			Locker.Init();\
			Result = sclm::OGetValue( entry, Value );\
		qRR\
		qRT\
		qRE\
			return Result;\
		}

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
	bso::sBool GetPluginFeatures(
		const char *Target,
		str::string_ &Filename,
		rgstry::entry__ &Configuration,
		rgstry::entry__ &Locale,
		str::string_ &Arguments,
		qRPD );

	void HandleLocale_(
		const rgstry::entry__ &Entry,
		const str::string_ &Filename );

	// Retrieve a plugin with given target AND Id.
	template <typename retriever> inline bso::sBool Plug(
		const char *Target,
		const str::dString &Id,
		const str::dString &Arguments,	// Useless if 'Id' empty.
		const char *Identifier,
		retriever &PluginRetriever,
		qRPD )
	{
		bso::sBool Exists = false;
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
			Exists = true;
		} else
			Exists = GetPluginFeatures( Target, Filename, Configuration, Locale, TrueArguments, qRP );

		if ( Exists ) {
			HandleLocale_( Locale, Filename );

			PluginRetriever.Initialize( Filename, Identifier, Configuration, TrueArguments, plgn::EmptyAbstracts );
		}
	qRR
	qRT
	qRE
		return Exists;
	}

	// Retrieve a plugin with given target. Its id is retrieved from the registry.
	template <typename retriever> inline bso::sBool Plug(
		const char *Target,
		const char *Identifier,
		retriever &PluginRetriever,
		qRPD )
	{
		return Plug( Target, str::wString(), str::wString(), Identifier, PluginRetriever, qRP );
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

namespace sclm {
	// To facilitate optional text output file handling. if no file name is given, then the standard output is given. Also creates a backup file, which can be restored.
	class rWDriverRack
	{
	private:
		fnm::name___ Filename_;
		flf::rODriver Driver_;
		bso::bool__ BackedUp_;
	public:
		void reset(bso::bool__ P = true)
		{
			tol::reset( P, Driver_, Filename_, BackedUp_ );
		}
		qCDTOR( rWDriverRack );
		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
		fdr::rWDriver &Init(const fnm::rName &FileName);
		void HandleError(void);	// A appeler  partir de 'qRR'.
		bso::sBool IsFile(void) const
		{
			return BackedUp_;
		}
	};

	template <typename f> class rWFlowRack_
	: public rWDriverRack
	{
  private:
    f Flow_;
  public:
    void reset(bso::sBool P = true)
    {
      Flow_.reset(P);
      rWDriverRack::reset(P);
    }
    qCDTOR(rWFlowRack_);
 		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
    f &Init(const fnm::rName &FileName)
    {
      Flow_.Init(rWDriverRack::Init(FileName));

      return Flow_;
    }
	};

	typedef rWFlowRack_<flw::rDressedWFlow<>> rWFlowRack;

  typedef rWFlowRack_<txf::rWFlow> rTWFlowRack;

	class rRDriverRack
	{
	private:
		fnm::name___ Filename_;
		flf::rIDriver Driver_;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Driver_, Filename_ );
		}
		qCDTOR( rRDriverRack );
		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
		fdr::rRDriver &Init( const fnm::name___ &FileName );
		void HandleError( void )	// To call from' qRR'.
		{
			// Does nothing. For symmetry with 'rODriverRack'.
		}
		bso::sBool IsFile( void ) const
		{
			return !Filename_.IsEmpty();
		}
	};

	template <typename f> class rRFlowRack_
	: public rRDriverRack
	{
  private:
    f Flow_;
  public:
    void reset(bso::sBool P = true)
    {
      Flow_.reset(P);
      rRDriverRack::reset(P);
    }
    qCDTOR(rRFlowRack_);
 		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
    f &Init(const fnm::rName &FileName)
    {
      Flow_.Init(rRDriverRack::Init(FileName));

      return Flow_;
    }
	};

	typedef rRFlowRack_<flw::rDressedRFlow<>> rRFlowRack;

  typedef rRFlowRack_<txf::rRFlow> rTRFlowRack;

  class rXRFlowRack
  : public rRFlowRack
  {
  private:
    xtf::sRFlow Flow_;
  public:
    void reset(bso::sBool P = true)
    {
      Flow_.reset(P);
      rRFlowRack::reset(P);
    }
    qCDTOR(rXRFlowRack);
 		// !!! Don't forget the 'HandleError()' in 'qRR'. !!!
    xtf::sRFlow &Init(
      const fnm::rName &FileName,
      utf::eFormat Format = utf::f_Default)
    {
      Flow_.Init(rRFlowRack::Init(FileName), Format);

      return Flow_;
    }
  };


	void ExitOnSignal( void );
}

#endif
