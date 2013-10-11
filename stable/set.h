/*
  Header for the 'set' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html) 

  This file is part of the Epeios (http://zeusw.org/epeios/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
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

//	$Id: set.h,v 1.5 2012/11/14 16:06:36 csimon Exp $

#ifndef SET__INC
#define SET__INC

#define SET_NAME		"SET"

#define	SET_VERSION	"$Revision: 1.5 $"	

#define SET_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#include "ttr.h"

extern class ttr_tutor &SETTutor;

#if defined( E_DEBUG ) && !defined( SET_NODBG )
#define SET_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:36 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#error Oboslete. Use library 'BCH' instead.

#include "err.h"
#include "flw.h"
#include "mmm.h"
#include "tym.h"
#include "aem.h"

namespace set {

	//c Set core. Don't use ; for internal use only.
	template <class type, class memory, class manager> class set_core_
	: public memory,
	  public manager
	{
	private:
		/* Pousse (décalge vers la fin de l'ensemble) les objets à la position
		'Position' de 'Quantite' positions */
		void Pousser_(
			POSITION__ Position,
			SIZE__ Quantite )
		{
			Allouer_( Amount() + Quantite, false );
			memory::Write( *this, Amount() - Position - Quantite, Position, Position + Quantite);
		}
		// Insere à 'PosDest' 'Quantite' objets situé à partir de 'PosSource' de 'Source'.
		void Inserer_(
			const set_core_ &Source,
			SIZE__ Quantite,
			POSITION__ PosSource,
			POSITION__ PosDest )
		{
			Pousser_( PosDest, Quantite );
			memory::Write( Source, Quantite, PosSource, PosDest );
		}
		// Insere 'Quantite' objets de 'Objets' à 'Position'.
		void Inserer_(
			const type *Objets,
			SIZE__ Quantite,
			POSITION__ Position,
			bso__bool Adjust )
		{
			Pousser_( Position, Quantite );
			memory::Write( Objets, Quantite, Position );
		}
		// Allocation de la place nécessaire à 'Taille' objets.
		void Allouer_(
			SIZE__ Taille,
			bso__bool Adjust )
		{
			if ( manager::AmountToAllocate( Taille, Adjust ) )
				memory::Allocate( Taille );
		}
		// allocate if the set not big enough.
		void AllocateIfNecessary_( SIZE__ Quantity )
		{
			if ( Quantity > Amount() )
				Allouer_( Quantity, false );
		}
	public:
		struct s
		: public memory::s,
		  public manager::s
		{};
		set_core_( s &S )
		: memory( S ),
		  manager( S )
		{}
		void reset( bso__bool P )
		{
			memory::reset( P );
			manager::reset( P );
		}
		set_core_ &operator =( const set_core_ &SC )
		{
			manager::operator =( SC );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			memory::Init();
			manager::Init();
		}
		//f Allocate 'Size' objects. Extent is forced to 'Size' when 'Adjust' at true.
		void Allocate(
			SIZE__ Size,
			bso__bool Adjust = false)
		{
			Allouer_( Size, Adjust );
		}
		//f Same as 'Write()', but allocate additionnaly memory if needed.
		void WriteAndAdjust(
			const set_core_ &Set,
			SIZE__ Amount,
			POSITION__ Position = 0,
			POSITION__ Offset = 0 )
		{
			AllocateIfNecessary_( Amount + Offset );

			memory::Write( Set, Amount, Position, Offset );
		}
		//f Same as 'Write()', but allocate additionnaly memory if needed.
		void WriteAndAdjust(
			const type *Buffer,
			BSIZE__ Amount,
			POSITION__ Position = 0 )
		{
			AllocateIfNecessary_( Amount + Position );

			memory::Write( Buffer, Amount, Position );
		}
		//f Add the object 'Object'. Return the position where object added.
		POSITION__ Add( const type &Object )
		{
			SIZE__ Amount = this->Amount();

			Allouer_( Amount + 1, false );

			memory::Write( Object , Amount );

			return Amount;
		}
		//f Adding 'Amount' object from 'Buffer'. Return the position of the first object added.
		POSITION__ Add(
			const type *Buffer,
			BSIZE__ Amount )
		{
			POSITION__ Retour = this->Amount();

			Allouer_( Retour + Amount, false );

			memory::Write( Buffer, Amount, Retour );

			return Retour;
		}
		//f Adding 'Amount' object from 'Set'. Return the position of the first object added.
		POSITION__ Add(
			const set_core_ &Set,
			SIZE__ Amount )
		{
			POSITION__ Retour = this->Amount();

			Allocate( Retour + Amount );

			memory::Write( Set, Amount, 0, Retour );

			return Retour;
		}
		//f Remove 'Amount' objects from the end of the set.
		void Truncate( SIZE__ Amount = 1 )
		{
	#ifdef SET_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allocate( this->Amoun() - Amount );
		}
		//f Add 'Set'. Return the position where added.
		POSITION__ Add( const set_core_ &Set )
		{
			return Add( Set, Set.Amount() );
		}
		//f Insert at 'PosDest' 'Amount' objects from 'Source' at 'PosSource'.
		void Insert(
			const set_core_ &Source,
			SIZE__ Amount,
			POSITION__ PosSource,
			POSITION__ PosDest )
		{
			Inserer_( Source, Amount, PosSource, PosDest );
		}
		//f Insert 'Set' at 'Position'.
		void Insert(
			const set_core_ &Set,
			POSITION__ Position )
		{
			Inserer_( Set, Set.Amount(), 0, Position );
		}
		//f Insert 'Object' at 'Position'
		void Insert(
			const type &Object,
			POSITION__ Position )
		{
			Inserer_( &Object, 1, Position );
		}
		//f Insert at 'Position' 'Amount' objects from 'Source'.
		void Insert(
			const type *Source,
			BSIZE__ Amount,
			POSITION__ Position )
		{
			Inserer_( Source, Amount, Position );
		}
		//f Delete 'Amount' objects at position 'Position'. The size of the set is reduced.
		void Delete(
			POSITION__ Position,
			SIZE__ Amount = 1)
		{
			memory::Write( *this, this->Amount() - ( Amount + Position ), Position + Amount, Position );

			Allouer_( this->Amount() - Amount, false );
		}
		//f Return the position of the first of 'Amount' new object.
		POSITION__ New( SIZE__ Amount = 1 )
		{
			POSITION__ P = this->Amount();

			Allocate( P + Amount );

			return P;
		}
		//f Return position of 'Object' between 'Begin' (included) and 'End' (excluded), or 'NONE' if not found.
		POSITION__ Position(
			const type &Object,
			POSITION__ Begin,
			POSITION__ End ) const
		{
			if ( Amount() )
				return memory::Position( Object, Begin, End );
		}
		//f Return position of 'Object' beginning at 'Begin' (included), or 'NONE' if not found.
		POSITION__ Position(
			const type &Object,
			POSITION__ Begin = 0 ) const
		{
			return Position( Object, Begin, Amount() );
		}
		//f Fill at 'Position' with 'Object' 'Count' time.
		void Fill(
			const type &Object,
			SIZE__ Count,
			POSITION__ Position = 0 )
		{
			memory::Fill( Object, Count, Position );
		}
		//f Fill from 'Position' with 'Object'.
		void Fill(
			const type &Object,
			POSITION__ Position = 0 )
		{
			Fill( Object, Position, Amount() );
		}
	};


	/*c A set of static object of type 'type'. Use 'SET_( type )' rather then directly this class. */
	template <class type> class set_
	: public set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>
	{
	public:
		struct s
		: public set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::s
		{};
		set_( s &S )
		: set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>( S )
		{};
		void reset( bool P = true )
		{
			set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::reset( P );
			MEMORY_( type )::reset( P );
		}
		set_ &operator =( const set_ &Op )
		{
			set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::operator =( Op );

			Allocate( Op.Amount() );

			MEMORY_( type )::Write( Op, Op.Amount() );

			return *this;
		}
		void write( flw::oflow___ &OFlow ) const
		{
			flw::Put( Amount(), OFlow );
			MEMORY_( type )::write( 0, set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::Amount(), OFlow );
		}
		void read( flw::iflow___ &IFlow )
		{
			SIZE__ Amount;

			flw::Get( IFlow, Amount );
			set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::Allocate( Amount );
			MEMORY_( type )::read( IFlow, 0, set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::Amount() );

		}
		//f Adjust the extent to amount.
		void Adjust( void )
		{
			if ( set_core_<type, MEMORY_( type ), aem::amount_extent_manager_>::Force( Amount() ) )
				MEMORY_( type )::Allocate( Amount() );
		}
	};

	AUTO1( set )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define SET_( Type )	set_< Type >

	#define SET( Type )		set< Type >


	//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
	template <class t> inline bso__sbyte Compare(
		const SET_( t ) &S1,
		const SET_( t ) &S2,
		POSITION__ BeginS1 = 0,
		POSITION__ BeginS2 = 0 )
	{
		if ( ( S1.Amount() - BeginS1 ) != ( S2.Amount() - BeginS2 ) )
			if ( ( S1.Amount() - BeginS1 ) > ( S2.Amount() - BeginS2 ) )
				return 1;
			else
				return -1;
		else
			return TYMCompare( S1, S2, BeginS1, BeginS2, S1.Amount() - BeginS1 );
	}

	//f Return 'S1' - 'S2' which respectively begin at 'BeginS1' et 'Begins2' and have a length of 'Amount'.
	template <class t> inline bso__sbyte Compare(
		const SET_( t ) &S1,
		const SET_( t ) &S2,
		POSITION__ BeginS1,
		POSITION__ BeginS2,
		SIZE__ Amount )
	{
		return TYMCompare( S1, S2, BeginS1, BeginS2, Amount );
	}


	template <class t> inline bso__bool operator ==(
		const SET_( t ) &S1,
		const SET_( t ) &S2 )
	{
		return !Compare( S1, S2 );
	}

	template <class t> inline bso__bool operator !=(
		const SET_( t ) &S1,
		const SET_( t ) &S2 )
	{
		return Compare( S1, S2 ) != 0;
	}

	//c A set of maximum 'size' static objects of type 'type'. Use 'SET__( type, size )' rather then directly this class.
	template <typename type, int size> class set__
	: public set_core_< type, MEMORY__( type, size ), aem::amount_extent_manager__< size > >
	{
	public:
		struct s
		: public set_core_<type, MEMORY__( type, size ), aem::amount_extent_manager__< size > >::s {} S_;
		set__( void ) 
		: set_core_<type, MEMORY__( type, size ), aem::amount_extent_manager__< size > >( S_ ) {}
		set__ &operator =( const set__ &S )
		{
			set_set_core_<type, MEMORY__( type, size )>::WriteAndAdjust( S, S.Amount_ );
			Size_ = S.Amount_;

			return *this;
		}
		void Init( void )
		{
			set_core_<type, MEMORY__( type, size )>::Init();
			set_core_<type, MEMORY__( type, size )>::SetStepValue( 0 );
		}
	};

	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define SET__( c, i ) set__<c,i>
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
