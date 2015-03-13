/*
	Header for the 'tys' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2013-2004 Claude SIMON.

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

//	$Id: tys.h,v 1.3 2013/04/15 10:50:58 csimon Exp $

#ifndef TYS__INC
#define TYS__INC

#define TYS_NAME		"TYS"

#define	TYS_VERSION	"$Revision: 1.3 $"

#define TYS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &TYSTutor;

#if defined( E_DEBUG ) && !defined( TYS_NODBG )
#define TYS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:58 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D TYped Storage 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "uys.h"
#include "ags.h"
#include "sdr.h"

namespace tys {
	#define TYS_MAX_SIZE	UYS_MAX_SIZE

	template <typename t, typename b, typename r> class _storage_
	: public b
	{
	private:
		// place dans 'Tampon' 'Nomnbre' objets à la position 'Position'
		void _Recall(
			sdr::row_t__ Position,
			sdr::size__ Nombre,
			t *Tampon ) const
		{
			b::Recall( Position * sizeof( t ), Nombre * sizeof( t ), (sdr::datum__ *)Tampon );
		}
		// écrit 'Taille' objets de 'Tampon' à la position 'Position'
		void _Store(
			const t *Tampon,
			sdr::size__ Nombre,
			sdr::row_t__ Position )
		{
			b::Store( (sdr::datum__ *)Tampon, Nombre * sizeof( t ), Position * sizeof( t ) );
		}
		/* écrit 'Nombre' objets de 'Source' à partir de 'Position'
		à la position 'Decalage' */
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
			sdr::datum__ V[sizeof( t )];

			_Recall( *Position, 1, (t *)V );

			return *(t *)V;
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
			sdr::datum__ O[sizeof( t )];

			Recall( Position1, *(t *)O );
			Store( Get( Position2 ), Position1 );
			Store( *(t *) O, Position2 );
		}
		// Remplit à partir de 'Position' avec 'Amount' 'Object'
		void Fill(
			const t &Object,
			r Position,
			sdr::size__ Amount )
		{
			b::Fill( (sdr::datum__ *)&Object, sizeof( t ), *Position * sizeof( t ), Amount );
		}
		//f Return the position from 'Object' between 'Begin' and 'End' (both included) or 'E_NIL' if non-existant.
		r Search(
			const t &Object,
			r Begin,
			r End ) const
		{
			sdr::row_t__ Position;

			if ( ( Position = b::Search( (sdr::datum__ *)&Object, sizeof( t ), *Begin * sizeof( t ), *End * sizeof( t ) ) ) != E_NIL )
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
		b &GetUnderlyingStorage( void )
		{
			return *this;
		}
	};

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
		void plug( ags::E_ASTORAGE_ &AS )
		{
			_storage_< t, uys::untyped_storage_, r >::plug( AS );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			_storage_< t, uys::untyped_storage_, r >::plug( SD );
		}
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

#ifndef FLM__COMPILATION
	using uys::files_hook___;

	using uys::hook_filenames___;

	template <typename storage> inline uys::state__ Plug(
		storage &Storage,
		files_hook___ &Hook )
	{
		return uys::Plug( Storage.GetUnderlyingStorage(), Hook );
	}
#endif

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


	//d A static set of 'amount' object of type 'Type'.
	#define E_STORAGEt___( type, r ) storage___< type, r > 
	#define E_STORAGE___( type ) storage___< type, sdr::row__ > 
}
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
