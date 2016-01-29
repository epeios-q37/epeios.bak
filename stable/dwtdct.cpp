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

#define DWTDCT__COMPILATION

#include "dwtdct.h"

#include "mtx.h"
#include "mtk.h"
#include "dir.h"
#include "tagsbs.h"

using namespace dwtdct;

using namespace dwtbsc;
using namespace dwtght;
using dwtxcl::excluder_;

# define I( n, p )\
	n.Init();\
	fnm::BuildPath( Path, Basename, #p, n );\
	n##_.Init( "", n )

void dwtdct::files_data_hf___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
qRH
	fnm::name___ Names, Exclusions, Sizes, Timestamps;
qRB
	I( Names, n );
	I( Exclusions, x );
	I( Sizes, s );
	I( Timestamps, t );
qRR
qRT
qRE
}

#define P( i )\
	if ( bch::Plug( Files.i, Hook.i##_ ) != State ) {\
		State = uys::sInconsistent;\
		qRReturn;\
	}\

uys::state__ dwtdct::Plug(
	files_data_ &Files,
	files_data_fh___ &Hook )
{
	uys::state__ State = uys::s_Undefined;
qRH
qRB
	State = ctn::Plug( Files.Names, Hook.Names_ );

	if ( State.IsError() )
		qRReturn;

	P( Exclusions );
	P( Sizes );
	P( Timestamps );
qRR
qRT
	if ( State.IsError() )
		Hook.reset();	
qRE
	return State;
}


void dwtdct::_Delete( _items_ &Items )
{
	irow__ Row = Items.First();

	while ( Row != qNIL ) {
		if ( Items( Row ) != NULL ) {
			delete Items( Row );

			Items.Store( NULL, Row );
		}

		Row = Items.Next( Row );
	}
}

inline void FillCommon_(
	const fnm::name___ &Name,
	names_ &Names,
	str::string_ &Buffer,
	common__ &Common )
{
	Common.Name = Names.Append( Name.UTF8( Buffer ) );
}

static inline void Fill_(
	const fnm::name___ &Name,
	const oddity_ &Oddity,
	const fil::info__ &Info,
	names_ &Names,
	oddities_ &Oddities,
	str::string_ &Buffer,
	goof__ &Goof )
{
	FillCommon_( Name, Names, Buffer, Goof );

	if ( Oddity.Amount() )
		Goof.Oddity = Oddities.Append( Oddity );
}

static inline void FillRegular_(
	const fnm::name___ &Name,
	exclusion__ Exclusion,
	const fil::info__ &Info,
	names_ &Names,
	str::string_ &Buffer,
	regular__ &Regular )
{
	FillCommon_( Name, Names, Buffer, Regular );

	Regular.Exclusion = Exclusion;
	Regular.Timestamp = Info.Time.Modification;
}

static inline void Fill_(
	const fnm::name___ &Name,
	exclusion__ Exclusion,
	const fil::info__ &Info,
	names_ &Names,
	str::string_ &Buffer,
	directory_ &Directory )
{
	FillRegular_( Name, Exclusion, Info, Names, Buffer, Directory() );
}

static inline void Fill_(
	const fnm::name___ &Name,
	exclusion__ Exclusion,
	const fil::info__ &Info,
	names_ &Names,
	str::string_ &Buffer,
	file__ &File )
{
	FillRegular_( Name, Exclusion, Info, Names, Buffer, File );

	if ( File.Size != 0 )
		qRGnr();

	File.Size = Info.Size;
}

inline static const fnm::name___ &Build_(
	const fnm::name___ &Path,
	const fnm::name___ &Name,
	fnm::name___ &LocalizedFileName )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string Intermediate;
qRB
	Intermediate.Init( Path.UTF8( Buffer ) );
	Intermediate.Append( '/' );
	Intermediate.Append( Name.UTF8( Buffer ) );

	LocalizedFileName.Init( Intermediate.Convert( Buffer ) );
qRR
qRT
qRE
return LocalizedFileName;
}

#define HERMETIC_ODDITY	"Hermetic"

static inline bso::bool__ Append_(
	exclusion__ Exclusion,
	exclusions_handling__ ExclusionsHandling )
{
	switch ( Exclusion ) {
	case xNo:
		return true;
		break;
	case xMatchList:
	case xGhost:
		switch ( ExclusionsHandling ) {
		case ehNone:
			return false;
			break;
		case ehSkip:
			return false;
			break;
		case ehRegular:
			return false;
			break;
		case ehKeep:
			return true;
			break;
		case ehKeepGhostLike:
			return true;
			break;
		default:
			qRGnr();
			break;
		}
	case xFileTooBig:
	case xNameTooLong:
		switch ( ExclusionsHandling ) {
		case ehSkip:
			return false;
		case ehRegular:
			return true;
			break;
		case ehKeep:
			return true;
			break;
		case ehKeepGhostLike:
			return true;
			break;
		default:
			qRGnr();
			break;
		}
		break;
	default:
		qRGnr();
		break;
	}

	return false;	// To avoid a 'warning'.
}

static exclusion__ Convert_( dwtxcl::state__ State )
{
	switch ( State )  {
	case dwtxcl::sIncluded:
		return xNo;
		break;
	case dwtxcl::sExcludedDir:
	case dwtxcl::sExcludedFile:
		return xMatchList;
		break;
	case dwtxcl::sGhost:
		return xGhost;
		break;
	default:
		qRGnr();
		break;
	}

	return x_Undefined;	// Pour �viter un 'warning'.
}

