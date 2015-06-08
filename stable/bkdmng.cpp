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

#define BKDMNG__COMPILATION

#include "bkdmng.h"

using namespace bkdmng;

static const char *PrimaryCommandName = bkdcmd::CommandsNames[bkdcmd::cGetCommand];

enum message__ {
	m_UnknowObjectType,
	m_UnknowObjectTypeName,
	m_UnknowCommandNameOrDescription,
	m_BadLanguage,
	m_BackendError,
	m__amount,
	m__OK,
};

#define CASE( m )\
	case m:\
		Message = #m + 1;\
		break

static const char *Label_( message__ MessageId )
{
	const char *Message = NULL;
	switch ( MessageId ) {
	CASE( m_UnknowObjectType );
	CASE( m_UnknowObjectTypeName );
	CASE( m_UnknowCommandNameOrDescription );
	CASE( m_BadLanguage );
	CASE( m_BackendError );
	default:
		ERRu();
		break;
	}

	return Message;
}

static const str::string_ &GetTranslation_(
	message__ Message,
	const bkdlcl::rack__ &Rack,
	str::string_ &Translation )
{
	Rack.GetTranslation( Label_( Message ), "NKDMNG_", Translation );

	return Translation;
}

static void ReportError_(
	message__ Message,
	const bkdlcl::rack__ &Rack,
	request_manager__ &Request )
{
qRH
	str::string Translation;
	STR_BUFFER___ Buffer;
qRB
	Translation.Init();

	Request.ReportError( GetTranslation_( Message, Rack, Translation ).Convert( Buffer ) );
qRR
qRT
qRE
}

void bkdmng::untyped_module::_Clean( void )
{
	epeios::row__ Row = Indexes.First();

	while ( Row != NONE ) {
		BKDMNGDelete( Indexes( Row ) );

		Row = Indexes.Next( Row );
	}
		
}


namespace {
	cast GetCastID_( const str::string_ &Name )
	{
		cast C = c_Unknow;
	qRH
		STR_BUFFER___ N;
	qRB
		C = bkdcst::GetID( Name.Convert( N  ) );
	qRR
	qRT
	qRE
		return C;
	}
}

struct master_data__ {
	bso::bool__ Deconnexion;
	void *UP;
};

void master_module::Handle_(
	index__ Index,
	request_manager__ &Requete,
	void *PU,
	log_functions__ &LogFunctions )
{
	command__ C;
	master_data__ &MasterData = *(master_data__ *)PU;
	ctn::E_CITEM( description_ ) Description;

	Description.Init( Descriptions );

	flw::Get( Requete.Input(), C );

	if (  C < Descriptions.Amount() )
	{
		Requete.Prepare( Description( C ).Casts );
		LogFunctions.Log( "", "MASTER", Description( C ).Name, false );
		(*(function__)UPs( C ))( *Backend(), *this, Index, C, Requete, MasterData.Deconnexion, MasterData.UP );
		LogFunctions.Log( "", "MASTER", Description( C ).Name, true );
	}
	else if ( C == BKDMNG_MASTER_COMMAND )
	{
		char Car;

		if ( ( Car = Requete.Input().Get() ) != 0 )
		{
qRH
			cast__ Cast;
			str::string S;
qRB
			Requete.Output().Put( 0 );	// No explanation message;
		
			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(Casts & languages)" ), false );
			
			// Casts.
			do {
				S.Init();

				S.Append( Car );

				while( ( Car = Requete.Input().Get() ) != 0 )
					S.Append( Car );

				Cast = GetCastID_( S );

				flw::Put( (id8__)Cast, Requete.Output() );

			} while ( ( Car = Requete.Input().Get() ) != 0 );

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(Casts & languages)" ), true );

qRR
qRT
qRE
		}
		else
		{
			epeios::row__ P = Descriptions.First();
			command__ C = BKDMNG_INVALID_COMMAND;

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(GetCommandCommand)" ), false );

			
			while ( ( P != NONE ) && ( Description( P ).Name != str::string( PrimaryCommandName ) ) )
				P = Descriptions.Next( P );

			if ( P != NONE )
				C = (command__)*P;

			Requete.Output().Put( 0 );	// No explanation message;

			flw::Put( C, Requete.Output() );

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(GetCommandCommand)" ), true );
		}
	}
	else
		ERRb();
}


// Donne la liste des identificateurs et des libells de types
static void GetTypesIDAndPrefixAndName_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
qRH
	xitem16 XItem;
	epeios::row__ P;
	type__ Type;
