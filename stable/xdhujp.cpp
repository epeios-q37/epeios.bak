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

#define XDHUJP__COMPILATION

#include "xdhujp.h"

#include "xdhujr.h"
#include "xdhutl.h"

#include "sclrgstry.h"
#include "sclmisc.h"

using namespace xdhujp;

using xdhutl::nstring___;
using xdhutl::nchar__;
using xdhujs::script_name__;

static const char *Execute_(
	cJS  &Callback,
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
	cJS  &Callback,
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
	cJS &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );
	const nchar__ *Title = va_arg( List, const nchar__ * );
	const nchar__ *CloseText = va_arg( List, const nchar__ * );

	Execute( Callback, ScriptName, Result, XML, XSL, Title, CloseText );
}

static void Append_(
	const str::string_ &Item,
	str::string_ &Tag )
{
	Tag.Append('"');
	Tag.Append( Item );
	Tag.Append('"');
}

static void HandleEvents_(
	const str::strings_ &Ids,
	const xdhutl::event_abstracts_ &Abstracts,
	str::string_ &IdsTag,
	str::string_ &EventsTag )
{
	sdr::row__ Row = qNIL;

	if ( Ids.Amount() != Abstracts.Amount() )
		qRFwk();

	Row = Ids.First();

	IdsTag.Append( "[ " );
	EventsTag.Append( "[ ");

	while ( Row != qNIL ) {
		Append_( Ids( Row ), IdsTag );
		Append_( Abstracts( Row ).Event, EventsTag );

		Row = Ids.Next( Row );

		if ( Row != qNIL ) {
			IdsTag.Append( ", " );
			EventsTag.Append( ", " );
		}
	}

	IdsTag.Append( " ]" );
	EventsTag.Append( " ]");
}

static void HandleEvents_(
	cJS &Callback,
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

	if ( Ids.Amount() != 0 ) {
		IdsTag.Init();
		EventsTag.Init();
		HandleEvents_( Ids, Abstracts, IdsTag, EventsTag );

		Execute( Callback, xdhujs::snEventHandlersSetter, NULL,nstring___( IdsTag ).Internal()(), nstring___( EventsTag ).Internal()() );
	}
qRR
qRT
qRE
}

static void HandleWidgets_(
	const str::strings_ &Ids,
	const str::strings_ &Types,
	const str::strings_ &ParametersSets,
	str::string_ &IdsTag,
	str::string_ &TypesTag,
	str::string_ &ParametersSetsTag )
{
	sdr::row__ Row = qNIL;

	if ( Ids.Amount() != Types.Amount() )
		qRFwk();

	if ( Ids.Amount() != ParametersSets.Amount() )
		qRFwk();

	Row = Ids.First();

	IdsTag.Append( "[ " );
	TypesTag.Append( "[ ");
	ParametersSetsTag.Append( "[ ");

	while ( Row != qNIL ) {
		Append_( Ids( Row ), IdsTag );
		Append_( Types( Row ), TypesTag );
		Append_( ParametersSets( Row ), ParametersSetsTag );

		Row = Ids.Next( Row );

		if ( Row != qNIL ) {
			IdsTag.Append( ", " );
			TypesTag.Append( ", " );
			ParametersSetsTag.Append( ", " );
		}
	}

	IdsTag.Append( " ]" );
	TypesTag.Append( " ]");
	ParametersSetsTag.Append( " ]");
}

static void SetLayout_(
	cJS &Callback,
	const nchar__ *Id,	// If 'Id' != NULL, it's the id of the element to apply to, otherwise it applies to the document.
	const nchar__ *XML,
	const nchar__ *XSL )	// If empty, 'XML' cotains pute '(X)HTML'.
{
qRH
	TOL_CBUFFER___ Result;
	str::string RawDigest;
	xdhcmn::digest Digest;
qRB
	RawDigest.Init( Execute( Callback, xdhujs::snEventsFetcher, &Result, Id, XML, XSL ) );

	Digest.Init();
	xdhcmn::Split( RawDigest, Digest );

	HandleEvents_( Callback, Digest );
qRR
qRT
qRE
}

static void SetLayout_(
	cJS &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	SetLayout_( Callback, Id, XML, XSL );
}

static void HandleWidgets_(
	cJS &Callback,
	const xdhcmn::digest_ &Descriptions )
{
qRH
	str::strings Ids, Types, ParametersSets;
	str::string IdsTag, TypesTag, ParametersSetsTag;
qRB
	Ids.Init();
	Types.Init();
	ParametersSets.Init();
	xdhutl::ExtractWidgetsTypesAndParametersSets( Descriptions, Ids, Types, ParametersSets );

	if ( Ids.Amount() != 0 ) {
		IdsTag.Init();
		TypesTag.Init();
		ParametersSetsTag.Init();
		HandleWidgets_( Ids, Types, ParametersSets, IdsTag, TypesTag, ParametersSetsTag );

		Execute( Callback, xdhujs::snWidgetsInstantiator, NULL, nstring___( IdsTag ).Internal()(), nstring___( TypesTag ).Internal()(), nstring___( ParametersSetsTag ).Internal()() );
	}
qRR
qRT
qRE
}

