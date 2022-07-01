## Content of the repository

The `atlastk` directory contains the *Java* source code of the *Atlas* toolkit.

`atlastk.jar` is the file you have to reference in the [*classpath*](https://en.wikipedia.org/wiki/Classpath_(Java)) in order to use the *Atlas* toolkit in your own program.

The `examples` directory contains some examples.

To run an example, go inside its directory (`Blank`, `Chatroom`…) and launch:

- under *Windows*: `java -cp .;../../atlastk.jar <Name>` (with semi-colon as *classpath* separator),
- under other platforms: `java -cp .:../../atlastk.jar <Name>` (with colon as *classpath* separator).

where `<Name>` is the name of the example (`Blank`, `Chatroom`…).