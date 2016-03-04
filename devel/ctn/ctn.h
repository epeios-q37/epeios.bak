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

#ifndef CTN__INC
# define CTN__INC

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

# define qMCONTAINERv( Type, r )	mono_container_< Type, r >
# define qMCONTAINERi( Type, r )	mono_container< Type, r >

# define qMCONTAINERvl( Type )		qMCONTAINERv( Type, sdr::row__ )
# define qMCONTAINERil( Type )		qMCONTAINERi( Type, sdr::row__ )


# define qCONTAINERv( Type, r )		poly_container_< Type, r >
# define qCONTAINERi( Type, r )		poly_container< Type, r >

# define qCONTAINERvl( Type )		qCONTAINERv( Type, sdr::row__ )
# define qCONTAINER( Type )			qCONTAINERi( Type, sdr::row__ )*

# define qMITEM( type, row )	volatile_mono_item< type, row >
# define qCMITEM( type, row )	const_mono_item< type, row >

# define qMITEMl( type )		qMITEM( type, sdr::fRow )
# define qCMITEMl( type )		qCMITEM( type, sdr::fRow )

# define qITEMr( type, row )	volatile_poly_item< type, row >
# define qCITEMf( type, row )	const_poly_item< type, row >

# define qMITEMrl( type )		qMITEMr( type, sdr::fRow )
# define qCMITEMfl( type )		qCMITEMf( type, sdr::fRow )

/*************************/
/****** Old version ******/
/*************************/

namespace ctn {

	using sdr::size__;
	using aem::amount_extent_manager_;

	class cHooks
	{
	protected:
		virtual tys::cHook &CTNGetStaticsHook( void ) = 0;
		virtual ias::cHooks &CTNGetDynamicsHooks( void ) = 0;
	public:
		tys::cHook &GetStaticsHook( void )
		{
			return CTNGetStaticsHook();
		}
		ias::cHooks &GetDynamicsHooks( void )
		{
			return CTNGetDynamicsHooks();
		}
	};