static void HandleWidgets_(
	cJS &Callback,
	const nchar__ *Id )
{
qRH
	TOL_CBUFFER___ Result;
	str::string RawDigest;
	xdhcmn::digest Digest;
qRB
	RawDigest.Init( Execute( Callback, xdhujs::snWidgetsFetcher, &Result, Id ) );

	Digest.Init();
	xdhcmn::Split( RawDigest, Digest );

	HandleWidgets_( Callback, Digest );
qRR
qRT
qRE
}

static void DressWidgets_(
	cJS &Callback,
	va_list List )
{
	const nchar__ *Id = va_arg( List, const nchar__ * );

	HandleWidgets_( Callback, Id );
}

static void SetContents_(
	cJS &Callback,
	const nchar__ *Ids,
	const nchar__ *Contents )
{
qRH
	TOL_CBUFFER___ Result;
qRB
	Execute( Callback, xdhujs::snContentsSetter, NULL, Ids, Contents );
qRR
qRT
qRE
}

static void SetContents_(
	cJS &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *Ids = va_arg( List, const nchar__ * );
	const nchar__ *Contents = va_arg( List, const nchar__ * );

	SetContents_( Callback, Ids, Contents );
}

namespace{
	namespace {
		// TODO: to optimize.
		void GetIds_(
			const str::dStrings &AllTags,	// All the tags ..
			const str::dStrings &AllIds,		// with the corresponding ids.
			const str::dString &Tag,
			const str::dString &Value,
			str::dStrings &Ids,
			str::dStrings &Values )
		{
			sdr::sRow Row = AllTags.First();

			while ( ( Row != qNIL ) && ( ( Row = str::Search( Tag, AllTags, Row ) ) != qNIL ) ) {
				Ids.Append( AllIds( Row ) );
				Values.Append( Value );
				Row = AllTags.Next( Row );
			}
		}
	}

	void MatchTagsWithIds_(
		const xdhcmn::digest_ &Digest,
		const str::dStrings &Tags,
		const str::dStrings &Values,
		str::dStrings &Ids,
		str::dStrings &CorrespondingValues )
	{
	qRH;
		str::wStrings AllIds, AllTags;
		sdr::sRow Row = qNIL;
	qRB;
		tol::Init( AllIds, AllTags );
		xdhutl::GetTags( Digest, AllIds, AllTags );

		Row = Tags.First();

		while ( Row != qNIL ) {
			GetIds_( AllTags, AllIds, Tags( Row ), Values( Row ), Ids, CorrespondingValues );

			Row = Tags.Next( Row );
		}
	qRR;
	qRT;
	qRE;
	}

	void MatchTagsWithIds_(
		const xdhcmn::digest_ &Digest,
		const str::dString &MergedTags,
		const str::dString &MergedValues,
		str::dStrings &Ids,
		str::dStrings &CorrespondingValues)
	{
	qRH;
		str::wStrings Tags, Values;
	qRB;
		Tags.Init();
		xdhcmn::FlatSplit( MergedTags, Tags );

		Values.Init();
		xdhcmn::FlatSplit( MergedValues, Values );

		MatchTagsWithIds_( Digest, Tags, Values, Ids, CorrespondingValues );
	qRR;
	qRT;
	qRE;
	}

	
	void MatchTagsWithIds_(
		const xdhcmn::digest_ &Digest,
		const str::dString &MergedTags,
		const str::dString &MergedValues,
		str::dString &MergedIds,
		str::dString &CorrespondingMergedValues)
	{
	qRH;
		str::wStrings Ids, Values;
	qRB;
		tol::Init( Ids, Values );
		MatchTagsWithIds_( Digest, MergedTags, MergedValues, Ids, Values );

		xdhcmn::FlatMerge( Ids, MergedIds, true );
		xdhcmn::FlatMerge( Values, CorrespondingMergedValues, true );
	qRR;
	qRT;
	qRE;
	}
}

static void GetValue_(
	cJS &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ *Result )
{
qRH
	str::string Args, Method;
	nstring___ WidgetAttributeName;;
	TOL_CBUFFER___ Buffer;
qRB
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, xdhujs::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )	// 'true' if the element 'Id' is a widget.
		xdhutl::ExtractWidgetContentRetrievingMethod( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhujs::snValueGetter, Result, Id );
	else
		Execute( Callback, xdhujs::snWidgetValueRetriever, Result, Id, nstring___( Method ).Internal()() );
