/*
	'mscmdf' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'mscmdf' header file ('mscmdf.h').
	Copyright (C) 2002, 2004 Claude SIMON.

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



//	$Id: mscmdf.cpp,v 1.2 2011/02/13 20:52:16 csimon Exp $

#define MSCMDF__COMPILATION

#include "mscmdf.h"

class mscmdftutor
: public ttr_tutor
{
public:
	mscmdftutor( void )
	: ttr_tutor( MSCMDF_NAME )
	{
#ifdef MSCMDF_DBG
		Version = MSCMDF_VERSION "\b\bD $";
#else
		Version = MSCMDF_VERSION;
#endif
		Owner = MSCMDF_OWNER;
		Date = "$Date: 2011/02/13 20:52:16 $";
	}
	virtual ~mscmdftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mscmdf;

#define HEADER_CHUNK_HEAD "MThd\x00\x00\x00\x06"
#define HEADER_CHUNK_HEAD_SIZE	( sizeof( HEADER_CHUNK_HEAD ) - 1 )

#define TRACK_CHUNK_HEAD "MTrk"
#define TRACK_CHUNK_HEAD_SIZE	( sizeof( TRACK_CHUNK_HEAD ) - 1 )

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

bso::bool__ mscmdf::GetHeaderChunk(
	flw::iflow__ &Flow,
	header_chunk__ &HeaderChunk,
	err::handling__ ErrHandling )
{
	flw::datum__ Buffer[HEADER_CHUNK_HEAD_SIZE];

	Flow.Read( HEADER_CHUNK_HEAD_SIZE, Buffer );

	if ( memcmp( Buffer, HEADER_CHUNK_HEAD, HEADER_CHUNK_HEAD_SIZE ) )
		switch ( ErrHandling ) {
		case err::hThrowException:
			ERRf();
			break;
		case err::hUserDefined:
			return false;
			break;
		default:
			ERRu();
			break;
		}

	HeaderChunk.SMFType = Flow.Get() << 8 | Flow.Get();

	HeaderChunk.TrackChunkAmount = Flow.Get() << 8 | Flow.Get();

	HeaderChunk.DeltaTimeTicks = Flow.Get() << 8 | Flow.Get();

	return true;
}

static void Put_(
	bso::ushort__ Value,
	flw::oflow__ &Flow )
{
	Flow.Put( Value >> 8 );
	Flow.Put( Value & 0xff );
}

void mscmdf::PutHeaderChunk(
	smf_type__ SMFType,
	track_chunk_amount__ TrackChunkAmount,
	delta_time_ticks__ DeltaTimeTicks,
	flw::oflow__ &OFlow )
{
	OFlow.Write( HEADER_CHUNK_HEAD, HEADER_CHUNK_HEAD_SIZE );

	Put_( SMFType, OFlow );
	Put_( TrackChunkAmount, OFlow );
	Put_( DeltaTimeTicks, OFlow );
}

static inline bso::bool__ HandleDeltaTimeTicks_(
	flw::iflow__ &IFlow,
	delta_time_ticks__ &DeltaTimeTicks )
{
	flw::datum__ Datum = IFlow.Get();

	DeltaTimeTicks = ( DeltaTimeTicks << 8 ) | ( Datum & 0x7f );

	return ( Datum & 0x80 ) != 0;
}

track_chunk_size__  mscmdf::GetTrackChunkSize(
	flw::iflow__ &IFlow,
	err::handling__ ErrHandling )
{
	flw::datum__ Buffer[TRACK_CHUNK_HEAD_SIZE];

	IFlow.Read( TRACK_CHUNK_HEAD_SIZE, Buffer );

	if ( memcmp( Buffer, TRACK_CHUNK_HEAD, TRACK_CHUNK_HEAD_SIZE ) )
		switch ( ErrHandling ) {
		case err::hThrowException:
			ERRf();
			break;
		case err::hUserDefined:
			return 0;	// I suppose that no track chunk can have a size of 0.
			break;
		default:
			ERRu();
			break;
		}

	return ( IFlow.Get() << 24 ) | ( IFlow.Get() << 16 )
		                         | ( IFlow.Get() << 8 ) | IFlow.Get();

}

void mscmdf::PutTrackChunkHeader(
	track_chunk_size__ Size,
	flw::oflow__ &OFlow )
{
	OFlow.Write( TRACK_CHUNK_HEAD, TRACK_CHUNK_HEAD_SIZE );

	OFlow.Put( (flw::datum__)( Size >> 24 ) );
	OFlow.Put( (flw::datum__)( ( Size >> 16 ) & 0xff ) );
	OFlow.Put( (flw::datum__)( ( Size >> 8 ) & 0xff ) );
	OFlow.Put( (flw::datum__)( Size & 0xff ) );
}



class mscmdfpersonnalization
: public mscmdftutor
{
public:
	mscmdfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscmdfpersonnalization( void )
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

static mscmdfpersonnalization Tutor;

ttr_tutor &MSCMDFTutor = Tutor;