exclusion__ GetExclusion_(
	const fnm::name___ &LocalizedFileNameWithoutRoot,
	const fnm::name___  &Name,
	const fil::info__ &Info,
	const dwtxcl::excluder_ &Excluder,
	const dwtbsc::limitations__ &Limitations )
{
	exclusion__ Exclusion = x_Undefined;
qRH
	str::string Buffer;
qRB
	Buffer.Init();

	switch ( Info.Type ) {
	case fil::tDir:
		Exclusion = Convert_( Excluder.GetState( LocalizedFileNameWithoutRoot.UTF8( Buffer ), false ) );
		break;
	case fil::tFile:
		Exclusion = Convert_( Excluder.GetState( Name.UTF8( Buffer ), true ) );

		if ( Exclusion != xGhost )
			Exclusion = Convert_( Excluder.GetState( Name.UTF8( Buffer ), false ) );
		break;
	default:
		qRGnr();
		break;
	}

	if ( Exclusion == xNo ) {
		if ( ( Limitations.NameLengthMax > 0 ) && ( Name.Amount() > Limitations.NameLengthMax ) )
			Exclusion = xNameTooLong;
		else if ( ( Limitations.FileSizeMax > 0 ) && ( Info.Type == fil::tFile ) && ( Info.Size > Limitations.FileSizeMax ) )
			Exclusion = xFileTooBig;
	}
qRR
qRT
qRE
	return Exclusion;
}

static void GetFile_(
	const fnm::name___ &LocalizedFileNameWithRoot,
	const fnm::name___ &LocalizedFileNameWithoutRoot,
	const fnm::name___ &Name,
	const excluder_ &Excluder,
	const dwtbsc::limitations__ Limitations,
	const dwtbsc::ghosts_oddities_ &GO,
	exclusions_handling__ ExclusionsHandling,
	kernel_ &Kernel,
	dwtght::grow__ &GhostRow )
{
qRH
	fil::info__ Info;
	fil::error__ Error = fil::e_Undefined;
	oddity Oddity;
	goof__ Goof;
	file__ File;
	directory Directory;
	str::string SBuffer;
	TOL_CBUFFER___ CBuffer;
	exclusion__ Exclusion = xNo;
	str::string GhostRawName;
qRB
	Oddity.Init();
	Info.Init();

	switch( Error = fil::GetInfo( LocalizedFileNameWithRoot, Info ) ) {
	case fil::eNone:
		switch ( Info.Type ) {
		case fil::tDir:
			Directory.Init();
			SBuffer.Init();
			Exclusion = GetExclusion_( LocalizedFileNameWithoutRoot, Name, Info, Excluder, Limitations );
			Fill_( Name, Exclusion, Info, Kernel.Names, SBuffer, Directory );
			if ( Append_( Exclusion, ExclusionsHandling) )
				Kernel.Directories.Append( Directory );
			break;
		case fil::tFile:
			File.Init();
			SBuffer.Init();
			Exclusion = GetExclusion_( LocalizedFileNameWithoutRoot, Name, Info, Excluder, Limitations );
			Fill_( Name, Exclusion, Info, Kernel.Names, SBuffer, File );
			if ( Append_( Exclusion, ExclusionsHandling ) )
				Kernel.Files.Append( File );
//			Kernel.Names( 0 ).Convert( CBuffer );
//			Kernel.Names.Flush();
			break;
		default:
			qRVct();
			break;
		}

		if ( Exclusion == xGhost ) {
			GhostRawName.Init();
			Name.UTF8( GhostRawName );
			if ( ( GhostRow = dwtght::GetGhostRow( GhostRawName, GO ) ) == qNIL )
				qRGnr();
		}
		break;
	default:
		Goof.Init();
		SBuffer.Init();
		Oddity.Init();	// Pas utilis� pour l'instant. Peut-�tre dans des versions ult�rieure, pour pr�ciser la raison du probl�me.
		Fill_( Name, Oddity, Info, Kernel.Names, Kernel.Oddities, SBuffer, Goof );
		Kernel.Goofs.Append( Goof );
		break;
	}

qRR
qRT
qRE
}

static bso::bool__ GetFiles_(
	const fnm::name___ &PathWithRoot,
	const fnm::name___ &PathWithoutRoot,
	const excluder_ &Excluder,
	const dwtbsc::limitations__ &Limitations,
	const dwtbsc::ghosts_oddities_ &GO,
	exclusions_handling__ ExclusionsHandling,
	kernel_ &Kernel,
	oddity_ &Oddity,
	dwtght::grow__ &GhostRow )
{
	bso::bool__ Success = true;
qRH
	dir::handle___ Handle;
	fnm::name___ Name;
	fnm::name___ LocalizedFileNameWithRoot, LocalizedFileNameWithoutRoot;
	TOL_CBUFFER___ Buffer;
qRB
	Name.Init();
	Name = dir::GetFirstFile( PathWithRoot, Handle );

	while ( !Name.IsEmpty() ) {
		if ( *Name.UTF8( Buffer ) != '.' ) {
			LocalizedFileNameWithRoot.Init();
			Build_( PathWithRoot, Name, LocalizedFileNameWithRoot );

			LocalizedFileNameWithoutRoot.Init();
			Build_( PathWithoutRoot, Name, LocalizedFileNameWithoutRoot );

			GetFile_( LocalizedFileNameWithRoot, LocalizedFileNameWithoutRoot, Name, Excluder, Limitations, GO, ExclusionsHandling, Kernel, GhostRow );
		}

		Name = dir::GetNextFile( Handle );
	}

	dir::Close( Handle );
qRR
	err::buffer__ Buffer;
	Oddity.Append( err::Message( Buffer ) );
	Success = false;
	ERRRst();
qRT
qRE
	return Success;
}

