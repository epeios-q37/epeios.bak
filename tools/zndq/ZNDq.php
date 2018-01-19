<?php
/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of ZNDq.

	ZNDq is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ZNDq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with ZNDq.  If not, see <http://www.gnu.org/licenses/>.
*/

global $zndq_location;

if (getenv("EPEIOS_SRC") === false)
	$zndq_location = realpath(dirname(__FILE__)) . '/';
else {
	switch (strtoupper(substr(php_uname('s') , 0, 3))) {
		case "WIN":
			$zndq_location = "h:\\bin\\";
		break;
		case "LIN":
			$zndq_location = "/home/csimon/bin/";
		break;
		case "DAR":
			$zndq_location = "/Users/csimon/bin/";
		break;
		default:
			echo "Unknown OS !!!\n";
		break;
	}
}

zndq_init( $zndq_location );

// Some method are suffixed with '_', so we can use the name without final '_' in subclasses.
class ZNDq {
	static protected function register_( $extension ) {
		global $zndq_location;
		return zndq_register(str_replace(' ', '\\ ', str_replace('\\', '/', $zndq_location)) . $extension . "znd");	
	}
	static public function wrapperInfo() {
		return zndq_wrapper_info();
	}
	static protected function componentInfo_( $launcher ) {
		return zndq_component_info( $launcher );
	}
	static protected function call_( $launcher, $index, ...$param )
	{
		return zndq_call( $launcher, $index, ...$param );
	}
}
?>
