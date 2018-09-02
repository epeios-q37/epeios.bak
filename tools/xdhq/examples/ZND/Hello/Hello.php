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

/*
	This is the one-file version, which does not need the 'Head.html' and the 'Main.html' files.
	See next comments.
*/

function getAtlas() {
 if (getenv("EPEIOS_SRC") === false) {
  $atlas_path = realpath(dirname(__FILE__)) . '/../';
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

  $atlas_path = $epeios_path . "tools/xdhq/Atlas/ZND/";
 }

 require $atlas_path . "Atlas.php";
}

getAtlas();

/*
	Before publication, remove this comment, all the above lines
	and uncomment following line.
	See also next comment.
*/
// require "phar://Atlas.phar/Atlas.php";

class Hello extends Threaded {
 static $head = <<<EOT
<title>"Hello, World !" example</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<style type="text/css">
 html,
 body {
  height: 100%;
  padding: 0;
  margin: 0;
 }

 .vcenter-out,
 .hcenter {
  display: table;
  height: 100%;
  margin: auto;
 }

 .vcenter-in {
  display: table-cell;
  vertical-align: middle;
 }
</style>
EOT;

 static $body = <<<EOT
<div class="vcenter-out">
 <div class="vcenter-in">
  <fieldset>
   <label>Name:</label>
   <input id="input" maxlength="20" placeholder="Enter a name here" type="text" data-xdh-onevent="input|Typing" />
   <button data-xdh-onevent="Clear">Clear</button>
   <hr />
   <h1>
    <span>Hello </span>
    <span style="font-style: italic;" id="name"></span>
    <!-- <span style="font-style: italic;" id="name" />	With this line rather then the above one,
                                                         the following line becomes a child of the above tag !!! -->
    <span>!</span>
   </h1>
  </fieldset>
 </div>
</div>
EOT;
 
 public function handle($dom, $action, $id) {
  switch ($action) {
  case "Connect":
   $dom->headUp(self::$head);
   $dom->setLayout("", self::$body);
   break;
  case "Typing":
   $dom->setContent("name", $dom->getContent($id));
   break;
  case "Clear":
   if ($dom->confirm("Are you sure?")) {
    $dom->setContents(["input" => "", "name" => ""]);
   }
   break;
  default:
   throw new Exception("Unknown action '" . $action . "' !!!");
  }
 }
}

function hello() {
 return new Hello();
}

Atlas::launch("Connect", 'hello', null, "Hello");
/*
	Before publication, remove above line
	and uncomment following line.
*/
// Atlas::launch("Connect", 'hello');
?>