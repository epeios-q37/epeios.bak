<?php
/*
Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

This file is part of XDHq.

XDHq is free software: you can redistribute it and/or
modify it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

XDHq is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with XDHq If not, see <http://www.gnu.org/licenses/>.
 */

function readAsset($path) {
    // Due to multi-threading constraints, a global variable can not be used here.
    return Atlas::readAsset($path, "chatroom");
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
    private $pseudo = "";
    private $lastMessage = 0;

    function __construct($shared) {
        $this->shared = $shared;
    }

    private function buildXML_($dom) {
        $i = count($this->shared->messages) - 1;
        $xml = Atlas::createXML("XDHTML");
        $xml->pushTag("Messages");
        $xml->setAttribute("pseudo", $this->pseudo);

        while ($i >= $this->lastMessage) {
            $message = $this->shared->messages[$i];
            $xml->pushTag("Message");
            $xml->setAttribute("pseudo", $message['pseudo']);
            $xml->setValue($message['content']);
            $xml->popTag();
            $i--;
        }

        $this->lastMessage = count($this->shared->messages);

        return $xml->toString();
    }

    private function displayMessages_($dom) {
        if ((count((array)$this->shared->messages) - 1) >= $this->lastMessage) {
            $xml = $dom->synchronized(function ($dom) {return $this->buildXML_($dom);}, $dom);

            $id = $dom->createElement("span");
            $dom->setLayoutXSL($id, $xml, "Messages.xsl");
            $dom->insertChild($id, "Board");
        }
    }

    private function connect_($dom) {
        $dom->setLayout("", readAsset("Main.html"));
        $dom->focus("Pseudo");
        $dom->setTimeout(1000, "Update");
        $dom->synchronized(function ($dom) {$this->displayMessages_($dom);}, $dom);
    }

    private function handlePseudo_($pseudo) {
        if (in_array($pseudo, (array) $this->shared->pseudos)) {
            return false;
        } else {
            $this->shared->pseudos[] = $pseudo;
            return true;
        }
    }

    private function submitPseudo_($dom, $id) {
        $pseudo = $dom->getContent("Pseudo");

        $pseudo = trim($pseudo);

        if (strlen($pseudo) == 0) {
            $dom->alert("Pseudo. can not be empty !");
            $dom->setContent("Pseudo", "");
            $dom->focus("Pseudo");
        } else if ($dom->synchronized(function ($pseudo) {return $this->handlePseudo_(strtoupper($pseudo));}, $pseudo)) {
            $this->pseudo = $pseudo;
            $dom->addClass("PseudoButton", "hidden");
            $dom->disableElements(["Pseudo", "PseudoButton"]);
            $dom->enableElements(["Message", "MessageButton"]);
            $dom->setContent("Pseudo", $pseudo);
            $dom->focus("Message");
            echo ("\t>>>> New user: " . $pseudo . "\n");
        } else {
            $dom->alert("Pseudo. not available !");
            $dom->setContent("Pseudo", $pseudo);
            $dom->focus("Pseudo");

        }
    }

    private function addMessage_($pseudo, $message) {
        $message = trim($message);

        if (strlen($message) != 0) {
            echo "'" . $pseudo . "': " . $message . "\n";
            $this->shared->messages[] = [
                'pseudo' => $pseudo,
                'content' => $message,
            ];
        }
    }

    private function submitMessage_($dom, $id) {
        $message = $dom->getContent("Message");
        $dom->setContent("Message", "");
        $dom->focus("Message");
        $dom->synchronized(function ($pseudo, $message) {$this->addMessage_($pseudo, $message);}, $this->pseudo, $message);
        $this->displayMessages_($dom);
    }

    private function update_($dom, $id) {
        $this->displayMessages_($dom);
        $dom->setTimeout(1000, "Update");
    }

    public function handle($dom, $action, $id) {
        switch ($action) {
        case "":
            $this->connect_($dom, $id);
            break;
        case "SubmitPseudo":
            $this->submitPseudo_($dom, $id);
            break;
        case "SubmitMessage":
            $this->submitMessage_($dom, $id);
            break;
        case "Update":
            $this->update_($dom, $id);
            break;
        default:
            throw new Exception("Unknown action '" . $action . "' !!!");
        }
    }
}

$shared = new Shared();

function myNew() {
    global $shared;
    return new Chatroom($shared);
}

Atlas::launch('myNew', readAsset("Head.html"), null, "chatroom");
?>
