# *Java* binding of the *Atlas* toolkit

The *Atlas* toolkit is a library for web and desktop UI. This is the *Java* binding of this toolkit.

## <u>Important</u>

Le séparateur de l'argument `-cp`  ou `-classpath` ainsi que du contenu la variable d'environnement `CLASSPATH`est `;` **sous Windows** (nécessitant d'entourer le paramètre de `-cp` ou `--classpath` par des quotes sous *Cygwin*).
Pour les autres *OSes*, cela semble être `:` (double-point).


## *Hello World!*

[![Little demonstration](http://q37.info/download/Hello.gif "A basic example")](http://q37.info/s/atk/Hello/)

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
    launch("Connect");

    for (;;) new Hello();
  }
}
```

### *XSL* file

This is the content of the `Main.xsl` file which name is given as parameter to above `dom.setLayout(...)` instruction. 

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

Here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](http://q37.info/s/atk/TodoMVC)

The source code of this application and more information about the *Atlas* toolkit can be found at <http://atlastk.org/>