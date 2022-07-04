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

# include "faasbckd.h"

# include "common.h"

using namespace faasbckd;

void rBackend::InvalidAll_(void)
{
qRH;
  faasgate::hGuard Guard;
  faasgate::sRow Row = qNIL;
qRB;
  faasgate::dGates &Gates = LockAndGetGates(Guard, false);

  Row = Gates.First();

  while ( Row != qNIL ) {
    Gates(Row)->UnsetId();

    Row = Gates.Next( Row );
  }
qRR;
qRT;
qRE;
}

void rBackend::reset(bso::sBool P)
{
  if ( P ) {
    InvalidAll_();

    WaitAndDestroy_(GatesAccess_);
    WaitAndDestroy_(Access_);

    if ( TRow != qNIL )
      common::GetCallback().Remove(TRow);
  }

  GatesAccess_ = mtx::Undefined;
  PendingDismiss_ = false;
  tol::reset( P, NoMoreClientBlocker_);
  TRow = qNIL;
  Row = qNIL;
  Driver = NULL;
  Access_ = mtx::Undefined;
  ProtocolVersion = csdcmn::UnknownVersion;
  tol::reset(P, Gates_, Switch, IP, Token, HeadCache, Depot);
}

void rBackend::Init(
  sRow Row,
  faas::sRow TRow,
  fdr::rRWDriver &Driver,
  csdcmn::sVersion ProtocolVersion,
  const str::dString &IP,
  const str::dString &Token,
  const str::dString &Head)
{
  reset();

  if ( ( ProtocolVersion > 1 ) && Head.Amount() ) // With protocol > 1, 'Head' is no more cached,
                                                  // but retrieved on each new browser connection.
    qRGnr();

  GatesAccess_ = mtx::Create();
  PendingDismiss_ = false;
  tol::Init(NoMoreClientBlocker_);
  this->Row = Row;
  this->TRow = TRow;
  this->Driver = &Driver;
  Gates_.Init();
  Access_ = mtx::Create();
  Switch.Init();
  this->ProtocolVersion = ProtocolVersion;
  this->IP.Init( IP );
  this->Token.Init(Token);
  this->HeadCache.Init(Head);
  Depot.Init();
}

namespace {
  faasgate::sRow Set_(
    faasgate::rGate &Gate,
		fdr::rRWDriver *Driver,
		tht::rBlocker &Switch,
    faasgate::dGates &Gates)
  {
    faasgate::sRow Row = qNIL;
  qRH
  qRB
    Row = Gates.New();

    if  (*Row > faas::MaxId )
      qRGnr();

    Gates.Store(&Gate, Row);

    Gate.Set(*Row, Driver, &Switch);
  qRR
    if ( Row != qNIL )
      Gates.Remove(Row);

    Row = qNIL;
  qRT
  qRE
    return Row;
  }
}

bso::sBool rBackend::Set(faasgate::rGate &Gate)
{
  faasgate::sRow Row = qNIL;
qRH
  faasgate::hGuard Guard;
qRB
  Row = Set_(Gate, Driver, Switch, LockAndGetGates(Guard));
qRR
qRT
qRE
  return Row != qNIL;
}

void rBackend::Release(faas::sId Id)
{
qRH
  faasgate::hGuard Guard;
qRB
  faasgate::dGates &Gates = LockAndGetGates(Guard);

  Gates.Remove((faasgate::sRow)Id);

  if ( PendingDismiss_ )
    if ( Gates.Amount() == 0 )
      NoMoreClientBlocker_.Unblock();
qRR
qRT
qRE
}

void rBackend::WaitUntilNoMoreClient(void)
{
qRH
  faasgate::hGuard Guard;
qRB
  common::GetCallback().QuitAll(TRow);

  faasgate::dGates &Gates = LockAndGetGates(Guard);

  if ( Gates.Amount() ) {
    PendingDismiss_ = true;
    Guard().Unlock();
    NoMoreClientBlocker_.Wait();
    Guard().Lock();	// Otherwise 'Guard' could be destroyed before above 'Release' unlocks it.
  }
qRR
qRT
qRE
}
