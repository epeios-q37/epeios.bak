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

#ifndef BITBCH__INC
# define BITBCH__INC

# define BITBCH_NAME		"BITBCH"

# if defined( E_DEBUG ) && !defined( BITBCH_NODBG )
#  define BITBCH_DBG
# endif

// BInary digiT BunCH

# include "err.h"
# include "flw.h"
# include "tys.h"
# include "aem.h"

/* ATTENTION: si la taille du rceptacle n'est plus de 1 octet, modifier la valeur
des #define ci-dessous. */
# define BITBCH__RECEPTACLE_SIZE_IN_BYTES	sizeof( bitbch::receptacle__ )

# define BITBCH__RECEPTACLE_SIZE_IN_BITS		( 8 * BITBCH__RECEPTACLE_SIZE_IN_BYTES )
# define BITBCH__RECEPTACLE_VALUE_MAX	((bitbch::receptacle__)-1L)


namespace bitbch {
	
	using aem::amount_extent_manager_;
	using sdr::row_t__;

	// Type du receptacle de bits.
	typedef bso::u8__		receptacle__;

	template <int t> class receptacles__
	{
	private:
		receptacle__ Table_[t];
	public:
		receptacle__ Get( row_t__ Position ) const
		{
			return Table_[Position];
		}
		void Store(
			receptacle__ Value,
			row_t__ Position )
		{
			Table_[Position] = Value;
		}
		void RAZ( void )
		{
			memset( Table_, 0, t * sizeof( receptacle__ ) );
		}
		void MAU( void )
		{
			memset( Table_, BITBCH__RECEPTACLE_VALUE_MAX, t * sizeof( receptacle__ ) );
		}
		sdr::size__ Taille( void ) const
		{
			return t;
		}
		row_t__ Suivant(
			bso::bool__ Value,
			row_t__ Courant ) const
		{
			return Suivant_( Table_, t, Value, Courant );
		}
		row_t__ Precedent(
			bso::bool__ Value,
			row_t__ Courant ) const
		{
			return Precedent_( Table_, Value, Courant );
		}
	};


	// Classes regroupant des fonctions agissant sur un objet de type 't'. Usage interne.
	template <class t, typename r> class functions__
	{
	private:
		static receptacle__ Offset_( r Position )
		{
			return (receptacle__)( *Position % BITBCH__RECEPTACLE_SIZE_IN_BITS );
		}
		// retourne l'offset correpondant  'Position'
		static r Indice_( r Position )
		{
			return *Position / BITBCH__RECEPTACLE_SIZE_IN_BITS;
		}
		// retourne l'indice correspondant  'Position'
		static bso::u8__ Masque_( r Position )
		{
			return (bso::u8__)( 1 << Offset_( Position ) );
		}
		// retourne le masque correspondant  'Position'
	public:
		static bso::bool__ Lire(
			r Position,
			const t &Table )
		{
			return ( Table.Get( Indice_( Position ) ) & Masque_( Position ) ) != 0;
		}
		// retourne la valeur du bit  la position 'Position' (>=0)
		static void Ecrire(
			bso::bool__ Valeur,
			r Position,
			t &Table )
		{
			Table.Store( (receptacle__)( ( Table.Get( Indice_( Position ) ) & ~Masque_( Position ) ) | ( ( Valeur ? 1 << Offset_( Position ) : 0 ) ) ), Indice_( Position ) );
		}
		// place un bit de valeur 'Valeur'  la position 'Position'
	};


	//c A set of a maximum of 't' bits.
	template <int t, typename r> class bit_bunch__
	{
	private:
		receptacles__<((t - 1)/BITBCH__RECEPTACLE_SIZE_IN_BITS)+1> Table_;
	public:
		bit_bunch__( void )
		{
			Reset();
		}
		//f Return the value at position 'Position'.
		bso::bool__ Get( r Position ) const
		{
			return (int)functions__< receptacles__<((t - 1)/BITBCH__RECEPTACLE_SIZE_IN_BYTES)+1>, r >::Lire( Position, Table_ );
		}
			//f Return the value at position 'Position'.
		bso::bool__ operator()( r Position )
		{
			return Get( Position );
		}
		//f Store 'Value' to 'Position'.
		void Store(
			bso::bool__ Value,
			r Position )
		{
			functions__< receptacles__<((t - 1)/BITBCH__RECEPTACLE_SIZE_IN_BYTES)+1>, r >::Ecrire( Value, Position, Table_ );
		}
		//f Set all the bits to 'Value'.
		void Reset( bso::bool__ Value = false )
		{
			if ( Value )
				Table_.MAU();
			else
				Table_.RAZ();
		}
		//f Return the position of the next bit after 'Current' at 'Value'.
		r Next(
			r Current,
			bso::bool__ Value ) const
		{
			Current = Table_.Suivant( Value, Current );

			if ( Current >= t )
				Current = qNIL;

			return Current;
		}
		//f Return the position of the previous bit after 'Current' at 'Value'.
		r Previous(
			r Courant,
			bso::bool__ Value ) const
		{
			return Table_.Precedent( Value, Courant );
		}
		//f Return the first bit at 'Value'.
		r First( bso::bool__ Value ) const
		{
			if ( Get( 0 ) == Value )
				return 0;
			else
				return Next( 0, Value );
		}
		//f Return the last bit at 'Value'.
		r Last( bso::bool__ Value ) const
		{
			if ( Get( t - 1 ) == Value )
				return t - 1;
			else
				return Previous( t - 1, Value );
		}
		//f Return the size.
		sdr::size__ Size( void ) const
		{
			return t;
		}
		//f Return the position of the top of the set.
		r Top( void ) const
		{
			return t - 1;
		}
		//f Return the position of the bottom of the set.
		r Bottom( void )
		{
			return 0;
		}
		//f Return the position of the bit next to 'Current'.
		r Up( r Current ) const
		{
			if ( ++Current < t )
				return Current;
			else
				return qNIL;
		}
		//f Return the position of the bit previous to 'Current'.
		r Down( r Current ) const
		{
			if ( Current )
				return Current - 1 ;
			else
				return qNIL;
		}
	};

