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

#ifndef FBLBKD__INC
# define FBLBKD__INC

# define FBLBKD_NAME		"FBLBKD"

# if defined( E_DEBUG ) && !defined( FBLBKD_NODBG )
#  define FBLBKD_DBG
# endif

// Frontend/Backend Layout BacKenD

# include "err.h"

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "tht.h"

# include "fblbrq.h"
# include "fbltyp.h"
# include "fblber.h"
# include "fblbrr.h"

# ifdef FBLBKD_DEFAULT_LANGUAGE
#  define FBLBKD__DEFAULT_LANGUAGE	FBLBKD_DEFAULT_LANGUAGE
# else
#  define FBLBKD__DEFAULT_LANGUAGE "fr"
# endif

//d An invalid value for a command.
# define FBLBKD_INVALID_COMMAND		FBLBRQ_INVALID_COMMAND
# define FBLBKD_COMMAND_MAX			FBLBRQ_COMMAND_MAX

//d An invalid value for a type.
# define FBLBKD_INVALID_TYPE		FBLBRQ_INVALID_TYPE
# define FBLBKD_TYPE_MAX			FBLBRQ_TYPE_MAX

//d An invalid value for an object.
# define FBLBKD_INVALID_OBJECT		FBLBRQ_INVALID_OBJECT
# define FBLBKD_OBJECT_MAX			FBLBRQ_OBJECT_MAX

# define FBLBKD_MASTER_OBJECT		FBLBKD_INVALID_OBJECT

# define FBLBKD_MASTER_TYPE		 	FBLBKD_INVALID_TYPE

# define FBLBKD_MASTER_COMMAND		FBLBKD_INVALID_COMMAND

//d This is the value of the index returned for a shared object already created.
# define FBLBKD_SHARED_OBJECT_INDEX	qNIL

# if 0
//d Value for an undefined Id32.
# define FBLBKD_UNDEFINED_ID32		FBLTYP_UNDEFINED_ID32

//d Value for an undefined Id16.
# define FBLBKD_UNDEFINED_ID16		FBLTYP_UNDEFINED_ID16

//d Value for an undefined Id8.
# define FBLBKD_UNDEFINED_ID8		FBLTYP_UNDEFINED_ID8

//d Value for an undefined Id8.
# define FBLBKD_UNDEFINED_ID		FBLTYP_UNDEFINED_ID
# endif

/***************/
/***** OLD *****/
/***************/

namespace fblbkd {
	using namespace fblbrq;
	using namespace fbltyp;

	typedef lcl::strings_	messages_;
	E_AUTO( messages )

	qROW( Index );
	qROW( IRow );	// Index row.
	typedef fbltyp::id16_t__	command__;

	typedef fbltyp::id16_t__	type_t__;
	E_TMIMIC__( type_t__, type__ );

	using fbltyp::object_t__;
	using fbltyp::object__;

