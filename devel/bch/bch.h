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

#ifndef BCH_INC_
# define BCH_INC_

# define BCH_NAME		"BCH"

# if defined( E_DEBUG ) && !defined( BCH_NODBG )
#  define BCH_DBG
# endif

// BunCH

# include "err.h"
# include "flw.h"
# include "tys.h"
# include "aem.h"
# include "dtfptb.h"
# include "flsq.h"

# ifndef IAS_INC_
#  undef BCH_INC_
#  include "ias.h"
# else

/*************************/
/****** New version ******/
/*************************/

# define qBUNCHd( type, row ) E_BUNCHt_( type, row )
# define qBUNCHw( type, row ) E_BUNCHt( type, row )

# define qBUNCHdl( type ) qBUNCHd( type, sdr::sRow )
# define qBUNCHwl( type ) qBUNCHw( type, sdr::sRow )


/*************************/
/****** Old version ******/
/*************************/


namespace bch {

	//c The kernel of a bunch. For internal use only. The 'sh' template parameter is to simplify the use of the 'STR' library.
	template <class type, class mmr, class mng, typename row, typename sh> class _bunch
	: public mmr,
	  public mng
	{
	private:
		/* Pousse (dcalge vers la fin de l'ensemble) les objets  la position
		'Position' de 'Quantite' positions */
		void Pousser_(
			sdr::row_t__ Position,
			sdr::size__ Quantite )
		{
			Allouer_( mng::Amount() + Quantite, aem::m_Default );
			mmr::Store( *this, mng::Amount() - Position - Quantite, Position + Quantite, Position );
		}
		// Insere  'PosDest' 'Quantite' objets situ  partir de 'PosSource' de 'Source'.
		void InsertAt_(
			const mmr &Source,
			sdr::size__ Quantite,
			sdr::row_t__ PosSource,
			sdr::row_t__ PosDest )
		{
			Pousser_( PosDest, Quantite );
			mmr::Store( Source, Quantite, PosDest, PosSource );
		}
		// Insere 'Quantite' objets de 'Objets'  'Position'.
		void InsertAt_(
			const type *Objets,
			sdr::size__ Quantite,
			sdr::row_t__ Position )
		{
			Pousser_( Position, Quantite );
			mmr::Store( Objets, Quantite, Position );
		}
		// Allocation de la place ncessaire  'Taille' objets.
		void Allouer_(
			sdr::size__ Taille,
			aem::mode__ Mode )
		{
			if ( mng::Handle( Taille, Mode ) )
				mmr::Allocate( Taille );
		}
		// allocate if the set not big enough.
		void AllocateIfNecessary_( sdr::size__ Quantity )
		{
			if ( Quantity > mng::Amount() )
				Allouer_( Quantity, aem::m_Default );
		}
		bso::bool__ _AppendInsteadOfInsert(	row Row )
		{
			return ( ( Row == qNIL ) || ( ( mng::Amount() == 0 ) && ( Row == 0 ) ) );
		}
	public:
		struct s
		: public mmr::s,
		  public mng::s
		{};
		_bunch( s &S )
		: mmr( S ),
		  mng( S )
		{}
		void reset( bso::bool__ P )
		{
			mmr::reset( P );
			mng::reset( P );
		}
		_bunch &operator =( const _bunch &SC )
		{
			mng::operator =( SC );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			mmr::Init();
			mng::Init();
		}
		//f Initialization with 'Seed' containing 'Size' objects.
		void Init(
			const type *Seed,
			sdr::size__ Size )
		{
			this->Init();
			
			StoreAndAdjust( Seed, Size );
		}
		//f Allocate 'Size' objects. Extent is forced to 'Size' when 'Mode' = 'mFitted'.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			Allouer_( Size, Mode );
		}
		//f Allocate 'Size' objects and fill newly created object with 'Object'. Extent is forced to 'Size' when 'Mode' = 'mFitted'.
		void Allocate(
			sdr::size__ Size,
			const type &Object,
			aem::mode__ Mode = aem::m_Default )
		{
			sdr::size__ PreviousSize = mng::Amount();

			Allouer_( Size, Mode );

			if ( Size > PreviousSize )
				mmr::Fill( Object, PreviousSize, Size - PreviousSize );
		}
		void PreAllocate( sdr::size__ Size )
		{
			mmr::Init();

			if ( mng::SetFixed( Size ) )
				mmr::Allocate( Size );
		}
		/*f Store at 'Offset' 'Amount' objects from 'Bunch' beginnig at 'Row'.
		Adjust the size of the bunch. */
		void SetStepped( sdr::size__ Step )
		{
			if ( mng::SetStepped( Step ) )
				mmr::Allocate( Step );
		}
		void StoreAndAdjust_(
			const mmr &Bunch,
			sdr::size__ Amount,
			row Row = 0,
			row Offset = 0 )
		{
			Allocate( Amount + *Row );

			mmr::Store( Bunch, Amount, Row, Offset );
		}
/*		void StoreAndAdjust__(
			const _bunch &Bunch,
			sdr::size__ Amount,
			row Row = 0,
			row Offset = 0 )
		{
			StoreAndAdjust_( (const mmr)Bunch, Amount, Row, Offset );
		}
*/		/*f Store at 'Offset' the content of 'Bunch' from position 'Row' to the end.
		Adjust the size of the bunch. */
		void StoreAndAdjust_(
			const mmr &Bunch,
			row Row,
			row Offset = 0 )
		{
			StoreAndAdjust_( Bunch, Bunch.Amount() - *Offset, Row, Offset );
		}
		//f Store at 'Offset' 'Amount' objects from 'Buffer'.
		void StoreAndAdjust(
			const type *Buffer,
			sdr::size__ Amount,
			row Offset = 0 )
		{
			Allocate( Amount + *Offset );

			mmr::Store( Buffer, Amount, Offset );
		}
		//f Store at 'Offset' objects from 'Buffer'.
		void StoreAndAdjust(
			const type *Buffer,
			row Offset = 0 )
		{
/*			Allocate( sh::SizeOf( Buffer ) + *Offset );

			mmr::Store( Buffer, sh::SizeOf( Buffer ), Offset );
*/
			StoreAndAjust( Buffer, sh::SizeOf( Buffer ), Offset );
		}
		// Remplit tout le conteneur avec 'object'  partir de la position 'offset'.
		void FillWith(
			const type &Object,
			row Offset = 0 )
		{
			mmr::Fill( Object, Offset, this->Amount() - *Offset );
		}
		void Set( void )
		{
			qRFbd();	// Replac par 'FillWith(...)'.
		}
		//f Append 'Amount' object from 'Buffer'. Return the position where the objects are put.
		row Append(
			const type *Buffer,
			sdr::size__ Amount )
		{
			row Position = this->Amount();

			StoreAndAdjust( Buffer, Amount, Position );

			return Position;
		}
		//f Append 'Buffer'. Return the position where the objects are put.
		row Append( const type *Buffer )
		{
			row Position = this->Amount();

			StoreAndAdjust( Buffer, sh::SizeOf( Buffer ), Position );

			return Position;
		}
		//f Append the object 'Object'. Return the position where the object is put.
		row Append( const type &Object )
		{
			return Append( &Object, 1 );
		}
		//f Append 'Amount' 'Object's. Return the position where appended.
		void Append(
			const type &Object,
			sdr::size__ Amount )
		{
			StoreAndAdjust( Object, this->Amount(), Amount );
		}
		//f Append 'Amount' objects at 'Position' from 'Bunch'. Return the position where the object are put.
		row Append(
			const mmr &Bunch,
			sdr::size__ Amount,
			row Offset = 0 )
		{
			row Position = this->Amount();

			StoreAndAdjust_( Bunch, Amount, Position, Offset );

			return Position;
		}
		// Ajoute le contenu de 'Bunch' de la position 'First'  la position 'Last', toute deux incluses.
		row Append(
			const mmr &Bunch,
			row First,
			row Last )
		{
#ifdef BCH_DBG
			if ( *First > *Last )
				qRFwk();
#endif
			return Append( Bunch, *Last - *First + 1, *First );
		}
		//f Append all the objects from 'Bunch' beginning at 'Position'. Return the position where the objects are put.
		row Append(
			const _bunch &Bunch,
			row Position = 0 )
		{
			return Append( Bunch, Bunch.Amount() - *Position, Position );
		}
		// Les mthodes 'Add(...)' sont l pour faciliter interchangeabilit avec les object du module 'lstbch'.
		row Add( const type *Buffer )
		{
			return Append( Buffer );
		}
		row Add( const type &Object )
		{
			return Append( Object );
		}
		row Push( const type &Object )
		{
			return Append( Object );
		}
		type Pop( void )
		{
			type Object = mmr::Get( this->Last() );

			Truncate();

			return Object;
		}
		type Top( void ) const
		{
			return mmr::Get( this->Last() );
		}
		//f Remove 'Amount' objects from the end of the bunch.
		void Truncate( sdr::size__ Amount = 1 )
		{
#ifdef BCH_DBG
			if ( Amount > this->Amount() )
				qRFwk();
#endif
			Allouer_( this->Amount() - Amount, aem::m_Default );
		}
		//f Remove objects all objects beginning at 'Row'.
		void Truncate( row Row )
		{
#ifdef BCH_DBG
			if ( !this->Exists( Row ) )
				qRFwk();
#endif
			Truncate ( mng::Amount() - *Row );
		}
		//f Remove all objects but 'Amount()' objects from 'Row'. The size of the bunch is readjusted.
		void Crop(
			sdr::size__ Amount,
			row Row = 0 )
		{
			Truncate( this->Amount() - ( *Row + Amount ) );

			Remove( 0, *Row );
		}
		//f Remove all objects but objects between 'First' and 'Last' included.
		void Crop(
			row First,
			row Last )
		{
#ifdef BCH_DBG
			if ( *Last < *First )
				qRFwk();
#endif
			Crop( *Last - *First + 1, First );
		}
		void InsertAt(
			const mmr &Source,
			sdr::size__ Amount,
			row RowSource,
			row Row = 0 )
		{
			if ( _AppendInsteadOfInsert( Row ) )
				Append( Source, Amount, RowSource );
			else
				InsertAt_( Source, Amount, *RowSource, *Row );
		}
		void InsertAt(
			const _bunch &Bunch,
			row Row = 0)
		{
			InsertAt( Bunch, Bunch.Amount(), 0, Row );
		}
		//f Insert 'Object' at 'Row'
		void InsertAt(
			const type *Source,
			sdr::size__ Amount,
			row Row = 0 )
		{
			if ( _AppendInsteadOfInsert( Row ) )
				Append( Source, Amount );
			else
				InsertAt_( Source, Amount, *Row );
		}
		void InsertAt(
			const type &Object,
			row Row = 0 )
		{
			InsertAt( &Object, 1, Row );
		}
		void InsertAt(
			const type *Source,
			row Row = 0 )
		{
			InsertAt( Source, sh::SizeOf( Source ), Row );
		}
		void InsertAfter(
			const mmr &Source,
			sdr::size__ Amount,
			row RowSource,
			row Row )
		{
			InsertAt( Source, Amount, RowSource, *Row + 1 );
		}
		void InsertAfter(
			const _bunch &Bunch,
			row Row )
		{
			InsertAt( Bunch, *Row + 1 );
		}
		//f Insert 'Object' at 'Row'
		void InsertAfter(
			const type *Source,
			sdr::size__ Amount,
			row Row )
		{
			InsertAt( Source, Amount, *Row + 1 );
		}
		void InsertAfter(
			const type &Object,
			row Row )
		{
			InsertAt( Object, *Row + 1 );
		}
		void InsertAfter(
			const type *Source,
			row Row )
		{
			InsertAt( Source, *Row + 1 );
		}
		//f Remove 'Amount' objects at row 'Row'. The size of the set is reduced.
		void Remove(
			row Row,
			sdr::size__ Amount = 1)
		{
			mmr::Store( *this, this->Amount() - ( Amount + *Row ), Row, *Row + Amount );

			Allouer_( this->Amount() - Amount, aem::m_Default );
		}
		bso::sBool Remove( const type &Object )
		{
			row Row = Search( Object );

			if ( Row != qNIL ) {
				Remove( Row );
				return true;
			} else
				return false;
		}
		//f Return the row of the first of 'Amount' new object.
		row New( sdr::size__ Amount = 1 )
		{
			sdr::row_t__ P = this->Amount();

			Allouer_( P + Amount, aem::m_Default );

			return P;
		}
		//f Return row of 'Object' between 'Begin' (included) and 'End' (excluded), or 'NONE' if not found.
		row Search(
			const type &Object,
			row Begin,
			row End ) const
		{
			return mmr::Search( Object, Begin, End );
		}
		//f Return row of 'Object' beginning at 'Begin' (included), or 'NONE' if not found.
		row Search(
			const type &Object,
			row Begin = 0 ) const
		{
			return Search( Object, Begin, mng::Amount() );
		}
		//f Store 'Count' 'Object' at 'Row'. Adjust the size of the bunch.
		void FillWithAndAdjust(
			const type &Object,
			row Row,
			sdr::size__ Count )
		{
			Allocate( *Row + Count );

			mmr::Fill( Object, Row, Count );
		}
		void SetAndAdjust( void )
		{
			qRFbd();	// Remplac par 'FillWithAndAdjust(...)'.
		}
		//f Return reference to memory.
		mmr &Memory( void )
		{
			return *this;
		}
		//f Return reference to memory.
		const mmr &Memory( void ) const
		{
			return *this;
		}
		const mng &AmountExtentManager( void ) const
		{
			return *this;
		}
		mng &AmountExtentManager( void )
		{
			return *this;
		}
	};

	/*c The core set of static object of type 'type'. Internal use only. */
	template <class type, typename row, typename mng, typename sh> class _bunch_
	: public _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >
	{
	public:
		struct s
		: public _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::s
		{};
		_bunch_( s &S )
		: _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >( S )
		{};
		void reset( bool P = true )
		{
			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::reset( P );
//			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Memory().reset( P );
		}
		_bunch_ &operator =( const _bunch_ &Op )
		{
			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row , sh>::operator =( Op );

			this->Allocate( Op.Amount(), aem::mFitted );

			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Memory().Store( Op, Op.Amount() );

			return *this;
		}
		void OldWriteToFlow(
			flw::oflow__ &OFlow,
			bso::bool__ WriteSize ) const
		{
			if ( WriteSize )
				dtfptb::OldPutSize( mng::Amount(), OFlow );

			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::WriteToFlow( 0, _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Amount(), OFlow );
		}
		void WriteToFlow(
			flw::oflow__ &OFlow,
			bso::bool__ WriteSize ) const
		{
			if ( WriteSize )
				dtfptb::VPut( mng::Amount(), OFlow );

			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::WriteToFlow( 0, _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Amount(), OFlow );
		}
		void OldReadFromFlow(
			flw::iflow__ &IFlow,
			sdr::size__ Amount )
		{
			if ( Amount == 0 )
				Amount = dtfptb::OldGetSize( IFlow );

			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Allocate( Amount );
			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::ReadFromFlow( IFlow, 0, _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Amount() );

		}
		void ReadFromFlow(
			flw::iflow__ &IFlow,
			sdr::size__ Amount )
		{
			if ( Amount == 0 )
				dtfptb::VGet( IFlow, Amount );

			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Allocate( Amount );
			_bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::ReadFromFlow( IFlow, 0, _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Amount() );

		}
		//f Adjust the extent to amount.
		void Adjust( void )
		{
			if ( _bunch<type, tys::E_STORAGEt_( type, row ), mng, row, sh >::Force( mng::Amount() ) )
				tys::E_STORAGEt_( type, row )::Memory().Allocate( mng::Amount() );
		}
	};

	class dummy_size_handler
	{
	public:
		static sdr::size__ SizeOf( void * )
		{
			qRFwk();
			return 0;	// To avoid a warning.
		}
	};

	using tys::sHook;

	/*c A bunch of static object of type 'type'. Use 'E_BUNCH_( type )' rather then directly this class. */
	template <class type, typename row, typename sh=dummy_size_handler> class bunch_
	: public _bunch_<type, row, aem::amount_extent_manager_< row >, sh >
	{
	public:
		struct s
		: public _bunch_<type, row, aem::amount_extent_manager_< row >, sh >::s
		{};
		bunch_( s &S )
		: _bunch_<type, row, aem::amount_extent_manager_< row >, sh >( S )
		{};
		void reset( bso::sBool P = true )
		{
			_bunch_<type, row, aem::amount_extent_manager_< row >, sh >::reset( P );
		}
		void plug( sHook &Hook )
		{
			aem::size__ Amount = Hook.D.Size() / sizeof( type );

			_bunch_<type, row, aem::amount_extent_manager_< row >, sh >::plug( Hook );

			if ( Amount != this->Amount() )
				_bunch_<type, row, aem::amount_extent_manager_< row >, sh >::Allocate( Amount, aem::mFitted );
		}
		void plug( qASd *AS )
		{
			_bunch_<type, row, aem::amount_extent_manager_< row >, sh >::plug( AS );
		}
		bunch_ &GetBunch( void )
		{
			return *this;
		}
		const bunch_ &GetBunch( void ) const
		{
			return *this;
		}
/*		void Init( void )
		{
			_bunch_<type, row, aem::amount_extent_manager_< row >, sh >::Init();
		}
		void Init(
			const type *Seed,
			sdr::size__ Size )
		{
			_bunch_<type, row, aem::amount_extent_manager_< row >, sh >::Init( Seed, Size );
		}
		*/
	};

	E_AUTO3( bunch )

	#define E_BUNCHxt_( Type, r, s )	bunch_< Type, r, s  >
	#define E_BUNCHxt( Type, r,s  )		bunch< Type, r, s >

	#define E_BUNCHt_( Type, r )	E_BUNCHxt_( Type, r, bch::dummy_size_handler )
	#define E_BUNCHt( Type, r )		E_BUNCHxt( Type, r, bch::dummy_size_handler )

	#define E_BUNCHx( Type, s )		E_BUNCHxt( Type, sdr::row__, s )
	#define E_BUNCHx_( Type, s )	E_BUNCHxt_( Type, sdr::row__, s )

	#define E_BUNCH( Type )		E_BUNCHt( Type, sdr::row__ )
	#define E_BUNCH_( Type )	E_BUNCHt_( Type, sdr::row__ )

