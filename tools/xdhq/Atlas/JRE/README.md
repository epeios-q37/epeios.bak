# *Java* version of the *Atlas* toolkit

![Java logo](https://q37.info/download/assets/Java.png "Java")

The [*Atlas* toolkit](https://atlastk.org/) is a library dedicated to the handling of web interfaces.

## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) type application made with the *Atlas* toolkit looks like:

![Little demonstration](http://q37.info/download/assets/Hello.gif "A basic example")

For an online demonstration: <http://q37.info/runkit/Hello>.

Source code:

```Java
import info.q37.atlas.*;
import java.util.HashMap;

class Hello extends Atlas {
	public void handle( DOM dom, String action, String id )
	{
		String body = 
		"<div class=\"vcenter-out\">" +
		" <div class=\"vcenter-in\">" +
		"  <fieldset>" +
		"   <label>Name:</label>" +
		"   <input id=\"input\" maxlength=\"20\" placeholder=\"Enter a name here\"'" +
		"	 type=\"text\" data-xdh-onevent=\"input|Typing\"/>" +
		"   <button data-xdh-onevent=\"Clear\">Clear</button>" +
		"   <hr/>" +
		"   <h1>" +
		"    <span>Hello </span>" +
		"    <span style=\"font-style: italic;\" id=\"name\"></span>" +
		"    <span>!</span>" +
		"   </h1>" +
		"  </fieldset>" +
		" </div>" +
		"</div>";

		if ( "Connect".equals( action ) ) {
			dom.setLayout("", body);
			dom.focus("input");
		} else if ("Typing".equals( action ) ) {
			dom.setContent("name", dom.getContent(id));
		} else if ( "Clear".equals( action ) ) {
			if ( dom.confirm( "Are you sure ?" ) )
				dom.setContents( new HashMap<String,String> ()
					{{ put( "input", ""); put( "name", ""); }} );
		} else {
			throw new RuntimeException( "Unknown action '" + action + "' !!!");
		}
	}
	public static void main(String[] args) throws Exception {
		String head = 
			"<title>\"Hello, World !\" example</title>" +
			"<style type=\"text/css\">" +
			" html, body { height: 100%; padding: 0; margin: 0; }" +
			" .vcenter-out, .hcenter { display: table; height: 100%; margin: auto; }" +
			" .vcenter-in { display: table-cell; vertical-align: middle; }" +
			"</style>";

		launch("Connect", head );

		for (;;)
			new Hello();
	}
}
```

## *TodoMVC*

And here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](https://github.com/epeios-q37/todomvc-java)

For an online demonstration: <http://q37.info/runkit/TodoMVC>.

## Content of the repository

The `Atlas` directory contains the *Java* source code of the *Atlas* toolkit, which is not needed to run the examples.

`Atlas.jar` is the file you have to reference in the [*classpath*](https://en.wikipedia.org/wiki/Classpath_(Java)) in order to use the *Atlas* toolkit in your own application.

All other directories are examples.

To run an example, go inside its directory (`Blank`, `Chatroom`…) and launch:
- under *Windows* : `java -cp .;../Atlas.jar <Name>` (with semi-colon as *classpath* separator),
- under other platforms : `java -cp .:../Atlas.jar <Name>` (with colon as *classpath* separator).

where `<Name>` is the name of the example (`Blank`, `Chatroom`…).

The *Atlas* toolkit is also available for:
- *Node.js*: <http://github.com/epeios-q37/atlas-node>
- *PHP*: <http://github.com/epeios-q37/atlas-php>
- *Python*: <http://github.com/epeios-q37/atlas-python>

For more information about the *Atlas* toolkit, go to <http://atlastk.org/>.