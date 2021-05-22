# Notes about *XDHELCq*

## Running

### From command-line

*Electron* path is `node_modules/.bin/electron`.

The module has to be launched as value of the `--module|-m` flag.

The module must be given <u>with</u> path, or the locale/configuration files will not be found.

The second argument (third with the `-m|--module` option) must be `index.js`. If not, it's `XDHElcQ.js` which will be used, as it is the one defined as the`main` entry in the `package.json` file, for `require('xdehelcq')` to be working properly.

As last argument, the subdirectory which contains all the file red directly by *Electron*.

Example to launch the *Atlas* frontend for *TodoMVC* : `node_modules/.bin/electron index.js -m=h:/bin/xdhqxdh TodoMVC`.

### En utilisant le *debugger* *Code::Blocks*

- Dans *Host application:* : `/media/csimon/H/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron` ;
- dans *Program arguments:* : `/home/csimon/epeios/tools/xdhelcq/index.js -m=/home/csimon/bin/libesketchxdh.so /home/csimon/epeios/apps/esketch/frontend/XDHTML/ `.

## Installation

Install, **in this directory, without the `-g` option**, the *NPM* packages *electron* and _electron-rebuild_ (`npm install electron electron-rebuild`).

## Compilation

Launch `DVLGenGYP`. `electron` (and also `electron-rebuild`) will be in `nodes_modules/.bin/`. Although `DVLGenGYP` builds the wrapper, rebuild it with the IDE from *Visual Studio* in order for the binary to be deployed correctly.

`node_module/.bin/electron-rebuild` is an alternative way to build the addon, but `DVLGenGYP` ìs required to create the proper `.sln`, and also to rebuilding with *Visual Studio*.

The `build` can be safely deleted ; it will be rebuild (no pun intended). This may solve some problems.

Resources:

* <https://electron.atom.io/docs/tutorial/using-native-node-modules/>
* <https://github.com/juliangruber/require-rebuild>

## Caveats:

**If you have the message `…/xdhelcq.node is not a valid Win32 application.` is displayed the arguments, this may only mean that the arguments are incorrect !!!**

If the build fails, retry after deleting `.electron`, `.electron-gyp` and `.node-gyp` in `C:/Users/csimon/`.
