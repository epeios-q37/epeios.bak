/*
	Header for the 'mmm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: mmm.h,v 1.53 2013/03/14 11:09:19 csimon Exp $

#ifndef MMM__INC
#define MMM__INC

#define MMM_NAME		"MMM"

#define	MMM_VERSION	"$Revision: 1.53 $"

#define MMM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MMMTutor;

#if defined( E_DEBUG ) && !defined( MMM_NODBG )
#define MMM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.53 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/03/14 11:09:19 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MultiMeMory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'AGS' instead !"

#include "err.h"
#include "flw.h"
#include "bso.h"
// #include "uym.h"	// Dport, parce que inclus 'mmm.h'.
#include "tol.h"
#include "txf.h"
#include "flm.h"
#include "flf.h"

#ifdef UYM__INC
#	ifndef UYM__HEADER_HANDLED
#		define MMM__HANDLE_PART_ONE
#		undef MMM__INC
#	else
#		define MMM__HANDLE_PART_TWO
#	endif
#else
#	define MMM__HANDLE_PART_ONE
#	define MMM__HANDLE_PART_TWO
#endif

#ifdef MMM__HANDLE_PART_ONE
#	ifdef	MMM__PART_ONE_HANDLED
#		error
#	endif
#	ifdef	MMM__PART_TWO_HANDLED
#		error
#	endif

#define MMM_UNDEFINED_DESCRIPTOR NONE

// Ce drapeau indique si la taille est stock dans le fragment. Ne concerne que les fragments libres.
#define MMM_FREE_FRAGMENT_SIZE_FLAG_POSITION	7
#define MMM_FREE_FRAGMENT_SIZE_FLAG_MASK ( 1 << MMM_FREE_FRAGMENT_SIZE_FLAG_POSITION )


// Ce drapeau indique si le fragment est li ou non (donne rparties sur deux fragments ou non). Ne concerne que les fragments utiliss.
#define MMM_USED_FRAGMENT_LINK_FLAG_POSITION	7
#define MMM_USED_FRAGMENT_LINK_FLAG_MASK ( 1 << MMM_USED_FRAGMENT_LINK_FLAG_POSITION )

// Ce drapeau indique si le fragment prcdent est libre ou non. Ne concerne que les fragments utiliss.
#define MMM_USED_FRAGMENT_FREE_FLAG_POSITION	6
#define MMM_USED_FRAGMENT_FREE_FLAG_MASK ( 1 << MMM_USED_FRAGMENT_FREE_FLAG_POSITION )

#define MMM_USED_FRAGMENT_FLAGS_MASK	( MMM_USED_FRAGMENT_LINK_FLAG_MASK | MMM_USED_FRAGMENT_FREE_FLAG_MASK )

#define MMM_L1	((unsigned char)~MMM_USED_FRAGMENT_FLAGS_MASK )
#define MMM_L2 ( 0xffffUL + MMM_L1 )
#define MMM_L3 0xffffffffUL
#define MMM_SIZE_BUFFER_MAX_LENGTH	7
#define MMM_LINK_SIZE	sizeof( mmm::row__ )
#define MMM_ORPHAN_MAX_SIZE	10
#define MMM_NORMAL_FREE_FRAGMENT_MIN_SIZE	( 1 + 1 + MMM_LINK_SIZE + MMM_LINK_SIZE + MMM_LINK_SIZE + 1 )
#define MMM_HEADER_MAX_LENGTH	( 1+ MMM_SIZE_BUFFER_MAX_LENGTH + 2 * MMM_LINK_SIZE  )	// Correspond  la struture d'un 'header' d'un fragment libre,
																							// ce dernier tant plus grand que le 'header' d'un fragment utilis.
#define MMM_FREE_FRAGMENT_FOOTER_MAX_SIZE	( MMM_LINK_SIZE + 1 )

namespace mmm {
	class multimemory_;	// Prdclaration.

	E_ROW( row__ );

	//t Type d'un descripteur de sous-mmoire dans le multimmoire.
	typedef mdr::row_t__	descriptor__;

	//c The standard memory driver for the multimemory.
	class multimemory_driver__
	: public mdr::E_MEMORY_DRIVER__
	{
	private:
		descriptor__ &_Descriptor;
		// memoire  laquelle il a t affect
		class multimemory_ *Multimemoire_;
		bso::ubyte__ &_Addendum;
		void Liberer_();
	protected:
		virtual void MDRAllocate( mdr::size__ Size );
		// Fonction dporte.
		virtual mdr::size__ MDRUnderlyingSize( void );
		// fonction dporte
		// lit  partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer );
		// fonction dporte
		// crit 'Nombre' octets  la position 'Position'
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Multimemoire_ != NULL )
					Liberer_();
			} else
				Multimemoire_ = NULL;

			E_MEMORY_DRIVER__::reset( P );

			// On ne touche ni  '_Descriptor', ni  '_Addendum' car ils sont grs extrieurement (ce sont des rfrences).
		}
		multimemory_driver__(
			descriptor__ &Descriptor,
			bso::ubyte__ &Addendum )
		: _Descriptor( Descriptor ),
		  _Addendum( Addendum ),
		  E_MEMORY_DRIVER__()
		{
			reset( false );
		}
		~multimemory_driver__( void )
		{
			reset();
		}
		//f Initialization with the 'Multimemory' multimemory.
		void Init( multimemory_ &Multimemory )
		{
			reset();

			Multimemoire_ = &Multimemory;
			E_MEMORY_DRIVER__::Init();

			// On ne touche ni  '_Descriptor', ni  '_Addendum' car ils sont grs extrieurement (ce sont des rfrences).
		}
		//f Return the current descriptor.
		descriptor__ Descriptor( void ) const
		{
			return _Descriptor;
		}
		//f Return the used multimemory.
		multimemory_ *Multimemory( void ) const
		{
			return Multimemoire_;
		}
	};

	typedef bso::ubyte__ addendum__;
}

#	define MMM__PART_ONE_HANDLED
#endif

#ifdef MMM__HANDLE_PART_TWO
#	ifndef MMM__PART_ONE_HANDLED
#		error
#	endif
#	ifdef MMM__PART_TWO_HANDLED
#		error
#	endif

#include "uym.h"
namespace mmm {

	class multimemory_
	{
	private:
		mdr::size__ _Grow( mdr::size__ Size )
		{
			mdr::size__ Buffer = Memory.GetSize();

			Memory.Allocate( Buffer + Size );

			return Buffer;
		}
		bso::size__ _Size( void ) const
		{
			return Memory.GetSize();
		}
		bso::ubyte__ _GetSizeLength( mdr::size__ Size ) const
		{
#ifdef MMM_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( Size < MMM_L1 )
				return 1;
			else if ( Size < MMM_L2 )
				return 3;
			else if ( Size <= MMM_L3 )
				return 7;
			else
				ERRc();

			return 0;	// Pour viter un 'warning'.
		}
		// Retourne la taile maximale telle que cette taille + l'indicateur de taille soit infrieur ou gal  'MaxSize'.
		mdr::size__ _AdjustSize( mdr::size__ MaxSize ) const
		{
#ifdef MMM_DBG
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
			if ( Size < MMM_L1 ) {
				Datum[0] = ( Datum[0] & MMM_USED_FRAGMENT_FLAGS_MASK ) | (mdr::datum__)Size;
				return;
			}

			Datum[0] = ( Datum[0] & MMM_USED_FRAGMENT_FLAGS_MASK ) | ~MMM_USED_FRAGMENT_FLAGS_MASK;

			Size -= MMM_L1;

			if ( Size < ( MMM_L2 - MMM_L1 ) ) {
				Datum[1] = (mdr::datum__)( Size >> 8 );
				Datum[2] = (mdr::datum__)( Size & 0xff );

				return;
			}

			Datum[1] = 0xff;
			Datum[2] = 0xff;

			Size -= 0xffff;

			if ( Size < ( MMM_L3 - MMM_L2 ) ) {
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

			Size = Datum[0] & ~MMM_USED_FRAGMENT_FLAGS_MASK;

			if ( Size == MMM_L1 ) {
				Size += ( Datum[1] << 8 ) + Datum[2];

				if ( Size == MMM_L2 ) {
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
			mdr::size__ Amount = MMM_HEADER_MAX_LENGTH ) const
		{
			Memory.Recall( *Position, ( *Position + Amount ) > _Size() ? _Size() - *Position : Amount, Header );
		}
		bso::bool__ _IsFragmentUsed( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM_USED_FRAGMENT_FLAGS_MASK ) != 0;
		}
		bso::bool__ _IsFragmentFree( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM_USED_FRAGMENT_FLAGS_MASK ) == 0;
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
			SizeBuffer[0] = ( LinkFlag << MMM_USED_FRAGMENT_LINK_FLAG_POSITION ) | ( FreeFlag << MMM_USED_FRAGMENT_FREE_FLAG_POSITION );

			_PutSize( Size, SizeBuffer );
		}
		void _SetRawSize(
			mdr::size__ Size,
			row__ Position,
			bso::bool__ LinkFlag,
			bso::bool__ FreeFlag )
		{
			mdr::datum__ SizeBuffer[MMM_SIZE_BUFFER_MAX_LENGTH];

			_SetRawSize( Size, LinkFlag, FreeFlag, SizeBuffer );

			Memory.Store( SizeBuffer, _GetSizeLength( Size ), *Position );
		}
		bso::bool__ _IsUsedFragmentFreeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM_USED_FRAGMENT_FREE_FLAG_MASK ) != 0;
		}
		void _SetUsedFragmentFreeFlag(
			row__ Position,
			const mdr::datum__ *Header,
			bso::bool__ Value )
		{
#ifdef MMM_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			Memory.Put( ( Header[0] & ~MMM_USED_FRAGMENT_FREE_FLAG_MASK ) | ( Value << MMM_USED_FRAGMENT_FREE_FLAG_POSITION ), *Position );
		}
		void _SetUsedFragmentFreeFlag(
			row__ Position,
			bso::bool__ Value )
		{
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetUsedFragmentFreeFlag( Position, Header, Value );
		}
		bso::bool__ _IsUsedFragmentLinkFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM_USED_FRAGMENT_LINK_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsFreeFragmentSizeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM_FREE_FRAGMENT_SIZE_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsUsedFragmentLinked( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _IsUsedFragmentLinkFlagSet( Header );
		}
		mdr::size__ _GetFreeFragmentSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
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
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			return _GetFreeFragmentSize( Header );
		}
		mdr::size__ _GetFreeFragmentHeaderLength( const mdr::datum__ *Header ) const
		{
			mdr::size__ Size = _GetFreeFragmentSize( Header );

			if ( Size == 1 )
				return 1;
			else if ( Size <= MMM_ORPHAN_MAX_SIZE )
				return _GetSizeLength( Size ) + 1;
			else
				return _GetSizeLength( Size ) + 1 + MMM_LINK_SIZE + MMM_LINK_SIZE;
		}
		bso::bool__ _IsFreeFragmentOrphan( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentSize( Header ) <= MMM_ORPHAN_MAX_SIZE;
		}
		row__ _GetUsedFragmentLink(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			row__ Link = NONE;
#ifdef MMM_DBG
			if ( !_IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			Memory.Recall( *Position + _GetUsedFragmentTotalSize( Header ) - MMM_LINK_SIZE, MMM_LINK_SIZE, (mdr::datum__ *)&Link );

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
#ifdef MMM_DBG
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
			return _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ) + MMM_LINK_SIZE;
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
				mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

				Header[0] = MMM_FREE_FRAGMENT_SIZE_FLAG_MASK;

				_SetRawSize( Size, false, false, Header + 1 );

				if ( Size <= MMM_ORPHAN_MAX_SIZE )
					Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );	// Marqueur  destination du fragement utilis qui suit.
				else {
					if ( Size < MMM_NORMAL_FREE_FRAGMENT_MIN_SIZE )
						Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );	// Marqueur  destination du fragement utilis qui suit.
					else {
						Memory.Put( 0xff, *Position + Size - 1 );	// Marqueur  destination du fragement utilis qui suit.
						Memory.Store( (const mdr::datum__ *)&Position, MMM_LINK_SIZE, *Position + Size - 5 );	// Position du dbut du fragment  destination du fragment utilis qui suit.
					}
					// Bien que 'Header' soit en cours de construction, son contenu est suffisant pour pouvoir l'utiliser.
					memcpy( Header + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ), &PreviousFragmentPosition, MMM_LINK_SIZE );
					memcpy( Header + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header ), &NextFragmentPosition, MMM_LINK_SIZE );
				}

				Memory.Store( Header, _GetFreeFragmentHeaderLength( Header ), *Position );
			}
		}
		mdr::size__ _GetUsedFragmentDataSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _GetRawSize( Header );
		}
		mdr::size__ _GetUsedFragmentTotalSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			mdr::size__ DataSize = _GetUsedFragmentDataSize( Header );

			return DataSize + _GetSizeLength( DataSize ) + ( _IsUsedFragmentLinked( Header ) ? MMM_LINK_SIZE : 0 );
		}
		row__ _GetFreeFragmentPosition(
			row__ AfterFreeFragmentFooterPosition,	// Position immedialty after the free fragment footer.
			const mdr::datum__ *FreeFragmentTail ) const
		{
			const mdr::datum__ *Pointer = FreeFragmentTail + MMM_FREE_FRAGMENT_FOOTER_MAX_SIZE - 1;

			if ( *Pointer == 0 )
				return *AfterFreeFragmentFooterPosition - 1;
			else if ( *Pointer != 0xff )
				return *AfterFreeFragmentFooterPosition - *Pointer;
			else
				return *(row__ *)( Pointer - MMM_LINK_SIZE );
		}
		void _GetFreeFragmentFooter(
			row__ AfterFreeFragmentFooterPosition,	// Position immedialty after the free fragment footer.
			mdr::datum__ *FreeFragmentFooter ) const
		{
#ifdef MMM_DBG
			if ( AfterFreeFragmentFooterPosition == 0 )
				ERRc();
#endif

			mdr::size__ Amount = MMM_FREE_FRAGMENT_FOOTER_MAX_SIZE;

			if ( *AfterFreeFragmentFooterPosition < Amount )
				Amount = *AfterFreeFragmentFooterPosition;

			Memory.Recall( *AfterFreeFragmentFooterPosition - Amount, Amount, FreeFragmentFooter + MMM_FREE_FRAGMENT_FOOTER_MAX_SIZE - Amount );
		}

		row__ _GetFreeFragmentPosition( row__ AfterFreeFragmentFooterPosition ) const	// Position immedialty after the free fragment footer.
		{
			mdr::datum__ FreeFragmentFooter[MMM_FREE_FRAGMENT_FOOTER_MAX_SIZE];

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

			memcpy( &Row, Header + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ), MMM_LINK_SIZE );

			return Row;
		}
		void _SetFreeFragmentPreviousFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			row__ PreviousFragmentPosition )
		{
			Memory.Store( (const mdr::datum__ *)&PreviousFragmentPosition, MMM_LINK_SIZE, *_GetFreeFragmentPreviousFreeFragmentPointerPosition( Position, Header ) );
		}
		void _SetFreeFragmentPreviousFreeFragmentPosition(
			row__ Position,
			row__ PreviousFragmentPosition )
		{
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetFreeFragmentPreviousFreeFragmentPosition( Position, Header, PreviousFragmentPosition );
		}
		row__ _GetFreeFragmentNextFreeFragmentPosition( const mdr::datum__ *Header ) const
		{
			row__ Row;

			memcpy( &Row, Header + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header ), MMM_LINK_SIZE );

			return Row;
		}
		void _SetFreeFragmentNextFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			row__ NextFragmentPosition )
		{
			Memory.Store( (const mdr::datum__ *)&NextFragmentPosition, MMM_LINK_SIZE, *_GetFreeFragmentNextFreeFragmentPointerPosition( Position, Header ) );
		}
		void _SetFreeFragmentNextFreeFragmentPosition(
			row__ Position,
			row__ NextFragmentPosition )
		{
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetFreeFragmentNextFreeFragmentPosition( Position, Header, NextFragmentPosition );
		}
		void _UpdateFreeFragmentMarker(
			row__ RemovedFragmentPosition,
			const mdr::datum__ *Header )
		{
#ifdef MMM_DBG
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
#ifdef MMM_DBG
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
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( RemovedFreeFragmentPosition, Header );

			_ExciseFreeFragment( RemovedFreeFragmentPosition, Header );
		}
		void _ReportNewFreeFragment(
			row__ NewFreeFragmentPosition,
			row__ &PreviousFreeFragmentPosition,
			row__ &NextFreeFragmentPosition )
		{
			if ( S_.FreeFragment != NONE ) {
				mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];
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
#ifdef MMM_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( ( *Position + Size ) != _Size() ) {
				mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

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

			if  ( Size > MMM_ORPHAN_MAX_SIZE ) {
				row__ PreviousFreeFragmentPosition = NONE, NextFreeFragmentPosition = NONE;

				if ( ( *Position + Size ) != _Size()	)	// If the freed fragment becomes the last one, it is not included in the list, because it is handled otherwher else.
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
#ifdef MMM_DBG
			if ( Size == 0 )
				ERRc();
#endif

			if ( _IsUsedFragmentFreeFlagSet( Header ) ) {
				mdr::datum__ TempHeader[MMM_HEADER_MAX_LENGTH];
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
			return _GetSizeLength( DataSize ) + DataSize + ( WithLink ? MMM_LINK_SIZE : 0 );
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
#ifdef MMM_DBG
			if ( DataSize == 0 )
				ERRc();
#endif
			_SetRawSize( DataSize, Position, ( Link != NONE ), FreeFlag );

			if ( Link != NONE )
				Memory.Store( (const mdr::datum__ *)&Link, MMM_LINK_SIZE, *Position + _GetSizeLength( DataSize ) + DataSize );
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
#ifdef MMM_DBG
			if ( Size == 0 )
				ERRc();

			if ( *RemainderPosition > _Size() )
				ERRc();
#endif
			_ExciseFreeFragment( Position, Header );

			if ( RemainderPosition != _Size() )
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
			return _AdjustSize( TotalSize - ( Linked ? MMM_LINK_SIZE : 0 ) );
		}
		mdr::size__ _GuessFreeFragmentAvailableSize(
			const mdr::datum__ *Header,
			bso::bool__ Linked ) const
		{
#ifdef MMM_DBG
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
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			return _IsFreeFragmentBigEnough( Header, DataSize, Linked );
		}
		row__ _AppendNewFragment( mdr::size__ Size )
		{
			row__ Row = NONE;

			if ( S_.TailingFreeFragmentPosition != NONE ) {
				mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];
				mdr::size__ FreeFragmentSize = 0;

				Row = S_.TailingFreeFragmentPosition;

				_GetHeader( Row, Header );

				FreeFragmentSize = _GetFreeFragmentSize( Header );

				if ( FreeFragmentSize > Size ) {
					S_.TailingFreeFragmentPosition = *Row + Size;
					_SetAsFreeFragment( S_.TailingFreeFragmentPosition, FreeFragmentSize - Size );
				} else if ( FreeFragmentSize <= Size ) {
					if ( FreeFragmentSize != Size ) {
/*
						S_.Size += Size - FreeFragmentSize;

						Memory.Allocate( S_.Size );
*/
						_Grow( Size - FreeFragmentSize );
					}

					S_.TailingFreeFragmentPosition = NONE;
				} else
					ERRc();

			} else {
/*				Row = _Size();

				S_.Size += Size;

				Memory.Allocate( S_.Size );
*/
				Row = _Grow( Size );
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
			mdr::datum__ FirstFragmentHeader[MMM_HEADER_MAX_LENGTH];

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
				mdr::datum__ SecondFragmentHeader[MMM_HEADER_MAX_LENGTH];

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

				mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

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

				mdr::datum__ FreeFragmentHeader[MMM_HEADER_MAX_LENGTH];

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

			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Position, Header );
				mdr::datum__ LinkHeader[MMM_HEADER_MAX_LENGTH];

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
				Memory.Store( Memory, OldDataSize, *Position + NewSizeLength, *Position + OldSizeLength );

			_SetRawSize( NewDataSize, Position, false, _IsUsedFragmentFreeFlagSet( Header ) );

			Addendum = _GetSizeLength( NewDataSize );
		}
		void _ShrinkUsedFragment(
			row__ Position,	// NOTA : Can be a linked fragment, but the fragment becomes unlinked.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum )
		{
#ifdef MMM_DBG
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

				mdr::datum__ LinkHeader[MMM_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				if ( DataSize > _GetUsedFragmentDataSize( Header ) ) {
					_ShrinkUsedFragment( Link, LinkHeader, DataSize - _GetUsedFragmentDataSize( Header ), Addendum );

					Addendum = 0;
				} else {
					mdr::datum__ NewHeader[MMM_HEADER_MAX_LENGTH];
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
				mdr::datum__ LinkHeader[MMM_HEADER_MAX_LENGTH];

				_GetHeader( _GetUsedFragmentLink( Descriptor, Header ), LinkHeader );

				Size += _GetUsedFragmentDataSize( LinkHeader );

#ifdef MMM_DBG
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
			Memory.Store(
				Memory, SourceFirstFragmentSize > TargetFirstFragmentSize ? TargetFirstFragmentSize : SourceFirstFragmentSize,
				*TargetFirstFragmentRow, *SourceFirstFragmentRow );

			if ( ( SourceSecondFragmentSize != 0 ) && ( ( SourceFirstFragmentSize + SourceSecondFragmentSize ) <= TargetFirstFragmentSize ) )
				Memory.Store(
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
				Memory.Store(
					Memory, SourceFirstFragmentSize - TargetFirstFragmentSize,
					*TargetSecondFragmentRow, *SourceFirstFragmentRow + TargetFirstFragmentSize );
			else
				Memory.Store(
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
			et non pas sur le dbut des fragments respectifs. */
		{
			if ( SourceFirstFragmentSize > TargetFirstFragmentSize )
				Memory.Store(
					Memory, SourceSecondFragmentSize,
					*TargetSecondFragmentRow + SourceFirstFragmentSize - TargetFirstFragmentSize,
					*SourceSecondFragmentRow );
			else
				Memory.Store(
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
			et non pas sur le dbut des fragments respectifs. */
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
			mdr::datum__ SourceSecondFragmentHeader[MMM_HEADER_MAX_LENGTH], TargetSecondFragmentHeader[MMM_HEADER_MAX_LENGTH];
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
			mdr::datum__ TargetFirstFragmentHeader[MMM_HEADER_MAX_LENGTH];

			_GetHeader( TargetFirstFragmentRow, TargetFirstFragmentHeader );

			_Move( SourceFirstFragmentRow, SourceFirstFragmentHeader, TargetFirstFragmentRow, TargetFirstFragmentHeader );
		}
		void _ExtendUsedFragmentNotFollowedByAnyFragment(
			row__ Descriptor,	// Le fragment ne doit pas tre un 'linked fragment'.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			addendum__ &Addendum,
			bso::bool__ SkipSizeTest = false )
		{
#ifdef MMM_DBG
			if ( ( *Descriptor + _GetUsedFragmentTotalSize( Header ) ) != _Size() )
				if ( !SkipSizeTest )
					ERRc();

			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
/*
			S_.Size += _GuessTotalSizeForUsedFragment( DataSize, false ) - _GetUsedFragmentTotalSize( Header );

			Memory.Allocate( S_.Size );
*/
			_Grow( _GuessTotalSizeForUsedFragment( DataSize, false ) - _GetUsedFragmentTotalSize( Header ) );

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
#ifdef MMM_DBG
			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();

			if ( NextFragmentPosition != S_.TailingFreeFragmentPosition )
				ERRc();
#endif
			S_.TailingFreeFragmentPosition = NONE;

//			#pragma message( "Point dlicat ici !" )
//			S_.Size = *Descriptor + _GetUsedFragmentTotalSize( Header );	// Permet l'utilisation de la mthode qui suit.

			// Le paramtre  'true' dans l'appel qui suit est la prise en compte de la ligne prcdente.
			_ExtendUsedFragmentNotFollowedByAnyFragment( Descriptor, Header, DataSize, Addendum, true );
		}
		void _ExtendUsedFragmentFollowedByBigEnoughFreeFragment(
			row__ Descriptor,	// The fragment must not be linked.
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			row__ NextFragmentPosition,	// It's assumed that this fragment is big enough to allow the extending.
			const mdr::datum__ *NextFragmentHeader,
			addendum__ &Addendum )
		{
#ifdef MMM_DBG
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
				if ( ( *NextFragmentPosition + Delta ) < _Size() )
					_SetUsedFragmentFreeFlag( *NextFragmentPosition + Delta, false );
#ifdef MMM_DBG
				else if ( ( *NextFragmentPosition + Delta ) > _Size() )
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
#ifdef MMM_DBG
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
#ifdef MMM_DBG
			if ( _IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			mdr::size__ TotalSize = _GetUsedFragmentTotalSize( Header );
			row__ NextFragmentPosition = _GetUsedFragmentNextFragmentPosition( Descriptor, Header );

			if ( NextFragmentPosition == _Size() )  {
				_ExtendUsedFragmentNotFollowedByAnyFragment( Descriptor, Header, DataSize, Addendum );

				return true;
			} else if ( *NextFragmentPosition < _Size() ) {
				mdr::datum__ NextFragmentHeader[MMM_HEADER_MAX_LENGTH];

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
				mdr::datum__ LinkHeader[MMM_HEADER_MAX_LENGTH];

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

#ifdef MMM_DBG
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

#ifdef MMM_DBG
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
#ifdef MMM_DBG
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
			row__ FreeFragment;	// Position d'un fragment libre. NONE si aucun.
			row__ TailingFreeFragmentPosition;	// If the last fragment is a free one, this is its position (orphan or not).
		} &S_;
		void reset( bso::bool__ P = true )
		{
			Memory.reset( P );
//			S_.Size = 0;
			S_.FreeFragment = NONE;
			S_.TailingFreeFragmentPosition = NONE;
		}
		multimemory_( s &S )
		: S_( S ) ,
		  Memory( S.Memory )
		{
			reset( false );
		}
		~multimemory_( void )
		{
			reset();
		}
		void plug( multimemory_ &MM )
		{
			Memory.plug( MM );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Memory.plug( MD );
		}
		multimemory_ &operator =( const multimemory_ &M )
		{
			Memory.Allocate( M._Size() );
			Memory.Store( M.Memory, M._Size(), 0 );

//			S_.Size = M.S_.Size;
			S_.FreeFragment = M.S_.FreeFragment;
			S_.TailingFreeFragmentPosition = M.S_.TailingFreeFragmentPosition;

			return *this;
		}
		void Init( void )
		{
			Memory.Init();

//			S_.Size = 0;
			
			S_.FreeFragment = NONE;
			S_.TailingFreeFragmentPosition = NONE;
		}
		void SetSize( bso::size__ Value )
		{
			Memory.SetSize( Value );
		}
		mdr::size__ Size( descriptor__ Descriptor ) const
		{
			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

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

			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

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
				//			mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];
				mdr::datum__ Header[1024];
				_GetHeader( Descriptor, Header, sizeof( Header ) );

#ifdef MMM_DBG
				if ( !_IsFragmentUsed( Header ) )
					ERRc();
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

#ifdef MMM_DBG
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
				mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];
				_GetHeader( Descriptor, Header );

#ifdef MMM_DBG
				if ( !_IsFragmentUsed( Header ) )
					ERRc();
#endif
				mdr::size__ AmountWritten = 0;
				mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Header );

				if ( Position <  FirstFragmentDataSize )
					AmountWritten = _WriteToFragment( Buffer, Descriptor, Header, Position, Amount );

				if ( AmountWritten < Amount ) {
					descriptor__ Link = *_GetUsedFragmentLink( Descriptor, Header );
					mdr::datum__ LinkHeader[MMM_HEADER_MAX_LENGTH];

					_GetHeader( Link, LinkHeader );

					AmountWritten += _WriteToFragment( Buffer + AmountWritten, Link, LinkHeader, Position + AmountWritten - FirstFragmentDataSize, Amount - AmountWritten );
				}

#ifdef MMM_DBG
				if ( Amount != AmountWritten )
					ERRc();
#endif
			} else
				Memory.Store( Buffer, Amount, Descriptor + Addendum + Position );
		}
		void Preallocate( mdr::size__ Size )
		{
			if ( _Size() > Size )
				ERRc();
			else if ( _Size() != Size ) {
				Size -= _Size();
				row__ Row = _AppendNewFragment( Size );
				_SetAsFreeFragment( Row,Size );

				S_.TailingFreeFragmentPosition = Row;
			}
		}
		void DisplayStructure( txf::text_oflow__ &Flow ) const;
		friend uym::state__ Plug(
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
		void _Set( multimemory_ &Multimemory )
		{
			if ( _Multimemory != NULL )
				ERRc();

			_Multimemory = &Multimemory;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_untyped_memory_file_manager___::ReleaseFile();

			if ( P ) {
				Settle();
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
			const char *FileName,
			const char *FreeFragmentPositionsFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			flm::id__ ID )
		{
			reset();

			if ( ( _FreeFragmentPositionFileName = malloc( strlen( FreeFragmentPositionsFileName ) + 1 ) ) == NULL )
				ERRa();

			strcpy( _FreeFragmentPositionFileName, FreeFragmentPositionsFileName );

			_untyped_memory_file_manager___::Init( FileName, Mode, Persistent, ID );
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			return _untyped_memory_file_manager___::Mode( Mode );
		}
		fil::mode__ Mode( void ) const
		{
			return _untyped_memory_file_manager___::Mode();
		}
		uym::state__ Bind( void )	// A appeler seulement aprs un 'Plug(...)'.
		{
			uym::state__ State = uym::s_Undefined;
		ERRProlog
			flf::file_iflow___ IFlow;
		ERRBegin
			if ( ( State = _untyped_memory_file_manager___::Bind() ) == uym::sExists ) {
				IFlow.Init( _FreeFragmentPositionFileName );

				flw::Get( IFlow, _Multimemory->S_.FreeFragment );
				flw::Get( IFlow, _Multimemory->S_.TailingFreeFragmentPosition );
			}
		ERRErr
		ERREnd
		ERREpilog
			return State;
		}
		uym::state__ Settle( void )
		{
			uym::state__ State = _untyped_memory_file_manager___::Settle();

			if ( ( _Multimemory != NULL )
					&& _untyped_memory_file_manager___::IsPersistent()
					&& untyped_memory_file_manager___::Exists()
					&& ( !fil::FileExists( _FreeFragmentPositionFileName )
					    || ( untyped_memory_file_manager___::TimeStamp()
						    >= fil::GetFileLastModificationTime( _FreeFragmentPositionFileName ) ) ) )
				_WriteFreeFragmentPositions();

			return State;

		}
		friend uym::state__ Plug(
			multimemory_ &Multimemory,
			multimemory_file_manager___ &FileManager );
	};

	inline uym::state__ Plug(
		multimemory_ &Multimemory,
		multimemory_file_manager___ &FileManager )
	{
		uym::state__ State = uym::Plug( Multimemory.Memory, FileManager );

		FileManager._Set( Multimemory );

		Multimemory.SetSize( FileManager.UnderlyingSize() );

		return State;
	}

	class standalone_multimemory_driver__
	: public multimemory_driver__
	{
	private:
		descriptor__ _Descriptor;
		bso::ubyte__ _Addendum;
	public:
		void reset( bso::bool__ = true )
		{
			_Descriptor = MMM_UNDEFINED_DESCRIPTOR;
			_Addendum = 0;
		}
		standalone_multimemory_driver__( void )
		: multimemory_driver__( _Descriptor, _Addendum )
		{
			reset( false );
		}
	};


	#define E_MULTIMEMORY_DRIVER__	multimemory_driver__
	E_AUTO( multimemory )

	inline void multimemory_driver__::Liberer_( void )
	{
		if ( _Descriptor != MMM_UNDEFINED_DESCRIPTOR )
			Multimemoire_->Free( _Descriptor );
	}

	inline void multimemory_driver__::MDRAllocate( mdr::size__ Size )
	{
		_Descriptor = Multimemoire_->Reallocate( _Descriptor, Size, _Addendum );
	}

	inline mdr::size__ multimemory_driver__::MDRUnderlyingSize( void )
	{
		if ( _Descriptor != MMM_UNDEFINED_DESCRIPTOR )
			return Multimemoire_->Size( _Descriptor );
		else
			return 0;
	}

	inline void multimemory_driver__::MDRRecall(
		mdr::row_t__ Position,
		mdr::size__ Amount,
		mdr::datum__ *Buffer )
	{
		Multimemoire_->Read( _Descriptor, Position, Amount, Buffer, _Addendum );
	}
	// lit  partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
	inline void multimemory_driver__::MDRStore(
		const mdr::datum__ *Buffer,
		mdr::size__ Amount,
		mdr::row_t__ Position )
	{
		Multimemoire_->Write( Buffer, Amount, _Descriptor, Position, _Addendum );
	}
	// crit 'Nombre' octets  la position 'Position'

}

#endif


//d A multimemory.
#define E_MULTIMEMORY_	multimemory_
#define E_MULTIMEMORY	multimemory


#undef MMM__HANDLE_PART_ONE
#undef MMM__HANDLE_PART_TWO


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
