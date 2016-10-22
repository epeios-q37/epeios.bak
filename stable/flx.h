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

// FLow eXtension 

#ifndef FLX__INC
# define FLX__INC

# define FLX_NAME		"FLX"

# include "err.h"
# include "fdr.h"
# include "flw.h"
# include "bso.h"
# include "bch.h"
# include "cpe.h"
# include "cslio.h"
# include "str.h"

# ifdef CPE_F_MT
#  define FLX__MT
# endif

# ifdef CPE_S_WIN
#  define FLX__WIN
# elif defined( CPE_S_POSIX )
#  define FLX__POSIX
# else
#  error
# endif

# ifdef FLX__MT
#  include "mtx.h"
# endif

# ifndef FLX_COPY_BUFFER_SIZE
#  define FLX_COPY_BUFFER_SIZE	1024
# endif

# ifndef FLX_BUFFER_BUFFER_SIZE
#  define FLX_BUFFER_BUFFER_SIZE	100
# endif

# ifndef FLX_BUNCH_BUFFER_SIZE
#  define FLX_BUNCH_BUFFER_SIZE		500
# endif

# ifndef FLX_VOID_BUFFER_SIZE
#  define FLX_VOID_BUFFER_SIZE		500
# endif

/***************/
/***** NEW *****/
/***************/

namespace flx {
	template < typename bunch_, typename so__,int  CacheSize = FLX_BUNCH_BUFFER_SIZE> class bunch_iflow__;
	typedef bunch_iflow__<str::string_, bso::char__> sStringIFlow;

	template < typename bunch_, typename so__> class bunch_oflow___;
	typedef bunch_oflow___<str::string_, bso::char__> rStringOFlow;

	class string_text_oflow__;
	typedef string_text_oflow__ fStrinfTOflow;

	using flw::Copy;
}

/***************/
/***** OLD *****/
/***************/


namespace flx {

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> E_TTCLONE__( fdr::iflow_driver___<cache_size>, _iflow_driver___ );

	//c Buffer as a standard input flow.
	class buffer_iflow_driver___
	: public _iflow_driver___<>
	{
	private:
		// Pointeur sur le prochain caractre  lire.
		const fdr::byte__ *Tampon_;
		// Nombre de caractre pouvant encore tre lus.
		bso::size__ Taille_;
	protected:
		// Mthode thoriquement inutile. Son type de retour est pass de 'void'  'int' pour dtecter les classes qui la surcharge. Sera supprim  terme.
		virtual int FLXUnavailable( void )
		{
			qRFwk();

			return 0;
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			if ( !Taille_ )
				return 0;
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
			const fdr::byte__ *Buffer,
			fdr::thread_safety__ ThreadSafety,
			bso::size__ Size = FLW_AMOUNT_MAX )
		{
			reset();

			_iflow_driver___<>::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;

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
			const flw::byte__ *Buffer,
			bso::size__ Size = FLW_AMOUNT_MAX )
		{
			_Driver.Init( Buffer, fdr::tsDisabled, Size );
			_iflow__::Init( _Driver );
		}
	};

	typedef fdr::oflow_driver___<> _oflow_driver___;

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow_driver___
	: public _oflow_driver___
	{
	private:
		// Pointeur sur le prochain caractre  crire.
		fdr::byte__ *Tampon_;
		// Nombre de caractres pouvant encore tre cris.
		bso::size__ Taille_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
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
			fdr::byte__ *Buffer,
			fdr::thread_safety__ ThreadSafety,
			bso::size__ Size )
		{
			reset();

			_oflow_driver___::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	typedef flw::standalone_oflow__<>  _oflow__;

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow___
	: public _oflow__
	{
	private:
		buffer_oflow_driver___ _Driver;
		// The cache.
		flw::byte__ _Cache[FLX_BUFFER_BUFFER_SIZE];
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
			flw::byte__ *Buffer,
			bso::size__ Size )
		{
			reset();

			_Driver.Init( Buffer, fdr::tsDisabled, Size );
			oflow__::Init( _Driver, _Cache, sizeof( _Cache ) );
		}
	};

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__, int cache_size> class bunch_iflow_driver___
	: public _iflow_driver___<cache_size>
	{ 
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
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
	template < typename bunch_, typename so__,int  CacheSize> class bunch_iflow__
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
			sdr::row_t__ Position = 0 )
		{
			reset();

			_Driver.Init( Bunch, fdr::tsDisabled, Position );
			iflow__::Init( _Driver );
		}
		flw::sIFlow &operator *( void )
		{
			return *this;
		}
	};

