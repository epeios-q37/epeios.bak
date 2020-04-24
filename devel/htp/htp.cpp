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

#define HTP_COMPILATION_

#include "htp.h"

#include "str.h"
#include "stdarg.h"
#include "stsfsm.h"

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

static bso::sU16 GetHTTPResponseCode_( flw::iflow__ &IFlow )
{
	bso::char__ C;
	bso::sU16 Code = 0;

	while( isdigit( C = IFlow.Get() ) )
		Code = Code * 10 + C - '0';

	return Code;
}

static void GetHeader_(
	flw::iflow__ &IFlow,
	str::string_ &Header )
{
	bso::bool__ Continue = true;
	bso::sU8 State = 0;
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

static inline const char *GetFieldLabel_( eField Name )
{
	switch ( Name ) {
	case fContentLength:
		return "Content-Length";
		break;
	case fTransferEncoding:
		return "Transfer-Encoding";
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
	case fConnection:
		return "Connection";
		break;
	default:
		qRFwk();
		return NULL;
		break;
	}
}

namespace {
	bso::sBool GetValue_(
		const str::string_ &RawHeader,
		const str::string_ &FieldName,
		str::string_ &Value )
	{
		sdr::sRow P = qNIL;

		P = *RawHeader.Search( FieldName );

		if ( P != qNIL ) {
			P = RawHeader.Next( P, FieldName.Amount() );

			while ( ( P != qNIL ) && ( RawHeader( P ) != NL[0] ) ) {
				Value.Append( RawHeader( P ) );
				P = RawHeader.Next( P );
			}

			if ( P == qNIL )
				qRFwk();

			return true;
		} else
			return false;
	}

	bso::sBool GetValue_(
		const str::string_ &RawHeader,
		eField Field,
		str::string_ &Value )
	{
		bso::sBool Exists = false;
	qRH
		str::string FieldName;
	qRB
		FieldName.Init( GetFieldLabel_( Field ) );

		FieldName.Append( ": " );

		Exists = GetValue_( RawHeader, FieldName, Value );
	qRR
	qRT
	qRE
		return Exists;
	}
}

void htp::rHeader::FillField_( flw::iflow__ &Flow )
{
qRH
	str::string
		RawHeader,
		Value;
	const char *Label = NULL;
qRB
	RawHeader.Init();

	GetHeader_( Flow, RawHeader );

	GetValue_( RawHeader, fLocation, Location );
	GetValue_( RawHeader, fContentType, ContentType );

	Value.Init();

	if ( GetValue_( RawHeader, fContentLength, Value ) ) {
		BlocFlow_.Init( Value.ToU32(), Flow );
		Flow_.Init( BlocFlow_ );
	} else if ( GetValue_(RawHeader, fTransferEncoding, Value) ) {
		if (  Value != "chunked" )
			qRFwk();

		ChunkFlow_.Init( Flow );

		Flow_.Init( ChunkFlow_ );
	} else {
		Flow.Dismiss();
		Flow_.Init( Flow.IDriver() );
	}
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

namespace {
	fdr::size__ GetSize_( flw::iflow__ &Flow )
	{
		fdr::size__ Size = 0;
	qRH
		str::wString Raw;
		sdr::sRow Error = qNIL;
	qRB
		Raw.Init();

		while ( !Flow.EndOfFlow() && ( Flow.View() != NL[0] ) )
			Raw.Append( Flow.Get() );

		if ( Flow.EndOfFlow() )
			qRFwk();

		Flow.Skip( 1 );

		if ( Flow.Get() != NL[1] )
			qRFwk();

		Size = Raw.ToU32( &Error, str::b16 );

		if ( Error != qNIL )
			qRFwk();
	qRR
	qRT
	qRE
		return Size;
	}
}

bso::sBool htp::rChunkFlow::GetSize_( void )
{
	if ( Size_ != 0 )
		qRFwk();

	Size_ = ::GetSize_( F_() );

	return Size_ != 0;
}


htp::eStatus htp::rHeader::Parse( flw::iflow__ &IFlow )
{
	htp::eStatus Status = htp::s_Undefined;

	if ( TestHTTPVersion_( IFlow ) ) {
		switch( StatusCode_ = GetHTTPResponseCode_( IFlow ) ) {
		case 100:
			Status = htp::sContinue;
			StripHeader_( IFlow );
			break;
		case 200:
			Status = htp::sOK;
			FillField_( IFlow );
			break;
		case 302:
			Status = htp::sFound;
			FillField_( IFlow );
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
	eField Name,
	txf::text_oflow__ &Flow )
{
	Flow << GetFieldLabel_( Name ) << ": ";
}

static inline void Write_(
	const dField &Field,
	txf::text_oflow__ &Flow )
{
	Write_( Field.GetName(), Flow );
	Flow  << Field.Value << NL;

}

#define C( name )	case m##name : return #name; break

const char *htp::GetLabel( eMethod Method )
{
	switch ( Method ) {
	C( Post );
	C( Get );
	C( Put );
	C( Delete );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

#undef C

namespace {
	stsfsm::wAutomat MethodAutomat_;

	void FillMethodAutomat_( void )
	{
		MethodAutomat_.Init();
		stsfsm::Fill<eMethod>( MethodAutomat_, m_amount, GetLabel );
	}
}

eMethod htp::GetMethod( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, MethodAutomat_, m_Undefined, m_amount );
}

void htp::Send(
	eMethod Method,
	const str::string_ &URL,
	const dFields &Fields,
	const str::string_ &Content,
	txf::text_oflow__ &Flow )
{
qRH
	str::wString MethodString;
	sdr::sRow Row = qNIL;
qRB
	MethodString.Init( GetLabel( Method ) );

	str::ToUpper( MethodString );

	Flow << MethodString << ' ' << URL << " HTTP/1.1" << NL;

	Row = Fields.First();

	while ( Row != qNIL ) {
		Write_( Fields( Row ), Flow );

		Row = Fields.Next( Row );
	}

	Flow << "Content-Length: " << Content.Amount() << NL;
	Flow << NL;

	Flow << Content;
qRR
qRT
qRE
}

qGCTOR( htp )
{
	FillMethodAutomat_();
}

