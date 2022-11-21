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

// FiLe Storage

#ifndef FLSQ_INC_
# define FLSQ_INC_

# define FLSQ_NAME		"FLSQ"

#if defined( E_DEBUG ) && !defined( FLSQ_NODBG )
# define FLSQ_DBG
#endif

# include "cpe.h"
# include "tol.h"
# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "iop.h"
# include "fil.h"
# include "osd.h"

# if defined( CPE_S_WIN ) || defined ( CPE_S_CYGWIN )
#  define FLSQ_DEFAULT_MAX_FILE_AMOUNT	1000
# elif defined ( CPE_S_UNIX )
#  define FLSQ_DEFAULT_MAX_FILE_AMOUNT	800	// At least under GNU/Linux, only 1024 descriptors (socket included) are available. Don't know for other unixes.
# elif defined ( CPE_S_DARWIN )
#  define FLSQ_DEFAULT_MAX_FILE_AMOUNT	200	// Under Darwin, by default, only 200 decriptors available.
# else
#  error "Unimplemented target !"
# endif


# ifdef FLSQ_MAX_FILE_AMOUNT
#  define FLSQ__MAX_FILE_AMOUNT	FLSQ_MAX_FILE_AMOUNT
# else
#  define FLSQ__MAX_FILE_AMOUNT	FLSQ_DEFAULT_MAX_FILE_AMOUNT
# endif

# ifndef FLSQ_NO_MT
#  ifdef CPE_F_MT
#   define FLSQ__MT
#  endif
# endif


# ifndef FLSQ_NO_AUTOFLUSH
#  ifdef FLSQ__MT
#   define FLSQ__AUTOFLUSH
#  endif
# endif

# ifdef CPE_F_MT
#  include "mtx.h"
# endif

/**************/
/**** NEW *****/
/**************/

namespace flsq {
	qROWr( Id );
	qCDEF( rId, Undefined, qNIL );

  template <typename storage, int offset> qTCLONE(osd::sDriver<qCOVER2(storage, offset)>, sOffsetDriver_);
}

/**************/
/**** OLD *****/
/**************/


namespace flsq {
	extern fdr::size__ MaxFileAmount;

	typedef bso::size__ position__;

	typedef rId id__;

	id__ GetId( void );

	void ReleaseId( id__ ID );

	enum creation
	{
		cNow,
		cFirstUse
	};

	typedef iop::io__		_io__;

	class _file___
	{
	private:
		iop::descriptor__ _D;
		_io__ _Core;
#ifdef FLSQ__MT
		mtx::handler___ _Mutex;
#endif
		void _Lock( void )
		{
#ifdef FLSQ__MT
			mtx::Lock( _Mutex );
#endif
		}
		void _Unlock( void )
		{
#ifdef FLSQ__MT
			mtx::Unlock( _Mutex );
#endif
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
#ifdef FLSQ__MT
				if ( _Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( _Mutex );
#endif
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
#ifdef FLSQ__MT
			_Mutex = MTX_INVALID_HANDLER;
#endif
		}
		_file___( void )
		{
			reset( false );
		}
		~_file___( void )
		{
			reset();
		}
		tol::report__ Init(
			const fnm::name___ &Name,
			fil::mode__ Mode,
			err::handling__ ErrorHandling = err::h_Default )
		{
			reset();

#ifdef FLSQ__MT
			_Mutex = mtx::Create();
#endif

			_D = fil::Open( Name, Mode );

			if ( _D == IOP_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrorHandling ) {
				case err::hUserDefined:
					return tol::rFailure;
					break;
				case err::hThrowException:
					qRLbr();
					break;
				default:
					qRFwk();
					break;
				}
			}

			_Core.Init( _D );

			return tol::rSuccess;
		}
		void Lock( void )
		{
			_Lock();
		}
		void Unlock( void )
		{
			_Unlock();
		}
		void Seek( fil::size__ Offset )
		{
			_Lock();

			_Core.Seek( Offset );

			_Unlock();
		}
		bso::size__ Read(
			bso::size__ Amount,
			void *Buffer )
		{
			_Lock();

			Amount = _Core.Read( Amount, Buffer );

			_Unlock();

			return Amount;
		}
		fil::size__ Write(
			const void *Buffer,
			fil::size__ Amount )
		{
			_Lock();

			Amount = _Core.Write( Buffer, Amount );

			_Unlock();

			return Amount;
		}
		fil::size__ Size( void )
		{
			_Lock();

			fil::size__ Size = fil::GetSize( _D );

			_Unlock();

			return Size;
		}
		void ThreadUnsafeFlush( void )
		{
			_Core.Flush();
		}
		void Flush( void )
		{
			_Lock();

			ThreadUnsafeFlush();

			_Unlock();
		}
	};

