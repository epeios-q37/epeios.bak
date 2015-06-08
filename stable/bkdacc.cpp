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



//	$Id: bkdacc.cpp,v 1.5 2011/06/11 18:15:59 csimon Exp $

#define BKDACC__COMPILATION

#include "bkdacc.h"

using namespace bkdacc;

bso::bool__ bkdacc::backend_access___::_TestCompatibility(
	const char *Language,
	const char *TargetLabel,
	const char *APIVersion,
	flw::ioflow__ &Flow,
	str::string_ &Message,
	str::string_ &URL )
{
	bso::bool__ Success = true;
	char Buffer[100];

	flw::PutString( Language, Flow );
	flw::PutString( BKDRPL_PROTOCOL_VERSION, Flow );
	flw::PutString( TargetLabel, Flow );
	flw::PutString( APIVersion, Flow );

	Flow.Commit();

	if ( Flow.Get() != 0  ) {

		Success = false;

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRb();

		Message.Append( Buffer );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRb();

		URL.Append( Buffer );
	}

	Flow.Dismiss();

	return Success;
}


bso::bool__ bkdacc::backend_access___::TestBackendCasts_()
{
	int i;

	PushHeader( BKDACC_MASTER_OBJECT, BKDACC_MASTER_COMMAND );

	for( i = 0; i < bkdcst::c_amount; i++ )
		flw::PutString( bkdcst::CastsNames[i], *Channel_ );

	Channel_->Put( 0 );

	_SendAndTest();

	for( i = 0; i < bkdcst::c_amount; i++ )
		if ( i != Channel_->Get() )
			return false;

	if ( Channel_->Get() != bkdcst::cEnd )
		return false;

	Channel_->Dismiss();

	return true;
}

command__ bkdacc::backend_access___::GetBackendDefaultCommand_()
{
	command__ DefaultCommand;

	PushHeader( BKDACC_MASTER_OBJECT, BKDACC_MASTER_COMMAND );
	
	Channel_->Put( 0 );	// End of request

	_SendAndTest();

	flw::Get( *Channel_, DefaultCommand );

	if ( Channel_->Get() != bkdcst::cEnd )
		ERRb();

	Channel_->Dismiss();

	return DefaultCommand;
}

namespace {
	void FillCasts_(
		const bkdcst::cast *Source,
		ids8_ &Dest )
	{
		int i = 0;

		do
			Dest.Append( Source[i] );
		while( Source[i++] != bkdcst::cEnd );

		while( Source[i] != bkdcst::cEnd )
			Dest.Append( Source[i++] );
	}
}

void bkdacc::backend_access___::GetGetCommandsCommand_( command__ DefaultCommand )
{
qRH
	ids8 Casts;
	bkdacc::string Name;
qRB
	Casts.Init();
	Name.Init( bkdcmd::CommandsNames[bkdcmd::cGetCommands] );

	FillCasts_( bkdcmd::CommandsParameters[bkdcmd::cGetCommands], Casts );

	PushHeader( BKDACC_MASTER_OBJECT, DefaultCommand );
	Id16In( BKDACC_MASTER_TYPE );	// MasterType;
	StringIn( Name );
	Ids8In( Casts );

	EndOfInParameters();

	Id16Out( Commands_[bkdcmd::cGetCommands] );

	_Handle();
qRR
qRT
qRE
}

void bkdacc::backend_access___::GetBackendCommands_( void )
{
qRH
	commands_details CommandsDetails;
	command_detail CommandDetail;
	ids16 Commands;
	int i;
qRB
	CommandsDetails.Init();

	for( i = 0; i < bkdcmd::c_amount; i++ ) {
		CommandDetail.Init();
		CommandDetail.Name = bkdcmd::CommandsNames[i];
		FillCasts_( bkdcmd::CommandsParameters[i], CommandDetail.Casts );
		CommandsDetails.Append( CommandDetail );
	}

	Commands.Init();
	GetCommands( BKDACC_MASTER_COMMAND, CommandsDetails, Commands );

	Commands.Recall( 0, Commands.Amount(), Commands_ );
qRR
qRT
qRE
}
