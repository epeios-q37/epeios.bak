/*
	'bkdmng' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'bkdmng' header file ('bkdmng.h').
	Copyright (C) 2001-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: bkdmng.cpp,v 1.15 2011/06/11 18:16:00 csimon Exp $

#define BKDMNG__COMPILATION

#include "bkdmng.h"

class bkdmngtutor
: public ttr_tutor
{
public:
	bkdmngtutor( void )
	: ttr_tutor( BKDMNG_NAME )
	{
#ifdef BKDMNG_DBG
		Version = BKDMNG_VERSION "\b\bD $";
#else
		Version = BKDMNG_VERSION;
#endif
		Owner = BKDMNG_OWNER;
		Date = "$Date: 2011/06/11 18:16:00 $";
	}
	virtual ~bkdmngtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
ERRProlog
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	Translation.Init();

	Request.ReportError( GetTranslation_( Message, Rack, Translation ).Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
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
	ERRProlog
		STR_BUFFER___ N;
	ERRBegin
		C = bkdcst::GetID( Name.Convert( N  ) );
	ERRErr
	ERREnd
	ERREpilog
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
ERRProlog
			cast__ Cast;
			str::string S;
ERRBegin
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

ERRErr
ERREnd
ERREpilog
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


// Donne la liste des identificateurs et des libellés de types
static void GetTypesIDAndPrefixAndName_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Requete,
	bso::bool__ &,
	void * )
{
ERRProlog
	xitem16 XItem;
	epeios::row__ P;
	type__ Type;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

// Give the list of messages.

static void WriteCommandsIDAndName_(
	const descriptions_ &Descriptions,
	request_manager__ &Requete )
{
ERRProlog
	item16 Item;
	ctn::E_CITEM( description_ ) Description;
	epeios::row__ P;
	command__ Command;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	Requete.ReportError( Requete.StringIn().Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	type__ T = BKDMNG_INVALID_TYPE;
	STR_BUFFER___ Buffer;
	object__ O;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

// Retourne l'identificateur correspondant à un type donné.
static void GetType_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Request,
	bso::bool__ &,
	void * )
{
ERRProlog
	STR_BUFFER___ Buffer;
	type__ T = BKDMNG_INVALID_TYPE;
ERRBegin
	const str::string_ &Type = Request.StringIn();
	T = Backend.Type( Type );

	if ( ( T != BKDMNG_INVALID_TYPE ) )
		Request.Id16Out() = *T;
	else
		ReportError_( m_UnknowObjectTypeName, Backend.LocaleRack(), Request );

	Request.Complete();
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	id16__ Command;
	epeios::row__ Position = CommandsDetails.First();
	ctn::E_CITEM( command_detail_ ) CommandDetail;
	description Description;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}



// Retourne l'identificateur de type et les identificateurs de commande demandé.
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


// Retourne l'identificateur d'une commande donnée pour un type donné.
static void GetCommand_(
	backend_ &Backend,
	untyped_module &Module,
	index__,
	command__ Command,
	request_manager__ &Request,
	bso::bool__ &,
	void * )
{
ERRProlog
	description Description;
	command__ Command;
	STR_BUFFER___ Buffer;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}


// Retourne l'identificateur des commandes demandées pour un type donné.
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
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	const str::string_ &Language = Request.StringIn();

	if ( Language.Amount() == 0 )
			ReportError_( m_BadLanguage, Backend.LocaleRack(), Request );
	else
		Backend.SetLanguage( Language.Convert( Buffer ) );

	Request.Complete();
ERRErr
ERREnd
ERREpilog
}

#define ADD( I )	Backend.Add( bkdcmd::CommandsNames[bkdcmd::c##I], ::I##_, bkdcmd::CommandsParameters[bkdcmd::c##I] )

namespace bkdmng {
	// Initialisation avec rattachement à l'interface 'Frontend'.
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

		// Gnération d'une erreur utilsateur.
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
ERRProlog
	char RemoteTargetLabel[10];
	char Language[10];
	char RemoteProtocolVersion[10];
	char RemoteAPIVersion[10];
	STR_BUFFER___ Buffer;
ERRBegin
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
		ERRl();	// Pas encore traité.

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
ERRErr
ERREnd
ERREpilog
	return Success;
}


bso::bool__ backend_::Handle(
	request_manager__ &Request,
	void *PU,
	log_functions__ &LogFunctions )
{
	master_data__ MasterData;
ERRProlog
	object__ O;
	err::buffer__ ErrorBuffer;
ERRBegin
	MasterData.Deconnexion = false;
	MasterData.UP = PU;

	flw::Get( Request.Input(), O );

	if ( ( !Links.Exists( O ) ) && ( O != BKDMNG_MASTER_OBJECT ) )
		ERRb();

	if ( O != BKDMNG_MASTER_OBJECT ) {
		Module_( O ).Handle( Index_( O ), Request, PU, LogFunctions );
	} else
		Master_.Handle( (index__)0, Request, &MasterData, LogFunctions );

ERRErr
	const char *ErrMsg = err::Message( ErrorBuffer );

	ERRRst();

	Request.ReportBackendError(	ErrMsg );
ERREnd
	Request.Complete();
ERREpilog
	return !MasterData.Deconnexion;
}
#if 0	
	epeios::row__ untyped_module::Add(
		const char *Name,
		const void *UP,
		... )
	{
		epeios::row__ P;
	ERRProlog
		va_list L;
		description Description;
		bkdcst::cast Cast;
	ERRBegin
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
	ERRErr
	ERREnd
	ERREpilog
		return P;
	}
#endif
	
}
/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bkdmngpersonnalization
: public bkdmngtutor
{
public:
	bkdmngpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */

	}
	~bkdmngpersonnalization( void )
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

static bkdmngpersonnalization Tutor;

ttr_tutor &BKDMNGTutor = Tutor;
