/*
	'fblfrd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblfrd' header file ('fblfrd.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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



//	$Id: fblfrd.cpp,v 1.8 2013/04/14 14:37:22 csimon Exp $

#define FBLFRD__COMPILATION

#include "fblfrd.h"

class fblfrdtutor
: public ttr_tutor
{
public:
	fblfrdtutor( void )
	: ttr_tutor( FBLFRD_NAME )
	{
#ifdef FBLFRD_DBG
		Version = FBLFRD_VERSION "\b\bD $";
#else
		Version = FBLFRD_VERSION;
#endif
		Owner = FBLFRD_OWNER;
		Date = "$Date: 2013/04/14 14:37:22 $";
	}
	virtual ~fblfrdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
			ERRDta();

		IncompatibilityInformations.Message.Append( Buffer );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRDta();

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
		ERRDta();

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
ERRProlog
	id8s Casts;
	fblfrd::string Name;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

void fblfrd::frontend___::GetBackendCommands_( void )
{
ERRProlog
	commands_details CommandsDetails;
	command_detail CommandDetail;
	id16s Commands;
	int i;
ERRBegin
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
		ERRDta();

	Commands.Recall( 0, Commands.Amount(), Commands_ );
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblfrdpersonnalization
: public fblfrdtutor
{
public:
	fblfrdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblfrdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static fblfrdpersonnalization Tutor;

ttr_tutor &FBLFRDTutor = Tutor;
