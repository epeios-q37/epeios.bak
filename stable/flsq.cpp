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

#define FLS__COMPILATION

#include "flsq.h"

using namespace flsq;

#include "lstbch.h"
#include "que.h"
#include "bch.h"

#ifdef FLS__MT
#	include "mtx.h"
#	include "mtk.h"
#endif

sdr::size__ flsq::MaxFileAmount = FLSQ__MAX_FILE_AMOUNT;

#ifdef FLS__MT
static mtx::mutex_handler__ Mutex_;
#endif

static inline void Lock_( void )
{
#ifdef FLS__MT
	mtx::Lock( Mutex_ );
#endif
}

static inline void Unlock_( void )
{
#ifdef FLS__MT
	mtx::Unlock( Mutex_ );
#endif
}
/*
static inline bso::bool__ IsLocked_( void )
{
#ifdef FLS__AUTOFLUSH
	return mtx::IsLocked( Mutex_ );
#else
	return true;
#endif
}
*/
struct _data__ {
#ifdef FLS__AUTOFLUSH
	bso::bool__ ToFlush;	// Doit tre 'flush' si  'true'.
#endif
	file_storage___ *File;
	id__ ID;
	void reset( bso::bool__ = true )
	{
#ifdef FLS__AUTOFLUSH
		ToFlush = false;
#endif
		File = NULL;
		ID = Undefined;
	}
};

static lstbch::E_LBUNCHt( _data__, row__ ) List_;
static que::E_MQUEUEt( row__ ) Queue_;

namespace {
	idsq::qIDSw( id__ )	Ids_;
}

id__ flsq::GetId( void )
{
	id__ ID = Undefined;

	Lock_();

	ID = Ids_.New();

	Unlock_();

	return ID;
}

void flsq::ReleaseId( id__ ID )
{
	Lock_();

	Ids_.Release( ID );

	Unlock_();
}

row__ flsq::_Register(
	file_storage___ &FS,
	id__ ID )
{
	row__ Row = qNIL;
	_data__ Data = {
#ifdef FLS__AUTOFLUSH
		false,
#endif
		&FS, ID };

	Lock_();

	Row = List_.New();

	if ( Queue_.Amount() < List_.Extent() )	// On teste 'Amount' parce que ce qui est entre 'Amount' et 'Extent' n'est pas initialis dans la file.
		Queue_.Allocate( List_.Extent() );

	List_.Store( Data, Row );

	Unlock_();

	return Row;
}

void flsq::_Unregister(
	row__ Row,
	id__ ID )
{
	_data__ Data;

	Lock_();

	if ( List_( Row ).ID != ID )
		qRFwk();

	List_.Store( Data, Row );
	List_.Delete( Row );

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );

	Unlock_();
}

#define DELAY	1	// en s.

#ifdef FLS__AUTOFLUSH
struct _flusher_data__
{
	row__ Row;
	time_t LastFileWriteTime;
	 _flusher_data__( void )
	{
		Row = qNIL;
		LastFileWriteTime = tol::EpochTime( false );
	}
} FlusherData_;

static inline void Flusher_( void * )
{
	bso::bool__ ToFlush = false;
	_file___ *File = NULL;

	Lock_();

	_data__ Data;

	while ( FlusherData_.Row != qNIL ) {

		Unlock_();
		tht::Defer();

		if ( ToFlush  ) {
			File->Lock();
			File->ThreadUnsafeFlush();
			File->Unlock();
			ToFlush = false;
			File = NULL;
		}

		Lock_();

		while ( ( tol::EpochTime( false ) - FlusherData_.LastFileWriteTime ) < DELAY ) {
			Unlock_();
			tht::Defer( DELAY * 1000 );
			Lock_();
		}

		if ( FlusherData_.Row != qNIL ) {
			List_.Recall( FlusherData_.Row, Data );

			if ( Data.ToFlush ) {
				ToFlush = true;
				File = &Data.File->File();

				Data.ToFlush = false;
				List_.Store( Data, FlusherData_.Row );
			}

			FlusherData_.Row = Queue_.Previous( FlusherData_.Row );
		}
	}

	Unlock_();
}
#endif
/*
inline static void LaunchFlusher_( void )
{
#ifdef FLS__AUTOFLUSH
#ifdef FLS_DBG
	if ( !IsLocked_() )
		ERRc();
#endif
	if ( FlusherData_.Row == qNIL )
		mtk::Launch( Flusher_, NULL );	// Le verrou est pos, donc ne fait rien tant que l'appelant n'te pas le verrou.

	FlusherData_.Row = Queue_.Last();
#endif
}
*/
inline static void TouchFlusher_( bso::bool__ ToFlush )	// Indique au 'flusher' qu'une criture  eu lieu.
{
#ifdef FLS__AUTOFLUSH
	if ( ToFlush )
		FlusherData_.LastFileWriteTime = tol::EpochTime( false );
#endif
}


