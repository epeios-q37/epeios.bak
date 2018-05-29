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

//	$Id: bst.h,v 1.5 2012/11/14 16:06:25 csimon Exp $

#ifndef BST__INC
#define BST__INC

#define BST_NAME		"BST"

#define	BST_VERSION	"$Revision: 1.5 $"	

#define BST_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#if defined( E_DEBUG ) && !defined( BST_NODBG )
#define BST_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:25 $

/* End of automatic documentation generation part. */
/*$BEGIN$*/

#error Obolete. Use 'BITMMR' or 'BITSET'.

#include "err.h"
#include "set.h"

/* ATTENTION: si la taille du rceptacle n'est plus de 1 octet, modifier la valeur
des #define ci-dessous. */
#define BST_NB_BITS_RECEPTACLE	8
//#define BST_NB_BITS_RECEPTACLE	( 8 * sizeof( receptacle__ ) )
#define BST_VALEUR_MAX_RECEPTACLE	((receptacle__)-1L)

/***************************************************/
/* CLASSE DE BASE DE GESTION D'UN ENSEMBLE DE BITS */
/***************************************************/

namespace bst {

	// Type du receptacle de bits.
	typedef bso__ubyte		receptacle__;

	// Classes regroupant des fonctions agissant sur un objet de type 't'. Usage interne.
	template <class t> class functions__
	{
	private:
		static receptacle__ Offset_( POSITION__ Position )
		{
			return (int)( Position % BST_NB_BITS_RECEPTACLE );
		}
		// retourne l'offset correpondant  'Position'
		static POSITION__ Indice_( POSITION__ Position )
		{
			return Position / BST_NB_BITS_RECEPTACLE;
		}
		// retourne l'indice correspondant  'Position'
		static bso__ubyte Masque_( POSITION__ Position )
		{
			return 1 << Offset_( Position );
		}
		// retourne le masque correspondant  'Position'
	public:
		static bso__bool Lire(
			POSITION__ Position,
			const t &Table )
		{
			return ( Table.Read( Indice_( Position ) ) & Masque_( Position ) ) != 0;
		}
		// retourne la valeur du bit  la position 'Position' (>=0)
		static void Ecrire(
			bso__bool Valeur,
			POSITION__ Position,
			t &Table )
		{
			Table.Write( ( Table.Read(Indice_( Position )) & ~Masque_( Position ) ) | ( ( Valeur ? 1 << Offset_( Position ) : 0 ) ), Indice_( Position ) );
		}
		// place un bit de valeur 'Valeur'  la position 'Position'
	};

