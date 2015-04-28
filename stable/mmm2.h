/*
	Header for the 'mmm2' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: mmm2.h,v 1.40 2012/11/14 16:06:31 csimon Exp $

#ifndef MMM2__INC
#define MMM2__INC

#define MMM2_NAME		"MMM2"

#define	MMM2_VERSION	"$Revision: 1.40 $"

#define MMM2_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( MMM2_NODBG )
#define MMM2_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.40 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MultiMeMory v2 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error Do noy use. Use 'MMM' instead.

/* NOTA : Lorsque le fragment est utilis, la taille totale est la taille utilisable (taille des donnes).
Lorsque le fragment est disponible, c'est la taille total du fragment qui est stocke. */

#include "err.h"
#include "flw.h"
#include "epeios.h"
#include "mmm0.h"
#include "uym.h"
#include "bch.h"
#include "stk.h"

// Ce drapeau indique si la taille est stock dans le fragment. Ne concerne que les fragments libres.
#define MMM2_FREE_FRAGMENT_SIZE_FLAG_POSITION	7
#define MMM2_FREE_FRAGMENT_SIZE_FLAG_MASK ( 1 << MMM2_FREE_FRAGMENT_SIZE_FLAG_POSITION )


// Ce drapeau indique si le fragment est li ou non (donne rparties sur deux fragments ou non). Ne concerne que les fragments utiliss.
#define MMM2_USED_FRAGMENT_LINK_FLAG_POSITION	7
#define MMM2_USED_FRAGMENT_LINK_FLAG_MASK ( 1 << MMM2_USED_FRAGMENT_LINK_FLAG_POSITION )

// Ce drapeau indique si le fragment prcdent est libre ou non. Ne concerne que les fragments utiliss.
#define MMM2_USED_FRAGMENT_FREE_FLAG_POSITION	6
#define MMM2_USED_FRAGMENT_FREE_FLAG_MASK ( 1 << MMM2_USED_FRAGMENT_FREE_FLAG_POSITION )

#define MMM2_USED_FRAGMENT_FLAGS_MASK	( MMM2_USED_FRAGMENT_LINK_FLAG_MASK | MMM2_USED_FRAGMENT_FREE_FLAG_MASK )

#define MMM2_L1	((unsigned char)~MMM2_USED_FRAGMENT_FLAGS_MASK )
#define MMM2_L2 ( 0xffffUL + MMM2_L1 )
#define MMM2_L3 0xffffffffUL
#define MMM2_SIZE_BUFFER_MAX_LENGTH	7
#define MMM2_LINK_SIZE	sizeof( mmm::row__ )
#define MMM2_ORPHAN_MAX_SIZE	10
#define MMM2_NORMAL_FREE_FRAGMENT_MIN_SIZE	( 1 + 1 + MMM2_LINK_SIZE + MMM2_LINK_SIZE + MMM2_LINK_SIZE + 1 )
#define MMM2_HEADER_MAX_LENGTH	( 1+ MMM2_SIZE_BUFFER_MAX_LENGTH + 2 * MMM2_LINK_SIZE  )	// Correspond  la struture d'un 'header' d'un fragment libre,
																							// ce dernier tant plus grand que le 'header' d'un fragment utilis.
#define MMM2_FREE_FRAGMENT_FOOTER_MAX_SIZE	( MMM2_LINK_SIZE + 1 )

namespace mmm {
	E_ROW( row__ );

	typedef bso::ubyte__ addendum__;

	class multimemory_
	{
	private:
		multimemory_driver__ _MultimemoryDriver;
		bso::ubyte__ _GetSizeLength( mdr::size__ Size ) const
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( Size < MMM2_L1 )
				return 1;
			else if ( Size < MMM2_L2 )
				return 3;
			else if ( Size <= MMM2_L3 )
				return 7;
			else
				ERRc();

			return 0;	// Pour viter un 'warning'.
		}
		// Retourne la taile maximale telle que cette taille + l'indicateur de taille soit infrieur ou gal  'MaxSize'.
		mdr::size__ _AdjustSize( mdr::size__ MaxSize ) const
		{
#ifdef MMM2_DBG
			if ( MaxSize <= 1 )
				ERRc();
#endif
			mdr::size__ Size = MaxSize - 1;	// au moins un octet pour stocker la taille.

			while ( ( Size + _GetSizeLength( Size ) ) > MaxSize )
				Size--;

			return Size;
		}
		void _PutSize(
			mdr::size__ Size,
			mdr::datum__ *Datum )
		{
			if ( Size < MMM2_L1 ) {
				Datum[0] = ( Datum[0] & MMM2_USED_FRAGMENT_FLAGS_MASK ) | (mdr::datum__)Size;
				return;
			}

			Datum[0] = ( Datum[0] & MMM2_USED_FRAGMENT_FLAGS_MASK ) | ~MMM2_USED_FRAGMENT_FLAGS_MASK;

			Size -= MMM2_L1;

			if ( Size < ( MMM2_L2 - MMM2_L1 ) ) {
				Datum[1] = (mdr::datum__)( Size >> 8 );
				Datum[2] = (mdr::datum__)( Size & 0xff );

				return;
			}

			Datum[1] = 0xff;
			Datum[2] = 0xff;

			Size -= 0xffff;

			if ( Size < ( MMM2_L3 - MMM2_L2 ) ) {
				Datum[3] = (mdr::datum__)( Size >> 24 );
				Datum[4] = (mdr::datum__)( ( Size >> 16 ) & 0xff );
				Datum[5] = (mdr::datum__)( ( Size >> 8 ) & 0xff );
				Datum[6] = (mdr::datum__)( Size & 0xff );
			} else
				ERRc();
		}
		mdr::size__ _GetSize( const mdr::datum__ *Datum ) const
		{	
			mdr::size__ Size = 0;

			Size = Datum[0] & ~MMM2_USED_FRAGMENT_FLAGS_MASK;

			if ( Size == MMM2_L1 ) {
				Size += ( Datum[1] << 8 ) + Datum[2];

				if ( Size == MMM2_L2 ) {
					Size += ( Datum[3] << 24 )
						    + ( Datum[4] << 16 )
							+ ( Datum[5] << 8 )
							+ Datum[6];
				}
			}

			return Size;
		}
		void _GetHeader(
			row__ Position,
			mdr::datum__ *Header,
			mdr::size__ Amount = MMM2_HEADER_MAX_LENGTH ) const
		{
			Memory.Recall( *Position, ( *Position + Amount ) > S_.Extent ? S_.Extent - *Position : Amount, Header );
		}
		bso::bool__ _IsFragmentUsed( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM2_USED_FRAGMENT_FLAGS_MASK ) != 0;
		}
		bso::bool__ _IsFragmentFree( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM2_USED_FRAGMENT_FLAGS_MASK ) == 0;
		}
		mdr::size__ _GetRawSize( const mdr::datum__ *SizeBuffer ) const
		{
			return _GetSize( SizeBuffer );
		}
		void _SetRawSize(
			mdr::size__ Size,
			bso::bool__ LinkFlag,
			bso::bool__ FreeFlag,
			mdr::datum__ *SizeBuffer )
		{
			SizeBuffer[0] = ( LinkFlag << MMM2_USED_FRAGMENT_LINK_FLAG_POSITION ) | ( FreeFlag << MMM2_USED_FRAGMENT_FREE_FLAG_POSITION );

			_PutSize( Size, SizeBuffer );
		}
		void _SetRawSize(
			mdr::size__ Size,
			row__ Position,
			bso::bool__ LinkFlag,
			bso::bool__ FreeFlag )
		{
			mdr::datum__ SizeBuffer[MMM2_SIZE_BUFFER_MAX_LENGTH];

			_SetRawSize( Size, LinkFlag, FreeFlag, SizeBuffer );

			Memory.Store( SizeBuffer, _GetSizeLength( Size ), *Position );
		}
		bso::bool__ _IsUsedFragmentFreeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_USED_FRAGMENT_FREE_FLAG_MASK ) != 0;
		}
		void _SetUsedFragmentFreeFlag(
			row__ Position,
			const mdr::datum__ *Header,
			bso::bool__ Value )
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			Memory.Put( ( Header[0] & ~MMM2_USED_FRAGMENT_FREE_FLAG_MASK ) | ( Value << MMM2_USED_FRAGMENT_FREE_FLAG_POSITION ), *Position );
		}
		void _SetUsedFragmentFreeFlag(
			row__ Position,
			bso::bool__ Value )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetUsedFragmentFreeFlag( Position, Header, Value );
		}
		bso::bool__ _IsUsedFragmentLinkFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_USED_FRAGMENT_LINK_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsFreeFragmentSizeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_FREE_FRAGMENT_SIZE_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsUsedFragmentLinked( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _IsUsedFragmentLinkFlagSet( Header );
		}
		mdr::size__ _GetFreeFragmentSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();
