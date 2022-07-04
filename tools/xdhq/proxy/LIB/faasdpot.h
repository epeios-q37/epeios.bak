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

// FAAS (FaaS) DePOT

#ifndef FAASDPOT_INC_
# define FAASDPOT_INC_

# include "str.h"

namespace faasdpot {
  // For use in FaaS protocol v1.
  // Request dor data send by a thread, answer handled by another.
  // Data is sored in this object. Used for the handling oh HTML head.
  class rDepot
  {
  private:
    str::dString *Pointer_;
    tht::rBlocker
      Processed_,  // Blocks until the relay data is processed.
      Guard_; // Used on error, to ensure containing backend not destroyed before this object.
  public:
    void reset(bso::sBool P = true)
    {
      Pointer_ = NULL;

      tol::reset(P, Processed_, Guard_);
    }
    qCDTOR(rDepot);
    void Init(void)
    {
      Pointer_ = NULL;
      tol::Init(Processed_, Guard_);
    }
   void SetReceptacle(str::dString &Receptacle)
    {
      if ( Pointer_ != NULL )
        qRGnr();

      Pointer_ = &Receptacle;
    }
    // If returns false, an error occurs and the head could not be retrieved.
    bso::sBool WaitForContent(void)
    {
      Processed_.Wait();

      if ( Pointer_ != NULL ) {
        Pointer_ = NULL;
        return true;
      } else
        return false;
    }
    str::dString &GetReceptacle(void) const
    {
      // No locking or blocking, as this is called only after a 'GetDepot' request.

      if ( Pointer_ == NULL )
        qRGnr();

      if ( Pointer_->Amount() )
        qRGnr();

      return *Pointer_;
    }
    void ReportContentAvailable(void)
    {
      Processed_.Unblock(); // The blocker is also rearmed.
    }
    bso::sBool IsBusy(void) const
    {
      return Pointer_ != NULL;
    }
    bso::sBool Dismiss(void)  // Called on error or disconnection.
    {
      if ( Pointer_ != NULL ) {
        Pointer_ = NULL;  // To report an error on head retrieving.

        Processed_.Unblock();
        Guard_.Wait();

        return true;
      } else
        return false;
    }
    void Release(void)
     {
        Guard_.Unblock();
     }
  };

}

#endif
