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

// This is the one-file version, which does not need the 'Head.html' and the 'Main.html' files.

function getAtlas() {
 if (getenv("Q37_EPEIOS") === false) {
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

class Hello extends Threaded {
 // The content of the 'Main.html' file.
 static $body = <<<EOT
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
  <input id="input" maxlength="20" placeholder="Enter a name here" type="text" data-xdh-onevent="Submit" value="World"/>
  <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
   <button data-xdh-onevent="Submit">Submit</button>
   <button data-xdh-onevent="Clear">Clear</button>
  </div>
 </fieldset>
</div>
EOT;

 public function handle($dom, $action, $id) {
  switch ($action) {
  case "":
   $dom->setLayout("", self::$body);
   break;
  case "Submit":
   $dom->alert("Hello, " . $dom->getContent("input") . "!" );
   break;
  case "Clear":
   if ($dom->confirm("Are you sure?"))
    $dom->setContent("input", "");
   break;
  default:
   throw new Exception("Unknown action '" . $action . "' !!!");
  }
  $dom->focus("input");
 }
}


function hello() {
 return new Hello();
}

// The content of the 'Head.html' file.
$head = <<<EOT
<title>"Hello, World !" example</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
EOT;

Atlas::launch('hello', $head, null, "Hello");
?>