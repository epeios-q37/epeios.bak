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

# include "prtcl.h"

# include "sck.h"

namespace dmopool {
	void Initialize();

	typedef bso::sU8 sId;

	qCDEF(sId, Max, bso::U8Max);

	qCDEF( sId, Undefined,Max );

	// eXetended socket.
	struct gData
	{
	public:
		sId Id;
		sck::sSocket Socket;
		mtx::rHandler Mutex;
		void reset( bso::sBool P = true )
		{
			Id = Undefined;
			Socket = sck::Undefined;
			Mutex = mtx::Undefined;
		}
		qCDTOR( gData );
	};

	class rRWDriver
	: public fdr::rRWDressedDriver
	{
	private:
		sck::rRWDriver Driver_;
		mtx::rMutex Mutex_;
		sId Id_;
		bso::sBool IdSent_;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			Mutex_.Lock();

			return Driver_.Read( Maximum, Buffer, fdr::b_Relay );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			Driver_.Dismiss( Unlock );

			Mutex_.Unlock();
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Driver_.RTake( Owner );
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			if ( !IdSent_ ) {
				prtcl::Put( Id_, Driver_ );
				IdSent_ = true;
			}

			return Driver_.Write( Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			Driver_.Commit( Unlock );

			IdSent_ = false;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return Driver_.WTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rRWDressedDriver::reset( P );
			Driver_.reset( P );
			Mutex_.reset( P );
			Id_ = Undefined;
			IdSent_ = false;
		}
		qCVDTOR( rRWDriver );
		void Init(
			const gData &Data,
			fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );

			if ( Data.Socket == sck::Undefined )
				qRGnr();
			Driver_.Init( Data.Socket, false, ThreadSafety );

			if ( Data.Mutex == mtx::Undefined )
				qRGnr();
			Mutex_.Init( Data.Mutex );

			if ( Data.Id == Undefined )
				qRGnr();
			Id_ = Data.Id;

			IdSent_ = false;
		}
	};

	void GetConnection(
		const str::dString &Token,
		str::dString &IP,
		gData &Data );
}

#endif