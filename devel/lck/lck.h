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

// LoCKs

#ifndef LCK_INC_
# define LCK_INC_

# define LCK_NAME		"LCK"

# define	LCK_VERSION	"$Revision: 1.21 $"

# define LCK_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# if defined( E_DEBUG ) && !defined( LCK_NODBG )
#  define LCK_DBG
# endif

# include "err.h"
# include "flw.h"
# include "mtx.h"

/*************/
/**** OLD ****/
/*************/

namespace lck {

	//t Type of an access counter.
	typedef bso::u8__ counter__;	// If changed, change below line too.

	//d Maximum amount of concurrent read.
	#define LCK_MAX_CONCURRENT_READ	BSO_U8_MAX

	class lock___
	{
	public:
		mtx::handler___ ReadCounterProtection;
		mtx::handler___ WritingRequest;
		mtx::handler___ WritingPermission;
		mtx::handler___ OverflowProtection;
		counter__ ReadCounter;
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( ReadCounterProtection != MTX_INVALID_HANDLER )
					mtx::Delete( ReadCounterProtection );

				if ( WritingRequest != MTX_INVALID_HANDLER )
					mtx::Delete( WritingRequest );

				if ( WritingPermission != MTX_INVALID_HANDLER )
					mtx::Delete( WritingPermission );

				if ( OverflowProtection != MTX_INVALID_HANDLER )
					mtx::Delete( OverflowProtection );
			}

			ReadCounterProtection = MTX_INVALID_HANDLER;
			WritingRequest= MTX_INVALID_HANDLER;
			WritingPermission = MTX_INVALID_HANDLER;
			OverflowProtection = MTX_INVALID_HANDLER;

			ReadCounter = 0;
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

