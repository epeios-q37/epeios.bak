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

#define IDXBTR_COMPILATION_

#include "idxbtr.h"

using namespace idxbtr;

namespace {
	/* structure de description d'un arbre.
	Est utilis lors du rquilibrage d'un arbre. Usage interne. */
	struct desc__
	{
		// Racine de l'arbre. N'a pas de fils droit. L'arbre de fils gauche est complet.
		sdr::row__ Racine;
		// Niveau de l'arbre.
		sdr::size__ Niveau;
	};
}

sdr::row_t__ Balance_(
	E_IBTREE_ &Tree,
	sdr::row_t__ Root )
{
qRH
	sdr::row__ Current, Head, Temp;
	que::E_QUEUE Queue;	
qRB
	Queue.Init();
	Queue.Allocate( Tree.BaseTree.Extent() );

	Current = Tree.First( Root );

	Head = Temp = Current;

	Current = Tree.Next( Temp );

	while ( Current != qNIL )
	{
		Queue.BecomeNext( Current, Temp );

		Temp = Current;

		Current = Tree.Next( Temp );
	}

	Root = *Tree.Fill( Queue, Head );

qRR
qRT
qRE
	return Root;
}

/* Equilibre l'arbre, sachant que l'ordre des lments est donne par
la file 'File' de tte 'Tete' et que l'on doit utiliser la pile 'Pile'. */
sdr::row_t__ idxbtr::Equilibrer_(
	E_IBTREE_ &Tree,
	const que::E_QUEUE_ &File,
	sdr::row_t__ Premier,
	uys::sHook *Pilote )
{
	sdr::row_t__ Racine, &Courant = Premier;
qRH
	stkbch::qBSTACKwl( desc__ ) Pile;
	sdr::size__ Niveau = 0;
	desc__ Sommet;
	bso::bool__ Boucler = true;
qRB

	if ( Pilote != NULL )
		Pile.plug( *Pilote );

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

		if ( Courant != qNIL )
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
				Tree.BaseTree.BecomeRight( Courant, Tree._SearchMostRightNode( Racine, NULL ) );
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
qRR
qRT
qRE
	return Racine;
}

sdr::row_t__ idxbtr::Compare_(
	const E_IBTREE_ &Tree,
	const que::E_QUEUE_ &Queue,
	sdr::row_t__ First )
{
	sdr::row_t__ &Row = First;

	while ( Row != qNIL ) {
		if ( Queue.Next( Row ) != Tree.Next( Row ) )
			return Row;

		Row = *Queue.Next( Row );
	}

	return Row;
}