	//c The base of a container. Internal use.
	template <class st, typename r> class basic_container_
	: public amount_extent_manager_<r>
	{
	private:
		void _Allocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			FlushTest();

			Dynamics.Allocate( Size, Mode );

			if ( amount_extent_manager_<r>::Handle( Size, Mode ) ) {
				Statics.Allocate( Size );
			}
		}
	protected:
		virtual bso::bool__ IsFlushed( void ) const = 0;
		void Insert(
			const st &ST,
			r Row,
			aem::mode__ Mode )
		{
			_Allocate( amount_extent_manager_<r>::Amount() + 1, Mode );

			Statics.Store( Statics, amount_extent_manager_<r>::Amount() - 1 - *Row, *Row + 1, Row );
			Dynamics.Shift( *Row );
			
			Statics.Store( ST, Row );
		}
	public:
		//r All the static parts.
		tys::E_STORAGEt_( st, r ) Statics;
		//r All the dynamic parts.
		ias::indexed_aggregated_storage_ Dynamics;
		struct s
		: public aem::amount_extent_manager_<r>::s
		{
			typename tys::E_STORAGEt_( st, r )::s Statics;
//			tys::E_STORAGEt_( st, r )::s Statics;
			ias::indexed_aggregated_storage_::s Dynamics;
		};
		basic_container_( s &S )
		: Dynamics( S.Dynamics ),
		  Statics( S.Statics ),
		  amount_extent_manager_<r>( S )
		{}
		void reset( bool P = true )
		{
			Dynamics.reset( P );
			Statics.reset( P );
			amount_extent_manager_<r>::reset( P );
		}
		void plug( cHooks &Hooks )
		{
			Statics.plug( Hooks.GetStaticsHook() );
			Dynamics.plug( Hooks.GetDynamicsHooks() );
			_Allocate( Dynamics.Amount(), aem::mFitted );
		}
		void plug( qASv &AS )
		{
			Dynamics.plug( AS );
			Statics.plug( AS );
	//		amount_extent_manager_::plug( M );	// Not relevant
		}
		basic_container_ &operator =( const basic_container_ &O )
		{
			FlushTest();
			O.FlushTest();

			size__ Size = O.Amount();

			Dynamics.Copy( O.Dynamics, Size );
			Statics.Allocate( Size );
			Statics.Store( O.Statics, Size ); 

			// Peu importe la valeur de retour, l'allocation des objets sous-jacent a dj t trait...
			amount_extent_manager_<r>::Handle( Size, aem::mFitted );
			amount_extent_manager_<r>::operator =( O );

			return *this;
		}
#if 0
		void write( flw::oflow__ &OFlow ) const
		{
			Dynamics.write( OFlow );
			Statics.write( OFlow );
		}
		void read( flw::iflow__ &IFlow )
		{
			Dynamics.read( IFlow );
			Statics.read( IFlow );
		}
#endif
		//f Debug feature. If the container is not flushed, throw an error.
		void FlushTest( void ) const
		{
# ifdef CTN_DBG
			if ( !IsFlushed() )
				qRFwk();
# endif
		}
		void SubInit( sdr::size__ Size )	// Obsolete ?
		{
			amount_extent_manager_<r>::Init();
			amount_extent_manager_<r>::Handle( Size, aem::mFitted );
		}
		//f Initialization.
		void Init( void )
		{
			FlushTest();

			Dynamics.Init();
			Statics.Init();

			amount_extent_manager_<r>::Init();
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
			const st &ST,
			aem::mode__ Mode )
		{
			FlushTest();

			sdr::size__ AncCap;
			sdr::size__ Amount = Size;

			AncCap = amount_extent_manager_<r>::Amount();

			_Allocate( Size, Mode );

			if ( AncCap < Size )
			{
				if ( ( Size - AncCap ) > 1 )
					Statics.Fill( ST, AncCap, Size - AncCap );
				else
					Statics.Store( ST, AncCap );
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
			Allocate( Size, *(st *)NULL, Mode );	// 'NULL' because this parameter is used only when size increased.
		}
		// Comme 'ecrire()', sauf pour la polymmoire, qui contient la partie dynamique.
	/*	void EcrireToutDansFlotSaufPartiesDynamiques( flo_sortie_ &Flot ) const
		{
			FLOEcrire( *AStorage.S_, Flot );
			Dynamique.ecrire( Flot );
			Statique.ecrire( Flot );
		}
		// Comme 'lire()', sauf pour la polymmoire, qui contient la partie dynamique.
		void LireToutDeFlotSaufPartiesDynamiques( flo_entree_ &Flot )
		{
			FLOLire( Flot, *AStorage.S_ );
			Dynamique.lire( Flot );
			Statique.lire( Flot );
		}
	*/
		//f Adjust the extent/amount to 'Size'.
#if 0
		void Adjust( void )
		{
			FlushTest();

			sdr::size__ Extent = this->Extent();

			if ( amount_extent_manager_<r>::Force( Size ) ) {
				Dynamics.Allocate( Size, Extent );
				Statics.Allocate( Size );
			}
		}
#endif
		//f Remove 'Amount' entries from 'Position'.
		void Remove(
			r Position,
			sdr::size__ Amount,
			aem::mode__ Mode )
		{
			FlushTest();

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
	};

	template <typename statics, typename dynamics> class rH_
	: public cHooks
	{
	protected:
		statics Statics_;
		dynamics Dynamics_;
		virtual tys::cHook &CTNGetStaticsHook( void ) override
		{
			return Statics_;
		}
		virtual ias::cHooks &CTNGetDynamicsHooks( void ) override
		{
			return Dynamics_;
		}
public:
		void reset( bso::fBool P = true )
		{
			Statics_.reset( P );
			Dynamics_.reset( P );
		}
		qCVDTOR( rH_ );
	};

	class rRH
	: public rH_<tys::rRH, ias::rRH>
	{
	public:
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
		uys::eState Init( 
			const rHF &Filenames,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			uys::eState State = Statics_.Init( Filenames.Statics, Mode, Behavior, ID );

			if ( !State.IsError() )
				if ( Dynamics_.Init( Filenames.Dynamics, Mode, Behavior, ID ) != State )
					State = uys::sInconsistent;

			return State;
		}
	};

	typedef uys::cHook cHook_;
	typedef ias::indexed_aggregated_storage_driver__ rADriver_;

	class rAHook_
	: public cHook_,
	  public rADriver_
	{
	protected:
		virtual sdr::fStorageDriver &UYSGetSD( void ) override
		{
			return *this;
		}
	};

	//c The base of a volatile item. Internal use.
	template <class st, typename r> class item_base_volatile__
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
		basic_container_<st, r> *Conteneur_;
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
		void Init( basic_container_<st, r> &Conteneur )
		{
			Init( &Conteneur );
		}
		// Rattache au conteneur 'Conteneur'.
		void Init( basic_container_<st,r> *Conteneur )
		{
			Conteneur->FlushTest();

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

	typedef ias::const_indexed_aggregated_storage_driver__ rCADriver_;

	class rCAHook_
	: public cHook_,
	  public rCADriver_
	{
	protected:
		virtual sdr::fStorageDriver &UYSGetSD( void ) override
		{
			return *this;
		}
	};


	template <class st, typename r> class item_base_const__
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
		const basic_container_<st,r> *Conteneur_;
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
		void Init( const basic_container_<st,r> &Conteneur )
		{
			Conteneur.FlushTest();

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


	template <class st> struct mono_static__
	: public st
	{
//		sdr::size__ Extent;
	};

	/*c To reach an object from a 'MCONTAINER_( t )'. Use 'MITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_mono_item
	: public item_base_volatile__< mono_static__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			item_base_volatile__< mono_static__<typename_ t::s>, r >::reset( P );
				
			Objet_.reset( false );

			Objet_.plug( item_base_volatile__< mono_static__< typename_ t::s >, r >::Pilote_ );
		}
		volatile_mono_item( void )
		: Objet_( item_base_volatile__< mono_static__< typename_ t::s >, r >::ctn_S_ )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation. 
		volatile_mono_item( basic_container_< mono_static__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< mono_static__< typename t::s >, r >::ctn_S_ )
		{
			reset( false );

			item_base_volatile__< mono_static__<typename_ t::s >, r >::Init( Conteneur );
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
			if ( item_base_volatile__< mono_static__< typename_ t::s >, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
		const t &operator()( void ) const
		{
			if ( item_base_volatile__< mono_static__< typename_ t::s >, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
	};

	template <class t, typename r> class mono_container_;

	/*c To reach an object of type from a 'MCONTAINER_( t )', but only for reading.
	Use 'CMITEM( t )' rather then directly this class. */
	template <class t, typename r> class const_mono_item
	: public item_base_const__< mono_static__< typename t::s >, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_const__< mono_static__< typename_ t::s >, r >::Flush();
			}

			item_base_const__< mono_static__<typename_ t::s >, r >::reset( P );
			Objet_.reset( false );

			Objet_.plug( item_base_const__< mono_static__< typename_ t::s >, r >::Pilote_ );
		}
		const_mono_item( void )
		: Objet_( item_base_const__< mono_static__< typename_ t::s >, r >::ctn_S_ )
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
			if ( item_base_const__< mono_static__< typename_ t::s >, r >::IsEmpty() )
				qRFwk();
# endif
			return Objet_;
		}
		//f Return the object at current position.
		const t &operator()( void ) const
		{
# ifdef CTN_DBG
			if ( item_base_const__< mono_static__< typename_ t::s >, r >::IsEmpty() )
				qRFwk();
# endif
			return Objet_;
		}
		//f Initialization with container 'Container'.
		void Init(const mono_container_< t , r > &Container )
		{
			Container.FlushTest();

			item_base_const__< mono_static__< typename_ t::s >, r >::Init( Container );
		}
	};



	// To reach an item from 'MCONTAINER_( Type )'.
