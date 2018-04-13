# Notes about the *PHP* version of *UnJSq*/*XDHq*

## Running

### Preamble

*UnJSq* (`UnJSq/ZND/UnJSq.php`) launches a web server through *Node.js*. If required, it also launches the desktop version through *Electron*.

The path to *php* executable have to be in the `PATH` environment variable.

### During development

When the web server is launched directly by the application, killing the application does not kill the web server. You should be able to relaunch the application; it fails to relaunch the web server, but this should not be a problem, as the previous launched server still runs. But, inexplicably, the port of the *xdhq* daemon is still in use, despite the application which bind to this port was killed. You have to kill the *Node.js* server to be able to launch the application again.

To avoid to have to kill the *Node.js* web server, in addition of the application, each time you want to relaunch the application, launch manually the *Node.js* web server by launching `node httpd.js <dir>` at `.../xdhq/examples/common/` `<dir>`must be the absolute path. Example : `node httpd.js h:/hg/epeios/tools/xdhq/examples/TodoMVC`.

### *Cygwin*

*Cygwin* doesn't properly send the *CTRL-C* to a windows application. Currently, this seems not e problematic, but it was once.