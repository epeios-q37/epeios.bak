/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'mscfdraftq' tool.

    'mscfdraftq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mscfdraftq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mscfdraftq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MESSAGES_INC_
# define MESSAGES_INC_

# include "tol.h"

namespace messages {
  qENUM( Name ) {
    nNoAvailableMIDIOutDevices,
    nAvailableMIDIOutDevices,
    nNoAvailableMIDIInDevices,
    nAvailableMIDIInDevices,
    nSavingFile,
    nFileSavingDone,
    nExecutingScript,
    nScriptExecutionDone,
    n_amount,
    n_Undefined
  };

  const char *GetLabel(eName Name);
}

#define ML( name ) messages::GetLabel(messages::n##name)

#endif
