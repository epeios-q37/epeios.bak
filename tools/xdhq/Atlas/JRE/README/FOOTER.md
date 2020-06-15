## Content of the repository

The `Atlas` directory contains the *Java* source code of the *Atlas* toolkit, which is not needed to run the examples.

`Atlas.jar` is the file you have to reference in the [*classpath*](https://en.wikipedia.org/wiki/Classpath_(Java)) in order to use the *Atlas* toolkit in your own program.

All other directories are examples.

To run an example, go inside its directory (`Blank`, `Chatroom`…) and launch:

- under *Windows* : `java -cp .;../Atlas.jar <Name>` (with semi-colon as *classpath* separator),
- under other platforms : `java -cp .:../Atlas.jar <Name>` (with colon as *classpath* separator).

where `<Name>` is the name of the example (`Blank`, `Chatroom`…).