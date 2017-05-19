# xppq-java

## Presentation

The *Epeios* XML [preprocessor](http://q37.info/s/t/xppq/) and parser as [*JRE*](http://en.wikipedia.org/wiki/Java_virtual_machine) (*Java*) package.

## Status
*GNU/Linux* & *OS X* : [![Travis CI](https://travis-ci.org/epeios-q37/xppq-java.png)](https://travis-ci.org/epeios-q37/xppq-java)
 
*Windows* : [![AppVeyor](http://ci.appveyor.com/api/projects/status/github/epeios-q37/xppq-java)](http://ci.appveyor.com/project/epeios-q37/xppq-java)

## Compilation

- Retrieve the repository,
- under *GNU/Linux* or *OS X* : from the root of the repository, launch `make`,
- under *Windows* : open the `xppqjre.vcxproj` with *Microsoft Visual Studio*, and compile.

You may have to set the `JAVA_HOME` environment variable to the root of the desired *JDK*. Under *Linux*, the *JDK* should be at `/usr/lib/jvm/default-java/`.

## Testing

From the root of the repository:
- under *GNU/Linux* or *OS X*: `java -Djava.library.path=. XPPQDemo`,
- under *Windows*: `java -Djava.library.path=x64/Release XPPQDemo` (if needed, replace `x64/Release`with the path where the *DLL* is located).