	E_ROW( row__ );

	row__ _Register(
		class file_storage___ &FS,
		id__ ID );
	void _Unregister(
		row__ Row,
		id__ ID );
	void _ReportFileUsing(
		row__ Row,
		bso::bool__ ToFlush );
	void _ReportFileClosing( row__ Row );

	void ReleaseFiles( id__ ID );

	class file_storage___
	{
	private:
		_file___ File_;
		// nom du fichier
		fnm::name___ _Name;
		// taille du fichier
//		sdr::size__ TailleFichier_;	// 'sdr::size__' et non 'fil::size__', car considr comme un 'storage', et donc soumis  ses limites.
		struct {
			int
				// signale si le Stream est ouvert ou non
				Ouvert			:1,
				// signale que la fermeture du fichier se fait manuellement
				Manuel  		:1,
				// Signale que le fichier ne doit pas tre dtruit  la destruction de l'objet
				Persistant		:1,
				// Signale que le nom du fichier a t cre de manire interne
				Interne			:1;
				// Mode d'accs  la mmoire.
				fil::mode__ Mode;
		} Temoin_;
		row__ _Row;	// Pour le suivi des 'file handler' ouverts.
		// diffrents tmoins
		time_t AccessEpochTimestamp_;	// Last access time.
		id__ _ID;
	// Fonctions
		bso::bool__ Open_(
			bso::bool__ ToFlush,
			err::handling__ ErrorHandling = err::h_Default )	// Si  'true', le fichier doit tre 'flush' (accs en criture).
		{
			bso::bool__ Success = true;

			if ( !Temoin_.Ouvert )
			{
				Success = File_.Init( _Name, Temoin_.Mode, ErrorHandling ) == tol::rSuccess;

				if ( Success )
					Temoin_.Ouvert = 1;
			}

			if ( Success ) {
				_ReportFileUsing( _Row, ToFlush );

				AccessEpochTimestamp_ = tol::EpochTime( false );
			}

			return Success;
		}
		/*
		void _AdjustPhysicalFileSize( void )	// Ajuste la taille physique du fichier  celle suppose.
		{
			if ( ( Temoin_.Mode != fil::mReadOnly ) && (  TailleFichier_ != 0 ) ) {

				Flush();	// Pour mettre  jour la taille physique du fichier pour que la mthode 'GetFileSize(...)' retourne la bonne valeur.

				if ( !fil::Exists( _Name ) || ( TailleFichier_ > fil::GetSize( _Name ) ) ) {
					sdr::byte__ Datum = 0;

					Open_( true );

					File_.Seek( TailleFichier_ - 1 );

					if ( File_.Write( &Datum, 1 ) != 1 ) {
						if ( !Temoin_.Manuel )
							ReleaseFile();

						qRLbr();
					} else
						if ( !Temoin_.Manuel )
							ReleaseFile();
				}
			}

		}
		*/
		void GrowToSize_( fil::size__ Size )
		{
			sdr::byte__ Datum = 0;

			Open_( true );

			File_.Seek( Size - 1 );

			if ( File_.Write( &Datum, 1 ) != 1 ) {
				if ( !Temoin_.Manuel )
					ReleaseFile();

				qRLbr();
			}
		}
		bso::sSize Read_(
			position__ Position,
			bso::size__ Nombre,
			void *Tampon,
			qRPD)
		{
			Open_( false );

			bso::sSize
        TotalAmount = 0,
        LoopAmount = 0;

			File_.Seek( Position );

			while( Nombre > 0 ) {
				LoopAmount = File_.Read( Nombre, Tampon );

				if ( LoopAmount <= 0 ) {
					if ( LoopAmount == 0 ) {
            if ( qRPT )
              qRFwk();
						Nombre = 0;
					} else
						qRFwk();
				} else {
				  TotalAmount += LoopAmount;
          Nombre -= LoopAmount;
          Tampon = (char *)Tampon + LoopAmount;
				}
			}

			if ( !Temoin_.Manuel )
				ReleaseFile();

      return TotalAmount;
		}
			/* lit  partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Write_(
			const void *Tampon,
			bso::size__ Nombre,
			position__ Position )
		{
			Open_( true );

			bso::size__ Amount;

			File_.Seek( Position );

			while( Nombre > 0 ) {

				Amount = (bso::size__)File_.Write( Tampon, Nombre );

				Tampon = (char *)Tampon + Amount;
				Nombre -= Amount;
			}

			if ( !Temoin_.Manuel )
				ReleaseFile();
/*			else
				File_.Flush();
*/		}
		void Allocate_( bso::size__ Capacite )
		{
			if ( fil::Exists( _Name ) ) {
				if ( Capacite < fil::GetSize( _Name ) ) {
					ReleaseFile();
					fil::Shrink( _Name, Capacite );	// Time consuming. If called too often, allocation to less should be avoid upstream.
				} // else nothing, as the file will grow with the writing of the data.
			}
		}
	public:
		E_CVDTOR( file_storage___ )
		void reset( bool P  = true )
		{
			if ( P )
			{
				ReleaseFile();

				if ( _Row != qNIL )
					_Unregister( _Row, _ID );

				if ( _Name.Amount() != 0  )
				{
					if ( !Temoin_.Persistant )
						fil::Remove( _Name );
				}
			}

			_Name.reset( P );
			Temoin_.Ouvert = false;
			Temoin_.Manuel = true;
			Temoin_.Persistant = false;
			Temoin_.Interne = false;
			Temoin_.Mode = fil::m_Undefined;
//			TailleFichier_ = 0;
			_Row = qNIL;
			_ID = Undefined;
			AccessEpochTimestamp_ = 0;
		}
		void Init(
			id__ ID,
			const fnm::name___ &Name,
			fil::mode__ Mode = fil::mReadWrite,
			flsq::creation Creation = flsq::cFirstUse )
		{
			if ( Name.Amount() != 0 ) {
				reset();

				_Name.Init( Name );

				Temoin_.Interne = false;

				_ID = ID;

				_Row = _Register( *this, _ID );

			} else if ( !Temoin_.Interne ) {
				reset();

				char Buffer[L_tmpnam];

# ifdef CPE_S_DARWIN
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# endif
				if ( tmpnam( Buffer ) == NULL )
					qRSys();
# ifdef CPE_S_DARWIN
#  pragma GCC diagnostic pop
# endif

				_Name.Init( Buffer );

				Temoin_.Interne = true;

				_ID = ID;

				_Row = _Register( *this, _ID );
			}

			Temoin_.Mode = Mode;

			if ( Creation == flsq::cNow  )
				if ( Mode == fil::mReadWrite )
					Open_( false );
				else
					qRFwk();
			else if ( Creation != flsq::cFirstUse )
				qRFwk();
		}
# ifdef CPE_C_MSC
#  undef CreateFile
# endif
    sdr::eType Type(void) const
    {
      return Temoin_.Persistant ? sdr::tPersistent : sdr::tVolatile;
    }
		bso::bool__ CreateFile( err::handling__ ErrHandle = err::h_Default )
		{
			bso::sBool Success = Open_( false, ErrHandle );

			if ( Success )
				if ( !Temoin_.Manuel )
					ReleaseFile();

			return Success;
		}
		void Touch( time_t ReferenceTime )
		{
			ReleaseFile();

			if ( FileExists() ) {
				if ( !fil::Touch( _Name ) )
					qRFwk();
			} else {
				CreateFile();
				ReleaseFile();
			}

			if ( ReferenceTime != 0 ) {
				while ( ModificationTimestamp() <= ReferenceTime ) {
					tol::EpochTime( true );

					if ( !fil::Touch( _Name ) )
						qRFwk();
				}
			}
		}
		void ReleaseFile( bso::bool__ ReportClosing = true )
		{
//			_AdjustPhysicalFileSize();

			if ( Temoin_.Ouvert ) {
				File_.reset();

				if ( ReportClosing )
					_ReportFileClosing( _Row );
			}

			Temoin_.Ouvert = 0;
		}
		void AdjustSize( fil::size__ Size )
		{
			ReleaseFile();

			if ( !FileExists() && ( Size != 0 ) )
				qRFwk();

			switch ( bso::Compare( Size, FileSize() ) ) {
			case -1:
				fil::Shrink( _Name, Size );
				break;
			case 0:
				break;
			case 1:
				GrowToSize_( Size );
				break;
			default:
				qRFwk();
				break;
			}
		}
		void Manual( void )
		{
			Temoin_.Manuel = 1;
		}
		void Automatic( void )
		{
			Temoin_.Manuel = 0;
		}
		void Persistent( void )
		{
			Temoin_.Persistant = true;
		}
		bso::bool__ IsPersistent( void ) const
		{
			return Temoin_.Persistant != 0;
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			if ( Temoin_.Mode != Mode )	{
				tol::Swap( Temoin_.Mode, Mode );
				ReleaseFile();
			}

			return Mode;
		}
		fil::mode__ Mode( void ) const
		{
			return Temoin_.Mode;
		}
		bso::sBool IsInitialized( void ) const
		{
			return _Name != NULL;
		}
		void Drop( void ) // Efface le fichier sous-jacent, s'il existe.
		{
			ReleaseFile();

			if ( ( _Name.Amount() != 0 ) && fil::Exists( _Name ) )
				if ( !fil::Remove( _Name ) )
					qRLbr();
		}
		const fnm::name___ &GetFilename( void ) const
		{
			return _Name;
		}
		bso::bool__ FileExists( void ) const
		{
			if ( _Name.IsEmpty() )
				return false;
			else
				return fil::Exists( _Name );
		}
		time_t ModificationTimestamp( void ) const
		{
			if ( FileExists() )
				return fil::GetLastModificationTime( _Name );
			else
				return 0;
		}
		fil::size__ FileSize( void ) const
		{
# if 0
			Open_( false );

			return File_.Size();
# else
			if ( FileExists() )
				return fil::GetSize( _Name );
			else
				return 0;
# endif
		}
		void Flush( void )
		{
			if ( Temoin_.Ouvert )
				File_.Flush();
		}
		qRODISCLOSEs( time_t, AccessEpochTimestamp );
		_file___ &File( void )
		{
			return File_;
		}
		friend class sOSDRelay_;
	};

	void ReleaseInactiveFiles_(
		time_t Delay,	// in s.
		bso::uint__ MaxAmount = BSO_UINT_MAX ); // Releases up to 'MaxAmount' files not accessed since 'Delay' ms. Thread-safe.
}

