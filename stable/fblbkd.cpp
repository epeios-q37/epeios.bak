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

#define FBLBKD__COMPILATION

#include "fblbkd.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "fblcmd.h"
#include "fblovl.h"

using namespace fblbkd;

using namespace fbltyp;
using namespace fblbrd;

static const char *PrimaryCommandName = fblcmd::CommandsNames[fblcmd::cGetCommand];

enum message__ {
	m_UnknowObjectType,
	m_UnknowObjectTypeName,
	m_UnknowCommandNameOrDescription,
	m_BadLanguage,
	m_BackendError,
	m__amount,
	m__OK,
};

#define CASE( l )\
	case m_##l:\
	return FBLBKD_NAME "_" #l;\
	break

static const char *GetLabel_( message__ MessageId )
{
	const char *Message = NULL;
	switch ( MessageId ) {
	CASE( UnknowObjectType );
	CASE( UnknowObjectTypeName );
	CASE( UnknowCommandNameOrDescription );
	CASE( BadLanguage );
	CASE( BackendError );
	default:
		qRFwk();
		break;
	}

	return Message;
}

static void Report_(
	message__ Message,
	const lcl::locale_ &Locale,
	const char *Language,
	request__ &Request )
{
qRH
	str::string Translation;
	TOL_CBUFFER___ Buffer;
qRB
	Translation.Init();

	Request.ReportRequestError( Locale.GetTranslation( GetLabel_( Message ), Language, Translation ).Convert( Buffer ) );
qRR
qRT
qRE
}

void fblbkd::untyped_module::_Clean( void )
{
	sdr::row__ Row = Indexes.First();

	while ( Row != qNIL ) {
		FBLBKDDelete( Indexes( Row ) );

		Row = Indexes.Next( Row );
	}
		
}


namespace {
	cast__ GetCastID_( const str::string_ &Name )
	{
		cast__ C = c_Undefined;
	qRH
		TOL_CBUFFER___ N;
	qRB
		C = fblcst::GetID( Name.Convert( N  ) );
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
	request__ &Requete,
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
	else if ( C == FBLBKD_MASTER_COMMAND )
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

				flw::Put( (fbltyp::id8__)Cast, Requete.Output() );

			} while ( ( Car = Requete.Input().Get() ) != 0 );

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(Casts & languages)" ), true );

qRR
qRT
qRE
		}
		else
		{
			sdr::row__ P = Descriptions.First();
			command__ C = FBLBKD_INVALID_COMMAND;

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(GetCommandCommand)" ), false );

			
			while ( ( P != qNIL ) && ( Description( P ).Name != str::string( PrimaryCommandName ) ) )
				P = Descriptions.Next( P );

			if ( P != qNIL )
				C = (command__)*P;

			Requete.Output().Put( 0 );	// No explanation message;

			flw::Put( C, Requete.Output() );

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(GetCommandCommand)" ), true );
		}
	}
	else
		qRFwk();
}


// Donne la liste des identificateurs et des libells de types
static void GetTypesIDAndPrefixAndName_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
qRH
	xitem16 XItem;
	sdr::row__ P;
	type__ Type;
qRB
	xitem16s_ &XItems = Requete.XItem16sOut();

	P = Backend.Modules.First();

	while( P != qNIL )
	{
		XItem.Init();

#ifdef FBLBKD_DEBUG
		if ( P > FBLBKD_TYPE_MAX )
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
	request__ &Requete )
{
qRH
	item16 Item;
	ctn::E_CITEM( description_ ) Description;
	sdr::row__ P;
	command__ Command;
qRB
	item16s_ &Items = Requete.Item16sOut();

	P = Descriptions.First();

	Description.Init( Descriptions );

	while( P != qNIL )
	{
		Item.Init();

#ifdef FBLBKD_DEBUG
		if ( P > FBLBKD_COMMAND_MAX )
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
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	type__ Type = Requete.Id16In();

	WriteCommandsIDAndName_( Backend.Module( Type ).Descriptions, Requete );

	Requete.Complete();
}

static inline void WriteParameters_(
	const description_ &Description,
	request__ &Requete )
{
	Requete.Id8sOut() = Description.Casts;
}


static void WriteParameters_(
	const descriptions_ &Descriptions,
	command__ Command,
	request__ &Requete )
{
	ctn::E_CITEM( description_ ) Description;

	Description.Init( Descriptions );

	WriteParameters_( Description( Command ), Requete );
}


static void GetParameters_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	type__ Type = Requete.Id16In();
	id16__ Command = Requete.Id16In();;

	WriteParameters_( Backend.Module( (type__)*Type ).Descriptions, *Command, Requete );

	Requete.Complete();
}