	using tys::sHook;

	// N.B.: le contenu du tableau est invers bit  bit
	//c Bits set.
	template <typename r> class bit_bunch_
	: public amount_extent_manager_<r>
	{
	private:
		sdr::size__ Convert_( sdr::size__ Amount )
		{
			return Amount ? ( Amount - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BITS + 1 : 0;
		}
		bso::bool__ Lire_( r Position ) const
		{
			return functions__<tys::E_STORAGEt_( receptacle__, r ), r>::Lire( Position, Table );
		}
		// retourne la valeur du bit  la position 'Position' (>=0)
		void Ecrire_(
			bso::bool__ Valeur,
			r Position )
		{
			functions__<tys::E_STORAGEt_( receptacle__, r ), r>::Ecrire( Valeur, Position, Table );

	//		Table.Ecrire( Indice, (receptacle__)( ( Table.Objet(Indice) & (receptacle__)~( (receptacle__)1 << Offset ) ) | ( !Valeur << Offset ) ) );
		}
		// place un bit de valeur 'Valeur'  la position 'Position'
		void Allouer_(
			sdr::size__ Nombre,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( amount_extent_manager_<r>::Handle( Nombre, Mode ) )
				Table.Allocate( Convert_( Nombre ) );
		}
		// alloue 'Nombre' (>=1) bits
	public:
		// Tableau contenant les bits.
		tys::E_STORAGEt_(receptacle__, r) Table;
		struct s
		: public aem::amount_extent_manager_<r>::s
		{
			typename tys::E_STORAGEt_(receptacle__, r )::s Table;
		};
		bit_bunch_( s &S )
		: aem::amount_extent_manager_<r>( S ),
		  Table( S.Table ){}
		void reset( bool P = true )
		{
			Table.reset( P );
			amount_extent_manager_<r>::reset( P );
		}
		void plug( sHook &Hook )
		{
			return Table.plug( Hook );
		}
		void plug( qASd *AS )
		{
			return Table.plug( AS );
		}
		bit_bunch_ &operator =( const bit_bunch_ &O )
		{
			amount_extent_manager_<r>::operator =( O );

			Allocate( O.Amount() );
			Table.Store( O.Table, Convert_( O.Amount() ) );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			amount_extent_manager_<r>::Init();
			Table.Init();
		}
	//	void Dup( bit_bunch &O );
		//f Return the value at position 'Position'.
		bso::bool__ Get( r Position ) const
		{
			return (bso::bool__)Lire_( Position );
		}
		//f Return the value at position 'Position'.
		bso::bool__ operator()( r Position ) const
		{
			return Get( Position );
		}
		//f Store 'Value' at 'Position'.
		void Store(
			bso::bool__ Value,
			r Position )
		{
			Ecrire_( Value, Position );
		}
		//f Allocate enough room to contain 'Size' bits.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			Allouer_( Size, Mode );
		}
		//f Append 'Value' to the end of the set.
		r Append( bso::bool__ Value )
		{
			Allouer_( amount_extent_manager_<r>::Amount() + 1, aem::m_Default );

			Store( Value, amount_extent_manager_<r>::Amount() - 1 );
			return amount_extent_manager_<r>::Amount() - 1;
		}
		//f Return the position of the first of 'Size' new bits.
		r New( sdr::size__ Size = 1 )
		{
			row_t__ P = amount_extent_manager_<r>::Amount();

			Allocate( P + Size );

			return P;
		}
		//f Set all the content to valu.
		void Reset( bso::bool__ Value = false )
		{
			receptacle__ Pattern = 0;

			if ( Value )
				Pattern = -1;

			Table.Fill( Pattern, 0, Convert_( amount_extent_manager_<r>::Amount() ) );
		}
	};

