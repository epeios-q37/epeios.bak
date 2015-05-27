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

#define CGIARG__COMPILATION

#include "cgiarg.h"

using namespace cgiarg;

row__ arguments_::Locate(
	const name_ &Name,
	row__ Begin,
	comparison_method__ Method ) const
{
	row__ &P = Begin;
ERRProlog
	name NameBuffer;
ERRBegin
	NameBuffer.Init();

	if ( Begin == E_NIL )
		P = First();
	else
		P = Next( Begin );

	while( P != E_NIL ) {
		if ( GetName( P, NameBuffer ).Amount() >= Name.Amount() )
			if ( str::Compare( NameBuffer, Name, 0, 0, Name.Amount() ) == 0 )
				if ( Method == cmBegins )
					break;
				else if ( Method == cmStrictly ) {
					if ( NameBuffer.Amount() == Name.Amount() ) 
						break;
				} else
					ERRFwk();

		P = Next( P );
	}
ERRErr
ERREnd
ERREpilog
	return P;
}

row__ arguments_::Locate(
	const name_ &Name,
	const suffix_ &Suffix,
	comparison_method__ Method ) const
{
	row__ P = E_NIL;
ERRProlog
	name NameBuffer;
	suffix SuffixBuffer;
ERRBegin
	NameBuffer.Init();
	SuffixBuffer.Init();

	P = First();

	while( P != E_NIL ) {
		if ( GetName( P, NameBuffer ).Amount() >= Name.Amount() )
			if ( ( str::Compare( NameBuffer, Name, 0, 0, Name.Amount() ) == 0 )
				&& ( str::Compare( GetSuffix( P, SuffixBuffer ), Suffix ) == 0 ) )
				if ( Method == cmBegins )
					break;
				else if ( Method == cmStrictly ) {
					if ( NameBuffer.Amount() == Name.Amount() ) 
						break;
				} else
					ERRFwk();

		P = Next( P );
	}
ERRErr
ERREnd
ERREpilog
	return P;
}

void arguments_::Parse(
	flw::iflow__ &Flow,
	bso::char__ SuffixDelimiter )
{
	char Buffer[1000];
	row__ P;

	if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
		ERRLmt();

	while( strlen( Buffer ) )
	{
		char *SuffixDelimiterPosition = strchr( Buffer, SuffixDelimiter );

		if ( SuffixDelimiterPosition != NULL ) {
			*SuffixDelimiterPosition = 0;
			if ( *++SuffixDelimiterPosition != 0 )
				P = Add( Buffer, SuffixDelimiterPosition, "" );
			else
				P = Add( Buffer );
		} else
			P = Add( Buffer );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRLmt();

		PutValue( str::string( Buffer ), P );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRLmt();
	}
}

#ifndef CPE__MT
const name_ &arguments_::GetName( row__ Position ) const
{
	static name Name;

	Name.Init();

	GetName( Position, Name );

	return Name;
}

const value_ &arguments_::GetValue( row__ Position ) const
{
	static value Value;

	Value.Init();

	GetValue( Position, Value );

	return Value;
}

const suffix_ &arguments_::GetSuffix( row__ Position ) const
{
	static suffix Suffix;

	Suffix.Init();

	GetSuffix( Position, Suffix );

	return Suffix;
}
#endif

bso::bool__ arguments_::GetValue(
	const name_ &Name,
	value_ &Value ) const
{
	row__ P = Locate( Name );

	while( ( P != E_NIL ) && ( GetSuffixPosition( P ) != E_NIL ) )
		P = Locate( Name, P );

	if ( P != E_NIL ) {
		GetValue( P, Value );
		return true;
	} else
		return false;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const cgiarg::arguments_ &A )
{
ERRProlog
	cgiarg::row__ P = E_NIL;
	str::string Buffer;
ERRBegin
	P = A.First();

	while( P != E_NIL )
	{
		Buffer.Init();
		F << A.GetName( P, Buffer );

		if ( A.GetSuffixPosition( P ) != E_NIL ) {
			Buffer.Init();
			F << CGIARG_SUFFIX_DELIMITER << A.GetSuffix( P, Buffer );
		}

		Buffer.Init();
		F << A.GetSuffix( P, Buffer ) << txf::nl;

		P = A.Next( P );
	}
ERRErr
ERREnd
ERREpilog
	return F;
}

