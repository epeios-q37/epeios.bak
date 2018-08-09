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

//require( 'XDHqPROD.php' );
require( 'XDHqDEMO.php' );


//class XDHqDOM extends XDHqDOM_PROD {
class XDHqDOM extends XDHqDOM_DEMO {
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
	function execute( $script ) {
		return self::call( 10 );
	}
	function alert( string $message ) {
		self::call( 11, $message );
	}
	function confirm( string $message ) {
		return self::call( "Confirm_1", 1, 1, $message, 0 ) == "true";
	}
	private function setLayout_(string  $id, $tree, string $xslFilename ) {
		self::call( "SetLayout_1", 0, 3, $id, $tree, $xslFilename, 0 );
	}
	function setLayout(string  $id, string $html ) {
		self::setLayout_( $id, $html, "" );
	}
	function getContents( array $ids ) {
		return self::unsplit($ids,self::call( "GetContents_1", 2, 0, 1, $ids ));
	}
	function getContent( string $id ) {
		return self::getContents( [$id] )[$id];
	}
	function setContents( array $idsAndContents ) {
		$ids = [];
		$contents = [];

		self::split( $idsAndContents, $ids, $contents );

		self::call( "SetContents_1", 0, 0, 2, $ids, $contents );
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
