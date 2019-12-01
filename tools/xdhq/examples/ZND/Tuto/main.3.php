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
 if (getenv("Q37_EPEIOS") === false) {
  $zndq_path = realpath(dirname(__FILE__)) . '/';
 } else {
  switch (strtoupper(substr(php_uname('s'), 0, 3))) {
  case "WIN":
   $epeios_path = "h:\\hg\\epeios\\";
   break;
  case "LIN":
   $epeios_path = "/home/csimon/hg/epeios/";
   break;
  case "DAR":
   $epeios_path = "/Users/csimon/hg/epeios/";
   break;
  default:
   echo "Unknown OS !!!\n";
   break;
  }

  $zndq_path = $epeios_path . "tools/xdhq/Atlas/ZND/";
 }

 require $zndq_path . "Atlas.php";
}

getAtlas();

class DOM extends AtlasDOM {
 public $exclude = NULL;
 public $id = -1;
 public $todos = [];
}

function push($todo, $id, $tree) {
 $tree->pushTag('Todo');
 $tree->putAttribute('id', $id);
 $tree->putValue($todo['label']);
 $tree->popTag();
}

function displayTodos($dom) {
 $tree = new AtlasTree();
 $i = 0;
 $count = count($dom->todos);

 $tree->pushTag("Todos");

 while ($i < $count) {
  push($dom->todos[$i], $i, $tree);
  $i++;
 }

 $tree->popTag();

 $dom->setLayout("Todos", $tree, "Todos.3.xsl");
}

function submitNew($dom) {
 $content = $dom->getContent("Input");
 $dom->setContent("Input", "");

 if (trim($content) != "") {
  array_unshift($dom->todos, [
   'completed' => false,
   'label' => $content,
  ]);
  displayTodos($dom);
 }
}

function toggle($dom, $id) {
 $i = intval($id);

 $dom->todos[$i]['completed'] = !$dom->todos[$i]['completed'];

 $dom->toggleClass("Todo." . $id, "completed");
}

function main() {
 $type = null;

 if (isDev()) {
  Atlas::launch("Connect", null, "Tuto");
 } else {
  Atlas::launch("Connect");
 }

 $dom = new DOM();

 while (true) {
  switch ($dom->getAction($id)) {
  case "Connect":
   $dom->setLayout("", new AtlasTree(), "Main.1.xsl");
   $dom->focus("Input");
   break;
  case "Submit":
   submitNew($dom);
   break;
  case "Destroy":
   array_splice($dom->todos, intval($dom->getContent($id)), 1);
   displayTodos($dom);
   break;
  case "Toggle":
   toggle($dom, $id);
   break;
  default:
   die("???");
   break;
  }
 }
}

main();
?>