void flsq::_ReportFileUsing(
	row__ Row,
	bso::bool__ ToFlush )
{
	Lock_();

#ifdef FLS__AUTOFLUSH
	if ( ToFlush ) {
		_data__ Data;

		List_.Recall( Row, Data );

		if ( !Data.ToFlush ) {
			Data.ToFlush = true;
			List_.Store( Data, Row );
		}

		LaunchFlusher_();
	}
#endif

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );
	else if ( Queue_.Amount() >= FLSQ__MAX_FILE_AMOUNT ) {
		List_( Queue_.Tail() ).File->ReleaseFile( false );
		Queue_.Delete( Queue_.Tail() );
	}

	if ( Queue_.IsEmpty() )
		Queue_.Create( Row );
	else
		Queue_.BecomePrevious( Row, Queue_.Head() );

	TouchFlusher_( ToFlush );

//	FlusherData_.LastFileAccessTime = tol::Clock( false );

	Unlock_();
}

void flsq::_ReportFileClosing( row__ Row )
{
	Lock_();

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );

	Unlock_();
}

static void _Search(
	id__ ID,
	bch::E_BUNCH_( row__ ) &Rows )
{
	row__ Row = List_.First();

	while ( Row != qNIL ) {
		if ( List_( Row ).ID == ID )
			Rows.Append( Row );

		Row = List_.Next( Row );
	}
}

static void _Release( const bch::E_BUNCH_( row__ ) &Rows )
{
	sdr::row__ Row = Rows.First();

	while ( Row != qNIL ) {
		List_( Rows( Row ) ).File->ReleaseFile( false );

		if ( Queue_.IsMember( Rows( Row ) ) )
			Queue_.Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

void flsq::ReleaseFiles( id__ ID )
{
qRH
	bch::E_BUNCH( row__ ) Rows;
qRB
	Lock_();

	Rows.Init();

	_Search( ID, Rows );

	_Release( Rows );
qRR
qRT
	Unlock_();
qRE
}


void flsq::ReleaseInactiveFiles_(
	time_t Delay,
	bso::uint__ MaxAmount )
{
	Lock_();

	time_t Now = tol::EpochTime( false );

	while ( MaxAmount-- && ( Queue_.Tail() != qNIL ) && ( ( Now - List_( Queue_.Tail() ).File->AccessEpochTimestamp() ) <= Delay ) ) {
		List_( Queue_.Tail() ).File->ReleaseFile( false );
		Queue_.Delete( Queue_.Tail() );
	}

	Unlock_();
}

/*
void fls::ReleaseAllFiles( void )
{
	Lock_();

	fls::row__ Row = List.First();
	fls::row__ RowBuffer;

	while ( Row != qNIL ) {
		RowBuffer = Row;

		Row = List.Next( Row );

		List( RowBuffer )->ReleaseFile( false );

		if ( Queue.IsMember( RowBuffer ) )
			Queue.Delete( RowBuffer );
	}

	Unlock_();

}
*/

Q37_GCTOR( flsq )
{
	List_.Init();
	Queue_.Init();
	Ids_.Init();

	flsq::MaxFileAmount = FLSQ__MAX_FILE_AMOUNT;

#ifdef FLS__MT
	Mutex_ = mtx::Create( mtx::mProtecting );
#endif
}

Q37_GDTOR( flsq )
{
#ifdef MT
	mtx::Delete( Mutex_ );
#endif
}	
