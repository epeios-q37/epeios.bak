/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef SCLFRNTND__INC
# define SCLFRNTND__INC

# define SCLFRNTND_NAME		"SCLFRNTND"

# if defined( E_DEBUG ) && !defined( SCLFRNTND_NODBG )
#  define SCLFRNTND_DBG
# endif

// SoCLe FRoNTeND

# include "sclrgstry.h"
# include "sclmisc.h"

# include "fblfrd.h"

# include "csducl.h"

# include "plgn.h"

# include "err.h"
# include "flw.h"
# include "xml.h"

// NOTA : 'SCLF_' is used in place of 'SCLFRNTND_' for macro. 

/***************/
/***** OLD *****/
/***************/

namespace sclfrntnd {
	using fblfrd::compatibility_informations__;

	using fblfrd::incompatibility_informations_;
	using fblfrd::incompatibility_informations;

	typedef fblfrd::universal_frontend___ _frontend___;

	enum backend_type__ {
		btNone,			// Use of no backend.
		btRemote,		// Remote, as a daemon.
		btEmbedded,		// Embedded, as a library.
		btPredefined,	// Retrieve backend parameters from configuration file.
		bt_amount,
		bt_Undefined
	};

	const char *GetLabel( backend_type__ BackendType );

	backend_type__ GetBackendType( const str::string_ &Pattern );

	void GetRemoteBackendPluginPath(
		const str::string_ &Id,
		str::string_ &Path );

	struct features___ {
	public:
		csducl::type__ Type;
		str::string Path;
		const char *Identifier;
		str::string Parameters;
		void reset( bso::bool__ P = true )
		{
			Type = csducl::t_Undefined;
			Path.reset( P );
			Identifier = NULL;
			Parameters.reset( P );
		}
		E_CDTOR( features___ );
		void Init(void)
		{
			Type = csducl::t_Undefined;
			Path.Init();
			Identifier = NULL;
			Parameters.Init();
		}
	};

	void SetBackendFeatures(
		backend_type__ BackendType,
		const str::string_ &Path,
		const str::string_ &Parameters,
		features___ &Features );

	// Is exposed because, even if there is generally only one kernel per frontend, there could be two (a frontend dealing with two different backends).
	class kernel___
	{
	private:
		csducl::universal_client_core___ _ClientCore;
	public:
		void reset( bso::bool__ P = true )
		{
			_ClientCore.reset( P );
		}
		E_CVDTOR( kernel___ );
		sdr::sRow Init(
			const features___ &Features,
			const plgn::dAbstracts &Abstracts );
		const csducl::universal_client_core___ &Core( void ) const
		{
			return _ClientCore;
		}
		csducl::universal_client_core___ &Core( void )
		{
			return _ClientCore;
		}
		const char *Details( void )
		{
			return _ClientCore.RemoteDetails();
		}
		const str::dString &AboutPlugin( str::dString &About );
	};

	class frontend___
	: public _frontend___
	{
	private:
		Q37_MRMDF( kernel___, K_, Kernel_ );
		csducl::universal_client_ioflow___ _Flow;
		rgstry::multi_level_registry _Registry;
		rgstry::level__ _RegistryLevel;
		mutable TOL_CBUFFER___ _Language;
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend___::reset( P );
			_Flow.reset( P );
			_Registry.reset( P );
			_RegistryLevel = rgstry::UndefinedLevel;
			_Language.reset();
			Kernel_ = NULL;
		}
		E_CVDTOR( frontend___ );
		void Init(
			kernel___ &Kernel,
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback,
			const rgstry::multi_level_registry_ &Registry );
		void Ping( void );
		void Crash( void );
		bso::bool__ Connect(
			const fblfrd::compatibility_informations__ &CompatibilityInformations,
			fblfrd::incompatibility_informations_ &IncompatibilityInformations );
		void Disconnect( void );
		const rgstry::multi_level_registry_ &Registry( void ) const
		{
			return _Registry;
		}
		const char *Language( void ) const
		{
			return sclrgstry::MGetValue( _Registry, rgstry::tentry___( sclrgstry::parameter::Language ), _Language );
		}
	};

	inline void LoadProject(
		sclmisc::project_type__ Type,
		const str::string_ &Feature )
	{
		sclmisc::LoadProject( Type, Feature );
	}

	inline void LoadProject( void )
	{
		sclmisc::LoadProject();
	}

	void GetProjectsFeatures(
		const char *Language,
		xml::writer_ &Writer );

	void GetBackendsFeatures(
		const char *Language,
		xml::writer_ &Writer );

	void GuessBackendFeatures( features___ &Features );	// Set features following what's in registry.

	const str::string_ &GetBackendPath(
		const kernel___ &Kernel,
		str::string_ &Path );

