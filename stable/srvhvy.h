/*
	Header for the 'srvhvy' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: srvhvy.h,v 1.10 2012/11/14 16:06:37 csimon Exp $

#ifndef SRVHVY__INC
#define SRVHVY__INC

#define SRVHVY_NAME		"SRVHVY"

#define	SRVHVY_VERSION	"$Revision: 1.10 $"

#define SRVHVY_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( SRVHVY_NODBG )
#define SRVHVY_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:37 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SeRCer fo HeaVY operations. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete. Use 'CSDSNS' library instead."

#include "err.h"
#include "flw.h"
#include "srv.h"
#include "lstbch.h"
#include "mtx.h"

namespace srvhvy {

	using srv::action__;
	using srv::aContinue;
	using srv::aStop;
	using srv::service__;

	enum log__ {
		lNew,
		lStore,
		lTestAndGet,
		lDelete,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( log__ Log );

	typedef bso::ushort__ id__;

#define SRVHVY_UNDEFINED	BSO_USHORT_MAX

	typedef void *_user_pointer__;

	class log_functions__ {
	protected:
		virtual void SRVHVYLog(
			log__ Log,
			id__ Id,
			void *UP,
			epeios::size__ Amount ) = 0;
	public:
		void Log(
			log__ Log,
			id__ Id,
			_user_pointer__ UP,
			epeios::size__ Amount )
		{
			SRVHVYLog( Log, Id, UP, Amount );
		}
	};

	typedef lstbch::E_LBUNCH_( _user_pointer__ ) user_pointers_;

	class core_
	{
	private:
		bso::bool__ _Exists( id__ Id ) const
		{
			return UPs.Exists( Id );
		}
		void _Log(
			log__ Log,
			id__ Id,
			void *UP ) const
		{
			if ( S_.Log.Functions != NULL ) {
ERRProlog
ERRBegin
				mtx::Lock( S_.Log.Mutex );
				S_.Log.Functions->Log( Log, Id, UP, UPs.Amount() );
ERRErr
ERREnd
				mtx::Unlock( S_.Log.Mutex );
ERREpilog
			}
		}
	public:
		struct s
		{
			user_pointers_::s UPs;
			mtx::mutex_handler__ Mutex;
			struct log__ {
				log_functions__ *Functions;
				mtx::mutex_handler__ Mutex;
			} Log;
		} &S_;
		user_pointers_ UPs;
		core_ ( s &S )
		: S_( S ),
		  UPs( S.UPs )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( S_.Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( S_.Mutex );
			}

			UPs.reset( P );
			S_.Mutex = MTX_INVALID_HANDLER;
			S_.Log.Mutex = MTX_INVALID_HANDLER;
			S_.Log.Functions = NULL;

		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			UPs.plug( MM );
		}
		core_ &operator =( const core_ &C )
		{
			ERRu();

			return *this;
		}
		void Init( log_functions__ &LogFunctions )
		{
			reset();

			UPs.Init();
			S_.Mutex = mtx::Create();
			S_.Log.Mutex = mtx::Create();
			S_.Log.Functions = &LogFunctions;
		}
		id__ New( void )
		{
			mtx::Lock( S_.Mutex );

			epeios::row__ Row = UPs.New();

			if ( *Row >= BSO_USHORT_MAX )
				ERRl();

			mtx::Unlock( S_.Mutex );

			_Log( lNew, (id__)*Row, NULL );

			return (id__)*Row;
		}
		void Store(
			_user_pointer__ UP,
			id__ Id )
		{
#ifdef SRVHVY_DBG
			if ( Id == SRVHVY_UNDEFINED )
				ERRu();
#endif
			mtx::Lock( S_.Mutex );
			UPs.Store( UP, Id );
			mtx::Unlock( S_.Mutex );

			_Log( lStore, Id, UP );
		}
		bso::bool__ Exists( id__ Id ) const
		{
			bso::bool__ Result;

			mtx::Lock( S_.Mutex );

			Result =  _Exists( Id );

			mtx::Unlock( S_.Mutex );

			return Result;
		}
		bso::bool__ TestAndGet(
			id__ Id,
			void *&UP ) const
		{
			bso::bool__ Result = false;

			mtx::Lock( S_.Mutex );

			Result = _Exists( Id );

			if ( Result )
				UP = UPs( Id );

			mtx::Unlock( S_.Mutex );

			_Log( lTestAndGet, Id, UP );

			return Result;
		}
		bso::bool__ Delete( id__ Id )
		{
			bso::bool__ Result = false;

			mtx::Lock( S_.Mutex );

			Result = _Exists( Id );

			if ( Result )
				UPs.Delete( Id );

			mtx::Unlock( S_.Mutex );

			_Log( lDelete, Id, NULL );

			return Result;
		}

	};

	E_AUTO( core)


	class user_functions__
	{
	protected:
		virtual void *SRVHVYPreProcess( flw::ioflow__ &Flow ) = 0;
		virtual action__ SRVHVYProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void SRVHVYPostProcess( void *UP ) = 0;
	public:
		void *PreProcess( flw::ioflow__ &Flow )
		{
			return SRVHVYPreProcess( Flow );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return SRVHVYProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			SRVHVYPostProcess( UP );
		}
	};

	class server___
	{
	private:
		srv::server___ _Server;
	public:
		void Init( void )
		{}
		void Process(
			service__ Service,
			user_functions__ &Functiosn,
			core_ &Core );
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
