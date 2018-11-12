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

class XDHqXML {
 private $xml_;

 private function writeString_($string) {
  $this->xml_ .= strval( $string ) . "\0";
 }

 function __construct(string $rootTag) {
  $this->writeString_( "dummy" );
  $this->writeString_( $rootTag );
 }
 function pushTag(string $tag) {
  $this->xml_ .= ">";
  $this->writeString_( $tag );
 }
 function popTag() {
  $this->xml_ .= "<";
 }
 function setAttribute(string $name, $value) {
  $this->xml_ .= "A";
  $this->writeString_( $name );
  $this->writeString_( $value );
 }
 function setValue($value) {
  $this->xml_ .= "V";
  $this->writeString_( $value );
 }
 function toString() {
  return $this->xml_;
 }
}
?>
