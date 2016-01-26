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

#define DWTXCL__COMPILATION

#include "dwtxcl.h"

#include "tol.h"

using namespace dwtxcl;

static void Reverse_(
	const str::string_ &Source,
	str::string_ &Reversed )
{
	sdr::row__ Row = Source.Last();

	while ( Row != qNIL ) {
		Reversed.Append( Source( Row ) );

		Row = Source.Previous( Row );
	}
}

static void Add_(
	const str::string_ &Tag,
	int Id,
	_automat_ &Automat )
{
qRH
	str::string Reversed;
	TOL_CBUFFER___ Buffer;
	stsfsm::id__ NewId;
qRB
	Reversed.Init();

	Reverse_( Tag, Reversed );

	NewId = stsfsm::Add( Reversed.Convert( Buffer ), Id, Automat );

	if ( ( NewId != stsfsm::UndefinedId) && ( NewId != Id ) )
		qRFwk();
qRR
qRT
qRE
}

void dwtxcl::excluder_::_AddGhostSuffix( const str::string_ &RawTag )
{
qRH
	str::string Tag;
qRB
	Tag.Init( "." );
	Tag.Append( RawTag );
	Tag.Append( '/' );

	Add_( Tag, sGhost, Automat );
qRR
qRT
qRE
}

void dwtxcl::excluder_::AddExtension( const str::string_ &RawExtension )
{
qRH
	str::string Extension;
qRB
	Extension.Init();

	if ( RawExtension.Amount() != 0 )
		if ( RawExtension( RawExtension.First() ) != '.' )
			Extension.Append( '.' );

	Extension.Append( RawExtension );

	Add_( Extension, sExcludedFile, Automat );
qRR
qRT
qRE
}

void dwtxcl::excluder_::AddDirectory( const str::string_ &RawDirectory )
{
qRH
	str::string Directory;
qRB
	Directory.Init();

	Directory.Append( RawDirectory );

	if ( Directory.Amount() != 0 )
		if ( Directory( Directory.Last() ) != '/' )
			Directory.Append( '/' );

	Add_( Directory, sExcludedDir, Automat );
qRR
qRT
qRE
}

static inline bso::bool__ IsDirectory_( const str::string_ &DirectoryOrExtension )
{
	return DirectoryOrExtension.Search( '/' ) != qNIL;
}

void dwtxcl::excluder_::Add( const str::string_ &DirectoryOrExtension )
{
	if ( IsDirectory_( DirectoryOrExtension ) )
		AddDirectory( DirectoryOrExtension );
	else
		AddExtension( DirectoryOrExtension );
}

// Athought it is used for both file and directory, this functions also hnadles the test about absolute or relative directory.
state__ Matches_(
	const str::string_ &Tag,
	const _automat_ &Automat )
{
	state__ State = s_Undefined;

	sdr::row__ Row = Tag.Last();
	stsfsm::parser__ Parser;

	Parser.Init( Automat );

	while ( ( Row != qNIL ) && ( State == s_Undefined ) ){
		switch ( Parser.Handle( Tag( Row ) ) ) {
		case stsfsm::sLost:
			State = sIncluded;
			break;
		case stsfsm::sMatch:
			State = (state__)Parser.GetId();
			break;
		case stsfsm::sPending:
			Row = Tag.Previous( Row );
			break;
		default:
			qRFwk();
			break;
		}
	}

	if ( Row == qNIL )
		State = sIncluded;
	else if ( ( Tag( Row ) == '/' )
		      && ( State == sExcludedDir )
		      && ( Tag.Previous( Row ) != qNIL ) )
		State = sIncluded;

	return State;
}

static const str::string_ &NormalizeDirectory_(
	const str::string_ &RawPattern,
	str::string_ &Pattern )
{
	Pattern.Init( RawPattern );

	if ( Pattern.Amount() != 0 ) {
		if ( Pattern( Pattern.First() ) != '/' )
			Pattern.InsertAt( '/' );

		if ( Pattern( Pattern.Last() ) != '/' )
			Pattern.Append( '/' );
	}

	return Pattern;
}

state__ dwtxcl::excluder_::GetState(
	const str::string_ &RawPattern,
	bso::bool__ ForceAsDirectory ) const
{
	state__ State = s_Undefined;
qRH
	str::string Pattern;
qRB
	if ( ForceAsDirectory || IsDirectory_( RawPattern ) ) {
		Pattern.Init();

		State = Matches_( NormalizeDirectory_( RawPattern, Pattern ), Automat );
	} else
		State = Matches_( RawPattern, Automat );
qRR
qRT
qRE
	return State;
}
