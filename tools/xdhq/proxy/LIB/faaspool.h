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

# include "common.h"

# include "sck.h"

namespace faaspool {
	using namespace common::faas;

	void _Initialize();

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

	// Gate regulating communications between upstream (session) and downstream (backend).
	class rGate
	{
	private:
    mtx::rMutex Mutex_;	// To protect below 2 members.
    bso::sBool Quit_;	// Set by the switcher to force closing.
    bso::sBool Pending_;	// Set by the session when blocked for reading.
    tht::rBlocker Read_;
		sId Id_;
		fdr::rRWDriver *Driver_;
		tht::rBlocker *Switch_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if (Mutex_ != mtx::Undefined)
					mtx::Delete(Mutex_);

				if ( Switch_ != NULL ) {	// At 'NULL' when destruction occurs after backend becomes unavailable.
					if ( !Read_.IsBlocked() ) {
							if ( Switch_->IsBlocked() )
								Switch_->Unblock();
							else
								qRGnr();
					}
				}
			}

			Mutex_ = mtx::Undefined;
			Quit_ = false;
			Pending_ = false;
			Read_.reset( P );
			Id_ = UndefinedId;
			Driver_ = NULL;
			Switch_ = NULL;
		}
		void Init( void )
		{
			reset();

			Mutex_ = mtx::Create();

			Read_.Init();
		}
		qCDTOR( rGate );
		qRODISCLOSEr(sId, Id);
		fdr::rRWDriver &Driver(void) const
		{
		  return *Driver_;
		}
		tht::rBlocker &Switch(void) const
		{
		  return *Switch_;
		}
		void Set(
      sId Id,
      fdr::rRWDriver *Driver,
      tht::rBlocker *Switch)
    {
      Id_ = Id;
      Driver_ = Driver;
      Switch_ = Switch;
    }
    void UnsetId(void)
    {
      Id_ = UndefinedId;
    }
		bso::sBool WaitForRead(void)
		{
			bso::sBool Return = true;
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);

			if ( Quit_) {
				Return = false;
			} else {
				Pending_ = true;
				Mutex.Unlock();
				Read_.Wait();
				Mutex.Lock();
				Return = !Quit_;
			}
		qRR
		qRT
		qRE
			return Return;
		}
		void UnblockReading(void)
		{
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);

			if ( Quit_ )
				qRGnr();

			Pending_ = false;

			Read_.Unblock();
		qRR
		qRT
		qRE
		}
		void UnblockAndQuit(void)
		{
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);

			if ( Quit_ )
				qRGnr();

			Quit_ = true;
			Switch_ = NULL;

			if ( Pending_ )
				Read_.Unblock();
		qRR
		qRT
		qRE
		}
		bso::sBool IsValid( void ) const
		{
			return Id_ != UndefinedId;
		}
	};

	class rBackend_;

	sRow NewSession_(
		const str::dString &Token,
		str::dString &IP,
		rGate &Gate,
		rBackend_ *&Backend);

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
		qRMV(rBackend_, B_, Backend_);
		bso::sSize Consumed_;
		bso::sBool IdSent_;
		rGate Gate_;
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
		sRow Init(
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
		rGate &GetGate( void )
		{
			return Gate_;
		}
	};

	bso::sBool GetHead(
		const str::dString &Token,
		str::dString &Head );
}

#endif
