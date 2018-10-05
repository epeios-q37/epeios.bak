/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SESSION__INC
# define SESSION__INC

# include "xdwrgstry.h"

# include "xdhcmn.h"
# include "xdhujp.h"

# include "sclmisc.h"

# include "tol.h"
# include "bch.h"
# include "idxbtq.h"
# include "lck.h"
# include "mtx.h"
# include "xdhups.h"

namespace session {
	class rSession;	// Predeclaration.

	using xdhups::rAgent;

	struct shared_data__
	{
	private:
		Q37_MRMDF( rSession, S_, Session_ );
		Q37_MRMDF( xdhcmn::cSession, C_, Callback_ );
		Q37_MRMDF( const str::string_ , _GetId, _Id );
		Q37_MRMDF( const str::string_ , _GetAction, _Action );
	public:
		void reset( bso::bool__ P = true )
		{
			_Id = NULL;
			_Action = NULL;
			Session_ = NULL;
			Callback_ = NULL;
		}
		E_CDTOR( shared_data__ );
		void Init( void )
		{
			// _Id.Init();	//	Buffer. No Initialization.
			// _Action.Init();	//	Buffer. No Initialization.

			Session_ = NULL;
			Callback_ = NULL;
		}
		void Init(
			const str::string_ &Id,
			const str::string_ &Action,
			rSession &Session,
			xdhcmn::cSession &Callback )
		{
			Init();

			_Id = &Id;
			_Action = &Action;
			Session_ = &Session;
			Callback_ = &Callback;
		}
		const str::string_ &Id( void )
		{
			return _GetId();
		}
		const str::string_ &Action( void )
		{
			return _GetAction();
		}
		xdhcmn::cSession &Callback( void )
		{
			return C_();
		}
		rSession &Session( void ) const
		{
			return S_();
		}
	};

	E_ENUM( target ) {
		tGlobal,
		tUpstream,
		tDownstream,
		m_amount,
		m_Undefined
	};

	class _mutexes___
	{
	private:
		mtx::handler___
			_Global, 
			_Upstream,
			_Downstream;
		void _Create( mtx::handler___ &Handler )
		{
			if ( Handler != mtx::Undefined )
				qRGnr();

			Handler = mtx::Create();
		}
		void _Create( void )
		{
			_Create( _Global );
			_Create( _Upstream );
			_Create( _Downstream );
		}
		void _Delete( mtx::handler___ &Handler )
		{
			if ( Handler != mtx::Undefined )
				mtx::Delete( Handler );

			Handler = NULL;
		}
		void _Delete( void )
		{
			_Delete( _Global );
			_Delete( _Upstream );
			_Delete( _Downstream );
		}
		mtx::handler___ &_Get( target__ Target )
		{
			switch ( Target ) {
			case tGlobal:
				return _Global;
				break;
			case tUpstream:
				return _Upstream;
				break;
			case tDownstream:
				return _Downstream;
				break;
			default:
				qRGnr();
				break;
			}

			qRGnr();
			return _Global;	// Never called, just to avoid a 'warning'.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_Delete();

			_Global = _Upstream = _Downstream = mtx::Undefined;
		}
		E_CDTOR( _mutexes___ );
		void Init( void )
		{
			_Delete();

			_Create();
		}
		void Lock( target__ Target )
		{
			mtx::Lock(_Get( Target ) );
		}
		void Unlock( target__ Target )
		{
			mtx::Unlock( _Get( Target ) );
		}
		bso::bool__ IsLocked( target__ Target )
		{
			return mtx::IsLocked( _Get( Target ) );
		}
		bso::bool__ TryToLock( target__ Target )
		{
			return mtx::TryToLock( _Get( Target ) );
		}
	};

	typedef xdhujp::cJS cJS_;