static void HandleGoofs_(
	const goofs_ &Goofs,
	const names_ &Names,
	const oddities_ &Oddities,
	goofs_data_ &Data )
{
qRH
	ctn::E_CMITEMt( name_, nrow__ ) Name;
	ctn::E_CMITEMt( oddity_, orow__ ) Oddity;
	dwtbsc::grow__ Row = qNIL;
	dwtbsc::grow__ Control = qNIL;
	goof__ Goof;
	str::string EmptyOddity;
qRB
	Name.Init( Names );
	Oddity.Init( Oddities );
	EmptyOddity.Init();

	Row = Goofs.First();

	Data.Names.PreAllocate( Goofs.Extent() );
	Data.Oddities.PreAllocate( Goofs.Extent() );

	while ( Row != qNIL ) {
		Goofs.Recall( Row, Goof );

		Control = Data.Names.Append( Name( Goof.Name ) );

		if ( Goof.Oddity == qNIL ) {
			if ( Control != Data.Oddities.Append( EmptyOddity ) )
				qRGnr();
		} else
			if ( Control != Data.Oddities.Append( Oddity( Goof.Oddity ) ) )
				qRGnr();

		Row = Goofs.Next( Row );
	}
qRR
qRT
qRE
}

static void HandleFiles_(
	const files_ &Files,
	const names_ &Names,
	files_data_ &Data )
{
qRH
	ctn::E_CMITEMt( name_, nrow__ ) Name;
	frow__ Row = qNIL;
	frow__ Control = qNIL;
	file__ File;
qRB
	Name.Init( Names );

	Row = Files.First();

	Data.Names.PreAllocate( Files.Extent() );
	Data.Exclusions.PreAllocate( Files.Extent() );
	Data.Sizes.PreAllocate( Files.Extent() );
	Data.Timestamps.PreAllocate( Files.Extent() );

	while ( Row != qNIL ) {
		Files.Recall( Row, File );

		Control = Data.Names.Append( Name( File.Name ) );

		if ( Control != Data.Exclusions.Append( File.Exclusion ) )
			qRGnr();

		if ( Control != Data.Sizes.Append( File.Size ) )
			qRGnr();

		if ( Control != Data.Timestamps.Append( File.Timestamp ) )
			qRGnr();

		Row = Files.Next( Row );
	}
qRR
qRT
qRE
}


static void HandleDirs_(
	irow__ Parent,
	const str::string_ &Path,
	const directories_ &Directories,
	const names_ &Names,
	items_ &Items,
	irows_ &ToHandle )
{
qRH
	item_ *Item = NULL;
	drow__ Row = qNIL;
	str::string NewPath;
	bso::size__ ParentPathSize;
	ctn::E_CITEMt( directory_, drow__ ) Directory;
	ctn::E_CMITEMt( name_, nrow__ ) Name;
qRB
	Directory.Init( Directories );
	Name.Init( Names );

	Row = Directories.First();

	NewPath.Init( Path );
	NewPath.Append( '/' );

	ParentPathSize = NewPath.Amount();

	while ( Row != qNIL ) {
		if ( ( Item = new item ) == NULL )
			qRAlc();

		NewPath.Crop( ParentPathSize );
		NewPath.Append( Name( Directory( Row )().Name ) );

		Item->Dir.Name.PreAllocate( Name( Directory( Row )().Name ).Extent() );
		Item->Path.PreAllocate( NewPath.Extent() );

		Item->Init();
		Item->Dir.Name = Name( Directory( Row )().Name );

		Item->Dir.Exclusion() = Directory( Row )().Exclusion;
		Item->Dir.Timestamp() = Directory( Row )().Timestamp;

		Item->Parent() = Parent;
		Item->Path = NewPath;

		ToHandle.Push( Items.Append( Item ) );
		// Item.reset( false );	// To avoid to destroy the content, as it is stored as is and not copied by the above 'Append(...)'
		Item = NULL;

		Row = Directories.Next( Row );
	}
qRR
qRT
	if ( Item != NULL )
		delete Item;
qRE
}

struct data___
{
private:
	tamount__ _ThreadAmountMax;	// Nombre maximum de 'thread' simultan�s.
	tamount__ _ThreadAmount;
	exclusions_handling__ _ExclusionsHandling;
	content_ *_Content;
	const excluder_ *_Excluder;
	const dwtbsc::limitations__ *_Limitations;
	const str::string_ *_RootPath;
	const dwtbsc::ghosts_oddities_ *_GO;
public:
	mtx::handler___ Protection;
	irows ToHandle;
	void reset( bso::bool__ P = true )
	{
		Protection = MTX_INVALID_HANDLER;

		_ThreadAmountMax = 0;
		_ThreadAmount = 0;

		_ExclusionsHandling = eh_Undefined;

		_Content = NULL;
		_Excluder = NULL;
		_Limitations = NULL;
		_RootPath = NULL;
		_GO = NULL;

		ToHandle.reset( P );
	}
	E_CDTOR( data___ );
	void Init(
		content_ &Content,
		tamount__ ThreadAmountMax,
		exclusions_handling__ ExclusionsHandling,
		const excluder_ &Excluder,
		const dwtbsc::limitations__ &Limitations,
		const str::string_ &RootPath,
		const dwtbsc::ghosts_oddities_ &GO )
	{
		_ThreadAmount = 0;
		_ThreadAmountMax = ThreadAmountMax;

		_ExclusionsHandling = ExclusionsHandling;

		_Content = &Content;
		_Excluder = &Excluder;
		_Limitations = &Limitations,
		_RootPath = &RootPath;
		_GO = &GO;

		ToHandle.Init();

		Protection = mtx::Create( !ThreadAllowed() );
	}
	content_ &Content( void )
	{
		if ( _Content == NULL )
			qRGnr();

		return *_Content;
	}
	const excluder_ &Excluder( void ) const
	{
		if ( _Excluder == NULL )
			qRGnr();

		return *_Excluder;
	}
	const dwtbsc::limitations__ &Limitations( void ) const
	{
		if ( _Limitations == NULL )
			qRGnr();

		return *_Limitations;
	}
	const str::string_ &RootPath( void ) const
	{
		if ( _RootPath == NULL )
			qRGnr();

		return *_RootPath;
	}
	const dwtbsc::ghosts_oddities_ &GO( void ) const
	{
		if ( _GO == NULL )
			qRGnr();

		return *_GO;
	}
	bso::bool__ ThreadAllowed( void ) const
	{
		return _ThreadAmountMax != 0;
	}
	void IncreaseThreadAmount( void )
	{
		if ( ThreadAllowed() ) {
			if ( _ThreadAmount == dwtdct::ThreadAmountMax )
				qRGnr();

			_ThreadAmount++;
		}
	}
	void DecreaseThreadAmount( void )
	{
		if ( ThreadAllowed() ) {
			if ( _ThreadAmount == 0 )
				qRGnr();

			_ThreadAmount--;
		}
	}
	tamount__ GetThreadAmount( void ) const
	{
		return _ThreadAmount;
	}
	tamount__ GetThreadAmountMax( void ) const
	{
		if ( !ThreadAllowed() )
			qRGnr();

		return _ThreadAmountMax;
	}
	E_RODISCLOSE__( exclusions_handling__, ExclusionsHandling );
};

