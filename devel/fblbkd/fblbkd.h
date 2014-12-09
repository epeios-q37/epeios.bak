/*
	'fblbkd.h' by Claude SIMON (http://zeusw.org/).

	'fblbkd' is part of the Epeios framework.

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

#ifndef FBLBKD__INC
# define FBLBKD__INC

# define FBLBKD_NAME		"FBLBKD"

# if defined( E_DEBUG ) && !defined( FBLBKD_NODBG )
#  define FBLBKD_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Frontend/Backend Layout BacKenD

# include "err.h"// Frontend/Backend Layout BacKenD

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "tht.h"

# include "fblbrq.h"
# include "fbltyp.h"
# include "fblbur.h"

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
# define FBLBKD_SHARED_OBJECT_INDEX	E_NIL

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

namespace fblbkd {
	using namespace fblbrq;
	using namespace fbltyp;

	typedef lcl::strings_	messages_;
	E_AUTO( messages )

	typedef sdr::row__ index__;
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
		fdr::datum__ _Cache[cache_size];
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

			_Flow.Init( Driver, _Cache, sizeof( _Cache ), FLW_AMOUNT_MAX );
			TFlow.Init( _Flow );
			log_functions__::Init();
		}
	};


	typedef void (* function__ )(
		class backend___ &Backend,
		class untyped_module &Module,
		index__ Index,
		command__ Command,
		request__ &Request,
		bso::bool__ &,
		void *UP );

	//c An untyped module.
	class untyped_module
	{
	private:
		// Prefix for the module/object name.
		const char *Prefix_;
		// Libelle du type de l'objet, du module.
		const char *Name_;
		// L'interface auquel le module est rattaché.
		class backend___ *Backend_;
		void _Clean( void );	// Permet l'effacement correct de chaque objet utilisateur.
	protected:
		//v To get the index of a new object.
		virtual index__ FBLBKDNew( void )
		{
			ERRFwk();
			return 0;	// Pour éviter un warning.
		}
		//v To delete the object with index 'Index'.
		virtual void FBLBKDDelete( index__ Index )
		{
			ERRFwk();
		}
		//v To get a pointer of the object of index 'Index'.
		virtual void *FBLBKDObject( index__ Index )
		{
			ERRFwk();
			return NULL;	// Pour éviter un 'warning'
		}
#if 0
		//v To get the raw messages.
		virtual void FBLBKDGetRawMessages( messages_ &Messages ) = 0;
#endif
		// Fonction appelée pour traiter la requête 'Requete' pour l'objet d'index 'Index'.
		virtual void Handle_(
			index__ Index,
			request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions ) = 0;
	public:
		//r The description of the request.
		fblbrd::descriptions Descriptions;
		//r User pointers.
		bch::E_BUNCH( const void *) UPs;
		bch::E_BUNCH( index__ ) Indexes;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_Clean();
			}

			Prefix_ = NULL;
			Name_ = NULL;
			Backend_ = NULL;

			Descriptions.reset( P );
			UPs.reset( P );
			Indexes.reset( P );
		}
		E_CDTOR( untyped_module );
		//f Give the index of a new object.
		index__ New( void )
		{
			index__ Index = FBLBKDNew();

			Indexes.Append( Index );

			return Index;
		}
		//f Delete the object of index 'Index'.
		void Delete( index__ Index )
		{
			sdr::row__ Row = Indexes.Search( Index );

			if ( Row == E_NIL )
				ERRFwk();

			Indexes.Remove( Index );

			FBLBKDDelete( Index );
		}
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
		//f Give an pointer to the object of index 'Index'.
		void *Object( index__ Index )
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
			const char *Name )
		{
			reset();

			Descriptions.Init();
			UPs.Init();
			Indexes.Init();

			Prefix_ = Prefix;
			Name_ = Name;
		}
		//f Handle the request 'Request' for object of index 'Index'.
		void Handle(
			index__ Index,
			request__ &Requete,
			void *UP,
			log_functions__ &LogFunctions )
		{
			Handle_( Index, Requete, UP, LogFunctions );
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

			if ( P == E_NIL )
				P = FBLBKD_INVALID_COMMAND;
			else if ( *P > FBLBKD_COMMAND_MAX )
				ERRLmt();

			return (command__)*P;
		}
		sdr::row__ Add(
			const char *Name,
			const cast__ *Casts,
			const void *UP )
		{
			sdr::row__ Row = Descriptions.Add( Name, Casts );
			
			if ( UPs.Append( UP ) != Row )
				ERRFwk();
				
			return Row;
		}			
		sdr::row__ Add(
			const char *Name,
			const void *UP,
			cast__ Cast,
			va_list VL )
		{
			sdr::row__ Row = Descriptions.Add( Name, Cast, VL );
			
			if ( UPs.Append( UP ) != Row )
				ERRFwk();
				
			return Row;
		}			
		sdr::row__ Add(
			const char *Name,
			const void *UP,
			cast__ Cast,
			... )
		{
			sdr::row__ Row = E_NIL;
			va_list VL;

			va_start( VL, Cast );

			Row = Add( Name, UP, Cast, VL );

			va_end( VL );

			return Row;
		}
		friend class backend___;
		friend class master_module;
	};

	//c A module for an object of type 't'.
	template <class t, class user> class module
	: public untyped_module
	{
	private:
		void Traiter_(
			t &Objet,
			index__ Index,
			fblbrq::request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions )
		{
			command__ C;
			ctn::E_CITEM( fblbrd::description_ ) Description;

			flw::Get( Requete.Input(), C );

			if ( C > Descriptions.Amount() )
				ERRFwk();

			Description.Init( Descriptions );

			Requete.Prepare( Description( C ).Casts );

			if ( &LogFunctions != NULL )
				LogFunctions.Log( Prefix(), Name(), Description( C ).Name, false );

			Objet.HANDLE( *Backend(), *this, Index, C, Requete, PU );

			if ( &LogFunctions != NULL )
				LogFunctions.Log( Prefix(), Name(), Description( C ).Name, true );

		}
	protected:
#if 0
		virtual void FBLBKDGetRawMessages( messages_ &Messages )
		{
			t::RAW_MESSAGES( Messages );
		}
#endif
		virtual void Handle_(
			index__ Index,
			fblbrq::request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions ) override
		{
			Traiter_( *(t *)untyped_module::Object( Index ), Index, Requete, PU, LogFunctions );
		} 
	public:
		//f Initialization.
		void Init( user &User )
		{
			untyped_module::Init( t::PREFIX, t::NAME  );

			t::NOTIFY( *this, User );
		}
	};

	namespace {
		typedef lst::E_LIST_ _list_;
		E_AUTO( _list );
	}
	
	//c A module with object stored in RAM.
	template <class t, class st, class user> class ram_module
	: public module<t,user>,
	  private _list
	{
	private:
		const _list_ &_List( void ) const
		{
			return *this;
		}
		_list_ &_List( void )
		{
			return *this;
		}
	protected:
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode ) override
		{
#ifdef FBLBKD_DBG
			if ( Mode != aem::m_Default )
				ERRPrm();
#endif
			Objets.Allocate( Size, aem::mFitted );
		}
		virtual index__ FBLBKDNew( void ) override
		{
			st *S = NULL;
			t *Pointeur = NULL;

			if ( ( S = new st ) == NULL )
				ERRAlc();

			if ( ( Pointeur = new t( *S ) ) == NULL )
			{
				delete S;
				ERRAlc();
			}

			Pointeur->reset( false );

			index__ Index = _List().New();

			Objets.Store( Pointeur, Index );

			return Index;
		}
		virtual void FBLBKDDelete( index__ Index ) override
		{
			t *Object = Objets( Index );

			Object->reset();

			delete Object;

			_List().Delete( Index );
		}
		virtual void *FBLBKDObject( index__ Index ) override
		{
			if ( *Index >= Objets.Amount() )
				ERRPrm();

			return (void *)( Objets( Index )->OBJECT() );
		}
	public:
		//r The pointer object.
		bch::E_BUNCH( t * ) Objets;
		void reset( bso::bool__ P = true )
		{
			Objets.reset( P );
			module::reset( P );
			_list::reset( P );
		}
		E_CDTOR( ram_module );
		//f Initialization.
		void Init( user &User )
		{
			_List().Init();
			Objets.Init();
			module<t,user>::Init( User );
		}
	};
	
	//c A module with object stored in standard memory.
	template <class t, class st, class user> class standard_module
	: public module<t,user>,
	  private lst::E_LIST
	{
	private:
		ctn::E_ITEM( t ) Element_;
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
			Element_.Commit();
			Objets.Allocate( Size, aem::mFitted );
		}
		virtual index__ FBLBKDNew( void )
		{
			Element_.Commit();
			return _List().New();
		}
		virtual void FBLBKDDelete( index__ Index )
		{
			Element_( Index ).reset();
			_List().Delete( Index );
			Element_.Commit();
		}
		virtual void *FBLBKDObject( index__ Index )
		{
			if ( *Index >= Objets.Amount() )
				ERRPrm();

			return (void *)Element_( Index ).OBJECT();
		}
	public:
		//r Contient les objets.
		ctn::E_CONTAINER( t ) Objets;
		void reset( bool P = true )
		{
			Element_.Commit();
			Objets.reset( P );
			_List().reset( P );
		}
		E_CDTOR( standard_module );
		// Initialisation.
		void Init( user &User )
		{
			_List().Init();
			Objets.Init();
			Element_.Init( Objets );
			module<t,user>::Init( User );
		}
	};

	//c A shared module. One instance for all.
	template <class t, class st, class user> class shared_module
	: public module<t,user>
	{
	private:
		t *T_;
		bso::bool__ Created_;
	protected:
		virtual index__ FBLBKDNew( void )
		{
			if ( Created_ )
				return FBLBKD_SHARED_OBJECT_INDEX;
			else {
				Created_ = true;
				return 0;
			}
		}
		virtual void FBLBKDDelete( index__ Index )
		{
		}
		virtual void *FBLBKDObject( index__ Index )
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
		void Init(
			user &User,
			t &Object )
		{
			reset();
			module<t,user>::Init( User );
			Created_ = false;
			T_ = &Object;
		}
	};

	// Module maître, qui fait tout le boulot.
	class master_module
	: public untyped_module
	{
	protected:
		virtual void Handle_(
			index__ Index,
			fblbrq::request__ &Requete,
			void *PU,
			log_functions__ &LogFunctions );
#if 0
		virtual void FBLBKDGetRawMessages( messages_ &RawMessages )
		{
			RawMessages = this->RawMessages;
		}
#endif
	public:
#if 0
		//o All raw messages from all modules.
		messages RawMessages;
#endif
		// Initialisation avec rattachement à l'interface 'Backend'.
		void Init( backend___ &Backend );
	};

	struct link__
	{
		// Le type de l'objet
		type__ Type;
		// L'index de l'objet;
		index__ Index;
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
			index__ Index )
		{
			link__ Lien;
			object__ P;
			
			P = _listx::New();

			if ( *P > FBLBKD_TYPE_MAX )
				ERRLmt();

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
		index__ Index( object__ IdObjet ) const
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
		bso::bool__ _CompatibilityTested;
		fblbur::mode__ _Mode;
		TOL_CBUFFER___ _ClientOrigin;
		TOL_CBUFFER___ _APIVersion;
		master_module Master_;
		TOL_CBUFFER___ _Language;
		const lcl::locale_ *_Locale;
		TOL_CBUFFER___ _BackendLabel;
		// Informations à propos du 'backend'.
		TOL_CBUFFER___ _ExtendedBackendInformations;
		TOL_CBUFFER___ _BackendCopyright;
		TOL_CBUFFER___ _SoftwareInformations;
		fblber::embedded_callbacks___ _Embedded;
		fblbrr::remote_callbacks___ _Remote;
		// Retourne le module correspondant à 'IdType'.
		untyped_module &Module_( type__ IdType ) const
		{
			if ( IdType != FBLBKD_MASTER_TYPE )
				return *Modules( *IdType );
			else
				return (untyped_module &)Master_;	// Not very happy about this conversion, 
		}
		// Retourne le module correspondant à 'IdObjet'.
		untyped_module &Module_( object__ IdObjet ) const
		{
			if ( IdObjet != FBLBKD_MASTER_OBJECT )
				return Module_( Type_( IdObjet ) );
			else
				return (untyped_module &)Master_; // Not very happy about this conversion, 
		}
		// Retourne le type correpondant à l'objet d'indetificateur 'IdObjet'.
		type__ Type_( object__ IdObjet ) const
		{
			return Links.Type( IdObjet );
		}
		// Retourne l'indexcorrespondant à l'objet d'identificateur 'IdObjet'.
		index__ Index_( object__ IdObjet ) const
		{
			return Links.Index( IdObjet );
		}
		bso::bool__ _TestCompatibility(
			flw::ioflow__ &Flow,
			const char *APIVersion,
			const char *MessageLabel,
			const char *URLLabel );
		bso::bool__ _TestCompatibility( flw::ioflow__ &Flow );
		bso::bool__ _HandleRequest(
			flw::ioflow__ &FrontendFlow,
			void *PU,
			log_functions__ &LogFunctions );
	public:
		//o The different modules.
		bch::E_BUNCH( untyped_module * ) Modules;
		//o The relation between modules an index.
		links Links;
		void reset( bso::bool__ P = true )
		{
			_CompatibilityTested = false;
			_Mode = fblbur::m_Undefined;
			_Embedded.reset( P );
			_Remote.reset( P );
		}
		E_CVDTOR( backend___ );
		// '[Backend|Publisher]Informations' ne sont PAS dupliqué. Leur contenu de doit pas être modifié.
		void Init(
			fblbur::mode__ Mode,
			const char *APIVersion,
			const char *ClientOrigin,
			const char *BackendLabel,
			const lcl::locale_ &Locale,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations )
		{
		ERRProlog
			str::string Buffer;
		ERRBegin
			Master_.Init( *this );

			Modules.Init();
			Links.Init();

			_CompatibilityTested = false;
			_Mode = Mode;

			str::string( BackendLabel ).Convert( _BackendLabel );

			Buffer.Init( BackendInformations );
			Buffer.Append( " Build " __DATE__ " " __TIME__ " - " ); 
			Buffer.Append( cpe::GetDescription() );
			Buffer.Convert( _ExtendedBackendInformations );

			str::string( BackendCopyright ).Convert( _BackendCopyright );
			str::string( SoftwareInformations ).Convert( _SoftwareInformations );

			str::string( ClientOrigin ).Convert( _ClientOrigin );
			str::string( APIVersion ).Convert( _APIVersion );

			_Locale = &Locale;

			str::string( FBLBKD__DEFAULT_LANGUAGE ).Convert( _Language );

			_Embedded.Init();
			_Remote.Init();
		ERRErr
		ERREnd
		ERREpilog
		}

		//f Add 'Module' to the interface.
		void Add( untyped_module &Module )
		{
			Module.Backend_ = this;
			Modules.Append( &Module );
		}
		/*f Handle the request which come by 'Channel' and write the answer to 'Channel'.
		If 'true' is returned, than the request contains a deconnection request. */
		bso::bool__ Handle(
			flw::ioflow__ &FrontendFlow,
			void *PU,
			log_functions__ &LogFunctions )
		{
			if ( !_CompatibilityTested ) {
				_CompatibilityTested = true;
				return _TestCompatibility( FrontendFlow );
			}else
				return _HandleRequest( FrontendFlow, PU, LogFunctions );
		}
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
		const untyped_module &Module( type__ Type ) const
		{
			return Module_( Type );
		}
		//f Give the module for object 'Object'.
		const untyped_module &Module( object__ Object ) const
		{
			return Module_( Object );
		}
		//f Return a pointer to the 'Object' object.
		const void *Object( object__ Object ) const
		{
			return Module_( Object ).Object( Links.Index( Object ) );
		}
		//f Give a new object.
		object__ New( type__ Type )
		{
			if ( (unsigned long)Links.Amount() >= (unsigned long)FBLBKD_TYPE_MAX )
				ERRLmt();

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
			sdr::row__ Row = E_NIL;
			va_list VL;

			va_start( VL, Cast );

			Row = Add( Name, FP, Cast, VL );

			va_end( VL );

			return Row;
		}
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
# define FBLBKD_RAM_MODULE( t, user )	fblbkd::ram_module<t, t::s, user>	

//d A standard module of an object of type 't'.
# define FBLBKD_STANDARD_MODULE( t, user )	fblbkd::standard_module<t, t::s, user>	

//d A shared module of an object od type 't'.
# define FBLBKD_SHARED_MODULE( t, user )	fblbkd::shared_module<t, t::s, user>	


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