/*******/
/* NEW */
/*******/

namespace flsq {
  typedef file_storage___ rStorage_;

	class sOSDRelay_
	{
  private:
    rStorage_ &S_;
  public:
    void reset(bso::sBool = true)
    {
      // Standardization.
    }
    sOSDRelay_( rStorage_ &Storage)
    : S_(Storage)
    {
      reset(false);
    }
    qDTOR(sOSDRelay_);
    void Init(void)
    {
      reset();
    }
		void OSDAllocate( sdr::sSize Size )
		{
			return S_.Allocate_(Size);
    }
    sdr::sSize OSDSize( void ) const
		{
#ifdef CPE_F_32BITS
      fil::sSize Size = S_.FileSize();

      if ( Size > SDR_SIZE_MAX )
        qRFwk();

      return (sdr::sSize)Size;
#else
      return S_.FileSize();
#endif
		}
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		sdr::sSize OSDFetch(
			sdr::tRow Position,
			sdr::sSize Amount,
			sdr::sByte *Buffer,
			qRPN)
    {
      return S_.Read_(Position, Amount, Buffer, qRP);
    }
		//v Write 'Amount' bytes from 'Buffer' to storage at position 'Position'.
		void OSDStore(
			const sdr::sByte *Buffer,
			sdr::sSize Amount,
			sdr::tRow Position )
		{
		  return S_.Write_(Buffer, Amount, Position);
		}
	};

	template <int offset> class rFileOffsetDriver
  : public rStorage_,
    public sOffsetDriver_<sOSDRelay_, offset>
  {
  private:
    sOSDRelay_ OSD_;
  public:
    void reset(bso::sBool P = true)
    {
      rStorage_::reset(P);
      OSD_.reset(P);
      sOffsetDriver_<sOSDRelay_, offset>::reset(P);
    }
    rFileOffsetDriver(void)
    : OSD_(*this)
    {
      reset(false);
    }
    qVDTOR(rFileOffsetDriver);
		void Init(
			id__ Id,
			const fnm::name___ &FileName = fnm::name___(),
			fil::mode__ Mode = fil::mReadWrite,
			flsq::creation Creation = flsq::cFirstUse )
		{
		  reset();

			rStorage_::Init( Id, FileName, Mode, Creation );
			OSD_.Init();
			sOffsetDriver_<sOSDRelay_, offset>::Init(OSD_);
		}
  };

  typedef rFileOffsetDriver<0> rFileDriver;
}

/*******/
/* Old */
/*******/

namespace flsq {
  typedef rFileDriver file_storage_driver___;
}

# define E_FILqSD___	file_storage_driver___}

#endif
