# *Java* version of the *Atlas* toolkit

![For Java](https://q37.info/download/assets/Java.png "Java logo")

[![Run on Repl.it](https://repl.it/badge/github/epeios-q37/atlas-java)](https://q37.info/s/3vwk3h3n) [![Version 0.11.0](https://img.shields.io/static/v1.svg?&color=90b4ed&label=Version&message=0.11.0)](http://q37.info/s/gei0veus)

*NOTA*: this toolkit is also available for:
- *Node.js*: <http://github.com/epeios-q37/atlas-node>,
- *Perl*: <http://github.com/epeios-q37/atlas-perl>,
- *Python*: <http://github.com/epeios-q37/atlas-python>,
- *Ruby*: <http://github.com/epeios-q37/atlas-ruby>.

---

With the *Atlas* toolkit, it has never been easier to write a modern web application ([*Single-page application*](https://q37.info/s/7sbmxd3j)):
- no *Javascript* to write: only *HTML* and *Java*,
- no [web server](https://q37.info/s/n3hpwsht) (*Apache*, *Nginx*…) to install: only the toolkit,
- no remote server to host your application: only your computer;
- no port to open on your internet box: a simple internet connection is enough for your application to be accessible from the entire internet,

and all this with only a library of about 30 Kb.

The Atlas toolkit is also the fastest and easiest way to add a [graphical user interface](https://q37.info/s/hw9n3pjs) to all your programs.

## Live demonstrations

Before diving into an example of source code, you can take a look on some live demonstrations, like this *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application:

![TodoMVC](https://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")

To see all the live demonstrations, simply go [here](https://q37.info/s/3vwk3h3n), click on the green `run` button, select the demonstration you want to see, and then click (or scan with your smartphone) the then displayed [QR code](https://q37.info/s/3pktvrj7).


## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program made with the *Atlas* toolkit looks like:

![Little demonstration](https://q37.info/download/assets/Hello.gif "A basic example")

- `git clone http://github.com/epeios-q37/atlas-java`
- `cd atlas-java/Hello`
- on *Windows*: `java -cp .;../Atlas.jar Hello`
- on other platforms: `java -cp .:../Atlas.jar Hello`

Source code:

```Java
import info.q37.atlas.*;

class Hello extends Atlas {
 // For the sake of simplification, the following HTML content has been
 // embedded in the source code, but it can be easily stored in a file. 
 private static String body =
  "<div style=\"display: table; margin: 50px auto auto auto;\">" +
  " <fieldset>" +
  "   <input id=\"input\" maxlength=\"20\" placeholder=\"Enter a name here\"'" +
  "          type=\"text\" data-xdh-onevent=\"Submit\"/>" +
  "   <div style=\"display: flex; justify-content: space-around; margin: 5px auto auto auto;\">" +
  "    <button data-xdh-onevent=\"Submit\">Submit</button>" +
  "    <button data-xdh-onevent=\"Clear\">Clear</button>" +
  "  </div>" +
  " </fieldset>" +
  "</div>";

 @Override
 public void handle(String action, String id)
 {
  switch(action) {
  case "": // Action label corresponding to a new session.
   dom.setLayout("", body);
   break;
  case "Submit":
   dom.alert("Hello, " + dom.getContent("input") + "!" );
   break;
  case "Clear":
   if ( dom.confirm("Are you sure ?") )
    dom.setContent("input", "");
   break;
  }

  dom.focus("input");
 }

 public static void main(String[] args
 {
  launch(() -> new Hello());
 }
}
```


## Content of the repository

The `Atlas` directory contains the *Java* source code of the *Atlas* toolkit, which is not needed to run the examples.

`Atlas.jar` is the file you have to reference in the [*classpath*](https://en.wikipedia.org/wiki/Classpath_(Java)) in order to use the *Atlas* toolkit in your own program.

All other directories are examples.

To run an example, go inside its directory (`Blank`, `Chatroom`…) and launch:

- under *Windows* : `java -cp .;../Atlas.jar <Name>` (with semi-colon as *classpath* separator),
- under other platforms : `java -cp .:../Atlas.jar <Name>` (with colon as *classpath* separator).

where `<Name>` is the name of the example (`Blank`, `Chatroom`…).

For more information about the *Atlas* toolkit: <http://atlastk.org/>.