	class log_functions__
	{
	protected:
		virtual void FBLBKDLog(
			const char *ObjectPrefix,
			const char *ObjectName,
			const str::string_ &MethodName,
			bso::bool__ Exit ) = 0;
	public:
		void Log(
			const char *ObjectPrefix,
			const char *ObjectName,
			const str::string_ &MethodName,
			bso::bool__ Exit )
		{
			FBLBKDLog( ObjectPrefix, ObjectName, MethodName, Exit );
		}
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		log_functions__( void )
		{
			reset( false );
		}
		~log_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standardisation.
		}
	};

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> class text_log_functions__
	: public log_functions__
	{
	private:
		fdr::byte__ _Cache[cache_size];
		flw::oflow__ _Flow;
	protected:
		txf::text_oflow__ TFlow;
		virtual void FBLBKDLog(
			const char *ObjectPrefix,
			const char *ObjectName,
			const str::string_ &MethodName,
			bso::bool__ Exit )
		{
			tol::buffer__ Buffer;
			TFlow << "[" << tol::DateAndTime( Buffer ) << "] (" << tht::GetTID() << ") : " << ( Exit ? "<-- " : "--> " ) << ObjectPrefix << ':' << ObjectName << ':' << MethodName << txf::nl << txf::commit;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			log_functions__::reset( P );
			_Flow.reset( P );
			TFlow.reset( P );
		}
		text_log_functions__( void )
		{
			reset( false );
		}
		~text_log_functions__( void )
		{
			reset();
		}
		void Init( fdr::oflow_driver_base___ &Driver )	// On s'appuie sur un 'fdr::oflow_fdr_base___', car cet objet est 'thread safe'.
		{
			reset();

			_Flow.Init( Driver, _Cache, sizeof( _Cache ) );
			TFlow.Init( _Flow );
			log_functions__::Init();
		}
	};


	typedef void (* function__ )(
		class backend___ &Backend,
		rRequest &Request );

	class rBaseModule_
	{
	private:
		// Prefix for the module/object name.
		const char *Prefix_;
		// Libelle du type de l'objet, du module.
		const char *Name_;
		// L'interface auquel le module est rattaché.
		class backend___ *Backend_;
	public:
		//r The description of the request.
		fblbrd::descriptions Descriptions;
		//r User pointers.
		bch::E_BUNCH( const void *) Functions;
		bch::qBUNCHw( sIndex, sIRow ) Indexes;
		void reset( bso::bool__ P = true )
		{
			Prefix_ = NULL;
			Name_ = NULL;
			Backend_ = NULL;

			Descriptions.reset( P );
			Functions.reset( P );
			Indexes.reset( P );
		}
		qCDTOR( rBaseModule_ );
		//f Give the name of the object.
		const char *Name( void ) const
		{
			return Name_;
		}
		//f Give the prefix of the object.
		const char *Prefix( void ) const
		{
			return Prefix_;
		}
		//f Get the raw messages.
#if 0
		void GetRawMessages( messages_ &RawMessages )
		{
			FBLBKDGetRawMessages( RawMessages );
		}
#endif
		/*f Initialization with the object prefix 'Prefix' and name 'Name'.
		'Prefix' and 'Name' are not duplicated. They should NOT be modified. */
		void Init(
			const char *Prefix,
			const char *Name,
			backend___ &Backend )
		{
			reset();

			Descriptions.Init();
			Functions.Init();
			Indexes.Init();

			Prefix_ = Prefix;
			Name_ = Name;
			Backend_ = &Backend;
		}
		//f Return the backend attached to this module.
		backend___ *Backend( void )
		{
			return Backend_;
		}
		//f Return the command which has 'Description' as description, or 'FBLBKD_INVALID_COMMAND' if non-existant.
		command__ Command( const fblbrd::description_ &Description ) const
		{
			sdr::row__ P = Descriptions.Position( Description );

			if ( P == qNIL )
				P = FBLBKD_INVALID_COMMAND;
			else if ( *P > FBLBKD_COMMAND_MAX )
				qRLmt();

			return (command__)*P;
		}
		sdr::row__ Add(
			const char *Name,
			const cast__ *Casts,
			const void *Function )
		{
			sdr::row__ Row = Descriptions.Add( Name, Casts );

			if ( Functions.Append( Function ) != Row )
				qRFwk();

			return Row;
		}
        template <typename... t> sdr::row__ Add(
			const char *Name,
			const void *Function,
            const t&... T )
        {
			sdr::row__ Row = Descriptions.Add( Name, T... );

			if ( Functions.Append( Function ) != Row )
				qRFwk();

			return Row;
        }
#if 0
		sdr::row__ Add(
			const char *Name,
			const void *Function,
			cast__ Cast,
			va_list VL )
		{
			sdr::row__ Row = Descriptions.Add( Name, Cast, VL );

			if ( Functions.Append( Function ) != Row )
				qRFwk();

			return Row;
		}
		sdr::row__ Add(
			const char *Name,
			const void *Function,
			cast__ Cast,
			... )
		{
			sdr::row__ Row = qNIL;
			va_list VL;

			va_start( VL, Cast );

			Row = Add( Name, Function, Cast, VL );

			va_end( VL );

			return Row;
		}
#endif
		friend class backend___;
		friend class master_module;
	};

		//c An untyped module.
	class rModule
	: public rBaseModule_
	{
	private:
		void _Clean( void );	// Permet l'effacement correct de chaque objet utilisateur.
	protected:
		//v To get the index of a new object.
		virtual sIndex FBLBKDNew( void )
		{
			qRFwk();
			return 0;	// Pour éviter un warning.
		}
		//v To delete the object with index 'Index'.
		virtual void FBLBKDDelete( sIndex Index )
		{
			qRFwk();
		}
		//v To get a pointer of the object of index 'Index'.
		virtual void *FBLBKDObject( sIndex Index ) const
		{
			qRFwk();
			return NULL;	// Pour éviter un 'warning'
		}
#if 0
		//v To get the raw messages.
		virtual void FBLBKDGetRawMessages( messages_ &Messages ) = 0;
#endif
		// Fonction appelée pour traiter la requête 'Requete' pour l'objet d'index 'Index'.
		virtual void Handle_(
			sIndex Index,
			rRequest &Requete,
			log_functions__ &LogFunctions ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_Clean();
			}

			rBaseModule_::reset( P );
		}
		E_CDTOR( rModule );
		//f Give the index of a new object.
		sIndex New( void )
		{
			sIndex Index = FBLBKDNew();

			Indexes.Append( Index );

			return Index;
		}
		//f Delete the object of index 'Index'.
		void Delete( sIndex Index )
		{
			sIRow Row = Indexes.Search( Index );

			if ( Row == qNIL )
				qRFwk();

			Indexes.Remove( Row );

			FBLBKDDelete( Index );
		}
		//f Give an pointer to the object of index 'Index'.
		void *Object( sIndex Index ) const
		{
			return FBLBKDObject( Index );
		}
		//f Get the raw messages.
