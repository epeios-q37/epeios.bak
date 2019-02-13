# *Epeios* TODO

FAST:

- Study the possibility to adapt the framework to 'emscripten'.

MIDDLE:

- CSDL...:
	- Reimplementation ; has no more to do with embedded library.
- ERR:
	Handling of `qRReturn` when an error is already in progress.
- Flow:
	- Reimplementing flow handling to be based on driver rather then on flow.
	- Implement the possibility to read/write in a flow without blocking (even without reading/writing a minimal amount of data).
- LOG:
	- Reimplementation of log handling to put is as part of 'SCLMISC' (depends if it's a file log or a console log).
- N4ALLW:
  - Sometimes, the path to the component can be `~/bin/` (*PHP*, *Java*), and sometime it must be `/home/csimon/bin/` (*Node.js*).
- SCL...:
	- Renaming ('SCLxyyy...' to 'SCLx').
	- Implementing better thread-safety for the registry, in peculiar the common registry as defined in 'SCLRGSTRY'/'SCLMISC'.
- SCLTOOL:
	- implementation of the possibility to display a free text defined in the configuration file for '--help' command.
	- implementation of the displaying of options which are common to all commands, to avoid to put them in each command definition
	  ('--verbose' in 'mmuaq', for example, or the '-s|--setup' option).
- STR:
	- Implementing the number conversion in another library based on a flow as input.
- UVQ:
	- Launch each 'work callback' in a 'mkt' thread, so that only one 'uv' thread will be used (only 'UV_THREADPOOL_SIZE' threads are available with 'uv').
- XDHTML:
	- Implementing data filling in a second pass through a 'data-xdh' attribute (like event and cast handling); should fix Mozilla issue.
	- Implement a solution which prevent that an 'Enter' (or other touch) event on a text box is also applied to a potential alert box reporting an error,
	  preventing this box to be displayed. Currently, there is a workaround in the 'handleEvent' function from the 'xdhtml.js' file.
	- Expose in the XML flow the nature of the interface (desktop, web, touchscreen, mobile...).
- DTFPTB: optimize string handling.


LONG:

- XPP:
	- Reimplementation in a simplified manner.
- Socket:
	- Adaptation of the network related libraries to IPv6.

DISMISSED :

- 2016-12-31 : 'XULRunner' is no longer maintained !
  - Implementing 'XDHTML' technology under 'Gecko' (without 'XUL' ; only 'HTML 5'). There are more platforms for 'Gecko' than for 'CEF', as,
    for 'Gecko', if there is no 'XULRunner', there will be 'Firefox' (check if the '--app' option is always available for 'Firefox').
    Furthermore, things as a file explorer are available under 'Gecko' and needs no to be reimplelmented as with 'CEF'.



--------------------------------------------------------------------

IN PROGRESS:

SUSPENDED:

- XDHTML:
	- Implementation for Android.
- Flow:
	- Implementing flow variations (TXF, XTF, FLX libraries) to take a driver, not a flow ; the needed flow will be instantiated internally.

	
MADE:

2016-09-24:
- Adding detection of 'crate' using as list without being a list (use of 'CRT' instead of 'LSTCRT' ; accessing a 'CRT' item with 'qNIL' row).

2016-09-02:
- Fixing errors occurring when compiling using WSL (Widows Subsystem for Linux).

2016-08-22:
- Fixing errors which occurs in backends on regular disconnection from front-end.

