# Binding for *Node.js* of the *Atlas* toolkit

The *Atlas* toolkit is a library for web and desktop UI. This is the binding of this toolkit for *Node.js*.


## *Hello World!*

![Little demonstration](http://q37.info/download/Hello.gif "A basic example")

### *JavaScript* source code

```JavaScript
const atlas = require('atlastk');

atlas.register( {
 "Connect": (dom, id) => dom.setLayout("", new atlas.Tree(), "Main.xsl"),
 "Typing": (dom, id) => dom.getContent(id, (name) => dom.setContent("name", name)),
 "Clear": (dom, id) => dom.confirm("Are you sure ?", (answer) => { if (answer) dom.setContents({"input": "", "name": ""})}),
 } );

atlas.launch(() => new atlas.DOM(), "Connect");
```


### *XSL* file

This is the content of the `Main.xsl` file which name is given given as parameter to above `dom.setLayout(...)` instruction. 

```XML
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" encoding="UTF-8"/>
  <xsl:template match="/">
    <label>Name:</label>
    <input id="input" maxlength="20" placeholder="Enter a name here" type="text" data-xdh-onevent="input|Typing"/>
    <button data-xdh-onevent="Clear">Clear</button>
    <hr/>
    <h1>
      <span>Hello </span>
      <span style="font-style: italic;" id="name"/>
      <span>!</span>
    </h1>
  </xsl:template>
</xsl:stylesheet>
```

## What's next ?

For more information: <http://atlastk.org/>