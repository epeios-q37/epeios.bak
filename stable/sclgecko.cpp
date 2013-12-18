/*
	'sclgecko' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclgecko' header file ('sclgecko.h').
	Copyright (C) 20012004 Claude SIMON.

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



//	$Id: sclgecko.cpp,v 1.16 2013/07/18 19:46:22 csimon Exp $

#define SCLGECKO__COMPILATION

#include "sclgecko.h"

class sclgeckotutor
: public ttr_tutor
{
public:
	sclgeckotutor( void )
	: ttr_tutor( SCLGECKO_NAME )
	{
#ifdef SCLGECKO_DBG
		Version = SCLGECKO_VERSION "\b\bD $";
#else
		Version = SCLGECKO_VERSION;
#endif
		Owner = SCLGECKO_OWNER;
		Date = "$Date: 2013/07/18 19:46:22 $";
	}
	virtual ~sclgeckotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cio.h"

#include "geckob.h"

#include "sclmisc.h"
#include "scllocale.h"
#include "sclerror.h"
#include "sclrgstry.h"

#include "xulftk.h"

using namespace sclgecko;

static bso::bool__ IsInitialized_ = false;

static TOL_CBUFFER___ Language_;

#define DEFAULT_LANGUAGE	"en"

static const char *GetLanguage_( void )
{
	if ( Language_ == NULL )
		return DEFAULT_LANGUAGE;

	return Language_;
}

static void ReportSCLPendingError_( void )
{
	if ( sclerror::ReportPendingError( GetLanguage_(), err::hUserDefined ) )
		ERRRst();
}

geckoo::steering_callback__ *geckob::GECKOBCreateSteering( geckoo::shared_data__ *Data )
{
	geckoo::steering_callback__ *Steering = NULL;
ERRProlog
ERRBegin
	if ( !IsInitialized_ )	{
		cio::COutDriver.Init( *Data->COut, fdr::ts_Default );
		cio::CErrDriver.Init( *Data->CErr, fdr::ts_Default );
		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );

		// Do not work when placed in 'global_cdtor'.
		sclmisc::Initialize( SCLGECKOProgramName, Data->Path );
		IsInitialized_ = true;
	}

	Steering = SCLGECKOCreateSteering( Data->LauncherIdentification, scllocale::GetLocale(), Data->Language, Data->UP );
ERRErr
	if ( ERRType >= err::t_amount )
		ReportSCLPendingError_();

	Steering = NULL;
ERREnd
	cio::COut << txf::commit;
	cio::CErr << txf::commit;
ERREpilog
	return Steering;
}

void geckob::GECKOBDeleteSteering( geckoo::steering_callback__ *Steering )
{
	if ( Steering != NULL )
		delete Steering;
}

#include "frdrgy.h"

static rgstry::entry___ XULFeatures_( "XULFeatures", frdrgy::Parameters );
static rgstry::entry___ Documents_( "Documents", XULFeatures_ );
static rgstry::entry___ FreeDocument_( "Document", Documents_ );
static rgstry::entry___ Document_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreeDocument_ );
static rgstry::entry___ FreeElement_( "Element", Document_ );
static rgstry::entry___ ElementId( "@id", FreeElement_ );
static rgstry::entry___ Element_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreeElement_ );
static rgstry::entry___ FreeAttribute_( "Attribute", Element_ );
rgstry::entry___ AttributeName( "@name", FreeAttribute_ );
rgstry::entry___ AttributeValue( RGSTRY_TAGGING_ATTRIBUTE( "name" ), FreeAttribute_ );

inline static void SetElementAttribute_(
	nsIDOMDocument *Document,
	const str::string_ &ElementId,
	const str::string_ &AttributeName,
	const str::string_ &AttributeValue )
{
ERRProlog
	TOL_CBUFFER___  Buffer;
ERRBegin
	nsxpcm::SetAttribute( nsxpcm::GetElementById( Document, ElementId ), AttributeName.Convert( Buffer ), AttributeValue );
ERRErr
ERREnd
ERREpilog
}

static inline void GetValue_(
	rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	sclrgstry::GetRegistry().GetValue( Entry, sclrgstry::GetRoot(), Value );
}

static inline void GetValues_(
	rgstry::tentry__ &Entry,
	rgstry::values_ &Values )
{
	sclrgstry::GetRegistry().GetValues( Entry, sclrgstry::GetRoot(), Values );
}

inline static void SetElementAttributeFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const str::string_ &ElementId,
	const str::string_ &AttributeName )
{
ERRProlog
	str::string Value;
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( DocumentId );
	Tags.Append( ElementId );
	Tags.Append( AttributeName );

	Value.Init();
	GetValue_( rgstry::tentry__( AttributeValue, Tags ), Value );

	SetElementAttribute_( Document, ElementId, AttributeName, Value );
ERRErr
ERREnd
ERREpilog
}

inline static void SetElementAttributeFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const str::string_ &ElementId,
	const rgstry::values_ &AttributeNames )
{
	ctn::E_CMITEM( rgstry::value_ ) Name;
	sdr::row__ Row = AttributeNames.First();

	Name.Init( AttributeNames );

	while ( Row != E_NIL ) {
		SetElementAttributeFromRegistry_( Document, DocumentId, ElementId, Name( Row ) );

		Row = AttributeNames.Next( Row );
	}
}

inline static void SetElementAttributesFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const str::string_ &ElementId )
{
ERRProlog
	rgstry::values Names;
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( DocumentId );
	Tags.Append( ElementId );

	Names.Init();

	GetValues_( rgstry::tentry__( AttributeName, Tags ), Names );

	SetElementAttributeFromRegistry_( Document, DocumentId, ElementId, Names );
ERRErr
ERREnd
ERREpilog
}

inline static void SetElementsAttributesFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const rgstry::values_ &ElementIds )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = ElementIds.First();

	Id.Init( ElementIds );

	while ( Row != E_NIL ) {
		SetElementAttributesFromRegistry_( Document, DocumentId, Id( Row ) );

		Row = ElementIds.Next( Row );
	}
}

inline static void SetElementsAttributesFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId )
{
ERRProlog
	rgstry::values Ids;
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( DocumentId );

	Ids.Init();

	GetValues_( rgstry::tentry__( ElementId, Tags ), Ids );

	SetElementsAttributesFromRegistry_( Document, DocumentId, Ids );
ERRErr
ERREnd
ERREpilog
}

void sclgecko::SetElementsAttributesFromRegistry( nsxpcm::window__ &Window )
{
ERRProlog
	str::string Id;
ERRBegin
	Id.Init();

	Window.GetId( Id );

	SetElementsAttributesFromRegistry_( Window.Document(), Id );

ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclgeckopersonnalization
: public sclgeckotutor
{
public:
	sclgeckopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclgeckopersonnalization( void )
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

static sclgeckopersonnalization Tutor;

ttr_tutor &SCLGECKOTutor = Tutor;
