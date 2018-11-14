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
 $dir = ".";

 if (XDHq::isDev()) {
  $dir = "TodoMVC";
 }

 // Due to multi-threading constraints, a global variable can not be used here.
 return Atlas::readAsset($path, $dir);
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

/*
In threaded classes, arrays are volatile, so no function
which takes a reference can be used ('array-splice()', for example).
Arrays are also immutable.
Hence the way 'todos' member is handled.
 */
class TodoMVC extends Threaded {
 private $exclude = NULL;
 private $id = -1;
 private $todos = [];
 private $next = 0; // next free index of '$todos'.

 function remove($index) {
  $limit = $this->next - 1;

  while ($index < $limit) {
   $this->todos[$index] = $this->todos[$index + 1];
   $index++;
  }

  unset($this->todos[$limit]);

  $this->next--;
 }

 public function itemsLeft() {
  $i = $this->next;
  $count = 0;

  while ($i--) {
   if (!$this->todos[$i]['completed']) {
    $count++;
   }
  }

  return $count;
 }

 function put($todo, $id, $xml) {
  $xml->pushTag("Todo");
  $xml->setAttribute("id", $id);
  $xml->setAttribute("completed", $todo['completed'] ? "true" : "false");
  $xml->setValue($todo['label']);
  $xml->popTag();
 }

 function displayCount($dom, $count) {
  $text = "";

  switch ($count) {
  case 0:
   break;
  case 1:
   $text = "1 item left";
   break;
  default:
   $text = $count . " items left";
   break;
  }

  $dom->setContent("Count", $text);
 }

 function handleCount($dom) {
  $count = $this->itemsLeft();

  if ($count != $this->next) {
   $dom->disableElement("HideClearCompleted");
  } else {
   $dom->enableElement("HideClearCompleted");
  }

  $this->displayCount($dom, $count);
 }

 function displayTodos($dom) {
  $xml = Atlas::createXML("XDHTML");
  $xml->pushTag("Todos");
  $i = 0;
  $count = $this->next;

  while ($i < $count) {
   $this->put($this->todos[$i], $i, $xml);

   $i++;
  }

  $dom->setLayoutXSL("Todos", $xml->toString(), "Todos.xsl");
  $this->handleCount($dom);
 }

 function submitNew($dom) {
  $content = trim($dom->getContent("Input"));
  $dom->setContent("Input", "");
//  var_dump( $this->todos );
  if ($content != "") {
   $this->todos[$this->next++] = [
    'completed' => false,
    'label' => $content,
   ];
   $this->displayTodos($dom);
  }
 }

 function submitModification($dom) {
  $id = $this->id;
  $this->id = -1;

  $content = trim($dom->getContent("Input." . $id));
  $dom->setContent("Input." . $id, "");

  if ($content != "") {
   $this->todos[$id]['label'] = $content;

   $dom->setContent("Label." . $id, $content);

   $dom->removeClasses([
    'View.' . $id => "hide",
    'Todo.' . $id => "editing",
   ]);
  } else {
   $this->remove($id);
   $this->displayTodos($dom);
  }
 }

 function destroy($dom, $id) {
  $this->remove(intval($dom->getContent($id)));
  $this->displayTodos($dom);
 }

 function toggle($dom, $id) {
  $i = intval($id);

  $this->todos[$i]['completed'] = !$this->todos[$i]['completed'];

  $dom->toggleClass("Todo." . $id, "completed");
  $dom->toggleClass("Todo." . $id, "active");

  $this->handleCount($dom);
 }

 function all($dom) {
  $this->exclude = null;

  $dom->addClass("All", "selected");
  $dom->removeClasses([
   'Active' => "selected",
   'Completed' => "selected",
  ]);

  $dom->disableElements(["HideActive", "HideCompleted"]);
 }

 function active($dom) {
  $this->exclude = true;

  $dom->addClass("Active", "selected");
  $dom->removeClasses([
   'All' => "selected",
   'Completed' => "selected",
  ]);

  $dom->disableElement("HideActive");
  $dom->enableElement("HideCompleted");

 }

 function completed($dom) {
  $this->exclude = false;

  $dom->addClass("Completed", "selected");
  $dom->removeClasses([
   'All' => "selected",
   'Active' => "selected",
  ]);

  $dom->enableElement("HideActive");
  $dom->disableElement("HideCompleted");
 }

 function clear($dom) {
  $i = 0;
  $count = $this->next;

  while ($i < $count) {
   if ($this->todos[$i]['completed']) {
    $this->remove($i);
    $count--;
   } else {
    $i++;
   }
  }

  $this->displayTodos($dom);
 }

 function edit($dom, $id) {
  $content = $dom->getContent($id);
  $this->id = intval($content);

  $dom->addClasses([
   "View." . $content => "hide",
   $id => "editing",
  ]);

  $dom->setContent("Input." . $content, $this->todos[$this->id]['label']);
  $dom->focus("Input." . $content);
 }

 function cancel($dom) {
  $id = $this->id;
  $this->id = -1;

  $dom->setContent("Input." . $id, "");
  $dom->removeClasses([
   'View.' . $id => 'hide',
   'Todo.' . $id => 'editing',
  ]);
 }

 function handle($dom, $action, $id) {
  $this->test = "Hello !!!";
  switch ($action) {
  case "Connect":
   $dom->setLayout("", readAsset("Main.html"));
   $dom->focus("Input");
   $dom->disableElements(["HideActive", "HideCompleted"]);
   $this->displayTodos($dom);
   break;
  case "Submit":
   if ($this->id == -1) {
    $this->submitNew($dom);
   } else {
    $this->submitModification($dom);
   }
   break;
  case "Destroy":
   $this->destroy($dom, $id);
   break;
  case "Toggle":
   $this->toggle($dom, $id);
   break;
  case "All":
   $this->all($dom);
   break;
  case "Active":
   $this->active($dom);
   break;
  case "Completed":
   $this->completed($dom);
   break;
  case "Clear":
   $this->clear($dom);
   break;
  case "Edit":
   $this->edit($dom, $id);
   break;
  case "Cancel":
   $this->cancel($dom);
   break;
  default:
   die("???");
   break;
  }
 }
}

function myNew() {
 return new TodoMVC();
}

function main() {
 $type = null;

 if (XDHq::isDev()) {
  Atlas::launch("Connect", 'myNew', readAsset("HeadDEMO.html"), null, "TodoMVC");
 } else {
  Atlas::launch("Connect", 'myNew', readAsset("HeadDEMO.html"), null, ".");
 }
}

main();
?>
