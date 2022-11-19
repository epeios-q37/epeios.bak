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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ctn.h"
#include "crt.h"

#include "err.h"
#include "cio.h"

#include "str.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

typedef str::string_ datum_;
E_AUTO( datum );

typedef ctn::E_MCONTAINER_( datum_ ) data_;
E_AUTO( data );

typedef ctn::E_CONTAINER_( data_ ) data_cluster_;
E_AUTO( data_cluster );

void Test1( int argc, char *argv[] )
{
qRH
	datum Datum;
	data Data;
	data_cluster DataCluster;
qRB

	Data.Init();

	Datum.Init( "Text A1" );
	Data.Append( Datum );

	Datum.Init( "Text A2" );
	Data.Append( Datum );

	Datum.Init( "Text A3" );
	Data.Append( Datum );

	cio::COut << Data( 0 ) << txf::tab;
	cio::COut << Data( 1 ) << txf::nl << txf::commit;

	DataCluster.Init();

	DataCluster.Append( Data );

	Data.Init();

	Datum.Init( "Text B1" );
	Data.Append( Datum );

	Datum.Init( "Text B2" );
	Data.Append( Datum );

	Datum.Init( "Text B3" );
	Data.Append( Datum );

	DataCluster.Append( Data );

	cio::COut << DataCluster( 1 ).Get( 1 ) << txf::commit << txf::nl;

	Data.Init();

	Datum.Init( "Text C1" );
	Data.Append( Datum );

	Datum.Init( "Text C2" );
	Data.Append( Datum );

	Datum.Init( "Text C3" );
	Data.Append( Datum );

//	DataCluster(1).Flush();	// !!!

	DataCluster.Append( Data );

	cio::COut << DataCluster( 1 ).Get( 1 ) << txf::commit << txf::nl;
qRR
qRT
qRE
}

void Test2( int argc, char *argv[] )
{
qRH
	datum Datum;
	data Data;
qRB
	Datum.Init( "123456" );

	Data.Init();

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	Data.Append( Datum );

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	Data.Append( Datum );

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	Data.Store( Datum, Data.First() );

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	Datum.Init( "12345" );
	Data.Store( Datum, Data.First() );

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	cio::COut << Data( 0 ) << txf::pad;
	cio::COut << Data( 1 ) << txf::nl << txf::commit;
qRR
qRT
qRE
}

void Dump_( const str::dStrings &Strings )
{
	sdr::sRow Row = Strings.First();

	while ( Row != qNIL )
	{
		cio::COut << Strings( Row ) << txf::nl << txf::commit;

		Row = Strings.Next( Row );
	}

	cio::COut << txf::nl << txf::commit;
}

#define A( text ) Strings( Strings.New() ).Init( #text )

void Test3( int argc, char *argv[] )
{
qRH
	str::wStrings Strings;
qRB
	Strings.Init();

	A( 1 );
	A( 2 );
	A( 3 );
	A( 4 );
	A( 5 );

	Dump_( Strings );

	Strings.Remove( 2 );
	Strings.Remove( 1 );

	Dump_( Strings );

	A( 6 );

	Dump_( Strings );

qRR
qRT
qRE
}

class  dObject {
public:
	struct s {
		str::dString::s S1, S2, S3;
	};
	str::dString S1, S2, S3;
	void reset( bso::sBool P = true )
	{
		tol::reset( P, S1, S2, S3 );
	}
	dObject( s &S )
		: S1( S.S1 ),
		  S2( S.S2 ),
		  S3( S.S3 )
	{}
	void plug( qASd *AS )
	{
		S1.plug( AS );
		S2.plug( AS );
		S3.plug( AS );
	}
	dObject &operator =(const dObject &O)
	{
		S1 = O.S1;
		S2 = O.S2;
		S3 = O.S3;

		return *this;
	}
	void Init( void )
	{
		tol::Init( S1, S2, S3 );
	}
};

qW( Object );

void Dump_( const dObject &Object )
{
	cio::COut << Object.S1 << ' ' << Object.S2 << ' ' << Object.S3;
}

typedef ctn::qCONTAINERdl( dObject ) dObjects;
qW( Objects );

void Dump_( const dObjects &Objects )
{
	sdr::sRow Row = Objects.First();

	while ( Row != qNIL ) {
		Dump_( Objects( Row ) );

		cio::COut << txf::nl;

		Row = Objects.Next( Row );
	}

	cio::COut << txf::nl;
}

#define F( n ) Object.S##n.Init( Prefix ); Object.S##n.Append( "."#n )

void Fill_(
	const char *Prefix,
	dObject &Object )
{
	F( 1 );
	F( 2 );
	F( 3 );
}


void Test4( int argc, char *argv[] )
{
qRH
	wObjects Objects;
qRB
	Objects.Init();

	Fill_( "1", Objects( Objects.New() ) );
	Fill_( "2", Objects( Objects.New() ) );

	Dump_( Objects );

	Objects( 0 ).reset();
	Objects.Remove( 0 );
	Fill_( "3", Objects( Objects.New() ) );

	Dump_( Objects );

	Objects( 0 ).reset();
	Objects.Remove( 0 );
	Fill_( "4", Objects( Objects.New() ) );

	Dump_( Objects );
qRR
qRT
qRE
}

#undef A

void Action( void )
{}

int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
qRFB
	cio::Initialize( cio::t_Default );
	COut << "Test of library " << CTN_NAME << ' ' << __DATE__ " " __TIME__ "\n";

	Test1( argc, argv );
	Test2( argc, argv );
	Test3( argc, argv );
	Test4( argc, argv );
qRFR
qRFT
	cio::COut << txf::commit;
	cio::CErr << txf::commit;
qRFE( Action() )
	return ExitValue;
}
