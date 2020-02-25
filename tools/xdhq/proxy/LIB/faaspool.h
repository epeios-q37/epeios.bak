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

# include "sck.h"

namespace faaspool {
	void Initialize();

	// If modified, modify also the 'Undefined' definition.
	typedef bso::sU8 sId;

	qCDEF( sId, Undefined, bso::U8Max );
	qCDEF( sId, Max, Undefined - 1 );

	inline void PutId(
		sId Id,
		flw::rWFlow &Flow )
	{
		dtfptb::FPut( Id, Flow );
	}

	template <typename fd> inline sId GetId(
		fd &FD,
		bso::sBool *IsError )
	{
		sId Id = Undefined;

		return dtfptb::FGet( FD, Id, IsError );
	}

	// Shared between upstream and downstream.
	struct rShared
	{
	public:
		sId Id;
		fdr::rRWDriver *Driver;
		tht::rBlocker Read;	// Handled (created/destroyed) upstream.
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
		qRH;
			fdr::sByte *NewBuffer = NULL;
		qRB;
			if ( !IdSent_ ) {	// To optimize.
				if ( ( NewBuffer = (fdr::sByte *)malloc( Maximum + 1 ) ) == NULL )
						qRAlc();

				NewBuffer[0] = Shared_.Id;
				memcpy( NewBuffer + 1, Buffer, Maximum );

				Buffer = NewBuffer;
				Maximum++;
				IdSent_ = true;
			}

			if ( Shared_.IsValid() ) {
				Maximum = Shared_.Driver->Write( Buffer, Maximum );

				if ( Maximum > 0 )
					Maximum--;
			} else
				Maximum = 0;
		qRR;
		qRT;
			if ( NewBuffer != NULL )
				free( NewBuffer );
		qRE;
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
			fdr::rRWDressedDriver::reset( P );
			Consumed_ = 0;
			Shared_.IsValid();
			IdSent_ = false;
		}
		qCVDTOR( rRWDriver );
		void Init( fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );

			Consumed_ = 0;

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