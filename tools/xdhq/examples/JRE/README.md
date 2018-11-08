# Examples of use of the *Java* version of the *Atlas* toolkit

![Java logo](https://q37.info/download/assets/Java.png "Java")

The *blank* application does almost nothing, and is intended as a basis for your own applications. The *notes* application is a more complex example which shows how to use widgets like [*CKEditor*](http://ckeditor.com/) with the *Atlas* toolkit.

Online demonstration:

  - of the *blank* application: <http://q37.info/runkit/Blank>,
  - of the *notes* application: <http://q37.info/runkit/Notes>.

To install and run these applications on your computer:

- get this repository (with *git*: `git clone https://github.com/epeios-q37/atlas-java-demo`),
- put, inside of the retrieved repository, the `Atlas.jar` file: https://q37.info/download/assets/Atlas.jar,
- for the *blank* demonstration:
  - go to the `blank` directory,
  - compile the source file (`javac -cp ../Atlas.jar main.java`),
  - launch:
	  - under *Windows*: `java -cp .;../Atlas.jar Blank` (with a semi-colon as separator for the *classpath*),
	  - under other platforms: `java -cp .:../Atlas.jar Blank` (with a colon as separator for the *classpath*).
- for the *notes* demonstration:
  - go to the `notes` directory,
  - compile the source file (`javac -cp ../Atlas.jar main.java`),
  - launch:
	  - under *Windows*: `java -cp .;../Atlas.jar Notes` (with a semi-colon as separator for the *classpath*),
	  - under other platforms: `java -cp .:../Atlas.jar Notes` (with a colon as separator for the *classpath*).

*NOTA*: you may have to give the full path for the *Java* compiler (`javac`), and may be for the *JVM* (`java`), if they are not in the [*PATH*](https://en.wikipedia.org/wiki/PATH_(variable)).

There is also a version for:
  * *Node.js* : <http://github.com/epeios-q37/atlas-node-demo/>,
  * *PHP* : <http://github.com/epeios-q37/atlas-php-demo/>.

For more information: <http://atlastk.org/>
