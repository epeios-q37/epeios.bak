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

	// If modified, modify also the 'Undefined' definition.
	typedef bso::sU8 sId;

	qCDEF( sId, Undefined, bso::U8Max );
	qCDEF( sId, Max, Undefined - 1 );

	template <typename fd> inline void PutId(
		sId Id,
		fd &FD )
	{
		dtfptb::FPut( Id, FD );
	}

	template <typename fd> inline sId GetId( fd &FD )
	{
		sId Id = Undefined;

		return dtfptb::FGet( FD, Id );
	}

	// Shared between upstream and downstream.
	struct rShared
	{
	public:
		sId Id;
		fdr::rRWDriver *Driver;
		tht::rBlocker Read;	// Handled (creates/destroyed) upstream.
		tht::rBlocker *Switch;
		void reset( bso::sBool P = true )
		{
			Id = Undefined;
			Driver = NULL;
			Read.reset( P );
			Switch = NULL;
		}
		void Init( void )
		{
			reset();

			Read.Init();
		}
		qCDTOR( rShared );
		bso::sBool IsValid( void ) const
		{
			return Id != Undefined;
		}
	};

	class rRWDriver
	: public fdr::rRWDressedDriver
	{
	private:
		bso::sBool IdSent_;
		rShared Shared_;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			if ( !Shared_.IsValid() )
				return 0;

			Shared_.Read.Wait();

			if ( !Shared_.IsValid() )
				return 0;

			Shared_.Driver->RTake();

			return Shared_.Driver->Read( Maximum, Buffer, fdr::b_Relay );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			if ( !Shared_.IsValid() )
				return;

			Shared_.Driver->Dismiss( Unlock );

			Shared_.Switch->Unblock();
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Shared_.Driver->RTake( Owner );
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			if ( !Shared_.IsValid() )
				return 0;

			if ( !IdSent_ ) {
				PutId( Shared_.Id, *Shared_.Driver );
				IdSent_ = true;
			}

			return Shared_.Driver->Write( Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			if ( !Shared_.IsValid() )
				return;

			Shared_.Driver->Commit( Unlock );

			IdSent_ = false;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return Shared_.Driver->WTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rRWDressedDriver::reset( P );
			Shared_.IsValid();
			IdSent_ = false;
		}
		qCVDTOR( rRWDriver );
		void Init( fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );

			Shared_.Init();

			IdSent_ = false;
		}
		rShared &GetShared( void )
		{
			return Shared_;
		}

	};

	bso::sBool GetConnection(
		const str::dString &Token,
		str::dString &IP,
		rShared &Shared );
}

#endif