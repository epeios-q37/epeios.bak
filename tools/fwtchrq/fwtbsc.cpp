/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of fwtchrq.

    fwtchrq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    fwtchrq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with fwtchrq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "fwtbsc.h"

#include "fwtdct.h"

#include "fnm.h"
#include "dir.h"
#include "tagsbs.h"

using namespace fwtbsc;

static stsfsm::automat SortTypeAutomat_;

static void FillSortTypeAutomat_( void )
{
	int i = 0;
	SortTypeAutomat_.Init();

	while ( i < st_amount ) {
		stsfsm::Add( GetLabel( (sort_type__)i ), i, SortTypeAutomat_ );

		i++;
	}
}

#define ST( name )	case st##name : return #name; break

const char *fwtbsc::GetLabel( sort_type__ SortType )
{
	switch ( SortType ) {
	ST( None );
	ST( Regular );
	ST( Reverse );
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

sort_type__ fwtbsc::GetSortType( const str::string_ &Pattern )
{
	stsfsm::id__ Id = GetId( Pattern, SortTypeAutomat_ );

	if ( Id == stsfsm::UndefinedId )
		return st_Undefined;
	else if ( Id >= st_amount )
		qRGnr();
	else
		return (sort_type__)Id;

	return st_Undefined;	// Pour éviter un 'warning'.
}

# define I( n, p )\
	n.Init();\
	fnm::BuildPath( Path, Basename, #p, n );\
	n##_.Init( "", n )


void fwtbsc::kernel_hook_filenames___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
qRH
	fnm::name___ Goofs, Files, Directories, Names, Oddities;
qRB
	I( Goofs, g );
	I( Files, f );
	I( Directories, d);
	I( Names, n );
	I( Oddities, o );
qRR
qRT
qRE
}

const fnm::name___ &fwtbsc::GetGhostLocalizedName(
	sdr::row_t__ Row,
	const str::string_ &Root,
	const str::string_ &Path,
	const ghosts_oddities_ &GO,
	fnm::name___ &LocalizedName )
{
qRH
	str::string Name, Localization;
	bso::integer_buffer__ Buffer;
qRB
	Name.Init();

	Name.Append( GO.Prefix );
	Name.Append( '.' );
	Name.Append( bso::Convert( Row, Buffer ) );
	Name.Append( '.' );

	Localization.Init( Root );
	Localization.Append( Path );

	fnm::BuildPath( Localization, Name, GO.Suffix, LocalizedName );
qRR
qRT
qRE
	return LocalizedName;
}

const fnm::name___ &fwtbsc::GetGhostsDataDirName(
	const str::string_ &Root,
	const ghosts_oddities_ &GO,
	fnm::name___ &Name )
{
	return GetGhostLocalizedName( 0, Root, str::string(), GO, Name );
}

#define B( i )\
	if ( i.Bind() != State ) {\
		State = uys::sInconsistent;\
		qRReturn;\
	}\

uys::state__ fwtbsc::kernel_files_hook___::Bind( void )
{
	uys::state__ State = uys::s_Undefined;
qRH
	State = Goofs_.Bind();

	if ( State.IsError() )
		qRReturn;

	B( Files_ );
	B( Directories_ );
	B( Names_ );
	B( Oddities_ );
qRB
qRR
qRT
qRE
	return State;
}

#define P( m, i )\
	if ( m::Plug( Kernel.i, Hook.i##_ ) != State ) {\
		State = uys::sInconsistent;\
		qRReturn;\
	}\

uys::state__ fwtbsc::Plug(
	kernel_ &Kernel,
	kernel_files_hook___ &Hook )
{
	uys::state__ State = uys::s_Undefined;
qRH
qRB
	State = bch::Plug( Kernel.Goofs, Hook.Goofs_ );

	if ( State.IsError() )
		qRReturn;

	P( bch, Files );
	P( ctn, Directories );
	P( ctn, Names );
	P( ctn, Oddities );
qRR
qRT
	if ( State.IsError() )
		Hook.reset();
qRE
	return State;
}

void fwtbsc::observer___::Report_(
	bso::uint__ Handled,
	bso::uint__ Total )
{
	static time_t Depart = 0;
qRH
	str::string Message;
	str::strings Values;
	time_t Diff = 0;
qRB
	if ( Handled == 0 )
		Depart = tol::EpochTime( false );

	if ( Depart != 0  )
		Diff = tol::EpochTime( false ) - Depart;

	Values.Init();

	str::Append( Handled, Values );
	str::Append( Total - Handled, Values );
	str::Append( Total, Values );

	if ( Diff != 0 )
		str::Append( bso::uint__( Handled / Diff ), Values );
	else
		Values.Append( str::string( "?" ) );

	Message.Init(_Message );
	tagsbs::SubstituteShortTags( Message, Values, '%' );

	_Flow.Clear();
	_Flow << Message;
	_Flow.CR();
	_Flow.Commit();
qRR
qRT
qRE
}

static const str::string_ &GetAbsolute_(
	const str::string_ &Root,
	const str::string_ &Path,
	const str::string_ &Name,
	str::string_ &Absolute )
{
	Absolute.Append( Root );
	if ( (Root.Amount() != 0) && (Root( Root.Last() ) != '/' ) )
		Absolute.Append( '/' );

	Absolute.Append( Path );
	if ( (Path.Amount() != 0) && (Path( Path.Last() ) != '/' ) )
		Absolute.Append( '/' );

	Absolute.Append( Name );

	return Absolute;
}

static bso::bool__ DeleteFile_( const str::string_ &Name )
{
	return fil::Remove( Name );
}

static bso::bool__ Delete_(
	const str::string_ &Root,
	const str::string_ &Path,
	const fwtdct::files_data_ &Files )
{
	bso::bool__ Failure = false;
qRH
	ctn::E_CMITEMt( str::string_, fwtdct::frow__ ) Name;
	fwtbsc::frow__ Row = qNIL;
	str::string Absolute;
qRB
	Row = Files.First();

	Name.Init( Files.Names );

	while ( Row != qNIL ) {
		Absolute.Init();
		Failure |= !DeleteFile_( GetAbsolute_( Root, Path, Name( Row ), Absolute ) );

		Row = Files.Next( Row );

	}
qRR
qRT
qRE
	return !Failure;
}

static bso::bool__ DeleteDir_( const str::string_ &Name )
{
	return dir::DropDir( Name ) == dir::sOK;
}

static bso::bool__ Delete_(
	const str::string_ &Root,
	const fwtdct::item_ &Item,
	root_fate__ RootFate )
{
	bso::bool__ Failure = false;
qRH
	str::string Absolute;
qRB
	// Sur des lignes différents, et non pas sulr la même avec un '||', car l'échec du premier ne doit pas entrapiner la non-execution du second.
	Failure |= !Delete_( Root, Item.Path, Item.Files );

	switch ( RootFate ) {
	case rfKeep:
		break;
	case rfDelete:
		Absolute.Init();
		Failure |= !DeleteDir_( GetAbsolute_( Root, Item.Path, str::string(), Absolute ) );
		break;
	default:
		qRGnr();
		break;
	}
qRR
qRT
qRE
	return !Failure;
}

static bso::bool__ Delete_( 
	const str::string_ &Root,
	const fwtdct::content_ &Content,
	root_fate__ RootFate )
{
	fwtdct::irow__ Row = Content.Last();	// On va dans l'ordre inverse pour commencer par les répertoires les plus imbriquées.
	bso::bool__ Failure = false;

	while ( Row != qNIL ) {

		Failure |=  !Delete_( Root, *Content( Row ), ( Row == Content.First() ? RootFate : rfDelete ) );

		Row = Content.Previous( Row );
	}

	return !Failure;
}

bso::bool__ fwtbsc::Delete(
	const str::string_ &Path,
	tamount__ ThreadAmountMax,
	root_fate__ RootFate )
{
	bso::bool__ Success = false;
qRH
	ghosts_oddities GO;
	fwtxcl::excluder Excluder;
	limitations__ Limitations;
	fwtdct::content Content;
qRB
	GO.Init();
	Excluder.Init( GO );
	Limitations.Init();
	Content.Init();
	if ( fil::Exists( Path ) ) {
		fwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, ehKeep, Content, *(fwtdct::exploration_observer__ *)NULL );
		Success = Delete_( Path, Content, RootFate );
	}
qRR
qRT
qRE
	return Success;
}

static inline const str::string_ &GetAbsolute_(
	const str::string_ &Path,
	const str::string_ &Name,
	str::string_ &Absolute )
{
	Absolute.Init( Path );

	if ( Path.Amount() != 0  )
		Absolute.Append( '/' );

	Absolute.Append( Name );

	return Absolute;
}

bso::bool__ fwtbsc::Delete(
	const str::string_ &Path,
	const str::string_ &Name,
	fwtbsc::tamount__ ThreadAmountMax )
{
	bso::bool__ Success = false;
qRH	
	str::string Absolute;
qRB
	Absolute.Init( Path );

	Success = fwtbsc::Delete( GetAbsolute_( Path, Name, Absolute ), ThreadAmountMax, rf_Default );
qRR
qRT
qRE
	return Success;
}

bso::bool__ fwtbsc::Delete(
	const str::string_ &Path,
	const str::strings_ &FileNames )
{
	bso::bool__ Failure = false;
qRH
	ctn::E_CMITEM( str::string_ ) FileName;
	sdr::row__ Row = qNIL;
	str::string Absolute, Empty;
qRB

	Empty.Init();
	FileName.Init( FileNames );

	Row = FileNames.First();

	while ( Row != qNIL ) {
		Absolute.Init();
		Failure |= !DeleteFile_( GetAbsolute_( Empty, Path, FileName( Row ), Absolute ) );

		Row = FileNames.Next( Row );

	}
qRR
qRT
qRE
	return !Failure;
}

Q37_GCTOR( fwtbsc )
{
	FillSortTypeAutomat_();
}