#if 0
		void GetRawMessages( messages_ &RawMessages )
		{
			FBLBKDGetRawMessages( RawMessages );
		}
#endif
		/*f Initialization with the object prefix 'Prefix' and name 'Name'.
		'Prefix' and 'Name' are not duplicated. They should NOT be modified. */
		void Init(
			const char *Prefix,
			const char *Name,
			backend___ &Backend )
		{
			reset();

			rBaseModule_::Init( Prefix, Name, Backend );

			Descriptions.Init();
			Functions.Init();
			Indexes.Init();
		}
		//f Handle the request 'Request' for object of index 'Index'.
		void Handle(
			sIndex Index,
			rRequest &Requete,
			log_functions__ &LogFunctions )
		{
			Handle_( Index, Requete, LogFunctions );
		}
		command__ Command( const fblbrd::description_ &Description ) const
		{
			sdr::row__ P = Descriptions.Position( Description );

			if ( P == qNIL )
				P = FBLBKD_INVALID_COMMAND;
			else if ( *P > FBLBKD_COMMAND_MAX )
				qRLmt();

			return (command__)*P;
		}
		sdr::row__ Add(
			const char *Name,
			const cast__ *Casts,
			const void *Function )
		{
			sdr::row__ Row = Descriptions.Add( Name, Casts );

			if ( Functions.Append( Function ) != Row )
				qRFwk();

			return Row;
		}
        template <typename... t> sdr::row__ Add(
			const char *Name,
			const void *Function,
			cast__ Cast,
            const t&... T )
        {
			sdr::row__ Row = Descriptions.Add( Name, Cast, T... );

			if ( Functions.Append( Function ) != Row )
				qRFwk();

			return Row;
        }
#if 0
		sdr::row__ Add(
			const char *Name,
			const void *Function,
			cast__ Cast,
			va_list VL )
		{
			sdr::row__ Row = Descriptions.Add( Name, Cast, VL );

			if ( Functions.Append( Function ) != Row )
				qRFwk();

			return Row;
		}
		sdr::row__ Add(
			const char *Name,
			const void *UP,
			cast__ Cast,
			... )
		{
			sdr::row__ Row = qNIL;
			va_list VL;

			va_start( VL, Cast );

			Row = Add( Name, UP, Cast, VL );

			va_end( VL );

			return Row;
		}
