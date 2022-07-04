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

# include "faasdpot.h"
# include "faasgate.h"

# include "csdcmn.h"
# include "xdhcmn.h"

# include "mtx.h"
# include "tht.h"

namespace faasbckd {
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
		void InvalidAll_(void);
		bso::sBool WaitAndDestroy_(mtx::rMutex &Mutex) {
		  bso::sBool HasWait = false;

		  if ( Mutex != mtx::Undefined ) {
        HasWait = mtx::Lock(Mutex);

        mtx::Delete(Mutex, true);

        Mutex = mtx::Undefined;
		  }

		  return HasWait;
		}
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
    // Currently used to retrieve HTML head section from backend.
    faasdpot::rDepot Depot;
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
		faasgate::dGates &LockAndGetGates(
      faasgate::hGuard &Guard,
      bso::sBool MustLock = true)
		{
		  if ( GatesAccess_ != mtx::Undefined) {
        if ( Guard().IsInitialized() )
          Guard().Lock();
        else
          Guard().InitAndLock(GatesAccess_);
      } else if ( MustLock )
        qRGnr();

		  return Gates_;
		}
		void UnlockGates(faasgate::hGuard &Guard)
		{
      Guard().Unlock();
		}
		void Lock(hGuard &Guard)
		{
		  Guard().InitAndLock(Access_);
		}
		bso::sBool Set(faasgate::rGate &Gate);
		void Release(faas::sId Id);
		void WaitUntilNoMoreClient(void);
	};
}

#endif
