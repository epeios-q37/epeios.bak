/*
	'egeckocom.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eGeckoCOM' software.

    'eGeckoCOM' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eGeckoCOM' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: egeckocom.cpp,v 1.41 2013/07/18 19:46:26 csimon Exp $

/*
   Placed before the dedicated header ('egeckocom.h') as this one is computer-generated
   a nd the 'nspxm.h' header has to included before any Gecko-related header to
   define the correct 'XP_(WIN|UNIX|MACOSX|...)'.
*/
#include "nsxpcm.h"

#include "egeckocom.h"

#include "geckof.h"

#include "locale.h"
#include "registry.h"

#include "sclmisc.h"
#include "sclerror.h"
#include "scllocale.h"

#include "atcplsrch.h"

#include "flx.h"
#include "fnm.h"
#include "mtx.h"


#define COMPONENT_VERSION	"5"

#define APP_NAME	"egeckocom"

#define EGECKOCOM_CONTRACTID "@zeusw.org/egeckocom;" COMPONENT_VERSION
#define EGECKOCOM_CLASSNAME "Generic Epeios component"
#define EGECKOCOM_CID  EIGECKOCOM_IID

static mtx::handler__ Mutex_ = MTX_INVALID_HANDLER;	// To protect access to following object.
static geckoo::steering_callback__ *CurrentSteering_ = NULL;
static bso::bool__ IsInitialized_ = false;

static str::string COutString_;
static flx::E_STRING_OFLOW_DRIVER___ COutDriver_;

static str::string CErrString_;
static flx::E_STRING_OFLOW_DRIVER___ CErrDriver_;

#define MESSAGE_UNABLE_TO_REGISTER_ELEMENT	"UnableToRegisterElement"
#define MESSAGE_UNABLE_TO_OPEN_COMPONENT	"UnableToOpenComponent"
#define MESSAGE_BAD_RETRIEVE_CONTEXT		"BadRetrieveContext"
#define MESSAGE_RETRIEVE_FAILURE		"RetrieveFailure"
#define MESSAGE_UNABLE_TO_HANDLE_PSEUDO_EVENT	"UnableToHandlePseudoEvent"

#define LOG utime( "LOG : " __FILE__ " " E_STRING( __LINE__ ), NULL )


class egeckocom___
: public EIGeckoCOM
{
private:
	geckof::gecko_wrapper___ _Wrapper;
	geckoo::shared_data__ _Data;			// Only used by the master window.
	TOL_CBUFFER___ _IdentificationBuffer;	//				"
	TOL_CBUFFER___ _LanguageBuffer;			//				"
	fnm::name___ _Location;
	~egeckocom___();
public:
  NS_DECL_ISUPPORTS
  NS_DECL_EIGECKOCOM

  egeckocom___();
private:
protected:
  /* additional members */
};


#define RP	\
		nsresult NSResult = NS_OK;\
	qRH\
		lcl::meaning ErrorMeaning;\
		str::string ErrorTranslation;\
		err::buffer__ ERRBuffer;

#define RB	qRH\
	ErrorMeaning.Init();

#define RR	\
	qRR
// BE CAREFUL : the exception can come from the library. In this case, the error data are NOT available. The library has its own 'ERR' library data.
#define RN\
		cio::CErr << txf::commit;\
		ErrorTranslation.Init();\
		if ( CErrString_.Amount() != 0 ) {\
			ErrorTranslation = CErrString_;\
			CErrString_.Init();\
		} else if ( !ErrorMeaning.IsEmpty() ) {\
			ERRRst();\
			scllocale::GetLocale().GetTranslation( ErrorMeaning, _LanguageBuffer, ErrorTranslation );\
		} else if ( sclerror::IsErrorPending() ) {\
			ERRRst();\
			scllocale::GetLocale().GetTranslation( sclerror::GetMeaning(), _LanguageBuffer, ErrorTranslation );\
			sclerror::ResetPendingError();\
		} else if ( ERRType != err::t_Free )\
			ErrorTranslation = err::Message( ERRBuffer );\
