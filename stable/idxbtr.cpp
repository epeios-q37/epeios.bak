/*
	'idxbtr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'idxbtr' header file ('idxbtr.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: idxbtr.cpp,v 1.56 2013/04/15 10:50:52 csimon Exp $

#define IDXBTR__COMPILATION

#include "idxbtr.h"

class idxbtrtutor
: public ttr_tutor
{
public:
	idxbtrtutor( void )
	: ttr_tutor( IDXBTR_NAME )
	{
#ifdef IDXBTR_DBG
		Version = IDXBTR_VERSION "\b\bD $";
#else
		Version = IDXBTR_VERSION;
#endif
		Owner = IDXBTR_OWNER;
		Date = "$Date: 2013/04/15 10:50:52 $";
	}
	virtual ~idxbtrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace idxbtr {

	/* structure de description d'un arbre.
	Est utilisé lors du rééquilibrage d'un arbre. Usage interne. */
	struct desc__
	{
		// Racine de l'arbre. N'a pas de fils droit. L'arbre de fils gauche est complet.
		sdr::row__ Racine;
		// Niveau de l'arbre.
		sdr::size__ Niveau;
	};

	sdr::row_t__ Balance_(
		E_IBTREE_ &Tree,
		sdr::row_t__ Root )
	{
	ERRProlog
		sdr::row__ Current, Head, Temp;
		que::E_QUEUE Queue;	
	ERRBegin
		Queue.Init();
		Queue.Allocate( Tree.BaseTree.Extent() );

		Current = Tree.First( Root );

		Head = Temp = Current;

		Current = Tree.Next( Temp );

		while ( Current != E_NIL )
		{
			Queue.BecomeNext( Current, Temp );

			Temp = Current;

			Current = Tree.Next( Temp );
		}

		Root = *Tree.Fill( Queue, Head );

	ERRErr
	ERREnd
	ERREpilog
		return Root;
	}

	/* Equilibre l'arbre, sachant que l'ordre des éléments est donnée par
	la file 'File' de tête 'Tete' et que l'on doit utiliser la pile 'Pile'. */
	sdr::row_t__ Equilibrer_(
		E_IBTREE_ &Tree,
		const que::E_QUEUE_ &File,
		sdr::row_t__ Premier,
		sdr::E_SDRIVER__ &Pilote )
	{
		sdr::row_t__ Racine, &Courant = Premier;
	ERRProlog
		stk::E_BSTACK( desc__ ) Pile;
		sdr::size__ Niveau = 0;
		desc__ Sommet;
		bso::bool__ Boucler = true;
	ERRBegin

		if ( &Pilote )
			Pile.plug( Pilote );

		Pile.Init();

		Racine = Courant;

		do
		{
			while( !Pile.IsEmpty() && ( Pile( Pile.Last() ).Niveau == Niveau ) )
			{
				Sommet = Pile.Pop();

				Niveau = Sommet.Niveau + 1;
				Tree.BaseTree.BecomeRight( Racine, Sommet.Racine );

				Racine = *Sommet.Racine;
			}

			Courant = *File.Next( Courant );

			if ( Courant != E_NIL )
			{
				if ( File.HasNext( Courant ) )
				{
					Tree.BaseTree.BecomeLeft( Racine, Courant );

					Sommet.Racine = Courant;
					Sommet.Niveau = Niveau;

					Pile.Push( Sommet );

					Courant = *File.Next( Courant );
				}
				else
				{
					Boucler = false;
					Tree.BaseTree.BecomeRight( Courant, Tree._SearchMostRightNode( Racine, *(btr::level__ *)NULL ) );
				}
			}
			else
				Boucler = false;

			if ( Boucler )
			{
				Racine = Courant;
				Niveau = 0;
			}
		} while( Boucler );

		while( !Pile.IsEmpty() )
		{
			Sommet = Pile.Pop();

			Tree.BaseTree.BecomeRight( Racine, Sommet.Racine );

			Racine = *Sommet.Racine;
		}
	ERRErr
	ERREnd
	ERREpilog
		return Racine;
	}
}

sdr::row_t__ idxbtr::Compare_(
	const E_IBTREE_ &Tree,
	const que::E_QUEUE_ &Queue,
	sdr::row_t__ First )
{
	sdr::row_t__ &Row = First;

	while ( Row != E_NIL ) {
		if ( Queue.Next( Row ) != Tree.Next( Row ) )
			return Row;

		Row = *Queue.Next( Row );
	}

	return Row;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class idxbtrpersonnalization
: public idxbtrtutor
{
public:
	idxbtrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~idxbtrpersonnalization( void )
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

static idxbtrpersonnalization Tutor;

ttr_tutor &IDXBTRTutor = Tutor;
