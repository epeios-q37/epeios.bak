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

#define SCLGECKO__COMPILATION

#include "sclgecko.h"

#include "cio.h"

#include "geckob.h"

#include "sclmisc.h"
#include "scllocale.h"
#include "sclerror.h"
#include "sclrgstry.h"

#include "xulftk.h"

using namespace sclgecko;

static bso::bool__ IsInitialized_ = false;

static void ReportSCLPendingError_( void )
{
qRH
	str::string Translation;
qRB
	Translation.Init();

	sclmisc::GetSCLPendingError( Translation );

	cio::CErr << Translation;
qRR
qRT
qRE
}

geckoo::steering_callback__ *geckob::GECKOBCreateSteering( geckoo::shared_data__ *Data )
{
	geckoo::steering_callback__ *Steering = NULL;
qRH
qRB
	if ( !IsInitialized_ )	{
		cio::COutDriver.Init( *Data->COut, fdr::ts_Default );
		cio::CErrDriver.Init( *Data->CErr, fdr::ts_Default );
		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );

		// Do not work when placed in 'global_cdtor'.
		sclmisc::Initialize( *Data->Path );
		IsInitialized_ = true;
	}

	Steering = SCLGECKOCreateSteering( Data->LauncherIdentification, scllocale::GetLocale(), Data->Language, Data->UP );
qRR
	if ( ERRType >= err::t_amount )
		ReportSCLPendingError_();

	Steering = NULL;
qRT
	cio::COut << txf::commit;
	cio::CErr << txf::commit;
qRE
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
qRH
	TOL_CBUFFER___  Buffer;
qRB
	nsxpcm::SetAttribute( nsxpcm::GetElementById( Document, ElementId ), AttributeName.Convert( Buffer ), AttributeValue );
qRR
qRT
qRE
}

static inline void GetValue_(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	sclrgstry::BGetValue( sclrgstry::GetRegistry(), Entry, Value );
}

static inline void GetValues_(
	const rgstry::tentry__ &Entry,
	rgstry::values_ &Values )
{
	sclrgstry::GetRegistry().GetValues( Entry, Values );
}

inline static void SetElementAttributeFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const str::string_ &ElementId,
	const str::string_ &AttributeName )
{
qRH
	str::string Value;
	rgstry::tags Tags;
qRB
	Tags.Init();
	Tags.Append( DocumentId );
	Tags.Append( ElementId );
	Tags.Append( AttributeName );

	Value.Init();
	GetValue_( rgstry::tentry__( AttributeValue, Tags ), Value );

	SetElementAttribute_( Document, ElementId, AttributeName, Value );
qRR
qRT
qRE
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

	while ( Row != qNIL ) {
		SetElementAttributeFromRegistry_( Document, DocumentId, ElementId, Name( Row ) );

		Row = AttributeNames.Next( Row );
	}
}

inline static void SetElementAttributesFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const str::string_ &ElementId )
{
qRH
	rgstry::values Names;
	rgstry::tags Tags;
qRB
	Tags.Init();
	Tags.Append( DocumentId );
	Tags.Append( ElementId );

	Names.Init();

	GetValues_( rgstry::tentry__( AttributeName, Tags ), Names );

	SetElementAttributeFromRegistry_( Document, DocumentId, ElementId, Names );
qRR
qRT
qRE
}

inline static void SetElementsAttributesFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId,
	const rgstry::values_ &ElementIds )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = ElementIds.First();

	Id.Init( ElementIds );

	while ( Row != qNIL ) {
		SetElementAttributesFromRegistry_( Document, DocumentId, Id( Row ) );

		Row = ElementIds.Next( Row );
	}
}

inline static void SetElementsAttributesFromRegistry_(
	nsIDOMDocument *Document,
	const str::string_ &DocumentId )
{
qRH
	rgstry::values Ids;
	rgstry::tags Tags;
qRB
	Tags.Init();
	Tags.Append( DocumentId );

	Ids.Init();

	GetValues_( rgstry::tentry__( ElementId, Tags ), Ids );

	SetElementsAttributesFromRegistry_( Document, DocumentId, Ids );
qRR
qRT
qRE
}

void sclgecko::SetElementsAttributesFromRegistry( nsxpcm::window__ &Window )
{
qRH
	str::string Id;
qRB
	Id.Init();

	Window.GetId( Id );

	SetElementsAttributesFromRegistry_( Window.Document(), Id );

qRR
qRT
qRE
}
