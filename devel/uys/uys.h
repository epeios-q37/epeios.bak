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

//D UnTyped Storage 

#ifndef UYS__INC
#define UYS__INC

#define UYS_NAME		"UYS"

#define	UYS_VERSION	"$Revision: 1.9 $"

#define UYS_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( UYS_NODBG )
#define UYS_DBG
#endif

# include "err.h"
# include "tol.h"
# include "sdr.h"
# include "mns.h"
# include "ags.h"
# ifndef FLS__COMPILATION	// To avoid resursive inclusion.
#  include "flsq.h"
# endif

//d Value that a position can not have.
#define UYS_UNREACHABLE_POSITION	((sdr::row_t__)-1)

//d Maximal size of a storage.
#define UYS_MAX_SIZE			UYS_UNREACHABLE_POSITION

#ifdef UYS__DEFAULT_STORAGE_DRIVER
#	define UYS_DEFAULT_STORAGE_DRIVER UYS__DEFAULT_STORAGE_DRIVER
#else
#	define UYS_DEFAULT_STORAGE_DRIVER mns::main_storage_driver__
#endif

namespace uys {
	typedef sdr::sStorageDriver sStorageDriver_;

	class _storage_driver__
	{
	private:
		// Le pilote.
		sStorageDriver_ *_Driver;
		// Indique si le pilote a t dfini de manire interne ou non.
		bso::bool__ _Internal;
		// Uniquement pour la 'conventional_storage__'.
		sdr::byte__ *_CVMBuffer;
		void _Test( void ) const
		{
#ifdef UYS_DBG
			if ( _Driver == NULL )
				qRFwk();
#endif
		}
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( _Internal ) {
					delete _Driver;
					_Internal = false;
					_Driver = NULL;
				}

			} else {
				_Driver = NULL;
				_Internal = false;
			}

