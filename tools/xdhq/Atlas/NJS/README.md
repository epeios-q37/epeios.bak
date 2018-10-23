# Binding for *Node.js* of the *Atlas* toolkit

![Node.js logo](https://q37.info/download/assets/Node.png "Node.js")

The [*Atlas* toolkit](https://atlastk.org/) is a library which facilitates the prototyping of web applications.

## *Hello, World!*

[![Little demonstration](https://q37.info/download/Hello.gif "A basic example")](http://q37.info/s/atk/Hello/)

```javascript
const atlas = require( 'atlastk' );

// Content of 'Head.html'.
const head = `
<title>"Hello, World !" example</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<style type="text/css">
	html, body {
		height: 100%;
		padding: 0;
		margin: 0;
	}
  
	.vcenter-out, .hcenter {
		display: table;
		height: 100%;
		margin: auto;
	}

	.vcenter-in {
		display: table-cell;
		vertical-align: middle;
	}
</style>
`;

// Content of 'Main.html'.
const body = `
<div class ="vcenter-out">
 <div class ="vcenter-in">
  <fieldset>
   <label>Name: </label>
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
`;

const callbacks = {
 "Connect": (dom, id) => dom.setLayout("", body,
  () => dom.focus("input")),
 "Typing": (dom, id) => dom.getContent(id,
  (name) => dom.setContent("name", name)),
 "Clear": (dom, id) => dom.confirm("Are you sure ?",
  (answer) => { if (answer) dom.setContents({ "input": "", "name": "" }) }),
};

atlas.launch(() => new atlas.DOM(), "Connect", callbacks, head);
```

- Launch `npm install atlastk`,
- in the same directory, create a `Hello.js` file with above source code, or get it directly here: <https://q37.info/download/assets/Hello.js>,
- launch `node Hello.js`,

or go to <https://npm.runkit.com/atlastk>.

## What's next ?

Here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](http://q37.info/s/atk/TodoMVC)

The source code of this application and more information about the *Atlas* toolkit can be found at <http://atlastk.org/>.