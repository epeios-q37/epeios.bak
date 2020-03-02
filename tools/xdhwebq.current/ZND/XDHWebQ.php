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

error_log( __LINE__ );

require( "h:\\hg\\epeios\\tools\\zndq\\ZNDq.php");

error_log( __LINE__ );

class XDHWebQ extends ZNDq {
	static private $launcher;
	static function init() {
	global $zndq_affix;
error_log( __LINE__ );
		self::$launcher = parent::register_( $zndq_affix, get_class() );
error_log( __LINE__ );
		parent::call_( self::$launcher, 1, "h:/bin/xdhqxdh" );
error_log( __LINE__ );
	}
	static public function returnArgument($argument) {
		return ZNDq_wrapper( ZNDq::$launcher, 0, $argument);
	}
	static private function split( array $keysAndValues, array &$keys, array &$values ) {
		foreach ($keysAndValues as $key => $value) {
			$keys[] = $key;
			$values[] = $value;
		}
	}
	static private function userHead() {
		$content = file_get_contents( 'head.html' );

		if ( $content === false )
			$content = "";

		return $content;
	}
	static private function prolog() {
		return implode( "\n",
			array (
				'<!DOCTYPE html>',
				'<html>',
				'	<head>',
				'		<meta name="generator" content="XDHWebQZND"/>',
				'		<meta charset="UTF-8" />',
				'		<meta http-equiv="X-UA-Compatible" content="IE=edge" />',
				self::userHead(),
				'		<script src="xdh/xdhtml.js"></script>',
				'		<script src="xdh/xdhwebq.js"></script>',
				'		<script>handleQuery("?_action=")</script>',
				'	</head>',
				'	<body id="XDHRoot">',
				'	</body>',
				'</html>',
			)
		);
	}
	static public function serve() {
		if ( count($_REQUEST) == 0 )
			return self::prolog();
		else {
			$ids = [];
			$values = [];

			self::split( $_REQUEST, $ids, $values );

			return parent::call_( self::$launcher, 2, $ids, $values);
		}
	}
}

XDHWebQ::init();
?>