# define E_STRING_IFLOW__	bunch_iflow__<str::string_, bso::char__>

	namespace {
		typedef txf::text_iflow__ _tiflow__;
	}

	class string_text_iflow__
	: public _tiflow__
	{
	private:
		E_STRING_IFLOW__ _Flow;
	public:
		void reset( bso::bool__ P = true )
		{
			_Flow.reset( P );
			_tiflow__::reset( P );
		}
		E_CVDTOR( string_text_iflow__ );
		void Init( str::string_ &Target )
		{
			_Flow.Init( Target );
			_tiflow__::Init( _Flow );
		}
	};

# define E_STRING_TOFLOW___	string_text_oflow___


	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow_driver___
	: public _oflow_driver___
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
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
		void Init( bunch_ &Bunch )
		{
			reset();

			_Driver.Init( Bunch, fdr::tsDisabled );
			_oflow__::Init( _Driver );
		}
	};

# define E_STRING_OFLOW___	bunch_oflow___<str::string_, bso::char__>

	namespace {
		typedef txf::text_oflow__ _toflow__;
	}

	class string_text_oflow___
	: public _toflow__
	{
	private:
		E_STRING_OFLOW___ _Flow;
	public:
		void reset( bso::bool__ P = true )
		{
			_Flow.reset( P );
			_toflow__::reset( P );
		}
		E_CVDTOR( string_text_oflow___ );
		void Init( str::string_ &Target )
		{
			_Flow.Init( Target );
			_toflow__::Init( _Flow );
		}
	};

	E_ENUM( access )
	{
		aAllowed,
		aForbidden,
		a_amount,
		a_Undefined,
		a_Default = aForbidden,
	};

	inline void Test_( access__ Access )
	{
		switch ( Access ) {
		case aAllowed:
			break;
		case aForbidden:
			qRFwk();
			break;
		default:
			qRFwk();
			break;
		}
	}