qRB
	xitems16_ &XItems = Requete.XItems16Out();

	P = Backend.Modules.First();

	while( P != NONE )
	{
		XItem.Init();

#ifdef BKDMNG_DEBUG
		if ( P > BKDMNG_TYPE_MAX )
			ERRc();
#endif
		Type = (type_t__)*P;

		XItem.ID( *Type );
		XItem.Values.Append( str::string( Backend.Module( Type ).Prefix() ) );
		XItem.Values.Append( str::string( Backend.Module( Type ).Name() ) );
		XItems.Append( XItem );
		P = Backend.Modules.Next( P );
	}

	Requete.Complete();
qRR
qRT
qRE
}

// Give the list of messages.

static void WriteCommandsIDAndName_(
	const descriptions_ &Descriptions,
	request_manager__ &Requete )
{
qRH
	item16 Item;
	ctn::E_CITEM( description_ ) Description;
	epeios::row__ P;
	command__ Command;
qRB
	items16_ &Items = Requete.Items16Out();

	P = Descriptions.First();

	Description.Init( Descriptions );

	while( P != NONE )
	{
		Item.Init();

#ifdef BKDMNG_DEBUG
		if ( P > BKDMNG_COMMAND_MAX )
			ERRC();
#endif
		Command = (command__)*P;

		Item.ID( Command );
		Item.Value = Description( P ).Name;
		
		if ( Item.Value( 0 ) != '_' )
			Items.Append( Item );
			
		P = Descriptions.Next( P );
	}
qRR
qRT
qRE
}


static void GetCommandsIDAndName_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	type__ Type = *Requete.Id16In();

	WriteCommandsIDAndName_( Backend.Module( Type ).Descriptions, Requete );

	Requete.Complete();
}

static inline void WriteParameters_(
	const description_ &Description,
	request_manager__ &Requete )
{
	Requete.Ids8Out() = Description.Casts;
}


static void WriteParameters_(
	const descriptions_ &Descriptions,
	command__ Command,
	request_manager__ &Requete )
{
	ctn::E_CITEM( description_ ) Description;

	Description.Init( Descriptions );

	WriteParameters_( Description( Command ), Requete );
}


static void GetParameters_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	type__ Type = *Requete.Id16In();
	id16__ Command = *Requete.Id16In();;

	WriteParameters_( Backend.Module( (type__)*Type ).Descriptions, *Command, Requete );

	Requete.Complete();
}

static void About_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	Requete.StringOut() = BKDRPL_PROTOCOL_VERSION;
	Requete.StringOut() = Backend.GetTargetLabel();
	Requete.StringOut() = Backend.GetAPIVersion();
	Requete.StringOut() = Backend.GetBackendInformations();
	Requete.StringOut() = Backend.GetPublisherInformations();
	Requete.Complete();
}

// Throw an user error (ERRu), for testing purpose.
static void ThrowUError_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	ERRu();

	Requete.Complete();
}

// Throw an itentional error (ERRi), for testing purpose.
static void ThrowIError_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	ERRi();

	Requete.Complete();
}

// Throw an itentional error (ERRi), for testing purpose.
static void ThrowUserDefinedError_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
qRH
	STR_BUFFER___ Buffer;
qRB
	Requete.ReportError( Requete.StringIn().Convert( Buffer ) );
qRR
qRT
qRE
}


// Retourne un nouvel objet.
static void GetNewObject_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Request,
	bso::bool__ &,
	void * )
{
qRH
	type__ T = BKDMNG_INVALID_TYPE;
	STR_BUFFER___ Buffer;
	object__ O;
qRB
	T = *Request.Id16In();

	if ( *T >= Backend.Modules.Amount() )
		ERRb();

	if ( T == BKDMNG_INVALID_TYPE )
		ERRb();

	O = Backend.New( (type__)*T );

	if ( O != BKDMNG_INVALID_TYPE )
		Request.ObjectOut() = O;
	else
		ReportError_( m_UnknowObjectType, Backend.LocaleRack(), Request );

	Request.Complete();
qRR
qRT
qRE
}

// Retourne l'identificateur correspondant  un type donn.
static void GetType_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Request,
	bso::bool__ &,
	void * )
{
qRH
	STR_BUFFER___ Buffer;
	type__ T = BKDMNG_INVALID_TYPE;
qRB
	const str::string_ &Type = Request.StringIn();
	T = Backend.Type( Type );

	if ( ( T != BKDMNG_INVALID_TYPE ) )
		Request.Id16Out() = *T;
	else
		ReportError_( m_UnknowObjectTypeName, Backend.LocaleRack(), Request );

	Request.Complete();
qRR
qRT
qRE
}
#if 0
// Returns all the raw messages.
static void GetRawMessages_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	epeios::row__ Row = NONE;
	strings_ &Messages = Requete.StringsOut();
	Messages = Backend.GetMasterRawMessages();

	Row = Backend.Modules.First();

	while ( Row != NONE ) {
		Backend.Modules( Row )->GetRawMessages( Messages );

		Row = Backend.Modules.Next( Row );
	}
}
#endif

