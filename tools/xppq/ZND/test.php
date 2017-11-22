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

require "XPPq.php";

echo ZNDq::componentInfo() . "\n" . ZNDq::wrapperInfo() . "\n";

echo XPPq::returnArgument("Argument from the PHP file !") . "\n\n";

$XML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" .
"<SomeTag xmlns:xpp=\"http://q37.info/ns/xpp/\" AnAttribute=\"SomeAttributeValue\">\n" .
" <SomeOtherTag AnotherAttribute=\"AnotherAttributeValue\">TagValue</SomeOtherTag>\n" .
" <xpp:define name=\"SomeMacro\">\n" .
"  <xpp:bloc>Some macro content from string !</xpp:bloc>\n" .
" </xpp:define>\n" .
" <YetAnotherTag YetAnotherAttribute=\"YetAnotherAttributeValue\">\n" .
"  <xpp:expand select=\"SomeMacro\"/>\n" .
" </YetAnotherTag>\n" .
"</SomeTag>\n"
;

function parse($stream) {
	$parser = XPPq::parserNew($stream);

	$token = XPPq::parserParse($parser);

	while ($token != XPPqParser::PROCESSED) {
		switch ($token) {
			case XPPqParser::START_TAG:
				echo "Start tag: '" . XPPqParser::tagName($parser) . "'\n";
			break;
			case XPPqParser::ATTRIBUTE:
				echo "Attribute: '" . XPPqParser::attributeName($parser) . "' = '" . trim(XPPqParser::Value($parser)) . "'\n";
			break;
			case XPPqParser::VALUE:
				echo "Value:     '" . trim(XPPqParser::Value($parser)) . "'\n";
			break;
			case XPPqParser::END_TAG:
				echo "End tag:   '" . XPPqParser::tagName($parser) . "'\n";
			break;
			default:
				die("Unknown token !!!");
		}

		$token = XPPq::parserParse($parser);
	}

	XPPqParser::delete($parser);
}

function getStream() {
	if (true) return fopen('data://text/plain,' . $GLOBALS["XML"], 'r');
	else return fopen('Project.xml', 'r');
}

function getProcessedStream($stream) {
	$GLOBALS["XMLStream"] = $stream;
	return fopen("xpp://XMLStream", 'r');
}

function test0($stream) {
	echo "No treatment ; to see the original file :\n";
	echo "-----------------------------------------\n";
	echo stream_get_contents($stream) . "\n";
}

function test1($stream) {
	echo "Preprocessing the file :\n";
	echo "------------------------\n";
	echo stream_get_contents(getProcessedStream($stream)) . "\n";
}

function test2($stream) {
	echo "XML parsing WITHOUT preprocessing :\n";
	echo "-----------------------------------\n";
	parse($stream);
}

function test3($stream) {
	echo "XML parsing WITH preprocessing :\n";
	echo "--------------------------------\n";
	parse(getProcessedStream($stream));
}

$argc = $_SERVER["argc"];
$argv = $_SERVER["argv"];

$test = 3;

if ($argc >= 2) {
	if (!is_numeric($argv[1])) die("'" . $argv[1] . "' is not a valid test id ; must be '0' to '3'.");
	else $test = (int)$argv[1];
}

switch ($test) {
	case 0:
		test0(getStream());
	break;
	case 1:
		test1(getStream());
	break;
	case 2:
		test2(getStream());
	break;
	case 3:
		test3(getStream());
	break;
	default:
		die("'" . $test . "' is not a valid test id ; must be '0' to '3'.");
	break;
}

?>
