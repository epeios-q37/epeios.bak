/*
	'xdhjsp.cpp' by Claude SIMON (http://zeusw.org/).

	'xdhjsp' is part of the Epeios framework.

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

#define XDHJSP__COMPILATION

#include "xdhjsp.h"

#include "sclrgstry.h"
#include "sclmisc.h"

using namespace xdhjsp;

using rgstry::entry___;

static entry___ Scripts_( "Scripts", sclrgstry::Definitions );

static entry___ PropertyScripts_( "Property", Scripts_ );
entry___ xdhjsp::script::property::Setter( "Setter", PropertyScripts_ );
entry___ xdhjsp::script::property::Getter( "Getter", PropertyScripts_ );

static entry___ AttributeScripts_( "Attribute", Scripts_ );
entry___ xdhjsp::script::attribute::Setter( "Setter", AttributeScripts_ );
entry___ xdhjsp::script::attribute::Getter( "Getter", AttributeScripts_ );
entry___ xdhjsp::script::attribute::Remover( "Remover", AttributeScripts_ );

static entry___ ContentScripts_( "Content", Scripts_ );
entry___ xdhjsp::script::content::Setter( "Setter", ContentScripts_ );
entry___ xdhjsp::script::content::Getter( "Getter", ContentScripts_ );

entry___ xdhjsp::script::DocumentSetter( "DocumentSetter", Scripts_ );
entry___ xdhjsp::script::ChildrenSetter( "ChildrenSetter", Scripts_ );

static entry___ DialogScripts_( "Dialog", Scripts_ );
entry___ xdhjsp::script::dialog::Alert( "Alert", DialogScripts_ );
entry___ xdhjsp::script::dialog::Confirm( "Confirm", DialogScripts_ );

static entry___ WidgetScripts_( "Widget", Scripts_ );
entry___ xdhjsp::script::widget::Instantiation( "Instantiation", WidgetScripts_ );
entry___ xdhjsp::script::widget::ContentRetriever( "ContentRetriever", WidgetScripts_ );
entry___ xdhjsp::script::widget::Focusing( "Focusing", WidgetScripts_ );

static entry___ CastingScripts_( "Casting", Scripts_ );
entry___ xdhjsp::script::casting::Definer( "Definer", CastingScripts_ );
entry___ xdhjsp::script::casting::Handler( "Handler", CastingScripts_ );

entry___ xdhjsp::script::Log( "Log", Scripts_ );
entry___ xdhjsp::script::Focusing( "Focusing", Scripts_ );

#define C( name, entry )\
	case sn##name:\
		sclmisc::MGetValue( script::entry, Buffer );\
		break

const str::string_ &xdhjsp::GetTaggedScript(
	xdhjsp::script_name__ Script,
	str::string_ &Buffer )
{
	switch ( Script ) {
	C( Log, dialog::Alert );
	C( DialogAlert, dialog::Alert );
	C( DialogConfirm, dialog::Confirm );
	C( DocumentSetter, DocumentSetter );
	C( ChildrenSetter, ChildrenSetter );
	C( CastingDefiner, casting::Definer );
	C( CastingHandler, casting::Handler );
	C( PropertySetter, property::Setter );
	C( PropertyGetter, property::Getter );
	C( AttributeSetter, attribute::Setter );
	C( AttributeGetter, attribute::Getter );
	C( AttributeRemover, attribute::Remover );
	C( WidgetContentRetriever, widget::ContentRetriever );
	C( ContentSetter, content::Setter );
	C( ContentGetter, content::Getter );
	C( WidgetFocusing, widget::Focusing );
	C( Focusing, Focusing );
	default:
		ERRFwk();
		break;
	}

	return Buffer;
}

using xdhcbk::nstring___;
using xdhcbk::nchar__;

static void AppendTag_(
	const char *Name,
	const nstring___ &Value,
	str::strings_ &Names,
	str::strings_ &Values )
{
ERRProlog
	str::string NameForRawValue;
	str::string EscapedValue;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Names.Append( str::string( Name ) );
	EscapedValue.Init();
	xdhcbk::Escape( str::string( Value.UTF8( Buffer ) ), EscapedValue, '"' );
	Values.Append( EscapedValue );

	NameForRawValue.Init( Name );
	NameForRawValue.Append( '_' );

	Names.Append( NameForRawValue );
	Values.Append( str::string( Value.UTF8( Buffer ) ) );
ERRErr
ERREnd
ERREpilog
}

static void SubstituteTags_(
	str::string_ &Script,	// Script with tags. When returning, tags are substitued.
	va_list ValueList,
	... )	// The list of the tag name, as 'const char *', with 'NULL' as end marker.
{
ERRProlog
	str::strings Names, Values;
	va_list NameList;
	const bso::char__ *Name = NULL;
ERRBegin
	Names.Init();
	Values.Init();

	va_start( NameList, ValueList );

	Name = va_arg( NameList, const bso::char__ * );

	while ( Name != NULL ) {
		AppendTag_( Name, va_arg( ValueList, const nchar__ * ), Names, Values );

		Name = va_arg( NameList, const bso::char__ * );
	}

	tagsbs::SubstituteLongTags( Script, Names, Values );
ERRErr
ERREnd
	va_end( NameList );
ERREpilog
}

#define D( name )\
	E_CDEF( char *, name##_, #name )

D( Message );
D( Id );
D( Name );
D( Value );
D( Method );
D( XML );
D( XSL );
D( Title );
D( CloseText );
D( Cast );

#define S( name, ... )\
	case sn##name:\
	SubstituteTags_( TaggedScript, List, __VA_ARGS__ );\
	break\

static void GetScript_(
	script_name__ ScriptName,
	str::string_ &Script,
	va_list List )
{
ERRProlog
	str::string TaggedScript;
ERRBegin
	TaggedScript.Init();
	GetTaggedScript( ScriptName, TaggedScript );

	switch ( ScriptName ) {
	S( Log, Message_, NULL );
	S( DialogAlert, XML_, XSL_, Title_, CloseText_, NULL );
	S( DialogConfirm, XML_, XSL_, Title_, CloseText_, NULL );
	S( AttributeSetter, Id_, Name_, Value_, NULL  );
	S( AttributeGetter, Id_, Name_, NULL  );
	S( AttributeRemover, Id_, Name_, NULL  );
	S( PropertySetter, Id_, Name_, Value_, NULL );
	S( PropertyGetter, Id_, Name_, NULL );
	S( DocumentSetter, Id_, XML_, XSL_, NULL );
	S( ContentSetter, Id_, Value_, NULL );
	S( ContentGetter, Id_, NULL );
	S( CastingDefiner, XML_, XSL_, NULL );
	S( CastingHandler, Id_, Cast_, NULL );
	S( ChildrenSetter, Id_, XML_, XSL_, NULL );
	S( WidgetContentRetriever, Id_, Method_, NULL );
	S( WidgetFocusing, Id_, Method_, NULL );
	S( Focusing, Id_, NULL );
	default:
		ERRFwk();
		break;
	}

	Script.Append( TaggedScript );
ERRErr
ERREnd
ERREpilog
}

const str::string_ &xdhjsp::GetScript(
	script_name__ ScriptName,
	str::string_ *Buffer,
	... )
{
ERRProlog
	va_list List;
ERRBegin
	va_start( List, Buffer );
	
	GetScript_( ScriptName, *Buffer, List );
ERRErr
ERREnd
	va_end( List );
ERREpilog
	return *Buffer;
}

static const char *Execute_(
	callback__  &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Buffer,
	va_list List )
{
	const char *Result = NULL;
ERRProlog
	str::string Script;
ERRBegin
	Script.Init();
	GetScript_( ScriptName, Script, List );

	Result = Callback.Execute( Script, Buffer );
ERRErr
ERREnd
ERREpilog
	return Result;
}

const char *xdhjsp::Execute(
	callback__  &Callback,
	script_name__ ScriptName,
	TOL_CBUFFER___ *Buffer,
	... )
{
	const char *Result = NULL;
ERRProlog
	va_list List;
ERRBegin
	va_start( List, Buffer );

	Result = Execute_( Callback, ScriptName, Buffer, List );
ERRErr
ERREnd
	va_end( List );
ERREpilog
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
ERRProlog
	str::string CloseText;
ERRBegin
	CloseText.Init();
	Callback.GetTranslation("CloseText", CloseText );

	Execute( Callback, ScriptName, Result, XML, XSL, Title, nstring___(CloseText).Internal()() );
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	xdhcbk::args Args;
	xdhcbk::retriever__ Retriever;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

static void GetWidgetContentRetrievingMethod_(
	const str::string_ &Args,
	str::string_ &Method )
{
ERRProlog
	str::string Type, Parameters, OtherMethod;
ERRBegin
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	GetWidgetFeatures_( Args, Type, Parameters, Method, OtherMethod );
ERRErr
ERREnd
ERREpilog
}

static void GetContent_(
	callback__ &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ *Result )
{
ERRProlog
	str::string Args, Method;
	nstring___ WidgetAttributeName;;
	TOL_CBUFFER___ Buffer;
ERRBegin
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()) );

	Method.Init();

	if ( Args.Amount() != 0 )	// 'true' if the element 'Id' is a widget.
		GetWidgetContentRetrievingMethod_( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, snContentGetter, Result, Id );
	else
		Execute( Callback, snWidgetContentRetriever, Result, Id, Method );
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	str::string Type, Parameters, OtherMethod;
ERRBegin
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	GetWidgetFeatures_( Args, Type, Parameters, OtherMethod, Method );
ERRErr
ERREnd
ERREpilog
}

static void Focus_(
	callback__ &Callback,
	const nchar__ *Id )
{
ERRProlog
	str::string Args, Method;
	nstring___ WidgetAttributeName;;
	TOL_CBUFFER___ Buffer;
ERRBegin
	WidgetAttributeName.Init( Callback.GetWidgetAttributeName( Buffer ) );
	Args.Init( Execute( Callback, snAttributeGetter, &Buffer, Id, WidgetAttributeName.Internal()) );

	Method.Init();

	if ( Args.Amount() != 0 )
		GetWidgetFocusingMethod_( Args, Method );

	if ( Method.Amount() == 0 )
		Execute( Callback, snFocusing, NULL, Id );
	else
		Execute( Callback, snWidgetFocusing, NULL, Id, Method );
ERRErr
ERREnd
ERREpilog
}

static void Focus_(
	callback__ &Callback,
	va_list List )
{
	Focus_( Callback, va_arg( List, const nchar__ * ) );
}

static void SetChildren_(
	callback__ &Callback,
	const nchar__ *Id,	// If == NULL, the entire document is replaced.
	const nchar__ *XML,
	const nchar__ *XSL )
{
ERRProlog
	script_name__ ScriptName = sn_Undefined;
	nstring___ RootTagId;
	TOL_CBUFFER___ Buffer;
ERRBegin
	if ( Id == NULL ) {
		RootTagId.Init( Callback.GetRootTagId( Buffer ) );
		Id = RootTagId.Internal();
		ScriptName = snDocumentSetter;
	} else
		ScriptName = snChildrenSetter;

	Execute( Callback, ScriptName, NULL, Id, XML, XSL );

	Callback.HandleExtensions( Id );
ERRErr
ERREnd
ERREpilog
}

static void SetChildren_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	SetChildren_( Callback, Id, XML, XSL );
}


static void SetCasting_(
	callback__ &Callback,
	const nchar__ *Id,	// If == NULL, the casting is applied to the entire document.
	const nchar__ *XML,
	const nchar__ *XSL )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
	nstring___ RootTagId;
ERRBegin
	Execute( Callback, snCastingDefiner, NULL, XML, XSL );

	if ( Id == NULL ) {
		RootTagId.Init( Callback.GetRootTagId( Buffer ) );
		Id = RootTagId.Internal();
	}

	Callback.HandleCastings( Id );
ERRErr
ERREnd
ERREpilog
}

static void SetCasting_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below function, it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	SetCasting_( Callback, Id, XML, XSL );
}


static void GetResult_(
	callback__ &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ *Result )
{
ERRProlog
	str::string ResultAttributeName;
	TOL_CBUFFER___ Buffer;
ERRBegin
	ResultAttributeName.Init( Callback.GetResultAttributeName( Buffer ) );
	Execute( Callback, snAttributeGetter, Result, Id );
ERRErr
ERREnd
ERREpilog
}

static void GetResult_(
	callback__ &Callback,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	GetResult_( Callback, va_arg( List, const nchar__ * ), Result );
}


static script_name__ Convert_( xdhcbk::function__ Function )
{
	switch ( Function ) {
	case xdhcbk::fAlert:
		return snDialogAlert;
		break;
	case xdhcbk::fConfirm:
		return snDialogConfirm;
		break;
	case xdhcbk::fSetChildren:
		ERRFwk();
		break;
	case xdhcbk::fSetCasting:
		ERRFwk();
		break;
	case xdhcbk::fSetProperty:
		return snPropertySetter;
		break;
	case xdhcbk::fGetProperty:
		return snPropertyGetter;
		break;
	case xdhcbk::fSetAttribute:
		return snAttributeSetter;
		break;
	case xdhcbk::fGetAttribute:
		return snAttributeGetter;
		break;
	case xdhcbk::fRemoveAttribute:
		return snAttributeRemover;
		break;
	case xdhcbk::fGetResult:
		ERRFwk();
		break;
	case xdhcbk::fSetContent:
		return snContentSetter;
		break;
	case xdhcbk::fGetContent:
		ERRFwk();
		break;
	case xdhcbk::fFocus:
		ERRFwk();
		break;
	default:
		ERRFwk();
		break;
	}

	return sn_Undefined;	// To avoid a warning.
}

void xdhjsp::proxy_callback__::XDHCBKProcess(
	xdhcbk::function__ Function,
	TOL_CBUFFER___ *Result,
	va_list List )
{
	switch ( Function ) {
	case xdhcbk::fSetProperty:
	case xdhcbk::fGetProperty:
	case xdhcbk::fSetAttribute:
	case xdhcbk::fGetAttribute:
	case xdhcbk::fRemoveAttribute:
	case xdhcbk::fSetContent:
		Execute_( _C(), Convert_( Function ), Result, List );
		break;
	case xdhcbk::fAlert:
	case xdhcbk::fConfirm:
		AlertConfirm_( _C(), Convert_( Function ), Result, List );
		break;
	case xdhcbk::fGetResult:
		GetResult_( _C(), Result, List );
		break;
	case xdhcbk::fSetChildren:
		SetChildren_( _C(), List );
		break;
	case xdhcbk::fSetCasting:
		SetCasting_( _C(), List );
		break;
	case xdhcbk::fGetContent:
		GetContent_( _C(), Result, List );
		break;
	case xdhcbk::fFocus:
		Focus_( _C(), List);
		break;
	default:
		ERRFwk();
		break;
	}
}

