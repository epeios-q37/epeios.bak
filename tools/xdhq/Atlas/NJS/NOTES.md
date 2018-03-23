# Developers/maintainers notes about the *Atlas* toolkit

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
  * [![NPM](https://nodei.co/npm/xdhwebq.png)](https://nodei.co/npm/xdhwebq/).

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
* Because `require('atlas-node')` was to clumsy, the `atlas-node` package was renamed to `atlastk`.