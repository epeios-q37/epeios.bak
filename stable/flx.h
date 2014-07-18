/*
	Header for the 'flx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: flx.h,v 1.65 2013/04/19 19:28:01 csimon Exp $

#ifndef FLX__INC
#define FLX__INC

#define FLX_NAME		"FLX"

#define	FLX_VERSION	"$Revision: 1.65 $"

#define FLX_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FLXTutor;

#if defined( E_DEBUG ) && !defined( FLX_NODBG )
#define FLX_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.65 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/19 19:28:01 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FLow eXtension 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "fdr.h"
# include "flw.h"
# include "bso.h"
# include "bch.h"
# include "cpe.h"
# include "cslio.h"

# ifdef CPE__MT
#  define FLX__MT
# endif

# ifdef FLX__MT
#  include "mtx.h"
# endif

# ifndef FLX_BUFFER_BUFFER_SIZE
//d Size of the buffer of a 'flx::buffer_flow___'.
#  define FLX_BUFFER_BUFFER_SIZE	100
# endif

# ifndef FLX_BUNCH_BUFFER_SIZE
//d Size of the buffer of a 'flx::bunch_flow___'.
#  define FLX_BUNCH_BUFFER_SIZE		500
# endif

# ifndef FLX_VOID_BUFFER_SIZE
//d Size of the buffer of a 'flx::void_oflow___'.
#  define FLX_VOID_BUFFER_SIZE		500
# endif

namespace flx {

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> E_TTCLONE__( fdr::iflow_driver___<cache_size>, _iflow_driver___ );

	//c Buffer as a standard input flow.
	class buffer_iflow_driver___
	: public _iflow_driver___<>
	{
	private:
		// Pointeur sur le prochain caractère à lire.
		const fdr::datum__ *Tampon_;
		// Nombre de caractère pouvant encore être lus.
		bso::size__ Taille_;
	protected:
		//v Is called if there is asked for more data as availble.
		virtual void FLXUnavailable( void )
		{
			ERRFwk();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			if ( !Taille_ )
				FLXUnavailable();
			else
			{
				if ( Maximum > Taille_ )
					Maximum = Taille_;
		
				memcpy( Buffer, Tampon_, (size_t)Maximum );

				Tampon_ += Maximum;

				Taille_ -= Maximum;
			}

			return Maximum;
		}
		virtual void FDRDismiss( void )
		{}
	public:
		void reset( bool P = true )
		{
			_iflow_driver___<>::reset( P );
			Taille_ = 0;
			Tampon_ = NULL;
		}
		buffer_iflow_driver___( void )
		{
			reset( false );
		}
		~buffer_iflow_driver___( void )
		{
			reset();
		}
		/*f Initialization with the buffer 'Buffer' of size 'Size'..'Size' is not
		needed if you are sure that you don't exceed the buffer size. */
		void Init(
			const fdr::datum__ *Buffer,
			fdr::thread_safety__ ThreadSafety,
			bso::size__ Size = FLW_AMOUNT_MAX )
		{
			reset();

			_iflow_driver___<>::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	typedef flw::iflow__ _iflow__;

	//c Buffer as a standard input flow.
	class buffer_iflow___
	: public _iflow__
	{
	private:
		buffer_iflow_driver___ _Driver;
	public:
		void reset( bool P = true )
		{
			_Driver.reset( P );
			_iflow__::reset( P );
		}
		buffer_iflow___( void )
		{
			reset( false );
		}
		~buffer_iflow___( void )
		{
			reset( true );
		}
		/*f Initialization with the buffer 'Buffer' of size 'Size'..'Size' is not
		needed if you are sure that you don't exceed the buffer size. */
		void Init(
			const flw::datum__ *Buffer,
			bso::size__ Size = FLW_AMOUNT_MAX,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_Driver.Init( Buffer, fdr::tsDisabled, Size );
			_iflow__::Init( _Driver, AmountMax );
		}
	};

	typedef fdr::oflow_driver___<> _oflow_driver___;

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow_driver___
	: public _oflow_driver___
	{
	private:
		// Pointeur sur le prochain caractère à écrire.
		fdr::datum__ *Tampon_;
		// Nombre de caractères pouvant encore être écris.
		bso::size__ Taille_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( Maximum > Taille_ )
				Maximum = Taille_;

			memcpy( Tampon_, Buffer, Maximum );

			Tampon_ += Maximum;
			Taille_ -= Maximum;

			return Maximum;
		}
		virtual void FDRCommit( void )
		{}
	public:
		void reset( bool P = true )
		{
			_oflow_driver___::reset( P );

			Tampon_ = NULL;
			Taille_ = 0;
		}
		buffer_oflow_driver___( void )
		{
			reset( false );
		}
		~buffer_oflow_driver___( void )
		{
			reset( true );
		}
		//f Initialization with 'Buffer' of size 'Size'.
		void Init(
			fdr::datum__ *Buffer,
			fdr::thread_safety__ ThreadSafety,
			bso::size__ Size )
		{
			reset();

			_oflow_driver___::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	typedef flw::oflow__ _oflow__;

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow___
	: public _oflow__
	{
	private:
		buffer_oflow_driver___ _Driver;
		// The cache.
		flw::datum__ _Cache[FLX_BUFFER_BUFFER_SIZE];
	public:
		void reset( bool P = true )
		{
			_oflow__::reset( P );
			_Driver.reset( P );
		}
		buffer_oflow___( void )
		{
			reset( false );
		}
		~buffer_oflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'Buffer' of size 'Size'.
		void Init(
			flw::datum__ *Buffer,
			bso::size__ Size,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			reset();

			_Driver.Init( Buffer, fdr::tsDisabled, Size );
			oflow__::Init( _Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__, int cache_size> class bunch_iflow_driver___
	: public _iflow_driver___<cache_size>
	{ 
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			if ( (fdr::size__)Maximum > ( Bunch_->Amount() - Position_ ) )
				Maximum = ( Bunch_->Amount() - Position_ );

			if ( Maximum )
			{
				Bunch_->Recall( Position_, Maximum, (so__ *)Buffer );
				Position_ += Maximum;
			}

			return Maximum;
		}
		virtual void FDRDismiss( void )
		{}
	private:
		const bunch_ *Bunch_;
		sdr::row_t__ Position_;
	public:
		void reset( bool P = true )
		{
			_iflow_driver___<cache_size>::reset( P );
			Bunch_ = NULL;
			Position_ = 0;
		}
		bunch_iflow_driver___( void )
		{
			reset( false );
		}
		~bunch_iflow_driver___( void )
		{
			reset( true );
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init(
			const bunch_ &Bunch,
			fdr::thread_safety__ ThreadSafety,
			sdr::row_t__ Position = 0 )
		{
			reset();

			Bunch_ = &Bunch;
			Position_ = Position;

			_iflow_driver___<cache_size>::Init( ThreadSafety );
		}
	};

# define E_STRING_IFLOW_DRIVER___	bunch_iflow_driver___<str::string_, bso::char__>

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__,int  CacheSize = FLX_BUNCH_BUFFER_SIZE> class bunch_iflow__
	: public _iflow__
	{ 
	private:
		bunch_iflow_driver___<bunch_, so__, CacheSize> _Driver;
	public:
		bunch_iflow__( void )
		{
			reset( false );
		}
		~bunch_iflow__( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			_iflow__::reset( P );
			_Driver.reset( P );
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init(
			const bunch_ &Bunch,
			sdr::row_t__ Position = 0,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			reset();

			_Driver.Init( Bunch, fdr::tsDisabled, Position );
			iflow__::Init( _Driver, AmountMax );
		}
	};

# define E_STRING_IFLOW__	bunch_iflow__<str::string_, bso::char__>

	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow_driver___
	: public _oflow_driver___
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			_Bunch->Append( (const so__ *)Buffer, Maximum );

			return Maximum;
		}
		virtual void FDRCommit()
		{}
	private:
		bunch_ *_Bunch;
	public:
		bunch_oflow_driver___( void )
		{
			reset( false );

			_Bunch = NULL;
		}
		~bunch_oflow_driver___( void )
		{
			reset();
		}
		void reset( bool P = true )
		{
			_oflow_driver___::reset( P );

			_Bunch = NULL;
		}
		//f Initializing with the buffer bunch 'Bunch'.
		void Init(
			bunch_ &Bunch,
			fdr::thread_safety__ ThreadSafety )
		{
			reset();

			_Bunch = &Bunch;

			_oflow_driver___::Init( ThreadSafety );
		}
	};

# define E_STRING_OFLOW_DRIVER___	bunch_oflow_driver___<str::string_, bso::char__>


	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow___
	: public _oflow__
	{
	private:
		bunch_oflow_driver___<bunch_, so__> _Driver;
		flw::datum__ _Cache[FLX_BUNCH_BUFFER_SIZE];
	public:
		bunch_oflow___( )
		{
			reset( false );
		}
		~bunch_oflow___( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			_oflow__::reset( P );
			_Driver.reset( P );
		}
		//f Initializing with the buffer bunch 'Bunch'.
		void Init(
			bunch_ &Bunch,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			reset();

			_Driver.Init( Bunch, fdr::tsDisabled );
			_oflow__::Init( _Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

# define E_STRING_OFLOW___	bunch_oflow___<str::string_, bso::char__>

	typedef fdr::oflow_driver___<> _oflow_driver___;

	// 'driver' qui n'écrit dans rien.
	class void_oflow_driver___
	: public _oflow_driver___
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *,
			fdr::size__ Maximum )
		{
			return Maximum;
		}
		virtual void FDRCommit()
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
		}
		void_oflow_driver___( void )
		{
			reset( false );
		}
		~void_oflow_driver___( void )
		{
			reset();
		}
		void Init( fdr::thread_safety__ ThreadSafety )
		{
			_oflow_driver___::Init( ThreadSafety );
		}
	};	

	extern void_oflow_driver___ VoidOFlowDriver;

	// 'flow' qui n'écrit dans rien.
	class void_oflow__
	: public _oflow__
	{
	private:
			// The cache.
		flw::datum__ _Cache[FLX_VOID_BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow__::reset( P );
		}
		void_oflow__( void )
		{
			reset( false );
		}
		~void_oflow__( void )
		{
			reset();
		}
		void Init( flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_oflow__::Init( VoidOFlowDriver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

	// ATTENTION : n'est pas 'therad-safe'.
	extern void_oflow__ VoidOFlow;


	// 'driver' qui ne lit rien.
	class void_iflow_driver___
	: public _iflow_driver___<>
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return 0;
		}
		virtual void FDRDismiss( void )
		{
		}
	public:
		~void_iflow_driver___( void )
		{
			reset();
		}
		void Init( fdr::thread_safety__ ThreadSafety )
		{
			fdr::iflow_driver___<>::Init( ThreadSafety );
		}
	};	

	extern void_iflow_driver___ VoidIFlowDriver;

	// 'flow' qui ne lit rien.
	class void_iflow__
	: public _iflow__
	{
	private:
	public:
		void reset( bso::bool__ P = true )
		{
			iflow__::reset( P );
		}
		void_iflow__( void )
		{
			reset( false );
		}
		~void_iflow__( void )
		{
			reset();
		}
		void Init( flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			iflow__::Init( VoidIFlowDriver, AmountMax );
		}
	};

	// ATTENTION : n'est pas 'thread-safe'.
	extern void_iflow__ VoidIFlow;

	// 'driver' qui relaye un autre 'driver'.
	class relay_oflow_driver___
	: public _oflow_driver___
	{
	private:
		fdr::oflow_driver_base___ *_Driver;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( _Driver == NULL )
				ERRFwk();

			return _Driver->Write( Buffer, Maximum );
		}
		virtual void FDRCommit()
		{
			if ( _Driver == NULL )
				ERRFwk();

			return _Driver->Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_Driver = NULL;
		}
		relay_oflow_driver___( void )
		{
			reset( false );
		}
		~relay_oflow_driver___( void )
		{
			reset();
		}
		void Init(
			fdr::oflow_driver_base___ &Driver,
			fdr::thread_safety__ ThreadSafety )
		{
			_Driver = &Driver;
			_oflow_driver___::Init( ThreadSafety );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Driver != NULL;
		}
	};	

	// 'driver' qui relaye un autre 'driver'.
	class relay_iflow_driver___
	: public _iflow_driver___<>
	{
	private:
		fdr::iflow_driver_base___ *_Driver;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			if ( _Driver == NULL )
				ERRFwk();

			return _Driver->Read( Maximum, Buffer, fdr::bNonBlocking );
		}
		virtual void FDRDismiss( void )
		{
			if ( _Driver == NULL )
				ERRFwk();

			_Driver->Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___<>::reset( P );
			_Driver = NULL;
		}
		relay_iflow_driver___( void )
		{
			reset( false );
		}
		~relay_iflow_driver___( void )
		{
			reset();
		}
		void Init(
			fdr::iflow_driver_base___ &Driver,
			fdr::thread_safety__ ThreadSafety )
		{
			_Driver = &Driver;
			_iflow_driver___<>::Init( ThreadSafety );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Driver != NULL;
		}
	};	

	// 'driver' qui relaye un 'oflow', mais dont la taille est 'encodée' dans le flux.
	class sizes_embedded_oflow_relay_driver___
	: public _oflow_driver___
	{
	private:
		_oflow__ *_Flow;
		fdr::size__ _EmbeddedSizeRemainder;
		bso::bool__ _PendingCommit;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ &Size = Maximum;

			if ( _Flow == NULL )
				ERRFwk();

			if ( _EmbeddedSizeRemainder == 0 ) {
				_EmbeddedSizeRemainder = Size;
				dtfptb::VPut( _EmbeddedSizeRemainder, *_Flow );
			}

			Size = _Flow->WriteUpTo(Buffer, ( Size > _EmbeddedSizeRemainder ? _EmbeddedSizeRemainder : Size ) );

			if ( Size == 0 )
				ERRDta();

			_EmbeddedSizeRemainder -= Size;

			_PendingCommit = true;

			return Size;
		}
		virtual void FDRCommit( void )
		{
			if ( !_PendingCommit )	// Pour éviter qu'un 'commit( suite à un 'reset()' réecrive un '0'.
				return;

			if ( _Flow == NULL )
				ERRFwk();

			if ( _EmbeddedSizeRemainder != 0 )
				ERRDta();

			dtfptb::VPut( (bso::u8__)0, *_Flow );

			_PendingCommit = false;

			//	_Flow->Commit();	// Le 'flow' est enclavé dans un autre flot ; c'est ce dernier qui devra faire le 'commit'.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_Flow = NULL;
			_EmbeddedSizeRemainder = 0;
			_PendingCommit = false;
		}
		E_CVDTOR( sizes_embedded_oflow_relay_driver___)
		void Init(
			_oflow__ &Flow,
			fdr::thread_safety__ ThreadSafety )
		{
			_Flow = &Flow;
			_EmbeddedSizeRemainder = 0;
			_PendingCommit = false;
			_oflow_driver___::Init( ThreadSafety );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Flow != NULL;
		}
	};

	enum dismiss_handling__ {
		dhPropagate,	// 'Dismiss()' est propagé au flux embarqué.
		dhHold,		// 'Dismiss()' n'est PAS propagé au flux embarqué. 
		dh_amount,
		dh_Undefined
	};


	// 'driver' qui relaye un 'iflow', mais dont la taille est 'encodée' dans le flux.
	class sizes_embedded_iflow_relay_driver___
	: public _iflow_driver___<>
	{
	private:
		_iflow__ *_Flow;
		fdr::size__ _EmbeddedSizeRemainder;
		dismiss_handling__ _DismissHandling;
		bso::bool__ _AllRed;
		void _Purge( void )
		{
			do {
				while ( _EmbeddedSizeRemainder != 0 ) {
					_Flow->Get();
					_EmbeddedSizeRemainder--;
				}

				dtfptb::VGet( *_Flow, _EmbeddedSizeRemainder );
			} while ( _EmbeddedSizeRemainder != 0 );
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			fdr::size__ &Size = Maximum;

			if ( _Flow == NULL )
				ERRFwk();

			if ( _EmbeddedSizeRemainder == 0 )
				dtfptb::VGet( *_Flow, _EmbeddedSizeRemainder );

			if ( _EmbeddedSizeRemainder == 0 ) {
				_AllRed = true;
				return 0;
			} else
				_AllRed = false;

			Size = _Flow->ReadUpTo( ( Size > _EmbeddedSizeRemainder ? _EmbeddedSizeRemainder : Size ), Buffer );

			if ( Size == 0 )
				ERRDta();

			_EmbeddedSizeRemainder -= Size;

			return Size;
		}
		virtual void FDRDismiss( void )
		{
			if ( _Flow == NULL )
				ERRFwk();

			if ( !_AllRed )
				_Purge();

			_AllRed = true;

			if ( _DismissHandling == dhPropagate )
				_Flow->Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___<>::reset( P );
			_Flow = NULL;
			_EmbeddedSizeRemainder = 0;
			_DismissHandling = dh_Undefined;
			_AllRed = false;
		}
		E_CVDTOR( sizes_embedded_iflow_relay_driver___)
		void Init(
			_iflow__ &Flow,
			dismiss_handling__ DismissHandling,
			fdr::thread_safety__ ThreadSafety )
		{
			_Flow = &Flow;
			_EmbeddedSizeRemainder = 0;
			_DismissHandling = DismissHandling,
			_iflow_driver___<>::Init( ThreadSafety );
			_AllRed = true;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Flow != NULL;
		}
	};	

# ifdef FLX__MT

#  if 0 // Bogué !!!
	// Permet de lire à partir d'un 'iflow' ce qui est écrit dans un 'oflow'.
	class mediator_ioflow_driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		mtx::mutex_handler__ 
			_Read,
			_Write;
		const fdr::datum__ *_Buffer;
		fdr::size__ _Size;
		fdr::size__ _Red;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			mtx::Lock( _Write );

			_Buffer = Buffer;
			_Size = Maximum;
			_Red = 0;

			mtx::Unlock( _Read );

			return _Red;
		}
		virtual void FDRCommit( void )
		{}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{

			mtx::Lock( _Read );

			if ( Maximum > _Size )
				Maximum = _Size;

			memcpy( Buffer, _Buffer, Maximum );

			_Red = Maximum;

			mtx::Unlock( _Write );

			return Maximum;
		}
		virtual void FDRDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{

			if ( P ) {
				if ( _Read != MTX_INVALID_HANDLER )
					mtx::Delete( _Read, true );

				if ( _Write != MTX_INVALID_HANDLER )
					mtx::Delete( _Write,true );
			}

			fdr::ioflow_driver___<>::reset( P );

			_Buffer = NULL;
			_Red = _Size = 0;
			_Read = _Write = MTX_INVALID_HANDLER;
		}
		mediator_ioflow_driver___( void )
		{
			reset( false );
		}
		~mediator_ioflow_driver___( void )
		{
			reset();
		}
		void Init( fdr::thread_safety__ ThreadSafety )
		{
			if ( _Read != MTX_INVALID_HANDLER )
				mtx::Delete( _Read, true );

			if ( _Write != MTX_INVALID_HANDLER )
				mtx::Delete( _Write, true );

			fdr::ioflow_driver___<>::Init( ThreadSafety );

			_Size = _Red = 0;

			_Buffer = NULL;

			_Read = mtx::Create( mtx::mSynchronizing );
			_Write = mtx::Create( mtx::mSynchronizing );

			mtx::Lock( _Read );
		}
	};

	template <int OutCacheSize = FLW__OUTPUT_CACHE_SIZE> class mediator_ioflow___
	: public flw::standalone_ioflow__<OutCacheSize>
	{
	private:
		mediator_ioflow_driver___ _Driver;
	public:
		void Init( void )
		{
			_Driver.Init( fdr::tsDisabled );
			flw::standalone_ioflow__<OutCacheSize>Init( _Driver );
		}
	};
