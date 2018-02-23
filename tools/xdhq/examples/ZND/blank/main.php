<?php
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

function getUnJSq() {
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

		$zndq_path = $epeios_path . "tools/xdhq/UnJSq/ZND/";
	}

	require( $zndq_path . "UnJSq.php");
}

getUnJSq();

function main() {
	$type = null;

	// Uncomment below for desktop interface. Default is web (port: 8080).
	// $type = UnJSq::DESKTOP;

	UnJSq::listen( "Connect", "blank", $type );

	$dom = new UnJSqDOM();

	while ( true ) {
		switch( $dom->getAction( $id ) ) {
		case "Connect":
			$dom->setLayout( "", new UnJSqTree(), "Main.xsl" );
			$dom->addClass("Input", "hidden");
			break;
		case "Submit":
			$dom->setContent( "Pattern", strtoupper( $dom->getContent( "Pattern" ) ) );
			break;
		case "HideInput":
			$dom->addClass( "Input", "hidden" );
			break;
		case "ShowInput":
			$dom->removeClass( "Input", "hidden" );
			break;
		default:
			die( "???" );
			break;
		}
	}
}

main();

?>
