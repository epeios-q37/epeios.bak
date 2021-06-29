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

#ifndef CTN_INC_
# define CTN_INC_

# define CTN_NAME		"CTN"

# if defined( E_DEBUG ) && !defined( CTN_NODBG )
#  define CTN_DBG
# endif

// ConTaiNer (of dynamic objects)

# include "err.h"
# include "flw.h"
# include "ias.h"
# include "cpe.h"
# include "aem.h"


#ifdef CPE_C_MSC
# define typename_
#else
# define	typename_	typename
#endif

/*************************/
/****** New version ******/
/*************************/

# define qMCONTAINERd( Type, r )	mono_container_< Type, r >
# define qMCONTAINERw( Type, r )	mono_container< Type, r >

# define qMCONTAINERdl( Type )		qMCONTAINERd( Type, sdr::row__ )
# define qMCONTAINERwl( Type )		qMCONTAINERw( Type, sdr::row__ )


# define qCONTAINERd( Type, r )		poly_container_< Type, r >
# define qCONTAINERw( Type, r )		poly_container< Type, r >

# define qCONTAINERdl( Type )		qCONTAINERd( Type, sdr::row__ )
# define qCONTAINERwl( Type )		qCONTAINERw( Type, sdr::row__ )*

# define qMITEMr( type, row )		volatile_mono_item< type, row >
# define qCMITEMs( type, row )		const_mono_item< type, row >

# define qMITEMrl( type )			qMITEMr( type, sdr::sRow )
# define qCMITEMsl( type )			qCMITEMs( type, sdr::sRow )

# define qITEMr( type, row )		volatile_poly_item< type, row >
# define qCITEMs( type, row )		const_poly_item< type, row >

# define qITEMrl( type )			qITEMr( type, sdr::sRow )
# define qCITEMsl( type )			qCITEMs( type, sdr::sRow )

/*************************/
/****** Old version ******/
/*************************/

namespace ctn {

	using sdr::size__;
	using aem::amount_extent_manager_;

	qHOOKS2( tys::sHook, Statics, ias::sHooks, Dynamics );

	typedef uys::sHook sHook_;
	typedef ias::indexed_aggregated_storage_driver__ rAHook_;

