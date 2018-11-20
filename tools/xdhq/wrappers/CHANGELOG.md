# *CHANGELOG* for the *XDHTML* wrappers (*all bindings*).

## 2018-11-20

- sending some information for logging purpose,
- switching to protocol version 1,

## 2018-07-12
- *xdhqnjs*:
  - no more `Tree` object, but a 'createTree()` function returning an object from a *xmlbuilder* module,

## 2018-04-14

- *xdhqnjs*:
  - adaptation to changes in underlying modules,

## 2018-04-06

- *xdhqnjs*:
  - using `ProcessBuilder` to launch *Electron* and *httpd*, as `Runtime.exec(...)` seems not to work under *macOS*,

## 2018-04-04
- *xdhqnjs*:
 - some modifications to be compatible with *Java* 1.6,

## 2018-02-27

- *xdhqnjs*:
  - *CTRL-C* now kills the application silently,

## 2018-02-23

- all:
  - desktop interface (*Electron*) can now be launched, instead of the default web one, by modifying the application source,

## 2018-02-19

- *xdhqznd*:
  - adaptation to changes in underlying libraries,

## 2018-02-16

- *common*:
  - handles now the action to launch on a new session; this action is launched when the action parameter on a query is empty or absent,

## 2018-02-14

- *xdhqnjs*:
  - changing *NPM* package name from *xdhq* to *xdhqnjs* due to coming *Java* and *PHP* version,

## 2018-01-14

- *xdhqnjs*:
  - all the files served by the *httpd* server are automatically now relative to the directory containing the main *JS* script; it's no more required to give this directory as parameter of the `launch(...)` method,

## 2018-01-13

- *xdhqnjs*:
  - replacing the *cast* system handling with a system based on *CSS* styles and classes,
  - adding functions to (en|dis)able elements (including styles).
