/*
	Header for the 'ctn' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2000-2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: ctn.h,v 1.98 2013/04/15 10:50:49 csimon Exp $

#ifndef CTN__INC
#define CTN__INC

#define CTN_NAME		"CTN"

#define	CTN_VERSION	"$Revision: 1.98 $"

#define CTN_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CTNTutor;

#if defined( E_DEBUG ) && !defined( CTN_NODBG )
#define CTN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.98 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:49 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D ConTaiNer (of dynamic objects) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ias.h"
#include "cpe.h"
#include "aem.h"


#ifdef CPE__VC
#	ifdef CPE__USE_WORKAROUNDS
#		define typename_
#	endif
#else
#	define	typename_	typename
#endif

namespace ctn {

	using sdr::size__;
	using aem::amount_extent_manager_;


	//c The base of a container. Internal use.
	template <class st, typename r> class basic_container_
	: public amount_extent_manager_<r>
	{
	private:
		void _Allocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
#ifdef CTN_DBG
			FlushTest();
#endif
			if ( amount_extent_manager_<r>::AmountToAllocate( Size, Mode ) ) {
				Dynamics.Allocate( Size, aem::mFit );
				Statics.Allocate( Size );
			}
		}

#ifdef CTN_DBG
	protected:
		virtual bso::bool__ IsFlushed( void ) const = 0;
#endif
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
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Dynamics.plug( AS );
			Statics.plug( AS );
	//		amount_extent_manager_::plug( M );	// Not relevant
		}
		basic_container_ &operator =( const basic_container_ &O )
		{
#ifdef CTN_DBG
			FlushTest();
			O.FlushTest();
#endif
			Dynamics.Copy( O.Dynamics, O.Amount() );
			Statics.Allocate( O.Amount() );
			Statics.Store( O.Statics, O.Amount() ); 

			amount_extent_manager_<r>::Force( O.Amount() );
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
#ifdef CTN_DBG
		//f Debug feature. If the container is not flushed, throw an error.
		void FlushTest( void ) const
		{
			if ( !IsFlushed() )
				ERRFwk();
		}
#endif
		//f Initialization.
		void Init( void )
		{
#ifdef CTN_DBG
			FlushTest();
#endif
			Dynamics.Init();
			Statics.Init();
			this->amount_extent_manager_<r>::Init();
//			amount_extent_manager_<r>::SetStepValue( 0 );	//Preallocation not very usefull for containers.
		}
		//f Allocation room for 'Size' object of statical part 'ST'.
		void Allocate(
			sdr::size__ Size,
			const st &ST,
			aem::mode__ Mode )
		{
#ifdef CTN_DBG
			FlushTest();
#endif
			sdr::size__ AncCap;
			sdr::size__ Amount = Size;

			AncCap = amount_extent_manager_<r>::Amount();

			_Allocate( Size, Mode );

			if ( AncCap < Size )
			{
#ifdef CTN_DBG
				if ( &ST == NULL )
					ERRFwk();
#endif
				if ( ( Size - AncCap ) > 1 )
					Statics.Store( ST, AncCap, Size - AncCap );
				else
					Statics.Store( ST, AncCap );
			}
		}
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
		void DecreaseTo(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
#ifdef CTN_DBG
			if ( Size > amount_extent_manager_<r>::Amount() )
				ERRFwk();
#endif
			Allocate( Size, *(st *)NULL, Mode );	// 'NULL' because this parameter is used only when size increased.
		}
		// Comme 'ecrire()', sauf pour la multim�moire, qui contient la partie dynamique.
	/*	void EcrireToutDansFlotSaufPartiesDynamiques( flo_sortie_ &Flot ) const
		{
			FLOEcrire( *AStorage.S_, Flot );
			Dynamique.ecrire( Flot );
			Statique.ecrire( Flot );
		}
		// Comme 'lire()', sauf pour la multim�moire, qui contient la partie dynamique.
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
#ifdef CTN_DBG
			FlushTest();
#endif
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
#ifdef CTN_DBG
			FlushTest();
#endif
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
	#ifdef CTN_DBG
			if ( Amount > this->Amount() )
				ERRPrm();
	#endif
			DecreaseTo( this->Amount() - Amount, Mode );
		}
		//f Remove objects all objects beginning at 'Row'.
		void Truncate(
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
#ifdef CTN_DBG
			if ( !Exists( Row ) )
				ERRFwk();
#endif
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
				ERRPrm();
#endif
			Crop( Last - First + 1, First, Mode );
		}
		static sdr::size__ GetStaticsItemSize( void )
		{
			return sizeof( st );
		}
		void SubInit( sdr::size__ Size )
		{
			amount_extent_manager_<r>::Init( Size );
			amount_extent_manager_<r>::SetStepValue( 0 );
		}
	};


	template <typename container> class container_file_manager___
	{
	private:
		tys::storage_file_manager___ _Statics;
		ias::indexed_aggregated_storage_file_manager___ _Dynamics;
	public:
		void reset( bso::bool__ P = true )
		{
			_Statics.reset( P );
			_Dynamics.reset( P );
		}
		container_file_manager___( void )
		{
			reset( false );
		}
		~container_file_manager___( void )
		{
			reset();
		}
		void Init( 
			const char *StaticsFileName,
			const char *DynamicsDescriptorsFileName,
			const char *DynamicsMultimemoryFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			fls::id__ ID )
		{
			_Statics.Init( StaticsFileName, Mode, Persistent, ID );
			_Dynamics.Init( DynamicsDescriptorsFileName, DynamicsMultimemoryFileName, Mode, Persistent, ID );
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = _Statics.State();

			if ( !uys::IsError( State ) )
				if ( State != _Dynamics.State() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Bind( void )
		{
			uys::state__ State = _Statics.Bind();

			if ( uys::IsError( State ) )
				return State;

			if ( _Dynamics.Bind() != State )
				State = uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Statics.Settle();

			if ( uys::IsError( State ) )
				return State;

			if ( _Dynamics.Settle() != State )
				State = uys::sInconsistent;

			return State;
		}
		void ReleaseFiles( void )
		{
			_Statics.ReleaseFile();
			_Dynamics.ReleaseFile();
		}
		fil::mode__ Mode( void ) const
		{
			fil::mode__ Mode = _Statics.Mode();

			if ( Mode != _Dynamics.Mode() )
				ERRPrm();

			return Mode;
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			fil::mode__ ModeBuffer = _Statics.Mode( Mode );

			if ( ModeBuffer != _Dynamics.Mode( ModeBuffer ) )
				ERRFwk();

			return ModeBuffer;
		}
		bso::bool__ IsPersistent( void ) const
		{
#ifdef CTN_DBG
			if ( _Statics.IsPersistent() != _Dynamics.IsPersistent() )
				ERRFwk();
#endif
			return _Statics.IsPersistent();
		}
		void Drop( void )
		{
			_Statics.Drop();
			_Dynamics.Drop();
		}
		tys::storage_file_manager___ &StaticsFileManager( void )
		{
			return _Statics;
		}
		ias::indexed_aggregated_storage_file_manager___ &DynamicsFileManager( void )
		{
			return _Dynamics;
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _Statics.Exists();

			if ( Exists != _Dynamics.Exists() )
				ERRFwk();

			return Exists;
		}
		time_t TimeStamp( void ) const
		{
			time_t StaticsTimeStamp, DynamicsTimeStamp;

			StaticsTimeStamp = _Statics.TimeStamp();
			DynamicsTimeStamp = _Dynamics.TimeStamp();

			return ( StaticsTimeStamp > DynamicsTimeStamp ? StaticsTimeStamp : DynamicsTimeStamp );
		}
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success = _Statics.CreateFile( ErrorHandling );

			if ( !Success )
				return false;

			Success = _Dynamics.CreateFiles( ErrorHandling );

			return Success;
		}
	};

	template <typename container, typename file_manager> inline uys::state__ Plug(
		container &Container,
		file_manager &FileManager )
	{
		uys::state__ State = tys::Plug( Container.Statics, FileManager.StaticsFileManager() );

		if ( uys::IsError( State ) ) {
			FileManager.reset();
			return State;
		}

		// Container.SetStepValue( 0 );	// Made by 'SubInit(...)'.

		if ( !uys::IsError( State ) ) {
			if ( ias::Plug( Container.Dynamics, FileManager.DynamicsFileManager() ) != State ) {
				FileManager.reset();
				State = uys::sInconsistent;
			} else
				Container.SubInit( Container.Dynamics.Descriptors.Amount() );
		}

		return State;
	}



	//c The base of a volatile item. Internal use.
	template <class st, typename r> class item_base_volatile__
	{
	private:
		bool Vide_( void ) const
		{
			return Pilote_.Index() == E_NIL;
		}
		void Vider_( void )
		{
			if ( !Vide_() ) {
#ifdef CTN_DBG
				if ( Conteneur_ == NULL )
				ERRFwk();
#endif
				Conteneur_->Statics.Store( ctn_S_, *Pilote_.Index() );
			}

			Pilote_.Index( E_NIL );
		}
	protected:
		// Conteneur auquel est rattach� l'�l�ment.
	//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
		basic_container_<st, r> *Conteneur_;
		/* Pilote permettant l'acc�s � la partie dynamique des objets contenus
		dans le conteneur auquel cet �l�ment est rattach�. */
		ias::indexed_aggregated_storage_driver__ Pilote_;
	public:
		struct s
		: public st
		{} ctn_S_; //pour ne pas risquer d'ambig�it�.
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
#ifdef CTN_DBG
			Conteneur->FlushTest();
