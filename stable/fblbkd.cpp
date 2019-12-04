/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define FBLBKD_COMPILATION_

#include "fblbkd.h"

#include "fblcmd.h"
#include "fblovl.h"

#include "cdgb64.h"

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
	rRequest &Request )
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

void fblbkd::rModule::_Clean( void )
{
	sIRow Row = Indexes.First();

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

void fblbkd::rMasterModule::Handle_(
	sIndex Index,
	rRequest &Request,
	log_functions__ &LogFunctions )
{
	command__ C;

	flw::Get( Request.Input(), C );

	if (  C < Descriptions.Amount() ) {
		Request.Prepare( Descriptions( C ).Casts );
		LogFunctions.Log( "", "MASTER", Descriptions( C ).Name, false );
		(*(function__)Functions( C ))( *Backend(), Request );
		LogFunctions.Log( "", "MASTER", Descriptions( C ).Name, true );
	} else if ( C == FBLBKD_MASTER_COMMAND ) {
		char Car;

		if ( ( Car = Request.Input().Get() ) != 0 )
		{
qRH
			cast__ Cast;
			str::string S;
qRB
			Request.Output().Put( 0 );	// No explanation message;

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(Casts & languages)" ), false );

			// Casts.
			do {
				S.Init();

				S.Append( Car );

				while( ( Car = Request.Input().Get() ) != 0 )
					S.Append( Car );

				Cast = GetCastID_( S );

				flw::Put( (fbltyp::id8__)Cast, Request.Output() );

			} while ( ( Car = Request.Input().Get() ) != 0 );

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(Casts & languages)" ), true );

qRR
qRT
qRE
		} else {
			sdr::row__ P = Descriptions.First();
			command__ C = FBLBKD_INVALID_COMMAND;

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(GetCommandCommand)" ), false );


			while ( ( P != qNIL ) && ( Descriptions( P ).Name != str::string( PrimaryCommandName ) ) )
				P = Descriptions.Next( P );

			if ( P != qNIL )
				C = (command__)*P;

			Request.Output().Put( 0 );	// No explanation message;

			flw::Put( C, Request.Output() );

			LogFunctions.Log( "", "MASTER", str::string( "MASTER_COMMAND(GetCommandCommand)" ), true );
		}
	} else
		qRFwk();
}


// Donne la liste des identificateurs et des libells de types
static void GetTypesIDAndPrefixAndName_(
	backend___ &Backend,
	rRequest &Request )
{
qRH
	xitem16 XItem;
	sdr::row__ P;
	type__ Type;
qRB
	xitem16s_ &XItems = Request.XItem16sOut();

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
qRR
qRT
qRE
}

// Give the list of messages.

static void WriteCommandsIDAndName_(
	const descriptions_ &Descriptions,
	rRequest &Request )
{
qRH
	item16 Item;
	sdr::row__ P;
	command__ Command;
qRB
	item16s_ &Items = Request.Item16sOut();

	P = Descriptions.First();

	while( P != qNIL )
	{
		Item.Init();

#ifdef FBLBKD_DEBUG
		if ( P > FBLBKD_COMMAND_MAX )
			ERRC();
#endif
		Command = (command__)*P;

		Item.ID( Command );
		Item.Value = Descriptions( P ).Name;

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
	rRequest &Request )
{
	type__ Type = *Request.Id16In();

	WriteCommandsIDAndName_( Backend.Module( Type ).Descriptions, Request );
}

static inline void WriteParameters_(
	const description_ &Description,
	rRequest &Request )
{
	Request.Id8sOut() = Description.Casts;
}


static void WriteParameters_(
	const descriptions_ &Descriptions,
	command__ Command,
	rRequest &Requete )
{
	WriteParameters_( Descriptions( Command ), Requete );
}


static void GetParameters_(
	backend___ &Backend,
	rRequest &Request )
{
	type__ Type = *Request.Id16In();
	id16__ Command = Request.Id16In();;

	WriteParameters_( Backend.Module( (type__)*Type ).Descriptions, *Command, Request );
}

static void About_(
	backend___ &Backend,
	rRequest &Request )
{
	Request.StringOut() = FBLOVL_PROTOCOL_VERSION;
	Request.StringOut() = Backend.GetBackendLabel();
	Request.StringOut() = Backend.GetAPIVersion();
	Request.StringOut() = Backend.GetExtendedBackendInformations();
	Request.StringOut() = Backend.GetBackendCopyright();
	Request.StringOut() = Backend.GetSoftwareInformations();
}

namespace {
	bso::sBool Test_(
		const str::dString &Crypted,
		const str::dString &Code,
		const str::dString &Key )
	{
		bso::sBool Correct = false;
	qRH
		str::wString Decoded, Plain;
	qRB
		if ( ( Crypted.Amount() != 0 ) && ( Key.Amount() != 0 ) ) {
			Decoded.Init();
			cdgb64::Decode( Crypted, Decoded );
			Plain.Init();
			crptgr::Decrypt( Decoded, Key, Plain );

			Correct = Plain == Code;
		}
	qRR
	qRT
	qRE
		return Correct;
	}
}

static void Ping_(
	backend___ &Backend,
	rRequest &Request )
{
	if ( Test_( Request.StringIn(), Backend.Code(), Backend.Key() ) )
		Request.Input().Get();
}

static void Crash_(
	backend___ &Backend,
	rRequest &Request )
{
	if ( Test_( Request.StringIn(), Backend.Code(), Backend.Key() ) )
# if 1
		tol::Crash();
# else
	// Doesn't work (i.e. doesn't crash) in release mode.
		memcpy( NULL, NULL, 1 );	// Crashes the application.
#endif
}

// Throw an framework error (ERRFwk), for testing purpose.
static void ThrowERRFwk_(
	backend___ &Backend,
	rRequest &Request )
{
	qRFwk();
}

// Throw an itentional error (ERRFree), for testing purpose.
static void ThrowERRFree_(
	backend___ &Backend,
	rRequest &Request )
{
	qRFree();
}

static void TestNotification_(
	backend___ &Backend,
	rRequest &Request )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Request.ReportRequestError( Request.StringIn().Convert( Buffer ) );
qRR
qRT
qRE
}

// Retourne un nouvel objet.
static void GetNewObject_(
	backend___ &Backend,
	rRequest &Request )
{
qRH
	type__ T = FBLBKD_INVALID_TYPE;
	TOL_CBUFFER___ Buffer;
	object__ O;
qRB
	T = *Request.Id16In();

	if ( *T >= Backend.Modules.Amount() )
		qRFwk();

	if ( T == FBLBKD_INVALID_TYPE )
		qRFwk();

	O = Backend.New( (type__)*T );

	if ( O != FBLBKD_INVALID_TYPE )
		Request.ObjectOut() = O;
	else
		Report_( m_UnknowObjectType, Backend.Locale(), Backend.Language(), Request );
qRR
qRT
qRE
}

// Retourne l'identificateur correspondant  un type donn.
static void GetType_(
	backend___ &Backend,
	rRequest &Request )
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
qRR
qRT
qRE
}

