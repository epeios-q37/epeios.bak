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

// TYped Storage

#ifndef TYS_INC_
# define TYS_INC_

# define TYS_NAME		"TYS"

# if defined( E_DEBUG ) && !defined( TYS_NODBG )
#  define TYS_DBG
# endif

# include "err.h"
# include "flw.h"
# include "uys.h"
# include "ags.h"
# include "sdr.h"

namespace tys {
	#define TYS_MAX_SIZE	UYS_MAX_SIZE

	template <typename t, typename b, typename r> class _storage_
	: public b
	{
	private:
		// place dans 'Tampon' 'Nomnbre' objets  la position 'Position'
		void _Recall(
			sdr::row_t__ Position,
			sdr::size__ Nombre,
			t *Tampon ) const
		{
			b::Fetch(Position * sizeof( t ), Nombre * sizeof( t ), (sdr::byte__ *)Tampon, qRPDefault);
		}
		// crit 'Taille' objets de 'Tampon'  la position 'Position'
		void _Store(
			const t *Tampon,
			sdr::size__ Nombre,
			sdr::row_t__ Position )
		{
			b::Store( (sdr::byte__ *)Tampon, Nombre * sizeof( t ), Position * sizeof( t ) );
		}
		/* crit 'Nombre' objets de 'Source'  partir de 'Position'
		 la position 'Decalage' */
		void _Store(
			const _storage_ &Source,
			sdr::size__ Quantite,
			sdr::row_t__ Position,
			sdr::row_t__ Decalage )
		{
			b::Store( Source, ( Quantite * sizeof( t ) ),( Position * sizeof( t ) ),  ( Decalage * sizeof( t ) ) );
		}
		// allocation de 'Capacite' objets
		void Allocate_( sdr::size__ Size )
		{
			b::Allocate( ( Size * sizeof( t ) ) );
		}
	public:
		struct s
		: public b::s
		{};
		_storage_( s &S )
		: b( S )
		{}
		void reset( bso::sBool P = true )
		{
			b::reset( P );
		}
		//f Initialization.
		void Init( void )
		{
			b::Init();
		}
		//f Put in 'Buffer' 'Amount' bytes at 'Position'.
		void Recall(
			r Position,
			sdr::size__ Amount,
			t *Buffer ) const
		{
			_Recall( *Position, Amount, Buffer );
		}
		void Recall(
			sdr::size__ Amount,
			r Position,
			_storage_<t,b,r> &Target )
		{
			Target._Store( *this, Amount, *Position, 0 );
		}
		//f Put in 'Buffer' 'Amount' bytes at 'Position'. Return 'Buffer'.
		t *Get(
			r Position,
			sdr::size__ Amount,
			t *Buffer ) const
		{
			_Recall( *Position, Amount, Buffer );

			return Buffer;
		}
		//f Put in 'Value' the object at 'Position'.
		void Recall(
			r Position,
			t &Value ) const
		{
			_Recall( *Position, 1, &Value );
		}
		//f Return the object at 'Position'.
		const t Get( r Position ) const
		{
			t V;

			_Recall( *Position, 1, &V );

			return V;
		}
		//f Store 'Amount' object in 'Buffer' at 'Position'.
		void Store(
			const t *Buffer,
			sdr::size__ Amount,
			r Position )
		{
			_Store( Buffer, Amount, *Position );
		}
		//f Store 'Value' at 'Position'.
		void Store(
			const t &Valeur,
			r Position )
		{
			_Store( &Valeur, 1, *Position );
		}
		/*f Store 'Amount' objects at 'Position' in 'Source' at 'Offset'. */
		void Store(
			const _storage_<t,b,r> &Source,
			sdr::size__ Amount,
			r Position = 0,
			r Offset = 0 )
		{
			_Store( Source, Amount, *Position, *Offset );
		}
		//f Swap objects at 'Position1' and 'Position2'.
		void Swap(
			r Position1,
			r Position2 )
		{
			sdr::byte__ O[sizeof( t )];

			Recall( Position1, *(t *)O );
			Store( Get( Position2 ), Position1 );
			Store( *(t *) O, Position2 );
		}
		// Remplit  partir de 'Position' avec 'Amount' 'Object'
		void Fill(
			const t &Object,
			r Position,
			sdr::size__ Amount )
		{
			b::Fill( (sdr::byte__ *)&Object, sizeof( t ), *Position * sizeof( t ), Amount );
		}
		//f Return the position from 'Object' between 'Begin' and 'End' (both included) or 'qNIL' if non-existant.
		r Search(
			const t &Object,
			r Begin,
			r End ) const
		{
			sdr::row_t__ Position;

			if ( ( Position = b::Search( (sdr::byte__ *)&Object, sizeof( t ), *Begin * sizeof( t ), *End * sizeof( t ) ) ) != qNIL )
				Position /= sizeof( t );

			return Position;
		}
		//f Allocate 'Size' objects.
		void Allocate( sdr::size__ Size )
		{
			Allocate_( Size );
		}
		//f Return the object at 'Position'..
		const t operator ()( r Position ) const
		{
			return Get( Position );
		}
		static size_t GetItemSize( void )
		{
			return sizeof( t );
		}
		b &GetStorage( void )
		{
			return *this;
		}
		const b &GetStorage( void ) const
		{
			return *this;
		}
	};