	// N.B.: le contenu du tableau est invers bit  bit
	//c Bits set.
	class bits_set_
	{
	private:
	// fonction
	/*	bso__ubyte Offset_( POSITION__ Position )
		{
			return functions__<ENSEMBLE(receptacle__)>::Offset( Position() );
		}
		// retourne l'offset correpondant  'Position'
		POSITION__ Indice_( POSITION__ Position )
		{
			return functions__<ENSEMBLE(receptacle__)>::Indice( Position() );
		}
		// retourne l'indice correspondant  'Position'
		bso__ubyte Masque_( POSITION__ Position )
		{
			return functions__<ENSEMBLE(receptacle__)>::Masque( Position() );
		}
		// retourne le masque correspondant  'Position'
	*/	bso__bool Lire_( POSITION__ Position ) const
		{
			return functions__<set::SET_(receptacle__)>::Lire( Position, Table );
		}
		// retourne la valeur du bit  la position 'Position' (>=0)
		void Ecrire_(
			bso__bool Valeur,
			POSITION__ Position )
		{
			functions__<set::SET_(receptacle__)>::Ecrire( Valeur, Position, Table );

	//		Table.Ecrire( Indice, (receptacle__)( ( Table.Objet(Indice) & (receptacle__)~( (receptacle__)1 << Offset ) ) | ( !Valeur << Offset ) ) );
		}
		// place un bit de valeur 'Valeur'  la position 'Position'
		void Allouer_( SIZE__ Nombre )
		{
			Table.Allocate( ( Nombre - 1 ) / BST_NB_BITS_RECEPTACLE + 1 );
			S_.Capacite = Nombre;
		}
		// alloue 'Nombre' (>=1) bits
	/*	void Sauver_(
			trf_sauve &Transfert,
			mbs__capacite Nombre )
		{
			Table.Sauver( Transfert );
		}
		// sauve le tableau dans 'Fichier' un fichier de version 'Version'
		void Charger_( trf_charge &Transfert )
		{
			Table.Charger( Transfert );
		}
		// charge le tableau  partir de 'Fichier' qui est de version 'Version'
	*/
	public:
		// Tableau contenant les bits.
		set::SET_(receptacle__) Table;
		struct s
		{
			SIZE__ Capacite;
			// le nombre de bits
			set::SET_(receptacle__)::s Table;
		} &S_;
		bits_set_( s &S )
		: S_( S ),
		Table( S.Table ){}
		void reset( bool P = true )
		{
			Table.reset( P );
			S_.Capacite = 0;
		}
		void plug( mmm_multimemory_ &M )
		{
			Table.plug( M );
		}
		void plug( MEMORY_DRIVER_ &MDriver )
		{
			Table.plug( MDriver );
		}
		bits_set_ &operator =( const bits_set_ &O )
		{
	//		Dup( O );
			Table = O.Table;
			S_.Capacite = O.S_.Capacite;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			S_.Capacite = 0;
			Table.Init();
		}
	//	void Dup( bits_set_ &O );
		//f Return the value at position 'Position'.
		bso__bool Read( POSITION__ Position ) const
		{
			return (int)Lire_( Position );
		}
		//f Return the value at position 'Position'.
		bso__bool operator()( POSITION__ Position ) const
		{
			return Read( Position );
		}
		//f Write 'Value' at 'Position'.
		void Write(
			bso__bool Value,
			POSITION__ Position )
		{
			Ecrire_( Value, Position );
		}
		//f Allocate enough room to contain 'Size' bits.
		void Allocate( SIZE__ Size )
		{
			Allouer_( Size );
			S_.Capacite = Size;
		}
		//f Return the size of the set.
		SIZE__ Size( void ) const
		{
			return S_.Capacite ;
		}
		//f Add 'Value' to the end of the set.
		POSITION__ Add( bso__bool Value )
		{
			Allouer_( S_.Capacite + 1 );

			Write( Value, S_.Capacite - 1 );
			return S_.Capacite - 1;
		}
		//f Return the position of the top of the set,
		POSITION__ Top( void ) const
		{
			if ( S_.Capacite )
				return S_.Capacite - 1;
			else
				return NONE;
		}
		//f Return the position of the bottom of the set.
		POSITION__ Bottom( void ) const
		{
			if ( S_.Capacite )
				return 0;
			else
				return NONE;
		}
		//f Return the position next to 'Current'.
		POSITION__ Up( POSITION__ Current ) const
		{
			if ( ++Current < S_.Capacite )
				return Current;
			else
				return NONE;
		}
		//f Return the position previous to 'Current'.
		POSITION__ Down( POSITION__ Current ) const
		{
			if ( Current )
				return Current - 1 ;
			else
				return NONE;
		}
		//f Return the position of the first of 'Size' new bits.
		POSITION__ New( SIZE__ Size = 1 )
		{
			POSITION__ P = S_.Capacite;

			Allocate( P + Size );

			return P;
		}
	};

	AUTO( bits_set )

	bso__bool ComparerBitABit_(
		bits_set_ &E1,
		bits_set_ &E2,
		SIZE__ Nombre,
		POSITION__ P1,
		POSITION__ P2 );

