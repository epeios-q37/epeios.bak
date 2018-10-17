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

#define XDHUJS__COMPILATION

#include "xdhujs.h"

#include "xdhujr.h"

#include "sclmisc.h"

#include "stsfsm.h"

using namespace xdhujs;

#define C( name, entry )\
	case sn##name:\
		sclmisc::MGetValue( xdhujr::script::entry, Buffer );\
		break

const str::string_ &xdhujs::GetTaggedScript(
	script_name__ Script,
	str::string_ &Buffer )
{
	switch ( Script ) {
	case snExecute:
		Buffer = "%Value_%";	// The script to execute is given as is.
		break;
	C( Log, Log );
	C( DialogAlert, dialog::Alert );
	C( DialogConfirm, dialog::Confirm );
	C( LayoutSetter, setter::Layout );
	C( PropertySetter, property::Setter );
	C( PropertyGetter, property::Getter );
	C( AttributeSetter, attribute::Setter );
	C( AttributeGetter, attribute::Getter );
	C( AttributeRemover, attribute::Remover );
	C( WidgetValueRetriever, widget::ValueRetriever );
	C( ValueSetter, value::Setter );
	C( ValueGetter, value::Getter );
	C( WidgetFocuser, widget::Focuser );
	C( WidgetSelector, widget::Selector );
	C( WidgetFetcher, widget::Fetcher );
	C( Focuser, Focuser );
	C( Selector, Selector );
	C( EventHandlersSetter, setter::EventHandlers );
	C( ContentsSetter, setter::Contents );
	C( TimeoutSetter, setter::Timeout );
	C( Parent, Parent );
	C( FirstChild, child::First );
	C( LastChild, child::Last );
	C( PreviousSibling, sibling::Previous );
	C( NextSibling, sibling::Next );
	C( CreateElement, CreateElement );
	C( InsertChild, child::Insert );
	C( AppendChild, child::Append );
	C( InsertBefore, insert::Before );
	C( InsertAfter, insert::After );
	C( WidgetsInstantiator, WidgetsInstantiator );
	/*
	C( CSSRuleInserter, css_rule::Insert );
	C( CSSRuleAppender, css_rule::Append );
	C( CSSRuleRemover, css_rule::Remove );
	*/
	C( ClassesAdder, classes::Add );
	C( ClassesRemover, classes::Remove );
	C( ClassesToggler, classes::Toggle );
	C( ElementsEnabler, elements::Enable );
	C( ElementsDisabler, elements::Disable );
	C( Dummy, Dummy );
	default:
		qRFwk();
		break;
	}

	return Buffer;
}

namespace {
	void AppendTag_(
		const char *Name,
		const nstring___ &Value,
		str::strings_ &Names,
		str::strings_ &Values )
	{
	qRH
		str::string NameForRawValue;
		str::string EscapedValue;
		TOL_CBUFFER___ Buffer;
	qRB
		Names.Append( str::string( Name ) );
		EscapedValue.Init();
		xdhcmn::Escape( str::string( Value.UTF8( Buffer ) ), EscapedValue, '"' );
		Values.Append( EscapedValue );

		NameForRawValue.Init( Name );
		NameForRawValue.Append( '_' );

		Names.Append( NameForRawValue );
		Values.Append( str::string( Value.UTF8( Buffer ) ) );
	qRR
	qRT
	qRE
	}

	void SubstituteTags_(
		str::string_ &Script,	// Script with tags. When returning, tags are substitued.
		va_list ValueList,
		... )	// The list of the tag name, as 'const char *', with 'NULL' as end marker.
	{
	qRH
		str::strings Names, Values;
		va_list NameList;
		const bso::char__ *Name = NULL;
	qRB
		Names.Init();
		Values.Init();

		va_start( NameList, ValueList );

		Name = va_arg( NameList, const bso::char__ * );

		while ( Name != NULL ) {
			AppendTag_( Name, va_arg( ValueList, const nchar__ * ), Names, Values );

			Name = va_arg( NameList, const bso::char__ * );
		}

		tagsbs::SubstituteLongTags( Script, Names, Values );
	qRR
	qRT
		va_end( NameList );
	qRE
	}
}