# define E_STRING_TOFLOW___	string_text_oflow___

	typedef fdr::oflow_driver___<> _oflow_driver___;

	// 'driver' qui n'crit dans rien.
	class void_oflow_driver___
	: public _oflow_driver___
	{
	private:
		access__ _Access;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *,
			fdr::size__ Maximum ) override
		{
			Test_( _Access );

			return Maximum;
		}
		virtual void FDRCommit( void ) override
		{
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_Access = a_Undefined;
		}
		E_CDTOR( void_oflow_driver___ );
		void Init(
			fdr::thread_safety__ ThreadSafety,
			access__ Access )
		{
			_oflow_driver___::Init( ThreadSafety );
			_Access = Access;
		}
	};	

	extern void_oflow_driver___ VoidOFlowDriver;

	// 'flow' qui n'crit dans rien.
	class void_oflow__
	: public _oflow__
	{
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
		void Init( void )
		{
			_oflow__::Init( VoidOFlowDriver );
		}
	};

	// ATTENTION : n'est pas 'therad-safe'.
	extern void_oflow__ VoidOFlow;


	// 'driver' qui ne lit rien.
	class void_iflow_driver___
	: public _iflow_driver___<>
	{
	private:
		access__ _Access;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			Test_( _Access );

			return 0;
		}
		virtual void FDRDismiss( void )
		{
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___<>::reset( P );
			_Access = a_Undefined;
		}
		E_CDTOR( void_iflow_driver___ );
		void Init(
			fdr::thread_safety__ ThreadSafety,
			access__ Access )
		{
			fdr::iflow_driver___<>::Init( ThreadSafety );
			_Access = Access;
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
		void Init( void )
		{
			iflow__::Init( VoidIFlowDriver );
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
		flw::oflow__ *_User;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( _Driver == NULL )
				qRFwk();

			return _Driver->Write( _User, Buffer, Maximum );
		}
		virtual void FDRCommit()
		{
			if ( _Driver == NULL )
				qRFwk();

			return _Driver->Commit( _User );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_Driver = NULL;
			_User = NULL;
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
			flw::oflow__ &User,
			fdr::thread_safety__ ThreadSafety )
		{
			_Driver = &Driver;
			_User = &User;
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
		flw::iflow__ *_User;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			if ( _Driver == NULL )
				qRFwk();

			return _Driver->Read( _User, Maximum, Buffer, fdr::bNonBlocking );
		}
		virtual void FDRDismiss( void )
		{
			if ( _Driver == NULL )
				qRFwk();

			_Driver->Dismiss( _User );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___<>::reset( P );
			_Driver = NULL;
			_User = NULL;
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
			flw::iflow__ &User,
			fdr::thread_safety__ ThreadSafety )
		{
			_Driver = &Driver;
			_User = &User;
			_iflow_driver___<>::Init( ThreadSafety );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Driver != NULL;
		}
	};

	qENUM( CommitHandling ) {
		chPropagate,	// 'Commit()' IS propagated to underlying flow.
		chHold,			// 'Commit()' is NOT propagated to underlying flow.
		ch_amount,
		ch_Undefined,
		ch_Default = chPropagate
	};



	// 'driver' qui relaye un 'oflow', mais dont la taille est 'encode' dans le flux.
	class size_embedded_oflow_driver___
	: public _oflow_driver___
	{
	private:
		flw::oflow__ *_Flow;
		fdr::size__ _EmbeddedSizeRemainder;
		bso::bool__ _PendingCommit;
		eCommitHandling CommitHandling_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ &Size = Maximum;

			if ( _Flow == NULL )
				qRFwk();

			if ( _EmbeddedSizeRemainder == 0 ) {
				_EmbeddedSizeRemainder = Size;
				dtfptb::VPut( _EmbeddedSizeRemainder, *_Flow );
			}

			Size = _Flow->WriteUpTo(Buffer, ( Size > _EmbeddedSizeRemainder ? _EmbeddedSizeRemainder : Size ) );

			_EmbeddedSizeRemainder -= Size;

			_PendingCommit = true;

			return Size;
		}
		virtual void FDRCommit( void )
		{
			if ( !_PendingCommit )	// Pour viter qu'un 'commit( suite  un 'reset()' recrive un '0'.
				return;

			if ( _Flow == NULL )
				qRFwk();

			if ( _EmbeddedSizeRemainder == 0 )
				dtfptb::VPut( (bso::u8__)0, *_Flow );

			_PendingCommit = false;

			if ( CommitHandling_ == chPropagate )
				_Flow->Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_Flow = NULL;
			CommitHandling_ = ch_Undefined;
			_EmbeddedSizeRemainder = 0;
			_PendingCommit = false;
		}
		E_CVDTOR( size_embedded_oflow_driver___)
		void Init(
			flw::oflow__ &Flow,
			fdr::thread_safety__ ThreadSafety,
			eCommitHandling CommitHandling )
		{
			_Flow = &Flow;
			CommitHandling_ = CommitHandling;
			_EmbeddedSizeRemainder = 0;
			_PendingCommit = true;	// So that, if there was no data, the final '0' is still written.
			_oflow_driver___::Init( ThreadSafety );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Flow != NULL;
		}
	};

	enum dismiss_handling__ {
		dhPropagate,	// 'Dismiss()' est propag au flux embarqu.
		dhHold,		// 'Dismiss()' n'est PAS propag au flux embarqu. 
		dh_amount,
		dh_Undefined,
		dh_Default = dhPropagate
	};

	// 'driver' qui relaye un 'iflow', mais dont la taille est 'encode' dans le flux.
	class size_embedded_iflow_driver___
	: public _iflow_driver___<>
	{
	private:
		flw::iflow__ *_Flow;
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
			fdr::byte__ *Buffer )
		{
			fdr::size__ &Size = Maximum;

			if ( _Flow == NULL )
				qRFwk();

			if ( _EmbeddedSizeRemainder == 0 ) {
				dtfptb::VGet( *_Flow, _EmbeddedSizeRemainder );

				if ( _EmbeddedSizeRemainder == 0 ) {
					_AllRed = true;
					return 0;
				}
			}

			_AllRed = false;

			Size = _Flow->ReadUpTo( ( Size > _EmbeddedSizeRemainder ? _EmbeddedSizeRemainder : Size ), Buffer );

			if ( Size == 0 ) {
				_AllRed = true;	// Even if not true, there is no more data available in the underlying flow.
				qRFwk();
			}

			_EmbeddedSizeRemainder -= Size;

			return Size;
		}
		virtual void FDRDismiss( void )
		{
			if ( _Flow == NULL )
				qRFwk();

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
		E_CVDTOR( size_embedded_iflow_driver___)
		void Init(
			flw::iflow__ &Flow,
			fdr::thread_safety__ ThreadSafety,
			dismiss_handling__ DismissHandling = dh_Default )
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

	template <typename flow, typename sflow, typename driver, typename handling> class _size_embbeded_flow___
	: public sflow
	{
	private:
		driver _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			sflow::reset( P );
			_Driver.reset( P );
		}
		E_CDTOR( _size_embbeded_flow___ );
		void Init(
			flow &Flow,
			handling Handling )
		{
			sflow::Init( _Driver );
			_Driver.Init( Flow, fdr::tsDisabled, Handling );
		}
	};

	typedef _size_embbeded_flow___<flw::iflow__, _iflow__, size_embedded_iflow_driver___, dismiss_handling__> size_embedded_iflow___;
	typedef _size_embbeded_flow___<flw::oflow__, _oflow__, size_embedded_oflow_driver___, eCommitHandling> size_embedded_oflow___;


