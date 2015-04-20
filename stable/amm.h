/*
	Header for the 'amm' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: amm.h,v 1.3 2013/03/09 12:22:11 csimon Exp $

#ifndef AMM__INC
#define AMM__INC

#define AMM_NAME		"AMM"

#define	AMM_VERSION	"$Revision: 1.3 $"

#define AMM_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &AMMTutor;

#if defined( E_DEBUG ) && !defined( AMM_NODBG )
#define AMM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/03/09 12:22:11 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Aggregated MeMory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "uym.h"

namespace amm {

	/*
	NOTA :
	- Par 'FragmentSize', on entend : 
		- pour les fragments occups (used), la taille disponible pour les donnes, et non pas la taille totale occupe par le fragement,
		- pour les fragments libres (free), la taille totale du fragment.
	- Un 'descriptor__' pointe sur le dbut des donnes. Les mtadonnes sot situs juste avant.
	*/		


	enum status__ {
		sFree,
		sUsed,
		s_amount,
		s_Undefined
	};

	enum flag_position__ {
		/*
		Statut du fragment : 
			- 0 : libre (free),
			- 1 : occup (used).
		*/
		fpStatus,
		/*
		- Pour n'importe quel type de fragment (libre ou occup), s'il est en premire position, statut du fragment en dernire position :
		- Pour un fragment occup, s'il n'est pas en premire position, statut de son prdecesseur :
			- 0 : libre (free),
			- 1 : occup (used).
		Pour un fragment libre, non situ en premire position, n'a pas de signification.
		*/
		fpPredecessorStatus,
		/*
		Type de la taille d'un fragment :
			- 0 : court (short) la taille du fragment tant encod dans les bits restant de l'octet,
			- 1 : long (large), la taille du fragment tant encod sur, au maximum, les 2^(valeurs des bits restants de l'octet).
		*/
		/*
			Les 5 bits qui suivent contiennent la taille (-1) du fragment lorsqu'elle est suffisament petit pour y tre contenue
			(elle est alors dite 'embedded') et qu'il s'agit d'un fragment libre (free).
		*/
		fp_SizeBegin,
		fp_SizeEnd = 6,
		/*
			Type de la taille :
				- 0 : longue (dynamique, 'long'), elle est stocke dans les octets qui suivent (nombre d'octets variables).
				- 1 : courte ('short') dans le 'header' (taille - 1).

			Pour des raisons de simplification, lorsque ce bit est  1 pour un fragment occup (used), alors la taille de ce fragment est
			de 1. Lorsque la taille d'un fragment occup est > 1, alors elle est stocke de manire dynamique.

			Ce bit sert galement de marqueur (lorsqu'il est  0) pour signaler que l'on est sur l'octet prcdent le premier
			d'une taille dynamique. Le 7me bit du dernier octet d'une taille dynamique est toujours  0. Les autres ont leur 7me bits  1.
		*/
		fpSizeType = 7,
		fp_amount
	};

	enum flag_ {
		fStatus = 1 << fpStatus,
		fPredecessorStatus = 1 << fpPredecessorStatus,
		fSizeType = 1 << fpSizeType,
		f_All = fStatus | fPredecessorStatus | fSizeType,
	};