# define SCLF_I( name, id  )\
	typedef fbltyp::s##id t##name;\
	E_TMIMIC__( t##name,	s##name );\
	typedef fbltyp::d##id##s	d##name##s;\
	qW( name##s );\
\
	E_CDEF( s##name, Undefined##name, fbltyp::Undefined##id )

	template <typename ids> class dI1S
	{
	public:
		struct s {
			typename ids::s Ids;
			str::dStrings::s Strings1;
		};
		ids Ids;
		str::dStrings Strings1;
		dI1S( s &S )
		: Ids( S.Ids ),
		  Strings1( S.Strings1 )
		{}
		void reset( bso::bool__ P = true )
		{
			Ids.reset( P );
			Strings1.reset( P );
		}
		void plug( qASd *AS )
		{
			Ids.plug( AS );
			Strings1.plug( AS );
		}
		dI1S &operator =(const dI1S &I1S)
		{
			Ids = I1S.Ids;
			Strings1 = I1S.Strings1;

			return *this;
		}
		void Init( void )
		{
			Ids.Init();
			Strings1.Init();
		}
		qNAV( Ids. );
	};

	qW1( I1S );

# define SCLF_I1S( name, id  )\
	SCLF_I( name, id );\
\
	typedef sclfrntnd::dI1S<d##name##s> d##name##sI1S;\
	qW( name##sI1S );

	template <typename id> class dI2S	// id, 2 strings.
	: public dI1S<id>
	{
	public:
		struct s
		: public dI1S<id>::s
		{
			str::dStrings::s Strings2;
		};
		str::dStrings Strings2;
		dI2S( s &S )
		: dI1S<id>( S ),
		  Strings2( S.Strings2 )
		{}
		void reset( bso::bool__ P = true )
		{
			dI1S<id>::reset( P );
			Strings2.reset( P );
		}
		void plug( qASd *AS )
		{
			dI1S<id>::plug( AS );
			Strings2.plug( AS );
		}
		dI2S &operator =(const dI2S &I2S)
		{
			dI1S<id>::operator =( I2S );
			Strings2 = I2S.Strings2;

			return *this;
		}
		void Init( void )
		{
			dI1S<id>::Init();
			this->Strings2.Init();
		}
	};

	qW1( I2S );


# define SCLF_I2S( name, id  )\
	SCLF_I( name, id );\
\
	typedef sclfrntnd::dI2S<d##name##s> d##name##sI2S;\
	qW( name##sI2S );

	E_CDEF( char *, IdAttribute, "id" );
	E_CDEF( char *, AmountAttribute, "Amount" );

	enum eKind {
		kTag,
		kAttribute,
		kValue,
		kAttr = kAttribute,
		k_amount,
		k_Undefined,
		k_End = k_Undefined
	};

	template <typename id> inline void Dump_(
		xml::dWriter &Writer,
		id Id,
		... )	// Kind, Value, [Label], kind, value, [Label]... k_End.
	{
		va_list List;
		va_start( List, Id );

		eKind Kind = va_arg( List, eKind );
		const str::dString *Value = NULL;

		Writer.PutAttribute( IdAttribute, *Id );

		while ( Kind != k_End ) {
			Value = va_arg( List, const str::dString *);

			switch ( Kind ) {
			case kTag:
				Writer.PutValue( *Value, va_arg( List, const char *) );
				break;
			case kAttribute:
				Writer.PutAttribute( va_arg( List, const char *), *Value );
				break;
			case kValue:
				Writer.PutValue( *Value );
				break;
			default:
				qRFwk();
				break;
			}

			Kind = va_arg( List, eKind );
		}

		va_end( List );
	}

	// Root tag is handled by user, so he/she cans put his/her own attributes. 'Amount' attribute is addded.
	template <typename ids> inline void Dump(
		const dI1S<ids> &I1S,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		sdr::row__ Row = I1S.First();

		xml::PutAttribute( sclfrntnd::AmountAttribute, I1S.Amount(), Writer );

		while ( Row != qNIL ) {
			Writer.PushTag( ItemLabel );

			Dump_( Writer, I1S.Ids( Row ), kValue, &I1S.Strings1( Row ), k_End );

			Writer.PopTag();

			Row = I1S.Next( Row );
		}
	}
	
	template <typename ids> inline void Dump(
		const dI1S<ids> &I1S,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Writer.PushTag( ItemsLabel );

		Dump( I1S, ItemLabel, Writer );

		Writer.PopTag();
	}

	// Root tag is handled by user, so he/she cans put his/her own attributes. 'Amount' attribute is addded.
	template <typename ids> inline void Dump(
		const dI1S<ids> &I1S,
		const char *ItemLabel,
		eKind String1LabelKind,
		const char *String1Label,
		xml::writer_ &Writer )
	{
		sdr::row__ Row = I1S.First();

		xml::PutAttribute( sclfrntnd::AmountAttribute, I1S.Amount(), Writer );

		while ( Row != qNIL ) {
			Writer.PushTag( ItemLabel );

			Dump_( Writer, I1S.Ids( Row ), String1LabelKind, &I1S.Strings1( Row ), String1Label, k_End );

			Writer.PopTag();

			Row = I1S.Next( Row );
		}
	}

	template <typename ids> inline void Dump(
		const dI1S<ids> &I1S,
		const char *ItemsLabel,
		const char *ItemLabel,
		eKind String1LabelKind,
		const char *String1Label,
		xml::writer_ &Writer )
	{
		Writer.PushTag( ItemsLabel );

		Dump( I1S, ItemLabel, String1LabelKind, String1Label, Writer );

		Writer.PopTag();
	}


	E_CDEF( char *, LabelAttribute, "label" );

	// Root tag is handled by user, so he/she cans put his/her own attributes. 'Amount' attribute is addded.
	template <typename ids> inline void DumpWithLabelAttribute(
		const sclfrntnd::dI1S<ids> &I1S,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump<ids>( I1S, ItemLabel, kAttribute, LabelAttribute, Writer );
	}

	template <typename ids> inline void DumpWithLabelAttribute(
		const sclfrntnd::dI1S<ids> &I1S,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump<ids>( I1S, ItemsLabel, ItemLabel, kAttribute, LabelAttribute, Writer );
	}
}

/***************/
/***** NEW *****/
/***************/

namespace sclfrntnd {

	// Authentication-related bevaviour.
	qENUM( Auth ) {
		aBlank,		// All the fields are left blank,
		aPartial,	// Only the 'Login' field is filled.
		aFull,		// Both 'Login' and 'Password' field are filled.
		aAutomatic,	// The 'Login' page is skipped, and the authentication parameters are retrieved frrom the configuration file.
		a_amount,
		a_Undefined
	};

	const char *GetLabel( eAuth Auth );

	eAuth GetAuth( const str::dString &Pattern );

	eAuth GetAuthParameters(
		str::dString &Login,
		str::dString &Password );

	eAuth GetAuthFeatures( xml::dWriter &Writer );

	/* An identifier usually identifies the plugin used to access the backend.
	Identifier belows are returned when there in no bckend, or if the backend is embedded. */
	namespace identifier {
		qCDEF(char *, NoneBackendIdentifier, "_NONE" );
		qCDEF(char *, EmbeddedBackendIdentifier, "_EMBEDDED" );
	}

	using fblfrd::cReporting;
	using fblfrd::sDefaultReportingCallback;

	typedef kernel___ rKernel;
	typedef frontend___ rFrontend;

	typedef features___ rFeatures;

	const str::dString &About(
		const rFeatures &Features,
		str::dString &About );

	qENUM( BackendSetupType ) {
		bstId,
		bstContent,
		bst_amount,
		bst_Undefined
	};

	const char *GetLabel( eBackendSetupType Type);

	eBackendSetupType GetBackendSetupType( const str::dString &Pattern );
}

#endif
