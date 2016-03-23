/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define PRXYBASE__COMPILATION

#include "prxybase.h"

#include "stsfsm.h"

using namespace prxybase;

const char *prxybase::GetLabel( eRequest Request )
{
	switch ( Request ) {
	case rPlug:
		return "Plug";
		break;
	default:
		qRFwk();
		break;
	}

	return NULL; // To avoid a 'warning'.
}

namespace {
	stsfsm::wAutomat RequestAutomat_;

	void FillRequestAutomat_( void )
	{
		RequestAutomat_.Init();
		stsfsm::Fill<eRequest>( RequestAutomat_, r_amount, GetLabel );
	}
}

eRequest prxybase::GetRequest( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, RequestAutomat_, r_Undefined, r_amount );
}

const char *prxybase::GetLabel( eAnswer Answer )
{
	switch ( Answer ) {
	case aPlugged:
		return "Plugged";
		break;
	default:
		qRFwk();
		break;
	}

	return NULL; // To avoid a 'warning'.
}

namespace {
	stsfsm::wAutomat AnswerAutomat_;

	void FillAnswerAutomat_( void )
	{
		AnswerAutomat_.Init();
		stsfsm::Fill<eAnswer>( AnswerAutomat_, a_amount, GetLabel );
	}
}

eAnswer prxybase::GetAnswer( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, AnswerAutomat_, a_Undefined, a_amount );
}

const char *prxybase::GetLabel( eType Type )
{
	switch ( Type ) {
	case tClient:
		return "Client";
		break;
	case tServer:
		return "Server";
		break;
	default:
		qRFwk();
		break;
	}

	return NULL; // To avoid a 'warning'.
}

namespace {
	stsfsm::wAutomat TypeAutomat_;

	void FillTypeAutomat_( void )
	{
		TypeAutomat_.Init();
		stsfsm::Fill<eType>( TypeAutomat_, t_amount, GetLabel );
	}
}

eType prxybase::GetType( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, TypeAutomat_, t_Undefined, t_amount );
}

namespace {
	void FillAutomats_( void ) {
		FillRequestAutomat_();
		FillAnswerAutomat_();
		FillTypeAutomat_();
	}
}

eRequest prxybase::GetRequest( flw::iflow__ &Flow )
{
	return stsfsm::GetId( Flow, RequestAutomat_, r_Undefined, r_amount );
}

eAnswer prxybase::GetAnswer( flw::iflow__ &Flow )
{
	return stsfsm::GetId( Flow, AnswerAutomat_, a_Undefined, a_amount );
}

eType prxybase::GetType( flw::iflow__ &Flow )
{
	return stsfsm::GetId( Flow, TypeAutomat_, t_Undefined, t_amount );
}

namespace {
	inline const str::dString &GetString_(
		flw::iflow__ &Flow,
		str::dString &String )
	{
		bso::char__ C = 0;

		while ( ( C = Flow.Get() ) != 0 )
			String.Append( C );

		return String;
	}
}


const str::dString &prxybase::GetId(
	flw::iflow__ &Flow,
	str::dString &Id )
{
	return GetString_( Flow, Id );
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

void prxybase::PutRequest(
	eRequest Request,
	flw::oflow__ &Flow )
{
	Put_( Request, Flow, GetLabel );
}

void prxybase::PutAnswer(
	eAnswer Answer,
	flw::oflow__ &Flow )
{
	Put_( Answer, Flow, GetLabel );
}

void prxybase::PutType(
	eType Type,
	flw::oflow__ &Flow )
{
	Put_( Type, Flow, GetLabel );
}



qGCTOR( prxybase )
{
	FillAutomats_();
}
