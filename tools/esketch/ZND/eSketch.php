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

function getZNDq() {
	if (getenv("EPEIOS_SRC") === false)
		$zndq_path = realpath(dirname(__FILE__)) . '/';
	else {
		switch (strtoupper(substr(php_uname('s') , 0, 3))) {
			case "WIN":
				$epeios_path = "h:\\hg\\epeios\\";
			break;
			case "LIN":
				$epeios_path = "/home/csimon/hg/epeios/";
			break;
			case "DAR":
				$epeios_path = "/Users/csimon/hg/epeios/";
			break;
			default:
				echo "Unknown OS !!!\n";
			break;
		}

		$zndq_path = $epeios_path . "tools/zndq/";
	}

	require( $zndq_path . "ZNDq.php");
}

getZNDq();

class eSketchWrapper extends ZNDq {
	static private $launcher;
	static function init() {
		self::$launcher = parent::register_( "esketch" );
	}
	static public function componentInfo() {
		return parent::componentInfo_( self::$launcher );
	}
	static protected function call( $id, ...$args ) {
		return parent::call_( self::$launcher, $id, ...$args );
	}
}

eSketchWrapper::init();

class eSketch extends eSketchWrapper {
	static public function returnArgument($argument) {
		return parent::call(0, $argument);
	}
	static public function TestStrings( array $array ) {
		return parent::call(1, $array );
	}
}

?>

