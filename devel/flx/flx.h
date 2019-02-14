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
# include "lck.h"

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
	typedef sStringIFlow sStringRFlow;

	template < typename bunch_, typename so__> class bunch_oflow___;
	typedef bunch_oflow___<str::string_, bso::char__> rStringOFlow;
	typedef rStringOFlow rStringWFlow;

	class string_text_oflow___;
	typedef string_text_oflow___ rStringTOFlow;
	typedef rStringTOFlow rStringTWFlow;

	class string_text_iflow___;
	typedef string_text_iflow___ rStringTIFlow;
	typedef rStringTIFlow rStringTIFlow;

	class string_text_oflow___;
	typedef string_text_oflow___ rStringTOFlow;
	typedef rStringTOFlow rStringTWFlow;

	using flw::Copy;
}

/***************/
/***** OLD *****/
/***************/


namespace flx {

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> E_TTCLONE__( fdr::iflow_driver___<cache_size>, _idriver___ );

	//c Buffer as a standard input flow.
	class buffer_iflow_driver___
	: public _idriver___<>
	{
	private:
		// Pointeur sur le prochain caractre  lire.
		const fdr::byte__ *Tampon_;
		// Nombre de caractre pouvant encore tre lus.
		bso::size__ Taille_;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
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
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bool P = true )
		{
			_idriver___<>::reset( P );
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

			_idriver___<>::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
		void Init(
			const fdr::byte__ *Buffer,
			bso::size__ Size,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			return Init( Buffer, ThreadSafety, Size );
		}
	};

	//c Buffer as a standard input flow.
	class buffer_iflow___
	: public flw::standalone_iflow__<>
	{
	private:
		buffer_iflow_driver___ _Driver;
	public:
		void reset( bool P = true )
		{
			_Driver.reset( P );
			standalone_iflow__<>::reset( P );
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
			standalone_iflow__<>::Init( _Driver );
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
			fdr::size__ Maximum ) override
		{
			if ( Maximum > Taille_ )
				Maximum = Taille_;

			memcpy( Tampon_, Buffer, Maximum );

			Tampon_ += Maximum;
			Taille_ -= Maximum;

			return Maximum;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
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
		void Init(
			fdr::byte__ *Buffer,
			bso::size__ Size,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			return Init( Buffer, ThreadSafety, Size );
		}
	};

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow___
	: public flw::oflow__
	{
	private:
		buffer_oflow_driver___ _Driver;
		// The cache.
		flw::byte__ _Cache[FLX_BUFFER_BUFFER_SIZE];
	public:
		void reset( bool P = true )
		{
			oflow__::reset( P );
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
	: public _idriver___<cache_size>
	{ 
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
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
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	private:
		const bunch_ *Bunch_;
		sdr::row_t__ Position_;
	public:
		void reset( bool P = true )
		{
			_idriver___<cache_size>::reset( P );
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
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default,
			sdr::row_t__ Position = 0 )
		{
			reset();

			Bunch_ = &Bunch;
			Position_ = Position;

			_idriver___<cache_size>::Init( ThreadSafety );
		}
		void Init(
			const bunch_ &Bunch,
			sdr::row_t__ Position,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			return Init( Bunch, ThreadSafety, Position );
		}

	};

# define E_STRING_IFLOW_DRIVER___	bunch_iflow_driver___<str::string_, bso::char__>

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__,int  CacheSize> class bunch_iflow__
	: public flw::standalone_iflow__<>
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
			standalone_iflow__<>::reset( P );
			_Driver.reset( P );
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init(
			const bunch_ &Bunch,
			sdr::row_t__ Position = 0 )
		{
			reset();

			_Driver.Init( Bunch, fdr::tsDisabled, Position );
			standalone_iflow__<>::Init( _Driver );
		}
		flw::rRFlow &operator *( void )
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
			fdr::size__ Maximum ) override
		{
			_Bunch->Append( (const so__ *)Buffer, Maximum );

			return Maximum;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
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
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			reset();

			_Bunch = &Bunch;

			_oflow_driver___::Init( ThreadSafety );
		}
	};

# define E_STRING_OFLOW_DRIVER___	bunch_oflow_driver___<str::string_, bso::char__>


	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow___
	: public flw::standalone_oflow__<>
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
			standalone_oflow__<>::reset( P );
			_Driver.reset( P );
		}
		//f Initializing with the buffer bunch 'Bunch'.
		void Init( bunch_ &Bunch )
		{
			reset();

			_Driver.Init( Bunch, fdr::tsDisabled );
			standalone_oflow__<>::Init( _Driver );
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

	// For the 'void' flow/driver.
	qENUM( Access )
	{
		aAllowed,	// Writing/reading allowed.
		aForbidden,	// Writing/reading forbidden. Useful to use the flow/driver as placeholder, which should not be used.
		a_amount,
		a_Undefined,
		a_Default = aForbidden,
	};

	inline void Test_( eAccess Access )
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

	typedef fdr::oflow_driver___<> _odriver___;

	// 'driver' qui n'crit dans rien.
	class void_odriver___
	: public _oflow_driver___
	{
	private:
		eAccess _Access;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *,
			fdr::size__ Maximum ) override
		{
			Test_( _Access );

			return Maximum;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
			_Access = a_Undefined;
		}
		E_CDTOR( void_odriver___ );
		void Init(
			fdr::thread_safety__ ThreadSafety,
			eAccess Access )
		{
			_oflow_driver___::Init( ThreadSafety );
			_Access = Access;
		}
		void Init(
			eAccess Access,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			Init( ThreadSafety, Access );
		}
	};

