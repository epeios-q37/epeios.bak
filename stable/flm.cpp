/*
	'flm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'flm' header file ('flm.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define FLM__COMPILATION

#include "flm.h"


#include "lstbch.h"
#include "que.h"
#include "bch.h"

#ifdef FLM__MT
#	include "mtx.h"
#	include "mtk.h"
#endif

using namespace flm;

mdr::size__ flm::MaxFileAmount = FLM__MAX_FILE_AMOUNT;

#ifdef FLM__MT
static mtx::mutex_handler__ Mutex_;
#endif

static inline void Lock_( void )
{
#ifdef FLM__MT
	mtx::Lock( Mutex_ );
#endif
}

static inline void Unlock_( void )
{
#ifdef FLM__MT
	mtx::Unlock( Mutex_ );
#endif
}

static inline bso::bool__ IsLocked_( void )
{
#ifdef FLM__AUTOFLUSH
	return mtx::IsLocked( Mutex_ );
#else
	return true;
#endif
}

struct _data__ {
#ifdef FLM__AUTOFLUSH
	bso::bool__ ToFlush;	// Doit tre 'flush' si  'true'.
#endif
	memoire_fichier_base___ *File;
	id__ ID;
	void reset( bso::bool__ = true )
	{
#ifdef FLM__AUTOFLUSH
		ToFlush = false;
#endif
		File = NULL;
		ID = FLM_UNDEFINED_ID;
	}
};

static lstbch::E_LBUNCHt( _data__, row__ ) List_;
static que::E_MQUEUEt( row__ ) Queue_;

typedef ids::E_IDS_STORE_( id__ )	_ids_;
E_AUTO( _ids );

static _ids	IDs_;

id__ flm::GetId( void )
{
	id__ ID = FLM_UNDEFINED_ID;

	Lock_();

	ID = IDs_.New();

	Unlock_();

	return ID;
}

void flm::ReleaseId( id__ ID )
{
	Lock_();

	IDs_.Release( ID );

	Unlock_();
}

row__ flm::_Register(
	memoire_fichier_base___ &MFB,
	id__ ID )
{
	row__ Row = NONE;
	_data__ Data = {
#ifdef FLM__AUTOFLUSH
		false,
#endif
		&MFB, ID };

	Lock_();

	Row = List_.New();

	if ( Queue_.Amount() < List_.Extent() )	// On teste 'Amount' parce que ce qui est entre 'Amount' et 'Extent' n'est pas initialis dans la file.
		Queue_.Allocate( List_.Extent() );

	List_.Store( Data, Row );

	Unlock_();

	return Row;
}

void flm::_Unregister(
	row__ Row,
	id__ ID )
{
	_data__ Data;

	Lock_();

	if ( List_( Row ).ID != ID )
		ERRu();

	List_.Store( Data, Row );
	List_.Delete( Row );

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );

	Unlock_();
}

#define DELAY	1	// en s.

#ifdef FLM__AUTOFLUSH
struct _flusher_data__
{
	row__ Row;
	time_t LastFileWriteTime;
	 _flusher_data__( void )
	{
		Row = NONE;
		LastFileWriteTime = tol::EpochTime( false );
	}
} FlusherData_;

static inline void Flusher_( void * )
{
	bso::bool__ ToFlush = false;
	_file___ *File = NULL;

	Lock_();

	_data__ Data;

	while ( FlusherData_.Row != NONE ) {

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

		if ( FlusherData_.Row != NONE ) {
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

inline static void LaunchFlusher_( void )
{
#ifdef FLM__AUTOFLUSH
#ifdef FLM_DBG
	if ( !IsLocked_() )
		ERRc();
#endif
	if ( FlusherData_.Row == NONE )
		mtk::Launch( Flusher_, NULL );	// Le verrou est pos, donc ne fait rien tant que l'appelant n'te pas le verrou.

	FlusherData_.Row = Queue_.Last();
#endif
}

inline static void TouchFlusher_( bso::bool__ ToFlush )	// Indique au 'flusher' qu'une criture  eu lieu.
{
#ifdef FLM__AUTOFLUSH
	if ( ToFlush )
		FlusherData_.LastFileWriteTime = tol::EpochTime( false );
#endif
}


void flm::_ReportFileUsing(
	row__ Row,
	bso::bool__ ToFlush )
{
	Lock_();

#ifdef FLM__AUTOFLUSH
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
	else if ( Queue_.Amount() >= FLM__MAX_FILE_AMOUNT ) {
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

void flm::_ReportFileClosing( row__ Row )
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

	while ( Row != NONE ) {
		if ( List_( Row ).ID == ID )
			Rows.Append( Row );

		Row = List_.Next( Row );
	}
}

static void _Release( const bch::E_BUNCH_( row__ ) &Rows )
{
	mdr::row__ Row = Rows.First();

	while ( Row != NONE ) {
		List_( Rows( Row ) ).File->ReleaseFile( false );

		if ( Queue_.IsMember( Rows( Row ) ) )
			Queue_.Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

void flm::ReleaseFiles( id__ ID )
{
ERRProlog
	bch::E_BUNCH( row__ ) Rows;
ERRBegin
	Lock_();

	Rows.Init();

	_Search( ID, Rows );

	_Release( Rows );
ERRErr
ERREnd
	Unlock_();
ERREpilog
}


void flm::ReleaseInactiveFiles_(
	bso::ulong__ Delay,
	bso::ulong__ MaxAmount )
{
	Lock_();

	time_t Now = tol::EpochTime( false );

	while ( MaxAmount-- && ( Queue_.Tail() != NONE ) && ( ( Now - List_( Queue_.Tail() ).File->EpochTimeStamp() ) <= Delay ) ) {
		List_( Queue_.Tail() ).File->ReleaseFile( false );
		Queue_.Delete( Queue_.Tail() );
	}

	Unlock_();
}

/*
void flm::ReleaseAllFiles( void )
{
	Lock_();

	flm::row__ Row = List.First();
	flm::row__ RowBuffer;

	while ( Row != NONE ) {
		RowBuffer = Row;

		Row = List.Next( Row );

		List( RowBuffer )->ReleaseFile( false );

		if ( Queue.IsMember( RowBuffer ) )
			Queue.Delete( RowBuffer );
	}

	Unlock_();

}
*/

Q37_GCTOR( flm )
{
	List_.Init();
	Queue_.Init();
	IDs_.Init();

	flm::MaxFileAmount = FLM__MAX_FILE_AMOUNT;

#ifdef FLM__MT
	Mutex_ = mtx::Create( mtx::mProtecting );
#endif
}

Q37_GDTOR( flm )
{

#ifdef MT
	mtx::Delete( Mutex_ );
#endif
}
