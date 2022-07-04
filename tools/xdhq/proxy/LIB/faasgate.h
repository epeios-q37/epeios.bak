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

// FAAS (FaaS) GATE

#ifndef FAASGATE_INC_
# define FAASGATE_INC_

# include "faas.h"

# include "mtx.h"
# include "tht.h"

namespace faasgate {

  // Gate regulating communications between upstream (session) and downstream (backend).
	class rGate
	{
	private:
    mtx::rMutex Mutex_;	// To protect below 2 members.
    bso::sBool Quit_;	// Set by the switcher to force closing.
    bso::sBool Pending_;	// Set by the session when blocked for reading.
    tht::rBlocker Read_;
		faas::sId Id_;
		fdr::rRWDriver *Driver_;
		tht::rBlocker *Switch_;
	public:
		void reset( bso::sBool P = true );
		void Init( void )
		{
			reset();

			Mutex_ = mtx::Create();

			Read_.Init();
		}
		qCDTOR( rGate );
		qRODISCLOSEr( faas::sId, Id );
		fdr::rRWDriver &Driver(void) const
		{
		  return *Driver_;
		}
		tht::rBlocker &Switch(void) const
		{
		  return *Switch_;
		}
		void Set(
      faas::sId Id,
      fdr::rRWDriver *Driver,
      tht::rBlocker *Switch)
    {
      Id_ = Id;
      Driver_ = Driver;
      Switch_ = Switch;
    }
    void UnsetId(void)
    {
      Id_ = faas::UndefinedId;
    }
		bso::sBool WaitForRead(void);
		void UnblockReading(void);
		void UnblockAndQuit(void);
		bso::sBool IsValid( void ) const
		{
			return Id_ != faas::UndefinedId;
		}
	};

	qROW( Row );

  qMIMICr(mtx::rHandle, hGuard);

	typedef lstbch::qLBUNCHd( rGate *, sRow ) dGates;
	qW( Gates );
}

#endif
