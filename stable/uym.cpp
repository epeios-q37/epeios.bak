/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define UYM__COMPILATION

#include "uym.h"

using namespace uym;

#ifdef UYM_BUFFER_SIZE
#	define BUFFER_SIZE	UYM_BUFFER_SIZE
#else
#	define BUFFER_SIZE	10000
#endif

using namespace mdr;

bso::sign__ uym::Compare(
	const untyped_memory_ &E1,
	const untyped_memory_ &E2,
	row_t__ DebutE1,
	row_t__ DebutE2,
	size__ Nombre )
{
	bso::sign__ Resultat = 0;
	datum__ Tampon[BUFFER_SIZE * 2];

	if ( Nombre )
	{
		size__ Taille = Nombre > BUFFER_SIZE ? BUFFER_SIZE : Nombre;

		datum__ *T1 = Tampon;
		datum__ *T2 = Tampon + BUFFER_SIZE;

		do
		{
			if ( Taille > Nombre )
				Taille = Nombre;

			E1.Recall( DebutE1, Taille, T1 );
			E2.Recall( DebutE2, Taille, T2 );

			DebutE1 += Taille;
			DebutE2 += Taille;
			Nombre -= Taille;

			Resultat = memcmp( T1, T2, Taille );
		}
		while ( ( Resultat == 0 ) && ( Nombre != 0 ) );
	}

	return Resultat;
}

inline void uym::_Copy(
	const class untyped_memory_ &Source,
	row_t__ PosSource,
	class untyped_memory_ &Dest,
	row_t__ PosDest,
	size__ Nombre,
	datum__ *Tampon,
	size__ TailleTampon )
{
	if ( PosSource >= PosDest )
	{
		while( Nombre > TailleTampon )
		{
			Source.Recall( PosSource, TailleTampon, Tampon );
			Dest.Store( Tampon, TailleTampon, PosDest );

			PosSource += TailleTampon;
			PosDest += TailleTampon;
			Nombre -= TailleTampon;
		}

		if ( Nombre )
		{
			Source.Recall( PosSource, Nombre, Tampon );
			Dest.Store( Tampon, Nombre, PosDest );
		}
	}
	else
	{
		PosDest += Nombre;
		PosSource += Nombre;

		while( Nombre > TailleTampon )
		{
			Source.Recall( PosSource - TailleTampon, TailleTampon, Tampon );
			Dest.Store( Tampon, TailleTampon, PosDest - TailleTampon );

			PosSource -= TailleTampon;
			PosDest -= TailleTampon;
			Nombre -= TailleTampon;
		}

		if ( Nombre )
		{
			Source.Recall( PosSource - Nombre, Nombre, Tampon );
			Dest.Store( Tampon, Nombre, PosDest - Nombre );
		}
	}
}


/* Duplique 'Objet' 'Nombre' fois  partir de 'Position'
sachant qu'il est de taille 'Taille'. */
void untyped_memory_::Store(
	const datum__ *Objet,
	size__ Taille,
	row_t__ Position,
	size__ Nombre )
{
	while ( Nombre-- )
	{
		Store( Objet, Taille, Position );
		Position += Taille;
	}
}

row_t__ untyped_memory_::Search(
	const datum__ *Objet,
	size__ Taille,
	row_t__ Debut,
	row_t__ Fin ) const
{
	if ( Taille > BUFFER_SIZE )
		ERRl();

	datum__ Tampon[BUFFER_SIZE];
	bool Trouve = false;
	size__ BufferSize = Taille * ( BUFFER_SIZE / Taille );
	size__ BufferPosition = 0;

	while( !Trouve && ( Debut < Fin ) )
	{
		BufferPosition = 0;

		if ( ( Debut + BufferSize ) > Fin )
			BufferSize = Fin - Debut;

		Recall( Debut, BufferSize, Tampon );

		while( !Trouve && ( BufferPosition < BufferSize ) )
		{
			Trouve = !memcmp( Objet, Tampon + BufferPosition, Taille );
			BufferPosition += Taille;
		}

		Debut += BufferPosition;
	}

	Debut -= Taille;

	if ( Trouve )
		return Debut;
	else
		return NONE;
}


void untyped_memory_::Store(
	const untyped_memory_ &Source,
	size__ Amount,
	row_t__ Position,
	row_t__ Offset )
{
	datum__ Buffer[BUFFER_SIZE];

	_Copy( Source, Offset, *this, Position, Amount, Buffer, BUFFER_SIZE );
}

#define BUFFER_SIZE_MAX		FLW_AMOUNT_MAX

void untyped_memory_::WriteToFlow(
	mdr::row_t__ Position,
	mdr::size__ Quantite,
	flw::oflow__ &OFlow ) const
{
qRH
	datum__ TamponSecurite[BUFFER_SIZE];
	datum__ *Tampon = TamponSecurite;
	size__ Taille = sizeof( TamponSecurite );
qRB
	if ( Quantite > BUFFER_SIZE )
	{
		Taille = ( Quantite > BUFFER_SIZE_MAX ? BUFFER_SIZE_MAX : Quantite );
		Tampon = (datum__ *)malloc( Taille );

		if ( !Tampon )
		{
			Tampon = TamponSecurite;
			Taille = sizeof( TamponSecurite );
		}
	}

	while( Quantite )
	{
		if ( Quantite < Taille )
			Taille = Quantite;

		Recall( Position, Taille, Tampon );

		OFlow.Write( Tampon, (flw::size__)Taille );

		Quantite -= Taille;
		Position += Taille;
	}
qRR
qRT
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		free( Tampon );
qRE
}

void untyped_memory_::ReadFromFlow(
	flw::iflow__ &IFlow,
	mdr::row_t__ Position,
	mdr::size__ Quantite )
{
qRH
	datum__ TamponSecurite[BUFFER_SIZE];
	datum__ *Tampon = TamponSecurite;
	size__ Taille = sizeof( TamponSecurite );
qRB
	if ( Quantite > BUFFER_SIZE )
	{
		Taille = ( Quantite > BUFFER_SIZE_MAX ? BUFFER_SIZE_MAX : Quantite );
		Tampon = (datum__ *)malloc( Taille );

		if ( !Tampon )
		{
			Tampon = TamponSecurite;
			Taille = sizeof( TamponSecurite );
		}
	}

	while( Quantite )
	{
		if ( Quantite < Taille )
			Taille = Quantite;

		IFlow.Read( (flw::size__)Taille, Tampon );

		Store( Tampon, Taille, Position );

		Quantite -= Taille;
		Position += Taille;
	}
qRR
qRT
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		free( Tampon );
qRE
}

void uym::_Fill(
	const datum__ *Object,
	size__ Size,
	size__ Count,
	row_t__ Position,
	datum__ *Data )
{
	while( Count )
		memcpy( Data + Size * Count + Position, Object, Size );
}

#if 0
row__ uym::_Position(
	const datum__ *Objet,
	size__ Size,
	row__ Begin,
	row__ End,
	const datum__ *Data )
{
	while( ( Begin < End ) && ( memcmp( Data + Begin, Objet, Size ) ) )
		Begin += Size;

	if ( Begin > End )
		return NONE;
	else
		return Begin;
}
#endif

Q37_GCTOR( uym )
{
	if ( UYM_STATE_AMOUNT != s_amount )
		ERRc();
}
	
