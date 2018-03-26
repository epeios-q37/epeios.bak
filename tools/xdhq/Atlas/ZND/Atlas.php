<?php
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	WDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

function getXDHq() {
	if (getenv("EPEIOS_SRC") === false)
		$xdhq_path = realpath(dirname(__FILE__)) . '/';
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
				die( "Unknown OS !!!");
			break;
		}

		$xdhq_path = $epeios_path . "tools/xdhq/wrappers/ZND/";
	}

	require( $xdhq_path . "XDHq.php");
}

getXDHq();

set_error_handler(function ($errno, $errstr) {
    return strpos($errstr, 'Declaration of') === 0;
}, E_WARNING);

class Atlas extends XDHq {
	const DESKTOP = 1;
	const WEB = 2;
	const DESKTOP_AND_WEB = 3;
	const DEFAULT = Atlas::DESKTOP;

	private static function launchWeb( $dir ) {
		popen( "start node h:/hg/epeios/tools/xdhq/examples/common/httpd.js h:/hg/epeios/tools/xdhq/examples/common/" . $dir, "r" );
	}

	private static function launchDesktop( $dir ) {
		popen( "start h:/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron h:/hg/epeios/tools/xdhelcq/ -m=h:/bin/xdhqxdh h:/hg/epeios/tools/xdhq/examples/common/" . $dir, "r" );
	}

	public static function launch( string $newSessionAction, $type = null, string $dir = "." ) {
		global $argv;

		if ( $type === null ) {
			$type = Atlas::DEFAULT;

			if ( count( $argv ) > 1 ) {
				switch ( $argv[1] ) {
				case "d":
				case "desktop":
					$type = Atlas::DESKTOP;
					break;
				case "w":
				case "web":
					$type = Atlas::WEB;
					break;
				case "dw":
				case "wd":
					$type = Atlas::DESKTOP_AND_WEB;
					break;
				default:
					die( "Unknown type !");
					break;
				}
			}
		}

		parent::launch( $newSessionAction);

		switch( $type ) {
		case Atlas::DESKTOP:
			Atlas::launchDesktop( $dir );
			break;
		case Atlas::WEB:
			Atlas::launchWeb( $dir );
			break;
		case Atlas::DESKTOP_AND_WEB:
			Atlas::launchDesktop( $dir );
			Atlas::launchWeb( $dir );
			break;
		default:
			die( "Unknown type !!!");
			break;
		}

	}
};

class AtlasTree extends XDHqTree {};

class AtlasDOM extends XDHqDOM {};

?>