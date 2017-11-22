<?php
/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of eSketch.

	eSketch is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	eSketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with eSketch.  If not, see <http://www.gnu.org/licenses/>.
*/

$zndq_affix = "esketch";

/***** Begin of generic part. ****/
if (getenv("EPEIOS_SRC") === false) $zndq_path = realpath(dirname(__FILE__)) . '/';
else {
	switch (strtoupper(substr(php_uname('s') , 0, 3))) {
		case "WIN":
			$zndq_path = "h:\\bin\\";
		break;
		case "LIN":
			$zndq_path = "/home/csimon/bin/";
		break;
		case "DAR":
			$zndq_path = "/Users/csimon/bin/";
		break;
		default:
			echo "Unknown OS !!!\n";
		break;
	}
}

zndq_init();
zndq_register(str_replace(' ', '\ ', str_replace('\\', '/', $zndq_path)) . $zndq_affix . "znd");

class ZNDq {
	static public function wrapperInfo() {
		return zndq_wrapper_info();
	}
	static public function componentInfo() {
		return zndq_component_info();
	}
	static protected function register($arguments) {
		return zndq_register($arguments);
	}
}
/**** End of generic part. ****/

class eSketch extends ZNDq {
	static public function returnArgument($argument) {
		return ZNDq_wrapper(0, $argument);
	}
}
?>
