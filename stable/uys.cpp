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

#define UYS__COMPILATION

#include "uys.h"

#include "str.h"

using namespace uys;

#ifdef UYS_BUFFER_SIZE
#	define BUFFER_SIZE	UYS_BUFFER_SIZE
#else
#	define BUFFER_SIZE	10000
#endif

using namespace sdr;

bso::sign__ uys::Compare(
	const untyped_storage_ &E1,
	const untyped_storage_ &E2,
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

inline void uys::_Copy(
	const untyped_storage_ &Source,
	row_t__ PosSource,
	untyped_storage_ &Dest,
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

void untyped_storage_::Fill(
	const datum__ *Object,
	size__ Size,
	row_t__ Position,
	size__ Amount )
{
	while ( Amount-- )
	{
		Store( Object, Size, Position );
		Position += Size;
	}
}

row_t__ untyped_storage_::Search(
	const datum__ *Objet,
	size__ Taille,
	row_t__ Debut,
	row_t__ Fin ) const
{
	if ( Taille > BUFFER_SIZE )
		ERRLmt();

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
		return E_NIL;
}


void untyped_storage_::Store(
	const untyped_storage_ &Source,
	size__ Amount,
	row_t__ Position,
	row_t__ Offset )
{
	datum__ Buffer[BUFFER_SIZE];

	_Copy( Source, Offset, *this, Position, Amount, Buffer, BUFFER_SIZE );
}

#define BUFFER_SIZE_MAX		FLW_AMOUNT_MAX

void untyped_storage_::WriteToFlow(
	sdr::row_t__ Position,
	sdr::size__ Quantite,
	flw::oflow__ &OFlow ) const
{
ERRProlog
	datum__ TamponSecurite[BUFFER_SIZE];
	datum__ *Tampon = TamponSecurite;
	size__ Taille = sizeof( TamponSecurite );
ERRBegin
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
ERRErr
ERREnd
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		free( Tampon );
ERREpilog
}

void untyped_storage_::ReadFromFlow(
	flw::iflow__ &IFlow,
	sdr::row_t__ Position,
	sdr::size__ Quantite )
{
ERRProlog
	datum__ TamponSecurite[BUFFER_SIZE];
	datum__ *Tampon = TamponSecurite;
	size__ Taille = sizeof( TamponSecurite );
ERRBegin
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
ERRErr
ERREnd
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		free( Tampon );
ERREpilog
}

void uys::_Fill(
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
row__ uys::_Position(
	const datum__ *Objet,
	size__ Size,
	row__ Begin,
	row__ End,
	const datum__ *Data )
{
	while( ( Begin < End ) && ( memcmp( Data + Begin, Objet, Size ) ) )
		Begin += Size;

	if ( Begin > End )
		return E_NIL;
	else
		return Begin;
}
#endif

Q37_GCTOR( uys )
{
	if ( UYS_STATE_AMOUNT != s_amount )
		ERRChk();
}