// Supprime un objet.
static void RemoveObject_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	object__ O = Requete.ObjectIn();

/*	if ( !Backend.Valide( O ) )
		ERRb();
*/
	Backend.Delete( O );

	Requete.Complete();
}

static void FillCommands_(
	backend_ &Backend,
	type__ Type,
	const commands_details_ &CommandsDetails,
	ids16_ &Commands )
{
qRH
	id16__ Command;
	epeios::row__ Position = CommandsDetails.First();
	ctn::E_CITEM( command_detail_ ) CommandDetail;
	description Description;
qRB
	CommandDetail.Init( CommandsDetails );

	while( Position != NONE )
	{
		Description.Init();

		Description.Name = CommandDetail( Position ).Name;
		Description.Casts = CommandDetail( Position ).Casts;

		if ( ( Command = Backend.Command( Type, Description ) ) == BKDMNG_INVALID_COMMAND )
			ERRb();

		Commands.Append( Command );

		Position = CommandsDetails.Next( Position );
	}
qRR
qRT
qRE
}



// Retourne l'identificateur de type et les identificateurs de commande demand.
static void GetTypeAndCommands_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	const string_ &Name = Requete.StringIn();
	const commands_details_ &CommandsDetails = Requete.CommandsDetailsIn();
	type__ &Type = (type__ &)Requete.Id16Out();
	ids16_ &Commands = Requete.Ids16Out();

	if ( ( Type = Backend.Type( Name ) ) == BKDMNG_INVALID_TYPE )
		ERRb();

	Commands.Init();

	FillCommands_( Backend, Type, CommandsDetails, Commands );

	Requete.Complete();
}


// Retourne l'identificateur d'une commande donne pour un type donn.
static void GetCommand_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Request,
	bso::bool__ &,
	void * )
{
qRH
	description Description;
	command__ Command;
	STR_BUFFER___ Buffer;
qRB
	type__ Type = *Request.Id16In();

	Description.Init();
	Description.Name  = Request.StringIn();
	Description.Casts = Request.Ids8In();

/*	if ( !Backend.Valide( T ) )
		if ( T() != BKDEND9_TYPE_MAITRE )
			ERRb();
*/
	Command = Backend.Command( *Type, Description );

	if ( ( Command != BKDMNG_INVALID_COMMAND ) )
		Request.Id16Out() = Command;
	else
		ReportError_( m_UnknowCommandNameOrDescription, Backend.LocaleRack(), Request );

	Request.Complete();
qRR
qRT
qRE
}


// Retourne l'identificateur des commandes demandes pour un type donn.
static void GetCommands_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
	type__ Type = *Requete.Id16In();
	const commands_details_ &CommandsDetails = Requete.CommandsDetailsIn();
	ids16_ &Commands = Requete.Ids16Out();

	FillCommands_( Backend, Type, CommandsDetails, Commands );

	Requete.Complete();
}


// Deconnection
static void Disconnect_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &Deconnexion,
	void * )
{
	Deconnexion = true;
}

// Return the current language.
static void GetLanguage_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &Deconnexion,
	void * )
{
	Requete.StringOut() = Backend.LocaleRack().Language;

	Requete.Complete();
}

// Set the current language;
static void SetLanguage_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Request,
	bso::bool__ &Deconnexion,
	void * )
{
qRH
	STR_BUFFER___ Buffer;
qRB
	const str::string_ &Language = Request.StringIn();

	if ( Language.Amount() == 0 )
			ReportError_( m_BadLanguage, Backend.LocaleRack(), Request );
	else
		Backend.SetLanguage( Language.Convert( Buffer ) );

	Request.Complete();
qRR
qRT
qRE
}

#define ADD( I )	Backend.Add( bkdcmd::CommandsNames[bkdcmd::c##I], ::I##_, bkdcmd::CommandsParameters[bkdcmd::c##I] )

namespace bkdmng {
	// Initialisation avec rattachement  l'interface 'Frontend'.
	void master_module::Init( bkdmng::backend_ &Backend )
	{
		untyped_module::Init( NULL, NULL );
		Backend_ = &Backend;
#if 0
		RawMessages.Init();
#endif
		// Throw an user error (ERRu), for testing purpose.
		ADD( ThrowUError );

		// Throw a throw error (ERRt), for testing purpose.
		ADD( ThrowIError );

		// Gnration d'une erreur utilsateur.
		ADD( ThrowUserDefinedError );

		// Create a new object.
		ADD( GetNewObject );

		// Give the type corresponding to the given name.
		ADD( GetType );
#if 0
		// Give all the raw messages.
		ADD( GetRawMessages );
#endif
		// Give the command of the given object type and command name.
		ADD( GetCommand );

		// Give the commands of the given object type and command names.
		ADD( GetCommands );

		// Give the type and the commands of the given type name, commands names and casts.
		ADD( GetTypeAndCommands );

		// Remove the given object.
		ADD( RemoveObject );

		// Informations au sujet du 'backend' et du 'publisher'.
		ADD( About );

		// Disconnection.
		ADD( Disconnect );

		// The id and name of the supported object types.
		ADD( GetTypesIDAndPrefixAndName );

		// The id and name of the commands of an object type.
		ADD( GetCommandsIDAndName );

		// The parameters (cast) of a command of an object type.
		ADD( GetParameters );

		// Return the current language.
		ADD( GetLanguage );

		// Set the current language.
		ADD( SetLanguage );
#if 0
		for ( int i = 0; i < m__amount ; i++ )
			Backend.GetMasterRawMessages().Append( str::string( GetRawMessage_( (message__)i ) ) );
#endif
	}

