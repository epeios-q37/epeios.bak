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

# include "err.h"
# include "flw.h"
# include "xml.h"

// NOTA : 'SCLF_' is used in place of 'SCLFRNTND_' for macro. 

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
		void Init( const features___ &Features );
		const csducl::universal_client_core___ &Core( void ) const
		{
			return _ClientCore;
		}
		csducl::universal_client_core___ &Core( void )
		{
			return _ClientCore;
		}
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

# define SCLF_I_( name, Name, id, Id  )\
	typedef fbltyp::id##__	name##_t__;\
	typedef fbltyp::id##s_	name##s_t_;\
	E_AUTO( name##s_t );\
\
	E_TMIMIC__( name##_t__, name##__ );\
	E_CDEF( name##__, Undefined##Name, fbltyp::Undefined##Id );\

	template <typename t> class il_	// id, label.
	{
	public:
		struct s {
			t Id;
			str::string_::s Label;
		} &S_;
		str::string_ Label;
		il_( s &S )
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
		void plug( qASd &AS )
		{
			Label.plug( AS );
		}
		il_ &operator =(const il_ &IL)
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

# define SCLF_IL( name, Name, id, Id  )\
	SCLF_I_( name, Name, id, Id );\
\
	typedef sclfrntnd::il_<name##__> name##_;\
	E_AUTO( name );\
\
	typedef ctn::E_MCONTAINER_( name##_ ) name##s_;\
	E_AUTO( name##s );


	template <typename t> class ilw_	// id, label, wording.
	: public il_<t>
	{
	public:
		struct s
		: public il_<t>::s
		{
			str::string_::s Wording;
		};
		str::string_ Wording;
		ilw_( s &S )
		: il_<t>( S ),
		  Wording( S.Wording )
		{}
		void reset( bso::bool__ P = true )
		{
			il_<t>::reset( P );
			Wording.reset( P );
		}
		void plug( qASd &AS )
		{
			il_<t>::plug( AS );
			Wording.plug( AS );
		}
		ilw_ &operator =(const ilw_ &ILW)
		{
			il_<t>::operator = ( ILW );
			Wording = ILW.Wording;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label,
			const str::string_ &Wording )
		{
			il_<t>::Init( Id, Label );
			this->Wording.Init( Wording );
		}
	};


# define SCLF_ILW( name, Name, id, Id  )\
	SCLF_I_( name, Name, id, Id );\
\
	typedef sclfrntnd::ilw_<name##__> name##_;\
	E_AUTO( name );\
\
	typedef ctn::E_CONTAINER_( name##_ ) name##s_;\
	E_AUTO( name##s );

	template <typename ids_t, typename item, typename items_> inline void Fill(
		const ids_t &Ids,
		const fbltyp::strings_ &Labels,
		items_ &Items )
	{
	qRH
		sdr::row__ Row = qNIL;
		item Item;
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

	template <typename ids_t, typename item, typename items_> inline void Fill(
		const ids_t &Ids,
		const fbltyp::strings_ &Labels,
		const fbltyp::strings_ &Wordings,
		items_ &Items )
	{
	qRH
		sdr::row__ Row = qNIL;
		item Item;
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

	template <typename id> static void Dump_(
		const sclfrntnd::il_<id> &Item,
		xml::writer_ &Writer )
	{
		xml::PutAttribute( sclfrntnd::IdAttribute, **Item.Id(), Writer );
		Writer.PutAttribute( sclfrntnd::LabelAttribute, Item.Label );
	}

	template <typename id> inline void Dump_(
		const sclfrntnd::ilw_<id> &Item,
		xml::writer_ &Writer )
	{
		Dump_( (sclfrntnd::il_<id>)Item, Writer );
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
		const ctn::E_MCONTAINER_( sclfrntnd::il_<id> ) &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump_<id, ctn::E_MCONTAINER_( sclfrntnd::il_<id> )>( Items, ItemsLabel, ItemLabel, Writer );
	}

	template <typename id> static void Dump(
		const ctn::E_CONTAINER_( sclfrntnd::ilw_<id> ) &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump_<id, ctn::E_CONTAINER_( sclfrntnd::ilw_<id> )>( Items, ItemsLabel, ItemLabel, Writer );
	}
}

#endif
