<?php
/*
	Copyright (C) 2018 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

$zndq_affix = "xdhwebq";

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

class ZNDq {
	static public $launcher = 0;
	static public function wrapperInfo() {
		return zndq_wrapper_info();
	}
	static public function componentInfo() {
		return zndq_component_info( ZNDq::$launcher );
	}
	static protected function register($arguments) {
		return zndq_register($arguments);
	}
}

ZNDq::$launcher = zndq_register(str_replace(' ', '\ ', str_replace('\\', '/', $zndq_path)) . $zndq_affix . "znd");

/**** End of generic part. ****/

class XDHWebQ extends ZNDq {
	static public function returnArgument($argument) {
		return ZNDq_wrapper( ZNDq::$launcher, 0, $argument);
	}
	private function split( array $keysAndValues, array &$keys, array &$values ) {
		foreach ($keysAndValues as $key => $value) {
			$keys[] = $key;
			$values[] = $value;
		}
	}
	static public void serve() {
		$ids = [];
		$values = [];

		split( $_REQUEST, $ids, $values );

		return ZNDq_wrapper( ZNDq::$launcher, 1, $ids, $values);
	}
}
?>
