/*
	Header for the 'csdsnc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: csdsnc.h,v 1.46 2013/07/26 10:45:22 csimon Exp $

#ifndef CSDSNC__INC
#define CSDSNC__INC

#define CSDSNC_NAME		"CSDSNC"

#define	CSDSNC_VERSION	"$Revision: 1.46 $"

#define CSDSNC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDSNCTutor;

#if defined( E_DEBUG ) && !defined( CSDSNC_NODBG )
#define CSDSNC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.46 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/07/26 10:45:22 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Standard Network Client 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "sck.h"
#include "stk.h"
#include "csdbnc.h"
#include "csdsnb.h"
#include "bso.h"
#include "cpe.h"
#include "ags.h"

#ifdef CPE_MT
#	define CSDSNC__MT
#endif

#ifdef CSDSNC__MT
#	include "mtx.h"
#endif

#define CSDSNC_PING_RESOLUTION	( 2 * 1000 )	// Résolution du délai d'inactivité donnée pour les 'ping's.

#define CSDSNC_DEFAULT_CACHE_SIZE	100

namespace csdsnc {
	using namespace csdsnb;
#ifdef CSDSNC__MT
	typedef mtx::mutex_handler__	mutex__;
#	define CSDSNC_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define CSDSNC_NO_MUTEX			NULL
#endif

	inline void _Lock( mutex__ Mutex )
	{
#ifdef CSDSNC__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void _Unlock( mutex__ Mutex )
	{
#ifdef CSDSNC__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void _Delete(
		mutex__ Mutex,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef CSDSNC__MT
		mtx::Delete( Mutex, EvenIfLocked );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef CSDSNC__MT
		return mtx::Create();
#else
		return CSDSNC_NO_MUTEX;
#endif
	}

	typedef sck::socket_ioflow___ _flow___;

	typedef stk::E_BSTACK_( _flow___ * )	flows_;

	enum log__ {
		lCreation,
		lRetrieval,
		lRelease,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( log__ Log );

	class log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			log__ Log,
			const void *Flow,
			sdr::size__ Amount ) = 0;
	public:
		void Log(
			log__ Log,
			const void *Flow,
			sdr::size__ Amount )
		{
			CSDSNCLog( Log ,Flow, Amount );
		}
	};

	class core_ {
	private:
		void _DeleteFlows( void );
		void _KeepAlive( time_t Delay );	// Lance le thread permettant de 'pinger' les connections inactives durant plus de 'Delay' ms.
		void _Log(
			log__ Log,
			const _flow___ *Flow )
		{
			if ( S_.Log.Functions != NULL ) {
ERRProlog
ERRBegin
				_Lock( S_.Log.Mutex );
				S_.Log.Functions->Log( Log, Flow, Flows.Amount() );
ERRErr
ERREnd
				_Unlock( S_.Log.Mutex );
ERREpilog
			}
		}
	public:
		struct s {
			char *HostService;
			mutex__ MainMutex;
			struct log__ {
				log_functions__ *Functions;
				mutex__ Mutex;
			} Log;
			struct ping__ {
				bso::uint__ Delay;	// Délai maximum d'inactivité.
				mutex__ Mutex;	// Pour signaler et attendre la terminaison du 'thread' dédié au 'ping'.
			} Ping;
			flows_::s Flows;
		} &S_;
		flows_ Flows;
		core_( s &S )
		: S_( S ),
		  Flows( S.Flows )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( S_.Ping.Mutex != CSDSNC_NO_MUTEX )
					_Lock( S_.Ping.Mutex );	// Signale au 'thread' du 'ping' qu'il doit se terminer.

				_DeleteFlows();

				if ( S_.HostService != NULL )
					free( S_.HostService );

				if ( S_.Ping.Mutex != CSDSNC_NO_MUTEX ) {
					_Lock( S_.Ping.Mutex );	// Attend que le 'thread' ud 'ping' prenne acte de la demnade de terminaison.
					_Delete( S_.Ping.Mutex, true );
				}

				if ( S_.MainMutex != CSDSNC_NO_MUTEX )
					_Delete( S_.MainMutex );

				if ( S_.Log.Mutex != CSDSNC_NO_MUTEX )
					_Delete( S_.Log.Mutex );
			}

			Flows.reset( P );

			S_.HostService = NULL;
			S_.MainMutex = CSDSNC_NO_MUTEX;
			S_.Log.Mutex = CSDSNC_NO_MUTEX;
			S_.Log.Functions = NULL;
			S_.Ping.Delay = 0;
			S_.Ping.Mutex = CSDSNC_NO_MUTEX;
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Flows.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Flows.plug( AS );
		}
		core_ &operator =( const core_ &C )
		{
			ERRFbd();

			return *this;
		}
		bso::bool__ Init( 
			const char *HostService,
			log_functions__ &LogFunctions,
			bso::uint__ PingDelay )
		{
			reset();

			if ( ( S_.HostService = (char *)malloc( strlen( HostService ) + 1 ) ) == NULL )
				ERRAlc();

			strcpy( S_.HostService, HostService );

			S_.MainMutex = _Create();
			S_.Log.Mutex = _Create();
			S_.Log.Functions = &LogFunctions;

			Flows.Init();

			_flow___ *Flow = Get( err::hUserDefined );

			if ( Flow == NULL )
				return false;

			Release( Flow );

			S_.Ping.Delay = PingDelay;

			if ( PingDelay != 0 ) {
				S_.Ping.Mutex = _Create();
				_KeepAlive( PingDelay );
			}

			return true;
		}
		_flow___ *Get( err::handling__ ErrorHandling = err::h_Default )
		{
			_flow___ *Flow = NULL;
		ERRProlog
			log__ Log = l_Undefined;
			bso::bool__ Locked = false;
			sck::socket__ Socket = SCK_INVALID_SOCKET;
		ERRBegin
			_Lock( S_.MainMutex );
			Locked = true;

			if ( Flows.Amount() ) {
				Flow = Flows.Pop();
				Log = lRetrieval;
			} else {
				Flow = new _flow___;

				if ( Flow == NULL )
					ERRAlc();

				if ( ( Socket = csdbnc::Connect( S_.HostService, ErrorHandling ) ) == SCK_INVALID_SOCKET ) {
					delete Flow;
					Flow = NULL;
					ERRReturn;
				}

				Flow->Init( Socket );

				Log = lCreation;
			}

			_Unlock( S_.MainMutex );
			Locked = false;

			_Log( Log, Flow );
		ERRErr
			if ( Flow != NULL ) {
				delete Flow;
				Flow = NULL;
			}
		ERREnd
			if ( Locked )
				_Unlock( S_.MainMutex );
		ERREpilog

			return Flow;
		}
		void Release( _flow___ *Flow )
		{
			_Lock( S_.MainMutex );

			Flows.Push( Flow );

			_Unlock( S_.MainMutex );

			_Log( lRelease, Flow );
		}
		void Ping( void );	// Emet un 'ping' sur les connections restée inactives trop longtemps.
	};

	E_AUTO( core );

	class _driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		_flow___ *_Flow;
		core_ *_Core;
		id__ _Id;
		bso::bool__ _Prepare( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = _Flow == NULL;

			if ( Created ) {
				_Flow = _Core->Get();

				PutId( _Id, *_Flow );
			}

			return !Created;
		}
		void _Commit( void )
		{
#ifdef CSDSNC_DBG
			if ( _Flow == NULL )
				ERRFwk();
#endif

			_Flow->Commit();

			if ( _Id == CSDSNB_UNDEFINED )
				_Id = GetId( *_Flow );
			else if ( _Flow->Get() != 0 )
				ERRDta();
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ Amount = 0;

			_Prepare();

			return _Flow->WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			if ( _Flow != NULL )
				_Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _Flow->ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			if ( _Flow != NULL ) {
				_Core->Release( _Flow );
				_Flow->Dismiss();
			}

			_Flow = NULL;
		}
		public:
			void reset( bso::bool__ P = true )
			{
				fdr::ioflow_driver___<>::reset( P );

				if ( P ) {
					if ( _Flow != NULL )
						delete _Flow;
				}

				_Flow = NULL;
				_Id = CSDSNB_UNDEFINED;
				_Core = NULL;
			}
			_driver___( void )
			{
				reset( false );
			}
			~_driver___( void )
			{
				reset();
			}
			void Init(
				core_ &Core,
				fdr::thread_safety__ ThreadSafety )
			{
				reset();

				fdr::ioflow_driver___<>::Init( ThreadSafety );
				_Core = &Core;
			}
	};

	class client_ioflow___
	: public flw::ioflow__
	{
	private:
		_driver___ _Driver;
		flw::datum__ _Cache[CSDSNC_DEFAULT_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			flw::ioflow__::reset( P );
			_Driver.reset( P );
		}
		client_ioflow___( void )
		{
			reset( false );
		}
		void Init( core_ &Core )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), FLW_AMOUNT_MAX );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
