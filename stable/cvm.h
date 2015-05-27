/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: cvm.h,v 1.21 2013/03/14 11:09:19 csimon Exp $

#ifndef CVM__INC
#define CVM__INC

#define CVM_NAME		"CVM"

#define	CVM_VERSION	"$Revision: 1.21 $"

#define CVM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( CVM_NODBG )
#define CVM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.21 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/03/14 11:09:19 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D ConVentional Memory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'MNS' instead !"

# include "err.h"
# include "flw.h"
# include "mdr.h"
# include "cpe.h"

namespace cvm {
	//c Basic conventional memory.
	class basic_conventional_memory__
	{
		mdr::size__ _Size;
		mdr::datum__ *&Tampon_;
			// le contenu de la mmoire
	#ifdef CVM_DBG
		void Test_( void )
		{
			if ( Tampon_ == NULL )
				ERRc();
		}
	#endif
	protected:
		// lit  partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		void Recall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer )
		{
	#ifdef CVM_DBG
			if ( Amount )
				Test_();
	#endif
			memcpy( Buffer, Tampon_ + Position, Amount );
		}
		// crit 'Nombre' octets  partir de 'Position' dans 'Tampon'
		void Store(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
	#ifdef CVM_DBG
			if (Amount ) 
				Test_();
	#endif
			memcpy( Tampon_ + Position, Buffer, Amount );
		}
		// alloue 'Nombre' octets
		void Allocate( mdr::size__ Size )
		{
			if ( Size > _Size ) {
				mdr::datum__ *Tampon = (mdr::datum__ *)realloc( Tampon_, Size );

				if ( ( Tampon == NULL ) && ( Size != 0 ) )
					ERRa();

				Tampon_ = Tampon;
				_Size = Size;
			}
		}
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Tampon_ )
					free( Tampon_ );
			}

			Tampon_ = NULL;
			_Size = 0;
		}
		basic_conventional_memory__( mdr::datum__ *&Pointer )
		: Tampon_( Pointer )
		{
			reset( false );
		}
		virtual ~basic_conventional_memory__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			// reset();
			// Inutile de faire un reset. On rutilise la mme mmoire (gain de temps).
		}
	};

	//c Conventional memory driver.
	class conventional_memory_driver__
	: public mdr::E_MEMORY_DRIVER__,
	  public basic_conventional_memory__
	{
	protected:
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer )
		{
			basic_conventional_memory__::Recall( Position, Amount, Buffer );
		}
		// crit 'Nombre' octets  la position 'Position'
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Nombre,
			mdr::row_t__ Position )
		{
			basic_conventional_memory__::Store( Buffer, Nombre, Position );
		}
		// alloue 'Taille' octets
		virtual void MDRAllocate( mdr::size__ Size )
		{
			basic_conventional_memory__::Allocate( Size );
		}
	public:
		void reset( bool P = true )
		{
			basic_conventional_memory__::reset( P );
			E_MEMORY_DRIVER__::reset( P );
		}
		conventional_memory_driver__( mdr::datum__ *&Buffer )
		: memory_driver__(),
		  basic_conventional_memory__( Buffer )
		{
			reset( false );
		}
		virtual ~conventional_memory_driver__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			basic_conventional_memory__::Init();
			E_MEMORY_DRIVER__::Init();
		}
	};

	class standalone_conventional_memory_driver___
	: public conventional_memory_driver__
	{
	private:
		mdr::datum__ *_Buffer;
	public:
		standalone_conventional_memory_driver___( void )
		: conventional_memory_driver__( _Buffer )
		{
		}
	};

	#define E_CONVENTIONAL_MEMORY_DRIVER__ standalone_conventional_memory_driver___
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
