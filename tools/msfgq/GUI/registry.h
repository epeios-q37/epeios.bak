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

#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "sclr.h"

namespace registry {
	using namespace sclr;

	namespace parameter {
		using namespace sclr::parameter;

    namespace devices	{
			namespace in {
				extern rEntry
					Policy,
					Value;
			}
			namespace out {
				extern rEntry
					Policy,
					Value;
			}
    }

    extern rEntry BaseOctave;

		namespace signature {
			extern rEntry
        Key,
        Time;
		}

		extern rEntry Width;
	}

	namespace definition {
		using namespace sclr::definition;

		extern rEntry Body;

		namespace script {
		  namespace tagged {
		    extern rEntry
          Label,
          Description,
          Mime,
          Content;
		  }

		  extern rEntry Id;
		}
	}

//	const str::dString &GetWebHost(str::dString &Host);

	const str::dStrings &GetScriptIds(str::dStrings &Ids);

	void GetScriptFeature(
    const str::dString &Id,
    str::dString &Label,
    str::dString &Description);

  void GetScriptContentAndMime(
    const str::dString &Id,
    str::dString &Content,
    str::dString &Mime);
}

#endif