			ReadCounterProtection = mtx::Create();
			WritingRequest = mtx::Create();
			WritingPermission = mtx::Create();
			OverflowProtection = mtx::Create();
		}
		void WaitUntilReadingAllowed( void )
		{
			mtx::Lock( WritingRequest );	// Wait until there is no
			mtx::Unlock( WritingRequest );	// more write request pending.

			mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
												// and avoid other to access them.

			while ( ReadCounter == LCK_MAX_CONCURRENT_READ ) { // While there is too more reading pending.
				mtx::Unlock( ReadCounterProtection );	// Release the read counter.
				mtx::Lock( OverflowProtection );	// Wait until there is lesser reading pending.
				mtx::Lock( WritingRequest );	// Wait until there is no
				mtx::Unlock( WritingRequest );	// more write request pending.
				mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
													// and avoid other to access them.
			}

			// There is not too much reading and no writing.

			ReadCounter++;	// To tell that there is one more reading pending.

			if ( ReadCounter == 1 )	// If there was no reading before.
				mtx::Lock( WritingPermission );	// Wait that there is no more writing
												// and forbid all comming writing request.
			else if ( ReadCounter == LCK_MAX_CONCURRENT_READ )	// If there is to much reading.
				mtx::Lock( OverflowProtection );	// Forbid more reading.

			mtx::Unlock( ReadCounterProtection );	// Read counter released.
		}
		void ReadingAchieved( void )
		{
			mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
												// and avoid other to access them.

			if ( ReadCounter == 1 )	// If this was the last reading,
				mtx::Unlock( WritingPermission );	// Allow comming writing.
			else if ( ReadCounter == LCK_MAX_CONCURRENT_READ )	// If there was too much reading.
				mtx::Unlock( OverflowProtection );	// Allox yet more reading.

			ReadCounter--;	// One less reading.

			mtx::Unlock( ReadCounterProtection );	// Read counter released.
		}
		void WaitUntilWritingAllowed( void )
		{
			mtx::Lock( WritingRequest );	// To lock all coming read (and write) request;

			mtx::Lock( WritingPermission );	// To ensure that writing is allowed.
		}
		void WritingAchieved( void )
		{
			mtx::Unlock( WritingPermission );	// Reading request can yet forbid all incoming writing.

			mtx::Unlock( WritingRequest );	// Another writing (an reading) can yet occurs.
		}
		bso::bool__ IsLockedForWriting( void ) const
		{
			return mtx::IsLocked( WritingPermission );
		}
		bso::bool__ IsLockedForReading( void ) const
		{
			bso::bool__ IsLocked;

			mtx::Lock( ReadCounterProtection );	// Wait until read counter available,
												// and avoid other to access them.

			IsLocked = ReadCounter != 0;

			mtx::Unlock( ReadCounterProtection );

			return IsLocked;
		}
		bso::bool__ IsLocked( void ) const
		{
			return IsLockedForWriting() || IsLockedForReading();
		}
	};

	template <typename object> class control___
	{
	private:
		object *_Object;
		struct {
			lck::lock___ Lock;
			bso::bool__ Locked;
			bso::bool__ ReadWrite;
			mtx::handler___ Mutex;
		} _Core, &_Access;
		/* Comme la volatibilit de l'objet en lui-mme n'est pas interssante, mais uniquement celle de l'objet qu'il contrle,
		on utilise '_Access' pour pouvoir mettre toutes les mthodes en 'const'. */
		void _Lock( void ) const
		{
			mtx::Lock( _Access.Mutex );
		}
		void _Unlock( void ) const
		{
			mtx::Unlock( _Access.Mutex );
		}
		void _ReleaseReadOnly( void ) const
		{
#ifdef LCK__DBG
			if ( !_Access.Locked )
				qRFwk();

			if ( _Access.ReadWrite )
				qRFwk();
#endif
			_Access.Lock.ReadingAchieved();

			_Access.Locked = false;
		}
		void _ReleaseReadWrite( void ) const
		{
#ifdef LCK__DBG
			if ( !_Access.Locked )
				qRFwk();

			if ( !_Access.ReadWrite )
				qRFwk();
#endif
			_Access.Lock.WritingAchieved();

			_Access.Locked = false;
		}
	public:
		void reset( bso::bool__ P = true )
		{

			if ( P ) {
				if ( _Access.Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( _Access.Mutex );
			}

			_Object = NULL;
			_Access.Lock.reset( P );
			_Access.Locked = false;
			_Access.ReadWrite = false;
			_Access.Mutex = MTX_INVALID_HANDLER;
		}
		control___( void )
		: _Access( _Core )
		{
			reset( false );
		}
		~control___( void )
		{
			reset( true );
		}
		void Init( object &Object )
		{
			reset();

			_Object = &Object;
			_Access.Lock.Init();
			_Access.Mutex = mtx::Create();
		}
		control___ &operator =( const control___ &C )
		{
			// Pour des raisons de standardisation, car cet oprateur n'a pas de sens dans ce contexte.
			qRFbd();

			return *this;	// Pour viter un 'warning'.
		}
		const object &GetReadOnly( void ) const
		{
#ifdef LCK__DBG
			_Lock();
			if ( _Access.Locked )
				qRFwk();
			_Unlock();
#endif
			_Access.Lock.WaitUntilReadingAllowed();

			_Lock();
			_Access.Locked = true;
			_Access.ReadWrite = false;
			_Unlock();

			return *_Object;
		}
		const object &GetSharedAccess( void ) const
		{
			return GetReadOnly();
		}
		void ReleaseReadOnly( void ) const
		{
			_Lock();
			_ReleaseReadOnly();
			_Unlock();
		}
		void ReleaseSharedAccess( void ) const
		{
			ReleaseReadOnly();
		}
		object &GetReadWrite( void ) const
		{
#ifdef LCK__DBG
			_Lock();
			if ( _Access.Locked )
				qRFwk();
			_Unlock();
#endif
			_Access.Lock.WaitUntilWritingAllowed();

			_Lock();
			_Access.Locked = true;
			_Access.ReadWrite = true;
			_Unlock();

			return *_Object;
		}
		object &GetExclusiveAccess( void ) const
		{
			return GetReadWrite();
		}
		void ReleaseReadWrite( void ) const
		{
			_Lock();
			_ReleaseReadWrite();
			_Unlock();
		}
		void ReleaseExclusiveAccess( void ) const
		{
			ReleaseReadWrite();
		}
		bso::bool__ ReleaseLock( void )	const // Return true if it was locked.
		{
			bso::bool__ WasLocked = false;

			_Lock();

			if ( _Access.Locked ) {
				if ( _Access.ReadWrite )
					ReleaseReadWrite();
				else
					ReleaseReadOnly();
				WasLocked =true;
			}

			_Unlock();

			return WasLocked;
		}
		object &GetWithoutLocking( void )
		{
			return *_Object;
		}
		const object &GetWithoutLocking( void ) const
		{
			return *_Object;
		}
		bso::bool__ IsLocked( void ) const
		{
			return _Access.Lock.IsLocked();
		}
	};

	template <typename object> class read_only_access___
	{
	private:
		Q37_MRMDF( control___<object>, C_, Control_ );
		bso::bool__ _Locked;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Control_ != NULL ) {
					if ( _Locked )
						C_().ReleaseReadOnly();
				}

			Control_ = NULL;
			_Locked = false;
		}
		read_only_access___( void )
		{
			reset( false );
		}
		~read_only_access___( void )
		{
			reset();
		}
		void Init( control___<object> &Control )
		{
			reset();

			Control_ = &Control;
		}
		const object &operator ()( void )
		{
			if ( !_Locked ) {
				C_().GetReadOnly();
				_Locked = true;
			}

			return C_().GetWithoutLocking();
		}
		bso::bool__ IsLocked( void ) const
		{
			return _Locked;
		}
		bso::bool__ Release( bso::bool__ ErrorIfNotLocked = true )
		{
			if ( _Locked ) {
				Control_->ReleaseReadOnly();
				_Locked = false;
				return true;
			} else if ( ErrorIfNotLocked )
				qRFwk();

			return false;
		}
	};

	template <typename object> class read_write_access___
	{
	private:
		control___<object> *Control_;
		bso::bool__ _Locked;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Control_ != NULL )
					if ( _Locked )
						Control_->ReleaseReadWrite();

			Control_ = NULL;
			_Locked = false;
		}
		read_write_access___( void )
		{
			reset( false );
		}
		~read_write_access___( void )
		{
			reset();
		}
		void Init( control___<object> &Control )
		{
			reset();

			Control_ = &Control;
		}
		object &operator ()( void )
		{
			if ( !_Locked ) {
				Control_->GetReadWrite();
				_Locked = true;
			}

			return Control_->GetWithoutLocking();
		}
		bso::bool__ IsLocked( void ) const
		{
			return _Locked;
		}
		bso::bool__ Release( bso::bool__ ErrorIfNotLocked = true )
		{
			if ( _Locked ) {
				Control_->ReleaseReadWrite();
				_Locked = false;
				return true;
			} else if ( ErrorIfNotLocked )
				qRFwk();

			return false;
		}
	};


	template <typename objet> E_TTCLONE__( read_write_access___<objet>,	exclusive_access___ );
	template <typename objet> E_TTCLONE__( read_only_access___<objet>,	shared_access___ );
}

/*************/
/**** OLD ****/
/*************/

namespace lck {
	template <typename object> E_TTCLONE__( control___<object>, rTutor );
	template <typename object> E_TTCLONE__( read_write_access___<object>, rReadWriteAccess );
	template <typename object> E_TTCLONE__( read_only_access___<object>, rReadOnlyAccess );
	template <typename object> E_TTCLONE__( read_write_access___<object>, rExclusiveAccess );
	template <typename object> E_TTCLONE__( read_only_access___<object>, rSharedAccess );
}

#endif