// Supprime un objet.
static void RemoveObject_(
	backend___ &Backend,
	rRequest &Request )
{
	object__ O = Request.ObjectIn();

/*	if ( !Backend.Valide( O ) )
		ERRb();
*/
	Backend.Delete( O );
}

static void FillCommands_(
	backend___ &Backend,
	type__ Type,
	const commands_details_ &CommandsDetails,
	id16s_ &Commands )
{
qRH
	id16_t__ Command;
	sdr::row__ Position = qNIL;
	description Description;
qRB
	Position = CommandsDetails.First();

	while( Position != qNIL )
	{
		Description.Init();

		Description.Casts = CommandsDetails( Position ).Casts;
		Description.Name = CommandsDetails( Position ).Name;

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
	rRequest &Request )
{
	const string_ &Name = Request.StringIn();
	const commands_details_ &CommandsDetails = Request.CommandsDetailsIn();
	type__ Type = *Request.Id16Out();
	id16s_ &Commands = Request.Id16sOut();

	if ( ( Type = Backend.Type( Name ) ) == FBLBKD_INVALID_TYPE )
		qRFwk();

	Commands.Init();

	FillCommands_( Backend, Type, CommandsDetails, Commands );
}


// Retourne l'identificateur d'une commande donne pour un type donn.
static void GetCommand_(
	backend___ &Backend,
	rRequest &Request )
{
qRH
	description Description;
	command__ Command;
	TOL_CBUFFER___ Buffer;
qRB
	type__ Type = *Request.Id16In();

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
qRR
qRT
qRE
}


// Retourne l'identificateur des commandes demandes pour un type donn.
static void GetCommands_(
	backend___ &Backend,
	rRequest &Request )
{
	type__ Type = *Request.Id16In();
	const commands_details_ &CommandsDetails = Request.CommandsDetailsIn();
	id16s_ &Commands = Request.Id16sOut();

	FillCommands_( Backend, Type, CommandsDetails, Commands );
}


// Deconnection
static void Disconnect_(
	backend___ &Backend,
	rRequest &Request )
{
	Request.Disconnect() = true;
}

// Return the current language.
static void GetLanguage_(
	backend___ &Backend,
	rRequest &Request )
{
	Request.StringOut() = Backend.Language();
}

// Set the current language;
static void SetLanguage_(
	backend___ &Backend,
	rRequest &Request )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	const str::string_ &Language = Request.StringIn();

	if ( Language.Amount() == 0 )
		Report_( m_BadLanguage, Backend.Locale(), Backend.Language(), Request );
	else
		Backend.SetLanguage( Language.Convert( Buffer ) );
qRR
qRT
qRE
}

