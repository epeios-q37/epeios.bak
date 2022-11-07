/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'MSFGq' software.

  'MSFGq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'MSFGq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'MSFGq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

#include "sclm.h"

using namespace registry;

// Parameters.

rEntry registry::parameter::BaseOctave( "BaseOctave", sclr::Parameters);

namespace {
	rEntry Signature_( "Signature", sclr::Parameters );
}

rEntry registry::parameter::signature::Key( "Key", Signature_ );
rEntry registry::parameter::signature::Time( "Time", Signature_ );

namespace {
	rEntry Devices_( "Devices", sclr::Parameters );
	rEntry In_( "In", Devices_ );
	rEntry Out_( "Out", Devices_ );
}

rEntry registry::parameter::Width( "Width", sclr::Parameters);

rEntry registry::parameter::devices::in::Value = In_;
rEntry registry::parameter::devices::in::Policy( "@Policy", In_ );

rEntry registry::parameter::devices::out::Value = Out_;
rEntry registry::parameter::devices::out::Policy( "@Policy", Out_ );

// Definitions.

rEntry registry::definition::Head( "Head", sclr::Definitions);
rEntry registry::definition::Body( "Body", sclr::Definitions);

namespace {
  rEntry Scripts_( "Scripts", sclr::Definitions );
  rEntry UntaggedScript_("Script", Scripts_);
  rEntry TaggedScript_(RGSTRY_TAGGING_ATTRIBUTE( "id" ), UntaggedScript_);
}

rEntry registry::definition::script::Id("@id", UntaggedScript_);
rEntry registry::definition::script::tagged::Label("@Label", TaggedScript_);
rEntry registry::definition::script::tagged::Description("@Description", TaggedScript_);
rEntry registry::definition::script::tagged::Mime("@Mime", TaggedScript_);
rEntry registry::definition::script::tagged::Content = TaggedScript_;


const str::dStrings &registry::GetScriptIds(str::dStrings &Ids)
{
  sclm::GetValues(registry::definition::script::Id, Ids);

  return Ids;
}

void registry::GetScriptFeature(
  const str::dString &Id,
  str::dString &Label,
  str::dString &Description)
{
  sclm::MGetValue(rgstry::rTEntry(registry::definition::script::tagged::Label, Id), Label);
  sclm::OGetValue(rgstry::rTEntry(registry::definition::script::tagged::Description, Id), Description);
}

void registry::GetScriptContentAndMime(
  const str::dString &Id,
  str::dString &Content,
  str::dString &Mime)
{
  sclm::MGetValue(rgstry::rTEntry(registry::definition::script::tagged::Content, Id), Content);
  sclm::MGetValue(rgstry::rTEntry(registry::definition::script::tagged::Mime, Id), Mime);
}