	E_AUTO1( bit_bunch )

	using tys::rRH;

	using tys::rHF;
	using tys::rFH;

# define E_BIT_BUNCHt_( row )	bit_bunch_< row >
# define E_BIT_BUNCHt( row )		bit_bunch< row >

# define E_BIT_BUNCH_	E_BIT_BUNCHt_( sdr::row__ )
# define E_BIT_BUNCH		E_BIT_BUNCHt( sdr::row__ )

	void And(
		const E_BIT_BUNCH_ &O1,
		const E_BIT_BUNCH_ &O2,
		E_BIT_BUNCH_ &R );

	void Or(
		const E_BIT_BUNCH_ &O1,
		const E_BIT_BUNCH_ &O2,
		E_BIT_BUNCH_ &R );

	void XOr(
		const E_BIT_BUNCH_ &O1,
		const E_BIT_BUNCH_ &O2,
		E_BIT_BUNCH_ &R );

	void Not(
		const E_BIT_BUNCH_ &O,
		E_BIT_BUNCH_ &R );

	template <typename r__> inline void And(
		const E_BIT_BUNCHt_(r__) &O1,
		const E_BIT_BUNCHt_(r__) &O2,
		E_BIT_BUNCHt_(r__) &R )
	{
		And( *(const E_BIT_BUNCH_ *)&O1, *(const E_BIT_BUNCH_ *)&O2, *(E_BIT_BUNCH_ *)&R );
	}

	template <typename r__> inline void Or(
		const E_BIT_BUNCHt_(r__) &O1,
		const E_BIT_BUNCHt_(r__) &O2,
		E_BIT_BUNCHt_(r__) &R )
	{
		Or( *(const E_BIT_BUNCH_ *)&O1, *(const E_BIT_BUNCH_ *)&O2, *(E_BIT_BUNCH_ *)&R );
	}

	template <typename r__> inline void XOr(
		const E_BIT_BUNCHt_(r__) &O1,
		const E_BIT_BUNCHt_(r__) &O2,
		E_BIT_BUNCHt_(r__) &R )
	{
		XOr( *(const E_BIT_BUNCH_ *)&O1, *(const E_BIT_BUNCH_ *)&O2, *(E_BIT_BUNCH_ *)&R );
	}

	template <typename r__> inline void Not(
		const E_BIT_BUNCHt_(r__) &O,
		E_BIT_BUNCHt_(r__) &R )
	{
		Not( *(const E_BIT_BUNCH_ *)&O, *(E_BIT_BUNCH_ *)&R );
	}


	//c A pairs of bits set.
	template <typename r> class bibit_bunch_
	{
	public:
		//o Les tableau des bits.
		bit_bunch_<r> T1, T2;
		struct s
		{
			typename bit_bunch_<r>::s T1, T2;
		};
		bibit_bunch_( s &S )
		: T1( S.T1 ),
		  T2( S.T2 ){}
		void reset( bool P = true )
		{
			T1.reset( P );
			T2.reset( P );
		}
		void plug( qASd *AS )
		{
			T1.plug( AS );
			T2.plug( AS );
		}
		// Operateur d'affectation.
		bibit_bunch_ &operator =( const bibit_bunch_ &O)
		{
			T1 = O.T1;
			T2 = O.T2;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			T1.Init();
			T2.Init();
		}
		//f Store 'Value' tou 'Position'.
		void Store(
			receptacle__ Value,
			r Position )
		{
			T1.Store(   Value & 1       , Position );
			T2.Store( ( Value & 2 ) != 0, Position );	// to avoid warning on some compilers.
		}
		//f Return the value at 'Position'.
		receptacle__ Get( r Position ) const
		{
			return (int)T1.Get( Position ) | (int)( T2.Get( Position ) << 1 );
		}
		//f Return the value at position 'Position'.
		receptacle__ operator()( r Position ) const
		{
			return Get( Position );
		}
		//f Allocate enough room to contain 'Size' pairs of bits.
		void Allocate( sdr::size__ Size )
		{
			T1.Allocate( Size );
			T2.Allocate( Size );
		}
		//f Append 'Value'. Return position where put.
		r Append( receptacle__ Value )
		{
			T1.Append(   Value & 1 );
			return T2.Append( ( Value & 2 ) != 0 );	// to avoid warning on some compilers.
		}
	};


	E_AUTO1( bibit_bunch )}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &OStream,
	const bitbch::E_BIT_BUNCH_ &Bunch );

template <typename r__> txf::text_oflow__ &operator <<(
	txf::text_oflow__ &OStream,
	const bitbch::E_BIT_BUNCHt_(r__) &Bunch )
{
	return OStream << *(const bitbch::E_BIT_BUNCH_ *)&Bunch;
}

/***************/
/***** NEW *****/
/***************/

#define qBBUNCHd( row ) bit_bunch_<row>
#define qBBUNCHdl bit_bunch_<sdr::sRow>

#endif