	//c The base of a container. Internal use.
	template <typename t, typename st, typename r> class basic_container_
	: public amount_extent_manager_<r>
	{
	private:
		void FlushObject_( void ) const
		{
			Object_.plug( NULL );	// Parameter = NULL means flushing.
		}
		void Flush_( bso::bool__ Reset ) const
		{
			if ( IsVolatile_ && IsSet_() ) {
				FlushObject_();
				Statics.Store( S_, *Hook_.Index() );
			}

			IsVolatile_ = false;

			if ( Reset )
				Hook_.Index( qNIL );
		}
		void _Allocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			Dynamics.Allocate( Size, Mode );

			if ( amount_extent_manager_<r>::Handle( Size, Mode ) ) {
				Statics.Allocate( Size );
			}
		}
		void Set_( r Row ) const
		{
			FlushObject_();

			Flush_( false );

			Hook_.Index( *Row );
			Statics.Recall( Row, S_ );
		}
		bso::sBool IsSet_( void ) const
		{
			return Hook_.Index() != qNIL;
		}
		void Get_(
			r Row,
			bso::sBool IsConst ) const
		{
			if ( Row == qNIL ) {
				if ( !IsSet_() )
					qRFwk();
				if ( !IsConst )
					IsVolatile_ = true;
			} else if ( Row != *Hook_.Index() ) {
				Set_( Row );

				IsVolatile_ = !IsConst;
			} else if ( !IsConst ) {
				IsVolatile_ = true;
			}
		}
		const t &Get_( r Row ) const
		{
			Get_( Row, true );

			return Object_;
		}
		t &Get_( r Row )
		{
			Get_( Row, false );

			return Object_;
		}
		bso::bool__ AppendInsteadOfInsert_(	r Row )
		{
			return ( ( Row == qNIL ) || ( ( amount_extent_manager_<r>::Amount() == 0 ) && ( Row == 0 ) ) );
		}
		void Insert_(
			const st &ST,
			r Row,
			aem::mode__ Mode )
		{
			_Allocate( amount_extent_manager_<r>::Amount() + 1, Mode );

			Statics.Store( Statics, amount_extent_manager_<r>::Amount() - 1 - *Row, *Row + 1, Row );
			Dynamics.Shift( *Row );

			Statics.Store( ST, Row );
		}
	protected:
		st Reseted_;	//Static part in a reseted state.
		mutable t Object_;
		mutable rAHook_ Hook_;
		mutable bso::sBool IsVolatile_;
	public:
		//r All the static parts.
		mutable tys::E_STORAGEt_( st, r ) Statics;
		//r All the dynamic parts.
		ias::indexed_aggregated_storage_ Dynamics;
		struct s
		: public aem::amount_extent_manager_<r>::s,
		  public st
		{
			typename tys::E_STORAGEt_( st, r )::s Statics;
			ias::indexed_aggregated_storage_::s Dynamics;
		} &S_;
		basic_container_( s &S )
		: amount_extent_manager_<r>( S ),
		  Object_( S.ST ),
		  Statics( S.Statics ),
		  Dynamics( S.Dynamics ),
		  S_( S )
		{}
		void reset( bool P = true )
		{
			if ( P )
				Flush_( true );

			Object_.reset( false ); // Due to 'Flush_()' above, its content is useless.
			Dynamics.reset( P );
			Statics.reset( P );
			amount_extent_manager_<r>::reset( P );
			Hook_.reset( P );
			IsVolatile_ = false;
		}
		void plug( sHooks &Hooks )
		{
			// 'Object' is plugged independently.
			Statics.plug( Hooks.Statics_ );
			Dynamics.plug( Hooks.Dynamics_ );
			_Allocate( Dynamics.Amount(), aem::mFitted );
			Hook_.Init( Dynamics );
		}
		void plug( qASd *AS )
		{
			if ( AS == NULL ) {
				Flush();
			} else {
				// 'Object' is plugged independently.
				Dynamics.plug( AS );
				Statics.plug( AS );
				Hook_.Init( Dynamics );
		//		amount_extent_manager_::plug( M );	// Not relevant
			}
		}
		basic_container_ &operator =( const basic_container_ &O )
		{
			Flush_( true );
			O.Flush_( true );

			size__ Size = O.Amount();

			Dynamics.Copy( O.Dynamics, Size );
			Statics.Allocate( Size );
			Statics.Store( O.Statics, Size );

			// Peu importe la valeur de retour, l'allocation des objets sous-jacent a dj t trait...
			amount_extent_manager_<r>::Handle( Size, aem::mFitted );
			amount_extent_manager_<r>::operator =( O );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Flush_( true );

			Dynamics.Init();
			Statics.Init();

			amount_extent_manager_<r>::Init();

			Hook_.Init( Dynamics );
		}
		void PreAllocate( sdr::size__ Size )
		{
			Dynamics.Init();
			Statics.Init();

			Dynamics.Allocate( Size );

			if ( amount_extent_manager_<r>::SetFixed( Size ) ) {
				Statics.Allocate( Size );
			}
		}
		void SetStepped( sdr::size__ Step )
		{
			Dynamics.SetStepped( Step );

			if ( amount_extent_manager_<r>::SetStepped( Step ) ) {
				Statics.Allocate( Step );
			}
		}
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			Flush_( true );

			sdr::size__ AncCap = amount_extent_manager_<r>::Amount();

			_Allocate( Size, Mode );

			if ( AncCap < Size )
			{
				if ( ( Size - AncCap ) > 1 )
					Statics.Fill( Reseted_, AncCap, Size - AncCap );
				else
					Statics.Store( Reseted_, AncCap );
			}
		}
		void DecreaseTo(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
# ifdef CTN_DBG
			if ( Size > amount_extent_manager_<r>::Amount() )
				qRFwk();
# endif
			Allocate( Size, Mode );
		}
		//f Remove 'Amount' entries from 'Position'.
		void Remove(
			r Position,
			sdr::size__ Amount = 1,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Amount == 0 )
				return;

			Flush_( true );

# ifdef CTN_DBG
			if ( ( *Position + Amount ) > this->Amount() )
				qRFwk();