#endif
		friend class backend___;
		friend class master_module;
	};

	//c A module for an object of type 't'.
	template <class t> class rModule_
	: public rModule
	{
	private:
		void Traiter_(
			t &Objet,
			rRequest &Requete,
			log_functions__ *LogFunctions )
		{
			command__ Command;

			flw::Get( Requete.Input(), Command );

			if ( Command > Descriptions.Amount() )
				qRFwk();

			Requete.Prepare( Descriptions( Command ).Casts );

			if ( LogFunctions != NULL )
				LogFunctions->Log( Prefix(), Name(), Descriptions( Command ).Name, false );

			Objet.HANDLE( *Backend(), *this, Command, Requete );

			if ( LogFunctions != NULL )
				LogFunctions->Log( Prefix(), Name(), Descriptions( Command ).Name, true );

		}
	protected:
#if 0
		virtual void FBLBKDGetRawMessages( messages_ &Messages )
		{
			t::RAW_MESSAGES( Messages );
		}
#endif
		virtual void Handle_(
			sIndex Index,
			rRequest &Requete,
			log_functions__ &LogFunctions ) override
		{
			Traiter_( *(t *)rModule_::Object( Index ), Requete, &LogFunctions );
		}
	public:
		//f Initialization.
		void Init( backend___ &Backend )
		{
			rModule::Init( t::PREFIX, t::NAME, Backend );

			t::NOTIFY( *this );
		}
	};

	//c A module with object stored in RAM.
	template <class t, class st> class ram_module
	: public rModule_<t>
	{
	protected:
		virtual sIndex FBLBKDNew( void ) override
		{
			t *Pointeur = NULL;

			if ( ( Pointeur = new t() ) == NULL )
				qRAlc();

			Pointeur->reset( false );

			sIndex Index = Objets.New();

			Objets.Store( Pointeur, Index );

			return Index;
		}
		virtual void FBLBKDDelete( sIndex Index ) override
		{
			t *Object = Objets( Index );

			Object->reset();

			delete Object;

			Objets.Delete( Index );
		}
		virtual void *FBLBKDObject( sIndex Index ) const override
		{
			if ( *Index >= Objets.Amount() )
				qRFwk();

			return (void *)( Objets( Index )->OBJECT() );
		}
	public:
		lstbch::qLBUNCHw( t *, sIndex ) Objets;
		void reset( bso::bool__ P = true )
		{
			rModule_<t>::reset( P );
			Objets.reset( P );
		}
		E_CDTOR( ram_module );
		//f Initialization.
		void Init( backend___ &Backend )
		{
			Objets.Init();
			rModule_<t>::Init( Backend );
		}
	};

	//c A module with object stored in standard memory.
	template <class t, class st> class standard_module
	: public rModule_<t>,
	  private lst::E_LIST
	{
	private:
		lst::E_LIST &_List( void )
		{
			return *this;
		}
		const lst::E_LIST &_List( void ) const
		{
			return *this;
		}
	protected:
		virtual void LSTAllocate( sdr::size__ Size )
		{
			Objets.Allocate( Size, aem::mFitted );
		}
		virtual sIndex FBLBKDNew( void )
		{
			return _List().New();
		}
		virtual void FBLBKDDelete( sIndex Index )
		{
			Objets( Index ).reset();
			_List().Delete( Index );
		}
		virtual void *FBLBKDObject( sIndex Index )
		{
			if ( *Index >= Objets.Amount() )
				qRFwk();

			return (void *)Objets( Index ).OBJECT();
		}
	public:
		//r Contient les objets.
		ctn::E_CONTAINER( t ) Objets;
		void reset( bool P = true )
		{
			Objets.reset( P );
			_List().reset( P );
		}
		E_CDTOR( standard_module );
		// Initialisation.
		void Init( void )
		{
			_List().Init();
			Objets.Init();
			rModule_<t>::Init();
		}
	};

	//c A shared module. One instance for all.
	template <class t, class st> class shared_module
	: public rModule_<t>
	{
	private:
		t *T_;
		bso::bool__ Created_;
	protected:
		virtual sIndex FBLBKDNew( void ) override
		{
			if ( Created_ )
				return FBLBKD_SHARED_OBJECT_INDEX;
			else {
				Created_ = true;
				return 0;
			}
		}
		virtual void FBLBKDDelete( sIndex Index ) override
		{
		}
		virtual void *FBLBKDObject( sIndex Index ) override
		{
			return T_->OBJECT();
		}
	public:
		void reset( bool P = true )
		{
			T_ = NULL;
			Created_ = false;
		}
		E_CDTOR( shared_module )
		// Initialisation.
		void Init( t &Object )
		{
			reset();
			rModule_<t>::Init();
			Created_ = false;
			T_ = &Object;
		}
	};

	// Module maître, qui fait tout le boulot.
	class rMasterModule
	: public rModule
	{
	protected:
		virtual void Handle_(
			sIndex Index,
			rRequest &Requete,
			log_functions__ &LogFunctions ) override;
	public:
		void Init( backend___ &Backend );
	};

	struct link__
	{
		// Le type de l'objet
		type__ Type;
		// L'index de l'objet;
		sIndex Index;
	};

	//t To by-pass a visual C++ bug.
	typedef lst::E_LISTtx( object__, object_t__ ) _listx;
	typedef tys::E_STORAGEt( link__, object__ ) _storage;

	// Classe de gestion des liens entre module et objets.
	class links
	: private _storage,
	  public _listx
	{
	protected:
		void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			_storage::Allocate( Size );
		}
	public:
		// Initialisation.
		void Init( void )
		{
			_listx::Init();
			_storage::Init();
		}
		object__ New(
			type__ IdType,
			sIndex Index )
		{
			link__ Lien;
			object__ P;

			P = _listx::New();

			if ( *P > FBLBKD_TYPE_MAX )
				qRLmt();

			Lien.Type = IdType;
			Lien.Index = Index;

			_storage::Store( Lien, P );

			return P;
		}
		void Delete( object__ IdObjet )
		{
			_listx::Delete( IdObjet );
		}
		type__ Type( object__ IdObjet ) const
		{
			return _storage::Get( IdObjet ).Type;
		}
		sIndex Index( object__ IdObjet ) const
		{
			return _storage::Get( IdObjet ).Index;
		}
		sdr::size__ Amount( void ) const
		{
			return _listx::Amount();
		}
	};


	//c A backend, which handles objects od different type and request to this object.
	class backend___
	{
	private:
		fblovl::eMode Mode_;
		TOL_CBUFFER___ _ClientOrigin;
		TOL_CBUFFER___ _APIVersion;
		rMasterModule Master_;
		TOL_CBUFFER___ _Language;
		const lcl::locale_ *_Locale;
		TOL_CBUFFER___ _BackendLabel;
		// Informations à propos du 'backend'.
		TOL_CBUFFER___ _ExtendedBackendInformations;
		TOL_CBUFFER___ _BackendCopyright;
		TOL_CBUFFER___ _SoftwareInformations;
		str::wString
			Key_,	// Key used for encrypting the codes.
			Code_;	// Code to allow a blocking ping or a crash.
		// Retourne le module correspondant à 'IdType'.
		rModule &Module_( type__ IdType )
		{
			if ( IdType != FBLBKD_MASTER_TYPE )
				return *Modules( *IdType );
			else
				return Master_;	// Not very happy about this conversion,
		}
		const rModule &Module_( type__ IdType ) const
		{
			if ( IdType != FBLBKD_MASTER_TYPE )
				return *Modules( *IdType );
			else
				return Master_;	// Not very happy about this conversion,
		}
		// Retourne le module correspondant à 'IdObjet'.
		rModule &Module_( object__ IdObjet )
		{
			return Module_( Type_( IdObjet ) );
		}
		const rModule &Module_( object__ IdObjet ) const
		{
			return Module_( Type_( IdObjet ) );
		}
		// Retourne le type correpondant à l'objet d'indetificateur 'IdObjet'.
		type__ Type_( object__ IdObjet ) const
		{
			return Links.Type( IdObjet );
		}
		// Retourne l'indexcorrespondant à l'objet d'identificateur 'IdObjet'.
		sIndex Index_( object__ IdObjet ) const
		{
			return Links.Index( IdObjet );
		}
		bso::bool__ _TestCompatibility(
			fdr::rRWDriver &FrontendIODriver,
			const char *APIVersion,
			const char *MessageLabel,
			const char *URLLabel );
	protected:
		virtual void *FBLBKDUserPointer( void ) = 0;
	public:
		//o The different modules.
		bch::E_BUNCH( rModule * ) Modules;
		//o The relation between modules an index.
		links Links;
		void reset( bso::bool__ P = true )
		{
			Mode_ = fblovl::m_Undefined;
			Key_.reset( P );
			Code_.reset( P );
		}
		E_CVDTOR( backend___ );
		// '[Backend|Publisher]Informations' ne sont PAS dupliqué. Leur contenu de doit pas être modifié.
		void Init(
			fblovl::eMode Mode,
			const char *APIVersion,
			const ntvstr::char__ *ClientOrigin,
			const char *BackendLabel,
			const lcl::locale_ &Locale,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations,
			const str::dString &Code,
			const str::dString &Key )
		{
		qRH
			str::string Buffer;
		qRB
			Master_.Init( *this );

			Modules.Init();
			Links.Init();

			Mode_ = Mode;

			str::string( BackendLabel ).Convert( _BackendLabel );

			Buffer.Init( BackendInformations );
			Buffer.Append( " Build " __DATE__ " " __TIME__ " - " );
			Buffer.Append( cpe::GetDescription() );
			Buffer.Convert( _ExtendedBackendInformations );

			str::string( BackendCopyright ).Convert( _BackendCopyright );
			str::string( SoftwareInformations ).Convert( _SoftwareInformations );
			ntvstr::string___( ClientOrigin ).UTF8( _ClientOrigin );
			str::string( APIVersion ).Convert( _APIVersion );

			_Locale = &Locale;

			str::string( FBLBKD__DEFAULT_LANGUAGE ).Convert( _Language );

			Key_.Init( Key );
			Code_.Init( Code );
		qRR
		qRT
		qRE
		}
		void *UP( void )
		{
			return FBLBKDUserPointer();
		}
		qRODISCLOSEr( str::dString, Code );
		qRODISCLOSEr( str::dString, Key );
		//f Add 'Module' to the interface.
		void Add( rModule &Module )
		{
			Module.Backend_ = this;
			Modules.Append( &Module );
		}
		bso::bool__ TestCompatibility( fdr::rRWDriver &FrontendIODriver );
		bso::bool__ HandleRequest(
			fdr::rRWDriver &FrontendIODriver,
			log_functions__ &LogFunctions );
		/*f Return the command corresponding at request description 'Description' and
		object type 'Type'. 'FBLBKD_INVALID_COMMAND' is returned if command not found. */
		command__ Command(
			type__ IdType,
			const fblbrd::description_ &Description ) const
		{
			return Module_( IdType ).Command( Description );
		}
		//f Return 'treu' if 'Object' exists, false otherwise.
		bso::bool__ Exists( object__ Object ) const
		{
			return Links.Exists( Object );
		}
		//f Give the module for the object of type 'Type'.
		const rModule &Module( type__ Type )
		{
			return Module_( Type );
		}
		//f Give the module for object 'Object'.
		const rModule &Module( object__ Object ) const
		{
			return Module_( Object );
		}
		//f Return a pointer to the 'Object' object.
		const void *GetUntypedObject(
			object__ Object,
			const char *Name ) const
		{
			if ( strcmp( Module_(Object).Name(), Name ) )
				qRFwk();

			return Module_( Object ).Object( Links.Index( Object ) );
		}
		template <typename object> object &Object( object__ Object )
		{
			return *(object *)GetUntypedObject( Object, object::NAME );
		}
		//f Give a new object.
		object__ New( type__ Type )
		{
			if ( (unsigned long)Links.Amount() >= (unsigned long)FBLBKD_TYPE_MAX )
				qRLmt();

			return Links.New( Type, Module_( Type ).New() );
		}
		//f Give the type of the object 'Object'.
		type__ Type( object__ Object) const
		{
			return Type_( Object );
		}
		//f Give the type of a nobject named 'Name'.
		type__ Type( const str::string_ &Name ) const;
		//f Delete object 'Object'.
		void Delete( object__ Object )
		{
			Module_( Object ).Delete( Links.Index( Object ) );

			Links.Delete( Object );
		}
		/*f Add a request description with name 'Name', function pointer 'FP'
		and a list of casts 'Casts'. The list must contain 2 'cEnd', the first
		at the end of the parameters casts,	and 1 of the end of returned values casts. */
		sdr::row__ Add(
			const char *Name,
			function__ FP,
			const cast__ *Casts )
		{
			return Master_.Add( Name, Casts, (void *)FP );
		}
        template <typename... t> sdr::row__ Add(
			const char *Name,
			function__ FP,
			cast__ Cast,
            const t&... T )
        {
			return Master_.Add( Name, (void *)FP, Cast, T... );
        }
#if 0
		sdr::row__ Add(
			const char *Name,
			function__ FP,
			cast__ Cast,
			va_list VL )
		{
			return Master_.Add( Name, (void *)FP, Cast, VL );
		}
		sdr::row__ Add(
			const char *Name,
			function__ FP,
			cast__ Cast,	// Added to avoid confusion with method above.
			... )
		{
			sdr::row__ Row = qNIL;
			va_list VL;

			va_start( VL, Cast );

			Row = Add( Name, FP, Cast, VL );

			va_end( VL );

			return Row;
		}
#endif
		const char *GetBackendLabel( void ) const
		{
			return _BackendLabel;
		}
		const char *GetExtendedBackendInformations( void ) const
		{
			return _ExtendedBackendInformations;
		}
		const char *GetBackendCopyright( void ) const
		{
			return _BackendCopyright;
		}
		const char *GetSoftwareInformations( void ) const
		{
			return _SoftwareInformations;
		}
		const char *GetAPIVersion( void ) const
		{
			return _APIVersion;
		}
		//f Return 'true' if 'Object' is of type given by 'Prefix' and 'Name'.
		bso::bool__ IsOfType(
			object__ Object,
			const char *Prefix,
			const char *Name ) const
		{
			return !strcmp( Module( Object ).Prefix(), Prefix ) && !strcmp( Module( Object ).Name(), Name );
		}
#if 0
		//f Return the raw messages.
		messages_ &GetMasterRawMessages( void )
		{
			return Master_.RawMessages;
		}
#endif
		const char *ClientOrigin( void ) const
		{
			return _ClientOrigin;
		}
		const lcl::locale_ &Locale() const
		{
			return *_Locale;
		}
		void SetLanguage( const char *Language )
		{
			str::string( Language ).Convert( _Language );
		}
		const char *Language( void ) const
		{
			return _Language;
		}
	};
}

//d A ram module of an object of type 't'.
# define FBLBKD_RAM_MODULE( t )	fblbkd::ram_module<t, t::s>

//d A standard module of an object of type 't'.
# define FBLBKD_STANDARD_MODULE( t )	fblbkd::standard_module<t, t::s>

//d A shared module of an object od type 't'.
# define FBLBKD_SHARED_MODULE( t )	fblbkd::shared_module<t, t::s>

/***************/
/***** NEW *****/
/***************/

namespace fblbkd {
	typedef backend___ rBackend;
}

#endif