# define E_MITEMt( Type, r )	qMITEM( Type, r )
# define E_CMITEMt( Type, r )	qCMITEM( Type, r )

#if 0	// Was a workaround for some compilers.
	#define E_MITEM( Type )		volatile_mono_item< Type, sdr::row__ >
	#define E_CMITEM( Type )	const_mono_item< Type, sdr::row__ >
#else
	//s To reach an item from 'MCONTAINER_( Type )' , but only for reading.
# define E_MITEM( Type )		qMITEMl( Type )
# define E_CMITEM( Type )		qCMITEMl( Type )
#endif

	/*c Container for object of type 'Type', which need only one memory.
	Use 'MCONTAINER_( Type )' rather then directly this class. */
	template <class t, typename r> class mono_container_
	: public basic_container_< mono_static__<typename t::s>, r >
	{
	private:
		E_MITEMt( t, r ) Ponctuel_;
		bso::bool__ _AppendInsteadOfInsert(	r Row )
		{
			return ( ( Row == qNIL ) || ( ( amount_extent_manager_<r>::Amount() == 0 ) && ( Row == 0 ) ) );
		}
	public:
		struct s
		: public basic_container_< mono_static__<typename t::s>, r >::s
		{};
		mono_container_( s &S )
		: basic_container_< mono_static__<typename_ t::s>, r >( S )
		{
			Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.reset( P );
			basic_container_< mono_static__< typename_ t::s >, r >::reset( P );
			Ponctuel_.Init( *this );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Flush()' before. */
		t &Get( r Position )
		{
			return Ponctuel_( Position );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Flush()' before. */
		t &operator()( r Position )
		{
			return Get( Position );
		}
		/*f Return the object at current position. This position is the position you
		gave to the previous function. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the previous,
		you MUST call the function 'Flush()' before. */
		t &operator()( void )
		{
			return Ponctuel_();
		}
		const t &operator()( void ) const
		{
			return Ponctuel_();
		}
		void FlushTest( void ) const
		{
			basic_container_< mono_static__<typename t::s>, r >::FlushTest();
		}
		/*f Call this function after calling one of the two previous function
		and before calling another function. */
		void Flush( void )
		{
			Ponctuel_.Flush();
		}
		//f Return true if the container with its item, false otherwise.
		bso::bool__ IsFlushed( void ) const override
		{
			return Ponctuel_.IsFlushed();
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& Get(
			r Position,
			E_CMITEMt( t, r ) &Item ) const
		{
			FlushTest();

			Item.Init( *this );

			return Item( Position );
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& operator()(
			r Position,
			E_CMITEMt( t, r ) &Item ) const
		{
			return Get( Position, Item );
		}
		//f Store 'Object' at 'Position'.
		r Store(
			const t & Objet,
			r Position )
		{
			operator()( Position ) = Objet;

			Flush();

			return Position;
		}
		//f Put in 'Object' the object at position 'Position'.
		void Recall(
			r Position,
			t &Objet ) const
		{
			E_CMITEMt( t, r ) Element;

			Objet = Get( Position, Element );
		}
		//f Allocate room for 'Size' objects.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			E_MITEMt( t, r ) E;

			basic_container_< mono_static__< typename_ t::s >, r >::Allocate( Size, E.ctn_S_, Mode );	// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		void InsertAt(
			const t &Object,
			r Row = 0,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( !IsFlushed() )
				qRFwk();

			if ( _AppendInsteadOfInsert( Row ) )
				Append( Object, Mode );
			else {
				E_CMITEMt( t, r ) E;

				basic_container_< mono_static__< typename_ t::s >, r >::Insert( E.ctn_S_, Row, Mode );

				operator()( Row ) = Object;

				Flush();
			}
		}
		mono_container_ &operator =( const mono_container_ &C )
		{
			Ponctuel_.Erase();
			
			basic_container_< mono_static__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		r Append(
			const t &Object,
			aem::mode__ Mode = aem::m_Default )
		{
			r P = New( Mode );

			operator()( P ) = Object;

			Flush();

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
			sdr::row_t__ P = this->Amount();

			Allocate( P + Size, Mode );

			return P;
		}
		r New( aem::mode__ Mode )
		{
			return New( 1, Mode );
		}
		//f Remove 'Amount' entries from 'Position'.
		void Remove(
			r Position,
			sdr::size__ Amount = 1,
			aem::mode__ Mode = aem::m_Default )
		{
			FlushTest();

			basic_container_< mono_static__< typename_ t::s >, r >::Remove( Position, Amount, Mode );
		}
		r Index( void ) const
		{
			return Ponctuel_.Index();
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
	: public st
	{
		ags::aggregated_storage_::s AStorage;
//		sdr::size__ Extent;
	};

	/*c To reach an object from a 'CONTAINER_( t )'. Use 'ITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_poly_item
	: public item_base_volatile__< poly_static__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		ags::aggregated_storage_ AStorage;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_volatile__< poly_static__<typename_ t::s>, r >::Flush();
			}

			item_base_volatile__< poly_static__<typename_ t::s>, r >::reset( P );

			Objet_.reset( false );
			AStorage.reset( P );

			AStorage.plug( item_base_volatile__< poly_static__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( AStorage );
		}
		volatile_poly_item( void )
		: Objet_( item_base_volatile__< poly_static__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_volatile__< poly_static__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation.
		volatile_poly_item( basic_container_< poly_static__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< poly_static__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_volatile__< poly_static__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );

			AStorage.Init();
			item_base_volatile__< poly_static__< typename_ t::s >, r >::Init( Conteneur );
		}
		~volatile_poly_item( void )
		{
			reset( true );
		}
		//f Initialize with container 'Container', in mode 'Mode'.
		void Init( basic_container_< poly_static__< typename t::s >, r > &Container )
		{
			Container.FlushTest();

			AStorage.Init();
			item_base_volatile__< poly_static__< typename_ t::s >, r >::Init( Container );
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
			if ( item_base_volatile__< poly_static__<typename_ t::s>, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
		const t &operator()( void ) const
		{
			if ( item_base_volatile__< poly_static__<typename_ t::s>, r >::IsFlushed() )
				qRFwk();

			return Objet_;
		}
	};


	/*c To reach an item from a 'CONTAINER_( t )', but only in read-only mode.
	Use 'CITEM( t )' rather then directly this class. */
	template <class t, typename r> class const_poly_item
	: public item_base_const__< poly_static__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		ags::aggregated_storage_ AStorage;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_const__< poly_static__<typename_ t::s>, r >::Flush();
			}

			item_base_const__< poly_static__<typename_ t::s>, r >::reset(  P) ;

			Objet_.reset( false );
			AStorage.reset( P );

			AStorage.plug( item_base_const__< poly_static__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( AStorage );
		}
		const_poly_item( void )
		: Objet_( item_base_const__< poly_static__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_const__< poly_static__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );
		}
		virtual ~const_poly_item( void )
		{
			reset( true );
		}
		//f Initializing with container 'Container'.
		void Init( const basic_container_< poly_static__<typename t::s>, r > &Container )
		{
			Container.FlushTest();

//			AStorage.Init();
			item_base_const__< poly_static__< typename_ t::s >, r >::Init( Container );
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
			if ( item_base_const__< poly_static__<typename_ t::s>, r >::IsEmpty() )
				qRFwk();
#endif
			return Objet_;
		}
	};


	#define E_ITEMt( Type, r )		volatile_poly_item< Type, r >
	#define E_CITEMt( Type,r  )		const_poly_item< Type, r  >

#if 0 // Was a workaround for some compiler.
	#define E_ITEM( Type )			volatile_poly_item< Type, sdr::row__ >
	#define E_CITEM( Type )			const_poly_item< Type, sdr::row__  >
#else
	#define E_ITEM( Type )			E_ITEMt( Type, sdr::row__ )
	#define E_CITEM( Type )			E_CITEMt( Type, sdr::row__ )
#endif

	/*c Container for objects 't', with static part 'st', which need more then one memory.
	Use 'CONTAINER_( t )' rather then directly this class.*/
	template <class t, typename r> class poly_container_
	: public basic_container_< poly_static__< typename t::s >, r >
	{
	private:
		E_ITEMt( t, r ) Ponctuel_;
	public:
		struct s
		: public basic_container_< poly_static__< typename t::s >, r >::s
		{};
		poly_container_( s &S )
		: basic_container_< poly_static__< typename_ t::s >, r >( S )
		{
			Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.reset( P );
			basic_container_< poly_static__< typename_ t::s >, r >::reset( P );
			Ponctuel_.Init( *this );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Flush()' before. */
		t &Get( r Position )
		{
			return Ponctuel_( Position );
		}
		/*f Return the object at position 'Position'. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the next, you MUST call
		the function 'Flush()' before. */
		t &operator()( r Position )
		{
			return Get( Position );
		}
		/*f Return the object at current position. This position is the position you
		gave to the previous function. BE CAREFUL: after calling this fonction
		and if you want to call another fonction as this fonction or the previous,
		you MUST call the function 'Flush()' before. */
		t &operator()( void )
		{
			return Ponctuel_();
		}
		const t &operator()( void ) const
		{
			return Ponctuel_();
		}
		/*f Always call this function after calling one of the two previous function
		and before calling another function. */
		void Flush( void )
		{
			Ponctuel_.Flush();
		}
		//f Return true if the container with its item, false otherwise.
		bso::bool__ IsFlushed( void ) const override
		{
			return Ponctuel_.IsFlushed();
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& Get(
			r Position,
			E_CITEMt( t, r ) &Item ) const
		{
			if ( !IsFlushed() )
				qRFwk();

			Item.Init( *this );

			return Item( Position );
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& operator()(
			r Position,
			E_CITEMt( t, r ) &Item ) const
		{
			return Get( Position, Item );
		}
		//f Store 'Object' at 'Position'.
		r Store(
			const t & Object,
			r Position )
		{
			operator()( Position ) = Object;

			Flush();

			return Position;
		}
		//f Put in 'Object' the object at position 'Position'.
		void Recall(
			r Position,
			t &Objet ) const
		{
			E_CITEMt( t, r ) Element;

			Objet = Get( Position, Element );
		}
		//f Allocate room for 'Capacity' objects.
		void Allocate(
			sdr::size__ Capacity,
			aem::mode__ Mode = aem::m_Default )
		{
			E_ITEMt( t, r ) E;

			basic_container_< poly_static__< typename_ t::s >, r >::Allocate( Capacity, E.ctn_S_, Mode );// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		void InsertAt(
			const t &Object,
			r Row = 0,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( _AppendInsteadOfInsert( Row ) )
				Append( Object, Mode );
			else {
				if ( !IsFlushed() )
					qRFwk();

				E_CITEMt( t, r ) E;

				basic_container_< poly_static__< typename_ t::s >, r >::Insert( E.ctn_S_, Row, Mode );

				operator()( Row ) = Object;

				Flush();
			}
		}
		poly_container_ &operator =( const poly_container_ &C )
		{
			Ponctuel_.Erase();
			
			basic_container_< poly_static__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		//f Create a new object and return its position.
		r New(
			sdr::size__ Size = 1,
			aem::mode__ Mode = aem::m_Default )
		{
			sdr::row_t__ P = this->Amount();

			Allocate( P + Size, Mode );

			return P;
		}
		r New( aem::mode__ Mode )
		{
			return New( 1, Mode );
		}
		r Append(
			const t &Object,
			aem::mode__ Mode = aem::m_Default )
		{
			r P = New( Mode );

			operator()( P ) = Object;

			Flush();

			return P;
		}
		// Pour faciliter l'interchangeabilit avec les object du module 'lstctn'.
		r Add(
			const t &Object,
			aem::mode__ Mode = aem::m_Default )
		{
			return Append( Object, Mode );
		}
		//f Remove 'Amount' entries from 'Position'.
		void Remove(
			r Position,
			sdr::size__ Amount = 1,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( !IsFlushed() )
				qRFwk();

			basic_container_< poly_static__< typename_ t::s >, r >::Remove( Position, Amount, Mode );
		}
		r Index( void ) const
		{
			return Ponctuel_.Index();
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
# endif
}

#undef typename_

#endif
