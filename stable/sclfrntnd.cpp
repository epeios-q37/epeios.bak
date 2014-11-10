/*
	'sclfrntnd.cpp' by Claude SIMON (http://zeusw.org/).

	'sclfrntnd' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define SCLFRNTND__COMPILATION

#include "sclfrntnd.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "scllocale.h"
#include "sclmisc.h"

using namespace sclfrntnd;

using sclrgstry::registry_;

static rgstry::entry___ PredefinedProjects_( "PredefinedProjects", sclrgstry::Parameters );
static rgstry::entry___ DefaultPredefinedProject_( "@Default", PredefinedProjects_ );
static rgstry::entry___ FreePredefinedProject_( "PredefinedProject", PredefinedProjects_ ); 
static rgstry::entry___ PredefinedProjectId_( "@id", FreePredefinedProject_ );
static rgstry::entry___ PredefinedProject_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreePredefinedProject_);
static rgstry::entry___ PredefinedProjectAlias_( "@Alias", PredefinedProject_ );

static rgstry::entry___ Backend_( "Backend", sclrgstry::Parameters );
static rgstry::entry___ BackendAccessMode_( "@AccessMode", Backend_ );
static rgstry::entry___ BackendType_( "@Type", Backend_ );
static rgstry::entry___ BackendPingDelay_( "PingDelay", Backend_ );

static rgstry::entry___ Authentication_( "Authentication", sclrgstry::Parameters );
static rgstry::entry___ AuthenticationCypherKey_( "@CypherKey", Authentication_ );
static rgstry::entry___ AuthenticationMode_( "@Mode", Authentication_ );
static rgstry::entry___ AuthenticationLogin_( "Login", Authentication_ );
static rgstry::entry___ AuthenticationPassword_( "Password", Authentication_ );

static rgstry::entry___ PredefinedBackends_( "PredefinedBackends", sclrgstry::Parameters );
static rgstry::entry___ DefaultPredefinedBackend_( "@Default", PredefinedBackends_ );
static rgstry::entry___ FreePredefinedBackend_( "PredefinedBackend", PredefinedBackends_ ); 
static rgstry::entry___ PredefinedBackendId_( "@id", FreePredefinedBackend_ );
static rgstry::entry___ PredefinedBackend_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreePredefinedBackend_);
static rgstry::entry___ PredefinedBackendAlias_( "@Alias", PredefinedBackend_ );
static rgstry::entry___ PredefinedBackendType_( "@Type", PredefinedBackend_ );

static rgstry::entry___ Internals_( "Internals" );
static rgstry::entry___ ProjectId_( "ProjectId", Internals_ );

static const lcl::meaning_ &GetMeaning_(
	const char *Message,
	lcl::meaning &Meaning )
{
ERRProlog
	str::string RefinedMessage;
ERRBegin
	RefinedMessage.Init( "FRD_" );
	RefinedMessage.Append( Message );

	Meaning.SetValue( RefinedMessage );
ERRErr
ERREnd
ERREpilog
	return Meaning;
}

void sclfrntnd::Report(
	frdkrn::kernel___ &Kernel,
	const char *Message )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Kernel.Report( GetMeaning_( Message, Meaning ) );
ERRErr
ERREnd
ERREpilog
}

void sclfrntnd::Notify(
	frdkrn::kernel___ &Kernel,
	const char *Message )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Kernel.Notify( GetMeaning_( Message, Meaning ) );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedProject_(
	const str::string_ &Id,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	str::string Value;
	str::string Translation;
	rgstry::tags Tags;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Value.Init();
	Registry.GetValue( rgstry::tentry__( PredefinedProjectAlias_, Tags ), Value );

	Translation.Init();
	Locale.GetTranslation( Value.Convert( Buffer ), Language, Translation );

	Writer.PutAttribute( "Alias", Translation );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedProjects_(
	const rgstry::values_ &Ids,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != E_NIL ) {
		Writer.PushTag( "PredefinedProject" );
		Writer.PutAttribute( "id", Id( Row ) );

		GetPredefinedProject_( Id( Row ), Registry, Locale, Language, Writer );

		Writer.PopTag();

		Row = Ids.Next( Row );
	}
}

static void GetPredefinedProjects_(
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	rgstry::values Ids;
ERRBegin
	Ids.Init();

	Registry.GetValues( PredefinedProjectId_, Ids );

	GetPredefinedProjects_( Ids, Registry, Locale, Language, Writer );
ERRErr
ERREnd
ERREpilog
}

void sclfrntnd::GetPredefinedProjects(
	const sclrgstry::registry_ &Registry,
	xml::writer_ &Writer )
{
	Writer.PushTag( "PredefinedProjects" );
	GetPredefinedProjects_( Registry, scllocale::GetLocale(), sclmisc::GetLanguage(), Writer );
	Writer.PopTag();
}

static const str::string_ &GetProjectFileName_(
	const str::string_ &Id,
	const registry_ &Registry,
	str::string_ &Location )
{
ERRProlog
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Registry.GetValue( rgstry::tentry__( PredefinedProject_, Tags ), Location );
ERRErr
ERREnd
ERREpilog
	return Location;
}

const str::string_ &sclfrntnd::GetProjectFileName(
	const sclrgstry::registry_ &Registry,
	const str::string_ &Id,
	str::string_ &Location )
{
	return GetProjectFileName_( Id, Registry, Location );
}

static void GetPredefinedBackend_(
	const str::string_ &Id,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	str::string Value;
	str::string Translation;
	rgstry::tags Tags;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Value.Init();
	Registry.GetValue( rgstry::tentry__( PredefinedBackendAlias_, Tags ), Value );

	Translation.Init();
	Locale.GetTranslation( Value.Convert( Buffer ), Language, Translation );

	Writer.PutAttribute( "Alias", Translation );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedBackends_(
	const rgstry::values_ &Ids,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != E_NIL ) {
		Writer.PushTag( "PredefinedBackend" );
		Writer.PutAttribute( "id", Id( Row ) );

		GetPredefinedBackend_( Id( Row ), Registry, Locale, Language, Writer );

		Writer.PopTag();

		Row = Ids.Next( Row );
	}
}

static void GetPredefinedBackends_(
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
ERRProlog
	rgstry::values Ids;
ERRBegin
	Ids.Init();

	Registry.GetValues( PredefinedBackendId_, Ids );

	GetPredefinedBackends_( Ids, Registry, Locale, Language, Writer );
ERRErr
ERREnd
ERREpilog
}

void sclfrntnd::GetPredefinedBackends(
		const sclrgstry::registry_ &Registry,
	xml::writer_ &Writer )
{
	Writer.PushTag( "PredefinedBackends" );
	GetPredefinedBackends_( Registry, scllocale::GetLocale(), sclmisc::GetLanguage(), Writer );
	Writer.PopTag();
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclfrntndpersonnalization
{
public:
	sclfrntndpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclfrntndpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static sclfrntndpersonnalization Tutor;
