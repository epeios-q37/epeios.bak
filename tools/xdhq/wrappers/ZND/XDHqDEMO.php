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

class XDHqDEMO {
	function getEnv_($name, $value = "") {
		if (getenv($name) !== false) {
			return trim(getenv($name));
		} else {
			return trim($value);
		}
	}
}

class XDHqDEMO_Shared extends Threaded {
	public $socket;
	public $instances = [];
	function writeByte($datum) {
		return pack("C", $datum);
	}
	function writeSize($size) {
		$result = pack("C", $size & 0x7f);
		$size >>= 7;

		while ($size != 0) {
			$result = pack("C", ($size & 0x7f) | 0x80) . $result;
			$size >>= 7;
		}

		return $result;
	}
	function writeString($string) {
		return self::writeSize(strlen($string)) . $string;
	}
	function writeStrings($strings) {
		$count = count($strings);
		$i = 0;
		
		$data = self::writeSize($count);

		while ($i < $count) {
			$data .= self::writeString($strings[$i]);
			$i++;
		}

		return $data;
	}
	// Thread-unsafe write.
	function writeTU( $data ) {
		fwrite($this->socket,$data);
		fflush($this->socket);
	}
	function write( $data, Threaded $thread ) {
		$thread->synchronized( function($data) {$this->writeTU($data);}, $data);
	}
	function getByte() {
		while (!($c = fgetc($this->socket))); // Workaround concerning an arbitrary timeout!

		return unpack("C", $c)[1];
	}
	function getSize() {
		$byte = self::getByte();
		$size = $byte & 0x7f;

		while ($byte & 0x80) {
			$byte = self::getByte();
			$size = ($size << 7) + ($byte & 0x7f);
		}

		return $size;
	}
	function getString() {
		$size = self::getSize();

		if ($size) {
			return fread($this->socket, $size);
		} else {
			return "";
		}
	}
	function getStrings() {
		$amount = self::getSize();

		while ($amount--) {
			$strings[] = self::getString();
		}

		return $strings;
	}
	function getQuery() {
		$c = fgetc($this->socket);
		$string = "";

		while ($c != "\0") {
			$string .= $c;
			$c = fgetc(self::$socket);
		}

		return $string;
	}
}

global $xdhq_shared_;
$xdhq_shared_ = new XDHqDEMO_Shared();

