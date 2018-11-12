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
require 'XDHqSHRD.php';
require 'XDHqDEMO.php';
require 'XDHqXML.php';

if ( false && XDHq_SHRD::isDev() )
	require 'XDHqPROD.php';


class XDHq extends XDHq_SHRD{
	const MODE_PROD = 0;
	const MODE_DEMO = 1;
	const MODE_UNDEFINED = 2;
	private static $mode_ = self::MODE_UNDEFINED;
	static $dir;
	private static function getAssetPath_( $dir ) {
		if ( parent::isDev() )
			return "h:/hg/epeios/tools/xdhq/examples/common/" . $dir . "/";
		else {
			list($script)=get_included_files();
			return dirname($script) . "/";
		}
	}
	private static function getAssetFilename_( $path, $dir ) {
		return self::getAssetPath_( $dir ) . $path;
	}
	static function readAsset( $path, string $dir = "" ) {
		return file_get_contents( self::getAssetFilename_( $path, $dir ) );
	}
	static function launch( string $newSessionAction, string $headContent, $mode, string $dir ) {
		self::$mode_ = $mode;
		self::$dir = $dir;

		switch ( $mode ) {
		case self::MODE_PROD:
			XDHq_PROD::launch( $newSessionAction );
			break;
		case self::MODE_DEMO:
			XDHq_DEMO::launch( $newSessionAction, $headContent );
			break;
		default:
			throw new Exception( "Unknown mode !!!");
			break;
		}
	}
	static function getMode() {
		return self::$mode_;
	}
	static function isDEMO() {
		return self::getMode() == self::MODE_DEMO;
	}
}


class XDHqDOM extends Threaded {
	private $dom_;
	private function split_( array $keysAndValues, array &$keys, array &$values ) {
		foreach ($keysAndValues as $key => $value) {
			$keys[] = $key;
			$values[] = $value;
		}
	}
	private function unsplit_( array $keys, array $values ) {
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
		switch ( XDHq::getMode() ) {
		case XDHq::MODE_PROD:
			$this->dom_ = new XDHqDOM_PROD;
			break;
		case XDHq::MODE_DEMO:
			$this->dom_ = new XDHqDOM_DEMO;
			break;
		default:
			die( "Unknown mode !!!");
		}
	}
	private function call_( ...$args ) {
		return $this->dom_->call( ...$args );
	}
	function getAction( &$id ) {
		return $this->dom_->getAction( $id );
	}
	function execute( $script ) {
		return self::call_( "Execute_1", XDHq::RT_STRING, 1, $script, 0 );
	}
	function alert( string $message ) {
		self::call_( "Alert_1", XDHq::RT_NONE, 1, $message, 0 );
	}
	function confirm( string $message ) {
		return self::call_( "Confirm_1", XDHq::RT_STRING, 1, $message, 0 ) == "true";
	}
	private function setLayout_(string  $id, $xml, string $xslFilename ) {
		self::call_( "SetLayout_1", XDHq::RT_NONE, 3, $id, $xml, $xslFilename, 0 );
	}
	function setLayout(string  $id, string $html ) {
		self::setLayout_( $id, $html, "" );
	}
	function setLayoutXSL(string  $id, $xml, string $xsl ) {
		$xslURL = $xsl;

		if ( XDHq::isDEMO() )
			$xslURL = "data:text/xml;charset=utf-8," . rawurlencode( XDHq::readAsset( $xsl, XDHq::$dir ) );
			
		self::setLayout_( $id, is_string($xml) ? $xml : $xml->toString(), $xslURL );
	}
	function getContents( array $ids ) {
		return self::unsplit_($ids,self::call_( "GetContents_1", XDHq::RT_STRINGS, 0, 1, $ids ));
	}
	function getContent( string $id ) {
		return self::getContents( [$id] )[$id];
	}
	function setContents( array $idsAndContents ) {
		$ids = [];
		$contents = [];

		self::split_( $idsAndContents, $ids, $contents );

		self::call_( "SetContents_1", XDHq::RT_NONE, 0, 2, $ids, $contents );
	}
	function setContent( string $id, string $content ) {
		self::setContents( [ $id => $content ] );
	}
	function setTimeout( int $delay, string $action ) {
		self::call_( "SetTimeout_1", XDHq::RT_NONE, 2, strval( $delay ), $action, 0 );
	}
	function createElement( string $name, string $id = "" ) {
		return self::call_( "CreateElement_1", XDHq::RT_STRING, 2, $name, $id, 0 );
	}
	function insertChild( string $child, string $id ) {
		self::call_( "InsertChild_1", XDHq::RT_NONE, 2, $child, $id, 0 );
	}
	function dressWidgets( string $id ) {
		return self::call_( "DressWidgets_1", XDHq::RT_NONE, 1, $id, 0 );
	}
	private function handleClasses( $command, array $idsAndClasses ) {
		$ids = [];
		$classes = [];

		self::split_( $idsAndClasses, $ids, $classes );
		self::call_( $command, XDHq::RT_NONE, 0, 2, $ids, $classes );
	}
	function addClasses( array $idsAndClasses ) {
		self::handleClasses( "AddClasses_1", $idsAndClasses );
	}
	function removeClasses( array $idsAndClasses ) {
		self::handleClasses( "RemoveClasses_1", $idsAndClasses );
	}
	function toggleClasses( array $idsAndClasses ) {
		self::handleClasses( "ToggleClasses_1", $idsAndClasses );
	}
	function addClass( string $id, string $class  ) {
		self::addClasses( [ $id => $class ] );
	}
	function removeClass( string $id, string $class  ) {
		self::removeClasses( [ $id => $class ] );
	}
	function toggleClass( string $id, string $class  ) {
		self::toggleClasses( [ $id => $class ] );
	}
	function enableElements( array $ids ) {
		self::call_( "EnableElements_1", XDHq::RT_NONE, 0, 1, $ids );
	}
	function enableElement( string $id ) {
		self::enableElements( array( $id ) );
	}
	function disableElements( array $ids ) {
		self::call_( "DisableElements_1", XDHq::RT_NONE, 0, 1, $ids  );
	}
	function disableElement( string $id ) {
		self::disableElements( array( $id ) );
	}
	function setAttribute( string $id, string $name, string $value ) {
		return self::call_( "SetAttribute_1", XDHq::RT_NONE, 2, $id, $value, 0 );
	}
	function getAttribute( string $id, string $name ) {
		return self::call_( "GetAttribute_1", XDHq::RT_STRING, 2, $id, $name, 0 );
	}
	function removeAttribute( string $id, string $name ) {
		self::call_( "RemoveAttribute_1", XDHq::RT_NONE, 1, $id, 0 );
	}
	function setProperty( string $id, string $name, string $value ) {
		return self::call_( "SetProperty_1", XDHq::RT_NONE, 3, $id, $name, $value, 0 );
	}
	function getProperty( string $id, string $name ) {
		return self::call_( "GetProperty_1", XDHq::RT_STRING, 2, $id, $name, 0 );
	}
	function focus( string $id ) {
		self::call_( "Focus_1", XDHq::RT_NONE, 1, $id, 0 );
	}
}
?>
