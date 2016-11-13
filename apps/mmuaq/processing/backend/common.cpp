/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

using namespace common;

 #define C( name )\
	case m##name:\
		return #name;\
		break


const char *common::GetLabel( eMessage Message )
{
	switch ( Message ){
	C( TestMessage );
	C( NotLoggedIn );
	C( AgentWithSuchNameExists );
/*
	C(  );
*/
	default:
		qRGnr();
		break;
	}

	return NULL;
}

#undef C

namespace {
	class rRack_ {
	private:
		muaacc::rRack Accounts_;
	public:
		rAuthentication Authentication;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Authentication, Accounts_ );
		}
		void Init( void )
		{
			tol::Init( Accounts_ );
			Authentication.Init( NULL, Accounts_ );
		}
	} Rack_;

	bso::bool__ IsInitialized_ = false;
}

void common::rAuthentication::Init(
	const char *Identifier,
	muaacc::lAccounts &Accounts )
{
	Retriever_.Init();
	sclmisc::Plug( muaplg::AuthenticationPluginTarget, Identifier, Retriever_ );

	lAuthentication_::Init( P_(), Accounts );
}

void common::Initialize( void )
{
qRH
qRB
	if ( IsInitialized() )
		qRGnr();

	Rack_.Init();

	IsInitialized_ = true;
qRR
qRT
qRE
}

bso::bool__ common::IsInitialized( void )
{
	return IsInitialized_;
}

muaacc::lAuthentication &common::Authentication( void )
{
	return Rack_.Authentication;
}

