<?php
/*
	Copyright (C) 2018 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

class XDHq_SHRD {
	// Return types ; must watch with the ones defined in the proxy.
	const RT_NONE = 0;
	const RT_STRING = 1;
	const RT_STRINGS = 2;

	static function isDev() {
		return getenv( "EPEIOS_SRC") !== false;
	}

	static function isWin() {
		switch (strtoupper(substr(php_uname('s') , 0, 3))) {
			case "WIN":
				return true;
				break;
			default:
				return false;
				break;
		}
	}

	static function open( $document ) {
		switch (strtoupper(substr(php_uname('s') , 0, 3))) {
			case "WIN":
				$command = "start /B ";
				$suffix = "";
				break;
			case "DAR":	// macOS.
				$command = "open ";
				$suffix = " &";
				break;
			default:	// Linux and others.
				$command = "xdg-open ";
				$suffix = " &";
				break;
		}

		exec( $command . $document . $suffix );
	}
}

?>