static void Thread_( void *UP )	// Explore thread.
{
qRFH
	data___ &Data = *(data___ *)UP;
	bso::bool__ Continue = true;
	item_ *Item = NULL;
	dwtbsc::kernel Kernel;
	TOL_CBUFFER___ Buffer;
	mtx::mutex___ Mutex;
	irow__ Row = qNIL;
	oddity Oddity;
	fnm::name___ PathWithRoot;
qRFB
	content_ &Content = Data.Content();
	const excluder_ &Excluder = Data.Excluder();
	const dwtbsc::limitations__ &Limitations = Data.Limitations();
	exclusions_handling__ ExclusionHandling = Data.ExclusionsHandling();
	const str::string_ &RootPath = Data.RootPath();
	const dwtbsc::ghosts_oddities_ &GO = Data.GO();
	Mutex.Init( Data.Protection );

	while ( Continue ) {
//_CrtMemDumpStatistics( &s1 );
		Mutex.Lock();

		if ( Data.ToHandle.Amount() != 0 ) {
			Row = Data.ToHandle.Pop();
			Content.Recall( Row, Item );

			Mutex.Unlock();

			Kernel.Init();

			Oddity.Init();

			PathWithRoot.Init();
			Build_( RootPath, Item->Path, PathWithRoot );

			if ( !GetFiles_( PathWithRoot, Item->Path, Excluder, Limitations, GO, ExclusionHandling, Kernel, Oddity, Item->Dir.S_.GhostRow ) ) {
				if ( Item->Dir.Oddity.Amount() != 0 )
					qRGnr();

				Item->Dir.Oddity = Oddity;
			}

			HandleGoofs_( Kernel.Goofs, Kernel.Names, Kernel.Oddities, Item->Goofs );
			HandleFiles_( Kernel.Files, Kernel.Names, Item->Files );

			Mutex.Lock();

			HandleDirs_( Row, Item->Path, Kernel.Directories, Kernel.Names, Content, Data.ToHandle );
		} else if ( Data.ThreadAllowed() && Data.GetThreadAmount() == 0 )
			qRGnr();
		else {
			Data.DecreaseThreadAmount();
			Continue = false;
		}

		Mutex.Unlock();

		if ( !Data.ThreadAllowed() )
			Continue = false;
	}

//	Item.reset( false );	// To avoid destructing 
qRFR
qRFT
	if ( Continue )	{ // Anormal exiting.
			Mutex.Lock();
			Data.DecreaseThreadAmount();
		}
qRFE(;)
}

static void WaitForThreadsExiting_(
	data___ &Data,
	mtx::mutex___ &Mutex )
{
	Mutex.Lock();

	Data.ToHandle.Init();	// This tells the 'xthread_' that there is no more data to handle, hence it exits.

	while ( Data.GetThreadAmount() ) {
		Mutex.Unlock();
		tht::Defer();
		Mutex.Lock();
	}
/*
	while ( Data.SItems.Amount() ) {
		Mutex.Unlock();
		tht::Defer();
		Mutex.Lock();
	}
*/
	Mutex.Unlock();
}

