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

$newSessionAction = '';

class XDHq_DEMO extends XDHq_SHRD {
	static function launch( string $newSessionAction ) {
		$GLOBALS["newSessionAction"] = $newSessionAction;
	}
}

class XDHqDOM_DEMO {
	private $socket;
	private $token = "";
	private $protocolLabel = "712a58bf-2c9a-47b2-ba5e-d359a99966de";
	private $protocolVersion = "0";
	private function writeSize_( $socket, $size ) {
		$result = pack( "C", $size & 0x7f );
		$size >>= 7;

		while ( $size != 0 ) {
			$result = pack( "C", ( $size & 0x7f ) | 0x80 ) . $result;
			$size >>= 7;
		}

		fwrite( $socket, $result );
	}
	private function writeString_( $string, $socket ) {
		$this->writeSize_( $socket, strlen( $string ) );
		fwrite( $socket, $string );
	}
	private function writeStrings_( $strings, $socket ) {
		$count = count( $strings );
		$i = 0;

		$this->writeSize_( $socket, $count );

		while ( $i < $count ) {
			$this->writeString_( $strings[$i], $socket );
			$i++;
		}
	}
	private function getByte_( $socket ) {
		return unpack( "C", fgetc( $socket ) )[1];
	}
	private function getSize_( $socket ) {
		$byte = $this->getByte_( $socket );
		$size = $byte & 0x7f;
		
		while ( $byte & 0x80 ) {
			$byte = $this->getByte_( $socket );

			$size = ($size << 7) + ( $byte & 0x7f );
		}

		return $size;
	}
	private function getString_( $socket ) {
		$size = $this->getSize_( $socket );

		if ( $size ) {
			return fread( $socket, $size );
		} else
			return "";
	}
	private function getStrings_( $socket ) {
		$amount = $this->getSize_( $socket );

		while( $amount-- ) {
			$strings[] = $this->getString_( $socket );
		}

		return $strings;
	}
	private function getQuery_( $socket ) {
		$c = fgetc( $socket );
		$string = "";

		while ( $c != "\0" ) {
			$string .= $c;
			$c = fgetc( $socket );
		}

		return $string;
	}
	function __construct() {
//		$address = "atlastk.org";$httpPort = "";
		$address = "localhost";$httpPort = ":8080";
		$port = 53800;

		$this->socket = fsockopen( $address, $port, $errno, $errstr );

		if (!$this->socket)
		    throw new Exception( "$errstr ($errno)\n" );

		$this->writeString_( $this->token, $this->socket );

		fflush( $this->socket );

		if ( empty($this->token) ) {
			$this->token = $this->getString_( $this->socket );

			if ( empty($this->token) )
				throw new Exception( "Invalid connection information !!!");

			echo "Token id : " . $this->token . "\n";
			XDHq_SHRD::open( "http://" . $address . $httpPort . "/atlas.php?_token=" . $this->token );
		} else {
			if ( $this->getString_( $this.socket) != $this->token )
				throw new Exception( "Unmatched token !!!");
		}

		$this->getString_( $this->socket );	// Language.
		$this->writeString_( $this->protocolLabel, $this->socket );
		$this->writeString_( $this->protocolVersion, $this->socket );
		fflush( $this->socket );
	}
	function getAction( &$id ) {
		static $firstLaunch = true;

		if ( !$firstLaunch) {
			fwrite($this->socket,  pack( "a*x", "StandBy_1" ));
			fflush($this->socket);
		} else
			$firstLaunch = false;


		$id = $this->getString_( $this->socket );

		$action = $this->getString_( $this->socket );

		if( empty( $action ) )
			$action = $GLOBALS["newSessionAction"];

		return $action;
	}
	function call( $command, $type, ...$args ) {
		$i = 0;

		fwrite( $this->socket, pack( "a*x", $command ) );

		$amount = $args[$i];
		$i++;

		while ( $amount-- ) {
			$this->writeString_( $args[$i], $this->socket );
			$i++;
		}

		$amount = $args[$i];
		$i++;

		while ( $amount-- ) {
			$this->writeStrings_( $args[$i], $this->socket );
			$i++;
		}

		fflush( $this->socket );

		switch ( $type ) {
		case XDHq::RT_NONE:
			break;
		case XDHq::RT_STRING:
			return $this->getString_( $this->socket );
			break;
		case XDHq::RT_STRINGS:
			return $this->getStrings_( $this->socket );
			break;
		default:
			throw new Exception( "Unknown return type !!!");
		}
	}
}
?>