static void About_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	Requete.StringOut() = FBLOVL_PROTOCOL_VERSION;
	Requete.StringOut() = Backend.GetBackendLabel();
	Requete.StringOut() = Backend.GetAPIVersion();
	Requete.StringOut() = Backend.GetExtendedBackendInformations();
	Requete.StringOut() = Backend.GetBackendCopyright();
	Requete.StringOut() = Backend.GetSoftwareInformations();
	Requete.Complete();
}

// Throw an framework error (ERRFwk), for testing purpose.
static void ThrowERRFwk_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	qRFwk();

	Requete.Complete();
}

// Throw an itentional error (ERRFree), for testing purpose.
static void ThrowERRFree_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	qRFree();

	Requete.Complete();
}

static void TestNotification_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Requete.ReportRequestError( Requete.StringIn().Convert( Buffer ) );
qRR
qRT
qRE
}


// Retourne un nouvel objet.
static void GetNewObject_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Request,
	bso::bool__ &,
	void * )
{
qRH
	type__ T = FBLBKD_INVALID_TYPE;
	TOL_CBUFFER___ Buffer;
	object__ O;
qRB
	T = Request.Id16In();

	if ( *T >= Backend.Modules.Amount() )
		qRFwk();

	if ( T == FBLBKD_INVALID_TYPE )
		qRFwk();

	O = Backend.New( (type__)*T );

	if ( O != FBLBKD_INVALID_TYPE )
		Request.ObjectOut() = O;
	else
		Report_( m_UnknowObjectType, Backend.Locale(), Backend.Language(), Request );

	Request.Complete();
qRR
qRT
qRE
}

// Retourne l'identificateur correspondant  un type donn.
static void GetType_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Request,
	bso::bool__ &,
	void * )
{
qRH
	TOL_CBUFFER___ Buffer;
	type__ T = FBLBKD_INVALID_TYPE;
qRB
	const str::string_ &Type = Request.StringIn();
	T = Backend.Type( Type );

	if ( ( T != FBLBKD_INVALID_TYPE ) )
		Request.Id16Out() = *T;
	else
		Report_( m_UnknowObjectTypeName, Backend.Locale(), Backend.Language(), Request );

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
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	mdr::row__ Row = qNIL;
	strings_ &Messages = Requete.StringsOut();
	Messages = Backend.GetMasterRawMessages();

	Row = Backend.Modules.First();

	while ( Row != qNIL ) {
		Backend.Modules( Row )->GetRawMessages( Messages );

		Row = Backend.Modules.Next( Row );
	}
}
#endif

// Supprime un objet.
static void RemoveObject_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
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
	backend___ &Backend,
	type__ Type,
	const commands_details_ &CommandsDetails,
	id16s_t_ &Commands )
{
qRH
	id16_t__ Command;
	sdr::row__ Position = qNIL;
	description Description;
	ctn::E_CITEM( command_detail_ ) CommandDetail;
qRB
	CommandDetail.Init( CommandsDetails );

	Position = CommandsDetails.First();

	while( Position != qNIL )
	{
		Description.Init();

		Description.Casts = CommandDetail( Position ).Casts;
		Description.Name = CommandDetail( Position ).Name;

		if ( ( Command = Backend.Command( Type, Description ) ) == FBLBKD_INVALID_COMMAND )
			qRFwk();

		Commands.Append( Command );

		Position = CommandsDetails.Next( Position );
	}
qRR
qRT
qRE
}



