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

class XDHq_DEMO extends XDHq_SHRD {
    static $headContent;
    public static function launch(string $headContent) {
        self::$headContent = $headContent;
    }
}

function getEnv_($name, $value = "") {
    if (getenv($name) !== false) {
        return trim(getenv($name));
    } else {
        return trim($value);
    }

}

class XDHqDOM_DEMO {
    private static $socket;
    private static $token = "";
    private static $demoprotocolLabel = "877c913f-62df-40a1-bf5d-4bb5e66a6dd9";
    private static $demoProtocolVersion = "0";
    private static $mainProtocolLabel = "6e010737-31d8-4be3-9195-c5b5b2a9d5d9";
    private static $mainProtocolVersion = "0";
	private static $instances_;
    private static function isTokenEmpty_() {
        return empty(self::$token) || (substr(self::$token, 0, 1) == '&');
    }
	// All write function returns data and do not write directly to socket.
    private function writeByte_($datum) {
        fwrite($this->socket, pack("C", $datum));
    }
    private function writeSize_($size) {
        $result = pack("C", $size & 0x7f);
        $size >>= 7;

        while ($size != 0) {
            $result = pack("C", ($size & 0x7f) | 0x80) . $result;
            $size >>= 7;
        }

        fwrite($this->socket, $result);
    }
    private function writeString_($string) {
        $this->writeSize_(strlen($string));
        fwrite($this->socket, $string);
    }
    private function writeStrings_($strings) {
        $count = count($strings);
        $i = 0;

        $this->writeSize_($count);

        while ($i < $count) {
            $this->writeString_($strings[$i]);
            $i++;
        }
    }
	private function subWrite_( $data ) {
		fwrite($this->socket,$data);
		fflush( $this->socket );
	}
	private function write_( $data, $thread ) {
		$thread->synchronize( function($thread) {$this->subWrite_($data);});
	}
    private function getByte_() {
        while (!($c = fgetc($this->socket))); // Workaround concerning an arbitrary timeout!

        return unpack("C", $c)[1];
    }
    private function getSize_() {
        $byte = $this->getByte_();
        $size = $byte & 0x7f;

        while ($byte & 0x80) {
            $byte = $this->getByte_();
            $size = ($size << 7) + ($byte & 0x7f);
        }

        return $size;
    }
    private function getString_() {
        $size = $this->getSize_();

        if ($size) {
            return fread($this->socket, $size);
        } else {
            return "";
        }
    }
    private function getStrings_() {
        $amount = $this->getSize_();

        while ($amount--) {
            $strings[] = $this->getString_();
        }

        return $strings;
    }
    private function getQuery_() {
        $c = fgetc($this->socket);
        $string = "";

        while ($c != "\0") {
            $string .= $c;
            $c = fgetc($this->socket);
        }

        return $string;
    }
	private function demosHandshake_() {
        $this->writeString_(self::$demoProtocolLabel);
        $this->writeString_(self::$demoProtocolVersion);
        fflush($this->socket);

        $errorMessage = $this->getString_();

        if ($errorMessage != "") {
            die($errorMessage);
        }

        $notificationMessage = $this->getString_();

        if ($notificationMessage != "") {
            echo $errorMessage . "\n";
        }
	}
	private function ignition_() {
        $this->writeString_(self::$token);

        $this->writeString_(XDHq_DEMO::$headContent);

        fflush($this->socket);

        if ($this->isTokenEmpty_()) {
            die( $this->getString_());
        }

        if ($wPort != ":0") {
            $url = "http://" . $wAddr . $wPort . "/" . $cgi . ".php?_token=" . self::$token;
            echo $url . "\n";
            echo "Open above URL in a web browser. Enjoy!\n";
            XDHq_SHRD::open($url);
        }
	}
	private function serve_(callable $callback) {
		while(true) {
			$id = getByte_();

			if ($id == 255) {	// Value reporting a new front-end.
				$if = getByte_();

				if ( in_array( $if, $this->instances_) )
					die(  "Instance of id  '" . id . "' exists but should not !");

				$this->instances_[id] = call_user_func($callback);

				$this->write_( $this->writeByte_( $id ) . $this->writString_( $this->mainProtocolLabel_ ) . $this->writeString_( $this->mainProtocolVersion_ ) );
			} else if ( !id_array( $id, $this->instances_ ) ) {
				die( "Unknown instance of id '" . $id . "'!" );
			} else if ( !$this->instances_[$id].handshakeDone ) {
				$error = getString_();

				if ( $error != "" )
					die( $error );

				$this->getString_();	// Language. Not currently handled.

				$this->write_( writeByte_($i) . writeString_("ZND") );

			} else {
				$this->instances_[$id]->notify();

				$this->wait();
			}
		}
	}
    public function __construct() {
        // Due to multithreading handling of PHP, global variables can not be used in methods !
        $pAddr = "atlastk.org";
        $pPort = 53800;
        $wAddr = "";
        $wPort = "";
        $cgi = "xdh";

        switch (getEnv_("ATK")) {
        case 'DEV':
            $pAddr = "localhost";
            $wPort = "8080";
            echo ("\tDEV mode !\n");
            break;
        case 'TEST':
            $cgi = "xdh_";
            echo ("\tTEST mode !\n");
            break;
        case '':
            break;
        default:
            die("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !");
        }

        $pAddr = getEnv_("ATK_PADDR", $pAddr);
        $pPort = intval(getEnv_("ATK_PPORT", strval($pPort)));
        $wAddr = getEnv_("ATK_WADDR", $wAddr);
        $wPort = getEnv_("ATK_WPORT", $wPort);

        if ($wAddr == "") {
            $wAddr = $pAddr;
        }

        if ($wPort != "") {
            $wPort = ":" . $wPort;
        }


        if ($this->isTokenEmpty_()) {
            $token = getEnv_("ATK_TOKEN");

            if (!empty($token)) {
                self::$token = "&" . $token;
            }

        }

        $this->socket = fsockopen($pAddr, $pPort, $errno, $errstr);

        if (!$this->socket) {
            die("$errstr ($errno)\n");
        }

/*

        $this->writeString_(self::$token);

        if ($this->isTokenEmpty_()) {
            $this->writeString_(XDHq_DEMO::$headContent);

            fflush($this->socket);

            if ($this->isTokenEmpty_()) {
                self::$token = $this->getString_();

                if ($this->isTokenEmpty_()) {
                    throw new Exception($this->getString_());
                }

            }

            if ($wPort != ":0") {
                $url = "http://" . $wAddr . $wPort . "/" . $cgi . ".php?_token=" . self::$token;
                echo $url . "\n";
                echo "Open above URL in a web browser. Enjoy!\n";
                XDHq_SHRD::open($url);
            }
        } else {

            $returnedToken = $this->getString_();

            if ($returnedToken == "") {
                throw new Exception($this->getString_());
            }

            if ($returnedToken != self::$token) {
                throw new Exception("Unmatched token !!!");
            }

        }

        $this->writeString_(self::$protocolLabel);
        $this->writeString_(self::$protocolVersion);
        fflush($this->socket);

        $errorMessage = $this->getString_();

        if ($errorMessage != "") {
            throw new Exception($errorMessage);
        }

        $this->getString_(); // Language.
        $this->writeString_("ZND");
        fflush($this->socket);
*/
    }
    public function getAction(&$id) {
        static $firstLaunch = true;

        if (!$firstLaunch) {
            fwrite($this->socket, pack("a*x", "StandBy_1"));
            fflush($this->socket);
        } else {
            $firstLaunch = false;
        }

        $id = $this->getString_();
        $action = $this->getString_();

        return $action;
    }
    public function call($command, $type, ...$args) {
        $i = 0;

		$data = writeByte_($this->id );

        $data .= pack("a*x", $command);

        $amount = $args[$i];
        $i++;

        while ($amount--) {
            $data .= $this->writeString_($args[$i]);
            $i++;
        }

        $amount = $args[$i];
        $i++;

        while ($amount--) {
            $data .= $this->writeStrings_($args[$i]);
            $i++;
        }

        $write_( $data );

		$this->wait();

        switch ($type) {
        case XDHq::RT_NONE:
            break;
        case XDHq::RT_STRING:
            return $this->getString_();
            break;
        case XDHq::RT_STRINGS:
            return $this->getStrings_();
            break;
        default:
            throw new Exception("Unknown return type !!!");
        }

		$this->mainThread->notify();
    }
}
