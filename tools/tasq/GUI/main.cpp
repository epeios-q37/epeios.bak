/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'TASq' software.

  'TASq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'TASq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'TASq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "registry.h"

using namespace main;

sclx::action_handler<sSession> main::Core;

#define D_( name )\
  namespace actions_ {\
    SCLX_ADec( sSession, name );\
  }\
  SCLX_ADef( sSession, actions_, name )


D_( OnNewSession )
{
qRH;
  str::wString Body;
qRB;
  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);
qRR;
qRT;
qRE;
}

namespace {
  using namespace actions_;

  namespace _ {
    template <typename action> void Add(
      sclx::action_handler<sSession> &Core,
      action &Action )
    {
        Core.Add(Action.Name, Action);
    }

    template <typename action, typename... actions> void Add(
      sclx::action_handler<sSession> &Core,
      action &Action,
      actions &...Actions)
    {
        Add(Core, Action);
        Add(Core, Actions...);
    }
  }

  void Register_(void)
  {
    _::Add(Core, OnNewSession);
  }
}

#define R_( name ) Add_(main::Core, actions_::name)

qGCTOR( main ) {
  Core.Init();
  Register_();
}
