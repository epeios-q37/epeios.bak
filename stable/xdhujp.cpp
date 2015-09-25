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

#define XDHUJP__COMPILATION

#include "xdhujp.h"

#include "xdhujr.h"
#include "xdhutl.h"

#include "sclrgstry.h"
#include "sclmisc.h"

using namespace xdhujp;

using xdhutl::nstring___;
using xdhutl::nchar__;
using xdhujt::script_name__;

static const char *Execute_(
	callback__  &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Buffer,
	va_list List )
{
	const char *Result = NULL;
qRH
	str::string Script;
qRB
	Script.Init();
	GetScript( ScriptName, Script, List );

	Result = Callback.Execute( Script, Buffer );
qRR
qRT
qRE
	return Result;
}

const char *xdhujp::Execute(
	callback__  &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Buffer,
	... )
{
	const char *Result = NULL;
qRH
	va_list List;
qRB
	va_start( List, Buffer );

	Result = Execute_( Callback, ScriptName, Buffer, List );
qRR
qRT
	va_end( List );
qRE
	return Result;
}

static void AlertConfirm_(
	callback__ &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Result,
	const nchar__ *XML,
	const nchar__ *XSL,
	const nchar__ *Title )
{
qRH
	str::string CloseText;
qRB
	CloseText.Init();
	Callback.GetTranslation( "CloseText", CloseText );

	Execute( Callback, ScriptName, Result, XML, XSL, Title, nstring___(CloseText).Internal()() );
qRR
qRT
qRE
}

static void AlertConfirm_(
	callback__ &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );
	const nchar__ *Title = va_arg( List, const nchar__ * );

	AlertConfirm_( Callback, ScriptName, Result, XML, XSL, Title );
}


static void HandleEventsDescriptions_(
	const str::strings_ &Ids,
	const xdhutl::event_abstracts_ &Abstracts,
	str::string_ &IdsTag,
	str::string_ &EventsTag )
{
qRH
	sdr::row__ Row = qNIL;
	ctn::E_CMITEM( str::string_ ) Id;
	ctn::E_CITEM( xdhutl::event_abstract_ ) Abstract;
qRB
	Row = Ids.First();
	Id.Init( Ids );
	Abstract.Init( Abstracts );

	IdsTag.Append( "[ " );
	EventsTag.Append( "[ ");

	while ( Row != qNIL ) {
		IdsTag.Append('"');
		IdsTag.Append(Id( Row) );
		IdsTag.Append('"');

		EventsTag.Append('"');
		EventsTag.Append(Abstract( Row ).Event );
		EventsTag.Append('"');

		Row = Ids.Next( Row );

		if ( Row != qNIL ) {
			IdsTag.Append( ", " );
			EventsTag.Append( ", " );
		}
	}

	IdsTag.Append( " ]" );
	EventsTag.Append( " ]");
qRR
qRE
qRT
}

static void HandleEventsDescriptions_(
	callback__ &Callback,
	const nchar__ *FrameId,
	const xdhcmn::digest_ &Descriptions )
{
qRH
	str::strings Ids;
	xdhutl::event_abstracts Abstracts;
	str::string IdsTag, EventsTag;
qRB
	Ids.Init();
	Abstracts.Init();
	xdhutl::FillEventAbstracts( Descriptions, Ids, Abstracts );

	IdsTag.Init();
	EventsTag.Init();
	HandleEventsDescriptions_( Ids, Abstracts, IdsTag, EventsTag );

	Execute( Callback, xdhujt::snEventHandlersSetter, NULL, FrameId, nstring___( IdsTag ).Internal()(), nstring___( EventsTag ).Internal()() );
qRR
qRT
qRE
}