// Retourne l'identificateur de type et les identificateurs de commande demand.
static void GetTypeAndCommands_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	const string_ &Name = Requete.StringIn();
	const commands_details_ &CommandsDetails = Requete.CommandsDetailsIn();
	type__ &Type = (type__ &)Requete.Id16Out();
	id16s_t_ &Commands = Requete.Id16sOut();

	if ( ( Type = Backend.Type( Name ) ) == FBLBKD_INVALID_TYPE )
		qRFwk();

	Commands.Init();

	FillCommands_( Backend, Type, CommandsDetails, Commands );

	Requete.Complete();
}


// Retourne l'identificateur d'une commande donne pour un type donn.
static void GetCommand_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Request,
	bso::bool__ &,
	void * )
{
qRH
	description Description;
	command__ Command;
	TOL_CBUFFER___ Buffer;
qRB
	type__ Type = Request.Id16In();

	Description.Init();
	Description.Name  = Request.StringIn();
	Description.Casts = Request.Id8sIn();

/*	if ( !Backend.Valide( T ) )
		if ( T() != BKDEND9_TYPE_MAITRE )
			ERRb();
*/
	Command = Backend.Command( *Type, Description );

	if ( ( Command != FBLBKD_INVALID_COMMAND ) )
		Request.Id16Out() = Command;
	else
		Report_( m_UnknowCommandNameOrDescription, Backend.Locale(), Backend.Language(), Request );

	Request.Complete();
qRR
qRT
qRE
}


// Retourne l'identificateur des commandes demandes pour un type donn.
static void GetCommands_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &,
	void * )
{
	type__ Type = Requete.Id16In();
	const commands_details_ &CommandsDetails = Requete.CommandsDetailsIn();
	id16s_t_ &Commands = Requete.Id16sOut();

	FillCommands_( Backend, Type, CommandsDetails, Commands );

	Requete.Complete();
}


// Deconnection
static void Disconnect_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &Deconnexion,
	void * )
{
	Deconnexion = true;

	Requete.Complete();
}

// Return the current language.
static void GetLanguage_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Requete,
	bso::bool__ &Deconnexion,
	void * )
{
	Requete.StringOut() = Backend.Language();

	Requete.Complete();
}

// Set the current language;
static void SetLanguage_(
	backend___ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request__ &Request,
	bso::bool__ &Deconnexion,
	void * )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	const str::string_ &Language = Request.StringIn();

	if ( Language.Amount() == 0 )
		Report_( m_BadLanguage, Backend.Locale(), Backend.Language(), Request );
	else
		Backend.SetLanguage( Language.Convert( Buffer ) );

	Request.Complete();
qRR
qRT
qRE
}

#define ADD( I )	Backend.Add( fblcmd::CommandsNames[fblcmd::c##I], ::I##_, fblcmd::CommandsParameters[fblcmd::c##I] )

