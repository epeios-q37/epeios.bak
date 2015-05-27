/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define SPP__COMPILATION

#include "spp.h"


using namespace spp;

static flw::amount__ Get_(
	shared_pipe___ &Pipe,
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	flw::amount__ Amount = 0;

	while ( Amount < Minimum )
		Amount += Pipe.Read( (amount__)( Wanted - Amount ), Buffer + Amount );

	return Amount;
}

static flw::amount__ Put_(
	shared_pipe___ &Pipe,
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bso::bool__ Synchronization )
{
	flw::amount__ Amount = 0;

	while ( Amount < Minimum )
		Amount += Pipe.Write( Buffer + Amount, (amount__)( Wanted - Amount ), Synchronization );

	return Amount;
}

flw::amount__ spp::shared_pipe_iflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( *Pipe_, Minimum, Buffer, Wanted );
}

flw::amount__ spp::shared_pipe_oflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( *Pipe_, Buffer, Wanted, Minimum, Synchronization );
}

flw::amount__ spp::master_shared_bipipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Bipipe_->GetSlave(), Minimum, Buffer, Wanted );
}

flw::amount__ spp::master_shared_bipipe_ioflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	Bipipe_->GetSlave().LockRead();

	return ::Put_( Bipipe_->GetMaster(), Buffer, Wanted, Minimum, Synchronization );
}


flw::amount__ spp::slave_shared_bipipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	Bipipe_->GetSlave().LockWrite();

	return ::Get_( Bipipe_->GetMaster(), Minimum, Buffer, Wanted );
}

flw::amount__ spp::slave_shared_bipipe_ioflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Bipipe_->GetSlave(), Buffer, Wanted, Minimum, Synchronization );
}

