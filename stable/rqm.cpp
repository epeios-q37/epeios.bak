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

#define RQM__COMPILATION

#include "rqm.h"

using namespace rqm;

#include <fstream.h>

#if 0
void rqm::description_::Init(
	const char *Name,
	cast Cast,
	... )
{
	va_list L;
	level__ ArrayLevel = 0;

	this->Name.Init();
	Casts.Init();

	this->Name = Name;

	Casts.Add( Cast );

	va_start( L, Cast );

	while ( ( MultiLevel != 0 ) || ( Cast != cEnd ) ) {
		if ( Cast == cMulti ) {
			if ( MultiLevel == _LEVEL_MAX )
				ERRl();
			MultiLevel++;
		} else if ( Cat == rqm::cEnd )
			MultiLevel--;

		Cast = va_arg( L, rqm::cast );
		Casts.Add( Cast );
	}

#ifdef RQM_DBG
	if ( MultiLevel != 0 )
		ERRC();
#endif

	while ( ( Cast = va_arg( L, rqm::cast ) ) != rqm::cEnd ) {
		if ( Cast == rqm::cMulti ) {
			if ( MultiLevel == RQM_LEVEL_MAX )
				ERRl();
			MultiLevel++;
		} else if ( Cat == rqm::cEnd )
			MultiLevel--;

		Casts.Add( Cast );
	}

#ifdef RQM_DBG
	if ( MultiLevel != 0 )
		ERRC();
#endif

	va_end( L );
}
#endif

void rqm::description_::Init(
	const char *Name,
	const cast *Casts )
{
	int i = 0;
	cast Cast = cInvalid;
	bso__bool Array = false;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		if ( Cast == cArray ) {
			if ( Array )
				ERRu();
			Array = true;
		} else if ( Cast == cEnd )
			Array = false;
		Cast = Casts[i++];
		this->Casts.Add( Cast );
	} while ( Array || ( Cast != cEnd ) );

#ifdef RQM_DBG
	if ( Array )
		ERRc();
