# Binding for *Node.js* of the *Atlas* toolkit

The *Atlas* toolkit is a library for web and desktop UI. This is the binding of this toolkit for *Node.js*.


## *Hello, World!*

[![Little demonstration](https://q37.info/download/Hello.gif "A basic example")](http://q37.info/s/atk/Hello/)

## source code

```javascript
const atlas = require( 'atlastk' );

const head = [
	'<title>"Hello, World !" example</title>',
	'<style type="text/css">',
	' html, body { height: 100%; padding: 0; margin: 0; }',
	' .vcenter-out, .hcenter { display: table; height: 100%; margin: auto; }',
	' .vcenter-in { display: table-cell; vertical-align: middle; }',
	'</style>',
].join('\n');

const body = [
	'<div class="vcenter-out">',
	' <div class="vcenter-in">',
	'  <fieldset>',
	'   <label>Name:</label>',
	'   <input id="input" maxlength="20" placeholder="Enter a name here"',
	'		type="text" data-xdh-onevent="input|Typing"/>',
	'   <button data-xdh-onevent="Clear">Clear</button>',
	'   <hr/>',
	'   <h1>',
	'    <span>Hello </span>',
	'    <span style="font-style: italic;" id="name"></span>',
	'    <span>!</span>',
	'   </h1>',
	'  </filedset>',
	' </div>',
	'</div>'].join('\n');

const callbacks = {
	"Connect": (dom, id) => dom.headUp(head, () => dom.setLayout("", body)),
	"Typing": (dom, id) => dom.getContent(id, (name) => dom.setContent("name", name)),
	"Clear": (dom, id) => dom.confirm("Are you sure ?",
		(answer) => { if (answer) dom.setContents({ "input": "", "name": "" }) }),
};

atlas.launch(() => new atlas.DOM(), "Connect", callbacks);
```

## What's next ?

Here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](http://q37.info/s/atk/TodoMVC)

The source code of this application and more information about the *Atlas* toolkit can be found at <http://atlastk.org/>