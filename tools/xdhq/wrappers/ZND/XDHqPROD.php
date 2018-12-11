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

class XDHqWrapper extends ZNDq {
	static private $launcher;
	static function init() {
		self::$launcher = parent::register_( "xdhq", get_class() );
	}
	static public function componentInfo() {
		return parent::componentInfo_( self::$launcher );
	}
	static public function _call( $id, ...$args ) {
		return parent::call_( self::$launcher, $id, ...$args );
	}
}

XDHqWrapper::init();

class XDHq_PROD extends XDHq_SHRD {
	static function launch() {
		XDHqWrapper::_call( 1, "53752");
	}
}

class XDHqDOM_PROD extends XDHqWrapper {
	private $core;
	function __construct() {
		$this->core = parent::_call( 2 );
	}
	function call( $command, $type, ...$args ) {
		return parent::_call( 4, $this->core, $command, $type, ...$args );
	}
	function getAction( &$id ) {
		$return = parent::_call( 3, $this->core );

		$id = $return[0];

		return $return[1];
	}
}
?>