	// All writing will always succeed.
	extern void_odriver___ VoidODriver;

	// 'flow' qui n'crit dans rien.
	class void_oflow__
	: public flw::standalone_oflow__<>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			standalone_oflow__<>::reset( P );
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
			standalone_oflow__<>::Init( VoidODriver );
		}
	};

	// All writing will always succeed.
	extern void_oflow__ VoidOFlow;
	// As placeholder : a writing attempt will throw an error.
	extern void_odriver___ PlaceholderODriver;
	extern void_oflow__ PlaceholderOFlow;

	// 'driver' qui ne lit rien.
	class void_idriver___
	: public _idriver___<>
	{
	private:
		eAccess _Access;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			Test_( _Access );

			return 0;
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_idriver___<>::reset( P );
			_Access = a_Undefined;
		}
		E_CDTOR( void_idriver___ );
		void Init(
			fdr::thread_safety__ ThreadSafety,
			eAccess Access )
		{
			fdr::iflow_driver___<>::Init( ThreadSafety );
			_Access = Access;
		}
		void Init(
			eAccess Access,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			return Init( ThreadSafety, Access );
		}
	};

	// Each reading attempt will return EOF.
	extern void_idriver___ VoidIDriver;

	// 'flow' qui ne lit rien.
	class void_iflow__
	: public flw::standalone_iflow__<>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			standalone_iflow__<>::reset( P );
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
			standalone_iflow__<>::Init( VoidIDriver );
		}
	};

	// Each reading attempt will return EOF.
	extern void_iflow__ VoidIFlow;
	// As placeholder : read attempt will throw an error.
	extern void_idriver___ PlaceholderIDriver;
	extern void_iflow__ PlaceholderIFlow;