\
		if ( ERRHit() )\
	 		ERRRst();\
\
		nsxpcm::Transform( ErrorTranslation, JSErrorMessage );\
\
	qRT

#define RE	\
	qRE\
	return NSResult;

static bso::bool__ GetValue_(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return registry::BGetValue( sclrgstry::GetRegistry(), Entry, Value );
}

static const str::string_ &GetMandatoryValue_(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !GetValue_( Entry, Value ) )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );

	return Value;
}

static const str::string_ &GetComponent_(
	const char *ComponentId,
	str::string_ &Component )
{
qRH
	registry::tags Tags;
qRH
	Tags.Init();
	Tags.Append( str::string( ComponentId ) );

	GetMandatoryValue_( rgstry::tentry__( registry::TaggedComponent, Tags ), Component );
qRR
qRT
qRE
	return Component;
}

const char *GetConfigurationDirectory_( TOL_CBUFFER___ &Buffer )
{
	LOG;
qRH
	str::string Directory;
qRH
	LOG;
	Directory.Init();
	LOG;

	nsxpcm::GetInstallationDirectory( Directory );
	LOG;

	Directory.Append( "/components" );
	LOG;

	Directory.Convert( Buffer );
	LOG;
qRR
qRT
qRE
	return Buffer;
}

static void Initialize_( void )
{
qRH
	TOL_CBUFFER___ Buffer;
qRH
LOG;
	sclmisc::Initialize( GetConfigurationDirectory_( Buffer ) );
LOG;
qRR
qRT
qRE
}

NS_IMETHODIMP egeckocom___::Create(
	const char *ComponentId,
	const char *Language,
	char **JSErrorMessage )
{
RP
	str::string RawLibraryName, CorrectedLibraryName;
	TOL_CBUFFER___ Buffer, LocationBuffer;
	str::string ConfigurationDirectory, Identification;
RB
	LOG;

	nsCOMPtr<nsxpcm::clh__>CLH;

	LOG;

	nsxpcm::GetService( CLH_CONTRACTID, CLH );

	LOG;

	if ( ( _LanguageBuffer.Malloc( strlen( Language ) + 1 ) ) == NULL )
		ERRAlc();

	LOG;


	strcpy( _LanguageBuffer, Language );

LOG;

	if ( !IsInitialized_ ) {
		Initialize_();
		IsInitialized_ = true;
	}

LOG;

	RawLibraryName.Init();
	GetComponent_( ComponentId, RawLibraryName );

LOG;

	mtx::Lock( Mutex_ );

	if ( CurrentSteering_ != NULL )
		ERRFwk();

	CorrectedLibraryName.Init();

	if ( fnm::Type( RawLibraryName.Convert( Buffer ) ) != fnm::tAbsolute )
		nsxpcm::GetInstallationDirectory( CorrectedLibraryName );

	CorrectedLibraryName.Append( RawLibraryName );

	Identification.Init();
	Identification.Append( APP_NAME " " VERSION " Build " __DATE__ " " __TIME__ " (" );
	Identification.Append( cpe::GetDescription() );
	Identification.Append( ')' );

	_Location.Init();
	_Data.Init( Identification.Convert( _IdentificationBuffer ), _LanguageBuffer, &fnm::GetLocation( CorrectedLibraryName.Convert( Buffer ), _Location ) );

LOG;

	if ( !_Wrapper.Init( Buffer, &_Data, err::hUserDefined ) ) {
		if ( CErrString_.Amount() == 0 ) {
			ErrorMeaning.SetValue( MESSAGE_UNABLE_TO_OPEN_COMPONENT );
			ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
			ErrorMeaning.AddTag( ComponentId );
		}
		qRFree();
	}

LOG;

	CurrentSteering_ = &_Wrapper.GetSteering();

LOG;

	CurrentSteering_->PreRegistration();

LOG;

RR
	CurrentSteering_ = NULL;

	if ( mtx::IsLocked( Mutex_ ) )
		mtx::Unlock( Mutex_ );
RN
RE
}