	using uys::sHook;

	template <typename t, typename r> class storage_
	: public _storage_< t, uys::untyped_storage_, r >
	/* NOTA: See 'storage_core about' '::s'. */
	{
	public:
		struct s
		: public _storage_< t, uys::untyped_storage_, r >::s
		{};
		storage_( s &S )
		:  _storage_< t, uys::untyped_storage_, r >( S )
		{}
		void reset( bool P = true )
		{
			_storage_< t, uys::untyped_storage_, r >::reset( P );
		}
		void Init( void )
		{
			_storage_< t, uys::untyped_storage_, r >::Init();
		}
		/*
		bso::fBool plug( qASd *AS )
		{
			return _storage_< t, uys::untyped_storage_, r >::plug( AS );
		}
		bso::fBool plug( cHook &Hook )
		{
			return _storage_< t, uys::untyped_storage_, r >::plug( Hook );
		}
		*/
		void WriteToFlow(
			r Position,
			sdr::size__ Quantity,
			flw::oflow__ &OFlow ) const
		{
			_storage_<t, uys::untyped_storage_, r >::WriteToFlow( *Position * sizeof( t ), Quantity * sizeof( t ) , OFlow );
		}
		void ReadFromFlow(
			flw::iflow__  &IFlow,
			r Position,
			sdr::size__ Quantite )
		{
			_storage_<t, uys::untyped_storage_, r >::ReadFromFlow( IFlow, *Position * sizeof( t ), Quantite * sizeof( t ) );
		}
	};

	E_AUTO2( storage )

# ifndef FLM_COMPILATION_
	using uys::rRH;
	using uys::rFH;
	using uys::rHF;
# endif

	//m 'memory' would be often used, then create a special name.
	#define E_STORAGEt( t, r )	storage< t, r >
	#define E_STORAGEt_( t, r )	storage_< t, r >

	#define E_STORAGE_( t )	E_STORAGEt_( t, sdr::row__ )
	#define E_STORAGE( t )	E_STORAGEt( t, sdr::row__ )

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	template <class t, typename r> inline bso::sign__ Compare(
		const E_STORAGEt_( t, r ) &S1,
		const E_STORAGEt_( t, r ) &S2,
		r BeginS1,
		r BeginS2,
		sdr::size__ Quantity )
	{
		return uys::Compare( S1, S2, *BeginS1 * sizeof( t ), *BeginS2 * sizeof( t ), Quantity * sizeof( t ) );
	}

	/*c A static set of object of 'amount' objects of type 't' of size 'size'.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_STORAGE(t)__', it's easier. */
	template <class t, int amount, int size, typename r> class storage__
	: public _storage_< t, uys::untyped_storage__< amount * size >, r >
	{
	public:
		storage__( typename _storage_<t, uys::untyped_storage__< amount * size >, r >::s &S = *( typename _storage_<t, uys::untyped_storage__< amount * size >, r >::s *) NULL )	// To simplify use in 'BCH'.
		: _storage_<t, uys::untyped_storage__< amount * size >, r >( S )
		{}

	};


	//d A static set of 'amount' object of type 'Type'.
	#define E_STORAGEt__( type, amount, r ) storage__< type, amount, sizeof( type ), r >
	#define E_STORAGE__( type, amount ) storage__< type, amount, sizeof( type ), sdr::row__ >

		/*c A bunch of object of 'amount' objects of type 't' of size 'size' stored in conventional memory.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, typename r> class storage___
	: public _storage_< t, uys::untyped_storage___, r >
	{
	private:
		typename _storage_<t, uys::untyped_storage___, r >::s Static_;
	public:
		storage___( typename _storage_<t, uys::untyped_storage___, r >::s &S = *( typename _storage_<t, uys::untyped_storage___, r >::s *) NULL )	// To simplify use in 'BCH'.
		: _storage_<t, uys::untyped_storage___, r >( S )
		{}
		t *Buffer( void )
		{
			return _storage_< t, uys::untyped_storage___, r >::Buffer();
		}
	};


# define E_STORAGEt___( type, r ) storage___< type, r >
# define E_STORAGE___( type ) storage___< type, sdr::ow__ >
}
#endif
