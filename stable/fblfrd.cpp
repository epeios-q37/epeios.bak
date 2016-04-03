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

#define FBLFRD__COMPILATION

#include "fblfrd.h"

#include "cio.h"

using namespace fblfrd;

bso::bool__ fblfrd::frontend___::_TestCompatibility(
	const char *Language,
	const compatibility_informations__ &CompatibilityInformations,
	flw::ioflow__ &Flow,
	incompatibility_informations_ &IncompatibilityInformations )
{
	bso::bool__ Success = true;
	char Buffer[100];

	flw::PutString( Language, Flow );
	flw::PutString( FBLOVL_PROTOCOL_VERSION, Flow );
	flw::PutString( CompatibilityInformations.BackendLabel, Flow );
	flw::PutString( CompatibilityInformations.APIVersion, Flow );

	Flow.Commit();

	if ( Flow.Get() != 0  ) {

		Success = false;

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			qRFwk();

		IncompatibilityInformations.Message.Append( Buffer );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			qRFwk();

		IncompatibilityInformations.URL.Append( Buffer );
	}

	Flow.Dismiss();

	return Success;
}

bso::bool__ fblfrd::frontend___::TestBackendCasts_()
{
	int i;

	PushHeader( FBLFRD_MASTER_OBJECT, FBLFRD_MASTER_COMMAND );

	for( i = 0; i < fblcst::c_amount; i++ )
		flw::PutString( fblcst::CastsNames[i], *Channel_ );

	Channel_->Put( 0 );

	_SendAndTest();

	for( i = 0; i < fblcst::c_amount; i++ )
		if ( i != Channel_->Get() )
			return false;

	if ( Channel_->Get() != fblcst::cEnd )
		return false;

	Channel_->Dismiss();

	return true;
}

command__ fblfrd::frontend___::GetBackendDefaultCommand_()
{
	command__ DefaultCommand;

	PushHeader( FBLFRD_MASTER_OBJECT, FBLFRD_MASTER_COMMAND );
	
	Channel_->Put( 0 );	// End of request

	_SendAndTest();

	flw::Get( *Channel_, DefaultCommand );

	if ( Channel_->Get() != fblcst::cEnd )
		qRFwk();

	Channel_->Dismiss();

	return DefaultCommand;
}

namespace {
	template <typename type> void FillCasts_(
		const fblcst::cast__ *Source,
		bch::E_BUNCH_( type ) &Dest )
	{
		int i = 0;

		do
			Dest.Append( Source[i] );
		while( Source[i++] != fblcst::cEnd );

		while( Source[i] != fblcst::cEnd )
			Dest.Append( Source[i++] );
	}
}

void fblfrd::frontend___::GetGetCommandsCommand_( command__ DefaultCommand )
{
qRH
	id8s Casts;
	fblfrd::string Name;
qRB
	Casts.Init();
	Name.Init( fblcmd::CommandsNames[fblcmd::cGetCommands] );

	FillCasts_( fblcmd::CommandsParameters[fblcmd::cGetCommands], Casts );

	PushHeader( FBLFRD_MASTER_OBJECT, DefaultCommand );
	Id16In( FBLFRD_MASTER_TYPE );	// MasterType;
	StringIn( Name );
	Id8sIn( Casts );

	EndOfInParameters();

	Id16Out( Commands_[fblcmd::cGetCommands] );

	_Handle();
qRR
qRT
qRE
}

void fblfrd::frontend___::GetBackendCommands_( void )
{
qRH
	commands_details CommandsDetails;
	command_detail CommandDetail;
	id16s Commands;
	int i;
qRB
	CommandsDetails.Init();

	for( i = 0; i < fblcmd::c_amount; i++ ) {
		CommandDetail.Init();
		CommandDetail.Name = fblcmd::CommandsNames[i];
		FillCasts_( fblcmd::CommandsParameters[i], CommandDetail.Casts );
		CommandsDetails.Append( CommandDetail );
	}

	Commands.Init();
	GetCommands( FBLFRD_MASTER_COMMAND, CommandsDetails, Commands );

	if ( CommandsDetails.Amount() != Commands.Amount() )
		qRFwk();

	Commands.Recall( 0, Commands.Amount(), Commands_ );
qRR
qRT
qRE
}

void fblfrd::sDefaultReportingCallback::FBLFRDReport(
	fblovl::reply__ Reply,
	const char *Message )
{
	switch ( Reply ) {
	case fblovl::rRequestError:
	case fblovl::rSoftwareError:
		cio::CErr << Message << txf::nl << txf::commit;
		break;
	case fblovl::rDisconnected:
		qRVct();
		break;
	default:
		qRGnr();
		break;
	}
}
