# Developers/maintainers notes about the *Atlas* toolkit binding for *Node.js*

## New packages

Currently, the new packaging is being developed for the *Node.js* binding. It will later be developed for the other bindings.

As the core of the *Atlas* toolkit is written *C/C++*, and the package manager for the different language (*NPM* for *Node.js*, *Composer* for *PHP*...) aren't designed to deploy native code, the installation process is somewhat cumbersome.

To allow those who are interested to fiddle with the *Atlas* toolkit, all the native component are on a server, and only components written in the language of the binding, an d hence easily installable with the package manager for this language, is locally installed.

This default installation can also be used for prototyping, for demonstrating a application to remote client, to ameliorate an application. The core of the application is executed locally, so no need to upload the source code to a remote server.

This default installation is not fitted for production, for which a full installation will be preferred. The desktop version also need the full installation.

To the currently available value for the command-line argument (`web`, `desktop`, `none` and corresponding shortcuts `w`, `d`, `n` and `wd` for both) can be prefixed with `P` to launch explicitly the *PROD* (production mode, which needs the full installation) or with `D` for the *DEMO* (demonstration) mode.

The developer's default mode id *PROD*, for all the others, it's *DEMO*.

## Current/old packages

Dependencies :

* *njsq*, **à packager en premier !!!**:
  * *wrapper* *Node.js*,
  * `njsq/`
  * <http://github.com/epeios-q37/njsq/>,
  * [![NPM](https://nodei.co/npm/njsq.png)](https://nodei.co/npm/njsq/).

* *xdhqxdh*:
  * *proxy*,
  * `xdhq/proxy/`,
  * <http://github.com/epeios-q37/xdhq/>,
  * [![NPM](https://nodei.co/npm/xdhqxdh.png)](https://nodei.co/npm/xdhqxdh/).

* *xdhqnjs*:
  * *wrapper* *XDHTML*,
  * `xdhq/wrapper/NJS/`
  * <http://github.com/epeios-q37/xdhq-node/>,
  * [![NPM](https://nodei.co/npm/xdhqnjs.png)](https://nodei.co/npm/xdhqnjs/).

* *xdhwebqnjs*:
  * *frontend* *web* *XDHTML*,
  * `xdhwebq/NJS`,
  * <https://github.com/epeios-q37/xdhwebq-node>
  * [![NPM](https://nodei.co/npm/xdhwebqnjs.png)](https://nodei.co/npm/xdhwebqnjs/).

* *xdhelcq*:
  * *frontend* *desktop* *XDHTML*,
  * `xdhwelcq`,
  * <https://github.com/epeios-q37/xdhelcq>
  * [![NPM](https://nodei.co/npm/xdhelcq.png)](https://nodei.co/npm/xdhelcq/).

* *atlasnjs*:
  * *wrapper* *XDHTML* + serveur *httpd* + *Electron*,
  * `xdhq/Atlas/NJS/`
  * <http://github.com/epeios-q37/atlas-node/>,
  * [![NPM](https://nodei.co/npm/atlastk.png)](https://nodei.co/npm/atlastk/).

*NOTA*:

* Following the installation of all *Atlas* toolkit bindings through *NPM*, the *xdhq* package was renamed to *xdhqnjs*,
* Because `require('atlas-node')` was to clumsy, the `atlas-node` package was renamed to `atlastk`,
* `xdhwebqnjs`was first `xdhwebq`.