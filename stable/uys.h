/*
	Header for the 'uys' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2013-2004 Claude SIMON.

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

//	$Id: uys.h,v 1.9 2013/04/15 10:50:58 csimon Exp $

#ifndef UYS__INC
#define UYS__INC

#define UYS_NAME		"UYS"

#define	UYS_VERSION	"$Revision: 1.9 $"

#define UYS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &UYSTutor;

#if defined( E_DEBUG ) && !defined( UYS_NODBG )
#define UYS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:58 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D UnTyped Storage 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "tol.h"
# include "sdr.h"
# include "mns.h"
# include "ags.h"
# ifndef FLS__COMPILATION	// To avoid resursive inclusion.
#  include "fls.h"
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
	class _storage_driver__
	{
	private:
		// Le pilote.
		sdr::E_SDRIVER__ *_Driver;
		// Indique si le pilote a été défini de manière interne ou non.
		bso::bool__ _Internal;
		// Uniquement pour la 'conventional_storage__'.
		sdr::datum__ *_CVMBuffer;
		void _Test( void ) const
		{
#ifdef UYS_DBG
			if ( _Driver == NULL )
				ERRFwk();
#endif
		}
	public:
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( _Internal )
				{
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
		void plug( sdr::E_SDRIVER__ &Driver )
		{
			reset();

			_Driver = &Driver;
		}
		// Initialization.
		void Init( void )
		{
			if ( _Driver == NULL )
			{
				if ( ( _Driver = new UYS_DEFAULT_STORAGE_DRIVER( _CVMBuffer ) ) == NULL )
					ERRAlc();
				else
				{
					_Internal = true;
					((UYS_DEFAULT_STORAGE_DRIVER *)_Driver)->Init();
				}
			}
		}
		sdr::E_SDRIVER__ *Driver( bso::bool__ Ignore = false ) const
		{
	#ifdef UYS_DBG
			if ( !Ignore && !_Driver )
				ERRPrm();
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
			sdr::datum__ *Buffer ) const
		{
			_Test();

			if ( _CVMBuffer != NULL )
				memcpy( Buffer, _CVMBuffer + Position, Amount );
			else
				_Driver->Recall( Position, Amount, Buffer );
		}
		void Store(
			const sdr::datum__ *Buffer,
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
	};

	//c Untyped storage.
	class untyped_storage_
	{
	private:
		// Le pilote mémoire.
		_storage_driver__ _Driver;
		// l'éventuel pilote de la multimemoire
		ags::aggregated_storage_driver__ _AggregatedStorageDriver;
		inline void _Test(
			sdr::row_t__ Position,
			sdr::size__ Amount ) const
		{
			if ( Position > S_. Size )
				ERRPrm();

			if ( Amount > ( S_. Size - Position ) )
				ERRPrm();
		}
		void _Recall(
			sdr::row_t__ Position,
			sdr::size__ Amount, 
			sdr::datum__ *Buffer ) const
		{
			_Test( Position, Amount );
			_Driver.Recall( Position, Amount, Buffer );
		}
		void _Store(
			const sdr::datum__ *Buffer,
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
	public:
		struct s {
			sdr::size__ Size;
			ags::descriptor__ Descriptor;
		} &S_;
		void reset( bool P = true )
		{
			_Driver.reset( P );
			_AggregatedStorageDriver.reset( P );

			S_.Descriptor = E_NIL;
			S_.Size = 0;
		}
		untyped_storage_( s &S )
		: S_( S ),
		  _AggregatedStorageDriver( S.Descriptor )
		{
			reset( false );
		}
		~untyped_storage_( void )
		{
			reset();
		}
		void plug( sdr::E_SDRIVER__ &Driver  )
		{
			reset();

			_Driver.plug( Driver );
		}
		void plug( ags::aggregated_storage_ &AS )
		{
			_AggregatedStorageDriver.Init( AS );
			_Driver.plug( _AggregatedStorageDriver );
			S_.Size = _Driver.Size();
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
			sdr::datum__ *Buffer ) const
		{
			_Recall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at 'Offset'.
		void Store(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			_Store( Buffer, Amount, Position );
		}
		//f Put byte at 'Position' in 'Datum'.
		void Recall(
			sdr::row_t__ Position,
			sdr::datum__ &Datum ) const
		{
			Recall( Position, 1, &Datum );
		}
		//f Return byte at 'Position'.
		sdr::datum__ Get( sdr::row_t__ Position ) const
		{
			sdr::datum__ D;

			Recall( Position, D );

			return D;
		}
		//f Write 'Byte' at 'Position'.
		void Store(
			sdr::datum__ Datum,
			sdr::row_t__ Position )
		{
			_Store( &Datum, 1, Position );
		}
		void Put(
			sdr::datum__ Datum,
			sdr::row_t__ Position )
		{
			Store( Datum, Position );
		}
		//f Store 'Amount' bytes at 'Position' in 'Begin' at 'Offset'.
		void Store(
			const untyped_storage_ &Source,
			sdr::size__ Amount,
			sdr::row_t__ Position,
			sdr::row_t__ Offset = 0 );
		// Remplit à partir de 'Position' avec 'Amount' 'Object' de taille 'Size'.
		void Fill(
			const sdr::datum__ *Object,
			sdr::size__ Size,
			sdr::row_t__ Position,
			sdr::size__ Amount );
		//f Search 'Object' of size 'Size' between 'Begin' and 'End' (excluded) and return its position or 'E_NIL' if non-existant.
		sdr::row_t__ Search(
			const sdr::datum__ *Objet,
			sdr::size__ Size,
			sdr::row_t__ Begin,
			sdr::row_t__ End ) const;
		//f Return the used storage driver. 'Ignore' is only for 'UYS_DBG' mode and for the 'MMG' library.
		sdr::E_SDRIVER__ *Driver( bso::bool__ Ignore = false )
		{
			return _Driver.Driver( Ignore );
		}
		sdr::size__ Size( void ) const
		{
			return S_.Size;
		}
	};

	E_ENUM( _state ) {	// Statut de l'opération de connection.
		sExists,		// le fichier rattaché existe.
		sAbsent,		// Fichier rattaché absent (ce n'est pas une erreur, cela signifie que des données n'ont pas encore été stockées).
		sInconsistent,	// Les fichiers sont dans une état incohérent, probablement dû à un arrêt inopiné du logiciel. Utilisé par les bibliothèques en amont.
		s_amount,
		s_Undefined
	};

	E_XENUM( state, s );

#define UYS_STATE_AMOUNT	3

	inline bso::bool__ IsError( state__ State )
	{
#if UYS_STATE_AMOUNT != 3
#	error "'state__' changed !"
#endif
		switch ( State() ) {
		case sExists:
		case sAbsent:
			return false;
			break;
		case sInconsistent:
			return true;
			break;
		default:
			ERRPrm();
			break;
		}

		return true;	// To avoid a 'warning'.
	}

	inline bso::bool__ Exists( state__ State )
	{
		if ( IsError( State ) )
			ERRPrm();

#if UYS_STATE_AMOUNT != 3
#	error "'state__' changed !"
#endif
		switch ( State() ) {
		case sExists:
			return true;
			break;
		case sAbsent:
			return false;
			break;
		case sInconsistent:
			ERRFwk();
			break;
		default:
			ERRFwk();
			break;
		}

		return false;	// Pour éviter un 'warning'.
	}

	typedef fls::E_FILE_SDRIVER___ _file_storage_driver___;

	struct hook_filenames___
	{
	public:
		fnm::name___ Filename;
		void reset( bso::bool__ P = true )
		{
			Filename.reset( P );
		}
		E_CDTOR( hook_filenames___ );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename )	// Peut être vide ('NULL') si 'Path' contient déjà le nom de fichier.
		{
			Filename.Init();
			fnm::BuildPath( Path, Filename, ".qs", Filename );
		}
	};
		
	class files_hook___
	: public _file_storage_driver___
	{
	public:
		void Init( 
			const hook_filenames___ &Filenames,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			fls::id__ ID )
		{
			_file_storage_driver___::Init( ID, Filenames.Filename, Mode, fls::cFirstUse );

			if ( Persistent )
				_file_storage_driver___::Persistent();
		}
		bso::bool__ CreateFiles( err::handling__ ErrHandling = err::h_Default )
		{
			return CreateFile( ErrHandling );
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			return _file_storage_driver___::Mode( Mode );
		}
		fil::mode__ Mode( void ) const
		{
			return _file_storage_driver___::Mode();
		}
		state__ State( void ) const
		{
			if ( GetFileName().Amount() == 0 )
				return sInconsistent;
			else if ( Exists() )
				return sExists;
			else
				return sAbsent;
		}
		state__ Bind( void )
		{
			return State();
		}
		state__ Settle( void )
		{
			if ( Mode() == fil::mReadWrite )
				_file_storage_driver___::Flush();

			return State();
		}
		friend state__ Plug(
			untyped_storage_ &Storage,
			files_hook___ &FilesHook );
	};

	inline state__ Plug(
		untyped_storage_ &Storage,
		files_hook___ &Hook )
	{
		state__ State = ( Hook.Exists() ? sExists : sAbsent );

		Storage.plug( Hook );

		Storage.Allocate( Hook.Size() );

		return State;
	}

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
#ifndef E_NIL
	#define E_NIL	UYS_UNREACHABLE_POSITION
#endif

	void _Copy(
		const class untyped_storage_ &Source,
		sdr::row_t__ PosSource,
		class untyped_storage_ &Dest,
		sdr::row_t__ PosDest,
		sdr::size__ Quantity,
		sdr::datum__ *Buffer,
		sdr::size__ BufferSize );

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	bso::sign__ Compare(
		const untyped_storage_ &M1,
		const untyped_storage_ &M2,
		sdr::row_t__ BeginM1,
		sdr::row_t__ BeginM2,
		sdr::size__ Quantity );

	void _Fill(
		const sdr::datum__ *Object,
		sdr::size__ Size,
		sdr::size__ Count,
		sdr::row_t__ Position,
		sdr::datum__ *Data );
#if 0
	row__ _Position(
		const datum__ *Objet,
		size__ Size,
		row__ Begin,
		row__ End,
		const datum__ *Data );
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
			sdr::datum__ *Buffer ) const
		{
			memcpy( Buffer, m::Data_ + Position, Amount ); 
		}
		//f Write to 'Position' 'Amount' bytes from 'Buffer'.
		void Store(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			memcpy( m::Data_ + Position, Buffer, Amount ); 
		}
		//f Return byte at 'Position'.
		sdr::datum__ Get( sdr::row_t__ Position ) const
		{
			return *m::Data_[Position];
		}
		//f Write 'Byte' at 'Position'.
		void Store(
			sdr::datum__ Datum,
			sdr::row_t__ Position )
		{
			*m::Data_[Position] = Datum;
		}
		void Put(
			sdr::datum__ Datum,
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
			const sdr::datum__ *Object,
			sdr::size__ Size,
			sdr::row_t__ Position,
			sdr::size__ Count )
		{
			_Store( Object, Size, Count, Position, m::Data_ );
		}
		//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) oR 'E_NIL' if non-existant.
		sdr::row_t__ Search(
			const sdr::datum__ *Object,
			sdr::size__ Size,
			sdr::row_t__ Begin,
			sdr::row_t__ End ) const
		{
			return _Search( Object, Size, Begin, End, m::Data_ );
		}
		//f Return the used buffer.
		const sdr::datum__ *Buffer( void ) const
		{
			return m::Data_;
		}
	};

	//c A fixed-size storage of size 'size'.
	template <int size> class _untyped_storage__
	{
	protected:
		sdr::datum__ Data_[size];
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
				ERRLmt();
		}
	};

	//c A untyped storage using conventional storage.
	class _untyped_storage___
	{
	protected:
		sdr::datum__ *_Data;
	public:
		struct s {};	// To simplify use in library 'BCH'
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Data != NULL )
					free( _Data );

			_Data = NULL;
		}
		_untyped_storage___( s &S = *(s *)NULL )	// To simplify use in library 'BCH'
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
			_Data = (sdr::datum__ *)realloc( _Data, Size );
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

inline bso::bool__ BoolOp( uys::_state__ State )
{
	if ( uys::IsError( State ) )
		ERRFwk();

	return uys::Exists( State );
}

# define UYS__HEADER_HANDLED	// A destination de 'AGS'.

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