#endif
			Conteneur_ = Conteneur;
			Pilote_.Init( Conteneur->Dynamics );
		}
		//* Cale l'�l�ment sur l'�l�ment du conteneur � la position 'Position'
		void Set( r Position )
		{
#ifdef CTN_DBG
			if ( Position == E_NIL )
				ERRPrm();
#endif
			if ( Pilote_.Index() != *Position )
			{
				Vider_();
	#ifdef CTN_DBG
				if ( Conteneur_ == NULL )
					ERRFwk();
	#endif
				Conteneur_->Statics.Recall( Position, ctn_S_ );
				Pilote_.Index( *Position );
			}
		}
		// Synchronise avec l'�l�ment du conteneur (leur contenu devient identique).
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
			Pilote_.Index( E_NIL );
		}
		r Index( void ) const
		{
			return *Pilote_.Index();
		}
	};

	template <class st, typename r> class item_base_const__
	{
	private:
		bool Vide_( void ) const
		{
			return Pilote_.Index() == E_NIL;
		}
		void Vider_( void )
		{
			Pilote_.Index( E_NIL );
		}
	protected:
		// Conteneur auquel est rattach� l'�l�ment.
	//	ctn_conteneur_base_ < ctn_item_mono < t, st > :: s  > *Conteneur_;
		const basic_container_<st,r> *Conteneur_;
		/* Pilote permettant l'acc�s � la partie dynamique des objets contenus
		dans le conteneur auquel cet �l�ment est rattach�. */
		ias::const_indexed_aggregated_storage_driver__ Pilote_;
	public:
		struct s
		: public st
		{} ctn_S_; //pour ne pas risquer d'ambig�it�.
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
#ifdef CTN_DBG
			Conteneur.FlushTest();
#endif
			Conteneur_ = &Conteneur;
			Pilote_.Init( Conteneur.Dynamics );
		}
		//* Cale l'�l�ment sur l'�l�ment du conteneur � la position 'Position'
		void Set( r Position )
		{
#ifdef CTN_DBG
			if ( Position == E_NIL )
				ERRPrm();
#endif
			if ( *Pilote_.Index() != *Position )
			{
				Vider_();
	#ifdef CTN_DBG
				if ( Conteneur_ == NULL )
					ERRFwk();
	#endif
				Conteneur_->Statics.Recall( Position, ctn_S_ );
				Pilote_.Index( *Position );
			}
		}
		// Synchronise avec l'�l�ment du conteneur (leur contenu devient identique).
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
			Pilote_.Index( E_NIL );
		}
		r Index( void ) const
		{
			return *Pilote_.Index();
		}
	};


	template <class st> struct item_mono_statique__
	: public st
	{
		sdr::size__ Extent;
	};

	/*c To reach an object from a 'MCONTAINER_( t )'. Use 'MITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_mono_item
	: public item_base_volatile__< item_mono_statique__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			item_base_volatile__< item_mono_statique__<typename_ t::s>, r >::reset( P );
				
			Objet_.reset( false );

			Objet_.plug( item_base_volatile__< item_mono_statique__< typename_ t::s >, r >::Pilote_ );
		}
		volatile_mono_item( void )
		: Objet_( item_base_volatile__< item_mono_statique__< typename_ t::s >, r >::ctn_S_ )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation. 
		volatile_mono_item( basic_container_< item_mono_statique__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< item_mono_statique__< typename t::s >, r >::ctn_S_ )
		{
			reset( false );

			item_base_volatile__< item_mono_statique__<typename_ t::s >, r >::Init( Conteneur );
		}
		virtual ~volatile_mono_item( void )
		{
			reset( true );
		}
		volatile_mono_item &operator =( const volatile_mono_item &O )
		{
			ERRFwk();
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
#ifdef CTN_DBG
			if ( item_base_volatile__< item_mono_statique__< typename_ t::s >, r >::IsFlushed() )
				ERRFwk();
#endif
			return Objet_;
		}
		const t &operator()( void ) const
		{
#ifdef CTN_DBG
			if ( item_base_volatile__< item_mono_statique__< typename_ t::s >, r >::IsFlushed() )
				ERRFwk();
#endif
			return Objet_;
		}
	};

	template <class t, typename r> class mono_container_;

	/*c To reach an object of type from a 'MCONTAINER_( t )', but only for reading.
	Use 'CMITEM( t )' rather then directly this class. */
	template <class t, typename r> class const_mono_item
	: public item_base_const__< item_mono_statique__< typename t::s >, r >
	{
	private:
		t Objet_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_const__< item_mono_statique__< typename_ t::s >, r >::Flush();
			}

			item_base_const__< item_mono_statique__<typename_ t::s >, r >::reset( P );
			Objet_.reset( false );

			Objet_.plug( item_base_const__< item_mono_statique__< typename_ t::s >, r >::Pilote_ );
		}
		const_mono_item( void )
		: Objet_( item_base_const__< item_mono_statique__< typename_ t::s >, r >::ctn_S_ )
		{
			reset( false );
		}
		virtual ~const_mono_item( void )
		{
			reset( true );
		}
		const_mono_item &operator =( const const_mono_item &O )
		{
			ERRFwk();
		}
		//f Return the object at 'Position'.
		const t &operator()( r Position )
		{
			this->Set( Position );
			return Objet_;
		}
		t &operator()( void )
		{
#ifdef CTN_DBG
			if ( item_base_const__< item_mono_statique__< typename_ t::s >, r >::IsEmpty() )
				ERRFwk();
#endif
			return Objet_;
		}
		//f Return the object at current position.
		const t &operator()( void ) const
		{
#ifdef CTN_DBG
			if ( item_base_const__< item_mono_statique__< typename_ t::s >, r >::IsEmpty() )
				ERRFwk();
#endif
			return Objet_;
		}
		//f Initialization with container 'Container'.
		void Init(const mono_container_< t , r > &Container )
		{
#ifdef CTN_DBG
			Container.FlushTest();
#endif
			item_base_const__< item_mono_statique__< typename_ t::s >, r >::Init( Container );
		}
	};



	//s To reach an item from 'MCONTAINER_( Type )'.
	#define E_MITEMt( Type, r )		volatile_mono_item< Type, r >
	#define E_CMITEMt( Type, r )	const_mono_item< Type, r >