NS_IMETHODIMP egeckocom___::Retrieve(
	const char *ComponentId,
	char **JSErrorMessage )
{
RP
	str::string LibraryName;
	TOL_CBUFFER___ Buffer;
RB
	LibraryName.Init();
	GetComponent_( ComponentId, LibraryName );

	 mtx::Lock( Mutex_ );

	if ( CurrentSteering_ != NULL ) {
		ErrorMeaning.SetValue( MESSAGE_BAD_RETRIEVE_CONTEXT );
		ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		qRFree();
	}

	if ( ( CurrentSteering_ = geckof::RetrieveSteering( LibraryName.Convert( Buffer ), err::hUserDefined ) ) == NULL ) {
		ErrorMeaning.SetValue( MESSAGE_RETRIEVE_FAILURE );
		ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		qRFree();
	}

	CurrentSteering_->PreRegistration();
RR
	CurrentSteering_ = NULL;

	if ( mtx::IsLocked( Mutex_ ) )
		mtx::Unlock( Mutex_ );
RN
RE
}

NS_IMETHODIMP egeckocom___::Register(
	nsIDOMWindow *Window,
	char **JSErrorMessage )
{
	// Don't know how to get a 'window' from its 'document'.
RP
	str::string Id;
RB
	if ( !mtx::IsLocked( Mutex_ ) )
		ERRFwk();

	if ( CurrentSteering_ == NULL )
		ERRFwk();

	Id.Init();

	nsxpcm::GetId( nsxpcm::GetElement( Window ), Id );

	if ( !CurrentSteering_->Register( Window, Id ) ) {
		ErrorMeaning.SetValue( MESSAGE_UNABLE_TO_REGISTER_ELEMENT );
		ErrorMeaning.AddTag( " F: " __FILE__ "; L: " E_STRING( __LINE__ ) );
		ErrorMeaning.AddTag( Id );
		qRFree();
	}
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::Stop(
	nsICommandLine *CommandLine,
	char **JSErrorMessage )
{
RP
RB
	if ( !mtx::IsLocked( Mutex_ ) )
		ERRFwk();

	if ( CurrentSteering_ == NULL )
		ERRFwk();

	CurrentSteering_->PostRegistration();

	CurrentSteering_ = NULL;

	mtx::Unlock( Mutex_ );
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::Terminate( char **JSErrorMessage )
{
RP
RB
	_Wrapper.reset();
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::HandlePseudoEvent(
	nsIDOMElement *Element,
	const char *Parameter,
	char **JSErrorMessage )
{
RP
RB
	((geckoo::pseudo_event_callback__ *)str::string( Parameter ).ToPointer() )->Handle( Element );
RR
RN
RE
}

NS_IMETHODIMP egeckocom___::Test( char **JSErrorMessage )
{
RP
	int i = 0;
RB
	i = i;
RR
RN
RE
}

egeckocom___::egeckocom___( void )
{
	LOG;
}

egeckocom___::~egeckocom___( void )
{
}

/* Gecko required part. */

using namespace nsxpcm;

# include "mozilla/ModuleUtils.h"

NS_IMPL_ISUPPORTS1(egeckocom___, EIGeckoCOM)
NS_GENERIC_FACTORY_CONSTRUCTOR(egeckocom___)

//NS_GENERIC_FACTORY_CONSTRUCTOR(event_listener)

// The following line defines a kNS_SAMPLE_CID CID variable.
NS_DEFINE_NAMED_CID(EGECKOCOM_CID);
NS_DEFINE_NAMED_CID(NSXPCM_EVENT_LISTENER_CID);
NS_DEFINE_NAMED_CID(NSXPCM_TREE_VIEW_CID);
NS_DEFINE_NAMED_CID(ATCPLSRCH_AUTOCOMPLETE_SEARCH_CID);
NS_DEFINE_NAMED_CID(NSXPCM_AUTOCOMPLETE_RESULT_CID);
NS_DEFINE_NAMED_CID(CLH_CID);

// Build a table of ClassIDs (CIDs) which are implemented by this module. CIDs
// should be completely unique UUIDs.
// each entry has the form { CID, service, factoryproc, constructorproc }
// where factoryproc is usually NULL.
static const mozilla::Module::CIDEntry kEGeckoCOMCIDs[] = {
    { &kEGECKOCOM_CID, false, NULL, egeckocom___Constructor },
	{ &kNSXPCM_EVENT_LISTENER_CID, false, NULL, nsxpcm::event_listener__Constructor },
	{ &kNSXPCM_TREE_VIEW_CID, false, NULL, nsxpcm::tree_view__Constructor },
	{ &kATCPLSRCH_AUTOCOMPLETE_SEARCH_CID, false, NULL, atcplsrch::autocomplete_search__Constructor },
	{ &kNSXPCM_AUTOCOMPLETE_RESULT_CID, false, NULL, nsxpcm::autocomplete_result___Constructor },
	{ &kCLH_CID, false, NULL, nsxpcm::clh__Constructor },
    { NULL }
};

// Build a table which maps contract IDs to CIDs.
// A contract is a string which identifies a particular set of functionality. In some
// cases an extension component may override the contract ID of a builtin gecko component
// to modify or extend functionality.
static const mozilla::Module::ContractIDEntry kEGeckoCOMContracts[] = {
    { EGECKOCOM_CONTRACTID, &kEGECKOCOM_CID },
    { NSXPCM_EVENT_LISTENER_CONTRACTID, &kNSXPCM_EVENT_LISTENER_CID },
    { NSXPCM_TREE_VIEW_CONTRACTID, &kNSXPCM_TREE_VIEW_CID },
    { ATCPLSRCH_AUTOCOMPLETE_SEARCH_CONTRACTID, &kATCPLSRCH_AUTOCOMPLETE_SEARCH_CID },
    { NSXPCM_AUTOCOMPLETE_RESULT_CONTRACTID, &kNSXPCM_AUTOCOMPLETE_RESULT_CID },
    { CLH_CONTRACTID, &kCLH_CID },
    { NULL }
};

// Category entries are category/key/value triples which can be used
// to register contract ID as content handlers or to observe certain
// notifications. Most modules do not need to register any category
// entries: this is just a sample of how you'd do it.
// @see nsICategoryManager for information on retrieving category data.
static const mozilla::Module::CategoryEntry kEGeckoCOMCategories[] = {
	{ "command-line-handler", "m-geckocom", CLH_CONTRACTID },
    { NULL }
};

static const mozilla::Module EGeckoCOMModule = {
    mozilla::Module::kVersion,
    kEGeckoCOMCIDs,
    kEGeckoCOMContracts,
    kEGeckoCOMCategories
};

static const mozilla::Module EEventListenerModule = {
    mozilla::Module::kVersion,
    kEGeckoCOMCIDs,
    kEGeckoCOMContracts,
    kEGeckoCOMCategories
};

// The following line implements the one-and-only "NSModule" symbol exported from this
// shared library.
NSMODULE_DEFN(EGeckoCOMModule) = &EGeckoCOMModule;

static class cdtor__ {
public:
	cdtor__( void )
	{
		Mutex_ = mtx::Create();

		COutString_.Init();
		COutDriver_.Init( COutString_, fdr::tsDisabled );
		cio::COutDriver.Init( COutDriver_, fdr::ts_Default );

		CErrString_.Init();
		CErrDriver_.Init( CErrString_, fdr::tsDisabled );
		cio::CErrDriver.Init( CErrDriver_, fdr::ts_Default );

		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );
	}
	~cdtor__( void )
	{
		if ( Mutex_ != MTX_INVALID_HANDLER )
			mtx::Delete( Mutex_ );

		Mutex_ = MTX_INVALID_HANDLER;

		if ( IsInitialized_ ) {
			cio::COut << txf::commit;
			cio::CErr << txf::commit;
		}
	}
} _CDTor;

const char *sclmisc::SCLMISCTargetName = APP_NAME;