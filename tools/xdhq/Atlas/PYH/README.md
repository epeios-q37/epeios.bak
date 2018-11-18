# *Python* version of the *Atlas* toolkit

![Python logo](https://q37.info/download/assets/Python.png "Python")

The [*Atlas* toolkit](https://atlastk.org/) is a library dedicated to the handling of web interfaces.

## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) type application made with the *Atlas* toolkit looks like:

![Little demonstration](http://q37.info/download/assets/Hello.gif "A basic example")

For an live demonstration: <http://q37.info/runkit/Hello>.

Source code:

```Python

# Following two lines can be removed if the 'Atlas.python.zip' file is referenced in 'PYTHONPATH'
import os, sys
sys.path.append("Atlas.python.zip") # Add the path to 'Atlas.python.zip' if needed.

import Atlas

def acConnect(this, dom, id):
dom.setLayout("", body )
dom.focus( "input")

callbacks = {
  "Connect": acConnect,
  "Typing": lambda this, dom, id:
    dom.setContent("name", dom.getContent(id)),
  "Clear": lambda this, dom, id:
    dom.setContents( {  "input": "", "name": ""} )
    if dom.confirm( "Are you sure ?" ) else None
}
  
head = """
<title>"Hello, World !" example</title>
<style type="text/css">
 html, body { height: 100%; padding: 0; margin: 0; }
 .vcenter-out, .hcenter { display: table; height: 100%; margin: auto; }
 .vcenter-in { display: table-cell; vertical-align: middle; }
</style>
"""

body = """
<div class="vcenter-out">
 <div class="vcenter-in">
  <fieldset>
   <label>Name:</label>
   <input id="input" maxlength="20" placeholder="Enter your name here" type="text" data-xdh-onevent="input|Typing" />
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
"""

Atlas.launch("Connect", callbacks, lambda: None, head)
```

## *TodoMVC*

And here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like:

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](https://github.com/epeios-q37/todomvc-java)

For an live demonstration: <http://q37.info/runkit/TodoMVC>.

## Content of the repository

The `Atlas` directory contains the *Python* source code of the *Atlas* toolkit, which is not needed to run the examples.s

`Atlas.python.zip` is the file you have to reference in `PYTHONPATH` in order to use the *Atlas* toolkit in your own application.

All other directories are examples.

To run an example, launch `python <Name>`, where `<Name>` is the name of the example (`Blank`, `Chatroom`…).

The *Atlas* toolkit is also available for:

- *Java*: <http://github.com/epeios-q37/atlas-java>
- *Node.js*: <http://github.com/epeios-q37/atlas-node>
- *PHP*: <http://github.com/epeios-q37/atlas-php>

For more information about the *Atlas* toolkit, go to <http://atlastk.org/>.