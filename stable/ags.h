/*
	'ags.h' by Claude SIMON (http://zeusw.org/).

	'ags' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AGS__INC
# define AGS__INC

# define AGS_NAME		"AGS"

# if defined( E_DEBUG ) && !defined( AGS_NODBG )
#  define AGS_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// AGgregated Storage

# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "txf.h"
// # include "uys.h"	// déporté, parce 'ags.h' est inclus par 'uys.h'.

# ifdef UYS__INC
#  ifndef UYS__HEADER_HANDLED
#   define AGS__HANDLE_PART_ONE
#   undef AGS__INC
#  else
#   define AGS__HANDLE_PART_TWO
#  endif
# else
#  define AGS__HANDLE_PART_ONE
#  define AGS__HANDLE_PART_TWO
# endif

# ifdef AGS__HANDLE_PART_ONE
#  ifdef	AGS__PART_ONE_HANDLED
#   error
#  endif
#  ifdef	AGS__PART_TWO_HANDLED
#   error
#  endif

# define AGS_UNDEFINED_DESCRIPTOR	( (ags::descriptor__)E_NIL )

namespace ags {

	E_TMIMIC__( sdr::row_t__, descriptor__ );

	class aggregated_storage_;

	class aggregated_storage_driver__
	: public sdr::E_SDRIVER__
	{
	private:
		descriptor__ &_Descriptor;
		// memoire à laquelle il a été affecté
		class aggregated_storage_ *_AStorage;
		void _Free( void );
	protected:
		virtual void SDRAllocate( sdr::size__ Size );
		// Fonction déportée.
		virtual sdr::size__ SDRSize( void ) const;
		// fonction déportée
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer );
		// fonction déportée
		// écrit 'Nombre' octets à la position 'Position'
		virtual void SDRStore(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( _AStorage != NULL )
					_Free();
			} else
				_AStorage = NULL;

			E_SDRIVER__::reset( P );

			// On ne touche ni à '_Descriptor', ni à '_Addendum' car ils sont gèrés extèrieurement (ce sont des références).
		}
		aggregated_storage_driver__( descriptor__ &Descriptor )
		: _Descriptor( Descriptor ),
		  E_SDRIVER__()
		{
			reset( false );
		}
		E_VDTOR( aggregated_storage_driver__ )
		void Init( aggregated_storage_ &AStorage )
		{
			reset();

			_AStorage = &AStorage;
			E_SDRIVER__::Init();

			// On ne touche ni à '_Descriptor', ni à '_Addendum' car ils sont gèrés extèrieurement (ce sont des références).
		}
		descriptor__ Descriptor( void ) const
		{
			return _Descriptor;
		}
		aggregated_storage_ *AStorage( void ) const
		{
			return _AStorage;
		}
	};

}

#  define AGS__PART_ONE_HANDLED
# endif

# ifdef AGS__HANDLE_PART_TWO
#  ifndef AGS__PART_ONE_HANDLED
#   error
#  endif
#  ifdef AGS__PART_TWO_HANDLED
#   error
# endif

# include "uys.h"

namespace ags {

	/*
	NOTA :
	- Par 'Size', on entend : 
		- pour les fragments occupés ('used'), la taille disponible pour les données, et non pas la taille totale occupée par le fragment,
		- pour les fragments libres ('free'), la taille totale du fragment.
	- Un descripteur ('descriptor__') pointe sur le début des données. Les métadonnées sont situées juste avant. Seuls les fragment occupés ('used') ont un descripteur.
	- Une 'Value' est une donnée brute, sans ajustement, telle que stockée.
	*/		


	enum status__ {
		sFree,
		sUsed,
		s_amount,
		s_Undefined
	};

	inline void Display(
		status__ Status,
		txf::text_oflow__ &Flow )
	{
		switch ( Status ) {
		case sFree:
			Flow << 'F';
			break;
		case sUsed:
			Flow << 'U';
			break;
		default:
			ERRPrm();
			break;
		}
	}

	enum flag_position__ {
		/*
		Statut du fragment : 
			- 0 : libre ('free'),
			- 1 : occupé ('used').
		*/
		fpStatus,
		/*
		- Pour n'importe quel type de fragment (libre ou occupé), s'il est en première position, statut du fragment en dernière position :
		- Pour un fragment occupé, s'il n'est pas en première position, statut de son prédecesseur :
			- 0 : libre ('free'),
			- 1 : occupé ('used').
		Pour un fragment libre, non situé en première position, n'a pas de signification (les fragments libres étant toujours fusionnés,
		le fragment précédent un fragment libre est toujours un fragment occupé).
		*/
		fpPredecessorStatus,
		/*
			Les 5 bits qui suivent contiennent la taille (-1) du fragment lorsqu'elle est suffisament petit pour y être contenue
			(elle est alors dite 'embedded') et qu'il s'agit d'un fragment libre (free).
		*/
		fp_SizeBegin,
		fp_SizeEnd = 6,
		/*
			Type de la taille :
				- 0 : longue (dynamique, 'long'), elle est stockée dans les octets qui suivent (nombre d'octets variables).
				- 1 : courte ('short') dans le 'header' (taille - 1).

			Pour des raisons de simplification, lorsque ce bit est à 1 pour un fragment libre, alors la taille de ce fragment est
			de 1. Lorsque la taille d'un fragment libre est > 1, alors elle est stockée de manière dynamique.

			Ce bit sert également de marqueur (lorsqu'il est à 0) pour signaler que l'on est sur l'octet précédent le premier
			d'une taille dynamique. Le 7ème bit du dernier octet d'une taille dynamique est toujours à 0. Les autres ont leur 7ème bits à 1.
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

# define AGS_HEADER_SIZE			sizeof( ags::header__ )
# define AGS__HEADER_SIZE			1	// Lorsque cette valeur change, permet de détecter le code qu'il faut modifier.
# define AGS_EMBEDDED_VALUE_MAX		( (bso::u8__)~ags::f_All >> ags::fp_SizeBegin )
# define AGS_SHORT_SIZE_MAX			( AGS_EMBEDDED_VALUE_MAX + 1 )
# define AGS_LONG_SIZE_SIZE_MAX		sizeof( bso::dint__ )
# define AGS_XHEADER_SIZE_MAX		( AGS_HEADER_SIZE + AGS_LONG_SIZE_SIZE_MAX )

	using sdr::size__;

	typedef bso::int__ value__;

	E_TMIMIC__( sdr::datum__, header__ );

	inline bso::bool__ IsUsed( header__ Header )
	{
		return ( *Header & fStatus ) != 0;
	}

	inline bso::bool__ IsFree( header__ Header )
	{
		return !IsUsed( Header );
	}

	inline status__ Status( header__ Header )
	{
		return IsUsed( Header ) ? sUsed : sFree;
	}

	inline bso::bool__ IsSizeShort( header__ Header )
	{
		return ( *Header & fSizeType ) != 0;
	}

	inline bso::bool__ IsSizeLong( header__ Header )
	{
		return !IsSizeShort( Header );
	}

	inline bso::bool__ IsPredecessorUsed( header__ Header )
	{
		return ( *Header & fPredecessorStatus ) != 0;
	}

	inline bso::bool__ IsPredecessorFree( header__ Header )
	{
		return !IsPredecessorUsed( Header );
	}

	inline status__ PredecessorStatus( header__ Header )
	{
		return IsPredecessorUsed( Header ) ? sUsed : sFree;
	}

	inline value__ GetEmbeddedValue( header__ Header )
	{
		return ( ( *Header & ~f_All ) >> fp_SizeBegin );
	}

	inline value__ GetShortValue( header__ Header )
	{
		if ( !IsSizeShort( Header ) )
			ERRPrm();

		return GetEmbeddedValue( Header );
	}

	inline size__ GetShortSize( header__ Header )
	{
		return GetShortValue( Header ) + 1;
	}

	inline size__ ConvertValueToFreeFragmentLongSize( value__ Value )
	{
		if ( Value < 1 )
			ERRPrm();

		return Value;
	}

	inline size__ ConvertValueToUsedFragmentLongSize( value__ Value )
	{
		return Value + AGS_SHORT_SIZE_MAX + 1;
	}

	inline size__ ConvertValueToLongSize(
		value__ Value,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return ConvertValueToFreeFragmentLongSize( Value );
			break;
		case sUsed:
			return ConvertValueToUsedFragmentLongSize( Value );
			break;
		default:
			ERRPrm();
			break;
		}

		return 0;	// Pour éviter un 'Warning'.
	}

	inline void Display(
		header__ Header,
		status__ PStatus,
		txf::text_oflow__ &Flow )
	{
		Display( Status( Header ), Flow );

		if ( IsSizeLong( Header ) )
			Flow << 'L';
		else if ( IsSizeShort( Header ) )
			Flow << 'S';
		else
			ERRPrm();

		Flow << '(';

		Display( PredecessorStatus( Header ), Flow );

		Flow << ')';

		if ( PredecessorStatus( Header ) != PStatus ) 
		{
			Flow << txf::commit;
			ERRFwk();
		}
	}

	inline void MarkAsFree( header__ &Header )
	{
		*Header &= ~fStatus;
	}

	inline void MarkAsUsed( header__ &Header )
	{
		*Header |= fStatus;
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
			ERRPrm();
			break;
		}
	}

	inline void MarkPredecessorAsFree( header__ &Header )
	{
		*Header &= ~fPredecessorStatus;
	}

	inline void MarkPredecessorAsUsed( header__ &Header )
	{
		*Header |= fPredecessorStatus;
	}

	inline void MarkPredecessorStatus(
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
			ERRPrm();
			break;
		}
	}

	inline void MarkSizeAsShort( header__ &Header )
	{
		*Header |= fSizeType;
	}

	inline void MarkSizeAsLong( header__ &Header )
	{
		*Header &= ~fSizeType;
	}

	inline bso::bool__ CanValueBeEmbedded( value__ Value )
	{
		return Value <= AGS_EMBEDDED_VALUE_MAX;
	}

	inline bso::bool__ IsFreeFragmentSizeShortSuitable( size__ Size )
	{
		if ( Size == 0 )
			ERRPrm();

		return Size == 1;
	}

	inline bso::bool__ IsUsedFragmentSizeShortSuitable( size__ Size )
	{
		if ( Size == 0 )
			ERRPrm();

		return Size <= AGS_SHORT_SIZE_MAX;
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
			ERRPrm();
			break;
		}

		return false;
	}

	inline value__ ConvertFreeFragmentLongSizeToValue( size__ Size )
	{
		if ( IsFreeFragmentSizeShortSuitable( Size ) )
			ERRPrm();

		return Size;
	}

	inline value__ ConvertUsedFragmentLongSizeToValue( size__ Size )
	{
		if ( IsUsedFragmentSizeShortSuitable( Size ) )
			ERRPrm();

		return Size - AGS_SHORT_SIZE_MAX - 1;
	}

	inline value__ ConvertLongSizeToValue(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return ConvertFreeFragmentLongSizeToValue( Size );
			break;
		case sUsed:
			return ConvertUsedFragmentLongSizeToValue( Size );
			break;
		default:
			ERRPrm();
			break;
		}

		return 0;	// Pour éviter un 'warning'.
	}

	inline value__ ConvertFreeFragmentShortSizeToValue( size__ Size )
	{
		if ( !IsFreeFragmentSizeShortSuitable( Size ) )
			ERRPrm();

		return 0;	// Pour un fragment libre, seule une taille de 1 peut être convertit en 'short'.
	}

	inline value__ ConvertUsedFragmentShortSizeToValue( size__ Size )
	{
		if ( !IsUsedFragmentSizeShortSuitable( Size ) )
			ERRPrm();

		return Size - 1;
	}

	inline value__ ConvertShortSizeToValue(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return ConvertFreeFragmentShortSizeToValue( Size );
			break;
		case sUsed:
			return ConvertUsedFragmentShortSizeToValue( Size );
			break;
		default:
			ERRPrm();
			break;
		}

		return 0;	// Pour éviter un 'warning'.
	}

	inline void SetEmbeddedValue(
		header__ &Header,
		value__ Value )
	{
		if ( !CanValueBeEmbedded( Value ) )
			ERRPrm();

		Header = ( *Header & f_All ) | ( (bso::u8__)Value << fp_SizeBegin );
	}

	class xsize__ {
	private:
		bso::xint__ _XSize;
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
			reset();

			_Size = Size;

			if ( !IsSizeShortSuitable( Size, Status ) )
				bso::ConvertToDInt( ConvertLongSizeToValue( Size, Status ), _XSize );

			_Status = Status;
		}
		void Init( const xsize__ &XSize )
		{
			reset();

			*this = XSize;
		}
		size__ FragmentSize( void ) const
		{
			switch ( _Status ) {
			case sFree:
				return _Size;
				break;
			case sUsed:
				return _XSize.BufferSize() + AGS_HEADER_SIZE + _Size;
				break;
			default:
				ERRPrm();
				break;
			}

			return 0;	// Pour éviter un 'warning'.
		}
		size__ MetaDataSize( void ) const
		{
			return _XSize.BufferSize() + AGS_HEADER_SIZE;
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
		const sdr::datum__ *DSizeBuffer( void ) const
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
			MarkPredecessorStatus( _Header, PredecessorStatus );

			if ( IsShortSuitable() ) {
				MarkSizeAsShort( _Header );
				SetEmbeddedValue( _Header, ConvertShortSizeToValue( xsize__::Size(), Status ) );
			} else
				MarkSizeAsLong( _Header );
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


	inline const sdr::datum__ *FindLongSizeBegin( const sdr::datum__ *Buffer )
	{
		bso::u8__ Counter = AGS_LONG_SIZE_SIZE_MAX + 1;

		if ( *Buffer & fSizeType )
			ERRPrm();

		do {
			Buffer--;
			Counter--;
		} while ( Counter && ( *Buffer & fSizeType ) );

		if ( Counter == 0 )
			ERRDta();

		return Buffer + 1;
	}

	// Caratéristiques d'un fragement libre ('free').
	struct tracker__
	{
	public:
		sdr::row_t__ Row;
		size__ Size;
		void reset( bso::bool__ = true )
		{
			Row = E_NIL;
			Size = 0;
		}
		E_CDTOR( tracker__ );
		void Init( void )
		{
			reset();
		}
		void Init(
			sdr::row_t__ Row,
			size__ Size )
		{
			if ( ( Row ==  E_NIL ) || ( Size == 0 ) )
				ERRPrm();

			this->Row = Row;
			this->Size = Size;
		}
		bso::bool__ IsSuitable( size__ Size ) const
		{
			return Size <= this->Size;
		}
	};

	// Récupère les méta-données dont 'Pointer' pointe sur le dernier octet.
	// Retourne la taille du 'xheader'.
	// NOTA : La valeur retournée ainsi que le contenu de 'Header' sont à ignorer lorsque 'Pointer' pointe sur le dernier octet d'un fragment libre ('free').
	inline size__ GetPriorMetaData(
		const sdr::datum__ *Pointer,
		status__ Status,
		header__ &Header,
		size__ &Size )
	{
# if AGS__HEADER_SIZE != 1
/*
NOTA : Le code de cette fonction part du principe que la taille d'un 'header__' est de 1 octet.
Si ce n'est plus le cas, alors il faut modifier cette fonction.
*/
#  error 
# endif
		if ( *Pointer & fSizeType ) {
			Header = (header__)*Pointer;
			Size = GetShortSize( Header );
			if ( ags::Status( Header ) != Status )
				ERRPrm();
			return AGS_HEADER_SIZE;
		} else {
			const sdr::datum__ *LongSizePointer = FindLongSizeBegin( Pointer );
			Size = ConvertValueToLongSize( bso::ConvertToInt( LongSizePointer ), Status );
			Header = *--LongSizePointer;

			if ( ( Status == sUsed ) && ( ags::Status( Header ) != sUsed ) )
				ERRPrm();

			return Pointer - LongSizePointer + 1;
		}
	}

	// Permet d'éviter d'avoir à récupèrer le 'header' équivalent à 'Row' de multiple fois.
	struct bundle__ {
		sdr::row_t__ Row;
		header__ Header;
		void reset( bso::bool__ P = true )
		{
			Row = E_NIL;
			Header = 0;
		}
		E_CDTOR( bundle__ );
		void Init( void )
		{
			reset();
		}
	};

	class aggregated_storage_
	{
	private:
		sdr::size__ _Size( void ) const
		{
			return Storage.Size();
		}
		void _Read(
			sdr::row_t__ Row,
			size__ Size,
			sdr::datum__ *Data ) const
		{
			Storage.Recall( Row, Size, Data );
		}
		void _Write(
			const sdr::datum__ *Data,
			sdr::row_t__ Row,
			size__ Size )
		{
			Storage.Store( Data, Size, Row );
		}
		// Récupère les méta-données placés juste avant 'Row'.
		// Retourne la taille du 'xheader'
		// NOTA : la valeur retournée ainsi que le contenu de 'Header' sont à ignorer lorsque 'Row' pointe juste aprés un fragment libre ('free').
		size__ _GetPriorMetaData(
			sdr::row_t__ Row,
			status__ Status,
			header__ &Header,
			size__ &Size ) const
		{
			sdr::datum__ Buffer[AGS_XHEADER_SIZE_MAX];
			size__ Amount = ( Row < sizeof( Buffer ) ? Row : sizeof( Buffer ) );
			sdr::datum__ *Pointer = &Buffer[Amount-1];

			if ( Amount == 0 )
				ERRPrm();

			_Read( Row - Amount, Amount, Buffer );

			return GetPriorMetaData( Pointer, Status, Header, Size );
		}
		size__ _GetPriorMetaData(
			descriptor__ Descriptor,
			header__ &Header,
			size__ &Size ) const
		{
			return _GetPriorMetaData( *Descriptor, sUsed, Header, Size );
		}
		size__ _GetPriorSize(
			sdr::row_t__ Row,
			status__ Status ) const
		{
			size__ Size;
			header__ Header;

			_GetPriorMetaData( Row, Status, Header, Size );

			return Size;
		}
		size__ _GetPriorSize( descriptor__ Descriptor ) const
		{
			return _GetPriorSize( *Descriptor, sUsed );
		}
		void _Get(
			sdr::row_t__ Row,
			header__ &Header ) const
		{
			_Read( Row, AGS_HEADER_SIZE, &*Header );
		}
		void _Set(
			sdr::row_t__ Row,
			header__ Header )
		{
			_Write( &*Header, Row, AGS_HEADER_SIZE );
		}
		status__ _TailFragmentStatus( void ) const
		{
			if ( _Size() == 0 )
				return s_Undefined;
			else {
				header__ Header;

				_Get( 0, Header );

				return PredecessorStatus( Header );
			}
		}
		bso::bool__ _IsTailFragmentFree( void ) const
		{
			if ( _Size() == 0 )
				return false;
			else
				return _TailFragmentStatus() == sFree;
		}
		void _UpdatePredecessorStatus(
			sdr::row_t__ Row,
			status__ Status )	// Le statut du prédecesseur du premier fragment reflète le statut du dernier fragment.
		{
			header__ Header;

			_Get( Row, Header );

			MarkPredecessorStatus( Header, Status );

			_Set( Row, Header );
		}
		void _UpdateFirstFragmentPredecessorStatus( status__ Status )	// Le statut du prédecesseur du premier fragment reflète le statut du dernier fragment.
		{
			_UpdatePredecessorStatus( 0, Status );
		}
		size__ _GetTailFreeFragmentSize( void ) const
		{
			if ( _IsTailFragmentFree() )
				return _GetPriorSize( _Size(), sFree );
			else
				return 0;
		}
		sdr::row_t__ _GetTailFreeFragment( void ) const
		{
			if ( _GetTailFreeFragmentSize() == 0 )
				return E_NIL;
			else
				return _Size() - _GetTailFreeFragmentSize();
		}
		size__ _GetLongSize(
			sdr::row_t__ Row,
			status__ Status,
			sdr::size__ &SizeLength ) const
		{
			bso::dint__ DSize;
			size__ Limit = _Size() - Row;

			_Read( Row, sizeof( DSize ) > Limit ? Limit : sizeof( DSize ), (sdr::datum__ *)&DSize );

			return ConvertValueToLongSize( bso::ConvertToInt( DSize, SizeLength ), Status );
		}
		void _GetMetaData(
			sdr::row_t__ Row,
			header__ &Header,
			size__ &Size,
			size__ &XHeaderLength ) const
		{
			_Get( Row, Header );
			size__ SizeLength = 0;

			if ( IsSizeShort( Header ) )
				Size = GetShortSize( Header );
			else
				Size = _GetLongSize( Row + AGS_HEADER_SIZE, Status( Header), SizeLength );

			XHeaderLength = SizeLength + AGS_HEADER_SIZE;
		}
		void _GetMetaData(
			sdr::row_t__ Row,
			header__ &Header,
			size__ &Size ) const
		{
			size__ Dummy = 0;

			_GetMetaData( Row, Header, Size, Dummy );
		}
		size__ _GetFragmentSize( sdr::row_t__ Row ) const
		{
			header__ Header;
			size__ Size = 0;
			size__ XHeaderLength = 0;

			_GetMetaData( Row, Header, Size, XHeaderLength );

			return Size + ( IsUsed( Header ) ? XHeaderLength : 0 );
		}
		size__ _GetSize( descriptor__ Descriptor ) const
		{
			return _GetPriorSize( Descriptor );
		}
		bso::bool__ _IsLast( sdr::row_t__ Row ) const
		{
			return ( Row + _GetFragmentSize( Row ) ) == _Size();
		}
		void _WriteHeader(
			sdr::row_t__ Row,
			header__ Header )
		{
			_Write( &*Header, Row, AGS_HEADER_SIZE );
		}
		descriptor__ _WriteHeadMetaData(
			sdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			_WriteHeader( Row, XHeader.Header() );

			Row += AGS_HEADER_SIZE;

			if ( !XHeader.IsShortSuitable() ) {
				_Write( XHeader.DSizeBuffer(), Row, XHeader.DSizeBufferLength() );
				Row += XHeader.DSizeBufferLength();
			}

			return Row;
		}
		void _WriteTailMetaData(
			sdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			if ( XHeader.Status() != sFree )
				ERRPrm();

			switch ( XHeader.FragmentSize() ) {
			default:
				// On écrit '0' pour que le marqueur précédent le début du 'long size' soit positionné (8ème bit à 0).
				_Write( (const sdr::datum__ *)"\x0", Row + XHeader.FragmentSize() - XHeader.DSizeBufferLength() - 1, 1 );
			case 3:
				// Si l'on saute directement ici, le 8ème bit de l'octet précédent le début du 'long size' est à 0 parce qu'il s'agit du dernier octet d'un entier dynamique.
			case 2:
				// Si l'on saute directement ici, le 8ème bit de l'octet précédent le début du 'long size' est à 0 parce qu'il s'agit du 'header' (fanion signalant un 'long size' lorsque à 0).
				_Write( XHeader.DSizeBuffer(), Row + XHeader.FragmentSize() - XHeader.DSizeBufferLength(), XHeader.DSizeBufferLength() );
				break;
			case 1:
				// Dans ce cas, compte tenu de la taille du fragment, le 'tail meta data' est constitué du 'header' du 'head meta data'.
				break;
			case 0:
				ERRPrm();
				break;
			}
		}
		descriptor__ _SetFragment(
			sdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			if ( XHeader.Status() == sFree )
				_WriteTailMetaData( Row, XHeader );

			return _WriteHeadMetaData( Row, XHeader );
		}
		descriptor__ _SetFragment(
			sdr::row_t__ Row,
			const xsize__ &XSize,
			status__ PredecessorStatus )
		{
			xheader__ XHeader;

			XHeader.Init( XSize, PredecessorStatus );

			return _SetFragment( Row, XHeader );
		}
		void _SetAsFreeFragment(
			sdr::row_t__ Row,
			sdr::size__ Size,
			status__ PredecessorStatus )	// Si 'Row' != 0, alors ce paramètre n'est pas significatif.
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
			size__ TailAvailableSize = _GetTailFreeFragmentSize();
			sdr::row_t__ Row = _Size() - TailAvailableSize;	// On stocke dans une variable, car '_Size()' est modifié par 'Allocate(...)'.

			UsingTail = TailAvailableSize != 0;

			if ( TailAvailableSize >= XSize.FragmentSize() )
				ERRPrm();

			Storage.Allocate( _Size() - TailAvailableSize + XSize.FragmentSize() );

			return _SetFragment( Row, XSize, PredecessorStatus );
		}
		descriptor__ _SetUsedFragmentUsingFreeFragment(
			sdr::row_t__ Row,
			const xsize__ &XSize,
			status__ PredecessorStatus,
			bso::bool__ &All )
		{
			size__ AvailableSize = _GetFragmentSize( Row );
			descriptor__ Descriptor = _SetFragment( Row, XSize, PredecessorStatus );
			All = false;

			if ( AvailableSize > XSize.FragmentSize() )
				_SetAsFreeFragment( Row + XSize.FragmentSize(), AvailableSize - XSize.FragmentSize(), sUsed );
			else if ( AvailableSize < XSize.FragmentSize() )
				ERRPrm();
			else
				All = true;

			return Descriptor;
		}
		sdr::row_t__ _GetUsableFreeFragmentIfAvailable( size__ Size )
		{
			sdr::row_t__ Row = E_NIL;

			if ( S_.Free.IsSuitable( Size ) ) {
				Row = S_.Free.Row;
				S_.Free.Init();
			}

			return Row;
		}
		descriptor__ _Allocate( sdr::size__ Size )
		{
			xsize__ XSize;
			sdr::row_t__ Row = E_NIL;
			descriptor__ Descriptor = E_NIL;
			bso::bool__ All = false;

			XSize.Init( Size, sUsed );

			if ( ( Row = _GetUsableFreeFragmentIfAvailable( XSize.FragmentSize() ) ) == E_NIL )
				if ( _GetTailFreeFragmentSize() >= XSize.FragmentSize() )
					Row = _GetTailFreeFragment();

			if ( Row != E_NIL ) {
				Descriptor = _SetUsedFragmentUsingFreeFragment( Row, XSize, ( Row == 0 ? _TailFragmentStatus() : sUsed ), All );	// Le cas où l'on utilise l'intégralité du 'TailFragment' (auquel cas '_TaileFragmentStatus()'
																																	// va changer de valeur) sera traité ci-dessous, grâce à la valeur de 'All'.		
			} else { 
				if ( ( Row = _GetTailFreeFragment() ) == E_NIL )
					Row = _Size();
				Descriptor = _AllocateAndSetUsedFragmentAtTail( XSize, sUsed, All );
			}

			if ( All ) {
				if ( _IsLast( Row ) )
					_UpdateFirstFragmentPredecessorStatus( sUsed );
				else
					_UpdatePredecessorStatus( Row + XSize.FragmentSize(), sUsed );
			}

			return Descriptor;
		}
		sdr::row_t__ _GetFragmentRow( descriptor__ Descriptor ) const	// Retourne la position à laquelle débute le fragemnt correspondant à 'Descriptor'.
		{
			header__ Header;
			size__ Size = 0;

			return *Descriptor - _GetPriorMetaData( Descriptor, Header, Size );
		}
		bso::bool__ _IsLast( descriptor__ Descriptor )	const	// Retourne 'true' si le fragment correspondant est le dernier de tous les fragments.
		{
			return _IsLast( _GetFragmentRow( Descriptor ) );
		}
		bso::bool__ _IsLastUsed( descriptor__ Descriptor )	const	/* Retourne 'true' si le fragment correspondant est le dernier de tous les fragments,
																	ou n'est suivi que d'un fragment libre qui est, lui, le dernier de tous les fragments. */
		{
			if ( _IsLast( Descriptor) )
				return true;
			else {
				header__
					Header = 0,
					SuccessorHeader = 0;
				size__
					Size = 0,
					SuccessorSize = 0;
				sdr::row_t__ SuccessorRow = E_NIL;

				_GetPriorMetaData( Descriptor, Header, Size );

				SuccessorRow = *Descriptor + Size;

				_GetMetaData( SuccessorRow, SuccessorHeader, SuccessorSize );

				return IsFree( SuccessorHeader ) && _IsLast( SuccessorRow );
			}
		}
		sdr::size__ _GetResultingFreeFragmentSizeIfFreed(
			descriptor__ Descriptor,
			sdr::row_t__ &Row ) const	/* Retourne la taille du fragment, en fusionnant avec les éventuels fragments libres et précédants et suivants,
										   si 'Descriptor' est libèré. Retourne le 'Row' sur le début du fragment. */
		{
			header__ Header;
			size__ FragmentSize;
			sdr::size__ XHeaderLength = _GetPriorMetaData( Descriptor, Header, FragmentSize );

			Row = *Descriptor - XHeaderLength;

			FragmentSize += XHeaderLength;

			if ( !_IsLast( Row ) ) {
				sdr::row_t__ SuccessorRow = Row + FragmentSize;
				header__ SuccessorHeader;
				size__ SuccessorSize = 0;

				_GetMetaData( SuccessorRow, SuccessorHeader, SuccessorSize );

				if ( IsFree( SuccessorHeader ) )
					FragmentSize += SuccessorSize;
			}

			if ( ( Row != 0 ) && IsPredecessorFree( Header ) ) {
				size__ PredecessorSize = _GetPriorSize( Row, sFree );

				FragmentSize += PredecessorSize;
				Row -= PredecessorSize;
			}

			return FragmentSize;
		}
		sdr::size__ _GetResultingFreeFragmentSizeIfFreed( descriptor__ Descriptor ) const
		{
			sdr::row_t__ Row = E_NIL;

			return _GetResultingFreeFragmentSizeIfFreed( Descriptor, Row );
		}
		descriptor__ _Reallocate(
			descriptor__ OldDescriptor,
			sdr::size__ NewSize )
		{
			descriptor__ NewDescriptor = E_NIL;
			sdr::size__ AvailableFragmentSize = _GetResultingFreeFragmentSizeIfFreed( OldDescriptor );
			xsize__ XSize;

			XSize.Init( NewSize, sUsed );

			if ( XSize.FragmentSize() >= AvailableFragmentSize )  {
				if ( !_IsLastUsed( OldDescriptor ) ) {
					sdr::size__ OldSize = _GetSize( OldDescriptor );

					NewDescriptor = _Allocate( NewSize );

					Storage.Store( Storage, OldSize > NewSize ? NewSize : OldSize, *NewDescriptor, *OldDescriptor );

					_Free( OldDescriptor );
				} else {
					header__ OldHeader;
					size__ OldSize;
					sdr::size__ OldXHeaderLength = _GetPriorMetaData( OldDescriptor, OldHeader, OldSize );
					xheader__ NewXHeader;
					sdr::row_t__
						OldRow = *OldDescriptor - OldXHeaderLength,
						NewRow = E_NIL;

					if ( ( OldRow + OldSize + OldXHeaderLength ) == S_.Free.Row )
						S_.Free.Init();

					if ( IsPredecessorFree( OldHeader ) && ( OldRow != 0 ) )
						NewRow = OldRow - _GetPriorSize( OldRow, sFree );
					else
						NewRow = OldRow;

					if ( S_.Free.Row == NewRow )
						S_.Free.Init();

					NewXHeader.Init( NewSize, sUsed, sUsed );

					Storage.Allocate( NewRow + NewXHeader.FragmentSize() );

					NewDescriptor = NewRow + NewXHeader.MetaDataSize();

					Storage.Store( Storage, OldSize, *NewDescriptor, *OldDescriptor );

					_WriteHeadMetaData( NewRow, NewXHeader );

					_UpdateFirstFragmentPredecessorStatus( sUsed );
				}
			} else {
				header__ OldHeader;
				size__ OldSize;
				sdr::size__ OldXHeaderLength = _GetPriorMetaData( OldDescriptor, OldHeader, OldSize );
				xheader__ NewXHeader;
				sdr::row_t__
					OldRow = *OldDescriptor - OldXHeaderLength,
					NewRow = E_NIL;

				if ( ( *OldDescriptor + OldSize ) == S_.Free.Row )
					S_.Free.Init();

				if ( IsPredecessorFree( OldHeader ) && ( OldRow != 0 ) )
					NewRow = OldRow - _GetPriorSize( OldRow, sFree );
				else
					NewRow = OldRow;

				if ( S_.Free.Row == NewRow )
					S_.Free.Init();

				NewXHeader.Init( NewSize, sUsed, NewRow == 0 ? ( !_IsLast( OldDescriptor ) ? _TailFragmentStatus() : sFree ) : sUsed );

				NewDescriptor = NewRow + NewXHeader.MetaDataSize();

				Storage.Store( Storage, OldSize > NewSize ? NewSize : OldSize, *NewDescriptor, *OldDescriptor );

				_WriteHeadMetaData( NewRow, NewXHeader );

				if ( AvailableFragmentSize > NewXHeader.FragmentSize() )  {
					sdr::row_t__ FreeSuccessorRow = NewRow + NewXHeader.FragmentSize();
					size__ FreeSuccessorSize = AvailableFragmentSize - NewXHeader.FragmentSize();

					_SetAsFreeFragment( FreeSuccessorRow, FreeSuccessorSize, sUsed );

					if ( ( NewRow + AvailableFragmentSize ) < Storage.Size() )
						_UpdatePredecessorStatus( NewRow + AvailableFragmentSize, sFree );

					if ( S_.Free.Size < FreeSuccessorSize )
						S_.Free.Init( FreeSuccessorRow, FreeSuccessorSize );
				} else
					ERRFwk();
			}

			return NewDescriptor;
		}
		void _Free( descriptor__ Descriptor )
		{
			sdr::row_t__ Row = E_NIL;
			size__ Size = _GetResultingFreeFragmentSizeIfFreed( Descriptor, Row );

			_SetAsFreeFragment( Row, Size, ( Row == 0 ? ( _IsLast( Descriptor ) ? sFree : _TailFragmentStatus() ) : sUsed ) );

			if ( _IsLast( Row ) )
				_UpdateFirstFragmentPredecessorStatus( sFree );
			else  {
				_UpdatePredecessorStatus( Row + Size, sFree );

				if ( S_.Free.Size < Size )
					S_.Free.Init( Row, Size );
			}
		}
		void _Display(
			sdr::row_t__ Row,
			status__ PStatus,
			txf::text_oflow__ &Flow ) const
		{
			header__ Header;
			sdr::size__ Size, XHeaderLength;

			Flow << Row << ' ';

			_GetMetaData( Row, Header, Size, XHeaderLength );
				
			_Get( Row, Header );

			Display( Header, PStatus, Flow );

			Flow << " : "<<  Size;

			if ( IsUsed( Header ) )
				Flow << '+' << XHeaderLength;
		}
	public:
		uys::untyped_storage_ Storage;
		struct s {
			uys::untyped_storage_::s Storage;
			tracker__ Free;	// Ne doit pas pointer sur le dernier fragment, même s'il s'agit du seul.
		} &S_;
		aggregated_storage_( s &S )
		: S_( S ),
		  Storage( S.Storage )
		{}
		void reset( bso::bool__  P = true )
		{
			Storage.reset( P );
			S_.Free.reset( P );
		}
		aggregated_storage_ &operator =( const aggregated_storage_ &AS )
		{
			sdr::size__ UsedSize = AS.Storage.Size() - AS._GetTailFreeFragmentSize();

			Storage.Allocate( UsedSize );

			if ( UsedSize != 0 ) {
				Storage.Store( AS.Storage, UsedSize, 0, 0 );
				_UpdateFirstFragmentPredecessorStatus( sUsed );
			}

			S_.Free = AS.S_.Free;

			return *this;
		}
		void plug( sdr::E_SDRIVER__ &MD )
		{
			Storage.plug( MD );
		}
		void plug( aggregated_storage_ &AS )
		{
			Storage.plug( AS );
		}
		void Init( void )
		{
			Storage.Init();
			S_.Free.Init();

			if ( Storage.Size() != 0 )
				_SetAsFreeFragment( 0, Storage.Size(), sFree );
		}
		void Preallocate( sdr::size__ Size )
		{
			if ( _Size() > Size )
				ERRPrm();
			else if ( _Size() != Size ) {
				sdr::row_t__ Row = _GetTailFreeFragment();
				sdr::size__ TailFreeFragmentSize = Size - _Size() + _GetTailFreeFragmentSize();

				Storage.Init();

				if ( Row == E_NIL )
					Row = _Size(); 

				Storage.Allocate( Size );

				_SetAsFreeFragment( Row, TailFreeFragmentSize, Row == 0 ? sFree : sUsed );

				_UpdateFirstFragmentPredecessorStatus( sFree );
			}
		}
		descriptor__ Allocate( size__ Size )
		{
			if ( Size == 0 )
				return E_NIL;
			else
				return _Allocate( Size );
		}
		void Free( descriptor__ Descriptor )
		{
			if ( Descriptor != E_NIL ) 
				return _Free( Descriptor );
		}
		descriptor__ Reallocate(
			descriptor__ Descriptor,
			size__ Size )
		{
			descriptor__ NewDescriptor = E_NIL;

			if ( Size == 0 )
				Free( Descriptor );
			else if ( Descriptor == E_NIL )
				NewDescriptor = Allocate( Size );
			else if ( false ) {	// Mettre à 'true' dans le cas ou '_Reallocate()' ci-dessous bug.
				size__ OldSize = _GetSize( Descriptor );
				NewDescriptor = _Allocate( Size );

				if ( OldSize > Size )
					OldSize = Size;

				Storage.Store( Storage, OldSize, *NewDescriptor, *Descriptor );

				_Free( Descriptor );
			} else
				NewDescriptor = _Reallocate( Descriptor, Size );

			return NewDescriptor;
		}
		void Read(
			descriptor__ Descriptor,
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer ) const
		{
			Storage.Recall( *Descriptor + Position, Amount, Buffer );
		}
		void Write(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			descriptor__ Descriptor,
			sdr::row_t__ Position )
		{
			Storage.Store( Buffer, Amount, *Descriptor + Position );
		}
		size__ Size( descriptor__ Descriptor ) const
		{
			return _GetSize( Descriptor );
		}
		void DisplayStructure( txf::text_oflow__ &Flow ) const;
	};

	E_AUTO( aggregated_storage )

	typedef uys::untyped_storage_file_manager___ aggregated_storage_file_manager___;

	inline uys::state__ Plug(
		aggregated_storage_ &AStorage,
		aggregated_storage_file_manager___ &FileManager )
	{
		uys::state__ State = uys::Plug( AStorage.Storage, FileManager );

		if ( !uys::IsError( State ) )
			AStorage.S_.Free.Init();

		return State;
	}
# define E_ASTORAGE_	aggregated_storage_
# define E_ASTORAGE	aggregated_storage
}

#endif

# undef AGS__HANDLE_PART_ONE
# undef AGS__HANDLE_PART_TWO


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
