# *PHP* version of the *Atlas* toolkit

![PHP logo](https://q37.info/download/assets/PHP.png "PHP")

The [*Atlas* toolkit](https://atlastk.org/) is a library to handle web interfaces.

## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) type application made with the *Atlas* toolkit looks like:

![Little demonstration](http://q37.info/download/assets/Hello.gif "A basic example")

For a live demonstration: <http://q37.info/runkit/Hello>.

Source code:

```PHP
<?php
require "phar://Atlas.phar/Atlas.php";

class Hello extends Threaded {
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

$head = <<<EOT
<title>"Hello, World !" example</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<style type="text/css">
 html, body {height: 100%; padding: 0; 	margin: 0;}
 .vcenter-out, .hcenter {display: table; height: 100%; margin: auto;}
 .vcenter-in {display: table-cell; vertical-align: middle;}
</style>
EOT;

Atlas::launch("Connect", 'hello', $head);
?>
```

## *TodoMVC*

And here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](https://github.com/epeios-q37/todomvc-java)

For a live demonstration: <http://q37.info/runkit/TodoMVC>.

## Content of the repository

The `Atlas` directory contains the *PHP* source code of the *Atlas* toolki, which is not needed to run the examples.

`Atlas.phar` is the file which is needed to use the *Atlas* toolkit in your own application.

All other directories are examples.

To run an example, launch `php <Name>/main.php`, where `<Name>` is the name of the example (`Blank`, `Chatroom`…).

NOTA: **You need *PHP* with [*pthreads*](http://php.net/manual/book.pthreads.php)!**

The *Atlas* toolkit is also available for:
- *Java*: <http://github.com/epeios-q37/atlas-java>
- *Node.js*: <http://github.com/epeios-q37/atlas-node>
- *Python*: <http://github.com/epeios-q37/atlas-python>

For more information about the *Atlas* toolkit: <http://atlastk.org/>.