static void FillDocument_(
	callback__ &Callback,
	const nchar__ *FrameId,
	const nchar__ *XML,
	const nchar__ *XSL )
{
qRH
	TOL_CBUFFER___ Result;
	str::string RawDigests;
	xdhcmn::digest Digests, Events, Widgets;
	xdhcmn::retriever__ Retriever;
qRB
	RawDigests.Init( Execute( Callback, xdhujt::snDocumentFiller, &Result, FrameId, XML, XSL ) );

	Digests.Init();
	xdhcmn::Split( RawDigests, Digests );

	Retriever.Init( Digests );

	Events.Init();
	Retriever.GetTable( Events );

	Widgets.Init();
	Retriever.GetTable( Widgets );

	HandleEventsDescriptions_( Callback, FrameId, Events );
qRR
qRT
qRE
}

static void FillDocument_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *FrameId = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	FillDocument_( Callback, FrameId, XML, XSL );
}

/**/

static void HandleCastings_(
	const str::strings_ &Ids,
	const str::strings_ &Castings,
	str::string_ &IdsTag,
	str::string_ &CastingsTag )
{
qRH
	sdr::row__ Row = qNIL;
	ctn::E_CMITEM( str::string_ ) Id, Casting;
qRB
	Row = Ids.First();
	Id.Init( Ids );
	Casting.Init( Castings );

	IdsTag.Append( "[ " );
	CastingsTag.Append( "[ ");

	while ( Row != qNIL ) {
		IdsTag.Append('"');
		IdsTag.Append(Id( Row) );
		IdsTag.Append('"');

		CastingsTag.Append('"');
		CastingsTag.Append( Casting( Row ) );
		CastingsTag.Append('"');

		Row = Ids.Next( Row );

		if ( Row != qNIL ) {
			IdsTag.Append( ", " );
			CastingsTag.Append( ", " );
		}
	}

	IdsTag.Append( " ]" );
	CastingsTag.Append( " ]");
qRR
qRE
qRT
}

static void HandleCastings_(
	callback__ &Callback,
	const nchar__ *FrameId,
	const xdhcmn::digest_ &Descriptions )
{
qRH
	str::strings Ids, Castings;
	str::string IdsTag, CastingsTag;
qRB
	Ids.Init();
	Castings.Init();
	xdhutl::FillCastings( Descriptions, Ids, Castings );

	IdsTag.Init();
	CastingsTag.Init();
	HandleCastings_( Ids, Castings, IdsTag, CastingsTag );

	Execute( Callback, xdhujt::snCastsSetter, NULL, FrameId, nstring___( IdsTag ).Internal()(), nstring___( CastingsTag ).Internal()() );
qRR
qRT
qRE
}

static void FillCastings_(
	callback__ &Callback,
	const nchar__ *FrameId,
	const nchar__ *XML,
	const nchar__ *XSL )
{
qRH
	TOL_CBUFFER___ Result;
	str::string RawDigests;
	xdhcmn::digest Castings;
	xdhcmn::retriever__ Retriever;
qRB
	RawDigests.Init( Execute( Callback, xdhujt::snCastingsFiller, &Result, FrameId, XML, XSL ) );

	Castings.Init();
	xdhcmn::Split( RawDigests, Castings );

	HandleCastings_( Callback, FrameId, Castings );
qRR
qRT
qRE
}

static void FillCastings_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *FrameId = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	FillCastings_( Callback, FrameId, XML, XSL );
}


static void GetContent_(
	callback__ &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ *Result )
{
qRH
	str::string Args, Method;
	nstring___ WidgetAttributeName;;
	TOL_CBUFFER___ Buffer;
qRB
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, xdhujt::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )	// 'true' if the element 'Id' is a widget.
		xdhutl::ExtractWidgetContentRetrievingMethod( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhujt::snContentGetter, Result, Id );
	else
		Execute( Callback, xdhujt::snWidgetContentRetriever, Result, Id, nstring___( Method ).Internal()() );
qRR
qRT
qRE
}

static void GetContent_(
	callback__ &Callback,
	TOL_CBUFFER___ *Content,
	va_list List )
{
	GetContent_( Callback, va_arg( List, const nchar__ * ), Content );
}

static void GetWidgetFocusingMethod_(
	const str::string_ &Args,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, OtherMethod;
qRB
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	xdhutl::ExtractWidgetFeatures( Args, Type, Parameters, OtherMethod, Method );
qRR
qRT
qRE
}

