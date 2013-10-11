/*
  'bst' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Requires the 'bst' header file ('bst.h').
  Copyright (C) 2000,2001 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: bst.cpp,v 1.5 2012/11/14 16:06:25 csimon Exp $

#define BST__COMPILATION

#include "bst.h"

class bsttutor
: public ttr_tutor
{
public:
	bsttutor( void )
	: ttr_tutor( BST_NAME )
	{
#ifdef BST_DBG
		Version = BST_VERSION "\b\bD $";
#else
		Version = BST_VERSION;
#endif
		Owner = BST_OWNER;
		Date = "$Date: 2012/11/14 16:06:25 $";
	}
	virtual ~bsttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
	/* Stocke dans 'D' 'O1' & 'O2', tous étant de taille 'Taille'.
	'D' peut être 'O1' ou 'O2'. Usage interne. */
	void Et_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = O1[Taille] & O2[Taille];
	}

	/* Stocke dans 'D' 'O1' | 'O2', tous étant de taille 'Taille'.
	'D' peut être 'O1' ou 'O2'. Usage interne. */
	void Ou_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = O1[Taille] | O2[Taille];
	}


	/* Stocke dans 'D' ~'O' tou deux de taille 'Taille'. 'O' peut être 'D'.
	Usage interne. */
	void Non_(
		const receptacle__ *O,
		receptacle__ *D,
		BSIZE__ Taille )
	{
		while( Taille-- )
			D[Taille] = ~O[Taille];
	}

	/* Stocke dans 'D' 'O1' ^ 'O2', tous étant de taille 'Taille'.
	'D' peut être 'O1' ou 'O2'. Usage interne. */
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


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class bstpersonnalization
: public bsttutor
{
public:
	bstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static bstpersonnalization Tutor;

ttr_tutor &BSTTutor = Tutor;
