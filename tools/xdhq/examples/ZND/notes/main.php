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
   die("Unknown OS !!!\n");
   break;
  }

  $atlasPath = $epeiosPath . "tools/xdhq/Atlas/ZND/";
 }

 require $atlasPath . "Atlas.php";
}

getAtlas();

function getViewModeElements() {
 return ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"];
}

function put($note, $id, $xml) {
 $xml->pushTag("Note");
 $xml->setAttribute("id", $id);

 foreach ((array) $note as $key => $value) {
  $xml->pushTag($key);
  $xml->setValue($value);
  $xml->popTag();
 }

 $xml->popTag();
}

function readAsset($path) {
 // Due to multi-threading constraints, a global variable can not be used here.
 return Atlas::readAsset($path, "notes");
}

class Notes extends Threaded {
 public $pattern = "";
 public $hideDescriptions = false;
 public $id = 0;
 public $notes = [
  // First one must be empty; it is used as buffer for new entries.
  [
   'title' => '',
   'description' => '',
  ],
  [
   'title' => 'Improve design',
   'description' => "Tastes and colors... (aka «CSS aren't my cup of tea...»)",
  ],
  [
   'title' => 'Fixing bugs',
   'description' => "There are bugs ? Really ?",
  ],
  [
   'title' => 'Implement new functionalities',
   'description' => "Although it's almost perfect..., isn't it ?",
  ],
 ];

 function handleDescriptions($dom) {
  if ($this->hideDescriptions) {
   $dom->disableElement("ViewDescriptions");
  } else {
   $dom->enableElement("ViewDescriptions");
  }
 }

 function displayList($dom) {
  $xml = Atlas::createXML("XDHTML");
  $i = 1; // 0 skipped, as it serves as buffer for the new ones.
  $contents = [];

  $xml->pushTag("Notes");

  $count = count($this->notes);

  while ($i < $count) {
   if (strtolower(substr($this->notes[$i]['title'], 0, strlen($this->pattern))) === $this->pattern) {
    put($this->notes[$i], $i, $xml);
    $contents["Description." . $i] = $this->notes[$i]['description'];
   }
   $i++;
  }

  $dom->setLayoutXSL("Notes", $xml, "Notes.xsl");
  $dom->setContents($contents);
  $dom->enableElements(getViewModeElements());
 }

 function edit($dom, $id) {
  $this->id = intval($id);
  $dom->setLayout("Edit." . $id, readAsset("Note.html"));
  $dom->setContents(
   [
    "Title" => $this->notes[$this->id]['title'],
    "Description" => $this->notes[$this->id]['description'],
   ]);
  $dom->disableElements(getViewModeElements());
  $dom->dressWidgets("Notes");
 }

 function view($dom) {
  $dom->enableElements(getViewModeElements());
  $dom->setContent("Edit." . $this->id, "");
  $this->id = -1;
 }

 function shift() {
  $i = count($this->notes) - 1;

  while ($i >= 0) {
   $this->notes[$i + 1] = $this->notes[$i];
   $i--;
  }
 }

 function delete($i) {
  $l = count($this->notes) - 1;

  while ($l > $i) {
   $this->notes[$l - 1] = $this->notes[$l];
   $l--;
  }

  unset($this->notes[count($this->notes) - 1]);
 }

 public function handle($dom, $action, $id) {
  switch ($action) {
  case "":
   $dom->setLayout("", readAsset("Main.html"));
   $this->displayList($dom);
   break;
  case "ToggleDescriptions":
   $this->hideDescriptions = $dom->getContent($id) == "true";
   $this->handleDescriptions($dom);
   break;
  case "Search":
   $this->pattern = strtolower($dom->getContent("Pattern"));
   $this->displayList($dom);
   break;
  case "Edit":
   $this->edit($dom, $dom->getContent($id));
   $dom->focus("Title");
   break;
  case "Delete":
   if ($dom->confirm("Are you sure you want to delete this entry ?")) {
    $this->delete(intval($dom->getContent($id)));
    // Below does not work in threaded class.
    // array_splice($this->notes, intval($dom->getContent($id)), 1);
    $this->displayList($dom);
   }
   break;
  case "Submit":
   $result = $dom->getContents(["Title", "Description"]);

   $title = trim($result['Title']);
   $description = $result['Description'];

   if ($title != '') {
    $this->notes[$this->id]['title'] = $title;
    $this->notes[$this->id]['description'] = $description;
    if ($this->id == 0) {
     $new = array_merge(['title' => '', 'description' => ''], (array) $this->notes);
     $this->shift();
     $this->notes[0] = ['title' => '', 'description' => ''];
     // Below does not work in threaded class.
     // array_unshift($this->notes, ['title' => '', 'description' => '']);
     $this->displayList($dom);
    } else {
     $contents["Title." . $this->id] = $title;
     $contents["Description." . $this->id] = $description;
     $dom->setContents($contents);
     $this->view($dom);
    }
   } else {
    $dom->alert("Title can not be empty !");
    $dom->focus("Title");
   }

   break;
  case "Cancel":
   $result = $dom->getContents(["Title", "Description"]);
   if (($this->notes[$this->id]['title'] != $result['Title']) or ($this->notes[$this->id]['description'] != $result['Description'])) {
    if ($dom->confirm("Are you sure you want to cancel your modifications ?")) {
     $this->view($dom);
    }

   } else {
    $this->view($dom);
   }

   break;
  default:
   die("???");
   break;
  }
 }
}

function notes() {
 return new Notes();
}

Atlas::launch('notes', readAsset("Head.html"), null, "notes");
?>