			_CVMBuffer = NULL;
		}
		_storage_driver__( void )
		{
			reset( false );
		}
		virtual ~_storage_driver__( void )
		{
			reset( true );
		}
		void plug( sStorageDriver_ &Driver )
		{
			reset();

			_Driver = &Driver;
		}
		// Initialization.
		void Init( void )
		{
			if ( _Driver == NULL ) {
				if ( ( _Driver = new UYS_DEFAULT_STORAGE_DRIVER( _CVMBuffer ) ) == NULL )
					qRAlc();
				else {
					_Internal = true;
					( (UYS_DEFAULT_STORAGE_DRIVER *)_Driver )->Init();
				}
			}
		}
		sStorageDriver_ *Driver( bso::bool__ Ignore = false ) const
		{
#ifdef UYS_DBG
			if ( !Ignore && !_Driver )
				qRFwk();
#endif
			return _Driver;
		}
		void Allocate( sdr::size__ Size )
		{
			_Test();

			_Driver->Allocate( Size );
		}
		void Recall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer ) const
		{
			_Test();

			if ( _CVMBuffer != NULL )
				memcpy( Buffer, _CVMBuffer + Position, Amount );
			else
				_Driver->Recall( Position, Amount, Buffer );
		}
		void Store(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			_Test();

			if ( _CVMBuffer != NULL )
				memcpy( _CVMBuffer + Position, Buffer, Amount );
			else
				_Driver->Store( Buffer, Amount, Position );
		}
		sdr::size__ Size( void ) const
		{
			if ( _Driver == NULL )
				return 0;
			else
				return _Driver->Size();
		}
		sdr::byte__ *CVMBuffer( void ) const
		{
			return _CVMBuffer;
		}
	};

	class untyped_storage_;

	void IndirectCopy_(
		const untyped_storage_ &Source,
		sdr::row_t__ PosSource,
		untyped_storage_ &Dest,
		sdr::row_t__ PosDest,
		sdr::size__ Nombre );

	struct sHook {
		sStorageDriver_ &D;
		sHook( sStorageDriver_ &Driver )
		: D( Driver )
		{}
	};

	//c Untyped storage.
	class untyped_storage_
	{
	private:
		// Le pilote mmoire.
		_storage_driver__ _Driver;
		// l'ventuel pilote de la multimemoire
		ags::aggregated_storage_driver__ _AggregatedStorageDriver;
		inline void _Test(
			sdr::row_t__ Position,
			sdr::size__ Amount ) const
		{
			if ( Position > S_.Size )
				qRFwk();

			if ( Amount > ( S_.Size - Position ) )
				qRFwk();
		}
		void _Recall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer ) const
		{
			_Test( Position, Amount );
			_Driver.Recall( Position, Amount, Buffer );
		}
		void _Store(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			_Test( Position, Amount );
			_Driver.Store( Buffer, Amount, Position );
		}
		void _Allocate( sdr::size__ Size )
		{
			_Driver.Allocate( Size );

			S_.Size = Size;
		}
		void plug_( sStorageDriver_ &Driver )
		{
			bso::size__ Size = Driver.Size();

			_Driver.plug( Driver );

			if ( S_.Size != Size )
				_Driver.Allocate( S_.Size = Size );
		}
	public:
		struct s
		{
			sdr::size__ Size;
			ags::descriptor__ Descriptor;
		} &S_;
		void reset( bool P = true )
		{
			_Driver.reset( P );
			_AggregatedStorageDriver.reset( P );

			S_.Descriptor = qNIL;
			S_.Size = 0;
		}
		untyped_storage_( s &S )
		: _AggregatedStorageDriver( S.Descriptor ),
		  S_( S )
		{
			reset( false );
		}
		~untyped_storage_( void )
		{
			reset();
		}
		void plug( sHook &Hook )
		{
			plug_( Hook.D );
		}
		void plug( ags::aggregated_storage_ *AS )
		{
			// 'AS' == 'NULL' is for flushing (mainly operated by the 'ctn' module. 
			if ( AS != NULL ) {
				_AggregatedStorageDriver.Init( *AS );
				plug_( _AggregatedStorageDriver );
			}
		}
		untyped_storage_ &operator =( const untyped_storage_ &US )
		{
			_Allocate( US.Size() );

			Store( US, US.Size(), 0, 0 );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			_Driver.Init();
			S_.Size = _Driver.Size();
		}
		void WriteToFlow(
			sdr::row_t__ Position,
			sdr::size__ Size,
			flw::oflow__ &OFlow ) const;
		void ReadFromFlow(
			flw::iflow__  &IFlow,
			sdr::row_t__ Position,
			sdr::size__ Size );
		//f Allocates 'Capacity' bytes.
		void Allocate( sdr::size__ Size )
		{
			_Allocate( Size );
		}
		/*f Recall 'Amount' bytes at 'Position' and put them in 'Buffer'.
		Ignore is only for 'UYS_DBG' mode and for the 'MMG' library.
		When 'true', it didn't make the test about the size. */
		void Recall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer ) const
		{
			_Recall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at 'Offset'.
		void Store(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			_Store( Buffer, Amount, Position );
		}
		//f Put byte at 'Position' in 'Datum'.
		void Recall(
			sdr::row_t__ Position,
			sdr::byte__ &Datum ) const
		{
			Recall( Position, 1, &Datum );
		}
		//f Return byte at 'Position'.
		sdr::byte__ Get( sdr::row_t__ Position ) const
		{
			sdr::byte__ D;

			Recall( Position, D );

			return D;
		}
		//f Write 'Byte' at 'Position'.
		void Store(
			sdr::byte__ Datum,
			sdr::row_t__ Position )
		{
			_Store( &Datum, 1, Position );
		}
		void Put(
			sdr::byte__ Datum,
			sdr::row_t__ Position )
		{
			Store( Datum, Position );
		}
		//f Store 'Amount' bytes at 'Position' in 'Begin' at 'Offset'.
		void Store(
			const untyped_storage_ &Source,
			sdr::size__ Amount,
			sdr::row_t__ Position,
			sdr::row_t__ Offset = 0 )
		{
			const sdr::byte__ *SourceBuffer = Source.CVMBuffer();
			sdr::byte__ *TargetBuffer = CVMBuffer();

			if ( SourceBuffer == NULL ) {
				if ( TargetBuffer == NULL )
					IndirectCopy_( Source, Offset, *this, Position, Amount );
				else if ( Amount != 0 )
					Source.Recall( Offset, Amount, TargetBuffer + Position );
			} else if ( TargetBuffer == NULL ) {
				if ( Amount != 0  )
					Store( SourceBuffer + Offset, Amount, Position );
			} else if ( SourceBuffer == TargetBuffer  )
				memmove( TargetBuffer + Position, SourceBuffer + Offset, Amount );
			else
				memcpy( TargetBuffer + Position, SourceBuffer + Offset, Amount );
		}
		// Remplit  partir de 'Position' avec 'Amount' 'Object' de taille 'Size'.
		void Fill(
			const sdr::byte__ *Object,
			sdr::size__ Size,
			sdr::row_t__ Position,
			sdr::size__ Amount );
		//f Search 'Object' of size 'Size' between 'Begin' and 'End' (excluded) and return its position or 'qNIL' if non-existant.
		sdr::row_t__ Search(
			const sdr::byte__ *Objet,
			sdr::size__ Size,
			sdr::row_t__ Begin,
			sdr::row_t__ End ) const;
		//f Return the used storage driver. 'Ignore' is only for 'UYS_DBG' mode and for the 'MMG' library.
		sStorageDriver_ *Driver( bso::bool__ Ignore = false )
		{
			return _Driver.Driver( Ignore );
		}
		sdr::size__ Size( void ) const
		{
			return S_.Size;
		}
		sdr::byte__ *CVMBuffer( void ) const
		{
			return _Driver.CVMBuffer();
		}
	};

	E_ENUM( mode )
	{
		mReadOnly,
		mReadWrite,
		m_amount,
		m_Undefined
	};

	inline fil::mode__ Convert_( mode__ Mode )
	{
		switch ( Mode ) {
		case mReadOnly:
			return fil::mReadOnly;
			break;
		case mReadWrite:
			return fil::mReadWrite;
			break;
		case m_Undefined:
			return fil::m_Undefined;
			break;
		default:
			qRFwk();
			break;
		}

		return fil::m_Undefined;	// Pour viter un 'warning'.
	}

	inline mode__ Convert_( fil::mode__ Mode )
	{
		switch ( Mode ) {
		case fil::mReadOnly:
			return mReadOnly;
			break;
		case fil::mReadWrite:
			return mReadWrite;
			break;
		case fil::m_Undefined:
			return m_Undefined;
			break;
		default:
			qRFwk();
			break;
		}

		return m_Undefined;	// Pour viter un 'warning'.
	}

	E_ENUM( behavior )
	{
		bVolatile,
			bPersistent,
			b_amount,
			b_Undefined
	};
}

