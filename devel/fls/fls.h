/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef FLS__INC
#define FLS__INC

#define FLS_NAME		"FLS"

#if defined( E_DEBUG ) && !defined( FLS_NODBG )
#define FLS_DBG
#endif

// FiLe Storage

# include "cpe.h"
# include "tol.h"
# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "iop.h"
# include "fil.h"

# if defined( CPE_MSVC ) || defined ( CPE_MINGW ) || defined ( CPE_CYGWIN )
#  define FLS_DEFAULT_MAX_FILE_AMOUNT	1000
# elif defined ( CPE_LINUX )
#  define FLS_DEFAULT_MAX_FILE_AMOUNT	800	// Linux, par dfaut, ne peut ouvrir que 1024 descripteurs (socket comprises).
# elif defined ( CPE_XCODE )
#  define FLS_DEFAULT_MAX_FILE_AMOUNT	200	// Mac, par dfaut, ne peut ouvrir que 256 descripteurs (socket comprises).
# else
#  error "Unimplemented target !"
# endif


#ifdef FLS_MAX_FILE_AMOUNT
#	define FLS__MAX_FILE_AMOUNT	FLS_MAX_FILE_AMOUNT
#else
#	define FLS__MAX_FILE_AMOUNT	FLS_DEFAULT_MAX_FILE_AMOUNT
#endif


#ifndef FLS_NO_MT
#	ifdef CPE__MT
#		define FLS__MT
#	endif
#endif


#ifndef FLS_NO_AUTOFLUSH
#	ifdef FLS__MT
#		define FLS__AUTOFLUSH
#	endif
#endif

# ifdef CPE__MT
#  include "mtx.h"
# endif


namespace fls {
	extern fdr::size__ MaxFileAmount;

	typedef bso::size__ position__;
	// type dfinissant une position dans la mmoire

