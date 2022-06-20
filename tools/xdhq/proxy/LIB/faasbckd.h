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

// FAAS (FaaS) BaCKenD

#ifndef FAASBCKD_INC_
# define FAASBCKD_INC_

# include "faasgate.h"

# include "csdcmn.h"
# include "xdhcmn.h"

# include "mtx.h"
# include "tht.h"

namespace faasbckd {
  // Head content handling since FaaS protocol v1.
  class rHeadRelay
  {
  private:
    str::dString *Relay_;
    tht::rBlocker
      Processed_,  // Blocks until the relay data is processed.
      Guard_; // Used on error, to ensure
  public:
    void reset(bso::sBool P = true)
    {
      Relay_ = NULL;

      tol::reset(P, Processed_, Guard_);
    }
    qCDTOR(rHeadRelay);
    void Init(void)
    {
      Relay_ = NULL;
      tol::Init(Processed_, Guard_);
    }
   void SetRelay(str::dString &Head)
    {
      if ( Relay_ != NULL )
        qRGnr();

      Relay_ = &Head;
    }
    // If returns false, an error occurs and the head could not be retrieved.
    bso::sBool WaitForData(void)
    {
      Processed_.Wait();

      if ( Relay_ != NULL ) {
        Relay_ = NULL;
        return true;
      } else
        return false;
    }
    str::dString &GetRelay(void) const
    {
      // No locking or blocking, as this is called only after a 'GetHead' request.

      if ( Relay_ == NULL )
        qRGnr();

      if ( Relay_->Amount() )
        qRGnr();

      return *Relay_;
    }
    void ReportProcessed(void)
    {
      Processed_.Unblock(); // The blocker is also rearmed.
    }
    bso::sBool IsBusy(void) const
    {
      return Relay_ != NULL;
    }
    bso::sBool Dismiss(void)  // Called on error or deconnection.
    {
      if ( Relay_ != NULL ) {
        Relay_ = NULL;  // To report an error on head retrieving.
        Processed_.Unblock();
        Guard_.Wait();
        return true;
      } else
        return false;
    }
    void ReleaseGuard(void)
     {
        Guard_.Unblock();
     }
  };

 	qROW( Row );	// Back-end Row.

  qMIMICr(mtx::rHandle, hGuard);

	class rBackend
	{
	private:
		faasgate::wGates Gates_;
		// Protects access to 'Gates' and 'PendingDismiss_';
		mtx::rMutex GatesAccess_;
		bso::sBool PendingDismiss_;
		// Protect access to backend.
		mtx::rMutex Access_;
		// Prevents destruction of 'Driver_' until no more client use it.
		tht::rBlocker NoMoreClientBlocker_;
		faasgate::dGates &LockAndGetGates_(mtx::rHandle &Guard)
		{
		  Guard.InitAndLock(GatesAccess_);

		  return Gates_;
		}
		void InvalidAll_(void);
	public:
		sRow Row;	// Backend row.
		faas::sRow TRow; // Token row.	// Can be 'qNIL' in self-hosted mode (no/empty token).
		fdr::rRWDriver *Driver;	// Is also set to NULL when the backend is no more present.
		tht::rBlocker Switch;
		csdcmn::sVersion ProtocolVersion;
		str::wString
			IP,
			Token,
			HeadCache; // Head content cache on old behavior (FaaS protocol v0).
    rHeadRelay HeadRelay;
		void reset( bso::sBool P = true );
		qCDTOR( rBackend );
		void Init(
			sRow Row,
			faas::sRow TRow,
			fdr::rRWDriver &Driver,
			csdcmn::sVersion ProtocolVersion,
			const str::dString &IP,
			const str::dString &Token,
			const str::dString &Head);
		void Hold(hGuard &Guard)
		{
		  Guard().InitAndLock(Access_);
		}
		bso::sBool Set(faasgate::rGate &Gate);
		void Release(faas::sId Id);
		void WaitUntilNoMoreClient(void);
	};
}

#endif