namespace uys {

	template <typename driver> struct rH_
	: public sHook
	{
	protected:
		driver Driver_;
	public:
		rH_( void )
		: sHook( Driver_ )
		{}
	};

	class rRH	// Regular (memory - RAM) hook.
	: public rH_<mns::standalone_conventional_memory_driver___>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Driver_.reset( P );
		}
		qCDTOR( rRH );
		void Init( void )
		{
			Driver_.Init();
		}
	};

	// Hook filenames.
	struct rHF
	{
	public:
		fnm::name___ Filename;
		void reset( bso::bool__ P = true )
		{
			Filename.reset( P );
		}
		E_CDTOR( rHF );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename )	// Peut tre vide ('NULL') si 'Path' contient dj le nom de fichier.
		{
			Filename.Init();
			fnm::BuildPath( Path, Basename, ".q37", Filename );
		}
	};
		
	qENUM( State_ )
	{	// Statut de l'opration de connection.
		sExists,		// le fichier rattach existe.
		s_False,
		sAbsent = s_False,		// Fichier rattach absent (ce n'est pas une erreur, cela signifie que des donnes n'ont pas encore t stockes).
		s_Error,
		sInconsistent = s_Error,	// Les fichiers sont dans une tat incohrent, probablement d  un arrt inopin du logiciel. Utilis par les bibliothques en amont.
		s_amount,
		s_Undefined
	};

	qXENUM( State, s );

	// Files hook.
	class rFH
	: public rH_<flsq::file_storage_driver___>
	{
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Driver_.IsInitialized() ) {
					if ( !State().Boolean() )
						if ( IsPersistent() && ( Mode() == mReadWrite ) )
							Driver_.CreateFile();
				}
			}

			Driver_.reset( P );
		}
		qCVDTOR( rFH );
		eState Init( 
			const rHF &Filenames,
			mode__ Mode,
			behavior__ Behavior,
			flsq::id__ ID )
		{
			Driver_.Init( ID, Filenames.Filename, Convert_( Mode ), flsq::cFirstUse );

			switch ( Behavior ) {
			case bVolatile:
				break;
			case bPersistent:
				Driver_.Persistent();
				break;
			default:
				qRFwk();
				break;
			}

			if ( Driver_.FileExists() )
				return sExists;
			else
				return sAbsent;
		}
		eState State( void ) const
		{
			return ( Driver_.FileExists() ? sExists : sAbsent );
		}
		void CreateFiles( void )
		{
			Driver_.CreateFile();
		}
		void ReleaseFiles( void )
		{
			Driver_.ReleaseFile();
		}
		void DropFiles( void )
		{
			Driver_.Drop();
		}
		bso::sBool IsPersistent( void ) const
		{
			return Driver_.IsPersistent();
		}
		mode__ SetMode( mode__ Mode )
		{
			return Convert_( Driver_.Mode( Convert_( Mode ) ) );
		}
		mode__ Mode( void ) const
		{
			return Convert_( Driver_.Mode() );
		}
	};

	class untyped_storage
	: public untyped_storage_
	{
	private:
		untyped_storage_::s S_;
	public:
		untyped_storage( void )
		: untyped_storage_( S_ )
		{}
	};


	//d A position take this value if an object cannot be find.
#ifndef qNIL
	#define qNIL	UYS_UNREACHABLE_POSITION
