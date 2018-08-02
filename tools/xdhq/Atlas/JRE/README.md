# *Java* binding of the *Atlas* toolkit

The [*Atlas* toolkit](https://atlastk.org/) is a library for web and desktop UI. This is the *Java* binding of this toolkit.

## *Hello World!*

![Little demonstration](http://q37.info/download/assets/Hello.gif "A basic example")

```Java
import info.q37.atlas.*;

class Hello extends Atlas {
	public void handle( DOM dom, String action, String id )
	{
		String head = 
		"<title>\"Hello, World !\" example</title>" +
		"<style type=\"text/css\">" +
		" html, body { height: 100%; padding: 0; margin: 0; }" +
		" .vcenter-out, .hcenter { display: table; height: 100%; margin: auto; }" +
		" .vcenter-in { display: table-cell; vertical-align: middle; }" +
		"</style>";
	
		String html = 
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
			dom.headUp( head );
			dom.setLayout("", html);
		} else if ("Typing".equals( action ) ) {
			dom.setContent("name", dom.getContent(id));
		} else if ( "Clear".equals( action ) ) {
			if ( dom.confirm( "Are you sure ?" ) )
				dom.setContents( new String[][] { { "input", ""}, { "name", ""} } );
		} else {
			throw new RuntimeException( "Unknown action '" + action + "' !!!");
		}
	}
	public static void main(String[] args) throws Exception {
		launch("Connect" );

		for (;;)
			new Hello();
	}
}
```

- Get the `Atlas.jar` file (https://q37.info/download/assets/Atlas.jar),
- in the same directory, create a `Hello.java` file with above source code, or get it directly here : <https://q37.info/download/assets/Hello.java>,
- compile this file : `javac -cp Atlas.jar Hello.java`,
- launch :
  - under *Windows* : `java -cp .;Atlas.jar Hello` (with semi-colon as *classpath* separator),
  - under other platforms : `java -cp .:Atlas.jar Hello` (with colon as *classpath* separator).

## What's next ?

Here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like: 

[![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](https://github.com/epeios-q37/todomvc-java)

See <https://github.com/epeios-q37/todomvc-java>.