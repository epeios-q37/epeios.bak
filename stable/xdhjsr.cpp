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

#define XDHJSR__COMPILATION

#include "xdhjsr.h"

#include "sclrgstry.h"

using namespace xdhjsr;

using rgstry::entry___;

entry___ xdhjsr::CustomItems( "CustomItems", sclrgstry::Definitions );

entry___ xdhjsr::custom_item::AttributeNames("AttributeNames", CustomItems );
entry___ xdhjsr::custom_item::attribute_name::Widget( "Widget", AttributeNames );
entry___ xdhjsr::custom_item::attribute_name::Result( "Result", AttributeNames );
/*
entry___ xdhjsr::custom_item::attribute_name::OnEvent( "OnEvent", AttributeNames );
entry___ xdhjsr::custom_item::attribute_name::OnEvents( "OnEvents", AttributeNames );
entry___ xdhjsr::custom_item::attribute_name::Cast( "Cast", AttributeNames );
entry___ xdhjsr::custom_item::attribute_name::Casts( "Casts", AttributeNames );
*/


entry___ xdhjsr::Scripts( "Scripts", sclrgstry::Definitions );

static entry___ PropertyScripts_( "Property", xdhjsr::Scripts );
entry___ xdhjsr::script::property::Setter( "Setter", PropertyScripts_ );
entry___ xdhjsr::script::property::Getter( "Getter", PropertyScripts_ );

static entry___ AttributeScripts_( "Attribute", xdhjsr::Scripts );
entry___ xdhjsr::script::attribute::Setter( "Setter", AttributeScripts_ );
entry___ xdhjsr::script::attribute::Getter( "Getter", AttributeScripts_ );
entry___ xdhjsr::script::attribute::Remover( "Remover", AttributeScripts_ );

static entry___ ContentScripts_( "Content", xdhjsr::Scripts );
entry___ xdhjsr::script::content::Setter( "Setter", ContentScripts_ );
entry___ xdhjsr::script::content::Getter( "Getter", ContentScripts_ );

entry___ xdhjsr::script::ElementFiller( "ElementFiller", xdhjsr::Scripts );
entry___ xdhjsr::script::DocumentFiller( "DocumentFiller", xdhjsr::Scripts );
entry___ xdhjsr::script::CastingFiller( "CastingFiller", xdhjsr::Scripts );

entry___ xdhjsr::script::DraggableSetter( "DraggableSetter", xdhjsr::Scripts );
entry___ xdhjsr::script::DroppableSetter( "DroppableSetter", xdhjsr::Scripts );

static entry___ DialogScripts_( "Dialog", xdhjsr::Scripts );
entry___ xdhjsr::script::dialog::Alert( "Alert", DialogScripts_ );
entry___ xdhjsr::script::dialog::Confirm( "Confirm", DialogScripts_ );

static entry___ WidgetScripts_( "Widget", xdhjsr::Scripts );
entry___ xdhjsr::script::widget::Instantiator( "Instantiator", WidgetScripts_ );
entry___ xdhjsr::script::widget::ContentRetriever( "ContentRetriever", WidgetScripts_ );
entry___ xdhjsr::script::widget::Focuser( "Focuser", WidgetScripts_ );

entry___ xdhjsr::script::Log( "Log", xdhjsr::Scripts );
entry___ xdhjsr::script::Focuser( "Focuser", Scripts );
entry___ xdhjsr::script::EventSetter( "EventSetter", Scripts );
entry___ xdhjsr::script::CastHandler( "CastHandler", Scripts );

