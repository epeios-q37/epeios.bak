/*
	'spp' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'spp' header file ('spp.h').
	Copyright (C) 2000-2002  Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
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