#endif

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	bso::sign__ Compare(
		const untyped_storage_ &M1,
		const untyped_storage_ &M2,
		sdr::row_t__ BeginM1,
		sdr::row_t__ BeginM2,
		sdr::size__ Quantity );

	void _Fill(
		const sdr::byte__ *Object,
		sdr::size__ Size,
		sdr::size__ Count,
		sdr::row_t__ Position,
		sdr::byte__ *Data );
#if 0
	row__ _Position(
		const byte__ *Objet,
		size__ Size,
		row__ Begin,
		row__ End,
		const byte__ *Data );
#endif

	template <typename m> class _storage__
	: public m
	{
	public:
		struct s
		: public m::s
		{};
		void reset( bso::bool__ P = true )
		{
			m::reset( P );
		}
		_storage__( s &S = *(s *)NULL )
		: m( S )
		{
			reset( false );
		}
		~_storage__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{}
		//f Put in 'Buffer' 'Amount' bytes at 'Position'.
		void Recall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer ) const
		{
			memcpy( Buffer, m::Data_ + Position, Amount ); 
		}
		//f Write to 'Position' 'Amount' bytes from 'Buffer'.
		void Store(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			memcpy( m::Data_ + Position, Buffer, Amount ); 
		}
		//f Return byte at 'Position'.
		sdr::byte__ Get( sdr::row_t__ Position ) const
		{
			return *m::Data_[Position];
		}
		//f Write 'Byte' at 'Position'.
		void Store(
			sdr::byte__ Datum,
			sdr::row_t__ Position )
		{
			*m::Data_[Position] = Datum;
		}
		void Put(
			sdr::byte__ Datum,
			sdr::row_t__ Position )
		{
			Store( Datum, Position );
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Store(
			const _storage__ &Source,
			sdr::size__ Quantity,
			sdr::row_t__ Position = 0,
			sdr::row_t__ Offset = 0 )
		{
			memmove( m::Data_ + Offset, Source.m::Data_ + Position, Quantity ); 
		}
		/*f Store to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Store(
			const untyped_storage_ &Source,
			sdr::size__ Quantity,
			sdr::row_t__ Position = 0,
			sdr::row_t__ Offset = 0 )
		{
			Source.Recall( Position, Quantity, *m::Data_ + Offset );
		}
		//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
		void Store(
			const sdr::byte__ *Object,
			sdr::size__ Size,
			sdr::row_t__ Position,
			sdr::size__ Count )
		{
			_Store( Object, Size, Count, Position, m::Data_ );
		}
		//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) oR 'qNIL' if non-existant.
		sdr::row_t__ Search(
			const sdr::byte__ *Object,
			sdr::size__ Size,
			sdr::row_t__ Begin,
			sdr::row_t__ End ) const
		{
			return _Search( Object, Size, Begin, End, m::Data_ );
		}
		//f Return the used buffer.
		const sdr::byte__ *Buffer( void ) const
		{
			return m::Data_;
		}
	};

	//c A fixed-size storage of size 'size'.
	template <int size> class _untyped_storage__
	{
	protected:
		sdr::byte__ Data_[size];
	public:
		void reset( bso::bool__ = true )
		{}
		struct s {};	// To simplify use in library 'BCH'
		_untyped_storage__( s &S = *(s *)NULL )
		{
			reset( false );
		}
		~_untyped_storage__( void )
		{
			reset();
		}
		// Simplifies the 'SET' library.
		void Allocate( sdr::size__ Size )
		{
			if ( Size >= size )
				qRLmt();
		}
	};

	//c A untyped storage using conventional storage.
	class _untyped_storage___
	{
	protected:
		sdr::byte__ *_Data;
	public:
		struct s {};	// To simplify use in library 'BCH'
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Data != NULL )
					free( _Data );

			_Data = NULL;
		}
		_untyped_storage___( s *S = NULL )	// To simplify use in library 'BCH'
		{
			reset( false );
		}
		~_untyped_storage___( void )
		{
			reset( true );
		}
		//f Allocation of size 'Size'.
		void Allocate( sdr::size__ Size )
		{
			_Data = (sdr::byte__ *)realloc( _Data, Size );
		}
		//f Initialization.
		void Init( void )
		{
			// Standardisation.
		}
	};

	template <int size> class untyped_storage__
	: public _storage__< _untyped_storage__< size > >
	{
	public:
		struct s
		: _storage__< _untyped_storage__< size > >::s
		{};
		untyped_storage__( s &S = *(s *)NULL )	// To simplify use in library 'BCH'
		: _storage__< _untyped_storage__< size > >( S )
		{}
	};

	typedef _storage__< _untyped_storage___>	untyped_storage___;
}

# define UYS__HEADER_HANDLED	// A destination de 'AGS'.

#endif