static void Focus_(
	callback__ &Callback,
	const nchar__ *Id )
{
qRH
	str::string Args, Method;
	nstring___ WidgetAttributeName;
	TOL_CBUFFER___ Buffer;
qRB
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, xdhujt::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )
		GetWidgetFocusingMethod_( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhujt::snFocuser, NULL, Id );
	else
		Execute( Callback, xdhujt::snWidgetFocuser, NULL, Id, nstring___( Method ).Internal()() );
qRR
qRT
qRE
}

static void Focus_(
	callback__ &Callback,
	va_list List )
{
	Focus_( Callback, va_arg( List, const nchar__ * ) );
}

static void GetResult_(
	callback__ &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ *Result )
{
qRH
	str::string ResultAttributeName;
	TOL_CBUFFER___ Buffer;
qRB
	ResultAttributeName.Init( Callback.GetResultAttributeName( Buffer ) );
	Execute( Callback, xdhujt::snAttributeGetter, Result, Id, nstring___( ResultAttributeName ).Internal()() );
qRR
qRT
qRE
}

static void GetResult_(
	callback__ &Callback,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	GetResult_( Callback, va_arg( List, const nchar__ * ), Result );
}

static script_name__ Convert_( xdhcmn::function__ Function )
{
	switch ( Function ) {
	case xdhcmn::fLog:
		return xdhujt::snLog;
		break;
	case xdhcmn::fAlert:
		return xdhujt::snDialogAlert;
		break;
	case xdhcmn::fConfirm:
		return xdhujt::snDialogConfirm;
		break;
	case xdhcmn::fFillElement:
		return xdhujt::snElementFiller;
		break;
	case xdhcmn::fSetProperty:
		return xdhujt::snPropertySetter;
		break;
	case xdhcmn::fGetProperty:
		return xdhujt::snPropertyGetter;
		break;
	case xdhcmn::fSetAttribute:
		return xdhujt::snAttributeSetter;
		break;
	case xdhcmn::fGetAttribute:
		return xdhujt::snAttributeGetter;
		break;
	case xdhcmn::fRemoveAttribute:
		return xdhujt::snAttributeRemover;
		break;
	case xdhcmn::fSetContent:
		return xdhujt::snContentSetter;
		break;
	case xdhcmn::fGetContent:
		qRFwk();
		break;
	case xdhcmn::fGetResult:
		qRFwk();
		break;
	case xdhcmn::fFocus:
		qRFwk();
		break;
	case xdhcmn::fFillDocument:
		qRFwk();
		break;
	case xdhcmn::fFillCastings:
		qRFwk();
		break;
	default:
		qRFwk();
		break;
	}

	return xdhujt::sn_Undefined;	// To avoid a warning.
}

void xdhujp::proxy_callback__::XDHCMNProcess(
	xdhcmn::function__ Function,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	switch ( Function ) {
	case xdhcmn::fFillElement:
	case xdhcmn::fSetProperty:
	case xdhcmn::fGetProperty:
	case xdhcmn::fSetAttribute:
	case xdhcmn::fGetAttribute:
	case xdhcmn::fRemoveAttribute:
	case xdhcmn::fSetContent:
	case xdhcmn::fLog:
		Execute_( C_(), Convert_( Function ), Result, List );
		break;
	case xdhcmn::fAlert:
	case xdhcmn::fConfirm:
		AlertConfirm_( C_(), Convert_( Function ), Result, List );
		break;
	case xdhcmn::fGetResult:
		GetResult_( C_(), Result, List );
		break;
	case xdhcmn::fGetContent:
		GetContent_( C_(), Result, List );
		break;
	case xdhcmn::fFocus:
		Focus_( C_(), List);
		break;
	case xdhcmn::fFillDocument:
		FillDocument_( C_(), List );
		break;
	case xdhcmn::fFillCastings:
		FillCastings_( C_(), List );
		break;
	default:
		qRFwk();
		break;
	}
}

