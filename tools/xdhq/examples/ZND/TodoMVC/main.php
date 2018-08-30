<?php
/*
Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

This file is part of XDHq.

WDHq is free software: you can redistribute it and/or
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
   echo "Unknown OS !!!\n";
   break;
  }

  $atlasPath = $epeiosPath . "tools/xdhq/Atlas/ZND/";
 }

 require $atlasPath . "Atlas.php";
}

getAtlas();

class TodoMVC {
 public $exclude = NULL;
 public $id = -1;
 public $todos = [];/*
  = [
  [
   'completed' => true,
   'label' => "Note 1",
  ],
  [
   'completed' => false,
   'label' => "Note 2",
  ],
 ];
 */
 public function itemsLeft() {
  $i = count($this->todos);
  $count = 0;

  while ($i--) {
   if (!$this->todos[$i]['completed']) {
    $count++;
   }
  }

  return $count;
 }
 function push($todo, $id, &$tree) {
  $tree .= "<Todo";
// $tree->pushTag( 'Todo' );

  $tree .= " id=\"" . $id . "\"";
// $tree->putAttribute ('id', $id );

  if ($todo['completed']) {
   $tree .= " completed=\"true\"";
//  $tree->putAttribute( 'completed', 'true' );
  } else {
   $tree .= " completed=\"false\"";

//  $tree->putAttribute( 'completed', 'false');
  }

  $tree .= ">";
  $tree .= htmlspecialchars($todo['label']);
// $tree->putValue( $todo['label']);

  $tree .= "</Todo>\n";
// $tree->popTag();
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

  if ($count != count($this->todos)) {
   $dom->disableElement("HideClearCompleted");
  } else {
   $dom->enableElement("HideClearCompleted");
  }

  $this->displayCount($dom, $count);
 }

 function displayTodos($dom) {
  $tree = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<XDHTML>\n<Todos>\n";
  $i = 0;
  $count = count($this->todos);

// $tree->pushTag( "Todos");

  while ($i < $count) {
   $this->push($this->todos[$i], $i, $tree);

   $i++;
  }

  $tree .= "</Todos>\n</XDHTML>";
// $tree->popTag();

  $dom->setLayoutXSL("Todos", $tree, "Todos.xsl");
  $this->handleCount($dom);
 }

 function submitNew($dom) {
  $content = $dom->getContent("Input");
  $dom->setContent("Input", "");
  var_dump( $this->todos );
  if (trim($content) != "") {
   array_unshift($this->todos, [
    'completed' => false,
    'label' => $content,
   ]);
   var_dump( $this->todos );
   $this->displayTodos($dom);
  }
 }

 function submitModification($dom) {
  $id = $this->id;
  $this->id = -1;

  $content = $dom->getContent("Input." . $id);
  $dom->setContent("Input." . $id, "");

  if (trim($content) != "") {
   $this->todos[$id]['label'] = $content;

   $dom->setContent("Label." . $id, $content);

   $dom->removeClasses([
    'View.' . $id => "hide",
    'Todo.' . $id => "editing",
   ]);
  } else {
   array_splice($this->todos, $id, 1);
   displayTodos($dom);
  }
 }

 function toggle($dom, $id) {
  $i = intval($id);

  $this->todos[$i]['completed'] = !$this->todos[$i]['completed'];

  $this->toggleClass("Todo." . $id, "completed");
  $this->toggleClass("Todo." . $id, "active");

  handleCount($dom);
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
  $count = count($this->todos);

  while ($i < $count) {
   if ($this->todos[$i]['completed']) {
    array_splice($this->todos, $i, 1);
    $count--;
   } else {
    $i++;
   }

  }

  displayTodos($dom);
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
  switch ($action) {
  case "Connect":
   $dom->headUp(Atlas::readAsset("HeadDEMO.html"));
   $dom->setLayout("", Atlas::readAsset("Main.html"));
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
   array_splice($this->todos, intval($dom->getContent($id)), 1);
   $this->displayTodos($dom);
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
  Atlas::launch("Connect", 'myNew', null, "TodoMVC");
 } else {
  Atlas::launch("Connect", 'myNew', null, ".");
 }

}

main();
?>
