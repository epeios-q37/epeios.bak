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
			const rgstry::multi_level_registry_ &Registry )
		{
			// _Flow.Init(...);	// Made on connection.
			_Registry.Init();
			_Registry.Push( Registry );
			_RegistryLevel = _Registry.CreateEmbedded( rgstry::name( "Session" ) );

			if ( (Language != NULL) && *Language )
				sclrgstry::SetValue( _Registry, str::string( Language ), rgstry::tentry___( sclrgstry::parameter::Language ) );

			Kernel_ = &Kernel;

			return _frontend___::Init( ReportingCallback );
		}
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
	typedef fbltyp::s##id b##name##Id;\
	E_TMIMIC__( b##name##Id,	s##name##Id );\
	typedef fbltyp::d##id##s	d##name##Ids;\
	qW( name##Ids );\
\
	E_CDEF( s##name##Id, Undefined##name, fbltyp::Undefined##id )

	template <typename t> class dIL_	// id, label.
	{
	public:
		struct s {
			t Id;
			str::string_::s Label;
		} &S_;
		str::string_ Label;
		dIL_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Id = t(-1);
			Label.reset( P );
		}
		void plug( str::cHook &Hook )
		{
			Label.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Label.plug( AS );
		}
		dIL_ &operator =(const dIL_ &IL)
		{
			S_.Id = IL.S_.Id;
			Label = IL.Label;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label )
		{
			S_.Id = Id;
			this->Label.Init( Label );
		}
		E_RODISCLOSE_( t, Id );
	};

	qW1( IL_ );

# define SCLF_IL( name, id  )\
	SCLF_I( name, id );\
\
	typedef sclfrntnd::dIL_<s##name##Id> d##name;\
	qW( name );\
\
	typedef ctn::qMCONTAINERdl( d##name ) d##name##s;\
	qW( name##s );

	template <typename id, typename ids> inline void Fill(
		const ids &Ids,
		const fbltyp::strings_ &Labels,
		ctn::qMCONTAINERdl( dIL_<id> ) &Items )
	{
	qRH
		sdr::row__ Row = qNIL;
		wIL_<id> Item;
	qRB
		Row = Ids.First();

		if ( Ids.Amount() != Labels.Amount() )
			qRGnr();

		while ( Row != qNIL ) {
			Item.Init( Ids(Row), Labels(Row) );
			Items.Append( Item );

			Row = Ids.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	template <typename t> class dILW_	// id, label, wording.
	: public dIL_<t>
	{
	public:
		struct s
		: public dIL_<t>::s
		{
			str::string_::s Wording;
		};
		str::string_ Wording;
		dILW_( s &S )
		: dIL_<t>( S ),
		  Wording( S.Wording )
		{}
		void reset( bso::bool__ P = true )
		{
			dIL_<t>::reset( P );
			Wording.reset( P );
		}
		void plug( qASd *AS )
		{
			dIL_<t>::plug( AS );
			Wording.plug( AS );
		}
		dILW_ &operator =(const dILW_ &ILW)
		{
			dIL_<t>::operator =( ILW );
			Wording = ILW.Wording;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label,
			const str::string_ &Wording )
		{
			dIL_<t>::Init( Id, Label );
			this->Wording.Init( Wording );
		}
	};

	qW1( ILW_ );


# define SCLF_ILW( name, id  )\
	SCLF_I( name, id );\
\
	typedef sclfrntnd::dILW_<s##name##Id> d##name;\
	qW( name );\
\
	typedef ctn::qCONTAINERdl( d##name ) d##name##s;\
	qW( name##s );

	template <typename id, typename ids> inline void Fill(
		const ids &Ids,
		const fbltyp::strings_ &Labels,
		const fbltyp::strings_ &Wordings,
		ctn::qCONTAINERdl( dILW_<id> ) &Items )
	{
	qRH
		sdr::row__ Row = qNIL;
		wILW_<id> Item;
	qRB
		Row = Ids.First();

		if ( Ids.Amount() != Labels.Amount() )
			qRGnr();

		if ( Ids.Amount() != Wordings.Amount() )
			qRGnr();

		while ( Row != qNIL ) {
			Item.Init( Ids(Row), Labels(Row), Wordings( Row ) );
			Items.Append( Item );

			Row = Ids.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	E_CDEF( char *, AmountAttribute, "Amount" );
	E_CDEF( char *, IdAttribute, "id" );
	E_CDEF( char *, LabelAttribute, "label" );
	E_CDEF( char *, WordingAttribute, "Wording" );

	template <typename id> inline void Dump_(
		const sclfrntnd::dIL_<id> &Item,
		xml::writer_ &Writer )
	{
		xml::PutAttribute( sclfrntnd::IdAttribute, **Item.Id(), Writer );
		Writer.PutAttribute( sclfrntnd::LabelAttribute, Item.Label );
	}

	template <typename id> inline void DumpIL_(
		const sclfrntnd::dIL_<id> &Item,
		xml::writer_ &Writer )
	{
		Dump_( Item, Writer );
	}

	template <typename id> inline void Dump_(
		const sclfrntnd::dILW_<id> &Item,
		xml::writer_ &Writer )
	{
# if 0	// Should work, but not with VC++.
		Dump_( (const sclfrntnd::dIL_<id>)Item, Writer );
# else
		DumpIL_( Item, Writer );
# endif
		Writer.PutValue( Item.Wording, sclfrntnd::WordingAttribute );
	}

	template <typename id, typename items> static void Dump_(
		const items &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		sdr::row__ Row = Items.First();

		Writer.PushTag( ItemsLabel );
		xml::PutAttribute( sclfrntnd::AmountAttribute, Items.Amount(), Writer );

		while ( Row != qNIL ) {
			Writer.PushTag( ItemLabel );

			Dump_<id>( Items( Row ), Writer );

			Writer.PopTag();

			Row = Items.Next( Row );
		}

		Writer.PopTag();
	}

	template <typename id> static void Dump(
		const ctn::E_MCONTAINER_( sclfrntnd::dIL_<id> ) &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump_<id, ctn::E_MCONTAINER_( sclfrntnd::dIL_<id> )>( Items, ItemsLabel, ItemLabel, Writer );
	}

	template <typename id> static void Dump(
		const ctn::E_CONTAINER_( sclfrntnd::dILW_<id> ) &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump_<id, ctn::E_CONTAINER_( sclfrntnd::dILW_<id> )>( Items, ItemsLabel, ItemLabel, Writer );
	}
}

/***************/
/***** NEW *****/
/***************/

namespace sclfrntnd {
	/* An identifier usually identifies the plugin used to access the backend.
	Identifier belows are returned when there in no bckend, or if the backend is embedded. */
	namespace identifier {
		qCDEF(char *, NoneBackendIdentifier, "_NONE" );
		qCDEF(char *, EmbeddedBackendIdentifier, "_EMBEDDED" );
	}

	typedef kernel___ rKernel;
	typedef frontend___ rFrontend;

	typedef features___ rFeatures;

	const str::dString &About(
		const rFeatures &Features,
		str::dString &About );
}

#endif
