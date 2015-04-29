/*
	'dhtjsbased.cpp' by Claude SIMON (http://zeusw.org/).

	'dhtjsbased' is part of the Epeios framework.

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

#define DHTJSBASED__COMPILATION

#include "dhtjsbased.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace dhtjsbased;

using xhtcllbk::nstring___;
using xhtcllbk::nchar__;

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
	xhtcllbk::Escape( str::string( Value.UTF8( Buffer ) ), EscapedValue, '"' );
	Values.Append( EscapedValue );

	NameForRawValue.Init( Name );
	NameForRawValue.Append( '_' );

	Names.Append( NameForRawValue );
	Values.Append( str::string( Value.UTF8( Buffer ) ) );
ERRErr
ERREnd
ERREpilog
}

static void SetProperty_(
	callback__ &Callback,
	const nstring___ &Id,
	const nstring___ &Name,
	const nstring___ &Value )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
ERRBegin
	Script.Init();
	Callback.GetScript( sPropertySetter, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Name", Name, TagNames, TagValues );
	AppendTag_( "Value", Value, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script );
ERRErr
ERREnd
ERREpilog
}

static const char *GetProperty_(
	callback__ &Callback,
	const nstring___ &Id,
	const nstring___ &Name,
	TOL_CBUFFER___ &Buffer )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
	TOL_CBUFFER___ EscapedBuffer;
ERRBegin
	Script.Init();
	Callback.GetScript( sPropertyGetter, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Name", Name, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script, Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

static void SetAttribute_(
	callback__ &Callback,
	const nstring___ &Id,
	const nstring___ &Name,
	const nstring___ &Value )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
ERRBegin
	Script.Init();
	Callback.GetScript( sAttributeSetter, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Name", Name, TagNames, TagValues );
	AppendTag_( "Value", Value, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script );
ERRErr
ERREnd
ERREpilog
}

static const char *GetAttribute_(
	callback__ &Callback,
	const nstring___ &Id,
	const nstring___ &Name,
	TOL_CBUFFER___ &Buffer )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
	TOL_CBUFFER___ EscapedBuffer;
ERRBegin
	Script.Init();
	Callback.GetScript( sAttributeGetter, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Name", Name, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script, Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

static const str::string_ &GetAttribute_(
	callback__ &Callback,
	const nstring___ &Id,
	const nstring___ &Name,
	str::string_ &Value )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Value.Append( GetAttribute_( Callback, Id, Name, Buffer ) );
ERRErr
ERREnd
ERREpilog
	return Value;
}

static void RemoveAttribute_(
	callback__ &Callback,
	const nstring___ &Id,
	const nstring___ &Name )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
ERRBegin
	Script.Init();
	Callback.GetScript( sAttributeRemover, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Name", Name, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script );
ERRErr
ERREnd
ERREpilog
}

static void GetWidgetFeatures_(
	const str::string_ &MergedArgs,
	str::string_ &Type,
	str::string_ &Parameters,
	str::string_ &ContentRetrievingMethod,
	str::string_ &FocusingMethod )
{
ERRProlog
	xhtcllbk::args Args;
	xhtcllbk::retriever__ Retriever;
ERRBegin
	Args.Init();
	xhtcllbk::Split( MergedArgs, Args );

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

static const char *GetRegularContent_(
	callback___ &Callback,
	const nstring___ &Id,
	TOL_CBUFFER___ &Buffer )
{
	const char *Result = NULL;
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
ERRBegin
	Script.Init();
	Callback.GetScript( sContentGetter, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag( "Id", Id, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Result = ExecuteJavascript( Script, Buffer, Frame );
ERRErr
ERREnd
ERREpilog
	return Result;
}

static const char *GetContent_(
	const nstring___ &Id,
	TOL_CBUFFER___ &Buffer )
{
	const char *Result = NULL;
ERRProlog
	str::string Args, Method;
	TOL_CBUFFER___ ABuffer;
ERRBegin
	Args.Init();
	misc::GetAttribute( Id, sclmisc::MGetValue( registry::WidgetAttributeName, ABuffer ), Frame, Args );

	Method.Init();

	if ( Args.Amount() != 0 )
		GetWidgetContentRetrievingMethod_( Args, Method );

	if ( Method.Amount() == 0 )
		Result = GetRegularContent_( Frame, Id, Buffer );
	else
		Result = GetWidgetContent_( Frame, Id, Method, Buffer );
ERRErr
ERREnd
ERREpilog
	return Result;
}


static void Alert_(
	callback__ &Callback,
	const nchar__ *XML,
	const nchar__ *XSL,
	const nchar__ *Title )
{
ERRProlog
	str::string Script, CloseText;
	str::strings TagNames, TagValues;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Script.Init();
	Callback.GetScript( sDialogAlert, Script );

	CloseText.Init();
	Callback.GetTranslation( "CloseText", CloseText );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "XML", XML, TagNames, TagValues );
	AppendTag_( "XSL", XSL, TagNames, TagValues );
	AppendTag_( "Title", Title, TagNames, TagValues );
	AppendTag_( "CloseText", CloseText, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script );
ERRErr
ERREnd
ERREpilog
}

static void Alert_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *XML = va_arg( List, const nchar__ *);
	const nchar__ *XSL = va_arg( List, const nchar__ *);
	const nchar__ *Title = va_arg( List, const nchar__ *);

	Alert_( Callback, XML, XSL, Title );
}

static void Confirm_(
	callback__ &Callback,
	const nchar__ *XML,
	const nchar__ *XSL,
	const nchar__ *Title,
	TOL_CBUFFER___ &Buffer )
{
ERRProlog
	str::string Script, CloseText;
	str::strings TagNames, TagValues;
	TOL_CBUFFER___ LBuffer;
	str::string Response;
ERRBegin
	Script.Init();
	Callback.GetScript( sDialogConfirm, Script );

	CloseText.Init();
	Callback.GetTranslation( "CloseText", CloseText );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "XML", XML, TagNames, TagValues );
	AppendTag_( "XSL", XSL, TagNames, TagValues );
	AppendTag_( "Title", Title, TagNames, TagValues );
	AppendTag_( "CloseText", CloseText, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

#if 1
	Callback.Execute( Script, Buffer );
#else
	Callback.Execute( Script, _F() );

	Response.Init();
	misc::WaitForResponse( Response );

	Response.Convert( Buffer );
#endif
ERRErr
ERREnd
ERREpilog
}

static void Confirm_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *XML = va_arg( List, const nchar__ *);
	const nchar__ *XSL = va_arg( List, const nchar__ * );
	const nchar__ *Title = va_arg( List, const nchar__ * );
	TOL_CBUFFER___ &Buffer = *va_arg( List, TOL_CBUFFER___ *);

	Confirm_( Callback, XML, XSL, Title, Buffer );
}

static void SetChildren_(
	callback__ &Callback,
	const nchar__ *Id,	// If == NULL, the entire document is replaced.
	const nchar__ *XML,
	const nchar__ *XSL )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
	nstring___ RootTagId;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Script.Init();

	if ( Id == NULL ) {
		RootTagId.Init( sclmisc::MGetValue( registry::RootTagId, Buffer ) );
		Id = RootTagId.Internal();
		Callback.GetScript( sDocumentSetter, Script );
	} else
		Callback.GetScript( sChildrenSetter, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "XML", XML, TagNames, TagValues );
	AppendTag_( "XSL", XSL, TagNames, TagValues );
	AppendTag_( "Id", Id, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script );

	HandleExtensions_( Id, _F(), _P(), _B() );
ERRErr
ERREnd
ERREpilog
}

static void SetChildren_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	SetChildren_( Callback, Id, XML, XSL );
}

void _SetCasting(
	callback__ &Callback,
	const nchar__ *Id,	// If == NULL, the casting is applied to the entire document.
	const nchar__ *XML,
	const nchar__ *XSL )
{
ERRProlog
	str::string Script;
	str::strings TagNames, TagValues;
	TOL_CBUFFER___ Buffer;
	nstring___ RootTagId;
ERRBegin
	Script.Init();
	Callback.GetScript( sCastingDefiner, Script );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "XML", XML, TagNames, TagValues );
	AppendTag_( "XSL", XSL, TagNames, TagValues );

	tagsbs::SubstituteLongTags( Script, TagNames, TagValues );

	Callback.Execute( Script );

	if ( Id == NULL ) {
		RootTagId.Init( sclmisc::MGetValue( registry::RootTagId, Buffer ) );
		Id = RootTagId.Internal();
	}

	HandleCastings_( Id, _P(), _B() );
ERRErr
ERREnd
ERREpilog
}

static void SetCasting_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *XML = va_arg( List, const nchar__ * );
	const nchar__ *XSL = va_arg( List, const nchar__ * );

	_SetCasting( Callback, Id, XML, XSL );
}

static void SetProperty_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *Name = va_arg( List, const nchar__ * );
	const nchar__ *Value = va_arg( List, const nchar__ * );

	SetProperty_( Callback, Id, Name, Value );
}

static void GetProperty_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *Name = va_arg( List, const nchar__ * );
	TOL_CBUFFER___ &Buffer = *va_arg( List, TOL_CBUFFER___ *);

	GetProperty_( Callback, Id, Name, Buffer );
}

static void SetAttribute_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *Name = va_arg( List, const nchar__ * );
	const nchar__ *Value = va_arg( List, const nchar__ * );

	SetAttribute_( Callback, Id, Name, Value );
}

static void GetAttribute_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *Name = va_arg( List, const nchar__ * );
	TOL_CBUFFER___ &Buffer = *va_arg( List, TOL_CBUFFER___ *);

	GetAttribute_( Callback, Id, Name, Buffer );
}

static void RemoveAttribute_(
	callback__ &Callback,
	va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *Name = va_arg( List, const nchar__ * );

	RemoveAttribute_( Callback, Id, Name );
}

static void GetResult_(
	callback__ &Callback,
	const nchar__ *Id,
	TOL_CBUFFER___ &Buffer )
{
ERRProlog
	str::string ResultAttributeName;
ERRBegin
	ResultAttributeName.Init();
	sclmisc::MGetValue( registry::ResultAttributeName, ResultAttributeName );
	GetAttribute_( Callback, Id, ResultAttributeName, Buffer );
ERRErr
ERREnd
ERREpilog
}

void _GetResult( va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	TOL_CBUFFER___ &Buffer = *va_arg( List, TOL_CBUFFER___ *);

	_GetResult( Id, Buffer );
}

void _GetContent( va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	TOL_CBUFFER___ &Buffer = *va_arg( List, TOL_CBUFFER___ *);

	misc::GetContent( Id, _F(), Buffer );
}

void _SetContent( va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );
	const nchar__ *Value = va_arg( List, const nchar__ * );

	misc::SetContent( Id, Value, _F() );
}

void _Focus( va_list List )
{
	// NOTA : we use variables, because if we put 'va_arg()' directly as parameter to below '_Alert()', it's not sure that they are called in the correct order.
	const nchar__ *Id = va_arg( List, const nchar__ * );

	misc::Focus( Id, _F() );
}

#define F( name )\
	case xhtcllbk::f##name:\
	name##_( List );\
	break\

void dhtjsbased::proxy_callback__::XHTCLLBKProcess(
	xhtcllbk::function__ Function,
	... )
{
ERRProlog
	va_list List;
ERRBegin
	va_start( List, Function );
	switch ( Function ) {
	F( Alert );
	F( Confirm );
	F( SetChildren );
	F( SetCasting );
	F( SetProperty );
	F( GetProperty );
	F( SetAttribute );
	F( GetAttribute );
	F( RemoveAttribute );
	F( GetResult );
	F( SetContent );
	F( GetContent );
	F( Focus );
	default:
		ERRFwk();
	}
ERRErr
ERREnd
	va_end( List );
ERREpilog
}

