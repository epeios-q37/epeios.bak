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

#define BST__COMPILATION

#include "bst.h"

namespace bst {

	bso__bool ComparerBitABit_(
		bits_set_ &E1,
		bits_set_ &E2,
		BSIZE__ Nombre,
		POSITION__ P1,
		POSITION__ P2 )
	{
		bso__bool Resultat = true;

		while ( Resultat && Nombre-- )
			Resultat = ( E1.Read( Nombre + P1 ) == E2.Read( Nombre + P2 ) );

		return Resultat;
	}

	/*
	void bst_bits_::Dup( bst_bits_ &O )
	{
		POSITION__ Position = O.S_->Capacite;

		Allouer( Position );

		while( Position-- )
			Ecrire( O.Lire( Position ), Position );
	}
	*/
	/* Stocke dans 'D' 'O1' & 'O2', tous tant de taille 'Taille'.
	'D' peut tre 'O1' ou 'O2'. Usage interne. */
	void Et_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = O1[Taille] & O2[Taille];
	}

	/* Stocke dans 'D' 'O1' | 'O2', tous tant de taille 'Taille'.
	'D' peut tre 'O1' ou 'O2'. Usage interne. */
	void Ou_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = O1[Taille] | O2[Taille];
	}


	/* Stocke dans 'D' ~'O' tou deux de taille 'Taille'. 'O' peut tre 'D'.
	Usage interne. */
	void Non_(
		const receptacle__ *O,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = ~O[Taille];
	}

	/* Stocke dans 'D' 'O1' ^ 'O2', tous tant de taille 'Taille'.
	'D' peut tre 'O1' ou 'O2'. Usage interne. */
	void Dff_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = O1[Taille] ^ O2[Taille];
	}

	BSIZE__ Compter_(
		const receptacle__ *O,
		BSIZE__ Taille )
	{
		BSIZE__ Nombre = 0;

		while ( Taille-- )
			if ( O[Taille] )
				for( int i = 0; i < BST_NB_BITS_RECEPTACLE; i++ )
					if ( O[Taille] & ( 1 << i ) )
						Nombre++;

		return Nombre;
	}


	struct pointeur
	{
		const receptacle__ *&Pointeur;
		receptacle__ Read( POSITION__ Position ) const
		{
			return Pointeur[Position];
		}
		void Write(
			const receptacle__ &,
			POSITION__ )
		{
			ERRu();
		}
		pointeur( const receptacle__ *&P )
		: Pointeur( P ){}
	};


	POSITION__ Suivant_(
		const receptacle__ *O,
		BSIZE__ Taille,
		bso__bool Valeur,
		POSITION__ Courant )
	{
		POSITION__ Limite = Taille * BST_NB_BITS_RECEPTACLE;
		pointeur P( O );

		while( ( ++Courant < Limite )
				 && ( functions__<pointeur>::Lire( Courant, P ) != Valeur ) );


		if ( Courant == Limite )
			return NONE;
		else
			return Courant;
	}

	POSITION__ Precedent_(
		const receptacle__ *O,
		bso__bool Valeur,
		POSITION__ Courant )
	{
		pointeur P( O );

		while( ( functions__<pointeur>::Lire( --Courant, P ) != Valeur ) && Courant );

		if ( !Courant && functions__<pointeur>::Lire( 0, P ) != Valeur )
			return NONE;
		else
			return Courant;
	}

	txf::text_oflow___ &operator <<(
		txf::text_oflow___ &Flot,
		bits_set_ &M )
	{
		for( POSITION__ P = 0; P < M.Size(); P++ )
			if ( M.Read( P ) )
				Flot << '*';
			else
				Flot << '-';

		return Flot;
	}
}
