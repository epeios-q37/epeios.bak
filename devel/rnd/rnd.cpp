/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define RND_COMPILATION_

#include "rnd.h"

using namespace rnd;


namespace {
  mtx::rMutex RandAccess_ = mtx::Undefined;
}

void rnd::LockRand(hGuard &Guard)
{
  Guard().InitAndLock(RandAccess_);
}

int rnd::Rand(void)
{
  int Value = 0;
qRH;
  hGuard Guard;
qRB;
  LockRand(Guard);

#undef rand
  Value = rand();
#define rand TOL_RAND_MACRO
qRR;
qRT;
qRE;
  return Value;
}


const char *rnd::UUIDGen( bUUID &UUID )
{
qRH;
  hGuard Guard;
qRB;
  LockRand(Guard);

	// http://stackoverflow.com/questions/2174768/generating-random-uuids-in-linux
	// Modified, because, with the original, the size of the UUID can vary.
	sprintf(UUID, "%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
		TURand()&0xffff, TURand()&0xffff,
		TURand()&0xffff,
		((TURand() & 0x0fff) | 0x4000),
		TURand() % 0x3fff + 0x8000,
		TURand()&0xffff, TURand()&0xffff, TURand()&0xffff);
qRR;
qRT;
qRE;
	return UUID;
}

qGCTOR( rnd )
{
	InitializeRandomGenerator();

	if ( RandAccess_ != mtx::Undefined )
    qRUnx();

  RandAccess_ = mtx::Create();
}

qGDTOR( rnd )
{
  if ( RandAccess_ != mtx::Undefined )
    mtx::Delete(RandAccess_, true);

  RandAccess_ = mtx::Undefined;
}

