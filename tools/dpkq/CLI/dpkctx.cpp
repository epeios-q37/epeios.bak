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

typedef bitbch::qBBUNCHd( sRRow ) dGrid;
qW( Grid );

#define COEFF	3	// 

#define CONTEXT_TAG_NAME				"Context"
#define CONTEXTE_TARGET_ATTRIBUTE_NAME	"target"

#define BOXES_TAG_NAME	"Boxes"
#define BOXES_AMOUT_ATTRIBUTE	"Amount"

#define BOX_TAG_NAME	"Box"
#define BOX_RECORDS_AMOUT_ATTRIBUTE	"RecordsAmount"


#define POOL_TAG_NAME						"Pool"

#define POOL_CYCLE_AMOUNT_ATTRIBUTE_NAME	"CycleAmount"
#define POOL_SESSION_AMOUNT_ATTRIBUTE_NAME	"SessionAmount"
#define POOL_TIMESTAMP_ATTRIBUTE_NAME		"TimeStamp"


#define RECORD_TAG_NAME					"Record"
#define RECORD_ID_ATTRIBUTE_NAME		"Id"

void Dump_(
	const dRRows &Records,
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
static sRRow RetrieveRecordId_( xml::parser___ &Parser )
{
	sRRow Id = qNIL;
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
	dRRows &Records )
{
	bso::bool__ Continue = true;
	sdr::row__ Error = qNIL;
	sRRow Row = qNIL;

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
	const dPool &Pool,
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

namespace {
	void Dump_(
		const dBox &Box,
		xml::writer_ &Writer )
	{
		sdr::sRow Row = Box.First();

		Writer.PushTag( BOX_TAG_NAME );
		Writer.PutAttribute( BOX_RECORDS_AMOUT_ATTRIBUTE, Box.Amount() );

		::Dump_( *Box, Writer );

		Writer.PopTag();
	}

	void Dump_(
		const dBoxes &Boxes,
		xml::writer_ &Writer )
	{
		sBRow Row = Boxes.First();

		Writer.PushTag( BOXES_TAG_NAME );
		Writer.PutAttribute( BOXES_AMOUT_ATTRIBUTE, Boxes.Amount() );

		while ( Row != qNIL ) {
			Dump_( Boxes( Row ), Writer );

			Row = Boxes.Next( Row );
		}

		Writer.PopTag();
	}
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
	amount__ BoxesAmount,
	context_ &Context )
{
	Context.AdjustBoxesAmount( BoxesAmount );
	RetrievePool_( Parser, Context );
}

static amount__ Remove_(
	const dRRows &Records,
	amount__ Amount,
	amount__ TotalAmount,
	dGrid &Grid )
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

static sRRow Pick_( const dGrid &Grid )
{
	sRRow Row = qNIL;

	tol::InitializeRandomGenerator();

	do {
		Row = rand() % Grid.Amount();
	} while ( Grid.Get( Row ) == false );

	return Row;
}

static void Add_(
	const dRRows &Source,
	dRRows &Target )
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

void dpkctx::context_::MoveToBox_(
	const dRRows &Records,
	dBox &Box )
{
	sdr::sRow Row = Records.First();

	while ( Row != qNIL ) 
	{
		Box.Append( Records( Row ) );

		Row = Records.Next( Row );
	}
}

void dpkctx::context_::MoveRecordsToBox_(
	sBRow Source,
	sBRow Target )
{
	ctn::qCITEMs( dBox, sBRow ) Box;

	Box.Init( Boxes );

	MoveToBox_( Box( Source ), Boxes( Target ) );

	Box.Flush();

	Boxes( Source ).Init();
}

void dpkctx::context_::BringBackRecordsToBox_( sBRow Target )
{
	sBRow Row = Boxes.Last();

	while ( ( Row != qNIL  ) && ( Row != Target ) )
	{
		MoveRecordsToBox_( Row, Target );
		Row = Boxes.Previous( Row );
	}

	if ( Row == qNIL )
		qRGnr();
}

void dpkctx::context_::AdjustBoxesAmount( amount__ Amount )
{
	switch ( Amount ) {
	case 0:
	case 1:
		Boxes.Init();
		break;
	default:
		if ( Boxes.Amount() > ( Amount - 1 ) )
			BringBackRecordsToBox_( Amount - 2 );

		if ( Boxes.Amount() != ( Amount - 1 ) )
			Boxes.Allocate( Amount - 1 );

		break;
	}
}

sRRow dpkctx::context_::Pick(
	amount__ Amount,
	bso::uint__ Duration )
{
	sRRow Row = qNIL;
qRH
	wGrid Grid;
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

