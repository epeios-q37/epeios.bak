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

# include "faasgate.h"

using namespace faasgate;

void rGate::reset(bso::sBool P)
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
  Id_ = faas::UndefinedId;
  Driver_ = NULL;
  Switch_ = NULL;
}

bso::sBool rGate::WaitForRead(void)
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

void rGate::UnblockReading(void)
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

void rGate::UnblockAndQuit(void)
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

