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

//	$Id: pip.h,v 1.14 2012/11/14 16:06:32 csimon Exp $

#ifndef PIP__INC
#define PIP__INC

#define PIP_NAME		"PIP"

#define	PIP_VERSION	"$Revision: 1.14 $"

#define PIP_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( PIP_NODBG )
#define PIP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.14 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:32 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D PIPe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "cpe.h"
#include "bso.h"
#include "mtx.h"

#if defined( CPE__UNIX ) || defined( CPE__BEOS )
#	define PIP__UNIX_LIKE
#endif

#ifndef PIP_FLOW_BUFFER_SIZE
//d Size of the buffer for the flow. 100 by default.
#define PIP_FLOW_BUFFER_SIZE	100
#endif


#ifdef CPE__MS
#	include <windows.h>
#	define PIP_INVALID_PIPE	INVALID_HANDLE_VALUE
#elif defined( PIP__UNIX_LIKE )
#	include <unistd.h>
#	define PIP_INVALID_PIPE	-1
#else
#	error "Unknow compilation enviroment."
#endif


#if defined( CPE__UNIX ) || defined( CPE__BEOS )
#	define PIP__UNIX_LIKE
#endif

#define PIP_INVALID_PIPE_HANDLER	NULL
#define PIP_INVALID_BIPIPE_HANDLER	NULL

namespace pip {
	using flw::datum__;
#ifdef CPE__MS
//t Amount of data.
	typedef DWORD amount__;
#elif defined( PIP__UNIX_LIKE )
	typedef int amount__;
#else
#	error "Unknow compilation enviroment."
#endif

	namespace {
		using mtx::mutex_handler__;
		using mtx::mutex___;
		enum
		{
			eRead = 0,
			eWrite = 1
		};
#ifdef CPE__MS
		typedef HANDLE	base_pipe_type__;
#elif defined( PIP__UNIX_LIKE )
		typedef int base_pipe_type__;
#else
		#error "Unknow compilation enviroment."
#endif
	}

	//t A pipe.
	typedef base_pipe_type__ base_pipe__[2];

	//f Return a new pipe in 'Pipe'.
	inline void CreateBasePipe( base_pipe__ &Pipe )
	{
#ifdef CPE__MS
		if ( !CreatePipe( &Pipe[pip::eRead], &Pipe[pip::eWrite], NULL, 0 ) )
#elif defined( PIP__UNIX_LIKE )
		if ( pipe( Pipe ) == -1 )
#else
	#error "Unknow compilation enviroment."
#endif
			ERRd();
	}

	//f Delete pipe 'Pipe'.
	inline void Delete( base_pipe__ &Pipe )
	{

#ifdef CPE__MS
		BOOL R1 = true, R2 = true;

		if ( Pipe[pip::eRead] != PIP_INVALID_PIPE )
			R1 = CloseHandle( Pipe[pip::eRead] );

		if ( Pipe[pip::eWrite] != PIP_INVALID_PIPE )
			R2 = CloseHandle( Pipe[pip::eWrite] );

		if ( !R1 || !R2 )
			ERRd();
#elif defined( PIP__UNIX_LIKE )
		bool R1 = false, R2 = false;

		if ( Pipe[pip::eRead] != PIP_INVALID_PIPE )
			R1 = close( Pipe[pip::eRead] );

		if ( Pipe[pip::eWrite] != PIP_INVALID_PIPE )
			R2 = close( Pipe[pip::eWrite] );

		if ( R1 || R2 )
			ERRd();
#else
#error "Unknow compilation enviroment."
#endif
		Pipe[pip::eRead] = Pipe[pip::eWrite] = PIP_INVALID_PIPE;
	}

	//f Write 'Amount' data from 'Buffer' into 'Pipe'.
	inline pip::amount__ Write(
		base_pipe__ &Pipe,
		const datum__ *Buffer,
		amount__ Amount )
	{
	#ifdef CPE__MS
		DWORD NombreEcrits;
	#elif defined( PIP__UNIX_LIKE )
		ssize_t NombreEcrits;
	#else
	#error "Unknow compilation enviroment."
	#endif

	#ifdef CPE__MS
		if ( !WriteFile( Pipe[pip::eWrite], Buffer, Amount, &NombreEcrits, NULL ) )
	#elif defined( PIP__UNIX_LIKE )
		if ( ( NombreEcrits = write( Pipe[pip::eWrite], Buffer, Amount ) ) < 0 )
	#else
	#error "Unknow compilation enviroment."
	#endif
			ERRd();

		return NombreEcrits;
	}

	//f Put 'Amount' data from 'Pipe' into 'Buffer'.
	inline pip::amount__ Read(
		base_pipe__ &Pipe,
		amount__ Amount,
		datum__ *Buffer )
	{
	#ifdef CPE__MS
		DWORD NombreLus;
	#elif defined( PIP__UNIX_LIKE )
		ssize_t NombreLus;
	#else
	#error "Unknow compilation enviroment."
	#endif

	#ifdef CPE__MS
		if ( !ReadFile( Pipe[pip::eRead], Buffer, Amount, &NombreLus, NULL ) )
	#elif defined( PIP__UNIX_LIKE )
			if ( ( NombreLus = read( Pipe[pip::eRead], Buffer, Amount ) ) < 0 )
	#else
	#error "Unknow compilation enviroment."
	#endif
			ERRd();

		return NombreLus;
	}

