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

//	$Id: mns.h,v 1.4 2013/04/05 16:33:40 csimon Exp $

#ifndef MNS_INC_
#define MNS_INC_

#define MNS_NAME		"MNS"

#define	MNS_VERSION	"$Revision: 1.4 $"

#define MNS_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( MNS_NODBG )
#define MNS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/05 16:33:40 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MaiN Storage
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "cpe.h"

namespace mns {
	class main_storage__
	{
		sdr::size__ _Size;
		sdr::byte__ *&Tampon_;
			// le contenu de la mmoire
# ifdef MNS_DBG
		void Test_( void )
		{
			if ( Tampon_ == NULL )
				qRFwk();
		}
# endif
	protected:
		// lit  partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		void Recall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer )
		{
# ifdef MNS_DBG
			if ( Amount )
				Test_();
# endif
      if ( Amount )
        memcpy( Buffer, Tampon_ + Position, Amount );
		}
		// crit 'Nombre' octets  partir de 'Position' dans 'Tampon'
		void Store(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
# ifdef MNS_DBG
			if ( Amount )
				Test_();
# endif
      if ( Amount )
        memcpy( Tampon_ + Position, Buffer, Amount );
		}
		// alloue 'Nombre' octets
		void Allocate( sdr::size__ Size )
		{
			if ( Size != _Size ) {
				sdr::byte__ *Tampon = (sdr::byte__ *)realloc( Tampon_, Size );

				if ( ( Tampon == NULL ) && ( Size != 0 ) )
					qRAlc();

				Tampon_ = Tampon;
				_Size = Size;
			}
		}
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Tampon_ != NULL )
					free( Tampon_ );
			}

			Tampon_ = NULL;
			_Size = 0;
		}
		main_storage__( sdr::byte__ *&Pointer )
		: Tampon_( Pointer )
		{
			reset( false );
		}
		virtual ~main_storage__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			// reset();
			// Inutile de faire un reset. On rutilise la mme mmoire (gain de temps).
		}
		sdr::size__ Size( void ) const
		{
			return _Size;
		}
	};

	typedef sdr::sStorageDriver sStorageDriver_;

	class main_storage_driver__
	: public sStorageDriver_,
	  public main_storage__
	{
	protected:
		virtual sdr::size__ SDRSize( void ) const
		{
			return main_storage__::Size();
		}
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer )
		{
			main_storage__::Recall( Position, Amount, Buffer );
		}
		// crit 'Nombre' octets  la position 'Position'
		virtual void SDRStore(
			const sdr::byte__ *Buffer,
			sdr::size__ Nombre,
			sdr::row_t__ Position )
		{
			main_storage__::Store( Buffer, Nombre, Position );
		}
		// alloue 'Taille' octets
		virtual void SDRAllocate( sdr::size__ Size )
		{
			main_storage__::Allocate( Size );
		}
	public:
		void reset( bool P = true )
		{
			main_storage__::reset( P );
			sStorageDriver_::reset( P );
		}
		main_storage_driver__( sdr::byte__ *&Buffer )
		: sStorageDriver_(),
		  main_storage__( Buffer )
		{
			reset( false );
		}
		virtual ~main_storage_driver__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			main_storage__::Init();
			sStorageDriver_::Init();
		}
	};

	class standalone_conventional_memory_driver___
	: public main_storage_driver__
	{
	private:
		sdr::byte__ *_Buffer;
	public:
		standalone_conventional_memory_driver___( void )
		: main_storage_driver__( _Buffer )
		{}
	};

	#define E_MAIN_STORAGE_DRIVER__ standalone_main_storage_driver___
}

/*$END$*/
#endif