qRR
qRT
qRE
}

static void GetValue_(
	cJS &Callback,
	TOL_CBUFFER___ *Value,
	va_list List )
{
	GetValue_( Callback, va_arg( List, const nchar__ * ), Value );
}

static void Focus_(
	cJS &Callback,
	const nchar__ *Id )
{
qRH
	str::string Args, Method;
	nstring___ WidgetAttributeName;
	TOL_CBUFFER___ Buffer;
qRB
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, xdhujs::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )
		xdhutl::ExtractWidgetFocusingMethod( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhujs::snFocuser, NULL, Id );
	else
		Execute( Callback, xdhujs::snWidgetFocuser, NULL, Id, nstring___( Method ).Internal()() );
qRR
qRT
qRE
}

static void Select_(
	cJS &Callback,
	const nchar__ *Id )
{
qRH
	str::string Args, Method;
	nstring___ WidgetAttributeName;
	TOL_CBUFFER___ Buffer;
qRB
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, xdhujs::snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()()) );

	Method.Init();

	if ( Args.Amount() != 0 )
		xdhutl::ExtractWidgetSelectionMethod( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, xdhujs::snSelector, NULL, Id );
	else
		Execute( Callback, xdhujs::snWidgetSelector, NULL, Id, nstring___( Method ).Internal()() );
qRR
qRT
qRE
}

static void Focus_(
	cJS &Callback,
	va_list List )
{
	Focus_( Callback, va_arg( List, const nchar__ * ) );
}

static void Select_(
	cJS &Callback,
	va_list List )
{
	Select_( Callback, va_arg( List, const nchar__ * ) );
}

static void GetResult_(
	cJS &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ *Result )
{
qRH
	str::string ResultAttributeName;
	TOL_CBUFFER___ Buffer;
qRB
	ResultAttributeName.Init( Callback.GetResultAttributeName( Buffer ) );
	Execute( Callback, xdhujs::snAttributeGetter, Result, Id, nstring___( ResultAttributeName ).Internal()() );
qRR
qRT
qRE
}

static void GetResult_(
	cJS &Callback,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	GetResult_( Callback, va_arg( List, const nchar__ * ), Result );
}

namespace {
	void InsertCSSRule_(
		cJS &Callback,
		const nchar__ *Rule,
		const nchar__ *Index )
	{
	qRH;
	qRB;
		Execute( Callback, xdhujs::snCSSRuleInserter, NULL, nstring___( Rule ).Internal()(), nstring___( Index ).Internal()() );
	qRR;
	qRT;
	qRE;
	}
}

void InsertCSSRule_(
	cJS &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *Rule = va_arg( List, const nchar__ * );
	const nchar__ *Index = va_arg( List, const nchar__ * );

	InsertCSSRule_( Callback, Rule, Index );
}

namespace {
	void AppendCSSRule_(
		cJS &Callback,
		const nchar__ *Rule,
		TOL_CBUFFER___ *Index )
	{
	qRH;
	qRB;
		Execute( Callback, xdhujs::snCSSRuleAppender, Index, Rule );
	qRR;
	qRT;
	qRE;
	}
}

static void AppendCSSRule_(
	cJS &Callback,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	AppendCSSRule_( Callback, va_arg( List, const nchar__ * ), Result );
}



namespace {
	void RemoveCSSRule_(
		cJS &Callback,
		const nchar__ *Index )
	{
	qRH;
	qRB;
		Execute( Callback, xdhujs::snCSSRuleRemover, NULL, nstring___( Index ).Internal()() );
	qRR;
	qRT;
	qRE;
	}
}

static void RemoveCSSRule_(
	cJS &Callback,
	va_list List )
{
	RemoveCSSRule_( Callback, va_arg( List, const nchar__ * ) );
}

namespace {
	void HandleClasses_(
		cJS &Callback,
		xdhujs::script_name__ ScriptName,
		const nchar__ *Ids,
		const nchar__ *Classes )
	{
	qRH;
	qRB;
		Execute( Callback, ScriptName, NULL, nstring___( Ids ).Internal()(), nstring___( Classes ).Internal()() );
	qRR;
	qRT;
	qRE;
	}
}


static void HandleClasses_(
	cJS &Callback,
	xdhujs::script_name__ ScriptName,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *Ids = va_arg( List, const nchar__ * );
	const nchar__ *Classes = va_arg( List, const nchar__ * );

	HandleClasses_( Callback, ScriptName, Ids, Classes );
}

namespace {
	void EnableElements_(
		cJS &Callback,
		const nchar__ *Ids )
	{
	qRH;
	qRB;
		Execute( Callback, xdhujs::snElementsEnabler, NULL, nstring___( Ids ).Internal()() );
	qRR;
	qRT;
	qRE;
	}
}