void dwtdct::Explore(
	const str::string_ &Path,
	bso::uint__ ThreadAmountMax,
	const excluder_ &Excluder,
	const dwtbsc::limitations__ &Limitations,
	const dwtbsc::ghosts_oddities_ &GO,
	exclusions_handling__ ExclusionsHandling,
	content_ &Content,
	exploration_observer__ &Observer )
{
	drow__ Row = qNIL;
qRH
	directory Directory;
	data___ Data;
	bso::bool__ Continue = true;
	bso::uint__ Handled = 0;
	TOL_CBUFFER___ Buffer;
	fil::info__ Info;
	mtx::mutex___ Mutex;
	item_ *Item = NULL;
	bso::bool__ DataInitialized = false;
qRB
	if ( !fil::Exists( Path ) )
		qRGnr();

	Data.Init( Content, ThreadAmountMax, ExclusionsHandling, Excluder, Limitations, Path, GO );
	DataInitialized = true;

	Mutex.Init( Data.Protection );

	Info.Init();
	if ( fil::GetInfo( Path, Info ) != fil::eNone )
		qRReturn;

	if ( ( Item = new item ) == NULL )
		qRAlc();

	Item->Init();
//	Item->Path = Path;
	Item->Parent() = qNIL;
	Item->Dir.Exclusion() = xNo;
	Item->Dir.Timestamp() = Info.Time.Modification;

	Data.ToHandle.Push( Content.Append( Item ) );
	Item = NULL;

	Observer.Report( 0, 1, 0 );

	while ( Continue ) {
		Mutex.Lock();

		if ( Observer.IsElapsed() )
			Observer.Report( Content.Amount(), Data.ToHandle.Amount(), Data.GetThreadAmount() );

		if ( Data.ToHandle.Amount() != 0 ) {
			if ( !Data.ThreadAllowed() )
				Thread_( &Data );
			else if ( Data.GetThreadAmount() < Data.GetThreadAmountMax() ) {
				mtk::Launch( Thread_, &Data );
				Data.IncreaseThreadAmount();
			}
		} else if ( ( Data.GetThreadAmount() == 0 ) && ( Data.ToHandle.Amount() == 0 ) )
			Continue = false;

		Mutex.Unlock();

		if ( Continue )
			tht::Defer();
	}

	Observer.Report( Content.Amount(), Data.ToHandle.Amount(), 0 );
qRR
qRT
	if ( Item != NULL )
		delete Item;
	if ( DataInitialized )
		WaitForThreadsExiting_( Data, Mutex );	
qRE
}

void dwtdct::basic_exploration_observer___::DWTDCTReport(
	bso::uint__ Handled,
	bso::uint__ ToHandle,
	tamount__ ThreadAmount )
{
	static time_t Depart = tol::EpochTime( false );
	static tamount__ ThreadAmountMax = ThreadAmount;
qRH
	str::string Message;
	str::strings Values;
	bso::integer_buffer__ Buffer;
qRB
	if ( ThreadAmount > ThreadAmountMax )
		ThreadAmountMax = ThreadAmount;

	time_t Diff = tol::EpochTime( false ) - Depart;

	Values.Init();

	Values.Append( str::string ( bso::Convert( ThreadAmount, Buffer ) ) );
	Values.Append( str::string ( bso::Convert( ThreadAmountMax, Buffer ) ) );
	Values.Append( str::string ( bso::Convert( Handled, Buffer ) ) );
	Values.Append( str::string ( bso::Convert( ToHandle, Buffer ) ) );

	if ( Diff != 0 )
		Values.Append( str::string ( bso::Convert( Handled / Diff, Buffer ) ) );
	else
		Values.Append( str::string( "?" ) );

	Message.Init( _Message );

	tagsbs::SubstituteShortTags( Message, Values, '%' );

	_Flow.Clear();
	_Flow << Message;
	_Flow.CR();
	_Flow.Commit();

qRR
qRT
qRE
}

static inline status__ CreateGhostIfRequired_(
	const str::string_ &Root,
	const item_ &Item,
	const dwtbsc::ghosts_oddities_ &GO,
	dwtght::grow__ Parent,
	dwtght::ghosts_ &Ghosts,
	dwtght::grow__ &Row )
{
	if ( ( ( Row = Item.Dir.GhostRow() ) == qNIL ) || !Ghosts.Exists( Row ) )
		return dwtght::CreateGhost( Root, Item.Path, Item.Dir.Name, GO, Parent, Ghosts, Row );
	else if ( (Ghosts( Row ).S_.Parent != Parent) || (Ghosts( Row ).Name != Item.Dir.Name) ) {
qRH
		dwtght::ghost Ghost;
qRB
		Ghost.Init();
		Ghost.S_.Parent = Parent;
		Ghost.Name = Item.Dir.Name;

		Ghosts.Store( Ghost, Row );

		Ghosts.Flush();
qRR
qRT
qRE
		return dwtght::sUpdated;
	} else {
		Ghosts.Flush();
		return dwtght::sSkipped;
	}
}

typedef bch::E_BUNCHt_( irow__, dwtght::grow__ ) g2i_;
E_AUTO( g2i );

static void Clean_(
	dwtght::ghosts_ &Ghosts,
	g2i_ &G2I )
{
	dwtght::grow__ Row = G2I.First();

	while ( Row != qNIL ) {
		if ( G2I( Row ) == qNIL )
			if ( Ghosts.Exists( Row ) )
				Ghosts.Remove( Row );

		Row = G2I.Next( Row );
	}
}

namespace {
	void DeleteExtraneousGhostFiles_(
		const str::vString &Root,
		const item_ &Item,
		const dwtdct::fstrings_ &Filenames,
		const dwtxcl::excluder_ &Excluder,
		const dwtbsc::ghosts_oddities_ &GO,
		const ghosts_ &Ghosts,
		fGhostsSettingStats &Stats )
	{
		ctn::qCMITEM( str::vString, dwtdct::frow__ ) Filename;
		dwtght::grow__ GRow = qNIL;
		dwtdct::frow__ Row = Filenames.First();

		Filename.Init( Filenames );

		while ( Row != qNIL ) {
			if ( Excluder.GetState( Filename( Row ), true ) == dwtxcl::sGhost ) {
				GRow = dwtght::GetGhostRow( Filename( Row ), GO );

				if ( GRow != qNIL ) {
					if ( !Ghosts.Exists( GRow ) || ( GRow != Item.Dir.GetGhostRow()) ) {
						Stats.Inc( gssIntruder );
						dwtbsc::Delete( Root, Item.Path, Filename( Row ) );
					} else
						Stats.Inc( gssExpected );
				}
			}

			Row = Filenames.Next( Row );
		}

	}