# endif

			sdr::size__ CurrentAmount = amount_extent_manager_<r>::Amount();
			sdr::size__ NewAmount = CurrentAmount - Amount;

			Dynamics.RemoveWithoutReallocating( *Position, CurrentAmount, Amount );
			Statics.Store( Statics, NewAmount - *Position, Position, *Position + Amount );

			_Allocate( NewAmount, Mode );
		}
		//f Remove 'Amount' objects from the end of the container.
		void Truncate(
			sdr::size__ Amount = 1,
			aem::mode__ Mode = aem::m_Default )
		{
# ifdef CTN_DBG
			if ( Amount > this->Amount() )
				qRFwk();
# endif
			DecreaseTo( this->Amount() - Amount, Mode );
		}
		//f Remove objects all objects beginning at 'Row'.
		void Truncate(
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
# ifdef CTN_DBG
			if ( !Exists( Row ) )
				qRFwk();
# endif
			DecreaseTo( amount_extent_manager_<r>::Amount() - *Row, Mode );
		}
		//f Remove all objects but 'Amount()' objects from 'Row'. The size of the bunch is readjusted.
		void Crop(
			sdr::size__ Amount,
			r Row = 0,
			aem::mode__ Mode = aem::m_Default )
		{
			Truncate( this->Amount() - ( *Row + Amount ), Mode );

			Remove( 0, *Row, Mode );
		}
		//f Remove all objects but objects between 'First' and 'Last' included.
		void Crop(
			r First,
			r Last,
			aem::mode__ Mode = aem::m_Default )
		{
#ifdef BCH_DBG
			if ( Last < First )
				qRFwk();
#endif
			Crop( Last - First + 1, First, Mode );
		}
		static sdr::size__ GetStaticsItemSize( void )
		{
			return sizeof( st );
		}
		t &Get( r Row = qNIL )
		{
			return Get_( Row );
		}
		const t &Get( r Row = qNIL ) const
		{
			return Get_( Row );
		}
		t &operator()( r Row = qNIL )
		{
			return Get( Row );
		}
		const t &operator()( r Row = qNIL ) const
		{
			return Get( Row );
		}
		void Store(
			const t &Object,
			r Row )
		{
			Get( Row ) = Object;
		}
		void Recall(
			r Row,
			t &Object ) const
		{
			Object = Get( Row );
		}
		void InsertAt(
			const t &Object,
			r Row = 0,	// If == qNIL, appends the object.
			aem::mode__ Mode = aem::m_Default )
		{
			if ( AppendInsteadOfInsert_( Row ) )
				Append( Object, Mode );
			else {
				Flush_( true );

				Insert_( Reseted_, Row, Mode );

				Get( Row ) = Object;
			}

		}
		void Flush( void ) const
		{
			Flush_( true );
		}
		r Append(
			const t &Object,
			aem::mode__ Mode = aem::m_Default )
		{
			r P = New( Mode );

			operator()( P ) = Object;

			return P;
		}
		// Pour faciliter l'interchangeabilit avec les object du module 'lstctn'.
		r Add(
			const t &Object,
			aem::mode__ Mode = aem::m_Default )
		{
			return Append( Object, Mode );
		}
		//f Create a new object and return its position.
		r New(
			sdr::size__ Size = 1,
			aem::mode__ Mode = aem::m_Default )
		{
			FlushObject_();

			Flush_( true );

			sdr::row_t__ P = this->Amount();

			Allocate( P + Size, Mode );

			return P;
		}
		r New( aem::mode__ Mode )
		{
			return New( 1, Mode );
		}
	};

	template <typename statics, typename dynamics> class rH_
	: public sHooks
	{
	protected:
		statics Statics_;
		dynamics Dynamics_;
	public:
		rH_( void )
		: sHooks( Statics_ , Dynamics_ )
		{}
	};

	class rRH
	: public rH_<tys::rRH, ias::rRH>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Statics_.reset( P );
			Dynamics_.reset( P );
		}
		qCDTOR( rRH );
		void Init( void )
		{
			Statics_.Init();
			Dynamics_.Init();
		}
	};

	struct rHF
	{
	public:
		tys::rHF Statics;
		ias::rHF Dynamics;
		void reset( bso::bool__ P = true )
		{
			Statics.reset( P );
			Dynamics.reset( P );
		}
		E_CDTOR( rHF );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};

	class rFH
	: public rH_<tys::rFH, ias::rFH>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Statics_.reset( P );
			Dynamics_.reset( P );
		}
		qCDTOR( rFH );
		uys::eState Init(
			const rHF &Filenames,
			uys::mode__ Mode,
			uys::eBehavior Behavior,
			flsq::rId Id)
		{
			uys::eState State = Statics_.Init(Filenames.Statics, Mode, Behavior, Id);

			if ( !State.IsError() )
				if ( Dynamics_.Init(Filenames.Dynamics, Mode, Behavior, Id) != State )
					State = uys::sInconsistent;

			return State;
		}
	};

	//c The base of a volatile item. Internal use.
	template <typename t, typename st, typename r> class item_base_volatile__
	{
	private:
		bool Vide_( void ) const
		{
			return Pilote_.Index() == qNIL;
		}
		void Vider_( void )
		{
			if ( !Vide_() ) {
# ifdef CTN_DBG
				if ( Conteneur_ == NULL )
				qRFwk();
# endif
				Conteneur_->Statics.Store( ctn_S_, *Pilote_.Index() );
			}

			Pilote_.Index( qNIL );
		}
	protected:
		// Conteneur auquel est rattach l'lment.
	//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
		basic_container_<t, st, r> *Conteneur_;
		/* Pilote permettant l'accs  la partie dynamique des objets contenus
		dans le conteneur auquel cet lment est rattach. */
		rAHook_ Pilote_;
	public:
		struct s
		: public st
		{} ctn_S_; //pour ne pas risquer d'ambigit.
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Flush();
			}

			Pilote_.reset( P );

			Conteneur_ = NULL;
		}
		item_base_volatile__( void )
		{
			reset( false );
		}
		~item_base_volatile__( void )
		{
			reset( true );
		}
	/*	tym::row__ ctn_Position( void )
		{
			return Pilote_.Index();
		}
	*/
		// Rattache au conteneur 'Conteneur'.
		void Init( basic_container_<t, st, r> &Conteneur )
		{
			Conteneur.Flush();

			Init( &Conteneur );
		}
		// Rattache au conteneur 'Conteneur'.
		void Init( basic_container_<t, st,r> *Conteneur )
		{
			Conteneur->Flush();

			Conteneur_ = Conteneur;
			Pilote_.Init( Conteneur->Dynamics );
		}
		//* Cale l'lment sur l'lment du conteneur  la position 'Position'
		void Set( r Position )
		{
# ifdef CTN_DBG
			if ( Position == qNIL )
				qRFwk();
# endif
			if ( Pilote_.Index() != *Position )
			{
				Vider_();
# ifdef CTN_DBG
				if ( Conteneur_ == NULL )
					qRFwk();
# endif
				Conteneur_->Statics.Recall( Position, ctn_S_ );
				Pilote_.Index( *Position );
			}
		}
		// Synchronise avec l'lment du conteneur (leur contenu devient identique).
		void Flush( void )
		{
			Vider_();
		}
		// Return true if item synchronized with its container, false otherwise.
		bso::bool__ IsFlushed( void ) const
		{
			return Vide_();
		}
		// Return true id empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return Vide_();
		}
		// Set as flushed, but without flushing the content.
		void Erase( void )
		{
			Pilote_.Index( qNIL );
		}
		r Index( void ) const
		{
			return *Pilote_.Index();
		}
	};

	typedef ias::const_indexed_aggregated_storage_driver__ rCAHook_;

	template <typename t, typename st, typename r> class item_base_const__
	{
	private:
		bool Vide_( void ) const
		{
			return Pilote_.Index() == qNIL;
		}
		void Vider_( void )
		{
			Pilote_.Index( qNIL );
		}
	protected:
		// Conteneur auquel est rattach l'lment.
	//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
		const basic_container_<t, st,r> *Conteneur_;
		/* Pilote permettant l'accs  la partie dynamique des objets contenus
		dans le conteneur auquel cet lment est rattach. */
		rCAHook_ Pilote_;
	public:
		struct s
		: public st
		{} ctn_S_; //pour ne pas risquer d'ambigit.
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Flush();
			}

			Pilote_.reset( P );

			Conteneur_ = NULL;
		}
		item_base_const__( void )
		{
			reset( false );
		}
		~item_base_const__( void )
		{
			reset( true );
		}
	/*	mbs__position Position( void )
		{
			return Pilote_.Index();
		}
	*/	// Rattache au conteneur 'Conteneur'.
		void Init( const basic_container_<t, st,r> &Conteneur )
		{
			Conteneur.Flush();

			Conteneur_ = &Conteneur;
			Pilote_.Init( Conteneur.Dynamics );
		}
		//* Cale l'lment sur l'lment du conteneur  la position 'Position'
		void Set( r Position )
		{
# ifdef CTN_DBG
			if ( Position == qNIL )
				qRFwk();
# endif
			if ( *Pilote_.Index() != *Position )
			{
				Vider_();
# ifdef CTN_DBG
				if ( Conteneur_ == NULL )
					qRFwk();
# endif
				Conteneur_->Statics.Recall( Position, ctn_S_ );
				Pilote_.Index( *Position );
			}
		}
		// Synchronise avec l'lment du conteneur (leur contenu devient identique).
		void Flush( void )
		{
			Vider_();
		}
		bso::bool__ IsEmpty( void ) const
		{
			return Vide_();
		}
		// Set as flushed, but without flushing the content.
		void Erase( void )
		{
			Pilote_.Index( qNIL );
		}
		r Index( void ) const
		{
			return *Pilote_.Index();
		}
	};


	template <typename st> struct mono_static__
	{
		st ST;	// Not inherited, or there will be a conflict with 'aem::amount_extent_manager'.
//		sdr::size__ Extent;
	};

	/*c To reach an object from a 'MCONTAINER_( t )'. Use 'MITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_mono_item
	: public item_base_volatile__< t, mono_static__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			item_base_volatile__< t, mono_static__<typename_ t::s>, r >::reset( P );

			Objet_.reset( false );

			Objet_.plug( item_base_volatile__< t, mono_static__< typename_ t::s >, r >::Pilote_ );
		}
		volatile_mono_item( void )
		: Objet_( item_base_volatile__< t, mono_static__< typename_ t::s >, r >::ctn_S_ )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation.
		volatile_mono_item( basic_container_< t, mono_static__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< t, mono_static__< typename t::s >, r >::ctn_S_ )
		{
			reset( false );

			item_base_volatile__< t, mono_static__<typename_ t::s >, r >::Init( Conteneur );
		}
		virtual ~volatile_mono_item( void )
		{
			reset( true );
		}
		volatile_mono_item &operator =( const volatile_mono_item &O )
		{
			qRFwk();
		}
		//f Return the object at 'Position'.
		t &operator()( r Position )
		{
			this->Set( Position );
			return Objet_;
		}
		//f Return the object at current position.
		t &operator()( void )
		{
			if ( item_base_volatile__< t, mono_static__< typename_ t::s >, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
		const t &operator()( void ) const
		{
			if ( item_base_volatile__< t, mono_static__< typename_ t::s >, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
	};

	template <class t, typename r> class mono_container_;

	/*c To reach an object of type from a 'MCONTAINER_( t )', but only for reading.
	Use 'CMITEM( t )' rather then directly this class. */
	template <class t, typename r> class const_mono_item
	: public item_base_const__< t, mono_static__< typename t::s >, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_const__< t, mono_static__< typename_ t::s >, r >::Flush();
			}

			item_base_const__< t, mono_static__<typename_ t::s >, r >::reset( P );
			Objet_.reset( false );

			Objet_.plug(item_base_const__< t, mono_static__< typename_ t::s >, r >::Pilote_);
		}
		const_mono_item( void )
		: Objet_( item_base_const__< t, mono_static__< typename_ t::s >, r >::ctn_S_.ST )
		{
			reset( false );
		}
		virtual ~const_mono_item( void )
		{
			reset( true );
		}
		const_mono_item &operator =( const const_mono_item &O )
		{
			qRFwk();
		}
		//f Return the object at 'Position'.
		const t &operator()( r Position )
		{
			this->Set( Position );
			return Objet_;
		}
		t &operator()( void )
		{
# ifdef CTN_DBG
			if ( item_base_const__< t, mono_static__< typename_ t::s >, r >::IsEmpty() )
				qRFwk();
# endif
			return Objet_;
		}
		//f Return the object at current position.
		const t &operator()( void ) const
		{
# ifdef CTN_DBG
			if ( item_base_const__< t, mono_static__< typename_ t::s >, r >::IsEmpty() )
				qRFwk();
# endif
			return Objet_;
		}
		//f Initialization with container 'Container'.
		void Init(const mono_container_< t , r > &Container )
		{
			item_base_const__< t, mono_static__< typename_ t::s >, r >::Init( Container );
		}
	};

	/*c Container for object of type 'Type', which need only one memory.
	Use 'MCONTAINER_( Type )' rather then directly this class. */
	template <class t, typename r> class mono_container_
	: public basic_container_< t, mono_static__<typename t::s>, r >
	{
	private:
		void SetReseted_( void )
		{
			t O( basic_container_< t, mono_static__<typename t::s>, r >::Reseted_.ST );

			O.reset( false );
		}
	public:
		struct s
		: public basic_container_< t, mono_static__<typename t::s>, r >::s
		{};
		mono_container_( s &S )
		: basic_container_< t, mono_static__<typename_ t::s>, r >( S )
		{
			reset( false );
		}
		void reset( bool P = true )
		{
			SetReseted_();
			basic_container_< t, mono_static__< typename_ t::s >, r >::reset( P );
			basic_container_< t, mono_static__< typename_ t::s >, r >::Object_.plug(basic_container_< t, mono_static__< typename_ t::s >, r >::Hook_);
		}
		void FlushTest( void ) const
		{
			basic_container_< t, mono_static__<typename t::s>, r >::FlushTest();
		}
		mono_container_ &operator =( const mono_container_ &C )
		{
			basic_container_< t, mono_static__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		void Init( void )
		{
			basic_container_< t, mono_static__< typename_ t::s >, r >::Init();
		}
	};

	E_AUTO2( mono_container )

	//s Container of object of type 'Type', which need only one memory.
	#define E_MCONTAINERt_( Type, r )	mono_container_< Type, r >
	#define E_MCONTAINERt( Type, r )	mono_container< Type, r >

#if 0	// Was a workaround for some compilers.
	#define E_MCONTAINER_( Type )	mono_container_< Type, sdr::row__ >
	#define E_MCONTAINER( Type )	mono_container< Type, sdr::row__ >
#else
	#define E_MCONTAINER_( Type )	E_MCONTAINERt_( Type, sdr::row__ )
	#define E_MCONTAINER( Type )	E_MCONTAINERt( Type, sdr::row__ )
#endif

	template <class st> struct poly_static__
	: public mono_static__<st>
	{
		ags::aggregated_storage_::s AStorage;
//		sdr::size__ Extent;
	};

	/*c To reach an object from a 'CONTAINER_( t )'. Use 'ITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_poly_item
	: public item_base_volatile__< t, poly_static__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		ags::aggregated_storage_ AStorage;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_volatile__< t, poly_static__<typename_ t::s>, r >::Flush();
			}

			item_base_volatile__< t, poly_static__<typename_ t::s>, r >::reset( P );

			Objet_.reset( false );
			AStorage.reset( P );

			AStorage.plug( item_base_volatile__< t, poly_static__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( AStorage );
		}
		volatile_poly_item( void )
		: Objet_( item_base_volatile__< t, poly_static__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_volatile__< t, poly_static__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation.
		volatile_poly_item( basic_container_< t, poly_static__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< t, poly_static__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_volatile__< t, poly_static__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );

			AStorage.Init();
			item_base_volatile__< t, poly_static__< typename_ t::s >, r >::Init( Conteneur );
		}
		~volatile_poly_item( void )
		{
			reset( true );
		}
		//f Initialize with container 'Container', in mode 'Mode'.
		void Init( basic_container_< t, poly_static__< typename t::s >, r > &Container )
		{
			AStorage.Init();
			item_base_volatile__< t, poly_static__< typename_ t::s >, r >::Init( Container );
		}
		volatile_poly_item &operator =( const volatile_poly_item &O )
		{
			qRFwk();
		}
		//f Return the object at 'Position'.
		t &operator()( r Position )
		{
			this->Set( Position );
			return Objet_;
		}
		t &operator()( void )
		{
			if ( item_base_volatile__< t, poly_static__<typename_ t::s>, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
		const t &operator()( void ) const
		{
			if ( item_base_volatile__< t, poly_static__<typename_ t::s>, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
	};


	/*c To reach an item from a 'CONTAINER_( t )', but only in read-only mode.
	Use 'CITEM( t )' rather then directly this class. */
	template <class t, typename r> class const_poly_item
	: public item_base_const__< t, poly_static__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		ags::aggregated_storage_ AStorage;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_const__< t, poly_static__<typename_ t::s>, r >::Flush();
			}

			item_base_const__< t, poly_static__<typename_ t::s>, r >::reset(  P) ;

			Objet_.reset( false );
			AStorage.reset( P );

			AStorage.plug(item_base_const__< t, poly_static__< typename_ t::s >, r >::Pilote_);
			Objet_.plug( &AStorage );
		}
		const_poly_item( void )
		: Objet_( item_base_const__< t, poly_static__< typename_ t::s >, r >::ctn_S_.ST ),
		  AStorage( item_base_const__< t, poly_static__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );
		}
		virtual ~const_poly_item( void )
		{
			reset( true );
		}
		//f Initializing with container 'Container'.
		void Init( const basic_container_< t, poly_static__<typename t::s>, r > &Container )
		{
//			AStorage.Init();
			item_base_const__< t, poly_static__< typename_ t::s >, r >::Init( Container );
		}
		const_poly_item &operator =( const const_poly_item &O )
		{
			qRFwk();
		}
		//f Return the object at 'Position'.
		const t &operator()( r Position )
		{
			this->Set( Position );
			return Objet_;
		}
		const t &operator()( void ) const
		{
#ifdef CTN_DBG
			if ( item_base_const__< t, poly_static__<typename_ t::s>, r >::IsEmpty() )
				qRFwk();
#endif
			return Objet_;
		}
	};

	/*c Container for objects 't', with static part 'st', which need more then one memory.
	Use 'CONTAINER_( t )' rather then directly this class.*/
	template <class t, typename r> class poly_container_
	: public basic_container_< t, poly_static__< typename t::s >, r >
	{
	private:
		ags::aggregated_storage_ AStorage_;
	protected:
		void SetReseted_( void )
		{
			t O( basic_container_< t, poly_static__< typename t::s >, r >::Reseted_.ST );
			ags::aggregated_storage_ A( basic_container_< t, poly_static__< typename t::s >, r >::Reseted_.AStorage );

			O.reset( false );
			A.reset( false );
		}
	public:
		struct s
		: public basic_container_< t, poly_static__< typename t::s >, r >::s
		{};
		poly_container_( s &S )
		: basic_container_< t, poly_static__< typename_ t::s >, r >( S ),
		  AStorage_( S.AStorage )
		{
			reset( false );
		}
		void reset( bool P = true )
		{
			SetReseted_();
			basic_container_< t, poly_static__< typename_ t::s >, r >::reset( P );
			AStorage_.plug(basic_container_< t, poly_static__< typename_ t::s >, r >::Hook_);
			basic_container_< t, poly_static__< typename_ t::s >, r >::Object_.plug( &AStorage_ );
		}
		poly_container_ &operator =( const poly_container_ &C )
		{
			basic_container_< t, poly_static__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		void Init( void )
		{
			AStorage_.Init();
			basic_container_< t, poly_static__< typename t::s >, r >::Init();
		}
	};

	E_AUTO2( poly_container )

	//s Container of object of type 'Type', which need more then one memory.
	#define E_CONTAINERt_( Type, r ) poly_container_< Type, r >
	#define E_CONTAINERt( Type, r ) poly_container< Type, r >

# if 0	// Was a workaround for some compilers.
	#define E_CONTAINER_( Type )	poly_container_< Type, sdr::row__ >
	#define E_CONTAINER( Type )		poly_container< Type, sdr::row__ >
# else
	#define E_CONTAINER_( Type )	E_CONTAINERt_( Type, sdr::row__ )
	#define E_CONTAINER( Type )		E_CONTAINERt( Type, sdr::row__ )

	template <typename row, typename item, typename container> inline row Search_(
		const item &Item,
		const container &Container,
		row Row )
	{
		if ( Row == qNIL )
			Row = Container.First();

		while ( ( Row != qNIL ) && ( Container( Row ) != Item ) )
			Row = Container.Next( Row );

		return Row;
	}

	template <typename row, typename item> inline row Search(
		const item &Item,
		const ctn::mono_container_<item, row> &Container,
		sdr::sRow First = qNIL )
	{
		return Search_<row, item, ctn::mono_container_<item, row>>( Item, Container, First );
	}

	template <typename row, typename item> inline row Search(
		const item &Item,
		const ctn::poly_container_<item, row> &Container,
		sdr::sRow First = qNIL )
	{
		return Search_<row, item, ctn::poly_container_<item, row>>( Item, Container, First );
	}

# endif
}

#undef typename_

#endif
