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

#define XDHJSP__COMPILATION

#include "xdhjsp.h"

#include "xdhjsr.h"

#include "sclrgstry.h"
#include "sclmisc.h"

using namespace xdhjsp;

using xdhcbk::nstring___;
using xdhcbk::nchar__;
using xdhjst::script_name__;

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

const char *xdhjsp::Execute(
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

static void GetWidgetFeatures_(
	const str::string_ &MergedArgs,
	str::string_ &Type,
	str::string_ &Parameters,
	str::string_ &ContentRetrievingMethod,
	str::string_ &FocusingMethod )
{
qRH
	xdhcbk::args Args;
	xdhcbk::retriever__ Retriever;
qRB
	Args.Init();
	xdhcbk::Split( MergedArgs, Args );

	Retriever.Init( Args );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Type );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Parameters );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( ContentRetrievingMethod );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FocusingMethod );
qRR
qRT
qRE
}

static void GetWidgetContentRetrievingMethod_(
	const str::string_ &Args,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, OtherMethod;
qRB
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	GetWidgetFeatures_( Args, Type, Parameters, Method, OtherMethod );
qRR
qRT
qRE
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
	Args.Init( Execute( Callback, xdhjst::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )	// 'true' if the element 'Id' is a widget.
		GetWidgetContentRetrievingMethod_( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhjst::snContentGetter, Result, Id );
	else
		Execute( Callback, xdhjst::snWidgetContentRetriever, Result, Id, nstring___( Method ).Internal()() );
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

	GetWidgetFeatures_( Args, Type, Parameters, OtherMethod, Method );
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
	Args.Init( Execute( Callback, xdhjst::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )
		GetWidgetFocusingMethod_( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhjst::snFocuser, NULL, Id );
	else
		Execute( Callback, xdhjst::snWidgetFocuser, NULL, Id, nstring___( Method ).Internal()() );
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
	Execute( Callback, xdhjst::snAttributeGetter, Result, Id, nstring___( ResultAttributeName ).Internal()() );
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

static void FillDocumentOrCasting_(
	callback__ &Callback,
	script_name__ ScriptName,
	const nchar__ *FrameId,
	const nchar__ *XML,
	const nchar__ *XSL )
{
qRH
	TOL_CBUFFER___ Result;
	str::string Digests;
	str::string Script;
qRB
	Digests.Init( Execute( Callback, ScriptName, &Result, FrameId, XML, XSL ) );

	xdhjst::scripter::HandleEventsWidgetsDigests( FrameId, Digests, Script );
qRR
qRT
qRE
}

static void AlertConfirm_(
	callback__ &Callback,
	script_name__ ScriptName,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *FrameId = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	FillDocumentOrCasting_( Callback, ScriptName, FrameId, XML, XSL );
}


static script_name__ Convert_( xdhcbk::function__ Function )
{
	// The 'Function' which have no entry have no 'direct' script and have special handling.
	switch ( Function ) {
	case xdhcbk::fLog:
		return xdhjst::snLog;
		break;
	case xdhcbk::fAlert:
		return xdhjst::snDialogAlert;
		break;
	case xdhcbk::fConfirm:
		return xdhjst::snDialogConfirm;
		break;
	case xdhcbk::fFillElement:
		return xdhjst::snElementFiller;
		break;
	case xdhcbk::fSetProperty:
		return xdhjst::snPropertySetter;
		break;
	case xdhcbk::fGetProperty:
		return xdhjst::snPropertyGetter;
		break;
	case xdhcbk::fSetAttribute:
		return xdhjst::snAttributeSetter;
		break;
	case xdhcbk::fGetAttribute:
		return xdhjst::snAttributeGetter;
		break;
	case xdhcbk::fRemoveAttribute:
		return xdhjst::snAttributeRemover;
		break;
	case xdhcbk::fSetContent:
		return xdhjst::snContentSetter;
		break;
	default:
		qRFwk();
		break;
	}

	return xdhjst::sn_Undefined;	// To avoid a warning.
}

void xdhjsp::proxy_callback__::XDHCBKProcess(
	xdhcbk::function__ Function,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	switch ( Function ) {
	case xdhcbk::fFillElement:
	case xdhcbk::fFillDocument:
	case xdhcbk::fFillCasting:
	case xdhcbk::fSetProperty:
	case xdhcbk::fGetProperty:
	case xdhcbk::fSetAttribute:
	case xdhcbk::fGetAttribute:
	case xdhcbk::fRemoveAttribute:
	case xdhcbk::fSetContent:
	case xdhcbk::fLog:
		Execute_( C_(), Convert_( Function ), Result, List );
		break;
	case xdhcbk::fAlert:
	case xdhcbk::fConfirm:
		AlertConfirm_( C_(), Convert_( Function ), Result, List );
		break;
	case xdhcbk::fGetResult:
		GetResult_( C_(), Result, List );
		break;
	case xdhcbk::fGetContent:
		GetContent_( C_(), Result, List );
		break;
	case xdhcbk::fFocus:
		Focus_( C_(), List);
		break;
	default:
		qRFwk();
		break;
	}
}

