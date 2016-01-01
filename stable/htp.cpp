/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define HTP__COMPILATION

#include "htp.h"

#include "str.h"
#include "stdarg.h"

using namespace htp;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

#define HTTP_SIGNATURE			"HTTP/1.1 "
#define NL	"\r\n"

static bso::bool__ TestHTTPVersion_( flw::iflow__ &IFlow )
{
	int Pos = 0;
	const char *Signature = HTTP_SIGNATURE;

	while( Signature[Pos]
		   && ( IFlow.Get() == Signature[Pos++] ) );

	return !Signature[Pos];
}

static bso::ushort__ GetHTTPResponseCode_( flw::iflow__ &IFlow )
{
	bso::char__ C;
	bso::ushort__ Code = 0;

	while( isdigit( C = IFlow.Get() ) )
		Code = Code * 10 + C - '0';

	return Code;
}

static void GetHeader_(
	flw::iflow__ &IFlow,
	str::string_ &Header )
{
	bso::bool__ Continue = true;
	bso::ubyte__ State = 0;
	bso::char__ C;

	do {
		C = IFlow.Get();

		Header.Append( C );

		if ( C == '\r' )
			if ( State == 2 )
				State = 3;
			else
				State = 1;
		else if ( C == '\n' )
			if ( State == 1 )
				State = 2;
			else if ( State == 3 )
				Continue = false;
			else
				State = 0;
		else
			State = 0;
	} while ( Continue );
}

static inline const char *GetFieldLabel_( field__ Name )
{
	switch ( Name ) {
	case fContentLength:
		return "Content-Length";
		break;
	case fLocation:
		return "Location";
		break;
	case fHost:
		return "Host";
		break;
	case fReferrer:
		return "Referrer";
		break;
	case fAccept:
		return "Accept";
		break;
	case fContentType:
		return "Content-Type";
		break;
	default:
		ERRu();
		return NULL;
		break;
	}
}

static void GetValue_(
	const str::string_ &RawHeader,
	const str::string_ &FieldName,
	str::string_ &Value )
{
	epeios::row__ P = NONE;

	P = *RawHeader.Search( FieldName );

	if ( P != NONE ) {
		P = RawHeader.Next( P, FieldName.Amount() );
		
		while ( ( P != NONE ) && ( RawHeader( P ) != NL[0] ) ) {
			Value.Append( RawHeader( P ) );
			P = RawHeader.Next( P );
		}
		
		if ( P == NONE )
			ERRf();
	}
}

static void GetValue_(
	const str::string_ &RawHeader,
	field__ Field,
	str::string_ &Value )
{
qRH
	str::string FieldName;
qRB
	FieldName.Init( GetFieldLabel_( Field ) );

	FieldName.Append( ": " );

	GetValue_( RawHeader, FieldName, Value );
qRR
qRT
qRE
}

static bso::ulong__ GetULong_( 
	const str::string_ &RawHeader,
	field__ Field )
{
	bso::ulong__ Value = 0;
qRH
	str::string SValue;
	epeios::row__ Error = NONE;
qRB
	SValue.Init();

	GetValue_( RawHeader, Field, SValue );

	Value = SValue.ToUL( &Error );

	if ( Error != NONE )
		ERRu();
qRR
qRT
qRE
	return Value;
}

static void FillField_(
	flw::iflow__ &IFlow,
	header_ &Header )
{
qRH
	str::string RawHeader;
	const char *Label = NULL;
qRB
	RawHeader.Init();

	GetHeader_( IFlow, RawHeader );

	Header.S_.ContentLength = GetULong_( RawHeader, fContentLength );

	GetValue_( RawHeader, fLocation, Header.Location );
	GetValue_( RawHeader, fContentType, Header.ContentType );
qRR
qRT
qRE
}

static void StripHeader_( flw::iflow__ &IFlow )
{
qRH
	str::string RawHeader;
qRB
	RawHeader.Init();

	GetHeader_( IFlow, RawHeader );
qRR
qRT
qRE
}


htp::status__ htp::Parse(
	flw::iflow__ &IFlow,
	header_ &Header )
{
	htp::status__ Status = htp::s_unknow;

	if ( TestHTTPVersion_( IFlow ) ) {
		switch( GetHTTPResponseCode_( IFlow ) ) {
		case 100:
			Status = htp::sContinue;
			StripHeader_( IFlow );
			break;
		case 200:
			Status = htp::sOK;
			FillField_( IFlow, Header );
			break;
		case 302:
			Status = htp::sFound;
			FillField_( IFlow, Header );
			break;
		case 404:
			Status = htp::sNotFound;
			break;
		default:
			Status = htp::sUnhandledCode;
			break;
		}
	} else
		Status = sUnknowProtocol;

	return Status;
}

static inline void Write_(
	field__ Name,
	txf::text_oflow__ &Flow )
{
	Flow << GetFieldLabel_( Name ) << ": ";
}

static inline void Write_(
	const field_ &Field,
	txf::text_oflow__ &Flow )
{
	Write_( Field.GetName(), Flow );
	Flow  << Field.Value << NL;

}

void htp::Post( 
	const str::string_ &URL,
	const fields_ &Fields,
	const str::string_ &Content,
	txf::text_oflow__ &Flow )
{
	epeios::row__ Row = Fields.First();
	ctn::E_CMITEM( field_ ) Field;

	Field.Init( Fields );

	Flow << "POST " << URL << " HTTP/1.1" << NL;

	while ( Row != NONE ) {
		Write_( Field( Row ), Flow );

		Row = Fields.Next( Row );
	}

	Flow << "Content-Length: " << Content.Amount() << NL;
	Flow << NL;

	Flow << Content;
}