#define ADD( I )	Backend.Add( fblcmd::CommandsNames[fblcmd::c##I], ::I##_, fblcmd::CommandsParameters[fblcmd::c##I] )

// Initialisation avec rattachement  l'interface 'Frontend'.
void fblbkd::rMasterModule::Init( fblbkd::backend___ &Backend )
{
	rBaseModule_::Init( NULL, NULL, Backend );
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

	// Command to verify if the backend is still responding. When the apporiate code is given, the backend does nor respond ; for watchdog testing purpose.
	ADD( Ping );

	// When the appropriate code is given, crashes the backend ; for watchdog testing purpose.
	ADD( Crash );

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
	fdr::rRWDriver &FrontendIODriver,
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
	flw::rDressedRWFlow<> Flow;
qRB
	Flow.Init( FrontendIODriver );

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
		qRVct();	// Not already handled.

	SetLanguage( Language );

	if ( strcmp( RemoteProtocolVersion, FBLOVL_PROTOCOL_VERSION )
		 || ( RemoteAPIVersion[0] && strcmp( RemoteAPIVersion, APIVersion ) ) ) {
		Success = false;

		Flow.Put( -1 );

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

bso::bool__ fblbkd::backend___::TestCompatibility( fdr::rRWDriver &FrontendIODriver )
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

	Success = _TestCompatibility( FrontendIODriver, _APIVersion, MessageLabel.Convert( MessageLabelBuffer ), URLLabel.Convert( URLLAbelBuffer ) );
qRR
qRT
qRE
	return Success;
}

bso::bool__ backend___::HandleRequest(
	fdr::rRWDriver &Driver,
	log_functions__ &LogFunctions )
{
	bso::sBool Disconnect = false;
qRH
	object__ O;
	rRequest Request;
	fblber::embedded_callbacks___ Embedded;
	fblbrr::remote_callbacks___ Remote;
qRB
	switch ( Mode_ ) {
	case fblovl::mReferenced:
		Embedded.Init();
		Request.Init( Embedded, Driver );
		break;
	case fblovl::mSerialized:
		Remote.Init();
		Request.Init( Remote, Driver );
		break;
	default:
		qRFwk();
		break;
	}

	flw::Get( Request.Input(), O );

	if ( ( !Links.Exists( O ) ) && ( O != FBLBKD_MASTER_OBJECT ) )
		qRFwk();

	if ( O != FBLBKD_MASTER_OBJECT ) {
		Module_( O ).Handle( Index_( O ), Request, LogFunctions );
	} else
		Master_.Handle( (sIndex)0, Request, LogFunctions );

	Disconnect = Request.Disconnect();

	Request.Complete();
qRR
qRT
qRE
	return !Disconnect;
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