	type__ backend::Type( const str::string_ &Name ) const
	{
		epeios::row__ C = Modules.First();

		while ( ( C != NONE ) && ( str::string( Modules(C)->Name() ) != Name )  )
			C = Modules.Next( C );

		if ( C == NONE )
			C = BKDMNG_INVALID_TYPE;
		else if ( *C > BKDMNG_INVALID_TYPE )
			ERRl();

		return (type_t__)*C;
	}

bso::bool__ bkdmng::backend_::_TestCompatibility(
	flw::ioflow__ &Flow,
	const char *APIVersion,
	const char *MessageLabel,
	const char *URLLabel )
{
	bso::bool__ Success = true;
qRH
	char RemoteTargetLabel[10];
	char Language[10];
	char RemoteProtocolVersion[10];
	char RemoteAPIVersion[10];
	STR_BUFFER___ Buffer;
qRB
	if ( !flw::GetString( Flow, Language, sizeof( Language ) ) )
		ERRf();

	if ( !flw::GetString( Flow, RemoteProtocolVersion, sizeof( RemoteProtocolVersion ) ) )
		ERRf();

	if ( !flw::GetString( Flow, RemoteTargetLabel, sizeof( RemoteTargetLabel ) ) )
		ERRf();

	if ( !flw::GetString( Flow, RemoteAPIVersion, sizeof( RemoteAPIVersion ) ) )
		ERRf();

	Flow.Dismiss();

	if ( RemoteTargetLabel[0] && strcmp( RemoteTargetLabel, _TargetLabel ) )
		ERRl();	// Pas encore trait.

	if ( strcmp( RemoteProtocolVersion, BKDRPL_PROTOCOL_VERSION )
		 || ( RemoteAPIVersion[0] && strcmp( RemoteAPIVersion, APIVersion ) ) ) {
		Success = false;


		Flow.Put( -1 );

		SetLanguage( Language );

		flw::PutString( _LocaleRack.GetTranslation( MessageLabel, "", Buffer ), Flow );
		flw::PutString( _LocaleRack.GetTranslation( URLLabel, "", Buffer ), Flow );

	} else
		Flow.Put( 0 );

	Flow.Commit();
qRR
qRT
qRE
	return Success;
}


bso::bool__ backend_::Handle(
	request_manager__ &Request,
	void *PU,
	log_functions__ &LogFunctions )
{
	master_data__ MasterData;
qRH
	object__ O;
	err::buffer__ ErrorBuffer;
qRB
	MasterData.Deconnexion = false;
	MasterData.UP = PU;

	flw::Get( Request.Input(), O );

	if ( ( !Links.Exists( O ) ) && ( O != BKDMNG_MASTER_OBJECT ) )
		ERRb();

	if ( O != BKDMNG_MASTER_OBJECT ) {
		Module_( O ).Handle( Index_( O ), Request, PU, LogFunctions );
	} else
		Master_.Handle( (index__)0, Request, &MasterData, LogFunctions );

qRR
	const char *ErrMsg = err::Message( ErrorBuffer );

	ERRRst();

	Request.ReportBackendError(	ErrMsg );
qRT
	Request.Complete();
qRE
	return !MasterData.Deconnexion;
}
#if 0	
	epeios::row__ untyped_module::Add(
		const char *Name,
		const void *UP,
		... )
	{
		epeios::row__ P;
	qRH
		va_list L;
		description Description;
		bkdcst::cast Cast;
	qRB
		Description.Init( Name );
		
		va_start( L, UP );

		while ( ( Cast = va_arg( L, bkdcst::cast ) ) != cEnd )
			Description.Add( Cast );
			
		Description.Add( Cast );
		
		while( ( Cast = va_arg( L, bkdcst::cast ) ) != cEnd )
			Description.Add( Cast );
			
		va_end( L );
		
		P = Descriptions.Add( Description );
		
		if ( UPs.Add( UP ) != P )
			ERRc();
	qRR
	qRT
	qRE
		return P;
	}
#endif
	