# ifdef FLX__MT

#  if 0 // Bugged !!!
	// Permet de lire  partir d'un 'iflow' ce qui est crit dans un 'oflow'.
	class mediator_ioflow_driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		mtx::mutex_handler__ 
			_Read,
			_Write;
		const fdr::byte__ *_Buffer;
		fdr::size__ _Size;
		fdr::size__ _Red;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
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
			fdr::byte__ *Buffer )
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
		const ntvstr::string___ &Command,
		const ntvstr::string___ &Mode );

	inline void _PClose( cslio::descriptor__ Descriptor )
		{
# ifdef FLX__WIN
			_pclose( Descriptor );
# elif defined( FLX__POSIX )
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
			const ntvstr::string___ &Command,
			const ntvstr::string___ &Mode )
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

# ifdef FLX__WIN
	E_CDEF(char *, _ReadMode, "rb" );
	E_CDEF(char *, _WriteMode, "wb" );
# elif defined( FLX__POSIX )
	E_CDEF(char *, _ReadMode, "r" );
	E_CDEF(char *, _WriteMode, "w" );
# else
#  error
#endif
	

	// Lance une commande dans le shell et rcupre les donnes crites par la commande.
	class exec_iflow_driver___
	: public _iflow_driver___<>,
	  public _exec_driver___<cslio::standard_input__>
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			if ( IO().OnEOF() )
				return 0;

			Maximum = IO().Read( Maximum, Buffer );

			if ( Maximum == 0 )
				if ( !IO().OnEOF() )
					qRFwk();

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
			const ntvstr::string___ &Command,
			fdr::thread_safety__ ThreadSafety )
		{
			_iflow_driver___<>::Init( ThreadSafety );
			return _exec_driver___::Init(Command, _ReadMode );
		}
	};


	typedef cslio::standard_output__ _output__;

	// Lance une commande dans le shell en lui passant des donnes.
	class exec_oflow_driver___
	: public _oflow_driver___,
	  public _exec_driver___<cslio::standard_output__>
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( IO().Write( Buffer, Maximum ) != Maximum )
				qRFwk();

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
			const ntvstr::string___ &Command,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			_oflow_driver___::Init( ThreadSafety );
			return _exec_driver___::Init(Command, _WriteMode );
		}
	};

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> E_TTCLONE__( fdr::ioflow_driver___<cache_size>, _ioflow_driver___ );

# ifdef FLX__WIN
	typedef  HANDLE pipe_descriptor__;
	E_CDEF( pipe_descriptor__, UndefinedPipeDescriptor, NULL );
# elif defined FLX__POSIX
	typedef int pipe_descriptor__;
	E_CDEF( pipe_descriptor__, UndefinedPipeDescriptor, -1 );
