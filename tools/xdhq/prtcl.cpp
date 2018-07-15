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

#include "prtcl.h"

#include "stsfsm.h"

using namespace prtcl;

#define C( name ) case r##name : return #name ; break

const char *prtcl::GetLabel( eRequest Request )
{
	switch ( Request ) {
	C( Launch_1 );
	C( Ready_1 );
	default:
		qRFwk();
		break;
	}

	return NULL; // To avoid a 'warning'.
}

#undef C

namespace {
	stsfsm::wAutomat RequestAutomat_;

	void FillRequestAutomat_( void )
	{
		RequestAutomat_.Init();
		stsfsm::Fill<eRequest>( RequestAutomat_, r_amount, GetLabel );
	}
}

eRequest prtcl::GetRequest( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, RequestAutomat_, r_Undefined, r_amount );
}

eRequest prtcl::GetRequest( flw::iflow__ &Flow )
{
	return stsfsm::GetId( Flow, RequestAutomat_, r_Undefined, r_amount );
}

namespace {
	inline void Put_(
		const char *Identifier,
		flw::oflow__ &Flow )
	{
		Flow.Write( Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.
	};

	template <typename t> void Put_(
		t T,
		flw::oflow__ &Flow,
		const char *( *Get )( t T ) )
	{
		Put_( Get( T ), Flow );
	}
}

void prtcl::PutRequest(
	eRequest Request,
	flw::oflow__ &Flow )
{
	Put_( Request, Flow, GetLabel );
}

#define C( name ) case c##name : return #name ; break

const char *prtcl::GetLabel( eCommand Command )
{
	switch ( Command ) {
		C( StandBy_1 );
		C( Error_1 );
		C( Execute_1 );
		C( Alert_1 );
		C( Confirm_1 );
		C( SetLayout_1 );
		C( GetContents_1 );
		C( SetContents_1 );
		C( DressWidgets_1 );
		C( AddClasses_1 );
		C( RemoveClasses_1 );
		C( ToggleClasses_1 );
		C( EnableElements_1 );
		C( DisableElements_1 );
		C( SetAttribute_1 );
		C( GetAttribute_1 );
		C( RemoveAttribute_1 );
		C( SetProperty_1 );
		C( GetProperty_1 );
		C( Focus_1 );
		C( New );
	default:
		qRFwk();
		break;
	}

	return NULL; // To avoid a 'warning'.
}

#undef C

namespace {
	stsfsm::wAutomat CommandAutomat_;

	void FillCommandAutomat_( void )
	{
		CommandAutomat_.Init();
		stsfsm::Fill<eCommand>( CommandAutomat_, c_amount, GetLabel );
	}
}

eCommand prtcl::GetCommand( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, CommandAutomat_, c_Undefined, c_amount );
}

eCommand prtcl::GetCommand( flw::iflow__ &Flow )
{
	return stsfsm::GetId( Flow, CommandAutomat_, c_Undefined, c_amount );
}

void prtcl::SendCommand(
	eCommand Command,
	flw::oflow__ &Flow )
{
	Put_( Command, Flow, GetLabel );
}

namespace {
	void FillAutomats_( void )
	{
		FillRequestAutomat_();
		FillCommandAutomat_();
	}
}

qGCTOR( prtcl )
{
	FillAutomats_();
}

