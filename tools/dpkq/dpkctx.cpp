/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "dpkctx.h"

#include "bitbch.h"
#include "dte.h"

using namespace dpkctx;

typedef bitbch::E_BIT_BUNCHt_( rrow__ ) grid_;
E_AUTO( grid );

#define COEFF	3	// 

#define CONTEXT_TAG_NAME				"Context"
#define CONTEXTE_TARGET_ATTRIBUTE_NAME	"target"

#define POOL_TAG_NAME						"Pool"

#define POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME	"CycleAmount"
#define POOL_SESSION_AMOUNT_ATTRIBUTE_NAME	"SessionAmount"
#define POOL_TIMESTAMP_ATTRIBUTE_NAME		"TimeStamp"


#define RECORD_TAG_NAME					"Record"
#define RECORD_ID_ATTRIBUTE_NAME		"Id"

void Dump_(
	const rrows_ &Records,
	xml::writer_ &Writer )
{
	sdr::row__ Row = Records.First();

	while ( Row != qNIL ) {
		Writer.PushTag( RECORD_TAG_NAME );
		xml::PutAttribute( RECORD_ID_ATTRIBUTE_NAME, *Records.Get( Row ), Writer );
		Writer.PopTag();

		Row = Records.Next( Row );
	}
}

// '<Record ...>/<Record>...
//          ^
//						 ^
static rrow__ RetrieveRecordId_( xml::parser___ &Parser )
{
	rrow__ Id = qNIL;
	bso::bool__ Continue = true;
	sdr::row__ Error = qNIL;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() != RECORD_ID_ATTRIBUTE_NAME )
				qRGnr();

			Id = Parser.Value().ToUInt( &Error );

			if ( Error != qNIL )
				qRGnr();
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			qRGnr();
			break;
		}
	}

	if ( Id == qNIL )
		qRGnr();

	return Id;
}


// '<... >...</...>...
//        ^
//				   ^
static void Retrieve_(
	xml::parser___ &Parser,
	rrows_ &Records )
{
	bso::bool__ Continue = true;
	sdr::row__ Error = qNIL;
	rrow__ Row = qNIL;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() != RECORD_TAG_NAME )
				qRGnr();

			Records.Append( RetrieveRecordId_( Parser ) );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			qRGnr();
			break;
		}
	}
}

static void Dump_(
	const pool_ &Pool,
	amount__ SessionAmount,
	amount__ CycleAmount,
	time_t TimeStamp,
	xml::writer_ &Writer )
{
	Writer.PushTag( POOL_TAG_NAME );
	xml::PutAttribute( POOL_SESSION_AMOUNT_ATTRIBUTE_NAME, SessionAmount, Writer );
	xml::PutAttribute( POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME, CycleAmount, Writer );
	xml::PutAttribute( POOL_TIMESTAMP_ATTRIBUTE_NAME, TimeStamp, Writer );
	Dump_( Pool, Writer );
	Writer.PopTag();
}

void dpkctx::Dump(
	const context_ &Context,
	xml::writer_ &Writer )
{
	Dump_( Context.Pool, Context.S_.Session, Context.S_.Cycle, Context.S_.TimeStamp, Writer );
}

static void RetrievePool_(
	xml::parser___ &Parser,
	context_ &Context )
{

	bso::bool__ Continue = true;

	while ( Continue ) {
		switch( Parser.Parse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() != POOL_TAG_NAME )
				qRGnr();

			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == POOL_SESSION_AMOUNT_ATTRIBUTE_NAME )
				Context.S_.Session = Parser.Value().ToUInt();
			else if ( Parser.AttributeName() == POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME )
				Context.S_.Cycle = Parser.Value().ToUInt();
			else if ( Parser.AttributeName() == POOL_TIMESTAMP_ATTRIBUTE_NAME )
				Context.S_.TimeStamp = Parser.Value().ToUInt();
			else
				qRGnr();

			break;
		case xml::tStartTagClosed:
			Retrieve_( Parser, Context.Pool );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		default:
			qRGnr();
			break;
		}
	}
}


void dpkctx::Retrieve(
	xml::parser___ &Parser,
	context_ &Context )
{
	RetrievePool_( Parser, Context );
}

static amount__ Remove_(
	const rrows_ &Records,
	amount__ Amount,
	amount__ TotalAmount,
	grid_ &Grid )
{
	sdr::row__ Row = Records.Last();
	amount__ Counter = 0;

	while ( ( Row != qNIL ) && ( Counter < Amount ) ) {
		if ( *Records( Row ) < TotalAmount ) {
			Grid.Store( false, Records( Row ) );
			Counter++;
		}

		Row = Records.Previous( Row );
	}

	return Counter;
}

static rrow__ Pick_( const grid_ &Grid )
{
	rrow__ Row = qNIL;

	tol::InitializeRandomGenerator();

	do {
		Row = rand() % Grid.Amount();
	} while ( Grid.Get( Row ) == false );

	return Row;
}

static void Add_(
	const rrows_ &Source,
	rrows_ &Target )
{
	sdr::row__ Row = Source.First();
	sdr::row__ Position = qNIL;

	while ( Row != qNIL ) {
		if ( ( Position = ( Target.Search( Source( Row ) ) ) ) != qNIL )
			Target.Remove( Position );

		Target.Append( Source ( Row ) );

		Row = Source.Next( Row );
	}
}

static inline bso::bool__ IsNewSession_(
	time_t TimeStamp,
	bso::uint__ Duration )	// in minutes.
{
	if ( Duration == 0 )
		return false;
	else
		return difftime( time( NULL ), TimeStamp ) > ( Duration * 60 );
}


rrow__ dpkctx::context_::Pick(
	amount__ Amount,
	bso::uint__ Duration )
{
	rrow__ Row = qNIL;
qRH
	grid Grid;
	amount__ ToExclude = 0;	// Amount of last picked records to exclude.
qRB
	Grid.Init();
	Grid.Allocate( Amount );

	Grid.Reset( true );

	if ( ( S_.Session >= Amount ) || IsNewSession_( S_.TimeStamp, Duration ) ) {
		if ( ( S_.Session < Amount ) && ( S_.Session > S_.Cycle ) )
			S_.Cycle = S_.Session;
		S_.Session = 0;
	}

	if ( S_.Cycle >= Amount )
		S_.Cycle = 0;

	ToExclude = Amount / 3;

	ToExclude = ( ToExclude > S_.Session ? ToExclude : S_.Session );

	ToExclude = ( ToExclude > S_.Cycle ? ToExclude : S_.Cycle );

	if ( ( S_.Cycle == 0) && ( S_.Session == 0 ) )
		ToExclude = 0;

	if ( Pool.Amount() != 0 )
		Pool.Remove( Pool.First(), Pool.Amount() - Remove_( Pool, ToExclude, Amount, Grid ) );

	Row = Pick_( Grid );

	Pool.Append( Row );

	S_.Session++;
	S_.Cycle++;

	S_.TimeStamp = time( NULL );
qRR
qRT
qRE
	return Row;
}