	//c A pipe.
	class pipe___
	{
	private:
		base_pipe__ Pipe_;
		amount__ Available_;
		bool Synchronization_;
		mutex_handler__ Mutex_;
		bso::bool__ WriteInProgress_, ReadInProgress_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
			
				while( WriteInProgress_ || ReadInProgress_ )
					tol::Defer();

				Delete( Pipe_ );

				if ( Mutex_ != MTX_INVALID_HANDLER )
					mtx::Delete( Mutex_ );
			}

			Pipe_[eRead] = Pipe_[eWrite] = PIP_INVALID_PIPE;
			Available_ = false;
			Synchronization_ = false;
			Mutex_ = MTX_INVALID_HANDLER;
			WriteInProgress_ = ReadInProgress_ = false;
		}
		pipe___( void )
		{
			reset( false );
		}
		~pipe___( void )
		{
			reset( true );
		}
		void Init( void )
		{
			reset();

			CreateBasePipe( Pipe_ );
			Mutex_ = mtx::Create();
		}
		void Vider( void );
		amount__ Write(
			const datum__ *Buffer,
			amount__ Amount,
			bso::bool__ Synchronization );
		//f Put up to 'Amount' bytes to 'Buffer'. Return the amount of bytes red.
		amount__ Read(
			amount__ Amount,
			datum__ *Buffer );
		//f Return true if all the data written until the last synchronization are read.
		bool IsEmpty( void )
		{
			bso::bool__ Return = false;

			if ( Synchronization_ ) {
				mtx::Lock( Mutex_ );
				Return =  !Available_;
				mtx::Unlock( Mutex_ );
			}

			return Return;
		}
		/*f Flushes the pipe. If 'Force' at true, then force the flush EVEN if the
		pipe already empty ; a write will be occurs. */
		void Flush(	bso::bool__ Force = false )
		{
			if ( !IsEmpty() || Force )
				Vider();
		}
		//f Return the amount of data currently in the pipe.
		amount__ Amount( void )
		{
			return Available_;
		}
	};

	//c A pipe input flow.
	class pipe_ioflow___
	: public flw::ioflow___
	{
	private:
		pipe___ Pipe_;
		flw::datum__ Cache_[PIP_FLOW_BUFFER_SIZE*2];
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Buffer,
			flw::amount__ Wanted );
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Buffer,
			flw::amount__ Amount,
			flw::amount__ Minimum,
			bool Synchronization );
	public:
		void reset( bool P = true )
		{
			ioflow___::reset( P );
			Pipe_.reset( P );
		}
		pipe_ioflow___( void )
		{
			reset( false );
		}
		virtual ~pipe_ioflow___( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			Pipe_.Init();
			ioflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};

	//t A bipipe;
	typedef struct { pipe___ Master, Slave; }  bipipe___;

	//c Master bipipe input/output flow.
	class master_bipipe_ioflow___
	: public flw::ioflow___
	{
	private:
		bipipe___ *Bipipe_;
		flw::datum__ Cache_[2*PIP_FLOW_BUFFER_SIZE];
	protected:
	virtual flw::amount__ FLWPut(
			const flw::datum__ *Buffer,
			flw::amount__ Amount,
			flw::amount__ Minimum,
			bool Synchronization );
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Buffer,
			flw::amount__ Wanted );
	public:
		void reset( bso::bool__ P = true )
		{
			ioflow___::reset( P );

			Bipipe_ = NULL;
		}
		master_bipipe_ioflow___( void )
		{
			reset( false );
		}
		virtual ~master_bipipe_ioflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'Bipipe'.
		void Init( bipipe___ &Bipipe )
		{
			ioflow___::Init( Cache_, sizeof( Cache_ ) );

			Bipipe_ = &Bipipe;
		}
	};

	//c Master bipipe input/output flow driver.
	class slave_bipipe_ioflow___
	: public flw::ioflow___
	{
	private:
		bipipe___ *Bipipe_;
		flw::datum__ Cache_[2*PIP_FLOW_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Buffer,
			flw::amount__ Amount,
			flw::amount__ Minimum,
			bool Synchronization );
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Buffer,
			flw::amount__ Wanted );
	public:
		void reset( bso::bool__ P = true )
		{
			ioflow___::reset( P );

			Bipipe_ = NULL;
		}
		slave_bipipe_ioflow___( void )
		{
			reset( false );
		}
		virtual ~slave_bipipe_ioflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'Bipipe'.
		void Init( bipipe___ &Bipipe )
		{
			ioflow___::Init( Cache_, sizeof( Cache_ ) );

			Bipipe_ = &Bipipe;
		}
	};

	//c Bipipe for flow.
	class bipipe_flow___
	{
	private:
		master_bipipe_ioflow___ Master_;
		slave_bipipe_ioflow___ Slave_;
		bipipe___ Bipipe_;
	public:
		void reset( bso::bool__ P = true )
		{
			Master_.reset( P );
			Slave_.reset( P );
			Bipipe_.Master.reset( P );
			Bipipe_.Slave.reset( P );
		}
		bipipe_flow___( void )
		{
			reset( false );
		}
		virtual ~bipipe_flow___( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			Bipipe_.Master.Init();
			Bipipe_.Slave.Init();

			Master_.Init( Bipipe_ );
			Slave_.Init( Bipipe_ );
		}
		//f Return slave bipipe input/output flow.
		flw::ioflow___ &Slave( void )
		{
			return Slave_;
		}
		//f Return master bipipe input/output flow.
		flw::ioflow___ &Master( void )
		{
			return Master_;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
