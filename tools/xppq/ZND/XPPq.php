<?php
/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq.  If not, see <http://www.gnu.org/licenses/>.
*/

$zndq_affix = "xppq";

/***** Begin of generic part. ****/
if (getenv("EPEIOS_SRC") === false) $zndq_path = realpath(dirname(__FILE__)) . '/';
else {
	switch (strtoupper(substr(php_uname('s') , 0, 3))) {
		case "WIN":
			$zndq_path = "h:\\bin\\";
		break;
		case "LIN":
			$zndq_path = "/home/csimon/bin/";
		break;
		case "DAR":
			$zndq_path = "/Users/csimon/bin/";
		break;
		default:
			echo "Unknown OS !!!\n";
		break;
	}
}

zndq_init();
zndq_register(str_replace(' ', '\ ', str_replace('\\', '/', $zndq_path)) . $zndq_affix . "znd");

class ZNDq {
	static public function wrapperInfo() {
		return zndq_wrapper_info();
	}
	static public function componentInfo() {
		return zndq_component_info();
	}
	static protected function register($arguments) {
		return zndq_register($arguments);
	}
}
/**** End of generic part. ****/

class XPPq extends ZNDq {
	static public function returnArgument($argument) {
		return zndq_wrapper(0, $argument);
	}
	static public function processorNew($stream) {
		return zndq_wrapper(1, $stream);
	}
	static public function processorEOF($stream) {
		return zndq_wrapper(2, $stream);
	}
	static public function processorRead($stream, $count) {
		return zndq_wrapper(3, $stream, $count);
	}
	static public function processorDelete($stream) {
		return zndq_wrapper(4, $stream);
	}
	static public function parserNew($stream) {
		return zndq_wrapper(5, $stream);
	}
	static public function parserParse($parser) {
		return zndq_wrapper(6, $parser);
	}
	static public function parserTagName($parser) {
		return zndq_wrapper(7, $parser);
	}
	static public function parserAttributeName($parser) {
		return zndq_wrapper(8, $parser);
	}
	static public function parserValue($parser) {
		return zndq_wrapper(9, $parser);
	}
	static public function parserDelete($parser) {
		return zndq_wrapper(10, $parser);
	}
}

class XPPqPreprocessor {
	var $stream;
	var $processor = 0;
	function stream_open($path, $mode, $options, &$opened_path) {
		$url = parse_url($path);
		$this->stream = $GLOBALS[$url["host"]];
		$this->processor = XPPq::processorNew($this->stream);

		return true;
	}
	function stream_read($count) {
		return XPPq::processorRead($this->processor, $count);
	}
	function stream_eof() {
		return XPPq::processorEOF($this->processor);
	}
	function stream_stat() {
		return fstat($this->stream);
	}
	function __destruct() {
		XPPq::processorDelete($this->processor);
	}
}

class XPPqParser {
	const PROCESSED = 0;
	const START_TAG = 1;
	const ATTRIBUTE = 2;
	const VALUE = 3;
	const END_TAG = 4;
	static public function New ($stream) {
		return XPPq::parserNew($stream);
	}
	static public function Parse($parser) {
		return XPPq::parserParse($parser);
	}
	static public function TagName($parser) {
		return XPPq::parserTagName($parser);
	}
	static public function AttributeName($parser) {
		return XPPq::parserAttributeName($parser);
	}
	static public function Value($parser) {
		return XPPq::parserValue($parser);
	}
	static public function Delete($parser) {
		return XPPq::parserDelete($parser);
	}
}

stream_wrapper_register("xpp", "XPPqPreprocessor") or die("Failed to register protocol");
?>