	// Identifiant sous lequel est regroup un ensemble de fichiers.
	E_ROW( id__ );
	#define FLS_UNDEFINED_ID	qNIL

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
#ifdef FLS__MT
		mtx::mutex_handler__ _Mutex;
#endif
		void _Lock( void )
		{
#ifdef FLS__MT
			mtx::Lock( _Mutex );
#endif
		}
		void _Unlock( void )
		{
#ifdef FLS__MT
			mtx::Unlock( _Mutex );
#endif
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
#ifdef FLS__MT
				if ( _Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( _Mutex );
#endif
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
#ifdef FLS__MT
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

#ifdef FLS__MT
			_Mutex = mtx::Create( mtx::mProtecting );
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
		sdr::size__ TailleFichier_;	// 'sdr::size__' et non 'fil::size__', car considr comme un 'storage', et donc soumis  ses limites.
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
		time_t _EpochTimeStamp;	// Last access time.
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

				_EpochTimeStamp = tol::EpochTime( false );
			}

			return Success;
		}
		void _AdjustPhysicalFileSize( void )	// Ajuste la taille physique du fichier  celle suppose.
		{
			if ( ( Temoin_.Mode != fil::mReadOnly ) && (  TailleFichier_ != 0 ) ) {

				Flush();	// Pour mettre  jour la taille physique du fichier pour que la mthode 'GetFileSize(...)' retourne la bonne valeur.

				if ( !fil::Exists( _Name ) || ( TailleFichier_ > fil::GetSize( _Name ) ) ) {
					sdr::datum__ Datum = 0;
					
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
	protected:
		void Read(
			position__ Position,
			bso::size__ Nombre,
			void *Tampon )
		{
			Open_( false );

			bso::size__ Amount;

			File_.Seek( Position );
				
			while( Nombre > 0 ) {
				
				Amount = (bso::size__)File_.Read( Nombre, Tampon );
					
				if ( Amount <= 0 ) {
					if ( Amount == 0 ) {
						if ( ( Position + Nombre ) <= TailleFichier_ )	{ /* Lors d'une allocation, la nouvelle taille est note, mais la taille du fichier n'est pas modifie
																		   (gain de temps). Or, certaines bibliothques ('MMM', par exemple) lisent un emplacement allou
																		   avant d'avoir crit dedans, on considre donc que la quantit, si correcte par rapport  la taille alloue,
																		   de donnes demande est disponible, peu importe le contenu.
																		*/
							Amount = Nombre;
						} else
							qRFwk();
					} else
						qRFwk();
				}
					
				Nombre -= Amount;
				Tampon = (char *)Tampon + Amount;
			}

			if ( !Temoin_.Manuel )
				ReleaseFile();
		}
			/* lit  partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Write(
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
		void Allocate( bso::size__ Capacite )
		{
			if ( ( TailleFichier_ == 0 ) && fil::Exists( _Name ) ) {
				fil::size__ RawFileSize = fil::GetSize( _Name );

				if ( RawFileSize > SDR_SIZE_MAX )
					qRFwk();

				TailleFichier_ = (sdr::size__)RawFileSize;
			}

			if ( Capacite > TailleFichier_ )
				TailleFichier_ = Capacite;
		}
		void Flush( void )
		{
			if ( Temoin_.Ouvert )
				File_.Flush();
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
			Temoin_.Mode = fil::mReadOnly;
			TailleFichier_ = 0;
			_Row = qNIL;
			_ID = FLS_UNDEFINED_ID;
			_EpochTimeStamp = 0;
		}
		void Init(
			id__ ID,
			const fnm::name___ &Name,
			fil::mode__ Mode = fil::mReadWrite,
			fls::creation Creation = fls::cFirstUse )
		{
			if ( Name.Amount() != 0 )
			{
				reset();

				_Name.Init( Name );

				Temoin_.Interne = false;

				_ID = ID;

				_Row = _Register( *this, _ID );

			}
			else if ( !Temoin_.Interne )
			{
				reset();

				char Buffer[L_tmpnam];
								
				if ( tmpnam( Buffer ) == NULL )
					qRSys();

				_Name.Init( Buffer );

				Temoin_.Interne = true;

				_ID = ID;

				_Row = _Register( *this, _ID );
			}

			Temoin_.Mode = Mode;

			if ( Creation == fls::cNow  )
				if ( Mode == fil::mReadWrite )
					Open_( false );
				else
					qRFwk();
			else if ( Creation != fls::cFirstUse )
				qRFwk();
		}
			// initialise l'objet avec le nom 'NomFichier'; si NULL, cration d'un nom
		void ReleaseFile( bso::bool__ ReportClosing = true )
		{
			_AdjustPhysicalFileSize();

			if ( Temoin_.Ouvert ) {
				File_.reset();

				if ( ReportClosing )
					_ReportFileClosing( _Row );
			}

			Temoin_.Ouvert = 0;
		}
			// libre le File Descriptor
		void Manual( void )
		{
			Temoin_.Manuel = 1;
		}
			// indique que la fermeture du fichier se fait manuellement
		void Automatic( void )
		{
			Temoin_.Manuel = 0;
		}
			// indique que la fermeture du fichier se fait automatiquement
		void Persistent( void )
		{
			Temoin_.Persistant = true;
		}
		bso::bool__ IsPersistent( void ) const
		{
			return Temoin_.Persistant != 0;
		}
		// bascule en mode d'acces 'Acces'.
		fil::mode__ Mode( fil::mode__ Mode )
		{
			if ( Temoin_.Mode != Mode )
			{
				tol::Swap( Temoin_.Mode, Mode );
				ReleaseFile();
			}

			return Mode;
		}
		// Retourne le mode d'accs.
		fil::mode__ Mode( void ) const
		{
			return Temoin_.Mode;
		}
		void Drop( void ) // Efface le fichier sous-jacent, s'il existe.
		{
			ReleaseFile();

			if ( ( _Name.Amount() != 0 ) && fil::Exists( _Name ) )
				if ( !fil::Remove( _Name ) )
					qRLbr();

			TailleFichier_ = 0;
		}
		const fnm::name___ &GetFileName( void ) const
		{
			return _Name;
		}
		bso::bool__ Exists( void ) const
		{
			return fil::Exists( _Name );
		}
		time_t TimeStamp( void ) const
		{
			if ( Exists() )
				return fil::GetLastModificationTime( _Name );
			else
				return 0;
		}
		fil::size__ FileSize( void ) const
		{
#	if 0
			Open_( false );

			return File_.Size();
#	else
			if ( Exists() )
				return fil::GetSize( _Name );
			else
				return 0;
#	endif
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFile( err::handling__ ErrHandle = err::h_Default )
		{
			return Open_( false, ErrHandle );
		}
		E_RODISCLOSE__( time_t, EpochTimeStamp );
		_file___ &File( void )
		{
			return File_;
		}
	};

	//c The standard storage driver which handle a file as storage.
	class file_storage_driver___
	: public qSD__,
	  public file_storage___
	{
	protected:
		virtual void SDRAllocate( sdr::size__ Size )
		{
			file_storage___::Allocate( Size );
		}
		virtual sdr::size__ SDRSize( void ) const
		{
			fil::size__ Size = FileSize();

			if ( Size > SDR_SIZE_MAX )
				qRFwk();

			return (sdr::size__)Size;
		}
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer )
		{
			file_storage___::Read( Position, Amount, Buffer );
		}
		// lit  partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		virtual void SDRStore(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			file_storage___::Write( Buffer, Amount, Position );
		}
	public:
		file_storage_driver___( void )
		: qSD__(),
		  file_storage___()
		{}
		void reset( bool P = true )
		{
			file_storage___::reset( P );
			qSD__::reset( P );
		}
		//f Return the mode.
		fil::mode__ Mode( void ) const
		{
			return file_storage___::Mode();
		}
		//f 'Mode' becomes the mode.
		fil::mode__ Mode( fil::mode__ Mode )
		{
			return file_storage___::Mode( Mode );
		}
		//f Initialize using 'Filename' as file, open it in mode 'Mode'.
		void Init(
			id__ ID,
			const fnm::name___ &FileName = (char *)NULL,
			fil::mode__ Mode = fil::mReadWrite,
			fls::creation Creation = fls::cFirstUse )
		{
			file_storage___::Init( ID, FileName, Mode, Creation );
			qSD__::Init();
		}
	};

	#define E_FILqSD___	file_storage_driver___

	void ReleaseInactiveFiles_(
		time_t Delay,	// in s.
		bso::uint__ MaxAmount = BSO_UINT_MAX ); // Releases up to 'MaxAmount' files not accessed since 'Delay' ms. Thread-safe.
}

#endif
