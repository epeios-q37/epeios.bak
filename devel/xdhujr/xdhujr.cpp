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

#define XDHUJR__COMPILATION

#include "xdhujr.h"

#include "sclrgstry.h"

using namespace xdhujr;

using rgstry::entry___;

entry___ xdhujr::CustomItems( "CustomItems", sclrgstry::Definitions );

entry___ xdhujr::custom_item::AttributeNames("AttributeNames", CustomItems );
entry___ xdhujr::custom_item::attribute_name::Widget( "Widget", AttributeNames );
entry___ xdhujr::custom_item::attribute_name::Result( "Result", AttributeNames );
/*
entry___ xdhujr::custom_item::attribute_name::OnEvent( "OnEvent", AttributeNames );
entry___ xdhujr::custom_item::attribute_name::OnEvents( "OnEvents", AttributeNames );
entry___ xdhujr::custom_item::attribute_name::Cast( "Cast", AttributeNames );
entry___ xdhujr::custom_item::attribute_name::Casts( "Casts", AttributeNames );
*/


entry___ xdhujr::Scripts( "Scripts", sclrgstry::Definitions );

static rEntry ChildScripts_( "Child", xdhujr::Scripts );
rEntry xdhujr::script::child::Insert( "Insert", ChildScripts_ );
rEntry xdhujr::script::child::Append( "Append", ChildScripts_ );
rEntry xdhujr::script::child::First( "First", ChildScripts_ );
rEntry xdhujr::script::child::Last( "Last", ChildScripts_ );

static rEntry SiblingScripts_( "Sibling", xdhujr::Scripts );
rEntry xdhujr::script::sibling::Previous( "Previous", SiblingScripts_ );
rEntry xdhujr::script::sibling::Next( "Next", SiblingScripts_ );

static rEntry InsertScripts_( "Insert", xdhujr::Scripts );
rEntry xdhujr::script::insert::Before( "Before", InsertScripts_ );
rEntry xdhujr::script::insert::After( "After", InsertScripts_ );


static entry___ PropertyScripts_( "Property", xdhujr::Scripts );
entry___ xdhujr::script::property::Setter( "Setter", PropertyScripts_ );
entry___ xdhujr::script::property::Getter( "Getter", PropertyScripts_ );

static entry___ AttributeScripts_( "Attribute", xdhujr::Scripts );
entry___ xdhujr::script::attribute::Setter( "Setter", AttributeScripts_ );
entry___ xdhujr::script::attribute::Getter( "Getter", AttributeScripts_ );
entry___ xdhujr::script::attribute::Remover( "Remover", AttributeScripts_ );

static entry___ ValueScripts_( "Value", xdhujr::Scripts );
entry___ xdhujr::script::value::Setter( "Setter", ValueScripts_ );
entry___ xdhujr::script::value::Getter( "Getter", ValueScripts_ );

static entry___ SetterScripts_( "Setter", xdhujr::Scripts );
entry___ xdhujr::script::setter::Layout("Layout", SetterScripts_);
entry___ xdhujr::script::setter::EventHandlers( "EventHandlers", SetterScripts_ );
entry___ xdhujr::script::setter::Contents( "Contents", SetterScripts_ );
entry___ xdhujr::script::setter::Timeout( "Timeout", SetterScripts_ );


entry___ xdhujr::script::DraggableSetter( "DraggableSetter", xdhujr::Scripts );
entry___ xdhujr::script::DroppableSetter( "DroppableSetter", xdhujr::Scripts );

static entry___ DialogScripts_( "Dialog", xdhujr::Scripts );
entry___ xdhujr::script::dialog::Alert( "Alert", DialogScripts_ );
entry___ xdhujr::script::dialog::Confirm( "Confirm", DialogScripts_ );

static entry___ WidgetScripts_( "Widget", xdhujr::Scripts );
entry___ xdhujr::script::widget::ValueRetriever( "ValueRetriever", WidgetScripts_ );
entry___ xdhujr::script::widget::Focuser( "Focuser", WidgetScripts_ );
entry___ xdhujr::script::widget::Selector( "Selector", WidgetScripts_ );
entry___ xdhujr::script::widget::Fetcher( "Fetcher", WidgetScripts_ );

entry___ xdhujr::script::Log( "Log", xdhujr::Scripts );
entry___ xdhujr::script::Focuser( "Focuser", Scripts );
entry___ xdhujr::script::Selector( "Selector", Scripts );
entry___ xdhujr::script::Parent( "Parent", Scripts );
entry___ xdhujr::script::CreateElement( "CreateElement", Scripts );
entry___ xdhujr::script::WidgetsInstantiator( "WidgetsInstantiator",Scripts );

static rEntry CSSRule_( "CSSRule", xdhujr::Scripts );
rEntry xdhujr::script::css_rule::Insert( "Insert", CSSRule_ );
rEntry xdhujr::script::css_rule::Append( "Append", CSSRule_ );
rEntry xdhujr::script::css_rule::Remove( "Remove", CSSRule_ );

static rEntry Classes_( "Classes", xdhujr::Scripts );
rEntry xdhujr::script::classes::Add( "Add", Classes_ );
rEntry xdhujr::script::classes::Remove( "Remove", Classes_ );
rEntry xdhujr::script::classes::Toggle( "Toggle", Classes_ );

static rEntry Elements_( "Elements", xdhujr::Scripts );
rEntry xdhujr::script::elements::Enable( "Enable", Elements_ );
rEntry xdhujr::script::elements::Disable( "Disable", Elements_ );

entry___ xdhujr::script::Dummy( "Dummy", Scripts );