class XDHqDEMO_Daemon extends Threaded {
	private static $pAddr = "atlastk.org";
	private static $pPort = 53800;
	private static $wAddr = "";
	private static $wPort = "";
	private static $cgi = "xdh";
	private static $demoProtocolLabel = "877c913f-62df-40a1-bf5d-4bb5e66a6dd9";
	private static $demoProtocolVersion = "0";
	private static $mainProtocolLabel = "6e010737-31d8-4be3-9195-c5b5b2a9d5d9";
	private static $mainProtocolVersion = "0";
	private static $token = "";
    function __construct(XDHqDEMO_Shared $shared) {
        $this->shared = $shared;
    }
	private static function isTokenEmpty_() {
		return empty(self::$token) || (substr(self::$token, 0, 1) == '&');
	}
	// All write function returns data and do not write directly to socket.
	public function init_() {
		switch (XDHqDEMO::getEnv_("ATK")) {
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

		self::$pAddr = XDHqDEMO::getEnv_("ATK_PADDR", self::$pAddr);
		self::$pPort = intval(XDHqDEMO::getEnv_("ATK_PPORT", strval(self::$pPort)));
		self::$wAddr = XDHqDEMO::getEnv_("ATK_WADDR", self::$wAddr);
		self::$wPort = XDHqDEMO::getEnv_("ATK_WPORT", self::$wPort);

		if ( empty(self::$wAddr)) {
			self::$wAddr = self::$pAddr;
		}

		if (!empty(self::$wPort)) {
			self::$wPort = ":" . self::$wPort;
		}


		if (self::isTokenEmpty_()) {
			$token = XDHqDEMO::getEnv_("ATK_TOKEN");

			if (!empty($token)) {
				self::$token = "&" . $token;
			}

		}

		($this->shared->socket = fsockopen(self::$pAddr, self::$pPort, $errno, $errstr) );

		if (!$this->shared->socket) {
			die("$errstr ($errno)\n");
		}
	}
	private function demoHandshake_() {
		$this->shared->writeTU( $this->shared->writeString(self::$demoProtocolLabel) . $this->shared->writeString(self::$demoProtocolVersion));

		$errorMessage = $this->shared->getString();

		if (!empty($errorMessage)) {
			die($errorMessage);
		}

		$notificationMessage = $this->shared->getString();

		if (!empty($notificationMessage)) {
			echo $notificationMessage . "\n";
		}
	}
	private function ignition_() {
		$this->shared->writeTU($this->shared->writeString(self::$token) . $this->shared->writeString(XDHq_DEMO::$headContent));

		self::$token = $this->shared->getString();

		if (self::isTokenEmpty_()) {
			die( $this->shared->getString_());
		}

		if (self::$wPort != ":0") {
			$url = "http://" . self::$wAddr . self::$wPort . "/" . self::$cgi . ".php?_token=" . self::$token;
			echo $url . "\n";
			echo str_pad("",strlen($url),'^') . "\n";
			echo "Open above URL in a web browser. Enjoy!\n";
			XDHq_SHRD::open($url);
		}
	}
	private function serve_($callback,$userCallback) {
		while(true) {
			$id = $this->shared->getByte();

			if ($id == 255) {	// Value reporting a new front-end.
				$id= $this->shared->getByte();

				if ( key_exists( $id, (array)$this->shared->instances) )
					die(  "Instance of id  '" . $id . "' exists but should not !");

				$this->shared->instances[$id] = call_user_func((array)$callback, $userCallback);
				$this->shared->instances[$id]->dom->setDEMOStuff( $this, $id );
				$this->shared->write( $this->shared->writeByte( $id ) . $this->shared->writeString( self::$mainProtocolLabel ) . $this->shared->writeString( self::$mainProtocolVersion ), $this );
				$this->shared->instances[$id]->start();
			} else if ( !key_exists( $id, (array)$this->shared->instances ) ) {
				die( "Unknown instance of id '" . $id . "'!" );
			} else if ( !$this->shared->instances[$id]->handshakeDone ) {
				$error = $this->shared->getString();

				if ( !empty($error))
					die( $error );

				$this->shared->getString();	// Language. Not currently handled.

				$this->shared->write( $this->shared->writeByte($id) . $this->shared->writeString("ZND"), $this );
				$this->shared->instances[$id]->handshakeDone = true;

			} else {
				$this->shared->instances[$id]->synchronized( function($thread) {
					if ( !$thread->notify() )
						die( __LINE__ . ": Notify failed !!!\n" );
				}, $this->shared->instances[$id]);
				$this->synchronized(function ($thread) {
					if ( !$thread->wait())
						die( __LINE__ . ": Wait failed !!!\n" );
				}, $this );
			}
		}
	}
	public function launch(callable $callback, callable $userCallback) {
		self::init_();
		self::demoHandshake_();
		self::ignition_();

		$this->serve_( $callback, $userCallback );
	}
}

class XDHqDOM_DEMO extends Threaded {
	function __construct () {
		$this->firstLaunch = true;
	}
	private function wait_() {
		$this->parent->synchronized(function ($thread) {
			if ( !$thread->wait())
				die( __LINE__ . ": Wait failed !!!\n" );
		}, $this->parent);
	}
	private function notify_() {
		$this->daemonThread->synchronized( function ($thread) {
			if ( !$thread->notify())
				die( __LINE__ . ": Notify failed !!!\n" );
		}, $this->daemonThread);
	}
    public function getAction(&$id) {
        if (!$this->firstLaunch) {
            $this->shared->write($this->shared->writeByte($this->id) . pack("a*x", "StandBy_1"),$this->daemonThread);
        } else {
            $this->firstLaunch = false;
        }

		$this->wait_();

        $id = $this->shared->getString();
        $action = $this->shared->getString();

		$this->notify_();
		
        return $action;
    }
    public function call($command, $type, ...$args) {
        $i = 0;

		$data = $this->shared->writeByte($this->id);

        $data .= pack("a*x", $command);

        $amount = $args[$i];
        $i++;

        while ($amount--) {
            $data .= $this->shared->writeString($args[$i]);
            $i++;
        }

        $amount = $args[$i];
        $i++;

        while ($amount--) {
            $data .= $this->shared->writeStrings($args[$i]);
            $i++;
        }

        $this->shared->write( $data, $this->daemonThread );

		$object = NULL;

        switch ($type) {
        case XDHq::RT_NONE:
            break;
        case XDHq::RT_STRING:
			$this->wait_();
            $object = $this->shared->getString();
			$this->notify_();
            break;
        case XDHq::RT_STRINGS:
			$this->wait_();
            $object = $this->shared->getStrings();
			$this->notify_();
            break;
        default:
            throw new Exception("Unknown return type !!!");
        }

		return $object;
    }
}

class XDHq_DEMO extends XDHq_SHRD {
    static $headContent;
    public static function launch(callable $callback, callable $userCallback, string $headContent) {
		global $xdhq_shared_;
        self::$headContent = $headContent;
		$daemon = new XDHqDEMO_Daemon($xdhq_shared_);
		$daemon->launch( $callback, $userCallback );
    }
}

