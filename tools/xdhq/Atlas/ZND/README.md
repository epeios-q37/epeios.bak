# *PHP* binding of the *Atlas* toolkit

The *Atlas* toolkit is a library for web and desktop UI. This is the *PHP* binding of this toolkit.


## *Hello World!*

![Little demonstration](http://q37.info/download/Hello.gif "A basic example")

### *PHP* source code

```PHP
<?php
require( "Atlas.php");

Atlas::launch( "Connect", null );

$dom = new AtlasDOM();

while ( true ) {
  switch( $dom->getAction($id) ) {
  case "Connect":
    $dom->setLayout("", new AtlasTree(), "Main.xsl");
    break;
  case "Typing":
    $dom->setContent("name", $dom->getContent($id));
    break;
  case "Clear":
    if ( $dom->confirm("Are you sure?") ) $dom->setContents(["input" => "", "name" => ""]);
    break;
  }
}
?>
```

### *XSL* file

This is the content of the `Main.xsl` file which name is given given as parameter to above `$dom.setLayout(...)` instruction. 

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

For more information: <http://atlastk.org/>.