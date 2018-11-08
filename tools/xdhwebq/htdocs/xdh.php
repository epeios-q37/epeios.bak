<?php
/*
Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

This file is part of xdhwebq.

xdhwebq is free software: you can redistribute it and/or
modify it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

xdhwebq is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with xdhwebq. If not, see <http://www.gnu.org/licenses/>.
 */

error_reporting(E_ALL);

$host = "localhost";
$service = 53741;

function send($query) {
 /* creates a TCP/IP socket. */
 global $host, $service;

 $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

 if ($socket === false) {
  echo "socket_create() fails :  " . socket_strerror(socket_last_error()) . "\n";
  return;
 }

 $result = socket_connect($socket, gethostbyname($host), $service);

 if ($result === false) {
  echo "socket_connect() fails : ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
  return;
 }

 socket_write($socket, $query, strlen($query));

 $answer = "";

 while ($data = socket_read($socket, 2048)) {
  $answer .= $data;
 }

 socket_close($socket);

 return $answer;
}

if (isset($_REQUEST["_prolog"]) || (isset($_REQUEST["_token"]) && !isset($_REQUEST["_cont"]))) {
 $cgi = $_SERVER['PHP_SELF'];
 $action = $_REQUEST["_action"];
 $language = $_REQUEST["_language"];
 $UserID = $_REQUEST["UserID"];
 $Password = $_REQUEST["Password"];
 $token = $_REQUEST["_token"];
 $head = send("_token" . "\00" . $token . "\00" . "_head" . "\00\00\00");

 require 'prolog.php';

 echo $out;
} else {
 $in = "";

 foreach ($_REQUEST as $key => $value) {
  $in .= $key . "\00" . $value . "\00";
 }

 $in .= "_CGI\00" . $_SERVER['PHP_SELF'] . "\00\00";

 echo send($in);
}
?>