// Initialisation avec rattachement  l'interface 'Frontend'.
void fblbkd::master_module::Init( fblbkd::backend___ &Backend )
{
	untyped_module::Init( NULL, NULL );
	Backend_ = &Backend;
#if 0
	RawMessages.Init();
#endif
	// Throw an user error (ERRFwk), for testing purpose.
	ADD( ThrowERRFwk );

	// Throw a throw error (ERRt), for testing purpose.
	ADD( ThrowERRFree );

	// Test de la notification.
	ADD( TestNotification );

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

type__ fblbkd::backend___::Type( const str::string_ &Name ) const
{
	sdr::row__ C = Modules.First();

	while ( ( C != qNIL ) && ( str::string( Modules(C)->Name() ) != Name )  )
		C = Modules.Next( C );

	if ( C == qNIL )
		C = FBLBKD_INVALID_TYPE;
	else if ( *C > FBLBKD_INVALID_TYPE )
		qRLmt();

	return (type_t__)*C;
}

bso::bool__ fblbkd::backend___::_TestCompatibility(
	flw::ioflow__ &Flow,
	const char *APIVersion,
	const char *MessageLabel,
	const char *URLLabel )
{
	bso::bool__ Success = true;
qRH
	char RemoteBackendLabel[20];
	char Language[10];
	char RemoteProtocolVersion[10];
	char RemoteAPIVersion[10];
	str::string Translation;
	TOL_CBUFFER___ Buffer;
qRB
	if ( !flw::GetString( Flow, Language, sizeof( Language ) ) )
		qRFwk();

	if ( !flw::GetString( Flow, RemoteProtocolVersion, sizeof( RemoteProtocolVersion ) ) )
		qRFwk();

	if ( !flw::GetString( Flow, RemoteBackendLabel, sizeof( RemoteBackendLabel ) ) )
		qRFwk();

	if ( !flw::GetString( Flow, RemoteAPIVersion, sizeof( RemoteAPIVersion ) ) )
		qRFwk();

	Flow.Dismiss();

	if ( RemoteBackendLabel[0] && strcmp( RemoteBackendLabel, _BackendLabel ) )
		qRVct();	// Pas encore trait.

	if ( strcmp( RemoteProtocolVersion, FBLOVL_PROTOCOL_VERSION )
		 || ( RemoteAPIVersion[0] && strcmp( RemoteAPIVersion, APIVersion ) ) ) {
		Success = false;


		Flow.Put( -1 );

		SetLanguage( Language );

		Translation.Init();
		flw::PutString( _Locale->GetTranslation( MessageLabel, Language, Translation ).Convert( Buffer ), Flow );

		Translation.Init();
		flw::PutString( _Locale->GetTranslation( URLLabel, Language, Translation ).Convert( Buffer ), Flow );
	} else
		Flow.Put( 0 );

	Flow.Commit();
qRR
qRT
qRE
	return Success;
}

bso::bool__ fblbkd::backend___::_TestCompatibility( flw::ioflow__ &Flow )
{
	bso::bool__ Success = false;
qRH
	str::string MessageLabel, URLLabel;
	TOL_CBUFFER___ MessageLabelBuffer, URLLAbelBuffer;
qRB
	MessageLabel.Init( _BackendLabel );
	MessageLabel.Append( "UpdateMessage" );

	URLLabel.Init( _BackendLabel );
	URLLabel.Append( "UpdateURL" );

	Success = _TestCompatibility( Flow, _APIVersion, MessageLabel.Convert( MessageLabelBuffer ), URLLabel.Convert( URLLAbelBuffer ) );
qRR
qRT
qRE
	return Success;
}


bso::bool__ backend___::_HandleRequest(
	flw::ioflow__ &FrontendFlow,
	void *PU,
	log_functions__ &LogFunctions )
{
	master_data__ MasterData;
qRH
	object__ O;
	err::buffer__ ErrorBuffer;
	fblbrq::request__ Request;
	fblbrq::callbacks__ *Callbacks = NULL;
qRB
	MasterData.Deconnexion = false;
	MasterData.UP = PU;

	switch ( _Mode ) {
	case fblovl::mEmbedded:
		_Embedded.Init();
		Callbacks = &_Embedded;
		break;
	case fblovl::mRemote:
		_Remote.Init();
		Callbacks = &_Remote;
		break;
	default:
		qRFwk();
		break;
	}


	Request.Init( *Callbacks, FrontendFlow );

	flw::Get( Request.Input(), O );

	if ( ( !Links.Exists( O ) ) && ( O != FBLBKD_MASTER_OBJECT ) )
		qRFwk();

	if ( O != FBLBKD_MASTER_OBJECT ) {
		Module_( O ).Handle( Index_( O ), Request, PU, LogFunctions );
	} else
		Master_.Handle( (index__)0, Request, &MasterData, LogFunctions );

	Request.Complete();
qRR
	if ( ERRType != err::t_Abort ) {	// Les erreurs de type 't_Abort' doivent tre traits en amont.
		const char *ErrMsg = err::Message( ErrorBuffer );

		ERRRst();

		Request.ReportSoftwareError( ErrMsg );
	}
qRT
qRE
	return !MasterData.Deconnexion;
}
#if 0	
	mdr::row__ untyped_module::Add(
		const char *Name,
		const void *UP,
		... )
	{
		mdr::row__ P;
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
