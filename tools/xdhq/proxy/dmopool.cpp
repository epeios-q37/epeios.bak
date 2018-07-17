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

# include "dmopool.h"

using namespace dmopool;

#include "bch.h"
#include "csdbns.h"
#include "mtk.h"

namespace {
	mtx::rHandler Mutex_ = mtx::UndefinedHandler;
	bch::qBUNCHwl( sck::sSocket ) Sockets_;
	csdbns::rListener Listener_;

	void Routine_( void * )
	{
		sck::sSocket Socket = sck::Undefined;
		const char *IP;

		while ( true ) {
			Socket = sck::Undefined;

			Socket = Listener_.GetConnection( IP );

			mtx::Lock( Mutex_ );

			Sockets_.Push( Socket );

			mtx::Unlock( Mutex_ );
		}



	}

	void Init_( void )
	{
		Mutex_ = mtx::Create();
		Sockets_.Init();
		Listener_.Init( 51000 );

		mtk::RawLaunch( Routine_, NULL );
	}
}

sck::sSocket dmopool::GetConnexion( const str::dString &Token )
{
	sck::sSocket Socket = sck::Undefined;

	mtx::Lock( Mutex_ );

	Socket = Sockets_.Pop();

	mtx::Unlock( Mutex_ );

	return Socket;
}

qGCTOR( dmopool )
{
	Init_();
}