	bso::fBool DeleteExtraneousGhosts_(
		const str::string_ &Root,
		const item_ &Item,
		const content_ &Content,
		dwtxcl::excluder_ &Excluder,
		const dwtbsc::ghosts_oddities_ &GO,
		const ghosts_ &Ghosts,
		fGhostsSettingStats &Stats )
	{
		dwtght::grow__ GRow = qNIL;

		if ( Excluder.GetState(Item.Dir.Name, true) == dwtxcl::sGhost ) {
			GRow = dwtght::GetGhostRow( Item.Dir.Name, GO );
	
			if ( GRow != qNIL ) {
				if ( GRow == 0  )
					Stats.Inc( gssExpected );
				else if ( !Ghosts.Exists( GRow ) || ( GRow != Content( Item.GetParent() )->Dir.GetGhostRow()) ) {
					Stats.Inc( gssIntruder );
					dwtbsc::Delete( Root, Item.Path, 0 );
				} else
					Stats.Inc( gssExpected );
			}

			return false;
		} else {
			DeleteExtraneousGhostFiles_( Root, Item, Item.Files.Names, Excluder, GO, Ghosts, Stats );

			return true;
		}
	}

}

void dwtdct::SetGhosts(
	const str::string_ &Root,
	const content_ &Content,
	const dwtbsc::ghosts_oddities_ &GO,
	fGhostsSettingObserver &GhostsSettingObserver,
	i2g_ &I2G )
{
qRH
	irow__ IRow = qNIL;
	fGhostsSettingStats Stats;
	g2i G2I;
	dwtght::grow__ GRow = qNIL;
	dwtght::rack___ GhostsRack;
	dwtght::ghost Ghost;
	dwtxcl::excluder Excluder;
qRB
	I2G.Init();
	I2G.Allocate( Content.Extent() );
	I2G.FillWith( qNIL );

	IRow = Content.First();

	Stats.Init();

	GhostsSettingObserver.Report( Stats );

	Excluder.Init( GO );

	GhostsRack.Init();
	ghosts_ &Ghosts = GetRWGhosts( Root, GO, GhostsRack );

	G2I.Init();
	G2I.Allocate( Ghosts.Extent() );
	G2I.FillWith( qNIL );

	if ( IRow != qNIL ) {
		Stats.SetValue( gssTotal, Content.Amount() - 1 );
		IRow = Content.Next( IRow );	// On saute le r�pertoire racine, qui correspond � 'Root'.

		Ghost.Init();
		Ghost.S_.Parent = qNIL;

		if ( Ghosts.Amount() == 0 )
			GRow = Ghosts.Add( Ghost );
		else
			GRow = Ghosts.First();

		I2G.Store( GRow, 0 );
		G2I.Allocate( Ghosts.Extent() );
		G2I.Store( 0, GRow );

		if ( GhostsSettingObserver.IsElapsed()  )
			GhostsSettingObserver.Report( Stats );
	}

	while ( IRow != qNIL ) {
		item_ &Item = *Content( IRow );

		if ( DeleteExtraneousGhosts_( Root, Item, Content, Excluder, GO, Ghosts, Stats ) ) {
			GRow = qNIL;
			switch ( CreateGhostIfRequired_( Root, Item, GO, I2G( Item.GetParent() ), Ghosts, GRow ) ) {
			case sCreated:
				Stats.Inc( gssCreated );
				I2G.Store( GRow, IRow );
				G2I.Allocate( Ghosts.Extent()  );
				G2I.Store( IRow, GRow );
				break;
			case sUpdated:
				I2G.Store( GRow, IRow );
				G2I.Store( IRow, GRow );
				Stats.Inc( gssUpdated );
				break;
			case sSkipped:
				I2G.Store( GRow, IRow );
				G2I.Store( IRow, GRow );
				Stats.Inc( gssSkipped );
				break;
			case sFailed:
				Stats.Inc( gssFailed );
				break;
			default:
				qRGnr();
				break;
			}
		}

		Stats.Inc( gssHandled );

		if ( GhostsSettingObserver.IsElapsed() )
			GhostsSettingObserver.Report( Stats );

		IRow = Content.Next( IRow );
	}

	GhostsSettingObserver.Report( Stats );

	Clean_( Ghosts, G2I );
qRR
qRT
qRE
}

namespace {
	void DelGhosts_(
		const str::vString &Root,
		const str::vString &Path,
		const dwtdct::fstrings_ &Filenames,
		const dwtxcl::excluder_ &Excluder )
	{
		ctn::qCMITEM( str::vString, dwtdct::frow__ ) Filename;
		dwtdct::frow__ Row = Filenames.First();

		Filename.Init( Filenames );

		while ( Row != qNIL ) {
			if ( Excluder.GetState( Filename( Row ), true ) == dwtxcl::sGhost )
				dwtbsc::Delete(Root, Path, Filename( Row ) );

			Row = Filenames.Next( Row );
		}
	}
}

void dwtdct::DelGhosts(
	const str::string_ &Root,
	const content_ &Content,
	const dwtbsc::ghosts_oddities_ &GO,
	dwtdct::exploration_observer__ &ExplorationObserver )
{
qRH
	irow__ IRow = qNIL;
	dwtxcl::excluder Excluder;
qRB
	IRow = Content.Last();	// On va � rebours, pour traiter les r�pertoires les plus imbriqu�s d'abord.

	Excluder.Init( GO );

	while ( IRow != qNIL ) {

		item_ &Item = *Content( IRow );

		if ( Excluder.GetState( Item.Dir.Name, true ) == dwtxcl::sGhost )
			if ( !dwtbsc::Delete( Root, Item.Path, 0 ) )
				qRGnr();

		DelGhosts_( Root, Item.Path, Item.Files.Names, Excluder );

		IRow = Content.Previous( IRow );
	}

qRR
qRT
qRE
}