static void EnableElements_(
	cJS &Callback,
	va_list List )
{
	EnableElements_( Callback, va_arg( List, const nchar__ * ) );
}

namespace {
	void DisableElements_(
		cJS &Callback,
		const nchar__ *Ids )
	{
	qRH;
	qRB;
		Execute( Callback, xdhujs::snElementsDisabler, NULL, nstring___( Ids ).Internal()() );
	qRR;
	qRT;
	qRE;
	}
}

static void DisableElements_(
	cJS &Callback,
	va_list List )
{
	DisableElements_( Callback, va_arg( List, const nchar__ * ) );
}

static script_name__ Convert_( xdhcmn::function__ Function )
{
	switch ( Function ) {
	case xdhcmn::fExecute:
		return xdhujs::snExecute;
		break;
	case xdhcmn::fLog:
		return xdhujs::snLog;
		break;
	case xdhcmn::fAlert:
		return xdhujs::snDialogAlert;
		break;
	case xdhcmn::fConfirm:
		return xdhujs::snDialogConfirm;
		break;
	case xdhcmn::fSetProperty:
		return xdhujs::snPropertySetter;
		break;
	case xdhcmn::fGetProperty:
		return xdhujs::snPropertyGetter;
		break;
	case xdhcmn::fSetAttribute:
		return xdhujs::snAttributeSetter;
		break;
	case xdhcmn::fGetAttribute:
		return xdhujs::snAttributeGetter;
		break;
	case xdhcmn::fRemoveAttribute:
		return xdhujs::snAttributeRemover;
		break;
	case xdhcmn::fSetValue:
		return xdhujs::snValueSetter;
		break;
	case xdhcmn::fGetValue:
		qRFwk();
		break;
	case xdhcmn::fGetResult:
		qRFwk();
		break;
	case xdhcmn::fFocus:
		qRFwk();
		break;
	case xdhcmn::fSelect:
		qRFwk();
		break;
	case xdhcmn::fSetLayout:
		qRFwk();
		break;
	case xdhcmn::fDressWidgets:
		qRFwk();
		break;
	case xdhcmn::fSetContents:
		qRFwk();
		break;
	case xdhcmn::fInsertCSSRule:
		qRFwk();
		break;
	case xdhcmn::fAppendCSSRule:
		qRFwk();
		break;
	case xdhcmn::fRemoveCSSRule:
		qRFwk();
		break;
	case xdhcmn::fAddClasses:
		qRFwk();
		break;
	case xdhcmn::fRemoveClasses:
		qRFwk();
		break;
	case xdhcmn::fToggleClasses:
		qRFwk();
		break;
	case xdhcmn::fEnableElements:
		qRFwk();
		break;
	case xdhcmn::fDisableElements:
		qRFwk();
		break;
	default:
		qRFwk();
		break;
	}

	return xdhujs::sn_Undefined;	// To avoid a warning.
}

void xdhujp::sProxyCallback::XDHCMNProcess(
	xdhcmn::function__ Function,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	switch ( Function ) {
	case xdhcmn::fExecute:
	case xdhcmn::fSetProperty:
	case xdhcmn::fGetProperty:
	case xdhcmn::fSetAttribute:
	case xdhcmn::fGetAttribute:
	case xdhcmn::fRemoveAttribute:
	case xdhcmn::fSetValue:
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
	case xdhcmn::fGetValue:
		GetValue_( C_(), Result, List );
		break;
	case xdhcmn::fFocus:
		Focus_( C_(), List);
		break;
	case xdhcmn::fSelect:
		Select_( C_(), List);
		break;
	case xdhcmn::fSetLayout:
		SetLayout_( C_(), List );
		break;
	case xdhcmn::fDressWidgets:
		DressWidgets_( C_(), List);
		break;
	case xdhcmn::fSetContents:
		SetContents_( C_(), List);
		break;
	case xdhcmn::fInsertCSSRule:
		InsertCSSRule_( C_(), List );
		break;
	case xdhcmn::fAppendCSSRule:
		AppendCSSRule_( C_(), Result, List );
		break;
	case xdhcmn::fRemoveCSSRule:
		RemoveCSSRule_( C_(), List );
		break;
	case xdhcmn::fAddClasses:
		HandleClasses_( C_(), xdhujs::snClassesAdder, List );
		break;
	case xdhcmn::fRemoveClasses:
		HandleClasses_( C_(), xdhujs::snClassesRemover, List );
		break;
	case xdhcmn::fToggleClasses:
		HandleClasses_( C_(), xdhujs::snClassesToggler, List );
		break;
	case xdhcmn::fEnableElements:
		EnableElements_( C_(), List );
		break;
	case xdhcmn::fDisableElements:
		DisableElements_( C_(), List );
		break;
	default:
		qRFwk();
		break;
	}
}

