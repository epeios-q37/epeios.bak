/*
	'htp' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'htp' header file ('htp.h').
	Copyright (C) 2002, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: htp.cpp,v 1.13 2010/06/05 16:21:59 csimon Exp $

#define HTP__COMPILATION

#include "htp.h"

class htptutor
: public ttr_tutor
{
public:
	htptutor( void )
	: ttr_tutor( HTP_NAME )
	{
#ifdef HTP_DBG
		Version = HTP_VERSION "\b\bD $";
#else
		Version = HTP_VERSION;
#endif
		Owner = HTP_OWNER;
		Date = "$Date: 2010/06/05 16:21:59 $";
	}
	virtual ~htptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
ERRProlog
	str::string FieldName;
ERRBegin
	FieldName.Init( GetFieldLabel_( Field ) );

	FieldName.Append( ": " );

	GetValue_( RawHeader, FieldName, Value );
ERRErr
ERREnd
ERREpilog
}

static bso::ulong__ GetULong_( 
	const str::string_ &RawHeader,
	field__ Field )
{
	bso::ulong__ Value = 0;
ERRProlog
	str::string SValue;
	epeios::row__ Error = NONE;
ERRBegin
	SValue.Init();

	GetValue_( RawHeader, Field, SValue );

	Value = SValue.ToUL( &Error );

	if ( Error != NONE )
		ERRu();
ERRErr
ERREnd
ERREpilog
	return Value;
}

static void FillField_(
	flw::iflow__ &IFlow,
	header_ &Header )
{
ERRProlog
	str::string RawHeader;
	const char *Label = NULL;
ERRBegin
	RawHeader.Init();

	GetHeader_( IFlow, RawHeader );

	Header.S_.ContentLength = GetULong_( RawHeader, fContentLength );

	GetValue_( RawHeader, fLocation, Header.Location );
	GetValue_( RawHeader, fContentType, Header.ContentType );
ERRErr
ERREnd
ERREpilog
}

static void StripHeader_( flw::iflow__ &IFlow )
{
ERRProlog
	str::string RawHeader;
ERRBegin
	RawHeader.Init();

	GetHeader_( IFlow, RawHeader );
ERRErr
ERREnd
ERREpilog
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


class htppersonnalization
: public htptutor
{
public:
	htppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~htppersonnalization( void )
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

static htppersonnalization Tutor;

ttr_tutor &HTPTutor = Tutor;