#endif
			if ( Header[0] == 0 )
				return 1;
			else
				return _GetRawSize( Header + 1 );
		}
		mdr::size__ _GetFreeFragmentSize( row__ Position ) const
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			return _GetFreeFragmentSize( Header );
		}
		mdr::size__ _GetFreeFragmentHeaderLength( const mdr::datum__ *Header ) const
		{
			mdr::size__ Size = _GetFreeFragmentSize( Header );

			if ( Size == 1 )
				return 1;
			else if ( Size <= MMM2_ORPHAN_MAX_SIZE )
				return _GetSizeLength( Size ) + 1;
			else
				return _GetSizeLength( Size ) + 1 + MMM2_LINK_SIZE + MMM2_LINK_SIZE;
		}
		bso::bool__ _IsFreeFragmentOrphan( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentSize( Header ) <= MMM2_ORPHAN_MAX_SIZE;
		}
		row__ _GetUsedFragmentLink(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			row__ Link = NONE;
#ifdef MMM2_DBG
			if ( !_IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			Memory.Recall( *Position + _GetUsedFragmentTotalSize( Header ) - MMM2_LINK_SIZE, MMM2_LINK_SIZE, (mdr::datum__ *)&Link );

			return Link;
		}
		row__ _GetUsedFragmentNextFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetUsedFragmentTotalSize( Header );
		}
		row__ _GetFreeFragmentNextFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetFreeFragmentSize( Header );
		}
		mdr::size__ _GetFreeFragmentFreeFragmentPointersRelativePosition( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();

			if ( _IsFreeFragmentOrphan( Header ) )
				ERRc();
#endif
			return 1 + _GetSizeLength( _GetFreeFragmentSize( Header ) );
		}
		mdr::size__ _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentFreeFragmentPointersRelativePosition( Header );
		}
		mdr::size__ _GetFreeFragmentNextFreeFragmentPointerRelativePosition( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ) + MMM2_LINK_SIZE;
		}
		void _MarkAsFreeFragment(
			row__ Position,
			mdr::size__ Size,
			row__ PreviousFragmentPosition,
			row__ NextFragmentPosition )
		{
			if ( Size == 0 )
				ERRc();
			else if ( Size == 1 )
				Memory.Put( 0, *Position );
			else {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				Header[0] = MMM2_FREE_FRAGMENT_SIZE_FLAG_MASK;

				_SetRawSize( Size, false, false, Header + 1 );

				if ( Size <= MMM2_ORPHAN_MAX_SIZE )
					Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );	// Marqueur  destination du fragement utilis qui suit.
				else {
					if ( Size < MMM2_NORMAL_FREE_FRAGMENT_MIN_SIZE )
						Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );	// Marqueur  destination du fragement utilis qui suit.
					else {
						Memory.Put( 0xff, *Position + Size - 1 );	// Marqueur  destination du fragement utilis qui suit.
						Memory.Store( (const mdr::datum__ *)&Position, MMM2_LINK_SIZE, *Position + Size - 5 );	// Position du dbut du fragment  destination du fragment utilis qui suit.
					}
					// Bien que 'Header' soit en cours de construction, son contenu est suffisant pour pouvoir l'utiliser.
					memcpy( Header + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ), &PreviousFragmentPosition, MMM2_LINK_SIZE );
					memcpy( Header + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header ), &NextFragmentPosition, MMM2_LINK_SIZE );
				}

				Memory.Store( Header, _GetFreeFragmentHeaderLength( Header ), *Position );
			}
		}
		mdr::size__ _GetUsedFragmentDataSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _GetRawSize( Header );
		}
		mdr::size__ _GetUsedFragmentTotalSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			mdr::size__ DataSize = _GetUsedFragmentDataSize( Header );

			return DataSize + _GetSizeLength( DataSize ) + ( _IsUsedFragmentLinked( Header ) ? MMM2_LINK_SIZE : 0 );
		}
		row__ _GetFreeFragmentPosition(
			row__ AfterFreeFragmentFooterPosition,	// Position immedialty after the free fragment footer.
			const mdr::datum__ *FreeFragmentTail ) const
		{
			const mdr::datum__ *Pointer = FreeFragmentTail + MMM2_FREE_FRAGMENT_FOOTER_MAX_SIZE - 1;

			if ( *Pointer == 0 )
				return *AfterFreeFragmentFooterPosition - 1;
			else if ( *Pointer != 0xff )
				return *AfterFreeFragmentFooterPosition - *Pointer;
			else
				return *(row__ *)( Pointer - MMM2_LINK_SIZE );
		}
		void _GetFreeFragmentFooter(
			row__ AfterFreeFragmentFooterPosition,	// Position immedialty after the free fragment footer.
			mdr::datum__ *FreeFragmentFooter ) const
		{
#ifdef MMM2_DBG
			if ( AfterFreeFragmentFooterPosition == 0 )
				ERRc();
#endif

			mdr::size__ Amount = MMM2_FREE_FRAGMENT_FOOTER_MAX_SIZE;

			if ( *AfterFreeFragmentFooterPosition < Amount )
				Amount = *AfterFreeFragmentFooterPosition;

			Memory.Recall( *AfterFreeFragmentFooterPosition - Amount, Amount, FreeFragmentFooter + MMM2_FREE_FRAGMENT_FOOTER_MAX_SIZE - Amount );
		}

		row__ _GetFreeFragmentPosition( row__ AfterFreeFragmentFooterPosition ) const	// Position immedialty after the free fragment footer.
		{
			mdr::datum__ FreeFragmentFooter[MMM2_FREE_FRAGMENT_FOOTER_MAX_SIZE];

			_GetFreeFragmentFooter( AfterFreeFragmentFooterPosition, FreeFragmentFooter );

			return _GetFreeFragmentPosition( AfterFreeFragmentFooterPosition, FreeFragmentFooter );
		}
		row__ _GetFreeFragmentPreviousFreeFragmentPointerPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header );
		}
		row__ _GetFreeFragmentNextFreeFragmentPointerPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header );
		}
		row__ _GetFreeFragmentPreviousFreeFragmentPosition( const mdr::datum__ *Header ) const
		{
			row__ Row;

			memcpy( &Row, Header + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ), MMM2_LINK_SIZE );

			return Row;
		}
		void _SetFreeFragmentPreviousFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			row__ PreviousFragmentPosition )
		{
			Memory.Store( (const mdr::datum__ *)&PreviousFragmentPosition, MMM2_LINK_SIZE, *_GetFreeFragmentPreviousFreeFragmentPointerPosition( Position, Header ) );
		}
		void _SetFreeFragmentPreviousFreeFragmentPosition(
			row__ Position,
			row__ PreviousFragmentPosition )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetFreeFragmentPreviousFreeFragmentPosition( Position, Header, PreviousFragmentPosition );
		}
		row__ _GetFreeFragmentNextFreeFragmentPosition( const mdr::datum__ *Header ) const
		{
			row__ Row;

			memcpy( &Row, Header + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header ), MMM2_LINK_SIZE );

			return Row;
		}
		void _SetFreeFragmentNextFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			row__ NextFragmentPosition )
		{
			Memory.Store( (const mdr::datum__ *)&NextFragmentPosition, MMM2_LINK_SIZE, *_GetFreeFragmentNextFreeFragmentPointerPosition( Position, Header ) );
		}
		void _SetFreeFragmentNextFreeFragmentPosition(
			row__ Position,
			row__ NextFragmentPosition )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetFreeFragmentNextFreeFragmentPosition( Position, Header, NextFragmentPosition );
		}
		void _UpdateFreeFragmentMarker(
			row__ RemovedFragmentPosition,
			const mdr::datum__ *Header )
		{
#ifdef MMM2_DBG
			if  ( _IsFreeFragmentOrphan( Header ) )
				ERRc();
#endif
			if ( S_.FreeFragment == RemovedFragmentPosition )
				if ( *RemovedFragmentPosition & 1 )	// Petit gnrateur al&atoire.
					S_.FreeFragment = _GetFreeFragmentNextFreeFragmentPosition( Header );
				else
					S_.FreeFragment = _GetFreeFragmentPreviousFreeFragmentPosition( Header );
		}
		void _ExciseFreeFragment(
			row__ RemovedFragmentPosition,
			const mdr::datum__ *Header )
		{
			_UpdateFreeFragmentMarker( RemovedFragmentPosition, Header );

			row__
					Previous = _GetFreeFragmentPreviousFreeFragmentPosition( Header ),
					Next = _GetFreeFragmentNextFreeFragmentPosition( Header );
			
			if ( Previous != NONE ) {
#ifdef MMM2_DBG
				if ( Next == NONE )
					ERRc();
#endif

				if ( Previous == Next ) {	// Il ne reste plus q'un fragment libre (non orphelin) ; on ne va pas le faire pointer sur lui-mme.
					_SetFreeFragmentNextFreeFragmentPosition( Previous, NONE );
					_SetFreeFragmentPreviousFreeFragmentPosition( Next, NONE );
				} else {
					_SetFreeFragmentNextFreeFragmentPosition( Previous, Next );
					_SetFreeFragmentPreviousFreeFragmentPosition( Next, Previous );
				}
			}
		}
		void _ExciseFreeFragment( row__ RemovedFreeFragmentPosition )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( RemovedFreeFragmentPosition, Header );

			_ExciseFreeFragment( RemovedFreeFragmentPosition, Header );
		}
		void _ReportNewFreeFragment(
			row__ NewFreeFragmentPosition,
			row__ &PreviousFreeFragmentPosition,
			row__ &NextFreeFragmentPosition )
		{
			if ( S_.FreeFragment != NONE ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				_GetHeader( S_.FreeFragment, Header );

				PreviousFreeFragmentPosition = S_.FreeFragment;
				NextFreeFragmentPosition = _GetFreeFragmentNextFreeFragmentPosition( Header );

				_SetFreeFragmentNextFreeFragmentPosition( S_.FreeFragment, NewFreeFragmentPosition );

				if ( NextFreeFragmentPosition != NONE )
					_SetFreeFragmentPreviousFreeFragmentPosition( NextFreeFragmentPosition, NewFreeFragmentPosition );
				else {
					NextFreeFragmentPosition = S_.FreeFragment;
					_SetFreeFragmentPreviousFreeFragmentPosition( S_.FreeFragment, NewFreeFragmentPosition );
				}

			} else {
				PreviousFreeFragmentPosition = NextFreeFragmentPosition = NONE;
			}

			S_.FreeFragment =  NewFreeFragmentPosition;
		}
		void _SetAsFreeFragment(
			row__ Position,	// 'Position' ne pointe pas ncessairement sur un 'header'.
			mdr::size__ Size )
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( ( *Position + Size ) != S_.Extent ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( *Position + Size, Header );

				if ( _IsFragmentFree( Header ) ) {
					if ( !_IsFreeFragmentOrphan( Header ) )
						_ExciseFreeFragment( *Position + Size, Header );

					if ( ( *Position + Size ) == *S_.TailingFreeFragmentPosition )
						S_.TailingFreeFragmentPosition = Position;

					Size += _GetFreeFragmentSize( Header );

				} else if ( _IsFragmentUsed( Header ) ) {
					if ( !_IsUsedFragmentFreeFlagSet( Header ) )
						_SetUsedFragmentFreeFlag( *Position + Size, Header, true );
				} else
					ERRc();
			} else
				S_.TailingFreeFragmentPosition = Position;

			if  ( Size > MMM2_ORPHAN_MAX_SIZE ) {
				row__ PreviousFreeFragmentPosition = NONE, NextFreeFragmentPosition = NONE;

				if ( ( *Position + Size ) != S_.Extent	)	// If the freed fragment becomes the last one, it is not included in the list, because it is handled otherwher else.
					_ReportNewFreeFragment( Position, PreviousFreeFragmentPosition, NextFreeFragmentPosition );

				_MarkAsFreeFragment( Position, Size, PreviousFreeFragmentPosition, NextFreeFragmentPosition );
			} else
				_MarkAsFreeFragment( Position, Size, NONE, NONE );
		}
