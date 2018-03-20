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
		self::$launcher = parent::register_( "xdhq" );
	}
	static public function componentInfo() {
		return parent::componentInfo_( self::$launcher );
	}
	static protected function _call( $id, ...$args ) {
		return parent::call_( self::$launcher, $id, ...$args );
	}
}

XDHqWrapper::init();

class XDHq extends XDHqWrapper {
	static function returnArgument($argument) {
		return parent::_call(0, $argument);
	}
	static function launch ( string $newSessionAction ) {
		parent::_call( 7, "53752", $newSessionAction );
	}
}

class XDHQTree extends XDHqWrapper {
	private $core;
	function __construct() {
		$this->core = parent::_call( 1 );
	}
	private function call( $id, ...$args ) {
		return parent::_call( $id, $this->core, ...$args );
	}
	function __destruct() {
		self::call( 2 );
	}
	function pushTag( string $name ) {
		self::call( 3, $name );
	}
	function popTag() {
		self::call( 4 );
	}
	function putValue( string $value )
	{
		self::call( 5, $value );
	}
	function putAttribute( string $name, string $value ) {
		self::call( 6, $name, $value );
	}
	function getCore() {
		return $this->core;
	}
}

class XDHqDOM extends XDHqWrapper {
	private $core;
	private function split( array $keysAndValues, array &$keys, array &$values ) {
		foreach ($keysAndValues as $key => $value) {
			$keys[] = $key;
			$values[] = $value;
		}
	}
	private function unsplit( array $keys, array $values ) {
		$count = count( $keys );
		$i = 0;
		$keysAndValues = [];

		while ( $i < $count ) {
			$keysAndValues[$keys[$i]] = $values[$i];

			$i++;
		}

		return $keysAndValues;
	}
	function __construct() {
		$this->core = parent::_call( 8 );
	}
	private function call( $id, ...$args ) {
		return parent::_call( $id, $this->core, ...$args );
	}
	function getAction( &$id ) {
		$return = self::call( 9 );

		$id = $return[0];

		return $return[1];
	}
	function execute( $script ) {
		return self::call( 10 );
	}
	function alert( string $message ) {
		self::call( 11, $message );
	}
	function confirm( string $message ) {
		return self::call( 12, $message );
	}
	function setLayout(string  $id, XDHqTree $tree, string $xslFilename ) {
		self::call( 13, $id, $tree->getCore(), $xslFilename );
	}
	function getContents( array $ids ) {
		return self::unsplit($ids,self::call( 14,$ids ));
	}
	function getContent( string $id ) {
		return self::getContents( [$id] )[$id];
	}
	function setContents( array $idsAndContents ) {
		$ids = [];
		$contents = [];

		self::split( $idsAndContents, $ids, $contents );

		self::call( 15, $ids, $contents );
	}
	function setContent( string $id, string $content ) {
		self::setContents( [ $id => $content ] );
	}
	function dressWidgets( string $id ) {
		return self::call( 16, $id );
	}
	private function handleClasses( $fid, array $idsAndClasses ) {
		$ids = [];
		$classes = [];

		self::split( $idsAndClasses, $ids, $classes );
		self::call( $fid, $ids, $classes );
	}
	private function handleClass( $fid, $id, $class ) {
		self::handleClasses( $fid, [ $id => $class ] );
	}
	function addClasses( array $idsAndClasses ) {
		self::handleClasses( 17, $idsAndClasses );
	}
	function addClass( string $id, string $class  ) {
		self::handleClass( 17, $id, $class );
	}
	function removeClasses( array $idsAndClasses ) {
		self::handleClasses( 18, $idsAndClasses );
	}
	function removeClass( string $id, string $class  ) {
		self::handleClass( 18, $id, $class );
	}
	function toggleClasses( array $idsAndClasses ) {
		self::handleClasses( 19, $idsAndClasses );
	}
	function toggleClass( string $id, string $class  ) {
		self::handleClass( 19, $id, $class );
	}
	function enableElements( array $ids ) {
		self::call( 20, $ids );
	}
	function enableElement( string $id ) {
		self::enableElements( array( $id ) );
	}
	function disableElements( array $ids ) {
		self::call( 21, $ids  );
	}
	function disableElement( string $id ) {
		self::disableElements( array( $id ) );
	}
	function setAttribute( string $id, string $name, string $value ) {
		return self::call( 22, $id, $value );
	}
	function getAttribute( string $id, string $name ) {
		return self::call( 23, $id, $name );
	}
	function removeAttribute( string $id, string $name ) {
		self::call( 24, $id );
	}
	function setProperty( string $id, string $name, string $value ) {
		return self::call( 25, $id, $name, $value );
	}
	function getProperty( string $id, string $name ) {
		return self::call( 26, $id, $name );
	}
	function focus( string $id ) {
		self::call( 27, $id );
	}
}
?>
