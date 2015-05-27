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

#define PIP__COMPILATION

#include "pip.h"

using namespace pip;

#include "stf.h"
#include "tol.h"

#ifdef CPE__UNIX
#	include <signal.h>
#endif

#ifdef PIP__UNIX_LIKE
#define INVALID_HANDLE_VALUE	-1	// Already defined under Windows.
#endif

void pip::pipe___::Vider( void )
{
	datum__ Tampon[100];

	do {
		Read( sizeof( Tampon ), Tampon );
	} while( !IsEmpty() );
}

pip::amount__ pip::pipe___::Write(
	const datum__ *Buffer,
	amount__ Amount,
	bso::bool__ Synchronization )
{
	pip::amount__ NombreEcrits;
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	WriteInProgress_ = true;
#ifdef PIP_DBG
	if ( ( Pipe_[pip::eWrite] == INVALID_HANDLE_VALUE )
		 || ( Pipe_[pip::eRead] == INVALID_HANDLE_VALUE ) )
		 ERRu();
#endif

#ifdef PIP_DBG
	if ( Mutex_ == NULL )
		ERRc();
#endif

	mtx::Lock( Mutex_ );
	Locked = true;
	
#ifdef PIP_DBG
	if ( Mutex_ == NULL )
		ERRc();
#endif

	Available_ += Amount;

	Synchronization_ = Synchronization;
	
	mtx::Unlock( Mutex_ );
	Locked = false;

#ifdef PIP_DBG
	if ( Mutex_ == NULL )
		ERRc();
#endif

#ifdef PIP_DBG
	if ( Pipe_[0] == PIP_INVALID_PIPE )
		Locked = false;
		
#endif

	NombreEcrits = pip::Write( Pipe_, Buffer, Amount );

#ifdef PIP_DBG
	if ( Mutex_ == NULL )
		ERRc();
#endif

	mtx::Lock( Mutex_ );
	Locked = true;

#ifdef PIP_DBG
	if ( Mutex_ == NULL )
		ERRc();
#endif

	Available_ -= Amount - NombreEcrits;

	Synchronization_ = Synchronization && ( NombreEcrits == Amount );
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( Mutex_ );
		
	WriteInProgress_ = false;

ERREpilog
	return NombreEcrits;
}

pip::amount__ pip::pipe___::Read(
	amount__ Amount,
	datum__ *Buffer )
{
	pip::amount__ NombreLus;
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	ReadInProgress_ = true;

#ifdef PIP_DBG
	if ( ( Pipe_[pip::eWrite] == INVALID_HANDLE_VALUE )
		 || ( Pipe_[pip::eRead] == INVALID_HANDLE_VALUE ) )
		 ERRu();
#endif

	NombreLus = pip::Read( Pipe_, Amount, Buffer );

	mtx::Lock( Mutex_ );
	Locked = true;

#ifdef PIP_DBG
	if ( Available_ < NombreLus )
		ERRc();
#endif

	Available_ -= NombreLus;

ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( Mutex_ );
		
	ReadInProgress_ = false;
ERREpilog
	return NombreLus;
}

static flw::amount__ Get_(
	pipe___ &Pipe,
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	flw::amount__ Amount = 0;

	if ( Minimum )
	{
		while ( Amount < Minimum )
			Amount += Pipe.Read( (amount__)( Wanted - Amount ), Buffer + Amount );
	}
	else
		if ( Pipe.Amount() )
			Amount = (flw::amount__)Pipe.Read( (amount__)Wanted, Buffer );

	return Amount;
}

static flw::amount__ Put_(
	pipe___ &Pipe,
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	flw::amount__ Amount = 0;

	if ( Minimum )
	{
		while ( Amount < Minimum )
			Amount += Pipe.Write( Buffer + Amount, (amount__)( Wanted - Amount ), Synchronization );
	}
	else if ( !Pipe.Amount() )
		Amount = (flw::amount__)Pipe.Write( Buffer, (amount__)Wanted, Synchronization );

	return Amount;
}

flw::amount__ pip::pipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Pipe_, Minimum, Buffer, Wanted );
}

flw::amount__ pip::pipe_ioflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Pipe_, Buffer, Wanted, Minimum, Synchronization );
}

flw::amount__ pip::master_bipipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Bipipe_->Slave, Minimum, Buffer, Wanted );
}

flw::amount__ pip::master_bipipe_ioflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Bipipe_->Master, Buffer, Wanted, Minimum, Synchronization );
}


flw::amount__ pip::slave_bipipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Bipipe_->Master, Minimum, Buffer, Wanted );
}

flw::amount__ pip::slave_bipipe_ioflow___::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Bipipe_->Slave, Buffer, Wanted, Minimum, Synchronization );
}

Q37_GCTOR( pip )
{
#ifdef CPE__UNIX
	signal( SIGPIPE, SIG_IGN );
#endif
}