#if defined( CPE__USE_GCC_WORKAROUND ) || defined( CPE__USE_WC_WORKAROUND )
	#define E_MITEM( Type )		volatile_mono_item< Type, sdr::row__ >
	#define E_CMITEM( Type )	const_mono_item< Type, sdr::row__ >
#else
	//s To reach an item from 'MCONTAINER_( Type )' , but only for reading.
	#define E_MITEM( Type )		E_MITEMt( Type, sdr::row__ )
	#define E_CMITEM( Type )	E_CMITEMt( Type, sdr::row__ )
#endif

	/*c Container for object of type 'Type', which need only one memory.
	Use 'MCONTAINER_( Type )' rather then directly this class. */
	template <class t, typename r> class mono_container_
	: public basic_container_< item_mono_statique__<typename t::s>, r >
	{
	private:
		E_MITEMt( t, r ) Ponctuel_;
	public:
		struct s
		: public basic_container_< item_mono_statique__<typename t::s>, r >::s
		{};
		mono_container_( s &S )
		: basic_container_< item_mono_statique__<typename_ t::s>, r >( S )
		{
			Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.reset( P );
			basic_container_< item_mono_statique__< typename_ t::s >, r >::reset( P );
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
		/*f Call this function after calling one of the two previous function
		and before calling another function. */
		void Flush( void )
		{
			Ponctuel_.Flush();
		}
		//f Return true if the container with its item, false otherwise.
		bso::bool__ IsFlushed( void ) const
		{
			return Ponctuel_.IsFlushed();
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& Get(
			r Position,
			E_CMITEMt( t, r ) &Item ) const
		{
#ifdef CTN_DBG
			if ( !IsFlushed() )
				ERRFwk();
#endif
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

			basic_container_< item_mono_statique__< typename_ t::s >, r >::Allocate( Size, E.ctn_S_, Mode );	// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		void Insert(
			const t &Object,
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
#ifdef CTN_DBG
			if ( !IsFlushed() )
				ERRFwk();
#endif
			E_CMITEMt( t, r ) E;

			basic_container_< item_mono_statique__< typename_ t::s >, r >::Insert( E.ctn_S_, Row, Mode );

			operator()( Row ) = Object;

			Flush();
		}
		mono_container_ &operator =( const mono_container_ &C )
		{
			Ponctuel_.Erase();
			
			basic_container_< item_mono_statique__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		r Append( const t &Object )
		{
			r P = New();

			operator()( P ) = Object;

			Flush();

			return P;
		}

		//f Create a new object and return its position.
		r New( sdr::size__ Size = 1 )
		{
			sdr::row_t__ P = this->Amount();

			Allocate( P + Size );

			return P;
		}
		//f Remove 'Amount' entries from 'Position'.
		void Remove(
			r Position,
			sdr::size__ Amount = 1,
			aem::mode__ Mode = aem::m_Default )
		{
#ifdef CTN_DBG
			if ( !IsFlushed() )
				ERRFwk();
#endif
			basic_container_< item_mono_statique__< typename_ t::s >, r >::Remove( Position, Amount, Mode );
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

#if defined( CPE__USE_GCC_WORKAROUND ) || defined( CPE__USE_WC_WORKAROUND )
	#define E_MCONTAINER_( Type )	mono_container_< Type, sdr::row__ >
	#define E_MCONTAINER( Type )	mono_container< Type, sdr::row__ >
#else
	#define E_MCONTAINER_( Type )	E_MCONTAINERt_( Type, sdr::row__ )
	#define E_MCONTAINER( Type )	E_MCONTAINERt( Type, sdr::row__ )
#endif

	template <class st> struct item_multi_statique__
	: public st
	{
		ags::aggregated_storage_::s AStorage;
		sdr::size__ Extent;
	};

	/*c To reach an object from a 'CONTAINER_( t )'. Use 'ITEM( t )'
	rather then directly this class. */
	template <class t, typename r> class volatile_multi_item
	: public item_base_volatile__< item_multi_statique__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		ags::aggregated_storage_ AStorage;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_volatile__< item_multi_statique__<typename_ t::s>, r >::Flush();
			}

			item_base_volatile__< item_multi_statique__<typename_ t::s>, r >::reset( P );

			Objet_.reset( false );
			AStorage.reset( P );

			AStorage.plug( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( AStorage );
		}
		volatile_multi_item( void )
		: Objet_( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );
		}
		// Remplace la fonction d'initialisation.
		volatile_multi_item( basic_container_< item_multi_statique__< typename t::s >, r > &Conteneur )
		: Objet_( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );

			AStorage.Init();
			item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::Init( Conteneur );
		}
		~volatile_multi_item( void )
		{
			reset( true );
		}
		//f Initialize with container 'Container', in mode 'Mode'.
		void Init( basic_container_< item_multi_statique__< typename t::s >, r > &Container )
		{
#ifdef CTN_DBG
			Container.FlushTest();
#endif
			AStorage.Init();
			item_base_volatile__< item_multi_statique__< typename_ t::s >, r >::Init( Container );
		}
		volatile_multi_item &operator =( const volatile_multi_item &O )
		{
			ERRFwk();
		}
		//f Return the object at 'Position'.
		t &operator()( r Position )
		{
			this->Set( Position );
			return Objet_;
		}
		t &operator()( void )
		{
#ifdef CTN_DBG
			if ( item_base_volatile__< item_multi_statique__<typename_ t::s>, r >::IsFlushed() )
				ERRFwk();
#endif
			return Objet_;
		}
		const t &operator()( void ) const
		{
#ifdef CTN_DBG
			if ( item_base_volatile__< item_multi_statique__<typename_ t::s>, r >::IsFlushed() )
				ERRFwk();
#endif
			return Objet_;
		}
	};


	/*c To reach an item from a 'CONTAINER_( t )', but only in read-only mode.
	Use 'CITEM( t )' rather then directly this class. */
	template <class t, typename r> class const_multi_item
	: public item_base_const__< item_multi_statique__<typename t::s>, r >
	{
	private:
		t Objet_;
	public:
		ags::aggregated_storage_ AStorage;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				item_base_const__< item_multi_statique__<typename_ t::s>, r >::Flush();
			}

			item_base_const__< item_multi_statique__<typename_ t::s>, r >::reset(  P) ;

			Objet_.reset( false );
			AStorage.reset( P );

			AStorage.plug( item_base_const__< item_multi_statique__< typename_ t::s >, r >::Pilote_ );
			Objet_.plug( AStorage );
		}
		const_multi_item( void )
		: Objet_( item_base_const__< item_multi_statique__< typename_ t::s >, r >::ctn_S_ ),
		  AStorage( item_base_const__< item_multi_statique__< typename_ t::s >, r >::ctn_S_.AStorage )
		{
			reset( false );
		}
		virtual ~const_multi_item( void )
		{
			reset( true );
		}
		//f Initializing with container 'Container'.
		void Init( const basic_container_< item_multi_statique__<typename t::s>, r > &Container )
		{
#ifdef CTN_DBG
			Container.FlushTest();
#endif
			AStorage.Init();
			item_base_const__< item_multi_statique__< typename_ t::s >, r >::Init( Container );
		}
		const_multi_item &operator =( const const_multi_item &O )
		{
			ERRFwk();
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
			if ( item_base_const__< item_multi_statique__<typename_ t::s>, r >::IsEmpty() )
				ERRFwk();
#endif
			return Objet_;
		}
	};


	#define E_ITEMt( Type, r )		volatile_multi_item< Type, r >
	#define E_CITEMt( Type,r  )		const_multi_item< Type, r  >

