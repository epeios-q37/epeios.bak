# *PHP* version of the *Atlas* toolkit

![PHP logo](https://q37.info/download/assets/PHP.png "PHP")

A fast and easy way to add web interfaces to *PHP* applications.

## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) type application made with the *Atlas* toolkit looks like:

![Little demonstration](http://q37.info/download/assets/Hello.gif "A basic example")

For a live demonstration: <http://q37.info/runkit/Hello>.

Source code:

```php
<?php
require "phar://Atlas.phar/Atlas.php";

class Hello extends Threaded {
 static $body = <<<EOT
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
  <input id="input" maxlength="20" placeholder="Enter a name here" type="text"
         data-xdh-onevent="Submit" value="World"/>
  <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
   <button data-xdh-onevent="Submit">Submit</button>
   <button data-xdh-onevent="Clear">Clear</button>
  </div>
 </fieldset>
</div>
EOT;

 public function handle($dom, $action, $id) {
  switch ($action) {
  case "":    // Action label for a new connection.
   $dom->setLayout("", self::$body);
   $dom->focus("input");
   break;
  case "Submit":
   $dom->alert("Hello, " . $dom->getContent("input") . "!");
   $dom->focus("input");
   break;
  case "Clear":
   if ($dom->confirm("Are you sure?"))
    $dom->setContent("input", "");
   $dom->focus("input");
   break;
  }
 }
}

function hello() {
 return new Hello();
}

Atlas::launch('hello');
?>
```

## *TodoMVC*

And here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")

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
- *Ruby*: <http://github.com/epeios-q37/atlas-ruby>

For more information about the *Atlas* toolkit: <http://atlastk.org/>.