<?php
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDGq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
*/

function getAtlas() {
	if (getenv("EPEIOS_SRC") === false)
		$zndq_path = realpath(dirname(__FILE__)) . '/../';
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

		$zndq_path = $epeios_path . "tools/xdhq/Atlas/ZND/";
	}

	require( $zndq_path . "Atlas.php");
}

getAtlas();

Atlas::launch( "Connect", null, "Hello" );

$dom = new AtlasDOM();

while ( true ) {
	switch( $dom->getAction( $id ) ) {
	case "Connect":
		$dom->headUp( file_get_contents( "h:/hg/epeios/tools/xdhq/examples/common/Hello/Head.html") );
		$dom->setLayout( "", file_get_contents( "h:/hg/epeios/tools/xdhq/examples/common/Hello/Main.html" ) );
		break;
	case "Typing":
		$dom->setContent( "name", $dom->getContent( $id ) );
		break;
	case "Clear":
		if ( $dom->confirm( "Are you sure?" ) ) $dom->setContents( [ "input" => "", "name" => "" ] );
		break;
	default:
		throw new Exception( "Unknown action '" . $action . "' !!!");
	}
}

?>
