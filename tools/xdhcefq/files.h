/*
	Header for the 'files' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xdhcefq' tool.
	Copyright (C) 2014 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILES__INC
# define FILES__INC

# include "misc.h"

namespace files {
	void FileDialog(
		cef_browser_t *Browser,
		cef_file_dialog_mode_t FileDialogMode,
		const misc::nstring___  &Params );

	void HandleResult(
		cef_list_value_t *ListValue,
		str::string_ &Id,
		str::string_ &Action,
		str::strings_ &FileNames );
}

#endif