// #ifndef FLS_COMPILATION_

	using tys::rRH;
	using tys::rHF;
	using tys::rFH;

// #endif

	typedef E_BUNCH_( sdr::row__ ) relations_;
	E_AUTO( relations )

	void _GetRelations(
		const uys::untyped_storage_ &Sorted,
		const uys::untyped_storage_ &Unsorted,
		sdr::size__ Size,
		sdr::row_t__ Limit,
		sdr::byte__ *Buffer,
		E_BUNCH_( sdr::row__ ) &Relations );

	template <typename t> inline void GetRelations(
		const bch::E_BUNCH_( t ) &Sorted,
		const bch::E_BUNCH_( t ) &Unsorted,
		bch::E_BUNCH_( sdr::row__ ) &Relations )
	{
		sdr::byte__ Buffer[sizeof( t )];
#ifdef BCH_DBG
		if ( Sorted.Amount() != Unsorted.Amount() )
			qRFwk();
#endif
		_GetRelations( Sorted, Unsorted, sizeof( t ), Sorted.Amount() * sizeof( t ), Buffer, Relations );
	}



# if 0
	/*c A portable bunch of static object of type 'type'. Use 'E_PBUNCH_( type )' rather then directly this class. */
	template <class type, typename row, typename sh = dummy_size_handler> class p_bunch_
	: public _bunch_<type, row, aem::p_amount_extent_manager_< row >, sh >
	{
	public:
		struct s
		: public _bunch_<type, row, aem::p_amount_extent_manager_< row >, sh >::s
		{};
		p_bunch_( s &S )
		: _bunch_<type, row, aem::p_amount_extent_manager_< row >, sh >( S )
		{};
	};

	E_AUTO3( p_bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_P_BUNCHt_( Type, r )		p_bunch_< Type, r >

	#define E_P_BUNCHt( Type, r )		p_bunch< Type, r >

	#define E_P_BUNCH( Type )		E_P_BUNCHt( Type, sdr::row__ )
	#define E_P_BUNCH_( Type )	E_P_BUNCHt_( Type, sdr::row__ )
# endif

	//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
	template <class t, typename r, typename m, typename s> inline bso::sign__ Compare(
		const _bunch_<t, r, m, s> &S1,
		const _bunch_<t, r, m, s> &S2,
		r BeginS1 = 0,
		r BeginS2 = 0 )
	{
		bso::sign__ Result = bso::Compare( S1.Amount() - *BeginS1, S2.Amount() - *BeginS2 );

		if ( Result == 0 )
			Result = tys::Compare( S1, S2, BeginS1, BeginS2, S1.Amount() - *BeginS1 );

		return Result;
	}


	//f Return 'S1' - 'S2' which respectively begin at 'BeginS1' et 'Begins2' and have a length of 'Amount'.
	template <class t, typename r, typename s> inline bso::sign__ Compare(
		const E_BUNCHxt_( t, r, s ) &S1,
		const E_BUNCHxt_( t, r, s ) &S2,
		r BeginS1,
		r BeginS2,
		sdr::size__ Amount )
	{
		return tys::Compare( S1, S2, BeginS1, BeginS2, Amount );
	}


	template <class t, typename r> inline bso::bool__ operator ==(
		const E_BUNCHt_( t, r ) &S1,
		const E_BUNCHt_( t, r ) &S2 )
	{
		return !Compare( S1, S2 );
	}

	template <class t, typename r> inline bso::bool__ operator !=(
		const E_BUNCHt_( t, r ) &S1,
		const E_BUNCHt_( t, r ) &S2 )
	{
		return Compare( S1, S2 ) != 0;
	}

	//c A set of maximum 'size' static objects of type 'type'. Use 'SET__( type, size )' rather then directly this class.
	template <typename type, int size, typename row, typename aem, typename sh> class _bunch__
	: public _bunch< type, tys::E_STORAGEt__( type, size, row ), aem, row, sh >
	{
	public:
		struct s
		: public _bunch<type, tys::E_STORAGEt__( type, size, row ), aem, row, sh >::s {} S_;
		_bunch__( void ) 
		: _bunch<type, tys::E_STORAGEt__( type, size, row ), aem, row, sh >( S_ ) {}
		_bunch__ &operator =( const _bunch__ &S )
		{
			_bunch<type, tys::E_STORAGEt__( type, size, row ), aem, row, sh >::StoreAndAdjust( S, S.Amount_ );

			return *this;
		}
		void Init( void )
		{
			_bunch<type, tys::E_STORAGEt__( type, size, row ), aem, row, sh >::Init();
//			_bunch<type, tys::E_STORAGEt__( type, size, row ), aem, row >::SetStepValue( 0 );
		}
	};

	template <typename type, int size, typename row, typename sh = dummy_size_handler> class bunch__
	: public _bunch__< type, size, row, aem::amount_extent_manager__<size, row>, sh >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt__( c, i, r )	bunch__<c, i, r>

	#define E_BUNCH__( c, i )		E_BUNCHt__( c, i , sdr::row__ )

	//c A set of static objects of type 'type'. Use 'BUNCH___( type )' rather then directly this class.
	template <typename type, typename row, typename aem, typename sh> class _bunch___
	: public _bunch< type, tys::E_STORAGEt___( type, row ), aem, row, sh >
	{
	public:
		struct s
		: public _bunch<type, tys::E_STORAGEt___( type, row ), aem, row, sh >::s {} S_;
		_bunch___( void ) 
		: _bunch<type, tys::E_STORAGEt___( type, row ), aem, row, sh >( S_ ) {}
		_bunch___ &operator =( const _bunch___ &S )
		{
			_bunch<type, tys::E_STORAGEt___( type, row ), aem, row, sh >::StoreAndAdjust( S, S.Amount_ );
	
			return *this;
		}
		void Init( void )
		{
			_bunch<type, tys::E_STORAGEt___( type, row ), aem, row, sh >::Init();
			if ( _bunch<type, tys::E_STORAGEt___( type, row ), aem, row, sh >::SetStepValue( 0 ) )
				qRFwk();
		}
	};

	template <typename type, typename row, typename sh = dummy_size_handler> class bunch___
	: public _bunch___< type, row, aem::amount_extent_manager___<row>, sh >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt___( c, r )	bunch___<c, r>

	#define E_BUNCH___( c )		E_BUNCHt___( c, sdr::row__ )


# if 0
	template <typename type, int size, typename row, typename sh = dummy_size_handler> class p_bunch__
	: public _bunch__< type, size, row, aem::p_amount_extent_manager__<size, row>, sh >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_P_BUNCHt__( c, i, r )	p_bunch__<c, i, r>

	#define E_P_BUNCH__( c, i )		E_P_BUNCHt__( c, i , sdr::row__ )
# endif

	template <typename row, typename item> inline row Search(
		item Item,
		const bunch_<item, row> &Bunch )
	{
		row Row = Bunch.First();

		while ( ( Row != qNIL ) && ( Bunch( Row ) != Item ) )
			Row = Bunch.Next( Row );

		return Row;
	}
}

# endif

#endif