# else
#  error
# endif

	class exec_ioflow_driver___
	: public _ioflow_driver___<>
	{
	private:
		pipe_descriptor__ _In, _Out, _Err;
		void Close_( pipe_descriptor__ Descriptor )
		{
# ifdef FLX__WIN
			CloseHandle( Descriptor );
# elif defined( FLX__POSIX )
			close( Descriptor );
# else
#  error
# endif
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum );
		virtual void FDRCommit( void )
		{
			if ( _In != UndefinedPipeDescriptor )
				Close_( _In );

			_In = UndefinedPipeDescriptor;
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer );
		virtual void FDRDismiss( void )
		{
			// Nothing to do.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _In != UndefinedPipeDescriptor )
					Close_( _In );
				if ( _Out != UndefinedPipeDescriptor )
					Close_( _Out );
				if ( _Err != UndefinedPipeDescriptor )
					Close_( _Err );
			}

			_In = _Out = _Err = UndefinedPipeDescriptor;

			_ioflow_driver___::reset( P );
		}
		E_CDTOR( exec_ioflow_driver___ );
		bso::bool__ Init(
			const ntvstr::string___ &Command,
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
		bso::bool__ Init( const ntvstr::string___ &Command )
		{
			flow::Init( _Driver );

			return _Driver.Init( Command, fdr::tsDisabled );
		}
	};

	typedef _exec_flow___<flw::standalone_iflow__<>, exec_iflow_driver___> exec_iflow__;

	typedef _exec_flow___<flw::standalone_oflow__<>, exec_oflow_driver___> exec_oflow__;
}

/***************/
/***** NEW *****/
/***************/

namespace flx {
	class cSizeDelimitedOFlow
	{
	protected:
		virtual void FLXOnEOF( void ) = 0;
	public:
		void OnEOF( void )
		{
			return FLXOnEOF();
		}
		qCALLBACK( SizeDelimitedOFlow );
	};

	class rSizeDelimitedIDriver
	: public _iflow_driver___<>
	{
	private:
		fdr::sSize Size_;
		flw::sIFlow Flow_;
		cSizeDelimitedOFlow *Callback_;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::sSize Amount = 0;

			if ( Size_ != 0 )
				Amount = Flow_.ReadUpTo( Maximum > Size_ ? Size_ : Maximum, Buffer );
			else if ( Callback_ != NULL )
				Callback_->OnEOF();

			Size_ -= Amount;

			return Amount;
		}
		virtual void FDRDismiss( void ) override
		{
			if ( Size_ != 0 )
				qRFwk();

			Flow_.Dismiss();
		}
	public:
		void reset( bso::sBool P = true )
		{
			_iflow_driver___::reset( P );

			tol::reset( P, Flow_, Callback_ );

			Size_ = 0;
		}
		qCVDTOR( rSizeDelimitedIDriver );
		void Init(
			fdr::sSize Size,
			fdr::rIDriver &Driver,
			cSizeDelimitedOFlow *Callback,
			fdr::thread_safety__ ThreadSafety )
		{
			_iflow_driver___::Init( ThreadSafety );

			Size_ = Size;
			Flow_.Init( Driver );
			Callback_ = Callback;
		}
	};

	typedef flw::sDressedIFlow<> sDressedIFlow_;

	class rSizeDelimitedOFlow
	: public sDressedIFlow_
	{
	private:
		rSizeDelimitedIDriver Driver_;
	public:
		void reset( bso::sBool P = true )
		{
			sDressedIFlow_::reset( P );
			tol::reset( P, Driver_ );
		}
		qCDTOR( rSizeDelimitedOFlow );
		void Init(
			fdr::sSize Size,
			fdr::rIDriver &Driver,
			cSizeDelimitedOFlow *Callback = NULL )
		{
			Driver_.Init( Size, Driver, Callback, fdr::ts_Default );
			sDressedIFlow_::Init( Driver_ );
		}
		void Init(
			fdr::sSize Size,
			fdr::rIDriver &Driver,
			cSizeDelimitedOFlow &Callback )
		{
			Init( Size, Driver, &Callback );
		}
	};
}

#endif
