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
	static function listen () {
		parent::_call( 7, "53752" );
	}
}

class XDHQTree extends XDHqWrapper {
	private $core;
	function __construct() {
		$core = parent::call( 1 );
	}
	private function call( $id, ...$args ) {
		return parent::_call( $id, self::$core, ...$args );
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
		return self::$core;
	}
}

class XDHqDOM extends XDHqWrapper {
	private $core;
	private function split( array $keysAndValues, array $keys, array $values ) {
		while ($item = current($keysAndValues)) {
			$keys[] = key( $$keysAndValues );
			$value[] = $item;
			next($array);
		}
	}
	function __construct() {
		$core = parent::call( 8 );
	}
	private function call( $id, ...$args ) {
		return parent::_call( $id, self::$core, ...$args );
	}
	function __destruct() {
		self::call( 9 );
	}
	function getAction( $id ) {
		$return = parent::call( 10 );

		$id = $return[0];

		return $return[1];
	}
	function alert( $message ) {
		self::call( 11, $message );
	}
	function confirm( $message ) {
		return self::call( 12, $message );
	}
	function setLayout( $id, XDHqTree $tree, string $xslFilename ) {
		self::call( 13, $id, $tree.getCore(), $xslFilename );
	}
	function getContents( array $ids ) {
		return self::call( 14,$ids );
	}
	function getContent( string $id ) {
		return self::getContents( array( $id ) )[0];
	}
	function setContents( array $idsAndContents ) {
		$ids = array();
		$contents = array();

		self::split( $idsAndContents, $ids, $contents );

		self::call( 15, $ids, $contents );
	}
	function setContent( $id, $content ) {
		self::setContents( [ $id => $content ] );
	}
	function dressWidgets( array $id ) {
		return self::call( 16, $id );
	}
	private function handleClasses( $fid, array $idsAndClasses ) {
		$ids = array();
		$classes = array();

		sel::split( $idsAndClasses, $ids, $classes );

		self::call( $fid, $ids, $classes );
	}
	private function handleClass( $fid, $id, $class ) {
		self::handleClasses( $fid, [ $id => $class ] );
	}
	function addClasses( array $idsAndClasses ) {
		handleClasses( 17, $idsAndClasses );
	}
	function addClass( string $id, string $class  ) {
		handleClass( 17, $id, $class );
	}
	function removeClasses( array $idsAndClasses ) {
		handleClasses( 18, $idsAndClasses );
	}
	function removeClass( string $id, string $class  ) {
		handleClass( 18, $id, $class );
	}
	function toggleClasses( array $idsAndClasses ) {
		handleClasses( 19, $idsAndClasses );
	}
	function toggleClass( string $id, string $class  ) {
		handleClass( 19, $id, $class );
	}
	function enableElements( array $ids ) {
		self::call( 20, $ids );
	}
	function enableElement( string $id ) {
		self::enableElements( array( $id ) );
	}
	function disableElements( aray $ids ) {
		self::call( 21, $ids  );
	}
	function disableElement( string $id ) {
		self::disableElements( array( $id ) );
	}
	function getAttribute( string $id ) {
		return sel::call( 22, $id );
	}
	function setAttribute( string $id, string $value ) {
		return parent::call( 23, $id, $value );
	}
	function getProperty( string $id ) {
		return parent::call( 24, $id );
	}
	function setProperty( string $id, string $value ) {
		return parent::call( 25, $id , $value );
	}
}
?>