typedef bch::E_BUNCHt_( irow__, dwtght::grow__ ) ghosts_reminder_;	// Garde la trace pour chaque 'ghost' de l'item l'utilisant.
															// Pour �viter qu'un 'ghost' soit utilis� par deux items (duplication de r�pertoire).
E_AUTO( ghosts_reminder );

void dwtdct::TestGhosts(
	const str::string_ &Root,
	const content_ &Content,
	const dwtbsc::ghosts_oddities_ &GO,
	const str::string_ &NoGhostMessage,
	const str::string_ &GhostIgnoredMessage,
	txf::text_oflow__ &TFlow )
{
qRH
	irow__ IRow = qNIL, ParentIRow = qNIL;
	dwtght::grow__ GRow = qNIL;
	rack___ GhostsRack;
	ctn::E_CMITEMt( ghost_, dwtght::grow__ ) Ghost;
	bso::bool__ Moved = false, Renamed = false, NoGhost = false, GhostIgnored = false;
	str::string Path;
	ghosts_reminder Reminder;
qRB
	IRow = Content.First();

	GhostsRack.Init();
	const ghosts_ &Ghosts = GetROGhosts( Root, GO, GhostsRack );

	Reminder.Init();
	Reminder.Allocate( Ghosts.Extent(), aem::mFitted );
	Reminder.FillWith( qNIL );

	Ghost.Init( Ghosts );

	if ( IRow != qNIL ) {
		IRow = Content.Next( IRow );	// On saute le r�pertoire racine, qui correspond � 'Root'.
	}

	while ( IRow != qNIL ) {
		item_ &Item = *Content( IRow );

		Moved = Renamed = NoGhost = GhostIgnored = false;

		ParentIRow = Item.Parent();
		GRow = Item.Dir.GhostRow();

		if ( GRow == qNIL )
			NoGhost = true;
		else if ( Reminder( GRow ) != qNIL )
			GhostIgnored = true;
		else {
			Reminder.Store( IRow, GRow );

			if ( Ghost( GRow ).Name != Item.Dir.Name )
				Renamed = true;

			if ( ParentIRow == qNIL )
				qRGnr();

			if ( Ghost( GRow ).S_.Parent != Content( ParentIRow )->Dir.GhostRow() )
				Moved = true;
		}

		if ( NoGhost || GhostIgnored || Moved || Renamed ) {
			if ( NoGhost )
				TFlow << Item.Path << " : " << NoGhostMessage;
			else if ( GhostIgnored )
				TFlow << Item.Path << " : " << GhostIgnoredMessage;
			else {
				Path.Init();

				TFlow << GetPath( GRow, Ghosts, Path );

				if ( Moved ) {
					if ( Renamed )
						TFlow << " => ";
					else
						TFlow << " -> ";

					TFlow << Item.Path;
				} else
					TFlow <<  " => " << Item.Dir.Name;
			}

			TFlow << txf::nl;
		}

		IRow = Content.Next( IRow );
	}

qRR
qRT
qRE
}

void dwtdct::rBasicGhostsSettingObserver::DWTDCTReport( const fGhostsSettingStats &Stats )
{
	static time_t Depart = tol::EpochTime( false );
qRH
	time_t Diff = 0;
	str::string Message;
	str::strings Values;
qRB
	Diff = tol::EpochTime( false ) - Depart;

	Values.Init();

	str::Append( Stats.GetValue( gssHandled ), Values );
	str::Append( Stats.GetValue( gssTotal ), Values );

	if ( Diff != 0 )
		str::Append( Stats.GetValue( gssHandled ) / Diff, Values );
	else
		Values.Append( str::string( "?" ) );

	str::Append( Stats.GetValue( gssCreated ), Values );
	str::Append( Stats.GetValue( gssUpdated ), Values );
	str::Append( Stats.GetValue( gssSkipped ), Values );
	str::Append( Stats.GetValue( gssFailed ), Values );
	str::Append( Stats.GetValue( gssIntruder ), Values );
	str::Append( Stats.GetValue( gssExpected ), Values );


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

namespace {
	void Append_(
		const fstrings_ &Names,
		const fexclusions_ &Exclusions,
		const sizes_ &Sizes,
		const timestamps_ &Timestamps,
		frows_ &FRows,
		files_data_ &Files )
	{
		frow__ SRow = qNIL, TRow = qNIL;

		ctn::E_CMITEMt( str::string_, frow__ ) Name;
		Name.Init( Names );

		SRow = Names.First();

		while ( SRow != qNIL )
		{
			if ( Exclusions(SRow) == dwtbsc::xNo ) {
				TRow = Files.Names.Append( Name( SRow ) );

				if ( TRow != Files.Exclusions.Append(Exclusions( SRow ) ) )
					qRFwk();

				if ( TRow != Files.Sizes.Append(Sizes( SRow ) ) )
					qRFwk();

				if ( TRow != Files.Timestamps.Append (Timestamps( SRow ) ) )
					qRFwk();

				FRows.Append( TRow );
			}

			SRow = Names.Next( SRow );
		}
	}

	void Append_(
		grow__ GRow,
		const files_data_ &SourceFiles,
		gfrows_ &GFRows,
		files_data_ &TargetFiles )
	{
	qRH
		frows FRows;
	qRB
		FRows.Init();

		Append_(SourceFiles.Names, SourceFiles.Exclusions, SourceFiles.Sizes, SourceFiles.Timestamps, FRows, TargetFiles );

		if ( !GFRows.Exists( GRow ) )
			GFRows.Allocate( *GRow + 1 );

		GFRows( GRow ) = FRows;
		GFRows.Flush();
	qRR
	qRT
	qRE
	}
}

void dwtdct::ghost2files_::Append(
	grow__ GRow,
	const item_ &Item )
{
	if ( GRow != qNIL )	// Could be 'qNIL' if ghost dir.
		Append_( GRow, Item.Files, GFRows, Files );
}

void dwtdct::ghost2files_::Append(
	const content_ &Content,
	const i2g_ &I2G )
{
	if ( Content.Size() != I2G.Size() )
		qRFwk();

	irow__ Row = Content.First();

	while ( Row != qNIL ) {
		Append( I2G( Row ), *Content( Row ) );

		Row = Content.Next( Row );
	}
}

void dwtdct::ghost2files_hf___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
qRH
	fnm::name___ GFRows, Files;
qRB
	I( GFRows, g );
	I( Files, f );
qRR
qRT
qRE
}