#  endif

# endif

	cslio::descriptor__ _POpen(
		const ntvstr::nstring___ &Command,
		const ntvstr::nstring___ &Mode );

	inline void _PClose( cslio::descriptor__ Descriptor )
		{
# ifdef CPE_WIN
			_pclose( Descriptor );
# elif defined( CPE_POSIX )
			pclose( Descriptor );
# else
#  error
#endif
		}

	template <typename io> class _exec_driver___
	{
	private:
		cslio::descriptor__ _Descriptor;
		io _IO;
	public:
		void reset( bso::bool__ P = true )
		{
			_IO.reset( P );

			if ( P ) 
				if ( _Descriptor != cslio::UndefinedDescriptor )
					_PClose( _Descriptor );

			_Descriptor = cslio::UndefinedDescriptor;
				
		}
		E_CVDTOR( _exec_driver___ );
		bso::bool__ Init(
			const ntvstr::nstring___ &Command,
			const ntvstr::nstring___ &Mode )
		{
			if ( _Descriptor != cslio::UndefinedDescriptor )
				_PClose( _Descriptor );

			_Descriptor = _POpen(Command, Mode );

			if ( _Descriptor == cslio::UndefinedDescriptor )
				return false;

			_IO.Init( _Descriptor );

			return true;
		}
		io IO( void )
		{
			return _IO;
		}
	};

	// Lance une commande dans le shell et récupère les données écrites par la commande.
	class exec_iflow_driver___
	: public _iflow_driver___<>,
	  public _exec_driver___<cslio::standard_input__>
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			if ( IO().OnEOF() )
				return 0;

			Maximum = IO().Read( Maximum, Buffer );

			if ( Maximum == 0 )
				if ( !IO().OnEOF() )
					ERRDta();

			return Maximum;

		}
		virtual void FDRDismiss( void )
		{
			// Nothing to do.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___::reset( P );
			_exec_driver___::reset( P );
				
		}
		E_CVDTOR( exec_iflow_driver___ );
		bso::bool__ Init(
			const ntvstr::nstring___ &Command,
			fdr::thread_safety__ ThreadSafety )
		{
			_iflow_driver___<>::Init( ThreadSafety );
			return _exec_driver___::Init(Command, "rb" );
		}
	};


	typedef cslio::standard_output__ _output__;

	// Lance une commande dans le shell en lui passant des données.
	class exec_oflow_driver___
	: public _oflow_driver___,
	  public _exec_driver___<cslio::standard_output__>
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( IO().Write( Buffer, Maximum ) != Maximum )
				ERRDta();

			return Maximum;
		}
		virtual void FDRCommit( void )
		{
			IO().Flush();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_exec_driver___::reset( P );
		}
		E_CVDTOR( exec_oflow_driver___ );
		bso::bool__ Init(
			const ntvstr::nstring___ &Command,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			_oflow_driver___::Init( ThreadSafety );
			return _exec_driver___::Init(Command, "wb" );
		}
	};

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> E_TTCLONE__( fdr::ioflow_driver___<cache_size>, _ioflow_driver___ );

	class exec_ioflow_driver___
	: public _ioflow_driver___<>
	{
	private:
		HANDLE _In, _Out;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum );
		virtual void FDRCommit( void )
		{

		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer );
		virtual void FDRDismiss( void )
		{
			// Nothing to do.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Out != NULL )
					CloseHandle( _Out );
				if ( _In != NULL )
					CloseHandle( _In );
			}

			_Out = NULL;
			_In = NULL;

			_ioflow_driver___::reset( P );
		}
		E_CDTOR( exec_ioflow_driver___ );
		bso::bool__ Init(
			const ntvstr::nstring___ &Command,
			fdr::thread_safety__ ThreadSafety );
	};

	template <typename flow, typename driver> class _exec_flow___
	: public flow
	{
	private:
		driver _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			flow::reset( P );
			_Driver.reset( P );
		}
		E_CDTOR( _exec_flow___ );
		bso::bool__ Init( const ntvstr::nstring___ &Command )
		{
			flow::Init( _Driver );

			return _Driver.Init( Command, fdr::tsDisabled );
		}
	};

	typedef _exec_flow___<flw::standalone_iflow__<>, exec_iflow_driver___> exec_iflow__;

	typedef _exec_flow___<flw::standalone_oflow__<>, exec_oflow_driver___> exec_oflow__;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