# define AMM_HEADER_SIZE			sizeof( amm::header__ )
# define AMM_EMBEDDED_VALUE_MAX		( ~amm::f_All >> amm::fp_SizeBegin )
# define AMM_SHORT_SIZE_MAX			( AMM_EMBEDDED_VALUE_MAX + 1 )
# define AMM_LONG_SIZE_SIZE_MAX		sizeof( mdr::dsize__ )
# define AMM_FULL_HEADER_SIZE_MAX	( AMM_HEADER_SIZE + AMM_LONG_SIZE_SIZE_MAX )

	using mdr::size__;

	typedef mdr::datum__ header__;

	inline bso::bool__ IsUsed( header__ Header )
	{
		return ( Header & fStatus ) != 0;
	}

	inline bso::bool__ IsFree( header__ Header )
	{
		return !IsUsed( Header );
	}

	inline bso::bool__ IsSizeShort( header__ Header )
	{
		return ( Header & fSizeType ) != 0;
	}

	inline bso::bool__ IsSizeLong( header__ Header )
	{
		return !IsSizeShort( Header );
	}

	inline bso::bool__ IsPredecessorUsed( header__ Header )
	{
		return ( Header & fPredecessorStatus ) != 0;
	}

	inline bso::bool__ IsPredecessorFree( header__ Header )
	{
		return !IsPredecessorUsed( Header );
	}

	inline size__ GetEmbeddedValue( header__ Header )
	{
		return ( ( Header & ~f_All ) >> fp_SizeBegin );
	}

	inline size__ GetRawShortSize( header__ Header )
	{
		if ( !IsSizeShort( Header ) )
			ERRc();

		return GetEmbeddedValue( Header );
	}

	inline size__ GetShortSize( header__ Header )
	{
		return GetRawShortSize( Header ) + 1;
	}

	inline void MarkAsFree( header__ &Header )
	{
		Header &= ~fStatus;
	}

	inline void MarkAsUsed( header__ &Header )
	{
		Header |= fStatus;
	}

	inline void Mark(
		header__ &Header,
		status__ Status )
	{
		switch( Status ) {
		case sFree:
			MarkAsFree( Header );
			break;
		case sUsed:
			MarkAsUsed( Header );
			break;
		default:
			ERRc();
			break;
		}
	}

	inline void MarkPredecessorAsFree( header__ &Header )
	{
		Header &= ~fPredecessorStatus;
	}

	inline void MarkPredecessorAsUsed( header__ &Header )
	{
		Header |= fPredecessorStatus;
	}

	void MarkPredecessorStatus(
		header__ &Header,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			MarkPredecessorAsFree( Header );
			break;
		case sUsed:
			MarkPredecessorAsUsed( Header );
			break;
		default:
			ERRc();
			break;
		}
	}

	inline void MarkSizeAsShort( header__ &Header )
	{
		Header &= ~fSizeType;
	}

	inline void MarkSizeAsLong( header__ &Header )
	{
		Header |= fSizeType;
	}

	inline bso::bool__ CanValueBeEmbedded( size__ Value )
	{
		return Value <= AMM_EMBEDDED_VALUE_MAX;
	}

	inline bso::bool__ IsFreeFragmentSizeShortSuitable( size__ Size )
	{
		if ( Size == 0 )
			ERRc();

		return Size == 1;
	}

	inline bso::bool__ IsUsedFragmentSizeShortSuitable( size__ Size )
	{
		if ( Size == 0 )
			ERRc();

		return Size <= AMM_SHORT_SIZE_MAX;
	}

	inline bso::bool__ IsSizeShortSuitable(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return IsFreeFragmentSizeShortSuitable( Size );
			break;
		case sUsed:
			return IsUsedFragmentSizeShortSuitable( Size );
			break;
		default:
			ERRc();
			break;
		}

		return false;
	}

	inline size__ AdjustFreeFragmentLongSize( size__ Size )
	{
		if ( IsFreeFragmentSizeShortSuitable( Size ) )
			ERRc();

		return Size;
	}

	inline size__ AdjustUsedFragmentLongSize( size__ Size )
	{
		if ( IsUsedFragmentSizeShortSuitable( Size ) )
			ERRc();

		return Size - AMM_SHORT_SIZE_MAX;
	}

	inline size__ AdjustLongSize(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return AdjustFreeFragmentLongSize( Size );
			break;
		case sUsed:
			return AdjustUsedFragmentLongSize( Size );
			break;
		default:
			ERRc();
			break;
		}

		return 0;	// Pour viter un 'warning'.
	}

	inline void SetEmbeddedValue(
		header__ &Header,
		size__ Value )
	{
		if ( !CanValueBeEmbedded( Value ) )
			ERRc();

		Header = ( Header & f_All ) | ( (bso::ubyte__)Value >> fp_SizeBegin );
	}

	E_TMIMIC__( mdr::row_t__, descriptor__ );

	typedef mdr::xsize__ _xsize__;

	class xsize__ {
	private:
		_xsize__ _XSize;
		size__ _Size;
		status__ _Status;
	public:
		void reset( bso::bool__ P = true )
		{
			_XSize.reset( P );
			_Size = 0;
			_Status = s_Undefined;
		}
		void Init(
			size__ Size,
			status__ Status )
		{
			_Size = Size;

			if ( !IsSizeShortSuitable( Size, Status ) )
				_XSize = mdr::Convert( AdjustLongSize( Size, Status ) );

			_Status = Status;
		}
		void Init( const xsize__ &XSize )
		{
			*this = XSize;
		}
		size__ FragmentSize( void ) const
		{
			switch ( _Status ) {
			case sFree:
				return _Size;
				break;
			case sUsed:
				return _XSize.BufferSize() + AMM_HEADER_SIZE + _Size;
				break;
			default:
				ERRc();
				break;
			}

			return 0;	// Pour viter un 'warning'.
		}
		status__ Status( void ) const
		{
			return _Status;
		}
		size__ Size( void ) const
		{
			return _Size;
		}
		bso::bool__ IsShortSuitable( void ) const
		{
			return _XSize.BufferSize() == 0;
		}
		const mdr::datum__ *DSizeBuffer( void ) const
		{
			return _XSize.DSizeBuffer();
		}
		size__ DSizeBufferLength( void ) const
		{
			return _XSize.BufferSize();
		}
	};

	class xheader__
	: public xsize__
	{
	private:
		header__ _Header;
		void _SubInit( 
			status__ Status,
			status__ PredecessorStatus )
		{
			Mark( _Header, Status );
			amm::MarkPredecessorStatus( _Header, PredecessorStatus );

			if ( IsShortSuitable() ) {
				amm::MarkSizeAsShort( _Header );
				SetEmbeddedValue( _Header, xsize__::Size() - 1 );
			} else
				amm::MarkSizeAsShort( _Header );
		}
	public:
		void reset( bso::bool__  P = true )
		{
			xsize__::reset( P );
			_Header = 0;
		}
		E_CDTOR( xheader__ )
		void Init(
			size__ Size,
			status__ Status,
			status__ PredecessorStatus )
		{
			xsize__::Init( Size, Status );

			_SubInit( Status, PredecessorStatus );
		}
		void Init(
			const xsize__ &XSize,
			status__ PredecessorStatus )
		{
			xsize__::Init( XSize );

			_SubInit( XSize.Status(), PredecessorStatus );
		}
		const header__ &Header( void ) const
		{
			return _Header;
		}
	};


	inline const mdr::datum__ *FindLongSizeBegin( const mdr::datum__ *Buffer )
	{
		bso::ubyte__ Counter = AMM_LONG_SIZE_SIZE_MAX;

		if ( Buffer[--Counter] & fSizeType )
			ERRc();

		while ( Counter && ( Buffer[--Counter] & fSizeType ) );

		if ( Counter == 0 )
			ERRc();

		return Buffer + Counter + 1;
	}

	struct tracker__
	{
	public:
		mdr::row_t__ Row;
		size__ Size;
		void reset( bso::bool__ = true )
		{
			Row = NONE;
			Size = 0;
		}
		E_CDTOR( tracker__ );
		void Init( void )
		{
			reset();
		}
		void Init(
			mdr::row_t__ Row,
			size__ Size )
		{
			if ( ( Row ==  NONE ) || ( Size == 0 ) )
				ERRc();

			this->Row = Row;
			this->Size = Size;
		}
		bso::bool__ IsSuitable(
			size__ Size,
			bso::bool__ &All ) const
		{
			if ( Size > this->Size )
				return false;
			else {
				All = Size == this->Size;

				return true;
			}
		}
	};

	typedef mdr::datum__ meta_data__[AMM_FULL_HEADER_SIZE_MAX];


	class aggregate_memory_
	{
	private:
		mdr::size__ _Size( void ) const
		{
			return Memory.Size();
		}
		void _Read(
			mdr::row_t__ Position,
			size__ Size,
			mdr::datum__ *Data ) const
		{
			Memory.Recall( Size, Position, Data );
		}
		void _Write(
			const mdr::datum__ *Data,
			mdr::row_t__ Position,
			size__ Size )
		{
			Memory.Store( Data, Size, Position );
		}
		mdr::row_t__ _GetPriorMetaData(	// Retourne la position du dbut des mta-donnes.
			mdr::row_t__ Row,	// Si pointe sur le dbut d'un fragment (cas d'un fragment libre), est probablement sans signification.
			header__ &Header,
			size__ &Size ) const
		{
			mdr::datum__ Buffer[AMM_FULL_HEADER_SIZE_MAX];
			size__ Amount = ( Row < AMM_FULL_HEADER_SIZE_MAX ? Row : AMM_FULL_HEADER_SIZE_MAX );

			if ( Amount == 0 )
				ERRc();

			_Read( Row - Amount, Amount, Buffer );

			if ( Buffer[Size] & amm::fSizeType ) {
				Header = (header__)Buffer[Amount];
				Size = GetShortSize( Header );
				Row--;
			} else {
				const mdr::datum__ *Pointer = FindLongSizeBegin( &Buffer[Amount] );
				Size = mdr::Convert( Pointer );
				Header = *--Pointer;
				 Row -= &Buffer[Amount] - Pointer + 1;
			}

			return Row;
		}
		size__ _GetPriorSize( mdr::row_t__ Row ) const
		{
			size__ Size;
			header__ Header;

			_GetPriorMetaData( Row, Header, Size );

			return Size;
		}
		void _Get(
			mdr::row_t__ Row,
			header__ &Header ) const
		{
			_Read( Row, AMM_HEADER_SIZE, &Header );
		}
		void _Set(
			mdr::row_t__ Row,
			header__ Header )
		{
			_Write( &Header, Row, AMM_HEADER_SIZE );
		}
		bso::bool__ _IsTailingFragmentFree( void ) const
		{
			if ( _Size() == 0 )
				return false;
			else {
				header__ Header;

				_Get( 0, Header );

				return IsFree( Header );
			}
		}
		void _UpdatePredecessorStatus(
			mdr::row_t__ Row,
			status__ Status )	// Le statut du prdecesseur du premier fragment reflte en fait le statut du dernier fragment.
		{
			header__ Header;

			_Get( Row, Header );

			MarkPredecessorStatus( Header, Status );

			_Set( Row, Header );
		}
		void _UpdateFirstFragmentPredecessorStatus( status__ Status )	// Le statut du prdecesseur du premier fragment reflte en fait le statut du dernier fragment.
		{
			_UpdatePredecessorStatus( 0, Status );
		}
		size__ _GetTailingFreeSize( void ) const
		{
			if ( _IsTailingFragmentFree() )
				return _GetPriorSize( _Size() );
			else
				return 0;
		}
		mdr::row_t__ _GetTailingFreeFragment( void ) const
		{
			return _Size() - _GetTailingFreeSize();
		}
		size__ _GetLongSize( mdr::row_t__ Row ) const
		{
			mdr::dsize__ DSize;
			size__ Limit = _Size() - Row;

			_Read( Row, AMM_LONG_SIZE_SIZE_MAX > Limit ? Limit : AMM_LONG_SIZE_SIZE_MAX, (mdr::datum__ *)&DSize );

			return mdr::Convert( DSize );
		}
		void _GetMetaData(
			mdr::row_t__ Row,
			header__ &Header,
			size__ &Size ) const
		{
			_Get( Row, Header );

			if ( amm::IsSizeShort( Header ) )
				Size = amm::GetShortSize( Header );
			else
				Size = _GetLongSize( Row + AMM_HEADER_SIZE );
		}
		size__ _GetFragmentSize( mdr::row_t__ Row ) const
		{
			header__ Header;
			size__ Size = 0;

			_GetMetaData( Row, Header, Size );

			return Size;
		}
		bso::bool__ _IsLast( mdr::row_t__ Row ) const
		{
			return ( Row + _GetFragmentSize( Row ) ) == _Size();
		}
		void _WriteHeader(
			mdr::row_t__ Row,
			const header__ Header )
		{
			_Write( &Header, Row, AMM_HEADER_SIZE );
		}
		descriptor__ _WriteHeadMetaData(
			mdr::row_t__ Row,
			const xheader__ XHeader )
		{
			_WriteHeader( Row, XHeader.Header() );

			Row += AMM_HEADER_SIZE;

			if ( !XHeader.IsShortSuitable() ) {
				_Write( XHeader.DSizeBuffer(), Row, XHeader.DSizeBufferLength() );
				Row += XHeader.DSizeBufferLength();
			}

			return Row;
		}
		void _WriteTailMetaData(
			mdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			if ( XHeader.IsShortSuitable() )
				ERRc();

			switch ( XHeader.Size() ) {
			case 0:
				ERRc();
				break;
			case 1:
			case 2:
				break;
			default:
				_Write( (const mdr::datum__ *)"\x0", Row + XHeader.Size() - XHeader.DSizeBufferLength() - 1, 1 );
			case 3:
				_Write( XHeader.DSizeBuffer(), Row + XHeader.Size() - XHeader.DSizeBufferLength(), XHeader.DSizeBufferLength() );
				break;
			}
		}
		descriptor__ _SetFragment(
			mdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			if ( XHeader.Status() == sFree )
				_WriteTailMetaData( Row, XHeader );

			return _WriteHeadMetaData( Row, XHeader );
		}
		descriptor__ _SetFragment(
			mdr::row_t__ Row,
			const xsize__ &XSize,
			status__ PredecessorStatus )
		{
			xheader__ XHeader;

			XHeader.Init( XSize, PredecessorStatus );

			return _SetFragment( Row, XHeader );
		}
		void _SetAsFreeFragment(
			mdr::row_t__ Row,
			mdr::size__ Size,
			status__ PredecessorStatus )
		{
			xsize__ XSize;

			XSize.Init( Size, sFree );

			_SetFragment( Row, XSize, PredecessorStatus );
		}
		descriptor__ _AllocateAndSetUsedFragmentAtTail(
			const xsize__ &XSize,
			status__ PredecessorStatus,
			bso::bool__ &UsingTail )
		{
			size__ TailAvailableSize = _GetTailingFreeSize();
			mdr::row_t__ Row = _Size() - TailAvailableSize;

			UsingTail = TailAvailableSize != 0;

			if ( TailAvailableSize >= XSize.Size() )
				ERRc();

			Memory.Allocate( _Size() - TailAvailableSize + XSize.FragmentSize() );

			return _SetFragment( Row, XSize, PredecessorStatus );
		}
		descriptor__ _SetUsedFragmentUsingFreeFragment(
			mdr::row_t__ Row,
			const xsize__ &XSize,
			status__ PredecessorStatus )
		{
			size__ AvailableSize = _GetFragmentSize( Row );
			descriptor__ Descriptor = _SetFragment( Row, XSize, PredecessorStatus );

			if ( AvailableSize < XSize.Size() )
				ERRc();

			_SetAsFreeFragment( Row + XSize.FragmentSize(), AvailableSize - XSize.FragmentSize(), sUsed );

			return Descriptor;
		}
		mdr::row_t__ _GetUsableFreeFragmentIfAvailable(
			const xsize__ &XSize,
			bso::bool__ &All )
		{
			mdr::row_t__ Row = NONE;

			if ( S_.Free.IsSuitable( XSize.Size(), All ) ) {
				Row = S_.Free.Row;
				S_.Free.Init();
			}

			return Row;
		}
		descriptor__ _Allocate( mdr::size__ Size )
		{
			xsize__ XSize;
			mdr::row_t__ Row = NONE;
			descriptor__ Descriptor = NONE;
			bso::bool__ All = false;
			size__ Available = _GetTailingFreeSize();

			XSize.Init( Size, sUsed );

			if ( ( Row = _GetUsableFreeFragmentIfAvailable( XSize, All ) ) != NONE )
				if ( Available >= Size ) {
					Row = _GetTailingFreeFragment();
					All = Available == Size;
				}

			if ( Row != NONE ) {
				Descriptor = _SetUsedFragmentUsingFreeFragment( Row, XSize, sUsed );
			} else { 
				Descriptor = _AllocateAndSetUsedFragmentAtTail( XSize, sUsed, All );
			}

			if ( All )
				if ( ( Row == NONE ) || _IsLast( Row ) )
					_UpdateFirstFragmentPredecessorStatus( sUsed );
				else
					_UpdatePredecessorStatus( Row + XSize.FragmentSize(), sUsed );

			return Descriptor;
		}
		void _Free( descriptor__ Descriptor )
		{
			header__ Header;
			size__ Size;
			mdr::row_t__ Row = _GetPriorMetaData( *Descriptor, Header, Size );
			mdr::row_t__ Next = *Descriptor + Size;

			if ( !_IsLast( Row ) ) {
				mdr::row_t__ SuccessorRow = *Descriptor + Size;
				header__ SuccessorHeader;
				size__ SuccessorSize = 0;

				_GetMetaData( SuccessorRow, SuccessorHeader, SuccessorSize );

				if ( IsFree( SuccessorHeader ) ) {
					if ( S_.Free.Row == SuccessorRow )
						S_.Free.Init();

					Size += SuccessorSize;
				}
			}

			if ( ( Row != 0 ) && IsPredecessorFree( Header ) ) {
				size__ PredecessorSize = _GetPriorSize( Row );
				Size += PredecessorSize;
				Row -= PredecessorSize;

				if ( S_.Free.Row == ( Row - PredecessorSize ) )
					S_.Free.Init();
			}

			_SetAsFreeFragment( Row, Size, ( Row == 0
											 ? sFree
											 : _IsTailingFragmentFree() 
											   ? sFree
											   : sUsed ) );

			if ( _IsLast( Row ) )
				_UpdateFirstFragmentPredecessorStatus( sFree );
			else  {
				_UpdatePredecessorStatus( Row + Size, sFree );

				if ( S_.Free.Size < Size )
					S_.Free.Init( Row, Size );
			}
		}
	public:
		uym::untyped_memory_ Memory;
		struct s {
			uym::untyped_memory_::s Memory;
			tracker__ Free;	// Ne doit pas pointer sur le dernier fragment, mme s'il s'agit du seul.
		} &S_;
		aggregate_memory_( s &S )
		: S_( S ),
		  Memory( S.Memory )
		{}
		void reset( bso::bool__  P = true )
		{
			Memory.reset( P );
			S_.Free.reset( P );
		}
		aggregate_memory_ operator =( const aggregate_memory_ &AM )
		{
			Memory = AM.Memory;
			S_.Free = AM.S_.Free;

			return *this;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Memory.plug( MD );
		}
		void Init( void )
		{
			Memory.Init();
			S_.Free.Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
