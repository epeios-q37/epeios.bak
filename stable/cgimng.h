/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#ifndef CGIMNG__INC
# define CGIMNG__INC

# define CGIMNG_NAME		"CGIMNG"

# if defined( E_DEBUG ) && !defined( CGIMNG_NODBG )
#  define CGIMNG_DBG
# endif

// CGI (Common Gateway Interface) MaNaGer

# include "csdbns.h"
# include "ssnmng.h"
# include "cgiarg.h"
# include "lck.h"
# include "csdleo.h"


namespace cgimng {

	enum log__ {
		lCreate,
		lGet,
		lGetButExpired,
		lGetButUnknown,
		lRelease,
		lClose,
		lCloseButExpired,
		lCloseButUnknown,
		lCloseButLocked,
		lCloseButInUse,
		lCleaning,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( log__ Log );

	class log_functions__
	{
	protected:
		virtual void CGIMNGLog(
			log__ Log,
			const str::string_ &SessionId ) = 0;
	public:
		void Log(
			log__ Log,
			const str::string_ &SessionId )
		{
			CGIMNGLog( Log, SessionId );
		}
	};



	//e Answer.
	enum answer__ {
		//i Undefined.
		aUndefined,
		//i All right.
		aOK,
		//i Session expired
		aExpired,
		//i Unknow session.
		aUnknow,
		//i Invalid session id.
		aInvalid,
		//i Unknow command,
		aUnknowCommand,
		//i Locked (so it's in use).
		aLocked,
		//i In use.
		aInUse,
		//i Amount of answer,
		a_amount
	};

	typedef ssnmng::sessions_manager_	sessions_;

#if 0
	class sessions_
	: public ssnmng::sessions_manager_
	{
	private:
		void _Delete(
			mdr::size__ Old,
			mdr::size__ New );
		void _Create(
			mdr::size__ Old,
			mdr::size__ New );
	protected:
		virtual void SSNMNGAllocate( mdr::size__ Size )
		{
			mdr::size__ OldSize = Mutexes.Amount();

			if ( OldSize > Size )
				_Delete( OldSize, Size );

			Mutexes.Allocate( Size );

			if ( OldSize < Size )
				_Create( OldSize, Size );
		}
	public:
		struct s
		: public ssnmng::sessions_manager_::s
		{
			mutexes_:: s Mutexes;
		};
		mutexes_ Mutexes;
		sessions_( s &S )
		: sessions_manager_( S ),
		  Mutexes( S.Mutexes )
		{}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			sessions_manager_::plug( MM );
			Mutexes.plug( MM );
		}
		void reset( bso::bool__ P = true )
		{
			sessions_manager_::reset( P );
			Mutexes.reset( P );
		}
		sessions_ &operator =( const sessions_ &S )
		{
			ERRu();

			return *this;
		}
		void Init(
			bso::ushort__ Relative,
			bso::ushort__ Absolute,
			ssnmng::user_functions__ &UserFunctions )
		{
			sessions_manager_::Init( Relative, Absolute, UserFunctions );
			Mutexes.Init();
		}

	};
#endif

	E_AUTO( sessions )

	using ssnmng::session_id__;

	//c The tool which allows to handle sessions (internal use).
	class tool___
	{
	private:
		lck::control___<sessions_> Sessions_;
		struct log_data__ {
			log_functions__ *Functions;
			mtx::handler__ Mutex;
		} _LogData;
		void _Log(
			log__ Log,
			const str::string_ &SessionId )
		{
			if ( _LogData.Functions != NULL ) {
			qRH
			qRB
				mtx::Lock(_LogData.Mutex );
				_LogData.Functions->Log( Log, SessionId );

			qRR
			qRT
				mtx::Unlock( _LogData.Mutex );
			qRE
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _LogData.Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( _LogData.Mutex );
			}

			Sessions_.reset( P );
			_LogData.Functions = NULL;
			_LogData.Mutex = MTX_INVALID_HANDLER;
		}
		tool___( void )
		{
			reset( false );
		}
		~tool___( void )
		{
			reset();
		}
		//f Initialization with 'UserFucntions' as user functions.
		void Init(
			sessions_ &Sessions,
			log_functions__ &LogFunctions )
		{
			Sessions_.Init( Sessions );

			_LogData.Functions = &LogFunctions;

			if ( _LogData.Functions != NULL )
				_LogData.Mutex = mtx::Create();
		}
		void Create(
			void *UP,
			str::string_ &SessionID );
		//f Read 'Data' for session 'SessionID'.
		answer__ GetAndLock(
			const str::string_ &SessionID,
			void *&UP );
		void Release( const str::string_ &SessionID );
		void Release( session_id__ SessionId )
		{
			Release( str::string( SessionId.Value() ) );
		}
		//f Close session of id 'SessionID'.
		answer__ Close( const str::string_ &SessionID );
		//f Close all expired sessions.
		void CloseExpired( bso::bool__ Balance = true );
	};

	//c User functions.
	class user_functions__
	: public ssnmng::user_functions__
	{
	protected:
		/*v Function called for each request, 'URLArguments' containing the
		argument given through the URL, 'Tool' to manage the session, and with 'Client'
		to answer the WEB server. */
		virtual void CGIMNGHandle(
			const cgiarg::arguments_ &URLArguments,
			tool___ &Tool,
			flw::oflow__ &Client ) = 0;
		virtual void CGIMNGDelete( void *UP ) = 0;
		virtual void SSNMNGDelete( void *UP );
	public:
		void Handle(
			const cgiarg::arguments_ &URLArguments,
			tool___ &Tool,
			flw::oflow__ &Client )
		{
			CGIMNGHandle( URLArguments, Tool, Client );
		}
		void Delete( void *UP )
		{
			CGIMNGDelete( UP );
		}
	};
	
	//c The core of the manager (internal use).
	class core_manager
	: public sessions,
	  public tool___,
	  public csdleo::callback__
	{
	protected:
		cgimng::user_functions__ *UserFunctions_;
		virtual void SSNMNGAllocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{}
		virtual void *CSDSCBPreProcess( const char *Origin )
		{
			return NULL;
		}
		csdleo::action__ CSDSCBProcess(
			flw::ioflow__ &Client,
			void *UP );
		void CSDSCBPostProcess( void *UP )
		{
#ifdef CGIMNG_DBG
			if ( UP != NULL )
				qRFwk();
#endif
		}
		void CSDSCBExit( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			sessions::reset( P );
			tool___::reset( P );
			UserFunctions_ = NULL;
		}
		//f Initialization with 'UserFunctions' as user functions.
		void Init(
			cgimng::user_functions__ &UserFunctions,
			bso::u16__ Relative,
			bso::u16__ Absolute,
			log_functions__ &LogFunctions )
		{
			sessions::Init( Relative, Absolute, UserFunctions );
			tool___::Init( *this, LogFunctions );
			UserFunctions_ = &UserFunctions;
		}
	};

	class cgi_manager
	: public csdbns::server___,
	  public core_manager
	{
	public:
		bso::bool__ Init(
			csdbns::port__ Port,
			cgimng::user_functions__ &UserFunctions,
			bso::u16__ Relative,
			bso::u16__ Absolute,
			cgimng::log_functions__ &LogFunctions)
		{
			core_manager::Init( UserFunctions, Relative, Absolute, LogFunctions );
			return server___::Init( Port, *this );
		}
		void Process( sck::duration__ Timeout = CSDNBS__DEFAULT_TIMEOUT )
		{
			server___::Process( Timeout );
		}
	};
}

#endif
