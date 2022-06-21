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

# include "faasbkds.h"

# include "common.h"

# include "sck.h"

namespace faaspool {
	void Initialize();

	template <typename fd>  inline void PutId(
		faas::sId Id,
		fd &FD )
	{
		dtfptb::VPut( Id, FD );
	}

	template <typename fd> inline faas::sId GetId(
		fd &FD,
		bso::sBool *IsError )
	{
		faas::sId Id = faas::UndefinedId;

		return dtfptb::VGet( FD, Id, IsError );
	}

	faas::sRow NewSession_(
		const str::dString &Token,
		str::dString &IP,
		faasgate::rGate &Gate,
		faasbckd::rBackend *&Backend);

	class cGuard
	{
	protected:
		virtual void XDXReportBackendLost(void) = 0;
	public:
		void ReportBackendLost(void)
		{
			return XDXReportBackendLost();
		}
	};

	class rRWDriver
	: public fdr::rRWDressedDriver
	{
	private:
		qRMV(faasbckd::rBackend, B_, Backend_);
		bso::sSize Consumed_;
		bso::sBool IdSent_;
		faasgate::rGate Gate_;
		bso::sBool IsValid_(void)
		{
			return (Backend_ != NULL) && Gate_.IsValid();
		}
		fdr::rRWDriver &D_(void)
		{
			// NOTA: The underlying driver is valid as long as the backend exists,
			// hence it does not really matter if the driver becomes invalid after
			// calling this function.
			if ( !IsValid_())
				qRGnr();

			return Gate_.Driver();
		}
		// A reading leaves the data in the underlying driver,
		// otherwise we could read data which are not for us,
		// and therefor will not be available for the real recipient.
		// This function consumes the data in the underlying driver
		// which were really for us.
		bso::sBool Consume_( bso::sSize Amount )
		{
			if ( Amount < Consumed_ )
				qRGnr();

			Amount -= Consumed_;

			Consumed_ += Amount;

			if ( Amount )
				if ( D_().Read( Amount, NULL, fdr::bBlocking ) != Amount )
					qRGnr();

			return Amount != 0;
		}
		void Release_(void);
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			if ( !Consume_( fdr::rRWDressedDriver::AmountRed() ) ) {
				if ( !Gate_.WaitForRead() )	// The underlying backend does no more exist.
					return 0;	// EOF.

				D_().RTake();
			}

			return D_().Read( Maximum, Buffer, fdr::bKeepNonBlocking );
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			Consume_( fdr::rRWDressedDriver::AmountRed() );

			fdr::rRWDressedDriver::EmptyCache();

			Consumed_ = 0;

			if ( D_().Dismiss( Unlock, ErrHandling ) ) {
				Gate_.Switch().Unblock();

				return true;
			} else
				return false;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return D_().RTake( Owner );
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			if ( !IdSent_ ) {
				PutId(Gate_.Id(), D_());

				IdSent_ = true;
			}

			return D_().Write( Buffer, Maximum );
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( D_().Commit( Unlock, ErrHandling ) ) {
				IdSent_ = false;
				return true;
			} else
				return false;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return D_().WTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( IsValid_() ) {
					Release_();
				}
			}

			fdr::rRWDressedDriver::reset( P );
			Backend_ = NULL;
			Consumed_ = 0;
			Gate_.reset(P);
			IdSent_ = false;
		}
		qCVDTOR( rRWDriver );
		faas::sRow Init(
			const str::dString &Token,
			str::dString &IP,
			fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );

			Consumed_ = 0;

			Gate_.Init();

			IdSent_ = false;

			return NewSession_(Token, IP, Gate_, Backend_);
		}
		faasgate::rGate &GetGate( void )
		{
			return Gate_;
		}
	};

	bso::sBool GetHead(
		const str::dString &Token,
		str::dString &Head );
}

#endif