#endif

	while ( ( ( Cast = Casts[i++] ) != cEnd ) || Array ) {
		if ( Cast == cArray ) {
			if ( Array )
				ERRu();
			Array = true;
		} else if ( Cast == cEnd )
			Array = false;

		this->Casts.Add( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}

#if 0
POSITION__ rqm::descriptions_::Add(
	const char *Name,
	cast Cast,
	... )
{
	POSITION__ Position = CONTAINER_( description_ )::New();
	va_list L;
	description_ &Description = CONTAINER_( description_ )::operator()( Position );

	Description.Init();
	Description.Name = Name;

	Description.Add( Cast );

	va_start( L, Cast );

	while ( Cast != cEnd ) {
		Cast = va_arg( L, cast );
		Description.Add( Cast );
	}

	while ( ( Cast = va_arg( L, cast ) ) != cEnd )
		Description.Add( Cast );

	va_end( L );

	CONTAINER_( description_ )::Sync();

	return Position;
}

POSITION__ rqm::descriptions_::Position( const char *Name ) const
{
	POSITION__ Position = CONTAINER_( description_ )::First();
qRH
	CITEM( description_ ) Description;
qRB

	Description.Init( *this );

	while( ( Position != NONE ) && ( Description( Position ).Name != str_string( Name ) ) )
		Position = CONTAINER_( description_ )::Next( Position );
qRR
qRT
qRE
	return Position;
}
#endif

POSITION__ rqm::descriptions_::Position( const description_ &Description ) const
{
	POSITION__ Position = CONTAINER_( description_ )::First();
qRH
	CITEM( description_ ) D;
qRB

	D.Init( *this );

	while( ( Position != NONE )
		    && ( ( D( Position ).Name != Description.Name )
		         || ( D( Position ).Casts != Description.Casts ) ) )
		Position = CONTAINER_( description_ )::Next( Position );
qRR
qRT
qRE
	return Position;
}

// Return the id of the cast 'Cast', or 'cInvalid' if non-existent.
rqm::cast rqm::IdCaste( const str_string_ &Caste )
{
	cast Retour = cEnd;
qRH
	int Id = c_amount;
	str_string S;
qRB
	S.Init();

	while( --Id && ( S = CastsNames[Id], S != Caste ) );

	Retour = (cast)Id;
qRR
qRT
qRE
	return Retour;
}

// Ajoute dans 'Flot' le paramtre 'Valeur' de caste 'Caste'.
void rqm::AddValue(
	cast Cast,
	const void *Valeur,
	flw::oflow___ &Flot )
{
	rqm::AddCast( Cast, Flot );

	switch( Cast ) {
	case cObject:
		Flot.Put( Valeur, sizeof( object__ ) );
		break;
	case cType:
		Flot.Put( Valeur, sizeof( type__ ) );
		break;
	case cCasts:
	{
		const SET_( cast__ ) &Casts = *( const SET_( cast__ ) * )Valeur;
		POSITION__ Position = Casts.First();

		while ( Position  != NONE )
		{
			flw::Put( Casts( Position ), Flot );
			Position = Casts.Next( Position );
		}

		Flot.Put( 0 );
		break;
	}
	case cCommand:
		Flot.Put( Valeur, sizeof( command__ ) );
		break;
	case cBoolean:
		Flot.Put( Valeur, sizeof( bso__bool ) );
		break;
	case cId8:
		Flot.Put( Valeur, sizeof( bso__ubyte ) );
		break;
	case cId16:
		Flot.Put( Valeur, sizeof( bso__ushort ) );
		break;
	case cId32:
		Flot.Put( Valeur, sizeof( bso__ulong ) );
		break;
	case cByte:
		Flot.Put( Valeur, sizeof( bso__raw ) );
		break;
	case cBinary:
	{
		const SET_( bso__raw ) &D = *(const SET_( bso__raw ) *)Valeur;
		SIZE__ Nombre = D.Amount();

		if ( Nombre >= 255 )
		{
			Flot.Put( 255U );
			Nombre -= 255;

			if ( Nombre >= 65535 )
			{
				Flot.Put( 255U );
				Flot.Put( 255U );
				Nombre -= 65535;

				if ( Nombre >= 16777215 )
				{
					Flot.Put( 255U );
					Flot.Put( 255U );
					Flot.Put( 255U );

					Nombre -= 16777215;

					Flot.Put( ( (flw::data__)( Nombre & ( 255 << 24 ) ) >> 24 ) );
				}
				Flot.Put( (flw::data__)( ( Nombre & ( 255 << 16 ) ) >> 16 ) );
			}
			Flot.Put( (flw::data__)( ( Nombre & ( 255 << 8 ) ) >> 8 ) );
		}
		Flot.Put( (flw::data__)( Nombre & 255 ) );

		while( Nombre < D.Amount() )
			Flot.Put( D( Nombre++ ) );

		break;
	}
	case cChar:
		Flot.Put( Valeur, sizeof( bso__char ) );
		break;
	case cString:
	{
		const str_string_ &S =*(const str_string_ *)Valeur;
		POSITION__ Position = 0;

		while ( Position < S.Amount() )
			Flot.Put( S( Position++ ) );

		Flot.Put( 0 );
		break;
	}
	case cPointer:
		Flot.Put( &Valeur, sizeof( void * ) );
		break;
	default:
		ERRu();	//r Case inconnue.
		break;
	}
}

void rqm::request_manager___::GetValue_(
	  cast Cast,
	  void *Valeur )
{
	switch ( Cast ) {
	case cObject:
	{
		Channel_->Get( sizeof( object__ ), Valeur );
		break;
	}
	case cType:
	{
		Channel_->Get( sizeof( type__ ), Valeur );
		break;
	}
	case cCasts:
	{
		cast__ Cast = cInvalid;
		SET_( cast__ ) &Casts = *( SET_( cast__ ) *)Valeur;
		bso__bool Fin = false;
		bso__bool Array = false;

		flw::Get( *Channel_, Cast );

		do
		{
			if ( Cast == cArray ) {
				if ( Array )
					ERRc();
				else
					Array = true;
			}
					
		
			if ( Cast == cEnd )
				if ( Array )
					Array = false;
				else
					Fin = true;

			Casts.Add( Cast );
			flw::Get( *Channel_, Cast );
		}
		while( ( Cast != cEnd ) || !Fin );

		break;
	}
	case cCommand:
	{
		Channel_->Get( sizeof( command__ ), Valeur );
		break;
	}
	case cBoolean:
	{
		Channel_->Get( sizeof( bso__bool ), Valeur );
		break;
	}
	case cId8:
	{
		Channel_->Get( sizeof( bso__ubyte ), Valeur );
		break;
	}
	case cId16:
	{
		Channel_->Get( sizeof( bso__ushort ), Valeur );
		break;
	}
	case cId32:
	{
		Channel_->Get( sizeof( bso__ulong ), Valeur );
		break;
	}
	case cByte:
	{
		Channel_->Get( sizeof( bso__raw ), Valeur );
		break;
	}
	case cBinary:
	{
		SET_( bso__raw ) &Binaire = *(SET_( bso__raw ) *)Valeur;
		bso__ulong Nombre = Channel_->Get();

		if ( Nombre == 255 )
		{
			Nombre += ( Channel_->Get() << 8 ) | Channel_->Get();

			if ( Nombre == 65790 )
			{
				Nombre += ( Channel_->Get() << 16 ) | ( Channel_->Get() << 8 ) | Channel_->Get();

				if ( Nombre == 16843005 )
				{
					Nombre += ( Channel_->Get() << 24 ) | ( Channel_->Get() << 16 ) | ( Channel_->Get() << 8 ) | Channel_->Get();

					if ( Nombre < 16843005 )
						ERRu();
				}
			}
		}

		while( Nombre-- )
			Binaire.Add( Channel_->Get() );

		break;
	}
	case cChar:
	{
		Channel_->Get( sizeof( bso__char ), Valeur );
		break;
	}
	case cString:
	{
			str_string_ &Chaine = *( str_string_ *)Valeur;
			bso__char C;

		while ( C = Channel_->Get() )
			Chaine.Add( C );

		break;
	}
	case cPointer:
	{
		Channel_->Get( sizeof( void * ), Valeur );
		break;
	}
	default:
		ERRu();
		break;
	}
}

/*f Place dans 'Valeur' le contenu du prochain paramtre contenu dans la requte
sachant qu'il est de caste 'Caste'. Un contrle est fait sur la caste.*/
kind rqm::request_manager___::GetValue(
	cast Cast,
	void *Valeur )
{
	kind Kind = kUndefined;

	if ( GetCast_() != Cast )
		ERRu();

	GetValue_( Cast, Valeur );

	if ( Array_ != NONE )
	{
		if ( ( Cast_ = GetCast_() ) == cEnd )
		{
			Array_ = NONE;
			Cast_ = cInvalid;

			Kind = kArrayLast;
		} else 
			Kind = kArray;

	} else
		Kind = kNormal;

	if ( ( Cast_ = GetCast_() ) == cArray )
	{
		Array_ = Position_;
		Cast_ = cInvalid;
	}

	return Kind;
}


struct a_ {
	int i;
};

struct b_
: public a_
{};

struct bv_
: public a_
{
	virtual ~bv_( void ) {}
};

template <class t> static inline void Test_( t &T )
{
	void *P = &T;
	a_ &A = *(a_ *)P;

	if ( &A.i != &T.i )
		ERRt();
}

static inline void Test_( void )
{
qRH
	b_ B;
	bv_ BV;
qRB
	Test_( B );
#if !defined( CPE__VC )
	/* 'VC++' is the only compiler I know which doesn't pass the following test.
	For this compiler, it doesn't matter.
	If another compiler doesn't pass this test, that's very few chances that this
	library doesn't work properly, but it can happen. If so, tell me.
	*/
	Test_( BV );
#endif
qRR
qRT
qRE
}

Q37_GCTOR( rqm )
{
qRFH
qRFB
	/* I assume that the compiler must have a behaviour
	that is not guaranted by the standard. Than I verify
	if the compiler has this behaviour. */
	Test_();
qRFR
	cout << '\t' << ERRMessage() << '\a' << endl;
	exit( EXIT_FAILURE );	// Due to the fact that 'main()'.isn't yet called.
qRFT
qRFE
}	