	//f Compare 'Quantity' bits from 'S1' and 'S2' beginning at 'P1' and 'P2'.
	inline bso__bool Compare(
		bits_set_ &S1,
		bits_set_ &S2,
		SIZE__ Quantity = 0,
		POSITION__ P1 = 0,
		POSITION__ P2 = 0 )
	{
		if ( !Quantity )
			if ( ( S1.Size() - P1 ) < ( S2.Size() - P2 ) )
				Quantity = S1.Size() - P1;
			else
				Quantity = S2.Size() - P2;

		if ( ( P1 % 8 ) == ( P2 % 8 ) )
		{
			POSITION__ P1C, P2C;
			SIZE__ NC;
			bso__bool Resultat = true;


			P1C = ( P1 ? ( P1 - 1 ) / 8 + 1 : 0 );
			P2C = ( P2 ? ( P2 - 1 ) / 8 + 1 : 0 );

			NC = ( Quantity - ( P1C * 8 - P1 ) ) / 8;

			if ( ( P1 * 8 ) != P1C )
				Resultat = ComparerBitABit_( S1, S2,  P1C * 8 - P1, P1, P2 );

			if ( Resultat )
			{
				if ( ( NC * 8 - ( P1C * 8 - P1 ) ) != Quantity )
					Resultat = ComparerBitABit_( S1, S2,
												 Quantity - NC * 8 - ( P1C * 8 - P1 ),
											   ( P1C + NC ) * 8,
											   ( P2C + NC ) * 8 );

				if ( Resultat && NC )
					Resultat = !set::Compare( S1.Table, S2.Table, P1C, P2C, NC );
			}

			return Resultat;
		}
		else
			return ComparerBitABit_( S1, S2, Quantity, P1, P2 );
	}

	inline bso__bool operator ==(
		bits_set_ &S1,
		bits_set_ &S2 )
	{
		if ( S1.Size() != S2.Size() )
			return 0;
		else
			return Compare( S1, S2 );
	}

	txf::text_oflow___ &operator <<(
		txf::text_oflow___ &OStream,
		bits_set_ &Set );

	txf::text_iflow___ &operator >>(
		txf::text_iflow___ &IStream,
		bits_set_ &Set );


	//c A pairs of bits set.
	class bibits_set_
	{
	public:
		//o Les tableau des bits.
		bits_set_ T1, T2;
		struct s
		{
			bits_set_::s T1, T2;
		};
		bibits_set_( s &S )
		: T1( S.T1 ),
		  T2( S.T2 ){}
		void reset( bool P = true )
		{
			T1.reset( P );
			T2.reset( P );
		}
		void plug( mmm_multimemory_ &M )
		{
			T1.plug( M );
			T2.plug( M );
		}
		// Operateur d'affectation.
		bibits_set_ &operator =( const bibits_set_ &O)
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
		//f Write 'Value' tou 'Position'.
		void Write(
			receptacle__ Value,
			POSITION__ Position )
		{
			T1.Write(   Value & 1       , Position );
			T2.Write( ( Value & 2 ) != 0, Position );	// to avoid warning on some compilers.
		}
		//f Return the value at 'Position'.
		receptacle__ Read( POSITION__ Position ) const
		{
			return (int)T1.Read( Position ) | (int)( T2.Read( Position ) << 1 );
		}
		//f Return the value at position 'Position'.
		receptacle__ operator()( POSITION__ Position ) const
		{
			return Read( Position );
		}
		//f Allocate enough room to contain 'Size' pairs of bits.
		void Allocate( SIZE__ Size )
		{
			T1.Allocate( Size );
			T2.Allocate( Size );
		}
		//f Add 'Value'. Return position where added.
		POSITION__ Add( receptacle__ Value )
		{
			T1.Add(   Value & 1 );
			return T2.Add( ( Value & 2 ) != 0 );	// to avoid warning on some compilers.
		}
	};


	AUTO( bibits_set )