# if 0	// Obsolete ? Seems to be useless.
	// 'driver' qui relaye un autre 'driver'.
	class relay_oflow_driver___
	: public _oflow_driver___
	{
	private:
		fdr::oflow_driver_base___ *_Driver;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			if ( _Driver == NULL )
				qRFwk();

			return _Driver->Write( Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			if ( _Driver == NULL )
				qRFwk();

			return _Driver->Commit( Unlock );
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
	: public _idriver___<>
	{
	private:
		fdr::iflow_driver_base___ *_Driver;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			if ( _Driver == NULL )
				qRFwk();

			return _Driver->Read( Maximum, Buffer, fdr::bNonBlocking );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			if ( _Driver == NULL )
				qRFwk();

			_Driver->Dismiss( Unlock );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_idriver___<>::reset( P );
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
			_idriver___<>::Init( ThreadSafety );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Driver != NULL;
		}
	};
#endif

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
			fdr::size__ Maximum ) override
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
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( !_PendingCommit )	// Pour viter qu'un 'commit( suite  un 'reset()' recrive un '0'.
				return true;

			if ( _Flow == NULL )
				qRFwk();

			if ( _EmbeddedSizeRemainder == 0 )
				dtfptb::VPut( (bso::u8__)0, *_Flow );

			_PendingCommit = false;

			if ( CommitHandling_ == chPropagate )
				return _Flow->Commit( Unlock, ErrHandling );
			else
				return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return _Flow->ODriver().WTake( Owner );
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
		void Init(
			flw::oflow__ &Flow,
			eCommitHandling CommitHandling,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			return Init( Flow, ThreadSafety, CommitHandling );
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
	: public _idriver___<>
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
			fdr::byte__ *Buffer ) override
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
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( _Flow == NULL )
				qRFwk();

			if ( !_AllRed )
				_Purge();

			_AllRed = true;

			if ( _DismissHandling == dhPropagate )
				return _Flow->Dismiss( Unlock, ErrHandling );
			else
				return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return _Flow->IDriver().RTake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_idriver___<>::reset( P );
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
			_idriver___<>::Init( ThreadSafety );
			_AllRed = true;
		}
		void Init(
			flw::iflow__ &Flow,
			dismiss_handling__ DismissHandling,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			return Init( Flow, ThreadSafety, DismissHandling );
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

	typedef _size_embbeded_flow___<flw::iflow__, flw::standalone_iflow__<>, size_embedded_iflow_driver___, dismiss_handling__> size_embedded_iflow___;
	typedef _size_embbeded_flow___<flw::oflow__, flw::standalone_oflow__<>, size_embedded_oflow_driver___, eCommitHandling> size_embedded_oflow___;


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
			fdr::size__ Maximum ) override
		{
			mtx::Lock( _Write );

			_Buffer = Buffer;
			_Size = Maximum;
			_Red = 0;

			mtx::Unlock( _Read );

			return _Red;
		}
		virtual void FDRCommit( void ) override
		{}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{

			mtx::Lock( _Read );

			if ( Maximum > _Size )
				Maximum = _Size;

			memcpy( Buffer, _Buffer, Maximum );

			_Red = Maximum;

			mtx::Unlock( _Write );

			return Maximum;
		}
		virtual void FDRDismiss( void ) override
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

	extern const char
		*_ReadMode,
		*_WriteMode;

	// Lance une commande dans le shell et rcupre les donnes crites par la commande.
	class exec_iflow_driver___
	: public _idriver___<>,
	  public _exec_driver___<cslio::standard_input__>
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			if ( IO().OnEOF() )
				return 0;

			Maximum = IO().Read( Maximum, Buffer );

			if ( Maximum == 0 )
				if ( !IO().OnEOF() )
					qRFwk();

			return Maximum;

		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_idriver___::reset( P );
			_exec_driver___::reset( P );
				
		}
		E_CVDTOR( exec_iflow_driver___ );
		bso::bool__ Init(
			const ntvstr::string___ &Command,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			_idriver___<>::Init( ThreadSafety );
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
			fdr::size__ Maximum ) override
		{
			return IO().Write( Buffer, Maximum );
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			IO().Flush();

			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
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
			fdr::size__ Maximum ) override;
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( _In != UndefinedPipeDescriptor )
				Close_( _In );

			_In = UndefinedPipeDescriptor;

			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
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
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default );
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
	class cSizeDelimitedWFlow
	{
	protected:
		virtual void FLXOnEOF( void ) = 0;
	public:
		void OnEOF( void )
		{
			return FLXOnEOF();
		}
		qCALLBACK( SizeDelimitedWFlow );
	};

	class rSizeDelimitedIDriver
	: public _idriver___<>
	{
	private:
		fdr::sSize Size_;
		flw::rDressedRFlow<> Flow_;
		cSizeDelimitedWFlow *Callback_;
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
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( Size_ != 0 )
				qRFwk();

			return Flow_.Dismiss( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Flow_.IDriver().RTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			_idriver___::reset( P );

			tol::reset( P, Flow_, Callback_ );

			Size_ = 0;
		}
		qCVDTOR( rSizeDelimitedIDriver );
		void Init(
			fdr::sSize Size,
			fdr::rRDriver &Driver,
			cSizeDelimitedWFlow *Callback,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			_idriver___::Init( ThreadSafety );

			Size_ = Size;
			Flow_.Init( Driver );
			Callback_ = Callback;
		}
	};

	typedef flw::rDressedRFlow<> rDressedRFlow_;

	class rSizeDelimitedWFlow
	: public rDressedRFlow_
	{
	private:
		rSizeDelimitedIDriver Driver_;
	public:
		void reset( bso::sBool P = true )
		{
			rDressedRFlow_::reset( P );
			tol::reset( P, Driver_ );
		}
		qCDTOR( rSizeDelimitedWFlow );
		void Init(
			fdr::sSize Size,
			fdr::rRDriver &Driver,
			cSizeDelimitedWFlow *Callback = NULL )
		{
			Driver_.Init( Size, Driver, Callback, fdr::ts_Default );
			rDressedRFlow_::Init( Driver_ );
		}
		void Init(
			fdr::sSize Size,
			fdr::rRDriver &Driver,
			cSizeDelimitedWFlow &Callback )
		{
			Init( Size, Driver, &Callback );
		}
	};

	typedef flx::bunch_iflow_driver___<str::string_, bso::char__, FDR__DEFAULT_CACHE_SIZE> rStringIDriver;
	typedef rStringIDriver rStringRDriver;

	typedef flx::bunch_oflow_driver___<str::string_, bso::char__> rStringODriver;
	typedef rStringODriver rStringWDriver;

	// Put the content of 'Driver' into 'String' until EOF.
	const str::dString &GetString(
		fdr::rRDriver &Driver,
		str::dString &String );

	// Channel(s) to monitor.
	qENUM( Channel ) {
		cNone,
		cIn,
		cOut,
		cInAndOut,
		c_amount,
		c_Undefined
	};

	typedef fdr::rRWDressedDriver rRWDriver_;

	struct sMarkers {
		struct {
			const char
				*Before,
				*After;
		} In, Out;
		bso::sBool Async;	// If at 'true', writing and reading can be occur simultaneously.
		void reset( bso::sBool P = true )
		{
			tol::reset(P, In.Before, In.After, Out.Before, Out.After, Async );
		}
		qCDTOR( sMarkers );
	};

	// Write to console the input and/or output data. For text based protocol exchanges (like HTTP, IMAP, POP...).
	class rRWMonitor
	: public rRWDriver_
	{
	private:
		tht::rLocker Locker_;
		qRMV( fdr::rRDriver, ID_, IDriver_ );
		qRMV( fdr::rWDriver, OD_, ODriver_ );
		qRMV( const sMarkers, M_, Markers_ );
		qRMV( txf::sWFlow, FI_, FlowI_ );
		qRMV( txf::sWFlow, FO_, FlowO_ );
		bso::sBool Uncommited_;
		bso::sBool Undismissed_;
		bso::sBool IsIn_( void ) const
		{
			return ( FlowI_ != NULL );
		}
		bso::sBool IsOut_( void ) const
		{
			return ( FlowO_ != NULL );
		}
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override;
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override;
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Maximum ) override;
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override;
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override;
		void Init_(
			fdr::rRDriver *RDriver,
			fdr::rWDriver *WDriver,
			const sMarkers &Markers,	// NOT duplicated !
			txf::sWFlow *FlowI,
			txf::sWFlow *FlowO )	// Two different flow, in case one instance is share between two differtent threads.
		{
			reset();

			Locker_.Init();

			Uncommited_ = false;
			Undismissed_ = false;
			rRWDriver_::Init( fdr::ts_Default );
			IDriver_ = RDriver;
			ODriver_ = WDriver;
			Markers_ = &Markers;
			FlowI_ = FlowI;
			FlowO_ = FlowO;
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Undismissed_ ) {
					if ( M_().Out.After != NULL )
						FO_() << M_().Out.After << txf::commit;
				}
			}

			tol::reset( P, Locker_, IDriver_, ODriver_, Markers_, FlowI_, FlowO_, Uncommited_, Undismissed_ );
			rRWDriver_::reset( P );
		}
		qCVDTOR( rRWMonitor );
		void Init(
			fdr::rRDriver &IDriver,
			fdr::rWDriver &ODriver,
			const sMarkers &Markers,	// NOT duplicated !
			txf::sWFlow &FlowI,
			txf::sWFlow &FlowO )	// Two different flow, in case one instance is share between two differtent threads.
		{
			Init_( &IDriver, &ODriver, Markers, &FlowI, &FlowO );
		}
		void Init(
			fdr::rRWDriver &Driver,
			const sMarkers &Markers,	// NOT duplicated !
			txf::sWFlow &FlowI,
			txf::sWFlow &FlowO )	// Two different flow, in case one instance is share between two differtent threads.
		{
			Init_( &Driver, &Driver, Markers, &FlowI, &FlowO );
		}
		void Init(
			fdr::rRDriver &Driver,
			const sMarkers &Markers,	// NOT duplicated !
			txf::sWFlow &Flow )
		{
			Init_( &Driver, NULL, Markers, &Flow, NULL );
		}
		void Init(
			fdr::rWDriver &Driver,
			const sMarkers &Markers,	// NOT duplicated !
			txf::sWFlow &Flow )
		{
			Init_( NULL, &Driver, Markers, NULL, &Flow );
		}
		void Init(
			fdr::rRWDriver &Driver,
			eChannel Channel,
			const sMarkers &Markers,	// NOT duplicated !
			txf::sWFlow &Flow )
		{
			switch ( Channel ) {
			case cNone:
				Init_( &Driver, &Driver, Markers, NULL, NULL );
				break;
			case cIn:
				Init_( &Driver, &Driver, Markers, &Flow, NULL );
				break;
			case cOut:
				Init_( &Driver, &Driver, Markers, NULL, &Flow );
				break;
			case cInAndOut:
				Init_( &Driver, &Driver, Markers, &Flow, &Flow );
				break;
			default:
				qRFwk();
				break;
			}
		}
	};

	// Relay classes relays data from�an output driver to an input one.
	// The core. Thread-safe.
	class rRelay_
	{
	private:
		tht::rLocker Locker_;
		tht::rBlocker Blocker_;
		fdr::sByte *Buffer_;
		fdr::sSize
			Size_,	// Size of 'Buffer_'. If == 0, the no more input data is available.
			Head_,	// Position of the first byte of data.
			Amount_;// Amount of available data.
		// Moves the data to the beginning of the buffer.
		void Normalize_( void )
		{
			if ( Head_ != 0 ) {
				if ( Amount_ != 0 )
					memmove( Buffer_, Buffer_+ Head_, Amount_ );
				Head_ = 0;
			}
		}
		fdr::sSize Read_(
			fdr::sByte *Buffer,
			fdr::sSize Amount )
		{
			if ( Amount > Amount_ )
				Amount = Amount_;

			if ( Amount == 0 )
				qRFwk();

			memcpy( Buffer, Buffer_ + Head_, Amount );

			Amount_ -= Amount;
			Head_ += Amount;

			return Amount;
		}
	protected:
		// Requires to get a new buffer of current size 'OldSize' to new size 'NewSize'.
		// Returns the size of the new buffer, which must, at least, be 'OldSize' (untouch buffer).
		virtual fdr::sSize FLXReallocate(
			fdr::sByte *&Buffer,
			fdr::sSize OldSize,
			fdr::sSize NewSize ) = 0;
		bso::sBool EOFReached_( void )
		{
			// If 'Size' == 0 and 'Buffer' == NULL, then the first buffer alllocation of a dynamic relay was not yet called.
			return ( Size_ == 0 ) && ( Buffer_ != NULL );
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Locker_, Blocker_, Buffer_ );

			Size_ = Head_ = Amount_ = 0;
		}
		qCDTOR( rRelay_ );
		void Init(
			fdr::sByte *Buffer,
			fdr::sSize Size )
		{
			Locker_.Init();
			Blocker_.Init( true );

			Buffer_ = Buffer;

			Size_ = Size;

			Head_ = Amount_ = 0;
		}
		fdr::sSize Write(
			const fdr::sByte *Buffer,
			fdr::sSize Amount )	// If 'Amount' == 0, this means that no more data will be written.
		{
		qRH
			tht::rLockerHandler Locker;
		qRB
			Locker.Init( Locker_ );

			if ( EOFReached_() ) // Should not be called twice to indicate that there will be no more data.
				qRFwk();

			if ( Amount_ == 0 )	// If no data available, 'Read(...)' will block until some available.
				Blocker_.Unblock();	// Will unblock 'Read(...)', which will then be blocked by 'Locker_'.

			if ( Amount != 0 ) {
				if ( Amount > ( Size_ - Amount_ ) ) {
					if ( Amount_ == Size_ )
						Size_ = FLXReallocate( Buffer_, Size_, Amount + Size_ );
					Amount = Size_ - Amount_;
				}

				Normalize_();

				if ( Amount != 0 ) {
					memcpy( Buffer_ + Amount_, Buffer, Amount );
					Amount_ += Amount;
				}
			} else
				Size_ = 0;
		qRR
		qRT
		qRE
			return Amount;
		}
		// If it returns 0, there would no more data (EOF). Otherwise, will block until some data available.
		fdr::sSize Read(
			fdr::sByte *Buffer,
			fdr::sSize Amount )
		{
		qRH
			tht::rLockerHandler Locker;
		qRB
			Locker.Init( Locker_ );

			if ( Amount_ != 0 ) {
				Amount = Read_( Buffer, Amount );
			} else if ( !EOFReached_() ) {
				Locker_.Unlock();
				Blocker_.Wait();
				Locker_.Lock();

				if ( Amount_ != 0 )	
					Amount = Read_( Buffer, Amount );
				else // There will be no more data.
					Amount = 0;
			} else
				Amount = 0;
		qRR
		qRT
		qRE
			return Amount;
		}
		bso::sBool IsCompleted( void )
		{
			bso::sBool Completed = false;
		qRH
			tht::rLockerHandler Locker;
		qRB
			Locker.Init( Locker_ );

			Completed = ( Size_ == 0 ) && ( Amount_ == 0 );
		qRR
		qRT
		qRE
			return Completed;
		} 
	};

	typedef fdr::rRDressedDriver rIDriver_;

	class sIRelay
	: public rIDriver_
	{
	private:
		qRMV( rRelay_, C_, Core_ );
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Wanted,
			fdr::sByte *Buffer ) override
		{
			return C_().Read( Buffer, Wanted );
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIDriver_::reset( P );
			tol::reset( P, Core_ );
		}
		qCVDTOR( sIRelay );
		void Init(
			rRelay_ &Core,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			rIDriver_::Init( ThreadSafety );
			Core_ = &Core;
		}
	};

	typedef fdr::rWDressedDriver rODriver_;

	class sORelay
	: public rODriver_
	{
	private:
		qRMV( rRelay_, C_, Core_ );
	protected:
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Wanted ) override
		{
			fdr::sSize Amount = 0;

			while ( ( Amount = C_().Write( Buffer, Wanted ) ) == 0 )
				tht::Defer();

			return Amount;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			C_().Write( NULL, 0 );

			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner )  override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rODriver_::reset( P );
			tol::reset( P, Core_ );
		}
		qCVDTOR( sORelay );
		void Init(
			rRelay_ &Core,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			rODriver_::Init( ThreadSafety );
			Core_ = &Core;
		}
	};

	// Fixed-size buffer relay.
	template <int size = 1024> class rFRelay
	: public rRelay_
	{
	private:
		fdr::sByte Buffer_[size];
	protected:
		virtual fdr::sSize FLXReallocate(
			fdr::sByte *&Buffer,
			fdr::sSize OldSize,
			fdr::sSize NewSize ) override
		{
			return OldSize;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRelay_::reset( P );
		}
		qCDTOR( rFRelay );
		void Init( void )
		{
			rRelay_::Init( Buffer_, size );
		}
	};

	// Dynamic buffer relay.
	template <int max = 64 * 1024> class rDRelay
	: public rRelay_
	{
	protected:
		virtual fdr::sSize FLXReallocate(
			fdr::sByte *&Buffer,
			fdr::sSize OldSize,
			fdr::sSize NewSize ) override
		{
			if ( ( max != 0 ) && ( NewSize > max ) )
				qRGnr();

			if ( ( Buffer = (fdr::sByte *)realloc( Buffer, NewSize ) ) == NULL )
				qRAlc();

			return NewSize;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRelay_::reset( P );
		}
		qCDTOR( rDRelay );
		void Init( void )
		{
			rRelay_::Init( NULL, 0 );
		}
	};

	// Asynchrone related class, which buffers an input driver so that a writing will never block (until too much data were written).
	class rASync_
	{
	private:
		sIRelay In_;
		sORelay Out_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, In_, Out_ );
		}
		qCDTOR( rASync_ );
		fdr::rWDriver &Init(
			rRelay_ &Relay,
			fdr::rWDriver &Driver );
	};

	// Fixed-size buffer asynchrone output.
	template <int size = 1024> class rFASync
	: public rASync_
	{
	private:
		rFRelay<size> Relay_;
	public:
		void reset( bso::sBool P = true )
		{
			rASync_::reset( P );
		}
		qCDTOR( rFASync );
		fdr::rWDriver &Init( fdr::rWDriver &Driver )
		{
			Relay_.Init();
			return rASync_::Init( Relay_, Driver );
		}
	};

	// Dynamic buffer asynchrone output.
	template <int size = 64 * 1024> class rDASync
	: public rASync_
	{
	private:
		rDRelay<size> Relay_;
	public:
		void reset( bso::sBool P = true )
		{
			rASync_::reset( P );
		}
		qCDTOR( rDASync );
		fdr::rWDriver &Init( fdr::rWDriver &Driver )
		{
			Relay_.Init();
			return rASync_::Init( Relay_, Driver );
		}
	};

	extern fdr::rRDriver &VoidRDriver;
	extern fdr::rWDriver &VoidWDriver;

	extern flw::rRFlow &VoidRFlow;
	extern flw::rWFlow &VoidWFlow;
}

#endif