	class sJS
	: public cJS_
	{
	private:
		Q37_MRMDF( session::rSession, S_, Session_ );
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Buffer ) override;
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( xdwrgstry::custom_item::attribute_name::Widget, Buffer);
		}
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( xdwrgstry::custom_item::attribute_name::Result, Buffer );
		}
		/*
		virtual void XDHJSPHandleExtensions( const xdhcbk::nstring___ &Id ) override;
		virtual void XDHJSPHandleCastings( const xdhcbk::nstring___ &Id ) override;
		*/
	public:
		void reset( bso::bool__ P = true )
		{
			Session_ = NULL;
		}
		E_CVDTOR( sJS );
		void Init( rSession &Session )
		{
			Session_ = &Session;
		}
	};


	class rSession
	{
	private:
		sJS _JSCallback;
		Q37_MRMDF( xdhcmn::cSession, _SC, _SessionCallback );
		_mutexes___ _Mutexes;
		str::string _Dispatch;
		bso::bool__ _UpstreamCall;	// At 'true' when a upstream call (a call from JS when launching a action, or handling an event) is in progress.
		void _Lock( target__ Target )
		{
			_Mutexes.Lock( Target );
		}
		void _Unlock( target__ Target )
		{
			_Mutexes.Unlock( Target );
		}
		bso::bool__ _IsLocked( target__ Target )
		{
			return _Mutexes.IsLocked( Target );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_JSCallback.reset( P );
			_SessionCallback = NULL;
			_Mutexes.reset( P );
			_Dispatch.reset( P );
			_UpstreamCall = false;
		}
		E_CDTOR( rSession );
		void Init( xdhcmn::cSession &SessionCallback )
		{
			_JSCallback.Init( *this );
			_SessionCallback = &SessionCallback;
			_Mutexes.Init();
			_Dispatch.Init();
			_UpstreamCall = false;
		}
		void UpstreamCallInProgress( bso::bool__ Value )
		{
			if ( !_IsLocked( tGlobal ) )
				qRGnr();

			if ( Value == _UpstreamCall )
				qRGnr();

			_UpstreamCall = Value;
		}
		bso::bool__ IsUpstreamCallInProgress( void )
		{
			if ( !_IsLocked( tGlobal ) )
				qRGnr();

			return _UpstreamCall;
		}
		cJS_ &JSCallback( void )
		{
			return _JSCallback;
		}
		xdhcmn::cSession *SessionCallback( void ) const
		{
			return _SessionCallback;
		}
		// upstream side.
		void UpstreamLaunch(
			const str::string_ &Id,
			const str::string_ &Action,
			str::string_ &Script );	// Script to execute. Empty if nothing to do.
		void UpstreamReport(
			const str::string_ &Response,	// Reponse from previous order.
			str::string_ &Script );	// Script to execute. Empty if nothing to do.
		// downstream side.
		void DownstreamExecute(
			const str::string_ &Script,
			str::string_ &Result );
		void Lock( void )
		{
			_Lock( tGlobal );
		}
		void Unlock( void )
		{
			_Unlock( tGlobal );
		}
		bso::bool__ IsLocked( void )
		{
			return _IsLocked( tGlobal );
		}
		void UnlockUpstream( void )
		{
			_Unlock( tUpstream );
		}
	};

	//t The type of the row of a session.
	E_ROW( row__ );

	typedef bch::E_BUNCH_( row__ ) rows_;
	E_AUTO( rows );

	E_CDEF( bso::u8__, IdSize, 5);

	class id__
	{
		char Raw_[IdSize+1];
	public:
		void reset( bso::bool__ = true )
		{
			memset( Raw_, 0, sizeof( Raw_) );
		}
		E_CDTOR( id__ );
		void Init( void )
		{
			memset( Raw_, 0, sizeof( Raw_) );
		}
		void New( void );
		const char *Value( void ) const
		{
			return Raw_;
		}
		bso::bool__ IsEmpty( void ) const
		{
			return Raw_[0] == 0;
		}
	};


	struct timer__ {
		time_t Relative;
		time_t Absolute;
		bso::bool__ Persistent;
		void reset( bso::bool__ = true )
		{
			Relative = Absolute = 0;
			Persistent = false;
		}
		E_CDTOR( timer__ );
		void Init( void )
		{
			Relative = Absolute = 0;
			Persistent = false;
		}
	};

	typedef lstbch::E_LBUNCHt_( rSession *, row__ ) _sessions_;
	typedef que::E_MQUEUEt_( row__ ) _order_;
	typedef bch::E_BUNCHt_( id__, row__ ) _ids_;
	typedef idxbtq::E_INDEXt_( row__ ) _index_;
	typedef bch::E_BUNCHt_( timer__, row__ ) _timers_;


	class dSessions
	{
	private:
		Q37_MRMDF( rAgent, _A, _Agent );
		void _Close( const rows_ &Rows );
		void _AdjustSizes( void )
		{
			sdr::size__ Size = Sessions.Extent();

			Order.Allocate( Size, aem::mFitted );
			Ids.Allocate( Size, aem::mFitted );
			Index.Allocate( Size, aem::mFitted );
			Timers.Allocate( Size, aem::mFitted );
			
		}
	public:
		struct s
		{
			_sessions_::s Sessions;
			_order_::s Order;
			_ids_::s Ids;
			_index_::s Index;
			_timers_::s Timers;
			bso::u16__ Absolute;
			bso::u16__ Relative;
			row__ Root;
		} &S_;
		_sessions_ Sessions;
		_order_ Order;
		_ids_ Ids;
		_index_ Index;
		_timers_ Timers;
		dSessions( s &S )
		: S_( S ),
		  Sessions( S.Sessions ),
		  Order( S.Order ),
		  Ids( S.Ids ),
		  Index( S.Index ),
		  Timers( S.Timers )
		{}
		void reset( bool P = true )
		{
			S_.Absolute = S_.Relative = 0;
			S_.Root = qNIL;
			_Agent = NULL;

			Sessions.reset( P );
			Order.reset( P );
			Ids.reset( P );
			Index.reset( P );
			Timers.reset( P );
		}
		void plug( qASd *AS )
		{
			Sessions.plug( AS );
			Order.plug( AS );
			Ids.plug( AS);
			Index.plug( AS );
			Timers.plug( AS );
		}
		dSessions &operator =( const dSessions &S )
		{
			Sessions = S.Sessions;
			Order = S.Order;
			Ids = S.Ids;
			Index = S.Index;
			Timers = S.Timers;

			S_.Relative = S.S_.Relative;
			S_.Absolute = S.S_.Absolute;
			S_.Root = S.S_.Root;

			return *this;
		}
		//f Initialization with 'Relative' and 'Absolute' amonut of second.
		void Init(
			bso::u16__ Relative,
			bso::u16__ Absolute,
			rAgent &Agent )
		{
			Sessions.Init();
			Order.Init();
			Ids.Init();
			Index.Init();
			Timers.Init();

			S_.Relative = Relative;
			S_.Absolute = Absolute;
			S_.Root = qNIL;
			_Agent = &Agent;
		}
		//f Return the position of a mandatory new session.
		row__ New(
			id__ &Id,
			const str::string_ &Language,
			const str::dString &Token );
		row__ New(
			str::string_ &Id,
			const str::string_ &Language,
			const str::dString &Token )
		{
			id__ RawId;
			row__ Row = New( RawId, Language, Token );

			Id.Append( RawId.Value() );

			return Row;
		}
		//f Remove the session id at position 'Position'.
		void Close( row__ Row )
		{
			xdhcmn::cSession *Callback = Sessions(Row)->SessionCallback();

			if ( Callback != NULL )
				_A().ReleaseCallback( Callback );

			delete Sessions( Row );
			S_.Root = Index.Delete( Row, S_.Root );
			Sessions.Delete( Row );
			Order.Delete( Row );
		}
		//f Return the position of 'SessionID' or qNIL if non-existent.
		row__ Search( const id__ &Id ) const
		{
			return Search( Id.Value() );
		}
		//f Return the position of 'SessionID' or qNIL if non-existent.
		row__ Search( const char *Id ) const;
		//f Return the position of 'SessionID' or qNIL if non-existent.
		row__ Search( const str::string_ &Id ) const;
		//f Return the session id. corresponding to 'Position'.
		id__ Id( row__ Position )
		{
			return Ids( Position );
		}
		//f Touch the session corresponding at position 'P'.
		void Touch( row__ P )
		{
			timer__ Timer = Timers.Get( P );

			if ( time( &Timer.Relative ) == -1 )
				qRLbr();

			Timer.Persistent = false;

			Timers.Store( Timer, P );
	#ifdef SSNMNG_DBG
			if ( Queue.Amount() == 0 )
				qRGnr();
	#endif

			if ( ( Order.Amount() != 1 ) && ( Order.Tail() != P ) ) {
				Order.Delete( P );
				Order.BecomeNext( P, Order.Tail() );
			}
		}
		// The session is always considered as persistent, at least until next 'Touch(...)'.
		void MarkAsPersistent( row__ P )
		{
			timer__ Timer = Timers.Get( P );

			Timer.Persistent = true;

			Timers.Store( Timer, P );
		}
		//f Return true if session corresponding to 'P' is valid.
		bso::bool__ IsAlive( row__ P ) const
		{
			timer__ Timer = Timers.Get( P );

			return ( Timer.Persistent
				     || ( ( difftime( time( NULL ), Timer.Absolute ) < S_.Absolute )
				          && ( difftime( time( NULL ), Timer.Relative ) < S_.Relative ) ) );
		}
		//f Return true if session (which must exists) corresponding to row 'Row' is expired, false otherwise.
		bso::bool__ IsExpired( row__ Row ) const
		{
			return !IsAlive( Row );
		}
		//f Balance the index. 
		void Balance( void )
		{
			if ( S_.Root != qNIL )
				S_.Root = Index.Balance( S_.Root );
		}
		//f Put in 'Expired' the expired sessions.
		void GetExpired( rows_ &Expired ) const;
		void GetAll( rows_ &Rows ) const;
		void CloseAll( void );
		void CloseExpired( void );
		E_NAVt( Order., row__ )
	};

	qW( Sessions );

	typedef lck::control___<dSessions> rSessions;

}

#endif
