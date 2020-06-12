# Chat room in *Java* with the *Atlas* toolkit

![Java logo](https://q37.info/download/assets/Java.png "Java")

A *Java* version of a basic chat room application, made with the [*Atlas* toolkit](https://atlastk.org/).

Online demonstration: <http://q37.info/runkit/Chatroom>.

To install and run this application on your computer:

- get this repository (with *git*: `git clone https://github.com/epeios-q37/chatroom-java`),
- put, inside of the retrieved repository, the `Atlas.jar` file: https://q37.info/download/assets/Atlas.jar,
- compile the source file (`javac -cp Atlas.jar main.java`),
- launch:
	- under *Windows*: `java -cp .;Atlas.jar Chatroom` (with a semi-colon as separator for the *classpath*),
	- under other platforms: `java -cp .:Atlas.jar Chatroom` (with a colon as separator for the *classpath*).

*NOTA*: you may have to give the full path for the *Java* compiler (`javac`), and may be for the *JVM* (`java`), if they are not in the [*PATH*](https://en.wikipedia.org/wiki/PATH_(variable)).


There is also a version for *Node.js*: <http://github.com/epeios-q37/chatroom-node>.