	/* Stocke dans 'D' 'O1' & 'O2', tous tant de taille 'Taille'.
	'D' peut tre 'O1' ou 'O2'. Usage interne. */
	void Et_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille );

	/* Stocke dans 'D' 'O1' | 'O2', tous tant de taille 'Taille'.
	'D' peut tre 'O1' ou 'O2'. Usage interne. */
	void Ou_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille );

	/* Stocke dans 'D' ~'O' tou deux de taille 'Taille'. 'O' peut tre 'D'.
	Usage interne. */
	void Non_(
		const receptacle__ *O,              
		receptacle__ *D,
		BSIZE__ Taille );

	/* Stocke dans 'D' la comparaison bit  bit de 'O1' et 'O2', tous 2 de taille 'Taille'.
	Usage interne. */
	void Dff_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille );

	// Retourne le nombre de bits  un dans 'O4.
	BSIZE__ Compter_(
		const receptacle__ *O,
		BSIZE__ Taille );

	POSITION__ Suivant_(
		const receptacle__ *O,
		BSIZE__ Taille,
		bso__bool Valeur,
		POSITION__ Courant );

	POSITION__ Precedent_(
		const receptacle__ *O,
		bso__bool Valeur,
		POSITION__ Courant );


	// N.B.: le contenu du tableau N'EST PAS invers bit  bit
	// classe enrichissant un tableau statique de 't' rceptacels de certaines fonctions ncessaires. Usage interne.
	template <int t> class receptacles__
	{
	private:
		receptacle__ Table_[t];
	public:
		receptacle__ Read( POSITION__ Position ) const
		{
			return Table_[Position];
		}
		void Write(
			receptacle__ Value,
			POSITION__ Position )
		{
			Table_[Position] = Value;
		}
		void RAZ( void )
		{
			memset( Table_, 0, t * sizeof( receptacle__ ) );
		}
		void MAU( void )
		{
			memset( Table_, BST_VALEUR_MAX_RECEPTACLE, t * sizeof( receptacle__ ) );
		}
		BSIZE__ Taille( void ) const
		{
			return t;
		}
		POSITION__ Suivant(
			bso__bool Value,
			POSITION__ Courant ) const
		{
			return BSTSuivant_( Table_, t, Value, Courant );
		}
		POSITION__ Precedent(
			bso__bool Value,
			POSITION__ Courant ) const
		{
			return BSTPrecedent_( Table_, Value, Courant );
		}
	};

	/* Place dans 'D', 'O1' & 'O2'. 'D' peut tre le mme objet que 'O1' ou 'O2'.
	Usage interne. */
	template <class t> inline void Et_(
		const t &O1,
		const t &O2,
		t &D )
	{
		BSTEt_( O1.Table_, O2.Table_, D.Table_, D.Taille() );
	}

	/* Place dans 'D', 'O1' | 'O2'. 'D' peut tre le mme objet que 'O1' ou 'O2'.
	Usage interne. */
	template <class t> inline void Ou_(
		const t &O1,
		const t &O2,
		t &D )
	{
		BSTOu_( O1.Table_, O2.Table_, D.Table_, D.Taille() );
	}

	// Place dans 'D', ~'O'. 'D' peut tre le mme objet que 'O'. Usage interne.
	template <class t> inline void Non_(
		const t &O,
		t &D )
	{
		BSTNon_( O.Table_, D.Table_, D.Taille() );
	}

	/* Place dans 'D', 'O1' | 'O2'. 'D' peut tre le mme objet que 'O1' ou 'O2'.
	Usage interne. */
	template <class t> inline void Dff_(
		const t &O1,
		const t &O2,
		t &D )
	{
		BSTDff_( O1.Table_, O2.Table_, D.Table_, D.Taille() );
	}

	// Retourne le nombre de bits  un dans 'O'.
	template <class t> inline BSIZE__ Compter_( const t &O )
	{
		return BSTCompter_( O.Table_, O.Taille() );
	}


	//c A set of a maximum of 't' bits.
	template <int t> class bits_set__
	{
	private:
		receptacles__<((t - 1)/BST_NB_BITS_RECEPTACLE)+1> Table_;
	public:
		bits_set__( void )
		{
			Reset();
		}
		//f Return the value at position 'Position'.
		bso__bool Read( POSITION__ Position ) const
		{
			return (int)functions__< receptacle__s<((t - 1)/BST_NB_BITS_RECEPTACLE)+1> >::Lire( Position, Table_ );
		}
			//f Return the value at position 'Position'.
		bso__bool operator()( POSITION__ Position )
		{
			return Read( Position );
		}
		//f Write 'Value' to 'Position'.
		void Write(
			bso__bool Value,
			POSITION__ Position )
		{
			functions__< receptacle__s<((t - 1)/BST_NB_BITS_RECEPTACLE)+1> >::Ecrire( Value, Position, Table_ );
		}
		//f Set all the bits to 'Value'.
		void Reset( bso__bool Value = false )
		{
			if ( Value )
				Table_.MAU();
			else
				Table_.RAZ();
		}
		//f Return the position of the next bit after 'Current' at 'Value'.
		POSITION__ Next(
			POSITION__ Current,
			bso__bool Value ) const
		{
			Current = Table_.Suivant( Value, Current );

			if ( Current >= t )
				Current = NONE;

			return Current;
		}
		//f Return the position of the previous bit after 'Current' at 'Value'.
		POSITION__ Previous(
			POSITION__ Courant,
			bso__bool Value ) const
		{
			return Table_.Precedent( Value, Courant );
		}
		//f Return the first bit at 'Value'.
		POSITION__ First( bso__bool Value ) const
		{
			if ( Read( 0 ) == Value )
				return 0;
			else
				return Next( 0, Value );
		}
		//f Return the last bit at 'Value'.
		POSITION__ Last( bso__bool Value ) const
		{
			if ( Lire( t - 1 ) == Value )
				return t - 1;
			else
				return Prev( t - 1, Value );
		}
		//f Return the size.
		SIZE__ Size( void ) const
		{
			return t;
		}
		//f Return the position of the top of the set.
		POSITION__ Top( void ) const
		{
			return t - 1;
		}
		//f Return the position of the bottom of the set.
		POSITION__ Bottom( void )
		{
			return 0;
		}
		//f Return the position of the bit next to 'Current'.
		POSITION__ Up( POSITION__ Current ) const
		{
			if ( ++Current < t )
				return Current;
			else
				return NONE;
		}
		//f Return the position of the bit previous to 'Current'.
		POSITION__ Down( POSITION__ Current ) const
		{
			if ( Current )
				return Current - 1 ;
			else
				return NONE;
		}
	};

	//f 'D' = 'O1' AND 'O2'.
	template <class t> inline void And(
		const t &O1,
		const t &O2,
		t &D )
	{
		BSTEt_( O1.Table_, O2.Table_, D.Table_ );
	}

	//f 'D' = 'O1' OR 'O2'.
	template <class t> inline void Or(
		const t &O1,
		const t &O2,
		t &D )
	{
		BSTOu_( O1.Table_, O2.Table_, D.Table_ );
	}

	//f 'D' = NOT 'O'.
	template <class t> inline void Not(
		const t &O,
		t &D )
	{
		BSTNon_( O.Table_, D.Table_ );
	}

	//f 'D' = 'O1' XOR 'O2'.
	template <class t> inline void XOr(
		const t &O1,
		const t &O2,
		t &D )
	{
		BSTDff_( O1.Table_, O2.Table_, D.Table_ );
	}

	//f Return the count of bit at true in 'O'.
	template <class t> inline BSIZE__ Count( const t &O )
	{
		return BSTCompter_( O.Table_ );
	}
}

/*$END$*/
#endif
