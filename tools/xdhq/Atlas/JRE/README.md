# *Java* binding of the *Atlas* toolkit

The *Atlas* toolkit is a library for web and desktop UI. This is the *Java* binding of this toolkit.


## *Hello World!*

![Little demonstration](http://q37.info/download/Hello.gif "A basic example")

### *Java* source code

```Java
import info.q37.atlas.*;

class Hello extends Atlas {
  public void handle( DOM dom, String action, String id )
  {
    switch( action) {
    case "Connect":
      dom.setLayout("", new Tree(), "Main.xsl");
      break;
    case "Typing":
      dom.setContent("name", dom.getContent(id));
      break;
    case "Clear":
      if ( dom.confirm( "Are you sure ?" ) ) dom.setContents( new String[][] { { "input", ""}, { "name", ""} } );
      break;
    }
  }

  public static void main(String[] args) throws Exception {
    launch("Connect", info.q37.atlas.Atlas.Type.DEFAULT);

    for (;;) new Hello();
  }
}
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

For more information: <http://atlastk.org/>.