/*
		row__ _FreeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ Size )	// Retourne la position du fragment libre ainsi cre.Peut tre diffrent de 'Position' (fusion avec le fragment libre prcdent).
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif

			if ( _IsUsedFragmentFreeFlagSet( Header ) ) {
				mdr::datum__ TempHeader[MMM2_HEADER_MAX_LENGTH];
				Position = _GetUsedFragmentPreviousFreeFragmentPosition( Position, Header );
				_GetHeader( Position, TempHeader );

				if ( !_IsFreeFragmentOrphan( TempHeader ) )
					_ExciseFreeFragment( Position, TempHeader );

				Size += _GetFreeFragmentSize( TempHeader );
			}

			_SetAsFreeFragment( Position, Size );

			return Position;
		}
*/		mdr::size__ _GuessTotalSizeForUsedFragment(
			mdr::size__ DataSize,
			bso::bool__ WithLink ) const
		{
			return _GetSizeLength( DataSize ) + DataSize + ( WithLink ? MMM2_LINK_SIZE : 0 );
		}
		mdr::size__ _GuessTotalSizeForUsedFragment(
			mdr::size__ DataSize,
			row__ Link ) const
		{
			return _GuessTotalSizeForUsedFragment( DataSize, Link != NONE );
		}
		void _MarkAsUsedFragment(
			row__ Position,
			mdr::size__ DataSize,
			row__ Link,	// Si pas li, est gal  'NONE'.
			bso::bool__ FreeFlag )
		{
#ifdef MMM2_DBG
			if ( DataSize == 0 )
				ERRc();
#endif
			_SetRawSize( DataSize, Position, ( Link != NONE ), FreeFlag );

			if ( Link != NONE )
				Memory.Store( (const mdr::datum__ *)&Link, MMM2_LINK_SIZE, *Position + _GetSizeLength( DataSize ) + DataSize );
		}
		void _ConvertFreeFragmentToUsedFragment(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			row__ Link )
		{
			mdr::size__ Size = _GuessTotalSizeForUsedFragment( DataSize, Link );
			row__ RemainderPosition = *Position + Size;
			mdr::size__ FragmentCurrentSize = _GetFreeFragmentSize( Header );
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();

			if ( *RemainderPosition > S_.Extent )
				ERRc();
#endif
			_ExciseFreeFragment( Position, Header );

			if ( RemainderPosition != S_.Extent )
				if ( Size != FragmentCurrentSize )
					_SetAsFreeFragment( RemainderPosition, FragmentCurrentSize - Size );
				else
					_SetUsedFragmentFreeFlag( RemainderPosition, false );	// 'RemainderPosition' is a used fragment, then report him
																			// that he is no more preceded by a free fragment.

			_MarkAsUsedFragment( Position, DataSize, Link, false );
		}
		mdr::size__ _GuessDataSize(
			mdr::size__ TotalSize,
			bso::bool__ Linked ) const
		{
			return _AdjustSize( TotalSize - ( Linked ? MMM2_LINK_SIZE : 0 ) );
		}
		mdr::size__ _GuessFreeFragmentAvailableSize(
			const mdr::datum__ *Header,
			bso::bool__ Linked ) const
		{
#ifdef MMM2_DBG
			if ( _IsFreeFragmentOrphan( Header ) )
				ERRc();
#endif
			return _GuessDataSize( _GetFreeFragmentSize( Header ), Linked );
		}
		bso::bool__ _IsFreeFragmentBigEnough(
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			bso::bool__ Linked ) const
		{
			return ( _GuessFreeFragmentAvailableSize( Header, Linked ) >= DataSize );
		}
		bso::bool__ _IsFreeFragmentBigEnough(
			row__ Position,
			mdr::size__ DataSize,
			bso::bool__ Linked ) const
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			return _IsFreeFragmentBigEnough( Header, DataSize, Linked );
		}
		row__ _AppendNewFragment( mdr::size__ Size )
		{
			row__ Row = NONE;

			if ( S_.TailingFreeFragmentPosition != NONE ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				mdr::size__ FreeFragmentSize = 0;

				Row = S_.TailingFreeFragmentPosition;

				_GetHeader( Row, Header );

				FreeFragmentSize = _GetFreeFragmentSize( Header );

				if ( FreeFragmentSize > Size ) {
					S_.TailingFreeFragmentPosition = *Row + Size;
					_SetAsFreeFragment( S_.TailingFreeFragmentPosition, FreeFragmentSize - Size );
				} else if ( FreeFragmentSize <= Size ) {
					if ( FreeFragmentSize != Size ) {
						S_.Extent += Size - FreeFragmentSize;

						Memory.Allocate( S_.Extent );
					}

					S_.TailingFreeFragmentPosition = NONE;
				} else
					ERRc();

			} else {
				Row = S_.Extent;

				S_.Extent += Size;

				Memory.Allocate( S_.Extent );
			}

			return Row;

		}
		row__ _AppendNewUnlinkedUsedFragment( mdr::size__ DataSize )
		{
			row__ Row = _AppendNewFragment( _GuessTotalSizeForUsedFragment( DataSize, false ) );

			_SetRawSize( DataSize, Row, false, false );

			return Row;
		}
		row__ _AllocateRetrievingFreeFragments(
			mdr::size__ DataSize,
			addendum__ &Addendum )
		{
			row__ FirstFragmentPosition = _GetFreeFragment(), SecondFragmentPosition = NONE;
			mdr::datum__ FirstFragmentHeader[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( FirstFragmentPosition, FirstFragmentHeader );

			if ( _IsFreeFragmentBigEnough( FirstFragmentHeader, DataSize, false ) ) {
				_ConvertFreeFragmentToUsedFragment( FirstFragmentPosition, FirstFragmentHeader, DataSize, NONE );

				Addendum = _GetSizeLength( DataSize );

				return FirstFragmentPosition;
			}

			_ExciseFreeFragment( FirstFragmentPosition, FirstFragmentHeader );

			Addendum = 0;

			mdr::size__ FirstFragmentDataSize = _GuessFreeFragmentAvailableSize( FirstFragmentHeader, true );

			if ( _IsFreeFragmentAvailable() ) {
				SecondFragmentPosition = _GetFreeFragment();
				mdr::datum__ SecondFragmentHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( SecondFragmentPosition, SecondFragmentHeader );

				if ( _IsFreeFragmentBigEnough( SecondFragmentHeader, DataSize - FirstFragmentDataSize, false ) ) {
					_ConvertFreeFragmentToUsedFragment( FirstFragmentPosition, FirstFragmentHeader, FirstFragmentDataSize, SecondFragmentPosition );

					_ConvertFreeFragmentToUsedFragment( SecondFragmentPosition, SecondFragmentHeader, DataSize - FirstFragmentDataSize, NONE );

					return FirstFragmentPosition;
				}
			}

			SecondFragmentPosition = _AppendNewUnlinkedUsedFragment( DataSize - FirstFragmentDataSize );

			_ConvertFreeFragmentToUsedFragment( FirstFragmentPosition, FirstFragmentHeader, FirstFragmentDataSize, SecondFragmentPosition );

			return FirstFragmentPosition;
		}
/*		row__ _GetNewUnlinkedFreeFragment( mdr::size__ DataSize )
		{
			row__ Row = NONE;

			if ( _IsFreeFragmentAvailable() ) {

				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				Row = _GetFreeFragment();

				_GetHeader( Row, Header );
			
				if ( _IsFreeFragmentBigEnough( Header, DataSize ) )
					_ConvertFreeFragmentToUsedFragment( Row, Header, DataSize, NONE );
				else
					Row = _AppendNewUnlinkedUsedFragment( DataSize );
			} else
				Row = _AppendNewUnlinkedUsedFragment( DataSize );

			return Row;
		}
*/		descriptor__ _Allocate(
			mdr::size__ Size,
			addendum__ &Addendum )
		{	
			if ( _IsFreeFragmentAvailable() )
				return *_AllocateRetrievingFreeFragments( Size, Addendum );
			else {
				Addendum = _GetSizeLength( Size );

				return *_AppendNewUnlinkedUsedFragment( Size );
			}
		}
		/* Free used fragment and merge with possibly preceding and following free fragments. */
		void _FreeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header )
		{
			if ( _IsUsedFragmentFreeFlagSet( Header ) ) {
				row__ FreeFragmentPosition = _GetFreeFragmentPosition( Position );

				mdr::datum__ FreeFragmentHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( FreeFragmentPosition, FreeFragmentHeader );

				if ( !_IsFreeFragmentOrphan( FreeFragmentHeader ) )
					_ExciseFreeFragment( FreeFragmentPosition, FreeFragmentHeader );

				_SetAsFreeFragment( FreeFragmentPosition, _GetFreeFragmentSize( FreeFragmentHeader ) + _GetUsedFragmentTotalSize( Header ) );
			} else
				_SetAsFreeFragment( Position, _GetUsedFragmentTotalSize( Header ) );
		}
		void _Free( row__ Position )
		{
			if ( Position == MMM_UNDEFINED_DESCRIPTOR )
				return;

			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Position, Header );
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				_FreeUsedFragment( Link, LinkHeader );

				_GetHeader( Position, Header );	// Retrieving the header, because it was perhaps modified by above function.
			}

			_FreeUsedFragment( Position, Header );
		}
		void _HandleResizedUsedFragmentHeader(
			row__ Position,	// NOTA : Can be a linked fragment, but the fragment becomes unlinked.
			const mdr::datum__ *Header,
			mdr::size__ OldDataSize,
			mdr::size__ NewDataSize,
			addendum__ &Addendum )	// Handles shrinking and growing used fragment.
		{
			mdr::size__
				OldSizeLength = _GetSizeLength( OldDataSize ),
				NewSizeLength = _GetSizeLength( NewDataSize );

			if ( OldSizeLength != NewSizeLength )
				Memory.Store_( Memory, OldDataSize, *Position + NewSizeLength, *Position + OldSizeLength );

			_SetRawSize( NewDataSize, Position, false, _IsUsedFragmentFreeFlagSet( Header ) );

			Addendum = _GetSizeLength( NewDataSize );
		}
		void _ShrinkUsedFragment(
			row__ Position,	// NOTA : Can be a linked fragment, but the fragment becomes unlinked.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )
		{
#ifdef MMM2_DBG
			if ( DataSize > _GetUsedFragmentDataSize( Header ) )
				ERRc();
#endif
			_HandleResizedUsedFragmentHeader( Position, Header, _GetUsedFragmentDataSize( Header ), DataSize, Addendum );

			_SetAsFreeFragment( *Position + _GetSizeLength( DataSize ) + DataSize, _GetUsedFragmentTotalSize( Header ) - _GetSizeLength( DataSize ) - DataSize );
		}
		row__ _Shrink(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )
		{
			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor, Header );

				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				if ( DataSize > _GetUsedFragmentDataSize( Header ) ) {
					_ShrinkUsedFragment( Link, LinkHeader, DataSize - _GetUsedFragmentDataSize( Header ), Addendum );

					Addendum = 0;
				} else {
					mdr::datum__ NewHeader[MMM2_HEADER_MAX_LENGTH];
					_FreeUsedFragment( Link, LinkHeader );

					_GetHeader( Descriptor, NewHeader );	// We retrieve the header, because it was perhaps modified by above method.

					_ShrinkUsedFragment( Descriptor, NewHeader, DataSize, Addendum );
				}
			} else {
				_ShrinkUsedFragment( Descriptor, Header, DataSize, Addendum );
			}

			return Descriptor;
		}
		mdr::size__ _Size(
			descriptor__ Descriptor,
			const mdr::datum__ *Header ) const
		{
			mdr::size__ Size = _GetUsedFragmentDataSize( Header );

			if ( _IsUsedFragmentLinked( Header ) ) {
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( _GetUsedFragmentLink( Descriptor, Header ), LinkHeader );

				Size += _GetUsedFragmentDataSize( LinkHeader );

#ifdef MMM2_DBG
				if ( _IsUsedFragmentLinked( LinkHeader ) )
					ERRc();
#endif
			}

			return Size;
		}
		void _MoveHead(
			row__ SourceFirstFragmentRow,
			const mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			const mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			const mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			const mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les diffrents 'row' pointent directement sur les donnes des fragment respectifs (aprs l'indicateur de taille),
			et non pas sur le dbut des fragments respectis. */
		{
			Memory.Store_(
				Memory, SourceFirstFragmentSize > TargetFirstFragmentSize ? TargetFirstFragmentSize : SourceFirstFragmentSize,
				*TargetFirstFragmentRow, *SourceFirstFragmentRow );

			if ( ( SourceSecondFragmentSize != 0 ) && ( ( SourceFirstFragmentSize + SourceSecondFragmentSize ) < TargetFirstFragmentSize ) )
				Memory.Store_(
					Memory, SourceSecondFragmentSize,
					*TargetFirstFragmentRow + SourceFirstFragmentSize,
					*SourceSecondFragmentRow );
		}
		void _MoveMiddle(
			row__ SourceFirstFragmentRow,
			const mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			const mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			const mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			const mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les diffrents 'row' pointent directement sur les donnes des fragment respectifs (aprs l'indicateur de taille),
			et non pas sur le dbut des fragments respectis. */
		{
			if ( SourceFirstFragmentSize > TargetFirstFragmentSize )
				Memory.Store_(
					Memory, SourceFirstFragmentSize - TargetFirstFragmentSize,
					*TargetSecondFragmentRow, *SourceFirstFragmentRow + TargetFirstFragmentSize );
			else
				Memory.Store_(
					Memory, TargetFirstFragmentSize - SourceFirstFragmentSize, 
					*TargetFirstFragmentRow + SourceFirstFragmentSize,
					*SourceSecondFragmentRow );
		}
		void _MoveTail(
			row__ SourceFirstFragmentRow,
			const mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			const mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			const mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			const mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les diffrents 'row' pointent directement sur les donnes des fragment respectifs (aprs l'indicateur de taille),
			et non pas sur le dbut des fragments respectis. */
		{
			if ( SourceFirstFragmentSize > TargetFirstFragmentSize )
				Memory.Store_(
					Memory, SourceSecondFragmentSize,
					*TargetSecondFragmentRow + SourceFirstFragmentSize - TargetFirstFragmentSize,
					*SourceSecondFragmentRow );
			else
				Memory.Store_(
					Memory, SourceSecondFragmentSize + SourceFirstFragmentSize - TargetFirstFragmentSize,
					*TargetSecondFragmentRow,
					*SourceSecondFragmentRow + TargetFirstFragmentSize - SourceFirstFragmentSize );
		}
		void _Move(
			row__ SourceFirstFragmentRow,
			mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les diffrents 'row' pointent directement sur les donnes des fragment respectifs (aprs l'indicateur de taille),
			et non pas sur le dbut des fragments respectis. */
		{
			_MoveHead(
				SourceFirstFragmentRow, SourceFirstFragmentSize,
				SourceSecondFragmentRow, SourceSecondFragmentSize,
				TargetFirstFragmentRow, TargetFirstFragmentSize,
				TargetSecondFragmentRow, TargetSecondFragmentSize );

			if ( ( SourceFirstFragmentSize != TargetFirstFragmentSize ) && ( ( SourceFirstFragmentSize + SourceSecondFragmentSize ) > TargetFirstFragmentSize ) )
				_MoveMiddle(
					SourceFirstFragmentRow, SourceFirstFragmentSize,
					SourceSecondFragmentRow, SourceSecondFragmentSize,
					TargetFirstFragmentRow, TargetFirstFragmentSize,
					TargetSecondFragmentRow, TargetSecondFragmentSize );

			if ( ( SourceFirstFragmentSize  + SourceSecondFragmentSize ) > TargetFirstFragmentSize )
				_MoveTail(
					SourceFirstFragmentRow, SourceFirstFragmentSize,
					SourceSecondFragmentRow, SourceSecondFragmentSize,
					TargetFirstFragmentRow, TargetFirstFragmentSize,
					TargetSecondFragmentRow, TargetSecondFragmentSize );
		}
		void _Move(
			row__ SourceFirstFragmentRow,
			const mdr::datum__ *SourceFirstFragmentHeader,
			row__ TargetFirstFragmentRow,
			const mdr::datum__ *TargetFirstFragmentHeader )
		{
			mdr::datum__ SourceSecondFragmentHeader[MMM2_HEADER_MAX_LENGTH], TargetSecondFragmentHeader[MMM2_HEADER_MAX_LENGTH];
			row__ SourceSecondFragmentRow = NONE, TargetSecondFragmentRow = NONE;
			mdr::size__
				SourceFirstFragmentSize = _GetUsedFragmentDataSize( SourceFirstFragmentHeader ), SourceSecondFragmentSize = 0,
				TargetFirstFragmentSize = _GetUsedFragmentDataSize( TargetFirstFragmentHeader ), TargetSecondFragmentSize = 0;

			if ( _IsUsedFragmentLinked( SourceFirstFragmentHeader ) ) {
				SourceSecondFragmentRow = _GetUsedFragmentLink( SourceFirstFragmentRow, SourceFirstFragmentHeader );
				_GetHeader( SourceSecondFragmentRow, SourceSecondFragmentHeader );
				SourceSecondFragmentSize = _GetUsedFragmentDataSize( SourceSecondFragmentHeader );
			}

			if ( _IsUsedFragmentLinked( TargetFirstFragmentHeader ) ) {
				TargetSecondFragmentRow = _GetUsedFragmentLink( TargetFirstFragmentRow, TargetFirstFragmentHeader );
				_GetHeader( TargetSecondFragmentRow, TargetSecondFragmentHeader );
				TargetSecondFragmentSize = _GetUsedFragmentDataSize( TargetSecondFragmentHeader );
			}

			_Move(
				*SourceFirstFragmentRow + _GetSizeLength( SourceFirstFragmentSize ), SourceFirstFragmentSize,
				*SourceSecondFragmentRow + ( SourceSecondFragmentSize ? _GetSizeLength( SourceSecondFragmentSize ) : 0 ), SourceSecondFragmentSize,
				*TargetFirstFragmentRow + _GetSizeLength( TargetFirstFragmentSize ), TargetFirstFragmentSize,
				*TargetSecondFragmentRow + ( TargetSecondFragmentSize ? _GetSizeLength( TargetSecondFragmentSize ) : 0 ), TargetSecondFragmentSize );
		}
		void _Move(
			row__ SourceFirstFragmentRow,
			const mdr::datum__ *SourceFirstFragmentHeader,
			row__ TargetFirstFragmentRow )
		{
			mdr::datum__ TargetFirstFragmentHeader[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( TargetFirstFragmentRow, TargetFirstFragmentHeader );

			_Move( SourceFirstFragmentRow, SourceFirstFragmentHeader, TargetFirstFragmentRow, TargetFirstFragmentHeader );
		}
		void _ExtendUsedFragmentNotFollowedByAnyFragment(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )	// The fragment must not be a  linked fragment.
		{
#ifdef MMM2_DBG
			if ( ( *Descriptor + _GetUsedFragmentTotalSize( Header ) ) != S_.Extent )
				ERRc();

			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			S_.Extent += _GuessTotalSizeForUsedFragment( DataSize, false ) - _GetUsedFragmentTotalSize( Header );

			Memory.Allocate( S_.Extent );

			_HandleResizedUsedFragmentHeader( Descriptor, Header, _GetUsedFragmentDataSize( Header ), DataSize, Addendum );
		}
		void _ExtendUsedFragmentFollowedByUndersizedTailingFreeFragment(
			row__ Descriptor,	// The fragment must not be linked.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			row__ NextFragmentPosition,
			const mdr::datum__ *NextFragmentHeader,
			addendum__ &Addendum )
		{
#ifdef MMM2_DBG
			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();

			if ( NextFragmentPosition != S_.TailingFreeFragmentPosition )
				ERRc();
#endif
			S_.TailingFreeFragmentPosition = NONE;

			S_.Extent = *Descriptor + _GetUsedFragmentTotalSize( Header );	// This allows to use following method.

			_ExtendUsedFragmentNotFollowedByAnyFragment( Descriptor, Header, DataSize, Addendum );
		}
		void _ExtendUsedFragmentFollowedByBigEnoughFreeFragment(
			row__ Descriptor,	// The fragment must not be linked.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			row__ NextFragmentPosition,	// It's assumed that this fragment is big enough to allow the extending.
			const mdr::datum__ *NextFragmentHeader,
			addendum__ &Addendum )
		{
#ifdef MMM2_DBG
			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			if ( NextFragmentPosition == S_.TailingFreeFragmentPosition )
				S_.TailingFreeFragmentPosition = NONE;
			else if ( !_IsFreeFragmentOrphan( NextFragmentHeader ) )
				_ExciseFreeFragment( NextFragmentPosition, NextFragmentHeader );

			mdr::size__ Delta = _GuessTotalSizeForUsedFragment( DataSize, false ) - _GetUsedFragmentTotalSize( Header );

			if ( Delta < _GetFreeFragmentSize( NextFragmentHeader ) )
				_SetAsFreeFragment( *NextFragmentPosition + Delta, _GetFreeFragmentSize( NextFragmentHeader ) - Delta );
			else if ( Delta == _GetFreeFragmentSize( NextFragmentHeader ) ) {
				if ( ( *NextFragmentPosition + Delta ) < S_.Extent )
					_SetUsedFragmentFreeFlag( *NextFragmentPosition + Delta, false );
#ifdef MMM2_DBG
				else if ( ( *NextFragmentPosition + Delta ) > S_.Extent )
					ERRc();
#endif
			} else if ( Delta > _GetFreeFragmentSize( NextFragmentHeader ) )
				ERRc();

			_HandleResizedUsedFragmentHeader( Descriptor, Header, _GetUsedFragmentDataSize( Header ), DataSize, Addendum );
		}
		bso::bool__ _TryToExtendUsedFragmentFollowedByFreeFragmentWithoutMoving(
			row__ Descriptor,	// The fragment must not be linked.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			row__ NextFragmentPosition,
			const mdr::datum__ *NextFragmentHeader,
			addendum__ &Addendum )
		{
#ifdef MMM2_DBG
			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();

			if ( !_IsFragmentFree( NextFragmentHeader ) )
				ERRc();
#endif
			mdr::size__ TotalSize = _GetUsedFragmentTotalSize( Header ) + _GetFreeFragmentSize( NextFragmentHeader );

			if ( _GuessTotalSizeForUsedFragment( DataSize, false ) <= TotalSize )
				_ExtendUsedFragmentFollowedByBigEnoughFreeFragment( Descriptor, Header, DataSize, NextFragmentPosition, NextFragmentHeader, Addendum );
			else if ( NextFragmentPosition == S_.TailingFreeFragmentPosition )
				_ExtendUsedFragmentFollowedByUndersizedTailingFreeFragment( Descriptor, Header, DataSize, NextFragmentPosition, NextFragmentHeader, Addendum );
			else
				return false;

			return true;
		}
		bso::bool__ _TryToExtendUsedFragmentWithoutMoving(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )	// The fragment must not be linked.
		{
#ifdef MMM2_DBG
			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			mdr::size__ TotalSize = _GetUsedFragmentTotalSize( Header );
			row__ NextFragmentPosition = _GetUsedFragmentNextFragmentPosition( Descriptor, Header );

			if ( NextFragmentPosition == S_.Extent )  {
				_ExtendUsedFragmentNotFollowedByAnyFragment( Descriptor, Header, DataSize, Addendum );

				return true;
			} else if ( *NextFragmentPosition < S_.Extent ) {
				mdr::datum__ NextFragmentHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( NextFragmentPosition, NextFragmentHeader );

				if ( _IsFragmentFree( NextFragmentHeader ) )
					return _TryToExtendUsedFragmentFollowedByFreeFragmentWithoutMoving( Descriptor, Header, DataSize, NextFragmentPosition, NextFragmentHeader, Addendum );
				else
					return false;
			} else
				ERRc();

			return false;	// To avoid a 'warning'.
		}
		row__ _ExtendUsingFreeFragments(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )
		{
			row__ NewDescriptor = _Allocate( DataSize, Addendum );

			_Move( Descriptor, Header, NewDescriptor );

			_Free( *Descriptor );

			return NewDescriptor;
		}
		row__ _Extend(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )
		{
			if ( _IsUsedFragmentLinked( Header ) ) {
				addendum__ DummyAddendum;
				row__ LinkPosition = _GetUsedFragmentLink( Descriptor, Header );
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( LinkPosition, LinkHeader );

				if ( !_TryToExtendUsedFragmentWithoutMoving( LinkPosition, LinkHeader, DataSize - _GetUsedFragmentDataSize( Header ), DummyAddendum ) )
					Descriptor = _ExtendUsingFreeFragments( Descriptor, Header, DataSize, Addendum );
			} else if ( !_TryToExtendUsedFragmentWithoutMoving( Descriptor, Header, DataSize, Addendum ) )
				Descriptor = _ExtendUsingFreeFragments( Descriptor, Header, DataSize, Addendum );

			return Descriptor;
		}
		mdr::size__ _ReadFromFragment(
			descriptor__ Descriptor,
			const mdr::datum__ *Header,
			bso::size__ HeaderSize,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer ) const
		{
			mdr::size__ FragmentDataSize = _GetUsedFragmentDataSize( Header );

#ifdef MMM2_DBG
			if ( Position >= FragmentDataSize )
				ERRc();
#endif

			if ( Amount > ( FragmentDataSize - Position ) )
				Amount = FragmentDataSize - Position;

			if ( ( Position + Amount + _GetSizeLength( FragmentDataSize ) ) <= HeaderSize )
				memcpy( Buffer, Header + _GetSizeLength( FragmentDataSize ) + Position, Amount );
			else
				Memory.Recall( Descriptor + _GetSizeLength( FragmentDataSize ) + Position, Amount, Buffer );

			return Amount;
		}
		mdr::size__ _WriteToFragment(
			const mdr::datum__ *Buffer,
			descriptor__ Descriptor,
			const mdr::datum__ *Header,
			mdr::row_t__ Position,
			mdr::size__ Amount )
		{
			mdr::size__ FragmentDataSize = _GetUsedFragmentDataSize( Header );

#ifdef MMM2_DBG
			if ( Position >= FragmentDataSize )
				ERRc();
#endif

			if ( Amount > ( FragmentDataSize - Position ) )
				Amount = FragmentDataSize - Position;

			Memory.Store( Buffer, Amount, Descriptor + _GetSizeLength( FragmentDataSize ) + Position );

			return Amount;
		}
		bso::bool__ _IsFreeFragmentAvailable( void ) const
		{
			return S_.FreeFragment != NONE;
		}
		row__ _GetFreeFragment( void ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFreeFragmentAvailable() )
				ERRc();
#endif
			return S_.FreeFragment;
		}
/*
		row__ _RetrieveFreeFragment( void )
		{
			row__ Row = _ConsultAvailableFreeFragment();

			_ExciseFreeFragment_( Row );

			return Row;
		}
*/	public:
		uym::untyped_memory_ Memory;
		struct s 
		{
			uym::untyped_memory_ ::s Memory;
			mdr::size__ Extent;
			row__ FreeFragment;	// Position d'un fragment libre. NONE si aucun.
			row__ TailingFreeFragmentPosition;	// If the last fragment is a free one, this is its position (orphan or not).
			descriptor__ MultimemoryDriverDescriptor;
			addendum__ MultimemoryDriverAddendum;
			mdr::size__ MultimemoryDriverExtent;
		} &S_;
		multimemory_( s &S )
		: S_( S ) ,
		  _MultimemoryDriver( S.MultimemoryDriverDescriptor, S.MultimemoryDriverAddendum, S.MultimemoryDriverExtent ),
		  Memory( S.Memory )
		{}
		void reset( bso::bool__ P = true )
		{
			_MultimemoryDriver.reset( P );
			Memory.reset( P );
			S_.Extent = 0;
			S_.FreeFragment = NONE;
			S_.TailingFreeFragmentPosition = NONE;
		}
		void plug( multimemory_ &MM )
		{
			_MultimemoryDriver.Init( MM );
			Memory.plug( _MultimemoryDriver );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			_MultimemoryDriver.reset();
			Memory.plug( MD );
		}
		multimemory_ &operator =( const multimemory_ &M )
		{
			Memory.Allocate( M.S_.Extent );
			Memory.Store_( M.Memory, M.S_.Extent, 0 );

			S_.Extent = M.S_.Extent;
			S_.FreeFragment = M.S_.FreeFragment;
			S_.TailingFreeFragmentPosition = M.S_.TailingFreeFragmentPosition;

			return *this;
		}
		void Init( void )
		{
			Memory.Init();

			S_.Extent = 0;
			S_.FreeFragment = NONE;
			S_.TailingFreeFragmentPosition = NONE;
		}
		void Flush( void ) const
		{
			Memory.Flush();
		}
		mdr::size__ Size( descriptor__ Descriptor ) const
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Descriptor, Header );

			return _Size( Descriptor, Header );
		}
		descriptor__ Allocate(
			mdr::size__ Size,
			addendum__ &Addendum )
		{	
			return _Allocate( Size, Addendum );
		}
		void Free( descriptor__ Descriptor )
		{
			_Free( Descriptor );
		}
		descriptor__ Reallocate(
			descriptor__ Descriptor,
			mdr::size__ Size,
			addendum__ &Addendum )
		{
			if ( Descriptor == NONE )
				if ( Size != 0 )
					return _Allocate( Size, Addendum );
				else
					return Descriptor;

			if ( Size == 0 ) {
				Free( Descriptor );
				return NONE; // Pour viter un 'warning'.
			}

			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Descriptor, Header );

			mdr::size__ CurrentSize = _Size( Descriptor, Header );

			if ( Size == CurrentSize )
				return Descriptor;
			else if ( Size < CurrentSize )
				return *_Shrink( Descriptor, Header, Size, Addendum );
			else
				return *_Extend( Descriptor, Header, Size, Addendum );
		}
		void Read(
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer,
			const addendum__ &Addendum ) const
		{
			if ( Addendum == 0 ) {
				//			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				mdr::datum__ Header[1024];
				_GetHeader( Descriptor, Header, sizeof( Header ) );

#ifdef MMM2_DBG
				if ( !_IsFragmentUsed( Header ) )
					ERRu();
#endif
				mdr::size__ AmountRed = 0;
				mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Header );

				if ( Position <  FirstFragmentDataSize )
					AmountRed = _ReadFromFragment( Descriptor, Header, sizeof( Header ), Position, Amount, Buffer );

				if ( AmountRed < Amount ) {
					descriptor__ Link = *_GetUsedFragmentLink( Descriptor, Header );

					_GetHeader( Link, Header, sizeof( Header ) );

					AmountRed += _ReadFromFragment( Link, Header, sizeof( Header ), Position + AmountRed - FirstFragmentDataSize, Amount - AmountRed, Buffer + AmountRed );
				}

#ifdef MMM2_DBG
				if ( Amount != AmountRed )
					ERRc();
#endif
			} else {
				Memory.Recall( Descriptor + Addendum + Position, Amount, Buffer );
			}
		}
		void Write(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			const addendum__ &Addendum )
		{

			if ( Addendum == 0 ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				_GetHeader( Descriptor, Header );

#ifdef MMM2_DBG
				if ( !_IsFragmentUsed( Header ) )
					ERRu();
#endif
				mdr::size__ AmountWritten = 0;
				mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Header );

				if ( Position <  FirstFragmentDataSize )
					AmountWritten = _WriteToFragment( Buffer, Descriptor, Header, Position, Amount );

				if ( AmountWritten < Amount ) {
					descriptor__ Link = *_GetUsedFragmentLink( Descriptor, Header );
					mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

					_GetHeader( Link, LinkHeader );

					AmountWritten += _WriteToFragment( Buffer + AmountWritten, Link, LinkHeader, Position + AmountWritten - FirstFragmentDataSize, Amount - AmountWritten );
				}

#ifdef MMM2_DBG
				if ( Amount != AmountWritten )
					ERRc();
#endif
			} else
				Memory.Store( Buffer, Amount, Descriptor + Addendum + Position );
		}
		void Preallocate( mdr::size__ Size )
		{
			if ( S_.Extent > Size )
				ERRu();
			else if ( S_.Extent != Size ) {
				Size -= S_.Extent;
				row__ Row = _AppendNewFragment( Size );
				_SetAsFreeFragment( Row,Size );

				S_.TailingFreeFragmentPosition = Row;
			}
		}
		void DisplayStructure( txf::text_oflow__ &Flow ) const;
		friend bso::bool__ Connect(
			multimemory_ &Multimemory,
			class multimemory_file_manager___ &FileManager );
	};

	typedef uym::untyped_memory_file_manager___ _untyped_memory_file_manager___;

	class multimemory_file_manager___
	: public _untyped_memory_file_manager___
	{
	private:
		multimemory_ *_Multimemory;
		void _WriteFreeFragmentPositions( void );
		tol::E_FPOINTER___( bso::char__ ) _FreeFragmentPositionFileName;
	public:
		void reset( bso::bool__ P = true )
		{
			_untyped_memory_file_manager___::ReleaseFile();

			if ( P ) {
				if ( ( _Multimemory != NULL )
					 && _untyped_memory_file_manager___::IsPersistent()
					 && untyped_memory_file_manager___::Exists()
					 && ( !tol::FileExists( _FreeFragmentPositionFileName )
					      || ( untyped_memory_file_manager___::TimeStamp()
						       >= tol::GetFileLastModificationTime( _FreeFragmentPositionFileName ) ) ) )
					_WriteFreeFragmentPositions();
			}

			_untyped_memory_file_manager___::reset( P );
			_FreeFragmentPositionFileName.reset( P );

			_Multimemory = NULL;
		}
		multimemory_file_manager___( void )
		: _untyped_memory_file_manager___()
		{
			reset( false );
		}
		~multimemory_file_manager___( void )
		{
			reset();
		}
		void Init(
			multimemory_ &Multimemory,
			const char *FileName,
			const char *FreeFragmentPositionsFileName,
			mdr::mode__ Mode,
			bso::bool__ Persistent,
			flm::files_group_ &FilesGroup )
		{
			reset();

			if ( ( _FreeFragmentPositionFileName = malloc( strlen( FreeFragmentPositionsFileName ) + 1 ) ) == NULL )
				ERRa();

			strcpy( _FreeFragmentPositionFileName, FreeFragmentPositionsFileName );

			_Multimemory = &Multimemory;

			_untyped_memory_file_manager___::Init( FileName, Mode, Persistent, FilesGroup );
		}
		friend bso::bool__ Connect(
			multimemory_ &Multimemory,
			multimemory_file_manager___ &FileManager );
	};


	bso::bool__ Connect(
		multimemory_ &Multimemory,
		multimemory_file_manager___ &FileManager );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
