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

// FAAS (FaaS) POOL
// Handles the pool of connexion needed by the FaaS mode.

#ifndef FAASPOOL_INC_
# define FAASPOOL_INC_

# include "prtcl.h"

# include "xdhcuc.h"

# include "sck.h"

namespace faaspool {
	void SetCallback(xdhcuc::cGlobal &Callback);

	void Initialize();

	typedef bso::sS16 sId;
	qCDEF( sId, Max, bso::S8Max );
	qCDEF( sId, UndefinedId, -1 );
	qCDEF( sId, CreationId, -2 );
	qCDEF( sId, ClosingId, -3 );

	template <typename fd>  inline void PutId(
		sId Id,
		fd &FD )
	{
		dtfptb::VPut( Id, FD );
	}

	template <typename fd> inline sId GetId(
		fd &FD,
		bso::sBool *IsError )
	{
		sId Id = UndefinedId;

		return dtfptb::VGet( FD, Id, IsError );
	}

	// Shared between upstream and downstream.
	struct rShared
	{
	public:
		sId Id;
		fdr::rRWDriver *Driver;
		tht::rBlocker Read;
		tht::rBlocker *Switch;
		void reset( bso::sBool P = true )
		{
			Id = UndefinedId;
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
			return Id != UndefinedId;
		}
	};

	class rBackend_;

	bso::sBool GetConnection_(
		const str::dString &Token,
		str::dString &IP,
		rShared &Shared,
		rBackend_ *&Backend);

	class rRWDriver
	: public fdr::rRWDressedDriver
	{
	private:
		qRMV(rBackend_, B_, Backend_);
		bso::sSize Consumed_;
		bso::sBool IdSent_;
		rShared Shared_;
		bso::sBool Consume_( bso::sSize Amount )
		{
			if ( Amount < Consumed_ )
				qRGnr();

			Amount -= Consumed_;

			Consumed_ += Amount;

			if ( Amount )
				if ( Shared_.Driver->Read( Amount, NULL, fdr::bBlocking ) != Amount )
					qRGnr();

			return Amount != 0;
		}
		void Release_(void);
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			if ( !Shared_.IsValid() )
				return 0;

			if ( !Consume_( fdr::rRWDressedDriver::AmountRed() ) ) {
				Shared_.Read.Wait();

				if ( !Shared_.IsValid() )
					return 0;

				Shared_.Driver->RTake();
			}

			return Shared_.Driver->Read( Maximum, Buffer, fdr::bKeepNonBlocking );
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( !Shared_.IsValid() )
				return true;

			Consume_( fdr::rRWDressedDriver::AmountRed() );

			fdr::rRWDressedDriver::EmptyCache();

			Consumed_ = 0;

			if ( Shared_.Driver->Dismiss( Unlock, ErrHandling ) ) {

				Shared_.Switch->Unblock();

				return true;
			} else
				return false;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Shared_.Driver->RTake( Owner );
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			if ( !IdSent_ ) {
				PutId(Shared_.Id, *Shared_.Driver);

				IdSent_ = true;
			}

			if ( Shared_.IsValid() ) {
				Maximum = Shared_.Driver->Write( Buffer, Maximum );
			} else
				Maximum = 0;

			return Maximum;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( !Shared_.IsValid() )
				return true;

			if ( Shared_.Driver->Commit( Unlock, ErrHandling ) ) {
				IdSent_ = false;
				return true;
			} else
				return false;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return Shared_.Driver->WTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Shared_.IsValid() ) {
					Release_();
				}
			}

			fdr::rRWDressedDriver::reset( P );
			Backend_ = NULL;
			Consumed_ = 0;
			Shared_.reset(P);
			IdSent_ = false;
		}
		qCVDTOR( rRWDriver );
		bso::sBool Init(
			const str::dString &Token,
			str::dString &IP,
			fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );

			Consumed_ = 0;

			Shared_.Init();

			IdSent_ = false;

			return GetConnection_(Token, IP, Shared_, Backend_);
		}
		rShared &GetShared( void )
		{
			return Shared_;
		}
	};

	bso::sBool GetHead(
		const str::dString &Token,
		str::dString &Head );
}

#endif
