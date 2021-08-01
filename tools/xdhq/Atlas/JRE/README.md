<!-- NO MORE IN USE !!!-->

# *Java* version of the *Atlas* toolkit

***<ins>NO MORE IN USE!!!</ins>***

![For Java](https://q37.info/download/assets/Java.png "Java logo")

[![Run on Repl.it](https://repl.it/badge/github/epeios-q37/atlas-java)](https://q37.info/s/3vwk3h3n)
[![Version 0.11](https://img.shields.io/static/v1.svg?&color=90b4ed&label=Version&message=0.11)](http://github.com/epeios-q37/atlas-java/)
[![Stars](https://img.shields.io/github/stars/epeios-q37/atlas-java.svg?style=social)](https://github.com/epeios-q37/atlas-java/stargazers)
[![license: MIT](https://img.shields.io/github/license/epeios-q37/atlas-java?color=yellow)](https://github.com/epeios-q37/atlas-java/blob/master/LICENSE)
[![Homepage](https://img.shields.io/static/v1?label=homepage&message=atlastk.org&color=ff69b4)](https://atlastk.org)

*NOTA*: this toolkit is also available for:
- *Node.js*: <http://github.com/epeios-q37/atlas-node>,
- *Perl*: <http://github.com/epeios-q37/atlas-perl>,
- *Python*: <http://github.com/epeios-q37/atlas-python>,
- *Ruby*: <http://github.com/epeios-q37/atlas-ruby>.

---

## *Hello, World!*

### Source code

```Java
import info.q37.atlas.*;

class Hello extends Atlas {

 private static String body =
  "<div style=\"display: table; margin: 50px auto auto auto;\">" +
  " <fieldset>" +
  "   <input id=\"input\" maxlength=\"20\" placeholder=\"Enter a name here\"'" +
  "          type=\"text\" xdh:onevent=\"Submit\"/>" +
  "   <div style=\"display: flex; justify-content: space-around; margin: 5px auto auto auto;\">" +
  "    <button xdh:onevent=\"Submit\">Submit</button>" +
  "    <button xdh:onevent=\"Clear\">Clear</button>" +
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

 public static void main(String[] args)
 {
  launch(() -> new Hello());
 }
}
```

[![Little demonstration](https://q37.info/download/assets/Hello.gif "A basic example")](https://q37.info/s/3vwk3h3n)

### Try it yourself now

See the below online demonstrations section, or launch:

- `git clone http://github.com/epeios-q37/atlas-python`
- `python atlas-python/examples/Hello/`

## Online demonstrations

Thanks to [Replit](https://q37.info/s/mxmgq3qm), an [online IDE](https://q37.info/s/zzkzbdw7), you can write and run programs using the *Atlas* toolkit directly in your web browser, without having to install *Python* on your computer.

To see some examples, like the following [*TodoMVC*](http://todomvc.com/) application or the above [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program, simply go [here](https://q37.info/s/vwpsw73v), click on the green `run` button, select the demonstration you want to see, and then click (or scan with your smartphone) the then displayed [QR code](https://q37.info/s/3pktvrj7).

[![TodoMVC](https://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")](https://q37.info/s/3vwk3h3n)

## What do you want to do today?

If you want to:

- take your code to the next level, from [CLI](https://q37.info/s/cnh9nrw9) to [GUI](https://q37.info/s/hw9n3pjs),
- teach your students to program a GUI, 
- impress your teacher with a blowing GUI,
- easily share your programs with all you family and friends,

then you found the right toolkit.

With the [*Atlas* toolkit](http://atlastk.org/), writing modern web applications ([*SPA*](https://q37.info/s/7sbmxd3j)) has never been this easy:
- no *Javascript* to write; only *HTML* and *Python*,
- no [front and back end architecture](https://q37.info/s/px7hhztd) to bother with,
- no [web server](https://q37.info/s/n3hpwsht) (*Apache*, *Nginx*…) to install,
- no need to deploy your application on a remote server,
- no incoming port to open on your internet box.

The *Atlas* toolkit is written in pure *Python* (compatible with version 2 and 3), with no native code and no dependencies, allowing the *Atlas* toolkit to be used on all environments where *Python* is available. 

Simply by running them on a local computer with a simple internet connexion, applications using the *Atlas* toolkit will be accessible from the entire internet on laptops, smartphones, tablets…




---
---

With the [*Atlas* toolkit](http://atlastk.org/), it has never been easier to create your own modern web application ([*SPA*](https://q37.info/s/7sbmxd3j)):
- no *Javascript* to write; only *HTML* and *Java*,
- no [front and back end architecture](https://q37.info/s/px7hhztd) to bother with,
- no [web server](https://q37.info/s/n3hpwsht) (*Apache*, *Nginx*…) to install,

and all this only with the help of a library of about 30 KB.

With the *Atlas* toolkit, your applications will be accessible from the entire internet on laptops, smartphones, tablets…, and this without having to deploy them on a remote server or to open an incoming port on your internet box. All you need is a local computer with a simple internet connection. 

The *Atlas* toolkit is also the fastest and easiest way to add a [graphical user interface](https://q37.info/s/hw9n3pjs) to all your programs.

## Live demonstrations

Before diving into source code, you can have a look on some live demonstrations, like this *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application:

![TodoMVC](https://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")

To see all the live demonstrations, simply go [here](https://q37.info/s/3vwk3h3n), click on the green `run` button, select the demonstration you want to see, and then click (or scan with your smartphone) the then displayed [QR code](https://q37.info/s/3pktvrj7).


## *Hello, World!*

Here's how the [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program made with the *Atlas* toolkit looks like:

![Little demonstration](https://q37.info/download/assets/Hello.gif "A basic example")

This example is part of the live demonstrations above, but you can launch it on your computer:

- `git clone https://github.com/epeios-q37/atlas-java`
- `cd atlas-java/Hello`
- on *Windows*: `java -cp .;../Atlas.jar Hello`
- on other platforms: `java -cp .:../Atlas.jar Hello`

Source code:




## Content of the repository

The `Atlas` directory contains the *Java* source code of the *Atlas* toolkit, which is not needed to run the examples.

`Atlas.jar` is the file you have to reference in the [*classpath*](https://en.wikipedia.org/wiki/Classpath_(Java)) in order to use the *Atlas* toolkit in your own program.

All other directories are examples.

To run an example, go inside its directory (`Blank`, `Chatroom`…) and launch:

- under *Windows* : `java -cp .;../Atlas.jar <Name>` (with semi-colon as *classpath* separator),
- under other platforms : `java -cp .:../Atlas.jar <Name>` (with colon as *classpath* separator).

where `<Name>` is the name of the example (`Blank`, `Chatroom`…).