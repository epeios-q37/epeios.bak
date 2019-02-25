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

function getEnv_($name, $value = "") {
    if (getenv($name) !== false) {
        return trim(getenv($name));
    } else {
        return trim($value);
    }

}

class XDHqDOM_DEMO {
    private static $pAddr = "atlastk.org";
    private static $pPort = 53800;
    private static $wAddr = "";
    private static $wPort = "";
    private static $cgi = "xdh";
    private static $demoProtocolLabel = "877c913f-62df-40a1-bf5d-4bb5e66a6dd9";
    private static $demoProtocolVersion = "0";
    private static $mainProtocolLabel = "6e010737-31d8-4be3-9195-c5b5b2a9d5d9";
    private static $mainProtocolVersion = "0";
    private static $socket;
    private static $token = "";
	private static $instances_;
    private static function isTokenEmpty_() {
        return empty(self::$token) || (substr(self::$token, 0, 1) == '&');
    }
	// All write function returns data and do not write directly to socket.
    private function writeByte_($datum) {
        return pack("C", $datum);
    }
    private function writeSize_($size) {
        $result = pack("C", $size & 0x7f);
        $size >>= 7;

        while ($size != 0) {
            $result = pack("C", ($size & 0x7f) | 0x80) . $result;
            $size >>= 7;
        }

        return $result;
    }
    private function writeString_($string) {
        return self::writeSize_(strlen($string)) . $string;
    }
    private function writeStrings_($strings) {
        $count = count($strings);
        $i = 0;
		
        $data = self::writeSize_($count);

        while ($i < $count) {
            $data .= self::writeString_($strings[$i]);
            $i++;
        }

		return $data;
    }
	// Thread-unsafe write.
	private function writeTU_( $data ) {
		fwrite(self::$socket,$data);
		fflush(self::$socket);
	}
	private function write_( $data, $thread ) {
		$thread->synchronize( function($thread) {self::writeTU_($data);});
	}
    private function getByte_() {
        while (!($c = fgetc(self::$socket))); // Workaround concerning an arbitrary timeout!

        return unpack("C", $c)[1];
    }
    private function getSize_() {
        $byte = self::getByte_();
        $size = $byte & 0x7f;

        while ($byte & 0x80) {
            $byte = $self::getByte_();
            $size = ($size << 7) + ($byte & 0x7f);
        }

        return $size;
    }
    private function getString_() {
        $size = self::getSize_();

        if ($size) {
            return fread(self::$socket, $size);
        } else {
            return "";
        }
    }
    private function getStrings_() {
        $amount = $self::getSize_();

        while ($amount--) {
            $strings[] = self::getString_();
        }

        return $strings;
    }
    private function getQuery_() {
        $c = fgetc(self::$socket);
        $string = "";

        while ($c != "\0") {
            $string .= $c;
            $c = fgetc(self::$socket);
        }

        return $string;
    }
    public static function init_() {
        switch (getEnv_("ATK")) {
        case 'DEV':
            self::$pAddr = "localhost";
            self::$wPort = "8080";
            echo ("\tDEV mode !\n");
            break;
        case 'TEST':
            self::$cgi = "xdh_";
            echo ("\tTEST mode !\n");
            break;
        case '':
            break;
        default:
            die("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !");
        }

        self::$pAddr = getEnv_("ATK_PADDR", self::$pAddr);
        self::$pPort = intval(getEnv_("ATK_PPORT", strval(self::$pPort)));
        self::$wAddr = getEnv_("ATK_WADDR", self::$wAddr);
        self::$wPort = getEnv_("ATK_WPORT", self::$wPort);

        if (self::$wAddr == "") {
            self::$wAddr = self::$pAddr;
        }

        if (self::$wPort != "") {
            self::$wPort = ":" . self::$wPort;
        }


        if (self::isTokenEmpty_()) {
            $token = getEnv_("ATK_TOKEN");

            if (!empty($token)) {
                self::$token = "&" . $token;
            }

        }

        self::$socket = fsockopen(self::$pAddr, self::$pPort, $errno, $errstr);

        if (!self::$socket) {
            die("$errstr ($errno)\n");
        }
    }
	private function demosHandshake_() {
        self::writeTU_( self::writeString_(self::$demoProtocolLabel) . self::writeString_(self::$demoProtocolVersion));

        $errorMessage = self::getString_();

        if ($errorMessage != "") {
            die($errorMessage);
        }

        $notificationMessage = self::getString_();

        if ($notificationMessage != "") {
            echo $errorMessage . "\n";
        }
	}
	private function ignition_() {
        self::writeTU_(self::writeString_(self::$token) . self::writeString_(XDHq_DEMO::$headContent));

		self::$token = self::getString_();

        if (self::isTokenEmpty_()) {
            die( self::getString_());
        }

        if (self::$wPort != ":0") {
            $url = "http://" . self::$wAddr . self::$wPort . "/" . self::$cgi . ".php?_token=" . self::$token;
            echo $url . "\n";
            echo "Open above URL in a web browser. Enjoy!\n";
            XDHq_SHRD::open($url);
        }
	}
	private function serve_(callable $callback, $userCallback) {
		while(true) {
			$id = self::getByte_();

			echo "Id : " . $id   . "\n";

			if ($id == 255) {	// Value reporting a new front-end.
				$id= self::getByte_();

				if ( in_array( $id, self::$instances_) )
					die(  "Instance of id  '" . $id . "' exists but should not !");

				self::$instances_[id] = call_user_func($callback, $userCallback);

				self::write_( self::writeByte_( $id ) . self::writeString_( self::$mainProtocolLabel ) . self::writeString_( self::$mainProtocolVersion ), Thread::getCurrentThread() );
			} else if ( !id_array( $id, self::$instances_ ) ) {
				die( "Unknown instance of id '" . $id . "'!" );
			} else if ( !self::$instances_[$id].handshakeDone ) {
				$error = self::getString_();

				if ( $error != "" )
					die( $error );

				self::getString_();	// Language. Not currently handled.

				self::write_( writeByte_($i) . writeString_("ZND") );

			} else {
				self::instances_[$id]->notify();

				self::wait();
			}
		}
	}
	public static function launch(callable $callback, callable $userCallback) {
		self::init_();

		self::demosHandshake_();

		self::ignition_();

		self::serve_( $callback, $userCallback );
	}
    public function getAction(&$id) {
        static $firstLaunch = true;

        if (!$firstLaunch) {
            $this->write_(pack("a*x", "StandBy_1"));
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

class XDHq_DEMO extends XDHq_SHRD {
    static $headContent;
    public static function launch(callable $callback, callable $userCallback, string $headContent) {
        self::$headContent = $headContent;
		XDHqDOM_DEMO::launch( $callback, $userCallback );
    }
}