#define D( name )\
	E_CDEF( char *, name##_, #name )

D( Message );
D( Id );
D( Name );
D( Child );
D( Sibling );
D( Value );
D( Method );
D( Delay );
D( Action );
D( XML );
D( XSL );
D( Title );
D( CloseText );
D( Cast );
D( Ids );
D( Events );
D( Casts );
D( Contents );
D( Types );
D( ParametersSets );
D( Rule );
D( Index );
D( Classes );

#define S( name, ... )\
	case sn##name:\
	SubstituteTags_( TaggedScript, List, __VA_ARGS__ );\
	break\

void xdhujs::GetScript(
	script_name__ ScriptName,
	str::string_ &Script,
	va_list List )
{
qRH
	str::string TaggedScript;
qRB
	TaggedScript.Init();
	GetTaggedScript( ScriptName, TaggedScript );

	switch ( ScriptName ) {
	S( Execute, Value_, NULL );
	S( Log, Message_, NULL );
	S( DialogAlert, XML_, XSL_, Title_, CloseText_, NULL );
	S( DialogConfirm, XML_, XSL_, Title_, CloseText_, NULL );
	S( AttributeSetter, Id_, Name_, Value_, NULL  );
	S( AttributeGetter, Id_, Name_, NULL  );
	S( AttributeRemover, Id_, Name_, NULL  );
	S( PropertySetter, Id_, Name_, Value_, NULL );
	S( PropertyGetter, Id_, Name_, NULL );
	S( LayoutSetter, Id_, XML_, XSL_, NULL );
	S( ValueSetter, Id_, Value_, NULL );
	S( ValueGetter, Id_, NULL );
	S( WidgetValueRetriever, Id_, Method_, NULL );
	S( WidgetFocuser, Id_, Method_, NULL );
	S( WidgetSelector, Id_, Method_, NULL );
	S( WidgetFetcher, Id_, NULL );
	S( Focuser, Id_, NULL );
	S( Selector, Id_, NULL );
	S( EventHandlersSetter, Ids_, Events_, NULL );
	S( ContentsSetter, Ids_, Contents_, NULL );
	S( TimeoutSetter, Delay_, Action_, NULL );
	S( Parent, Id_, NULL );
	S( FirstChild, Id_, NULL );
	S( LastChild, Id_, NULL );
	S( PreviousSibling, NULL );
	S( NextSibling, NULL );
	S( CreateElement, Name_, Id_, NULL );
	S( InsertChild, Child_, Id_, NULL );
	S( AppendChild, Child_, Id_, NULL );
	S( InsertBefore, Sibling_, Id_, NULL );
	S( InsertAfter, Sibling_, Id_, NULL );
	S( WidgetsInstantiator, Ids_, Types_, ParametersSets_, NULL );
	/*
	S( CSSRuleInserter, Rule_, Index_, NULL );
	S( CSSRuleAppender, Rule_, NULL );
	S( CSSRuleRemover, Index_, NULL );
	*/
	S( ClassesAdder, Ids_, Classes_, NULL );
	S( ClassesRemover, Ids_, Classes_, NULL );
	S( ClassesToggler, Ids_, Classes_, NULL );
	S( ElementsEnabler, Ids_, NULL );
	S( ElementsDisabler, Ids_, NULL );
	default:
		qRFwk();
		break;
	}

	Script.Append( TaggedScript );
qRR
qRT
qRE
}

const str::string_ &xdhujs::GetScript(
	script_name__ ScriptName,
	str::string_ *Buffer,
	... )
{
qRH
	va_list List;
qRB
	va_start( List, Buffer );
	
	GetScript( ScriptName, *Buffer, List );
qRR
qRT
	va_end( List );
qRE
	return *Buffer;
}