uys::state__ dwtdct::Plug(
	ghost2files_ &Files,
	ghost2files_fh___ &Hook )
{
	uys::state__ State = uys::s_Undefined;
qRH
qRB
	State = Plug( Files.Files, Hook.Files_ );

	if ( State.IsError() )
		qRReturn;

	if ( State != ctn::Plug( Files.GFRows, Hook.GFRows_ ) )
		qRFwk();
qRR
qRT
	if ( State.IsError() )
		Hook.reset();	
qRE
	return State;
}

namespace {
	void SetHook_(
		const fnm::name___ &Path,
		uys::mode__ Mode,
		ghost2files_fh___ &Hook )
	{
	qRH
		ghost2files_hf___ Filenames;
	qRB
		Filenames.Init( Path, "Files_" );

		Hook.Init( Filenames, Mode, uys::bPersistent, flsq::GetId() );
	qRR
	qRT
	qRE
	}

	void GetRack_(
		fnm::name___ &DataDirName,
		uys::mode__ Mode,
		ghost2files_rack___ &Rack )
	{
		SetHook_( DataDirName, Mode, Rack.Hook );

		if ( !Plug( Rack.G2F, Rack.Hook ).Boolean() )
			Rack.G2F.Init();
	}
}


ghost2files_ &dwtdct::GetRWG2F(
	const str::string_ &Root,
	const dwtbsc::ghosts_oddities_ &GO,
	ghost2files_rack___ &Rack )
{
qRH
	fnm::name___ Name;
qRB
	Name.Init();
	GetGhostsDataDirName( Root, GO, Name );

	GetRack_( Name, uys::mReadWrite, Rack );
qRR
qRT
qRE
	return Rack.G2F;
}

const ghost2files_ &dwtdct::GetROG2F(
	const str::string_ &Root,
	const dwtbsc::ghosts_oddities_ &GO,
	ghost2files_rack___ &Rack )
{
qRH
	fnm::name___ Name;
qRB
	Name.Init();
	GetGhostsDataDirName( Root, GO, Name );

	if ( fil::Exists( Name ) )
		GetRack_( Name, uys::mReadOnly, Rack );
	else
		Rack.G2F.Init();
qRR
qRT
qRE
	return Rack.G2F;
}

namespace {
	void Fill_(
		const frows_ &FRows,
		const files_data_ &Files,
		directory_ &Directory,
		kernel_ &Kernel )
	{
		ctn::E_CMITEMt( str::string_, frow__ ) Name;
		file__ File;
		frow__ FRow = qNIL;
		sdr::row__ Row = FRows.First();

		Name.Init( Files.Names );

		while ( Row != qNIL ) {
			FRow = FRows( Row );

			File.Init();
			File.Name = Kernel.Names.Append( Name( FRow ) );
			File.Exclusion = Files.Exclusions( FRow );
			File.Size = Files.Sizes( FRow );
			File.Timestamp = Files.Timestamps( FRow );

			Directory.Files.Append( Kernel.Files.Append( File ) );

			Row = FRows.Next( Row );
		}
	}

	void Fill_(
		grow__ GRow,
		const ghost2files_ &G2F,
		directory_ &Directory,
		kernel_ &Kernel )
	{
		if ( GRow != qNIL ) {
			ctn::E_CMITEMt( frows_, grow__ ) FRows;

			FRows.Init( G2F.GFRows );

			Fill_(FRows( GRow ), G2F.Files, Directory, Kernel );
		}
	}

	drow__ Fill_(
		const item_ &Item,
		const ghost2files_ &G2F,
		kernel_ &Kernel )
	{
		drow__ Row = qNIL;
	qRH
		directory Directory;
	qRB
		Directory.Init();

		Fill_( Item.Dir.GetGhostRow(), G2F, Directory, Kernel );

		Directory().Exclusion = Item.Dir.Exclusion();
		Directory().Timestamp = Item.Dir.Timestamp();
		Directory().Name = Kernel.Names.Append( Item.Dir.Name );

		Row = Kernel.Directories.Append( Directory );
	qRR
	qRT
	qRE
		return Row;
	}
}

void dwtdct::Fill(
	const content_ &Content,
	const ghost2files_ &G2F,
	kernel_ &Kernel )
{
qRH
	irow__ Row = qNIL;
	item_ *Item = NULL;
	drow__ DRow = qNIL;
	i2d I2D;
qRB
	I2D.Init();

	Row = Content.First();

	while ( Row != qNIL ) {
		Item = Content( Row );

		if ( Item == NULL )
			qRFwk();

		DRow = Fill_( *Item, G2F, Kernel );

		if ( I2D.Append( DRow ) != Row )
			qRFwk();

		if ( Item->GetParent() != qNIL ) {
			Kernel.Directories( I2D( Item->GetParent() ) ).Dirs.Append( DRow );
			Kernel.Directories.Flush();
		}

		Row = Content.Next( Row );
	}
qRR
qRT
qRE
}




