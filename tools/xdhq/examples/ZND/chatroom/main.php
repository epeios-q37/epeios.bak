<?php
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDGq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
*/

function readAsset( $path ) {
	// Due to multi-threading constraints, a global variable can not be used here.
	return Atlas::readAsset( $path, "chatroom" );
}

function getAtlas() {
 if (getenv("EPEIOS_SRC") === false) {
  $atlasPath = "phar://Atlas.phar/";
 } else {
  switch (strtoupper(substr(php_uname('s'), 0, 3))) {
  case "WIN":
   $epeiosPath = "h:\\hg\\epeios\\";
   break;
  case "LIN":
   $epeiosPath = "/home/csimon/hg/epeios/";
   break;
  case "DAR":
   $epeiosPath = "/Users/csimon/hg/epeios/";
   break;
  default:
   echo "Unknown OS !!!\n";
   break;
  }

  $atlasPath = $epeiosPath . "tools/xdhq/Atlas/ZND/";
 }

 require $atlasPath . "Atlas.php";
}

getAtlas();

class Shared extends Threaded {
	public $messages = [];
	public $pseudos = [];
}

class Chatroom extends Threaded {
 private $shared;
 private $pseudo = "";
 private $lastMessage = 0;

 function __construct( $shared ) {
 	 $this->shared = $shared;
	 var_dump( $shared, $this->shared );
 }

 private function buildTree_( $dom ) {
	$i = count( $this->shared->messages) - 1;
	$tree="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<XDHTML>\n\t<Messages pseudo=\"" . $this->pseudo ."\">\n";

	while ( $i >= $this->lastMessage ) {
  		$message = $this->shared->messages[$i];
		$tree .= "\t\t<Message id=\"" . $i . "\" pseudo=\"" . $message['pseudo'] . "\">" . $message['content'] . "</Message>\n";
		$i--;
	}

	$this->lastMessage = count( $this->shared->messages );

	return $tree . "\t</Messages>\n</XDHTML>";
 }

 private function displayMessages_( $dom ) {
   if ( ( count( $this->shared->messages) - 1 )>= $this->lastMessage ) {
		$tree = $dom->synchronized( function( $dom ){ return $this->buildTree_( $dom ); }, $dom );

		var_dump( $tree );

		$id = $dom->createElement( "span" );
		$dom->setLayout( $id, $tree, "Messages.xsl");
		$dom->insertChild( $id, "Board");
	}
 }

 private function connect_( $dom ) {
   $dom->setLayout("", readAsset("Main.html"));
   $dom->focus( "Pseudo");
   echo( "P\n");
   $dom->setTimeout( 1000, "Update");
   echo( "Av\n");
   $dom->synchronized( function($dom){$this->displayMessages_($dom);}, $dom );
   echo( "Ap\n");
 }

 private function handlePseudo_( $pseudo ) {
	var_dump( $this->shared->pseudos );

 	 if ( in_array( $pseudo, (array)$this->shared->pseudos ) )
		return false;
	else {
		$this->shared->pseudos[] = $pseudo;
		return true;
	}
 }

 private function submitPseudo_( $dom, $id ) {
 	 $pseudo = $dom->getContent( "Pseudo" );

	 $pseudo = trim( $pseudo );

	 if ( strlen( $pseudo ) == 0 ) {
	 	 $dom.alert( "Pseudo. can not be empty !");
		 $dom.setContent( "Pseudo", "" );
		 $dom->focus( "Pseudo");
	 } else if ( $dom->synchronized( function($pseudo){return $this->handlePseudo_($pseudo);}, $pseudo ) ) {
		$this->pseudo = $pseudo;
		$dom->setContent( "Pseudo", $pseudo );
		$dom->addClass( "PseudoButton", "hidden" );
		$dom->disableElements(["Pseudo", "PseudoButton"]);
		$dom->enableElements(["Message", "MessageButton"]);
		$dom->focus( "Message");
		echo( "\t>>>> New user: " . $pseudo);
	 } else {
	 	 $dom->alert( "Pseudo. not available !");
		 $dom->setContent( "Pseudo", $pseudo );
		 $dom->focus( "Pseudo" );

	 }
 }

  private function addMessage_( $pseudo, $message ) {
 	 $message = trim( $message );

	 if ( strlen( $message ) != 0 ) {
	 	 $this->shared->messages[] = [
		 	 'pseudo' => $pseudo,
			 'content' => $message
		 ];
	 }

	 var_dump( $this->shared);
 }

 private function submitMessage_( $dom, $id ) {
	$message = $dom->getContent( "Message" );
	$dom->setContent( "Message", "" );
	$dom->focus( "Message" );
	$dom->synchronized( function($pseudo, $message){$this->addMessage_( $pseudo, $message);}, $this->pseudo, $message );
	$this->displayMessages_( $dom );
 }

 private function update_( $dom, $id ) {
	$this->displayMessages_( $dom );
 	$dom->setTimeout( 1000, "Update");
 }

 public function handle($dom, $action, $id) {
  switch ($action) {
  case "Connect":
   $this->connect_( $dom, $�d );
   break;
  case "SubmitPseudo":
   $this->submitPseudo_( $dom, $id );
   break;
  case "SubmitMessage":
   $this->submitMessage_( $dom, $id );
   break;
  case "Update":
//   $this->update_( $dom, $id );
   break;
  default:
   throw new Exception("Unknown action '" . $action . "' !!!");
  }
 }
}

$shared = new Shared();

function myNew() {
 global $shared;
 return new Chatroom( $shared );
}

Atlas::launch("Connect", 'myNew', readAsset( "Head.html" ), null, "chatroom" );
?>
