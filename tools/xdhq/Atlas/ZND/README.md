# *PHP* binding of the *Atlas* toolkit

![PHP logo](https://q37.info/download/assets/PHP.png "PHP")

The [*Atlas* toolkit](https://atlastk.org/) is a library which facilitates the prototyping of web applications.

## *Hello World!*

[![Little demonstration](http://q37.info/download/Hello.gif "A basic example")](http://q37.info/s/atk/Hello/)

```PHP
<?php
require "phar://Atlas.phar/Atlas.php";

class Hello extends Threaded {
 // The content of the 'Main.html' file.
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
    <span>!</span>
   </h1>
  </fieldset>
 </div>
</div>
EOT;
 
 public function handle($dom, $action, $id) {
  switch ($action) {
  case "Connect":
   $dom->setLayout("", self::$body);
   $dom->focus("input");
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

 // The content of the 'Head.html' file.
$head = <<<EOT
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

Atlas::launch("Connect", 'hello', $head, null, "Hello");
?>
```

- Get the `Atlas.phar` file: <https://q37.info/download/assets/Atlas.phar>,
- in the same directory, create a `Hello.php` file with above source code, or get it directly here: <https://q37.info/download/assets/Hello.php>,
- launch `php Hello.php`.

## What's next ?

Here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](http://q37.info/s/atk/TodoMVC)

The source code of this application and more information about the *Atlas* toolkit can be found at <http://atlastk.org/>