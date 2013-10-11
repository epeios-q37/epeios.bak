/*
	Header for the 'spp' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2002  Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: spp.h,v 1.6 2012/11/14 16:06:37 csimon Exp $

#ifndef SPP__INC
#define SPP__INC

#define SPP_NAME		"SPP"

#define	SPP_VERSION	"$Revision: 1.6 $"

#define SPP_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &SPPTutor;

#if defined( E_DEBUG ) && !defined( SPP_NODBG )
#define SPP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:37 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Shared PiPe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "pip.h"
#include "mtk.h"

#ifndef SPP_FLOW_BUFFER_SIZE
//d The size of a flow buffer.
#define SPP_FLOW_BUFFER_SIZE	PIP_FLOW_BUFFER_SIZE
#endif

namespace spp {

	using pip::datum__;

	using pip::amount__;

	//e Modus.
	enum modus {
		//i Normal modus.
		mNormal,
		//i Request modus, after a writing is always followed by a reading.
		mRequest
	};


	namespace {
		struct lock___ {
			mtx::mutex_handler__ Mutex;
			mtk::thread_id__ Owner;
			bso::bool__ Locked;
			mtx::mutex_handler__ Exclusive;
			void reset( bso::bool__ P = true )
			{
				if ( P ) {
					if ( Mutex != MTX_INVALID_HANDLER )
						mtx::Delete( Mutex );

					if ( Exclusive != MTX_INVALID_HANDLER )
						mtx::Delete( Exclusive );
				}

				Mutex = MTX_INVALID_HANDLER;
				Exclusive = MTX_INVALID_HANDLER;
				Locked = false;
			}
			lock___( void )
			{
				reset( false );
			}
			~lock___( void )
			{
				reset( true );
			}
			void Init( void )
			{
				reset();
	
				Mutex = mtx::Create();
				Exclusive = mtx::Create();
			}
		};
	}

	//A shared pipe.
	class shared_pipe___
	: public pip::pipe___
	{
	private:
		lock___ Read_, Write_;
		void Lock_( lock___ &Lock )
		{
			bso::bool__ L = false;

			mtx::Lock( Lock.Exclusive );

			if ( !Lock.Locked || ( Lock.Owner != mtk::GetTID() ) )
				L = true;

			mtx::Unlock( Lock.Exclusive );

			if ( L ) {
				mtx::Lock( Lock.Mutex );
				mtx::Lock( Lock.Exclusive );
				Lock.Owner = mtk::GetTID();
				Lock.Locked = true;
				mtx::Unlock( Lock.Exclusive );
			}
		}
		void Unlock_( lock___ &Lock )
		{
			mtx::Lock( Lock.Exclusive );

#ifdef SPP_DBG
			if ( !Lock.Locked )
				ERRc();

			if ( Lock.Owner != mtk::GetTID() )
				ERRc();
#endif

			mtx::Unlock( Lock.Mutex );

			Lock.Locked = false;

			mtx::Unlock( Lock.Exclusive );
		}

	public:
		void reset( bool P = true )
		{
			pipe___::reset( P );
			Read_.reset( P );
			Write_.reset( P );
		}
		shared_pipe___( void )
		{
			reset( false );
		}
		virtual ~shared_pipe___( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			reset();

			Read_.Init();
			Write_.Init();

			pipe___::Init();
		}
		//f Write 'Amount' bytes from 'Buffer'. If 'Synchronization' at true, a synchronization occurs.
		amount__ Write(
			const datum__ *Buffer,
			amount__ Amount,
			bso::bool__ Synchronization )
		{
			amount__ Return;

			Lock_( Write_ );

			Return = pipe___::Write( Buffer, Amount, Synchronization );

			if ( Synchronization && ( Return == Amount ) )
				Unlock_( Write_ );

			return Return;
		}
		//f Put 'Amount( bytes into 'Buffer'.
		amount__ Read(
			amount__ Amount,
			datum__ *Buffer )
		{
			amount__ Count;

			Lock_( Read_ );

			Count = pipe___::Read( Amount, Buffer );

			if ( pipe___::IsEmpty() )
				Unlock_( Read_ );

			return Count;
		}
		//f Lock the read end of the pipe.
		void LockRead( void )
		{
			Lock_( Read_ );
		}
		//f Lock the write end of the pipe.
		void LockWrite( void )
		{
			Lock_( Write_ );
		}
	};
	
	//c A shared pipe input flow.
	class shared_pipe_iflow___
	: public flw::iflow___
	{
	private:
		shared_pipe___ *Pipe_;
		flw::datum__ Cache_[SPP_FLOW_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Buffer,
			flw::amount__ Wanted );
	public:
		void reset( bool P =  true )
		{
			iflow___::reset( P );
			Pipe_ = NULL;
		}
		shared_pipe_iflow___( void )
		{
			reset( false );
		}
		virtual ~shared_pipe_iflow___( void )
		{
			reset( true );
		}
		//f Initialization with shared pipe handler 'Handler'.
		void Init( shared_pipe___ &SharedPipe )
		{
			reset();
			
			Pipe_ = &SharedPipe;	
			iflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};
	
	//c A shared pipe output flow.
	class shared_pipe_oflow___
	: public flw::oflow___
	{
	private:
		shared_pipe___ *Pipe_;
		flw::datum__ Cache_[SPP_FLOW_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Buffer,
			flw::amount__ Amount,
			flw::amount__ Minimum,
			bool Synchronization );
	public:
		void reset( bool P =  true )
		{
			oflow___::reset( P );
			Pipe_ = NULL;
		}
		shared_pipe_oflow___( void )
		{
			reset( false );
		}
		virtual ~shared_pipe_oflow___( void )
		{
			reset( true );
		}
		//f Initialization with shared pipe handler 'Handler'.
		void Init( shared_pipe___ &SharedPipe )
		{
			reset();
			
			Pipe_ = &SharedPipe;
			oflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};

	//c A shared bipipe.
	class shared_bipipe___
	{
	private:
		shared_pipe___ Master_, Slave_;
	public:
		void reset( bool P = true )
		{
			Master_.reset( P );
			Slave_.reset( P );
		}
		shared_bipipe___( void )
		{
			reset( false );
		}
		virtual ~shared_bipipe___( void )
		{
			reset( true );
		}
		//f Initialmization.
		void Init( void )
		{
			reset();
			
			Master_.Init();
			Slave_.Init();
		}
		//f Return the handler of the master shared pipe.
		shared_pipe___ &GetMaster( void )
		{
			return Master_;
		}
		//f Return the handler of the slave shared pipe.
		shared_pipe___ &GetSlave( void )
		{
			return Slave_;
		}
	};
			
	//c Master shared bipipe input/output flow.
	class master_shared_bipipe_ioflow___
	: public flw::ioflow___
	{
	private:
		shared_bipipe___ *Bipipe_;
		flw::datum__ Cache_[2*SPP_FLOW_BUFFER_SIZE];
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
		master_shared_bipipe_ioflow___( void )
		{
			reset( false );
		}
		~master_shared_bipipe_ioflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'SharedBipipe'
		void Init( shared_bipipe___ &SharedBipipe )
		{
			reset();
			
			Bipipe_ = &SharedBipipe;
			ioflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};
	
	//c Slave shared input/output pipe flow.
	class slave_shared_bipipe_ioflow___
	: public flw::ioflow___
	{
	private:
		shared_bipipe___ *Bipipe_;
		flw::datum__ Cache_[2*SPP_FLOW_BUFFER_SIZE];
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
		slave_shared_bipipe_ioflow___( void )
		{
			reset( false );
		}
		~slave_shared_bipipe_ioflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'SharedBipipe'.
		void Init( shared_bipipe___ &SharedBipipe )
		{
			reset();
		
			Bipipe_ = &SharedBipipe;
			ioflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};

}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