#if defined( CPE__USE_GCC_WORKAROUND ) || defined( CPE__USE_WC_WORKAROUND )
	#define E_ITEM( Type )			volatile_multi_item< Type, sdr::row__ >
	#define E_CITEM( Type )			const_multi_item< Type, sdr::row__  >
#else
	#define E_ITEM( Type )			E_ITEMt( Type, sdr::row__ )
	#define E_CITEM( Type )			E_CITEMt( Type, sdr::row__ )
#endif

	/*c Container for objects 't', with static part 'st', which need more then one memory.
	Use 'CONTAINER_( t )' rather then directly this class.*/
	template <class t, typename r> class multi_container_
	: public basic_container_< item_multi_statique__< typename t::s >, r >
	{
	private:
		E_ITEMt( t, r ) Ponctuel_;
	public:
		struct s
		: public basic_container_< item_multi_statique__< typename t::s >, r >::s
		{};
		multi_container_( s &S )
		: basic_container_< item_multi_statique__< typename_ t::s >, r >( S )
		{
			Ponctuel_.Init( *this );
		}
		void reset( bool P = true )
		{
			Ponctuel_.reset( P );
			basic_container_< item_multi_statique__< typename_ t::s >, r >::reset( P );
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
		bso::bool__ IsFlushed( void ) const
		{
			return Ponctuel_.IsFlushed();
		}
		/*f Return the object at 'Position' using 'Item'.
		Valid only until next modification of 'Item'. */
		const t& Get(
			r Position,
			E_CITEMt( t, r ) &Item ) const
		{
#ifdef CTN_DBG
			if ( !IsFlushed() )
				ERRFwk();
#endif
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

			basic_container_< item_multi_statique__< typename_ t::s >, r >::Allocate( Capacity, E.ctn_S_, Mode );// pas de E.xxx::ctn_S_ car G++ V2.90.29 n'aime pas
		}
		void Insert(
			const t &Object,
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
#ifdef CTN_DBG
			if ( !IsFlushed() )
				ERRFwk();
#endif
			E_CITEMt( t, r ) E;

			basic_container_< item_multi_statique__< typename_ t::s >, r >::Insert( E.ctn_S_, Row, Mode );

			operator()( Row ) = Object;

			Flush();
		}
		multi_container_ &operator =( const multi_container_ &C )
		{
			Ponctuel_.Erase();
			
			basic_container_< item_multi_statique__< typename_ t::s >, r >::operator =( C );

			return *this;
		}
		//f Create a new object and return its position.
		r New( sdr::size__ Size = 1 )
		{
			sdr::row_t__ P = this->Amount();

			Allocate( P + Size );

			return P;
		}
		r Append( const t &Object )
		{
			r P = New();

			operator()( P ) = Object;

			Flush();

			return P;
		}
		//f Remove 'Amount' entries from 'Position'.
		void Remove(
			r Position,
			sdr::size__ Amount = 1,
			aem::mode__ Mode = aem::m_Default )
		{
#ifdef CTN_DBG
			if ( !IsFlushed() )
				ERRFwk();
#endif
			basic_container_< item_multi_statique__< typename_ t::s >, r >::Remove( Position, Amount, Mode );
		}
		r Index( void ) const
		{
			return Ponctuel_.Index();
		}
	};

	E_AUTO2( multi_container )

	//s Container of object of type 'Type', which need more then one memory.
	#define E_CONTAINERt_( Type, r ) multi_container_< Type, r >
	#define E_CONTAINERt( Type, r ) multi_container< Type, r >

# if defined( CPE__USE_GCC_WORKAROUND ) || defined( CPE__USE_WC_WORKAROUND )
	#define E_CONTAINER_( Type )	multi_container_< Type, sdr::row__ >
	#define E_CONTAINER( Type )		multi_container< Type, sdr::row__ >
# else
	#define E_CONTAINER_( Type )	E_CONTAINERt_( Type, sdr::row__ )
	#define E_CONTAINER( Type )		E_CONTAINERt( Type, sdr::row__ )
# endif
}

#undef typename_

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
