# Notes about *XDHELCq*

## Running

*Electron* path is `node_modules/.bin/electron`.

The module has to be launched as value of the `--module|-m` flag.

The module must be given <u>with</u> path, or the locale/configuration files will not be found.

As last argument, the subdirectory which contains all the file red directly by *Electron*.

Example to launch the *Atlas* frontend for *TodoMVC* : `nodes_module/.bin/electron . -m=h:/bin/xdhqxdh TodoMVC`.

## Installation

Install, **in this directory, without the `-g` option**, the *NPM* packages *electron* and _electron-rebuild_ (`npm install electron electron-rebuild`).

## Compilation 

Launch `DVLGenGYP`. `electron` (and also `electron-rebuild`) will be in `nodes_modules/.bin/`. Although `DVLGenGYP` builds the wrapper, rebuild it with the IDE from *Visual Studio* in order for the binary to be deployed correctly.

`node_module/.bin/electron-rebuild` is an alternative way to build the addon, but `DVLGenGYP`ìs required to create the proper `.sln`, and also th rebuilding with *Visual Studio*.

Resources:

* <https://electron.atom.io/docs/tutorial/using-native-node-modules/>
* <https://github.com/juliangruber/require-rebuild>

## Caveats:

If the build fails, retry after deleting `.electron`, `.electron-gyp` and `.node-gyp` in `C:/Users/csimon/`.