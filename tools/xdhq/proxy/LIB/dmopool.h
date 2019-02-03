/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	This is the new launcher which will replace the existing one.
	Both will exist together until switching from old to new one is achieved.
*/

// DeMO POOL
// Handles the pool of connexion needed by the DEMO mode.

#ifndef DMOPOOL_INC_
# define DMOPOOL_INC_

# include "sck.h"

namespace dmopool {
	void Initialize();

	typedef bso::sU8 sId;

	qCDEF(sId, Max, bso::U8Max);

	qCDEF( sId, Undefined,Max );

	// eXetended socket.
	struct sXSocket
	{
	public:
		sId Id;
		sck::rRWDriver Driver;
		void reset( bso::sBool P = true )
		{
			Driver.reset( P );
			Id = Undefined;
		}
		qCVDTOR( sXSocket );
		void Init(
			sId Id = Undefined,
			sck::sSocket Socket = sck::Undefined )
		{
			this->Id = Id;

			if ( Socket != sck::Undefined )
				Driver.Init( Socket, true, fdr::ts_Default );
			else
				Driver.reset();
		}
	};

	class rRWDriver_
	: public fdr::rRWDressedDriver
	{
	private:
		sXSocket XSocket_;
		mtx::rMutex Mutex_;
		bso::sBool IdSent_;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			Mutex_.Lock();

			return XSocket_.Driver.Read( Maximum, Buffer );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			XSocket_.Driver.Dismiss( Unlock );

			Mutex_.Unlock();
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return XSocket_.Driver.RTake( Owner );
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			if ( !IdSent_ ) {
				prtcl::Put( XSocket_.Id, XSocket_.Driver );
				IdSent_ = true;
			}

			return XSocket_.Driver.Write( Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			XSocket_.Driver.Commit( Unlock );

			IdSetn_ = false;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rRWDressedDriver::reset( P );
			XSocket_.reset( P );
			Mutex_.reset( P );
			IdSent_ = false;
		}
		qCVDTOR( rRWDriver_ );
		void Init(
			sXSocket XSocket,
			mtx::rHandler MutexHandler,
			fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );
			XSocket_ = XSocket;
			Mutex_.Init( MutexHandler );
			IdSent_ = false;
		}
	};

	sXSocket GetConnection(
		const str::dString &Token,
		str::dString &IP );
}

#endif