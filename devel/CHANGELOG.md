# *Epeios* *CHANGELOG*

## *Shortcuts*

- [adpt]: Adaptation to changes in underlying modules;
- [stdz]: modification to comply with current standards;

## 2018-08-07
- STSFSM :
  - handling thread-safety with item rather then mutex.

## 2018-08-05
- STSFSM:
  - handling thread-safety.

## 2018-07-28

- SCLJRE:
  - introducing array of strings (`(d|w)XStrings`),

## 2018-07-23

- CSDBNS:
  - [adpt],
  - Introducing `Undefined` as service (port) value,
- CSDMXS:
  - [adpt],
- CNVFDR:
  - fixing some template issues occuring with *g++*,
- FDR:
  - [adpt],
- MTX:
  - `UndefinedHandler` -> `Undefined`,
- SCLERROR:
  - [adpt],
- SCLNJS:
  - fixing some template issues occuring with *g++*,
- THT:
  - [adpt],

## 2018-07-17

- SCLXDHTML:
  - handling DEMO mode as server,
- XDHCMN:
  - handling DEMO mode as server,
- XDHUPS:
  - handling DEMO mode as server,

## 2018-07-16

- N4NJS:
  - introducing integer type,
- SCLNJS:
  - introducing integer type,
- V8Q:
  - introducing integer type,

## 2018-07-15

- SCLNJS:
  - handling `int`parameter,
- XDHDWS:
  - adding`Proxy::Execute(...)` variant,

## 2018-07-14

- SCLXDHTML
  - introducing *Demo* mode,
- XDHCMN
  - introducing *Demo* mode,
- XDHUPS:
  - introducing *Demo* mode,

## 2018-07-04

- MSCMDD:
  - [adpt];
  - [stdz];

## 2018-07-03

- MSCMDD:
  - [stdz];
- MSCMDF:
  - [stdz];
- MSCMDM:
  - [adpt];
  - [stdz];
- MSCMLM:
  - [adpt];
  - [stdz];

## 2018-06-11

- CSDBNS:
  - [adpt];
- CSDSCB:
  - removing `IODriver` parameter to `cProcessing::[CSDSCB]PreProcess(...)` [virtual] method as the use of this parameter leads to incompatibilities between backends and the *Atlas* toolkit;
- CSDLEC:
  - [adpt];
- CSDMXS:
  - [adpt];
  - following modifications made to *CSDSCB*, reverting some modifications introduced with the *Atlas* toolkit, but which were incompatible with remote backends;
- SCLBACKND:
  - [adpt];
- SCLDAEMON:
  - [adpt];
  - removing `[SCLDAEMON]PreProcess(...)` to `cDaemon`, as it causes some issue with embedded backends;

## 2018-06-08

- *Common*:
  - fixing incomplete first request processing;
- FBLBKD:
  - some modifications which facilitates the fixing of a bug;
- SCLBACKND:
  - [adpt];
- SCLDAEMON:
  - standardization of `daemon___` (becomes `cDaemon`);
  - adding `[SCLDAEMON]PreProcess(...)` to `cDaemon`;

## 2018-05-31

- SCLPLUGIN:
 - [adpt];
- STSFSM:
  - fixing matching on shorter entry;

## 2018-05-29

- SCLXDHTML:
  - rewriting backend handling in *login* context,

## 2018-05-17

- SCLXDHTML:
  - adding some missing methods to `sProxy`,
  - the *About* and *Refesh* actions are automatically added,

## 2018-05-16

- CDGURL:
  - [adpt],
- CNVFDR:
  - improvements,
- SCLIMISC:
  - standardization,
- SCLXDHTML:
  - the handling of the *XSL* files (either name or content is sent) can now be retrieved from registry,
  - fixing element disabling bug,
  - improvements,
  - [adpt],

## 2018-05-15

- FDR:
  - `Commit()`does no more return a boolean,
  - The `Unlock` parameter of the `Dismiss(...)`/`Commit(...)` methods, when set to `false`, prevents the setting to `0` of the `Red_`/`Written_` variables, 

## 2018-05-14

- FLX:
  - adding `Init(...)` methods with a default value for the `ThreadSafety` parameter,

## 2018-05-11

- FDR:
  - adding handling of the amount of data written in the write driver,
- FLW:
  - using the count of the written data amount made in *FDR* instead of handling this internally,

## 2018-05-10

- SCLXDHTML:
  - the callbacks now receive a *Mode* parameters,

## 2018-05-03

- SCLXDHTML
  - regarding the *XSL* files, switching from old behavior (transmitting file's content) to new one (transmitting file's name),

## 2018-04-29

- CSDMXC:
  - when failing to connect to server, throws now a specific error,

## 2018-04-28

- N4ALLW:
  - reverting previous modification,
- SCLN4A:
  - reinforcing error handling,

## 2018-04-27

- N4ALLW:
  - skip registration if a registration has been already made,

## 2018-04-17

- FBLBKD:
  - [adpt],
- FBLBRQ:
  - [adpt],
- SCLI:
  - creation,
- SCLARGMNT:
  - [adpt],
- SCLBACKND:
  - [adpt],
- SCLDAEMON:
  - [adpt],
- SCLFRNTND:
  - [adpt],
- SCLJRE:
  - [adpt],
- SCLMISC:
  - moving info related stuff to new *SCLI* library,
- SCLN4A:
  - [adpt],
- SCLNJS:
  - [adpt],
- SCLTOOL:
  - [adpt],
- SCLXDHTML:
  - [adpt],
- SCLZND:
  - [adpt],
 
## 2018-04-16

- SCLFRNTND:
  - [adpt],
- SCLXDHTML:
  - [adpt],

## 2018-04-14

- SCLJRE:
  - [adpt],
- SCLNJS:
  - [adpt],

## 2018-04-13

- EPSMSC:
  - [adpt],
- N4ALL:
  - [adpt],
- N4ALLW:
  - [adpt],
- SCLMISC:
  - removing all functions and global variables related to `(Target|Product|Organization)Name`, as *PHP*, under, at least, *GNU/Linux*, loads extension using the `RTLD_GLOBAL` parameter, which causes all global variables to be shared between all dynamic libraries,
  - introducing `sInfo`,
- SCLARGMNT:
  - [adpt],
- SCLN4A:
  - [adpt],

## 2018-04-12

- EPSMSC:
  - [adpt],
- SCLMISC:
  - the `SCLMISC(Target|Product|Organization)Name` which had to be defined by the user are replaced by functions, to avoid sharing of the variables between dynamic libraries, as it seems to occur under *GNULinux*,
- SCLARGMNT:
  - [adpt],

## 2018-04-09

- SCLNJS:
  - [adpt],
- SCLZND:
  - [adpt],

## 2018-04-07

- JNIQ:
  - removing `JNIEnv *` parameter's default value, as its value change from one thread to another,
  - the `JNIEnv *` parameter becomes the first one,
  - deprecating (adding `_` to name) `GetENV(...)` function (see above),
  - *NOTE*: sharing the same `JNIEnv *` between different threads seems to work under *Windows* and *macOS*, but does not under *GNU/Linux*,
- JREBSE:
  - adaptation to changes in *JNIQ*,
- N4ALL:
  - introducing `sEnv` needed by adaptation to changes in underlying modules,
- N4ALLW:
  - [adpt],
- N4JRE:
  - [adpt],
- SCLJRE:
  - [adpt],

## 2018-03-28

- N4JRE:
  - securing padding of structure shared between two libraries,
- SCLJRE:
  - workaround of a *Visual C++ 2017* bug in *Release* configuration,
  
## 2018-02-27

- SCLMISC:
  - moving `ExitOnSignal()`from *SCLTOOL* to *SCLMISC*,
- SCLTOOL:
   - *idem*,

## 2018-02-19

- XDHUTL
  - fixing bug occurring when event label size too small,

## 2018-02-18

- *shared*:
  - **$**: reverting path parameter handling,
- SCLXDHTML:
  - adaptation to modifications in underlying modules,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJS:
  - **$**,

## 2018-02-16

- *shared*:
  - **$**: a path parameter is now coming along with *XSL* parameter, for the desktop version be able to load the file,
- SCLXDHTML:
  - adaptation to modifications in underlying modules,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJS:
  - **$**,
  - commenting out some obsolete functions,

## 2018-02-14

- CSDMXC
  - a failing server is no more blocking; however, the request has to be relaunched, an automatic reconnection has yet to be implemented.

## 2018-01-31

- SCLJRE:
  - fixing parameter mess with template variadics,

## 2018-01-27:
- V8Q:
  - fixing missing method in *Node.js* v4,

## 2018-01-26:
- NODEQ:
  - adaptation to modification in *V8Q*,
 - N4NJS
  - reporting missing callback,
- SCLNJS:
  - adaptation to modifications in *N4NJS*,
- V8Q:
  - a value and derived can now be initialized with an undefined value; useful to handle callback parameter, when none is provided by user,
  - consolidation of the emptiness of an object,

## 2018-01-25:
- MTHTMC
  - using *MTHRTN* instead of *MTHFRC*,
- THT:
  - introducing `rReadWrite`,

## 2018-01-17:
- *shared*:
  - **$**: handling array of strings,
- N4ZND:
  - **$**,
- SCLZND:
  - **$**,

## 2018-01-16:
- *shared*:
  - **$**: handling array of strings,
- JNIQ:
  - **$**,
- N4JRE:
  - **$**,
- SCLJRE:
  - **$**,

## 2018-01-15:
- *shared*:
  - **$**: adding handling of booleans,
- N4JRE:
  - **$**,
- SCLJRE:
  - **$**,

## 2018-01-12:
- *shared*:
  - **$**:
    - improving *CSS* classes and rules handling,
    - adding element disabling/enabling features,
- SCLXDHTML:
  - **$**,
- XDHCMN:
  - **$**,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJS:
  - **$**,

## 2018-01-11:
- *shared*:
  - **$**: replacing *cast* related operations handling by operations on *CSS* related classes and rules (continued),
- SCLXDHTML:
  - **$**,
- XDHCMN:
  - **$**,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJR:
  - **$**,
- XDHUJS:
  - **$**,

## 2018-01-10:
- *shared*:
  - **$**: replacing *cast* related operations handling by operations on *CSS* related classes and rules,
- SCLXDHTML:
  - **$**,
- XDHCMN:
  - **$**,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJR:
  - **$**,
- XDHUJS:
  - **$**,
- XDHUTL:
  - fixing *Clang* warning,

## 2018-01-09:
- *shared*:
  - **$**: getting rid of faulty `type &var = *(type *)NULL` parameter definitions,
- BTR:
  - adding some default parameter to ease the modification in *IDXBTR* (see below),
- CSDMXS:
  - fixing missing `override` to overriding virtual function,
- CSDMXC:
  - fixing *clang* warning,
- IDXBTR:
  - **$**,
- IDXBTQ:
  - **$**,
- TOL:
  - adding a feature to ease the temporary disabling of the `system` function message warning.
- V8Q:
  - fixing issue with the `v8.h` use of the `system` command (see above),

## 2018-01-05:
- V8Q:
  - fixing *g++* warning,
- XDHUPS:
  - adding `dlbrry::eNormalization` parameter to `agent___::Init(...)`, due to fact that *Node.js* (*node-pre-gyp*) does not prepend dynamic libraries with the usual `lib` prefix under *POSIX*.

## 2018-01-02:
- CSDMXC:
  - Fixing bad variable type,
- SCLXDHTML:
  - Fixing missing update due to previous modification,
  
## 2017-12-28:
- *shared*:
  - **$**: adding `Execute` function,
- CDSDCMN:
  - adding `Put(...)` variant to resolve some template ambiguity,
- XDHCMN:
  - **$**,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJS:
  - **$**,

## 2017-12-26:
- *shared*:
  - **$**: *casts* can now be set by ids or by tags,
- SCLXDHTML:
  - **$**,
- XDHCMN:
  - **$**,
- XDHDWS:
  - **$**,
- XDHUJP:
  - **$**,

## 2017-12-21:
- SCLXDHTML
  - fixing issue with casting.

## 2017-12-20:
- SCLJRE, SCLZND:
  - adaptation to changes in underlying modules,

## 2017-12-19:
- *shared*:
  - **$1**: more that one wrapper can now be loaded together,
  - **$2**: adaptation to modifications of underlying modules,
- MTHITG:
  - **$2**,
- MTHRTNn:
  - **$2**,
- N4ALL:
  - **$1**,
- SCLNJS:
  - fixing some issues due to previous modifications of this module,
  - fixing some *g++*/*Clang* issues,
- V8Q:
  - adding `v8::External` related functions,
  - fixing some *v8* coming deprecating issue on string object.

## 2017-12-17:
- SCLNJS:
  - handling Strings.

## 2017-12-16:
- SCLNJS:
  - adding `rCore_::HasAssignation`,

## 2017-12-15:
- SCLNJS:
  - functions handling `dArguments` become methods attached to `dArguments`; this will simplify below new feature,
  - a callback can now be launched with all their arguments stored in a `dArguments`object.
- TOL:
  - Adding `*` operator to wrapped (`qW(...)`) object to expose the unwrapped version (`d...`) of the object; this facilitates the disambiguation when using template variadics,

## 2017-12-14:
- *shared*:
  - **$**: adding optional `First` parameter to `Search(...)` functions.
- CTN:
  - **$**,
- STR:
  - **$**,
- XDHUJP:
  - finalization of below new features (continued),

## 2017-12-11:
- *shared*:
  - **$**: finalization of below new features (continued),
- SCLXDHTML:
  - **$**,
- V8Q:
  - Implementing `ToArray(...)`.
- XDHUJP:
  - **$**,

## 2017-12-10:
- *shared*:
  - **$**: finalization of below new features,
- SCLXDHTML:
  - **$**,
- XDHUJP:
  - **$**,
- XDHUJS:
  - **$**,

## 2017-12-09:
- *shared*:
  - **$**:  new *casting* handling (continued),
- SCLXDHTML:
  - **$**,
- XDHUJP:
  - **$**,

## 2017-12-08:
- *shared*:
  - **$1**: unifying argument handling,
  - **$2**: new *casting* handling,
- N4NJS:
  - **$1**,
- SCLN4A:
  - **$1**,
- SCLNJS:
  - **$1**,
- SCLXDHTML:
  - **$2**.
- XDHCMN:
  - **$2**.
- XDHDWS:
  - **$2**.
- XDHUJP:
  - **$2**.

## 2017-12-07:
- *shared*:
  - **$**: adding strings and callbacks handling.
- N4NJS:
  - **$**.

## 2017-12-06:
- SCLMISC:
  - Adding variant of `LoadXMLAndTranslateTags(...)`.

## 2017-12-05:
- *shared*:
  - **$**: reworking of the content handling setting to handle correctly the widgets (continued).
- SCLXDHTML:
  - **$**,
  - some function are becoming `rSession` methods,
  - simplification.
- XDHCMN:
  - **$**.
- XDHDWS:
  - **$**.
- XDHUJP:
  - **$**.
- XDHUJS:
  - **$**.
- XDHUJR:
  - **$**.

## 2017-12-04:
- *shared*:
  - **$**: Reworking of the content handling setting to handle correctly the widgets.
- SCLXDHTML:
  - **$**.
- XDHCMN:
   - **$**,
   - Adding `Flat(Merge|Split(...))`functions.
- XDHDWS:
   - **$**.
- XDHUJR:
   - **$**.
- XDHUJS:
   - **$**.

## 2017-12-03:
- N4NJS:
  - Handling return value when calling a callback.
  
## 2017-12-01:
- N4NJS, NODEQ, SCLNJS:
  - Adding handling of generic object type.
- N4NJS:
  - Merging `eArgumentType_` with `eType`.
- SCLNJS:
  - Handling return value when calling a callback.
  - Adaptation to changes in *N4NJS*.

## 2017-11-30:
- FLX, TXF:
  - Introducing `R` (read) and `W` (write) prefixed objects.
- SCK:
  - Introducing `Undefined`.
- SCLJRE:
  - Introducing some function to ease the handling of user data.
  - Adding `rObject::CallVoidMethod(...)` .

## 2017-11-29:
- N4JRE, SCLJRE:
  - Adding *Short* *Java* type related stuff.
- SCK:
  - Adding default timeout parameter.
- SCLJRE:
  - Fixing `Null()`, according modification of `JREq`.
  - Moving `Delete_` to *.cpp* to avoid some confusion by compilators when user declares some function of some name.
 - SCLXDHTML:
   - Adding `Mode` parameter handling for `SCLXDHTMLRetrieveCallback`.

## 2017-11-28:
- FLW:
  - Adding `R`(ead) and `W`(rite) prefix as synonym for `I`(nput) and `O`(utput).

## 2017-11-24:
- SCLXDHTML, XDHDWS, XDHUJP:
  - Introducing the possibility to put the content before handling widgets (*jQuery* related stuff) when setting the layout, or some widget (*jQTE*, for example) are not properly pre-filled. 

## 2017-11-21:
- SCLFRNTND:
  - Fixing bad handling when no back-end is selected.

## 2017-11-20:
- SCLMISC:
  - Adding target to the message displayed when unable to load locale or configuration file.

## 2017-11-13:
- LOG:
  - Becomes *LOGQ*, to avoid some conflict.

## 2017-11-12:
- CSDLEC:
  - Adding missing `override`.
- CSDULC:
  - Fixing a *Clang* warning.
- V8Q:
  - (**reverted**; was already partially handled and will find another way to fix this issue) Fixing an issue which sometimes occurs under *macOS* depending on the used *Node.js* version.

## 2017-11-07:
- STRMRG:
  - Patching `retriever___::GetTable()` to avoid an error when there are no data.

## 2017-10-29:
- BSO:
  - Fixing *Clang* warning.
- N4ZND, SCLZND
  - Implementing *Stream* object, and *Long* and *Bool*(ean) types.
- SCLJRE:
  - Introducing the new standard where objects are initialized with the `sCaller`, and not the result of one of its method.

## 2017-10-27:
- DLBRRY:
    - Adding the possibility to skip library unloading in destructor (*PHP* workaround on *GNU/Linux* systems).
- N4ALLW:
  - Propagating change in *DLBRRY*.

## 2017-10-18:
- TOL:
    - Surrounding class definition in `qG(C|D)TOR(...)` with anonymous namespace, so that such defined global (con|de)structor will not be shared between different libraries under *GNU/Linux*.

## 2017-10-17:
- SCLLOCALE
  - Fixing error under *POSIX* systems, on which `RackPointer_` seems to be shared between all dynamic libraries !

## 2017-10-12:
- N4ALLW
  - New parameter on `Register(...)` to indicate how the prefix and extension for the *Java*/*Node.js*/*PHP* component/addon/extension should be normalized.

## 2017-10-10:
- N4ALLW:
  - Adding `Launch(...)`.

## 2017-09-27:
- MTHITG:
  - Fixing *g++* warning (displayed under *npm* compilation).

## 2017-09-24:
- TOL:
  - Upgrading `dObject<>`.

## 2017-09-22:
- MTHFRC, MTHRTN
  - Renaming *MTHFRC* to *MTHRTN*.
- MTHITG:
  - Adding `dInteger::Init(...)`variants.
- XTF:
  - Adding `sIFlow` methods variants.

## 2017-09-17:
- N4ALL:
  - Deporting some code to *N4...* libraries.
- N4NJS, SCLN4A, SCLNJS:
  - Upgrading.

## 2017-09-15:
- MTHITG:
    - Upgrading.
- SCLJRE
  - Introducing `SCLNJSInfo(...)`.

## 2017-09-12:
- SCLNJS:
  - Adding `rRStream::Push(...)` variant.

## 2017-09-11:
- THT:
  - Adding `IgnoreTarget` parameter to some *Blocker* methods.

## 2017-09-10:
- FLW:
  - Adding `Take()` methods.
- TOL:
  - Introducing `Same(...)` function.

## 2017-08-31:
- MTX:
  - Reverting a *clang++* workaround due to the fact that some older *clang++* version issues a linker error (see http://travis-ci.org/epeios-q37/xppq-cli/jobs/270384361).
  - Adding some *pragma*s for *clang++*, as it is the only way to disable the warning visible at http://travis-ci.org/epeios-q37/xppq-cli/jobs/233135308.

## 2017-08-29:
- NODEQ, V8Q:
  - Fixing issues occurring with *g++*.
- TOL:
  - With *g++*, The use of `qCOVERx(...)` doesn't work when a macro is calling another one.
- V8Q:
  - Fixing issues occurring with *clang++*.
  - Fixing issue with *Node.js* v4.

## 2017-08-27:
- V8Q:
    - `sData_`and related exists now as persistent (use of `v8::Persistent`) and *local* (use of `v8::Local`).

## 2017-08-26:
- V8Q:
  - `sData_` and related becomes persistent (use of `v8::Persistent` instead of `v8::Local`).
  - Adding missing `Isolate` default parameters.

## 2017-08-25:
- UVQ(DCL):
  - Moving the *UVQ* header to *UVQDCL*.

## 2017-08-20:
- NODEQ:
  - `sBuffer` is now base on `sObject` instead of `sValue`.
- V8Q:
  - Reworking so the objects hierarchy will match the one defined in the *v8* library.

## 2017-08-19:
- NODEQ:
  - Adding some `str::dString` variant methods.

## 2017-08-09:
- N4A, NJS, SCLN4A, SCLNJS:
  - Moving *...NJS...* content to *...N4A...*, in an attempt to reuse the same libraries for all native components.

## 2017-08-08:
- FLW:
  - Initializing some variables, although not necessary, to fix some warnings issued when compiling a *node.js* *addon* on installation on a *POSIX* OS. 

## 2017-08-06:
- ERR:
  - Fixing *clang* warning.

## 2017-08-04:
- MTX:
  - Adding missing `std::` reference introduced in previous change to this library.

## 2017-07-31:
- CIO:
  - Adaptation to changes in *Visual Studio 2017*.
- DLBRRY:
  - Fixing missing default parameter value.
- ERR:
  - Fixing potential stack overflow situation.
- SCLPLUGIN:
  - Adaptation to changes in *SCLARGMNT*.

## 2017-07-27:
- BCH, RGSTRY, UYS:
  - Getting rid of faulty `type &var = *(type *)NULL` parameter definitions.
- DIR:
  - Fixing erroneous assignments.
- DTE:
  - Fixing *clang* warning.
- MTX:
  - Fixing *clang* warning.
- SCLLOCALE:
  - Fixing bad expression.

## 2017-07-17:
- TOL:
  - Adding `qTMIMICS`.

## 2017-07-02:
- NJS:
  - `cArguments` -> `cCaller` (standardization issue).
- SCLNJS:
  - Adaptation to changes in *NJS*.

(2017-06-29: This file becomes a *markdown* file.)

## 2017-06-20:
- DLBRRY:
  - Introducing the `Normalization` parameter.

## 2017-06-04:
- SCLARGMNT:
	- Adding `FillRegistry(...)` variant.
	- Removing useless function since adding above function.

## 2017-05-12:
- RGSTRY:
	- Fixing infinite auto-recursion bug.

## 2017-05-05:
- SCLXDHTML, XDHCMN, XDHDWS, XDHUJP, XDHUJR, XDHUJS:
	- `content` related features are renamed to `value`, to avoid confusion of the new `content` handling.
- XDHDWS:
	- Replacing `Set(Element|Document)...` with a unique `Set...` (equivalent to `SetElement...`).

## 2017-04-28:
- SCLXDHTML, XDHUJP, XDHUJR, XDHUJSn XDHUTL:
	- Adaptation to changes in *XDHCMN*.
- XDHCMN:
	- Replacing `Fill...` by `Fetch...Marks`.
- XDHDWS:
	- `rack`s now contains the `target` string.

## 2017-04-25:
- XDHDWS:
	- The content rack related stuff becomes layout rack.
	- The context rack related stuff becomes casting rack.

## 2017-04-22:
- FLX:
	- Adding missing `override`.

## 2017-04-19:
- XDHCMN, XDHDWS, XDHUJx ;
	- Adding data filling handling.

## 2017-03-24:
- TXF
	- Adding missing `int` variant for operator `<<`.

## 2017-03-23:
- `JNIOBJ` -> `JREBSE`.

## 2017-03-17:
- SCLN -> SCLNJS.
- V8Q, SCLNJS
	- Moving `v8q::sArguments` to *SCLNJS*.

## 2017-03-09:
- MSCMDD, MSCMDF, MSCMDM:
	- Adaptation to changes.

## 2017-03-09:
- SCLARGMNT:
	- Removing unused variable.

## 2017-03-08:
- CSDBNC, CSDMXC, DWTDCT:
	- Adaptation to changes in 'MTK'.
- PRXY:
	- Updating.
- PRXYBASE, SCLFRNTND, SCLXDHTML, XDHDWS:
	- Workaround to modification of '(q|E_)CDEF(...)'.

## 2017-03-07:
- SCLMISC:
	- Adding possibility to 'Initialize(...)' without '.x(cgf|lcl)' files.

## 2017-03-06:
- V8Q:
	- Adding 'console' related features.

## 2017-03-04:
- NODEQ:
	- Rewriting 'sBuffer' so it is based on 'node::Buffer'.

## 2017-03-03:
- SCLN:
	- Adding ABI version in the text returned by the 'info()' function.

## 2017-02-25:
- Global:
	- Fixing g++ warnings.

## 2017-02-24:
- Global:
	- Fixing g++ warnings.

## 2017-02-22:
- Global:
	- Fixing g++ warnings.

## 2017-02-21:
- FLX:
	- Adaptation to changes in 'MTK'.
- MTK:
	- Introducing a variant which protect user data from being freed too early.
	- The original version being prefixed with 'Raw'.

## 2017-02-16:
- FLX:
	- Obsoleting the seemingly useless 'relay_...' classes.
	- 'ASync' related classes becomes 'Relay'.

## 2017-02-15:
- FLX:
	- Fixing twice commit outputing.
	- Improving coherence.

## 2017-02-14:
- FLX:
	-'rMonitor' becomes more versatile.
- XTF:
	- Adding 'Dismiss()' method.

## 2017-02-13:
- FLX:
	- Introducing 'Async...' stuff.
- THT:
	- 'rBlock':
		- can now be used several times.
		- unblocking will do nothing when no blocked counter-part available.

## 2017-02-08:
- DWTDCT:
	- Fixing no retrieving of file beginning with '.' ('.' and '..' are still not retrieved).
- DWTGHT:
	- Fixing bad ghost row retrieving.

## 2017-02-05:
- BSO:
	Fixing some issue due to some issues with 'V8'.
- ERR:
	- Fixing missing definition when used with 'setjmp'.
- FNM, NTVSTR:
	- Moving 'txf::OFlow::operator <<' from 'FNM' to 'NTVSTR'.

## 2017-02-01:
- DWTGHT:
	- If a 'ghost' cannot be created (name too long, for example), this does no more interrupt the global ghost creation,
	but the ghost is simply ignored.
- MTK:
	- Like under 'Windows', threads are no more kept after been used.

## 2017-01-29:
- FLX:
	- Introducing 'GetString(...)'.
	- Introducing 'rIOMonitor'.

## 2017-01-27:
- STR:
	- Introducing 'qSTRING(s)(l)(...)'.

## 2017-01-26:
- SCLMISC:
	- Handling the storing of the lasting registry under 'POSIX' ('AppData' directory handling).
- SCLXDHTML:
	- Software error issued by the backend were not correctly reported.

## 2017-01-20:
- CSDMXC:
	- Simplification.
- FDR, FLW:
	- Introducing the automatic dismissing on enf of flow.
- STSFSM:
	- Introducing 'eUnmatchingBehavior'.
- TAGSBS:
	- Introducing 'Append(...)' method with variadics.

## 2017-01-19:
- CIO, SCLBACKND, SCLTOOL:
	- Adaptation to changes in 'FLX'.
- FLX:
	- Simplification of all 'void' related stuff.

## 2017-01-17:
- DWTCPR:
	- Returning the amount of modified/created files, and the cumulated size of all modified/created files.

## 2017-01-15:
- BSO:
	- Introducing 'SizeMax'.

## 2017-01-13:
- FDR:
	- Adding 'rIDriver::Drain()'.

## 2017-01-12:
- CSDBNC:
	- Adding 'IsConnected(...)' methods.

## 2017-01-04:
- ERR:
	- Adding notice when 'qRReturn' is called when an error is already in progress.
- SCLMISC:
	- Fixing 'qRReturn' issue.
- PLGN, PLGNCORE, SCLPLUGIN:
	- Reverting to 'PLGNCOREReleasePlugin(...)', due to type issue ; exceptions issues still handled.


## 2017-01-03:
- PLGNCORE:
	- 'PLGNCOREReleasePlugin(...)' is replaced by 'PLGNCOREOnPluginRelease(...)'.
- PLGN:
	- Adaptation to changes in 'PLGNCORE'.
- PLGNCORE, SCLPLUGIN:
	- When a plugin is released, it is deleted upstream (delete operator), and not downstream,
	  to prevent issues with exceptions.

## 2016-12-27:
- SCLMISC:
	- Introducing 'SetBaseLanguage(...)'.

## 2016-12-24:
- TOL:
	- Adding 'HaveSameAmount(...)' function.

## 2016-12-22:
- FBLFRD:
	- Introducing frontend callback.
- SCLFRNTND:
	- Exposing some login registry entries.
	- Adaptation to changes in 'FBLFRD'.

## 2016-12-21:
- TOL:
	- The 'reset(...)' functions handles now their arguments in reverse order.

## 2016-12-19:
- FLW:
	- Fixing error handling issues, on writing operations.
- FBLFRD:
	- Improving error handling.

## 2016-12-16:
- RGSTRY:
	- Fixing bug introduced by yesterday's modifications.

## 2016-12-15:
- RGSTRY:
	- Introducing locking feature.
- SCLARGMNT, SCLBACKND, SCLFRNTND, SCLMISC, SCLRGSTRY, SCLXDHTML:
	- Adaptation to changes in 'RGSTRY'.

## 2016-12-13:
- CSDBNC:
	- Fixing bad handling of error parameter.
- SCLARGMNT:
	- Adaptation to changes in 'SCLMISC' (continued).
- SCLFRNTND, SCLXDHTML:
	- Handling default port for straight connexion to backend.
- SCLMISC:
	- Fixing registry locker bug.

## 2016-12-12:
- SCLARGMNT, SCLBACKND, SCLFRNTND, SCLXDHTML:
	- Adaptation to changes in 'SCLMISC'.
- SCLMISC:
	- All registry related functions are now thread safe.
- SCLRGSTRY:
	- Adding registry locking related features.
- THT:
	- Adding 'IsLocked(...)' methods.

## 2016-12-03:
- XDHCMN, XDHDWS, XDHUJP, XDGUJR, XDHUJS, XDHUTL:
	- Adding text selection related features.

## 2016-12-02:
- SCLFRNTND:
	- Extending 'SCLF_I...' macros.

## 2016-11-30:
- DWTDCT:
	- Fixing unreferenced ghost bug.

## 2016-11-29:
- DWTDCT:
	- Handling duplicated ghosts.

## 2016-11-27:
- SCLXDHTML:
	- Adding 'rSession::Alert(...)' variant for integers.

## 2016-11-25:
- CRT, STR:
	- Adding 'NewAndInit(...)'
- LCL:
	- Adding 'meaning_::AddTag(...)' integer variant.

## 2016-11-24:
- BCH:
	- Adding a 'Remove(...)' method removing an item if existing.
- TOL:
	- Adding 'Search(...)'.

## 2016-11-23:
- FBLBKD:
	- Fixing language issue.

## 2016-11-22:
- FBLTYP:
	- Adding 'Convert(...)'.

## 2016-11-20:
- BCH, CTN, CRT, STR:
	- Adding 'Search(...)' functions.

## 2016-11-18:
- TXMTBL:
	- Fixing 'GetCell(...)' bad return value.

## 2016-11-17:
- XDHDWS:
	- Adding 'proxy__::GetBooleanContent(...)'.

## 2016-11-15:
- LTF:
	- Fixing cache size of 0 issue.
- LCL, SCLLOCALE, SCLMISC:
	- Fixing translation with no tag discarding tags issue.

## 2016-11-14:
- FBLTYP:
	- Redefinition of 'wString(s(Set))' to avoid some ambiguities.
- LCL:
	Redefinition of 'wMeaning' to avoid some ambiguities.
- SCLMIC:
	- Fixing 'ReportAndAbort(...') endless recursive call.

## 2016-11-13:
- LCK:
	- 'rControl' -> 'rTutor'.
- SCLMISC:
	- New more versatile 'ReportAndAbort(...)' versions, by using template variadics.

## 2016-11-08:
- FLW:
	- Adding timer in 'Copy(...).'
- SCLFRNTND:
	- Adaptation to changes in 'TOL'.

## 2016-11-01:
- CDBG64:
	- Adding missing header.
- CIO, FLX, HTP, IOF:
	- Adaptation to changes in 'FLW'.
- CSDMXC:
	- Amelioration of error handling.
- FLW:
	- 'iflow' can no more be used in place of the dressed/standalone version.

## 2016-10-30:
- FBLBKD:
	- Fixing bug occurring when on error during the reading of a flow frontend/backend parameter.
- FLX:
	- Error failed 'exec_oflow_driver___' writing does no more generate an error. The error will be handled upstream.

## 2016-10-28:
- GLOBAL:
	- Adaptation to FDR modifications.
- FBLBKD:
	- More accurate handling of frontend/backend parameters.

## 2016-10-27:
- CSDMXC:
	- Simplification.
- FDR:
	- Fixing 'Take' issue.
- FLW:
	- Adding some (des|constr)uctor to be sure the rights ones are executed.
- FLX:
	- Adaptation to changes in 'FDR'.

## 2016-10-27:
- FDR:
	-Adding possibility to change the owner of a io driver.
- GLOBAL:
	- Adaptation to changes in 'FDR'.


## 2016-10-25:
- CDGB64:
	- Introducing 's(En|De)coding(I|O)Drive.)'
	- Adaptation to changes in 'FDR'.
	- Fixing wrong end of flow when decoding.
- CIO:
	- Adding 'Get(In|Out|Err)Driver()'.
- CSDMXC:
	- Amelioration of error handling.
- FDR:
	- Adding 'Copy(...)' and 'Purge(...)'.
	- Using thread ID instead of owning flow to handle thread-safety.
- FLF:
	- Adding some default parameter variants.
- FLW, FLX:
	- Adaptation to changes in 'FDR'.
- RGSTRY:
	- Fixing bad error relaying.
- SCLMISC:
	- Adding 'r(I|O)DriverRack'


## 2016-10-24:
- CSDBNC:
	-  Removing 'OwnershipTaken' argument in `cProcessing::CSDSCBPreProcess(...)`, in favor of  the boolean returned value of `cProcessing::CSDSCBPostProcess`.
- CSDBNC, CSDLEC, CSDMXS, SCLDAEMON:
	- Adaptation to changes in 'CSDBNC'.
- DTW:
	- Detecting some infinite loop.
- FLW:
	- Virtualization of destructors.
- TOL:
	- In 'sTimer', 'Elapsed'()' -> 'ElapsedTime()', to avoid some oddities with 'sTimer::Elpased()'.
- MTX:
	- Adding timeout feature to lock operations.

## 2016-10-23:
- CSDBNS, CSDSCB, CSDMXS:
	- Adding a feature which simplifying the handling of socket reading and writing in different thread (as in 'prxyq').
- SCLDAEMON:
	- Adaptation to changes above.

## 2016-10-22:
- FLX:
	- Avoid further read attempt from underlying flow after abrupt deconnection.

## 2016-10-21:
- FBLBUR:
	- Is now obsolete.
- FBLBKD, SCLBACKND:
	- Adaptation to the fact that 'FBLBUR' is obsolete.

## 2016-10-20:
- CSDBNC:
	- Introducing 'rIODriver'.
- FBLFRD:
	- Adaptation to changes in 'CDGB64'.
- FDR:
	- Introducing 'r(I|O|IO)[Dressed]Driver'.
- FLW:
	- Adding missing 'Dismiss()' in 'iflow__::reset()'.
- FLX:
	- 'sSizeDelimitedIFlowDriver'' -> 'rSizeDelimitedIDriver'.
	- 'sSizelimitedOFlow' (?) -> 'rSizeDelimitedOFlow'.
- SCK:
	- Removing 'socket_ioflow_driver___' useless 'User_' member.
- TXF:
	- Introducing 'rOFlow'.

## 2016-10-19:
- CSDB64
	- Implementing the true base 64 encoding/decoding algorithmes.
	- Handling both original and URL varaint.
	- Skipping no base 64 characters (newlines, as found in mail bodies) when decoding.
- FDR, FLW:
	- Adding some new notations aliases.
- FLX:
	- Introducing 'SizeDelimitedIFlow(Driver)'.

## 2016-10-18:
- CSDMXC:
	- Skip reading/writing after having give up.

## 2016-10-16
- HTP:
	- Implementing missing methods.

## 2016-10-12:
- CSDMXC:
	- Fixing dead lock problem.
	- An error leads now to disconnection (no more try to /read:write on a closed connection).
- FDR:
	- Introducing new notation.
- FBLFRD:
	- Handling of input flow deconnection.
	- Moving reporting callback to 'SCLFRNTND'.
- SCLFRNTND:
	- Retrieving reporting callback from 'fblfrd'.

## 2016-10-11:
- FDR:
	- 'Unlock(...)' method is now public.
- FLW:
	- No more call to 'Commit(...)' for unlocking, but directly 'Unlock(...)'.
- DWTCPR:
	- Fixing bug occuring when there were excluded directories (bug occuring due to the 'DWTDCT' modification the '2016-10-10').

## 2016-10-10:
- DWTDCT:
	- The filename size limit concerns now the absolute path of the file and not only its name.
- SCLBACKND:
	- Versionning the to frontend exposed function.

## 2016-10-09:
- CPE:
	- Adaptation to Android.
	- Adaptation for MIPSel architecture.
- NTVSTR:
	- Removing inclusion of 'langinfo.h'.
- THTSUB:
	- Inclusion of 'pthread.h' instead of 'sys/types.h'.
- TOL:
	- Restricting inclusion of 'sys/timeb.h' to Windows.

## 2016-10-07:
- DWTMOV:
	- On directory creation, when the directory already exists, the creation is skipped.
	- On directory creation, when a file exists with same name, the file is deleted.
	- When an error occurs on directory creation, no more 'ERR' error is throwned,
	  but the regular error handling is used.
- DWTCPR:
	- File modification timestamps within 3 seconds are considered as equal (to handle Windows file timestamp imprecisions).

## 2016-10-03:
- DWTGHT
	- Ghosts files/directories are now (again) made systm (hidden). Only under Windows. For POSIX system, their name have to begin with a '.'.

## 2016-10-02:
- ERR:
	- The 'qRFE(...)' macro now resets error only if there was one pending.

## 2016-09-27:
- BSO:
	- Fixing issue occuring with the 'mingw' compiler.

## 2016-09-24:
- CTN:
	- Adding parameter testing.

## 2016-09-22:
- DIR:
	- Introducing 'rFileBrowser' to replace 'rHandle' and related functions.
- SCLMISC:
	- Fixing parameters overriding for '(Dump|Delete)LastingRegistryFile(...)'.

## 2016-09-21:
- ERR:
  - Fixing bug which blocks all error handling in other threads after a 'ERRFinal'.
- RGSTRY:
	- Fixing 'multi_level_registry_::Push(...)' bug.
	- Fixing 'multi_level_registry_::Set(...)' bug.
- SCLMISC:
	- Adding '(Dump|Delete)LastingRegistryFile(...)'.
- SCLPUGIN:
	- Adaptation to changes in 'SCLRGSTRY'.

## 2016-09-20:
- DIR:
	- The 'Get(User|Common)AppDataPath(...)' are prefixed with 'Win'.
	- Adding 'GetAppDataPath( ...)'.
- SCLMISC:
	- Adaptation to changes in 'SCLRGSTRY' and 'DIR'.'
	- Adding '(Set|Add)LAstingValue(...)'.
	- Adding 'StoreLastingRegistry()'.
- SCLRGSTRY:
	- 'User' and 'Common ' levels become 'Lasting'.
- TOL:
	- Adding 'GetnEnv(...)'.

## 2016-09-18:
- DIR:
	- Adding application data related functions.
- SCLMISC:
	- Adding application data handling functions.
	- Adaptation to chnages in 'SCLRGSTRY'.
- SCLRGSTRY:
	- Adding application data related registry levels.
	- Simplification.

## 2016-09-16:
- DIR:
	- Adding error parameter to 'CreateDir(...)' and 'CreateDirWithParents(...)'.
- EPSMSC:
	- Adding 'sclmisc::SCLMISCOrganizationName' definition.
- RGSTRY:
	- When an error occurs, the details is passed, and no more a generic errror.
	- 'multi_level_registry___' now returns 'true' is the registry has no children, instead of not being initialized.
- SCLARGMNT:
	- Adaptation to changes in 'SCLRGSTRY'.
- SCLMISC:
	- 'SCLMISCManufacturerName' -> 'SCLMISCOrganizationName'.
- SCLRGSTRY:
	- 'name__' -> 'eLevel'.
	- Simplification.
- XML:
	- Adding error parameter to Parse(...)'.'

## 2016-09-15:
- FBLBRR:
	- Adding some missing types.
- XDHDWS:
	- Adding error flag to 'proxy__::GetNumeriaclaContent(...)'.

## 2016-09-14:
- DIR:
	Adding 'Get(User|Common)AppDataPath(...)' functions (implemented for windows only).
- SCLMISC:
	- Adaptation to changes in 'SCLRGSTRY'.
	- Adding 'SCLMISCManufacturer' and 'SCLMISCProduct'.
- SCLRGSTRY:
	- Adding 'Common' and 'User' registry level.
	- Replacing 'Configuration' related level by 'Main'.
	- Replacing 'Configuration' related functions by generic ones.

## 2016-09-09:
- CPE:
	- Adaptation to ARM 64 bits architecture (may compromise ARM 32 bits architecture).

## 2016-09-08:
- PLGN:
	- Normalizing arguments before passing them to plugin.
- SCLARGMNT
	- Adding 'Split(...)' &nd 'Normamlize(...)'.
- SCLPUGIN:
	- Using ''

## 2016-09-06:
- LCL:
	- From now on, translation of an empty text returns the lists of the tags ('%0' marker), even if there is an entry in the locale.
- SCLMISC:
	- Adding missng 'scelerror:ResetPendingError(...)', otherwise fiurther errors are not displayed.

## 2016-09-05:
- LOG:
	- Fixing the no newline bug when the new message is same but shorter as the old one.

## 2016-09-04:
- LTF:
	- Fixing overflow bug.
- LOG:
	- Creation.

## 2016-09-02:
- BSO:
	- Fixing warning issued when compiling under WSL (Widows Subsystem for Linux).

## 2016-08-30:
- CSDBNS:
	- Fixing some issues related to automatisation of socket closing.
	- 'SkipSocketClosing' -> 'OwnerShipTaken'.
- CSDLEC, CSDMXS, SCLDAEMON:
	- Adaptation to 'CSDSCB'.
- CSDSCB:
	- Introducing 'OwnerShipTaken'.
	- the flow is passed as pointer, so it can be deleted.
- PRXYBASE:
	- Introducing 'GetOther(...)'.

## 2016-08-28:
- CSDBNC:
	- Introducing 'SkipSocketClosing'.

## 2016-08-22:
- CSDBNC, CSDBNS:
	- Adaptation to changes in 'SCK'.
- CSDMXS:
	- Fixing error occuring on regular deconnection from frontend.
- SCK:
	- Adding possibility to give the ownership for a socket, so it can be properly closed.

## 2016-08-21:
- SCLFRNTND:
	- '&' ->'&&' in test.
- SDR:
	- Adding 'tol::reset(...)' ('tol' namespace to facilitate header inclusions) funtion to hnadle 'bRow'.
- TOL:
	- Adding 'reset(...)' function to handle boolean.

## 2016-08-20:
- SCLMISC:
	- 'ErrFinal()' displays now the pending 'SCL' errors.

## 2016-08-17:
- PRXYBASE:
	- Adding missing 'eAnswer' 'aOK' label.
- THT:
	- Fixing 'rLocker::Unlock()' bug.
- SCLRGSTRY:
	- Adding '0' and '1' as recognized boolean values.

## 2016-08-16:
- CSDCMN:
	- Fixing 'Get(...)' return value issue.

## 2016-08-15:
- ERR, THT:
	- Use of 'THTSUB'.
- THTSUB:
	- Creation to put type declaration to simplify some header handlings.

## 2016-08-14:
- CSDCMN:
	- 'wStrings' were not correctly detected (template issue).
- TOL:
	- Adding 'plug(...)' for serialization.

## 2016-08-13:
- PRXY:
	- Adapting to changes in 'PRXYBASE'.
	- 'csdbnc::rIOFlow' is now herited by 'rProxy_'.
- PRXYBASE:
	- Simplifyng answers.

## 2016-08-12:
- PRXY:
	- 'rProxy' -> 'rProxy_'.
	- Moving 'csdbnc::flow___ Flow_' from 'rFlow' to 'rProxy'.
	- Adding 'RequestDimiss(...)' function.
- TOL:
	- Adding 'reset(...)' function to handle pointers.

## 2016-08-11:
- MTX:
	- Moving 'THT' related function to '.cpp' to facilitate header managing.

## 2016-08-10:
- PRXYBASE:
	- Adding the 'Dismiss' request.
- MTX:
	- Adding predeclaration oh 'THT' functions.
- SCLERROR:
	- Adaptation to changes in underlying modules.
- THT:
	- Adding 'rBlocker' and 'rLocker'.

## 2016-08-07:
- CDGURL:
	- Adding missing header.

## 2016-08-06:
- CSDCMN:
	- Fixing bad argument type.

## 2016-08-03:
- FBLBKD:
	- Fixing object deletion bug.
	- Introducing 'sIRow' to avoid above type of bugs.
	- 'index__' -> 'sIndex' and has its own type (no more a 'sdr::sRow' type).

## 2016-08-03:
- CSDCMN:
	- Adding integer 'Put'/'Get'.
	- Adding strings 'Put'/'Get'.
	- Some parameters renaming.
- CSDSCB:
	- Introducing timeout in plugin override.
- SCLBACKND, SCLDAEMON:
	- Adaptation to 'CSDSCB' changes.

## 2016-08-01:
- FLW:
	- New namings.
- PLGN:
	- Adding 'IsReady()' methods.
- SCLFRNTND:
	- Reorganizing and simplifying backend parameters handling.
	- Fixing '\' issue with backend parameters feature.
- SCLMISC:
	- Adding 'rTOFlowRack::IsFile()'.
- SCLPLUGIN:
	- Fixing crash when reporting an error in the arguments.
- SCLXDHTML:
	- Adapting to changes in 'SCLFRNTND'.

## 2016-07-31:
- CSDLEC:
	- Introducing 'rDriver'.
- FBLOVL:
	- 'mode__' -> 'eMode'.
	- 'mRemote' -> 'mSerialized'.
	- 'mEmbedded' -> 'mReferenced'.
	- Introducing 'cDriver'.
- FBLBKD, FBLBUR, FBLFRD, SCLBACKND:
	- Adaptation to changes in 'FBLOVL'.
- SCLFRNTND:
	- 'features___' -> 'rFeatures'.
	- 'kernel___' -> 'rKernel'.
	- 'frontend___' -> 'rFrontend'.
	- Embedded backend is now accessed through a plugin too.

## 2016-07-29:
- TME:
	- Prise en compte du format sur 12 h ('AM'/'PM').

## 2016-07-28:
- SCLXDHTML:
	- 'Refresh' related actrion moved to action herlpet.
	- Introducing 'SCLX' instead of 'SCLXDHTML' notation, which will be the new one in the future.
- XDHCMN:
	- Introducing 'RefreshActionLabel'.

## 2016-07-25:
- SCLXDHTML:
	- The backend visibility is now stored in 'rSession'.

## 2016-07-22:
- CSDMXB:
	- 'fId' -> 'sId'.
- CSDMXS:
	- Adaptation to changes in CSDMXB.
- CSDMXS:
	- Introducing file log callback.
	- Fixing the logical connection closing also closing the physical connection bug.
	- Adaptation to changes in CSDMXB.

## 2016-07-21:
- CSDMNC:
	- Adaptation to changes in CSDMXC.
	- Fixing 'Timeout'<->'PingDelay' bug.
- CSDMXC:
	- 'cLogCallback' -> 'cLog'.
	- Introducing file log callback.
	- 'Retrieve' -> 'Acquire'.
- SCLBACKND:
	- Adaptation to changes in SCLDAEMON.
- SCLDAEMON:
	- The muxing flow is deported to the 'straight' 'dmnzq' plugin.

## 2016-07-20:
- CSDMXB:
	- Fixing bug when mixing 32/64 bits client/server.

## 2016-07-17:
- SCLFRNTND:
	- Modifications to the 'RemotePlugins' handling.
- SCLXDHTML:
	- Adaptation to changes in SCLFRNTND.

## 2016-07-16:
- SCLFRNTND:
	- Improving default project and backend handling.
- SCLXDHTML:
	- Adapting to changes in SCLFRNTND.
	- The login page can now be configured to hide the backend-related part.

## 2016-07-15:
- RGSTRY:
	- Modifications to facilitate a thread-safe use.

## 2016-07-10:
- DLLBRY:
	- Under 'Windows', all '/' in library path are replaced with '\', as Windows ('XP' only ?) needs '\'.'

## 2016-07-07:
- LCL:
	- Reorganization to arrange some oddities due to below modifications.
- SCLFRNTND:
	- Fixing missing login parameters bug.

## 2016-07-06:
- LCL:
	- Adding 'dMeaning::AddTags(...)' method.
	- Adding 'locale_::GetTranslation(...)' method variant.
- SCLLOCALE:
	- Replacing all 'GetTranslation(...)' with one based on variadics.
- STSFSM:
	- Adding 'Fill(...)' function variant.
		- Adding 'qRP' parameters to 'GetId(...)' functions.

...

## 2016-05-23:
- SCLBACKND, FBL...
	- Reorganization.

## 2016-05-20:
- SCLFRNTND, SCLXDHTML:
	- Adding authentication features.

## 2016-05-18:
- CRT, LSTCRT:
	- NEw modules as replacement for '(LDT)CTN' modules.

## 2016-05-11:
- FBLBKD:
	- Adding templated 'Object' method to 'backend__'.

## 2016-05-06:
- PLGN:
	- Adding 'Details(..)' and 'Identifier(...)' methods to 'wRetrievers'.

## 2016-05-04:
- FBLBRQ:
	- 'request__' -> 'request___'.
	- 'sRequest' -> 'rRequest'.
- FBL...
	- Adapting to changes in 'FBLBRQ'.

## 2016-05-02:
- SCLFRNTND:
	- Macros to define ids, label, and/or comment define shorter names for ids.*
- XML:
	- Adding 'dWriter::Put(Value|Attribute)(...)' methods which take a number.

## 2016-04-28:
- XPP:
	- Fixing error which occurs when a 'expand' directive was within a 'cdata' directive.

## 2016-04-27:
- FBLBKD:
	- Using new 'tol::Crash()' function to crash the backend.
- FBLFRD:
	- IF the message returned by the backend is too long, it does no more génrate an error, but is truncated.
- PLGNCORE, SCLPLUGIN:
	- The error is now intercepted when it occurs while relesing a plugin, and does no more crash the program.
- TOL:
	- Adding 'Crash()' function.

## 2016-04-26:
- SCLMISC, SCLRGSTRY:
	- Adaptation to changes in 'XPP'.
- XPP:
	- 'criterions___::Directory' parameter is now mandatory.
	- Introducing the '_SelfPath' variable containig the path of the currently preprocessed file.
	- Introducing the '_BinPath' variable containig the path of the binary processing file.

## 2016-04-20:
- CSDBNS:
	- the 'server___' can now be freezed ; for watchdog testing purpose.

## 2016-04-20:
- CDGB64:
	- Adding '(En|De)code' functions.
	- Fixing 'EnfOfFlow()' issue.
- FBL...:
	- Adding 'Ping' and 'Crash' commands.
- SCLBACKND:
	- Adapting to changed in 'FBL...' modules.

## 2016-04-19:
- CSDBNS, CSDMNC:
	- Adaptation to changes in 'CSDBNC'.
- CSDBNC:
	- Adding 'TimeOut' parameter to 'flow___::Init(...)'.
	- Removing default value for 'ErrHandling' parameter in 'flow___::Init(...)', because a 'err::handling___' value is confused with a 'duration__'.
- SCK:
	- Removing default value for 'TimeOut' parameter.

## 2016-04-17:
- TOL
	- Adding 'rTimer::Elapsed()'.

## 2016-04-14:
- FDR, FLW:
	- Adding amount of satared/written since last dismiss/commit.
- FLX:
	- When no data is written in a 'embedded_size_oflow___', the final '0' is now sent.
- GLOBAL:
	- Adaptation to changes in above modules.

## 2016-04-12:
- SCLRGSTRY:
	- Adding runtime level.

## 2016-04-09:
- SCLFRNTND:
	- Introducing 'eBackendSetupType' abd associated functions.

## 2016-04-08:
- BSO:
	- Adding 's[U|S]Big' type.
- DIR:
	- Adding 'GetDisk(Free|Total)Space(...)'.
- FBL...
	- Adding '(U|S]Big)' type handling.
- GLOBAL:
	- Adaptation to BSO.

## 2016-04-07:
- CTN:
	- Fixing 'basic_cintainer_::Remove(...)' limit case.
- DIR:
	- Adding handling of empty device (DVD drive, for example) in 'GetFirstFile(...)'.
- XDHDWS:
	- Adding 'proxy__::GetNumericalAttribute(...)'.

## 2016-04-06
- DIR
	- Adding 'FileSystem(...)'

## 2016-04-04:
- CTN:
	- Fixing incomplete 'Flush_()'.

## 2016-04-03:
- DWTDCT:
	- Fixing mutitasking issue in exclude handling.
- XPP:
	- Diwing cdata nesting bug.

## 2016-04-01:
- GLOBAL:
	- Fixing missing flush.

## 2016-03-29:
- TOL:
	- fixing 'dObject' 'reset(...)'.

## 2016-03-29:
- CSDRCU, CSDUCL, PLGN, PLGNCORE, SCLPLUGIN:
	- Adding error handling alternative (cont.).

## 2016-03-28:
- CSDRCU, CSDUCL, PLGN, PLGNCORE, SCLPLUGIN:
	- Adding error handling alternative.
- LCL, SCLMISC:
	- A 'tag" can now be a 'ntvstr::string___'.

## 2016-03-27:
- PLGNCORE:
	- 'AboutPlugin()' can now be called without initialization.

## 2016-03-26:
- GLOBAL:
	- Reorganization of handling of error, locale, consile input/output.

## 2016-03-25:
- GLOBAL:
	- Reorganization of handling of error, locale, consile input/output.

## 2016-03-22:
- PRXY, PRXYBASE:
	- Rewriting the protocole to be more verbose, so it can be extended.

## 2016-03-21:
- FBLBKD:
	- Simplification.
- TOL:
	- Fixing some bug in 'dObject'/

## 2016-03-15
- DWTFTR:
	- The 'Goofs' tag was not handled when in 'Load(...)'.
- FLW:
	- Adding 'Purge()' function.
- FNM:
	- Applying new naming conventions.

## 2016-03-14:
- CSDRCU, PLGN:
	- Adding 'About()' and 'Identifier'()' methods.
- CSDUCL:
	- Adding 'RemoteAbout()' and 'RemoteIdentifier'()' methods.
- LCL:
	- Fixing bug by reverting to the version using the 'CTN' 'ITEM's.
- LCK:
	- Applying new naming conventions.
- PLGNCORE:
	- Adding '(PLGNCORE)AboutPlugin()' method.
	- Changing some method parameters and fixing name.
- SCLFRNTND:
	- Adding 'kernel___::AboutPlugin()'.

## 2016-03-13:
- FBLBKD:
	- Moving error handling from 'FBLBKD' to 'SCLBACKND'.
- SCLBACKND:
	- Moving error handling from 'FBLBKD' to 'SCLBACKND'.
	- Fixing 'Setup' registry bug.

## 2016-03-09:
- Global:
	- Simplification.

	.
	.
	.

## 2016-02-29
- GLOBAL:
	- Huge rewriting (cont.) of the 'hook's and 'file hooks'.

## 2016-02-28:
- GLOBAL:
	- Huge rewriting (cont.) of the 'hook's and 'file hooks'.

## 2016-02-27:
- GLOBAL:
	- Huge rewriting (again) of the 'hook's and 'file hooks'.

## 2016-02-26:
- XML
	- Adding 'vWriter::Put(...)' (retrieved from 'XPP' library) to put and indent XML data.
- XPP:
	- Use of above new methods.

## 2016-02-22
* TXF:
	- Adding 'Copy(..)' function.	
- UYS:
	- Optimization of copy between 2 'vUntypesStorage'.

## 2016-02-21:
- GLOBAL:
	- misc.
	- Huge rewriting of the 'hook's and 'file hooks'.

			.
			.
			.

## 2016-02-12:
- DWTCPR, DWTFTR:
	- Adaptation to chages in 'STSFSM'.
	- The 'Dump' functionnalitiés are always made in the current version.
- GLOBAL:
	- Applying new naming conventions.
- XML:
	- An 'iflow' can now be used as value.

## 2016-02-03:
- MISC:
	- Heavy changes about muxed connections handling.
- TOL:
	- Introducing 'qCALLBACK_DEF(...)'.

## 2016-01-31:
- FLX:
	- 'size_embedded_oflow[_driver]___' can be configured so the 'Commit()' is or not propagated to underlying flow.
- FBLBRR, FBLFRP, PRXY:
	- Adaptation to changes in 'FLX'.

## 2016-01-30:
- IDS, STK:
	- Adding files hook related features.
- LST:
	- Fixing files hook bad released entries reading.

## 2016-01-27:
- CSDRCC, CSDRCU, CSDUCL, SCLFRNTND:
	- Adaptation to undelying modules modifications.
- PLGN, PLGNCORE, SCLMISC, SCLPLUGIN 
	- 'Identification' -> 'Label'.
	- Introducing 'Identifier', which is an UUID peculiar to each plugin.

## 2016-01-26:
- DWT...
	- Ghosts are now files.
- PLGN:
	- Temporary workaround for twice (or mode) loaded plugin.
- SCLARGMNT:
	- Erasing 'Arguments' registry when the same plugin is loaded more that once from the same excutable.
- SCLRGSTRY:
	- Adding 'EraseArgumentsRegistry()'.

## 2016-01-25:
- SCLMISC, SCLRGSTRY, SCLTOOL:
	- Adding 'sclmisc::DumpRegistriesIfRequired()', retrieved from similar function in 'SCLTOOL'.
- GLOBAL:
	- Applying new naming conventions.

## 2016-01-24:
- GLOBAL:
	- Applying new naming conventions.

## 2016-01-22:
- FBLBKF:
	- Fixing bad order destruction in 'ram_module::reset()'
	- Using the list bunch object rather than the combination of a list and a bunch for simplification.
- FBLFRD:
	- Adding 'universal_frontend___::Disconnect()'.
	- Fixing incomplete deconnection.
- GLOBAL:
	- Applying new naming conventions.
- PLGN:
	- Fixing bad locale filling.
- SCLXDHTML:
	- Fixing bad deconnection procedure.

## 2016-01-21:
- GLOBAL:
	- Applying new naming conventions.

## 2016-01-20:
- FBLBKD, FBLBRD:
	- Fixing Clang complainings.
- GLOBAL:
	- Introducing new conventions.
- PLGN:	
	- Fixing 'rRetrievers::Plugin(...)' wrong deletion.

## 2016-01-19:
- PLGN:
	- Fixing retrievers deleting.

## 2016-01-18:
- GLOBAL:
	- Introducing new naming conventions.

## 2016-01-17:
- GLOBAL:
	- Introducing new naming conventions.
- PLGN, SCLMISC, SCLRGSTRY:
	- Introducing plugins set.

## 2016-01-16:
- GLOBAL:
	- Introducing new naming conventions.

## 2016-01-15:
- GLOBAL:
	- Introducing new naming conventions.

## 2016-01-14:
- GLOBAL:
	- Introducing new naming conventions.

## 2016-01-13:
- CSDBNC:
	- Adding 'flow___'.
- SCLMISC:
	- Adding the 'ErrFinal()' function retrieved from 'SCLTOOL'.
- SCLTOOL:
	- Using the 'ErrFinal()' function from 'SCLMISC'.

## 2016-01-12:
- CSDLEC:
	- Adaptation to chnges in 'CSDLEO'.
- CSDLEO:
	- Removing the useless variadic parameter.
- CSDMNS:
	'_callback___' -> 'callback___'.
- SCLBACKND:
	- Adaptation to changes in 'SCLDAEMON'.
- SCLDAEMON:
	- Adaptation to chnges in 'CSDLEO'.
	- Handles now the muxed mode too.

## 2016-01-08:
- CSDMNS:
	- 'log_functions__' -> 'log_callback__'.
	- Standardisation of the optionality of the 'LogCallback' parameter.
	- Adding missing methods to 'server___'.
- SCLMISC:
	- The plugin arguments were retrieved from the 'Filename' entry instead of the 'Arguments' one.
	- The plugin arguments are now retrieved from the 'Parmeters/Plugin' entry value, and no more from the 'Definitions/Plugins/Plugin/Arguemnts' entry.
	- The plugin id for a given target is now retrieved from the 'Id' attribute.
- SCLPLUGIN:
	- The plugin arguments are now separated by a space, with '\' as escape character.
- SCLRGSTRY:
	- Removing/adding entries related to plugins needed to changes in 'SCLMISC'.

## 2016-01-06:
- 'CSDSNS' -> 'CSDMNS'.
- 'CSDSNB' -> 'CSDMNB'.
- 'CSDSNC' -> 'CSDMNC'.

## 2016-01-04
- CSDRCU
	- Adaptation to changes in 'PLGN'.

## 2016-01-03:
- PLGN:
	- 'Init(...)' -> 'Initialize(...)'.
- RGSTRY:
	- Adding 'Dump( const entry__ &, ... )'.
- SCLMISC:
	- The plugin locale is now added to the main locale.
	- Plugin arguments are now handled.
	- Adaptation to changes in 'PLGN'.

## 2016-01-02:
- SCLRGSTRY:
	- Adding 'Arguments' entry for the plugins.

## 2015-12-21:
- CSDUCL, PLGN:
	- Fixing Clang complainings.

## 2015-12-14:
- DIR:
	- When trying to access a forbidden dirctory, it's assumed to be empty.

## 2015-12-09:
- SCLARGMNT, SCLBACKND, SCLMISC, SCLTOOL:
	- Adaptation to changes in 'SCLRGSTRY'.
- SCLRGSTRY:
	- Adding 'Debug' entry.
	- Introducing 'name__'.
	- 'FillSetupRegistry(...)' -> 'FillWithSetup(...)'.

## 2015-12-08:
- BSO, TOL:
	- Conversion from string to number are now more complete.
- IAS:	
	- Fixing CLang complainig.
- RGSTRY:
	- Adaptation to modifications in 'BSO'.

## 2015-12-05:
- TOL:
	- Implementing 'Reset( tick__ &)' for mossiong plateform.

## 2015-12-02:
- GLOBAL:
	- '..._hook_filenames___' -> '..._hf___'
	- '..._files_hook___' -> '..._fh___'

## 2015-12-01:
- FLSQ
	- When instanciated, 'file_storage___::Temoin_.Mode' is set to 'fil::m_Undefined', so it can be detected
	  that it's not yet initialized.
- UYS:
	- Adapting to changes in 'FLSQ'.

## 2015-11-30:
- TOL:
	- 'timer__':
		- Can now be disabled,
		- right after initialization, is considered as elapsed, until call of 'Launch()'.

## 2015-11-29:
- PLNGCORE:
	- Modification to fit with changes in 'SCLMISC'.
- PLNG:
	- Adaptation to changes in 'PLNGCORE'.
- SCLLOCALE:
	- Adding 'Dump(...)'.
	- Adding 'Fill(...)'.
	- Removing 'Locale' suffix from functions.
- SCLMISC:
	- Modifying some 'Initialization(...)' functions, so, either the configuration registry is given, either the
	  directory for configuration and locale file ; in thgis latter case, the content of the locale registry is
	  returned. This functions are used for plugins, which locales are handled by the calling program.
	- Adpattion to changes in 'SLLOCALE'.
- SCLPLUGIN:
	- Adaptation to 'PLGNCORE'.

## 2015-11-27:
- XDHUTL:
	- Automats were not filled.

## 2015-11-26:
- CSDUCL:
	- Using new 'CSDRMU' module instead of 'CSDRMC'.

## 2015-11-25:
- CSDUCL:
	- Fixing missing return value.
	- Removing daemon features (replaced by remote features).
- ERR:
	- 'err___::File' stores now the content of the string, and no more only the pointer, because of plugins	which may be
	  unloaded before the error was handled, causing a segmentation fault (due to acces to data of the unlodad plugin).
- SCLFRNTND:
	- Adding handling of remote backend.
	- Usign 'features___'.
	- Adapting to changes in 'CSDUCL'.
- SCLPLUGIN:
	- Adding a initialization member called when the plugin is created.
- SCLXDHTML:
	- Taking account of the remote access plugin.

## 2015-11-24:
- CSDUCL:
	- Introducing 'CSDRMC'.
- SCLFRNTND:
	- Use of remote plugin instead of embedded daemon access.

## 2015-11-23:
- OLNG, PLGNCORE, SCLPLNG:
	- Adding/changing arguments handling features.
- SCLARGMNT, SCLTOOL:
	- Moving all argument handling code from 'SCLTOOL' to new library 'SCLARGMNT', in order to allow the use of arguments
	  to other program type as tools (in particulary plugins).

## 2015-11-22:
- DLBRRY:
	- Fixing 'clang' complaining.
- SCLFRNTND:
	- As 'kernel___' instances are now handled externally, 'Connect(...)' functions has been replace by functions
	    filling 'features___' instances.
- SCLXDHTML:
	- 'login::GetBackendFeatures(...)' deals now with 'sclfrntnd::features___'.

## 2015-11-19:
- SCLXDHTML:
	- Adaptation to chnages in 'SCLFRNTND'.
	- Changing 'GetBackendFeatures(...)' parameters.

## 2015-11-18:
- FBLFRD:
	- 'frontend___::Dismiss(...)' -> 'DismissFlow'.

## 2015-11-17:
- SCLFRNTND:
	- Exposing 'kernel___' in order to allow the access to several backend.
- CTN, DTR, IDWBTQ, LSTBCH, LSTCTN, UYS:
	- Removing 'files_hook___' useless 'Bind()' member.
- IAS:
	- Removing 'files_hook___' useless 'Bind()' member.
	- Some files were not created when storage was empty, albeit other files were created.
- SCLFRNTND:
	- If the connection to the backend fails, writing of a error message to the console and aborting.
- TOL:
	- Bad flavor handling.

## 2015-11-16:
- SCLBACKND:
	- 'backend___' hosts now a registry, so each instance can have its own setup.
- SCLMISC, SCLTOOL:
	- Adapting to changes in 'SCLRSTRY'.
- SCLRGSTRY:
	- Exposing setup registry handling with registry specification.

## 2015-11-13:
- TOL:
	- Cosmetic change.

## 2015-11-12:
- DTR:
	- Adding files hook related features.
- LSTBCH:
	- Privatization of members of 'struct hook_filenames___'

## 2015-11-11:
- BCH:
	- Adaptation to changes in underlying module.

## 2015-11-10:
- SCLRGSTRY:
	- Adding a comment.

## 2015-11-05:
- ERR:
	- Removing forgotten old 'Data' related item.
- LTF:
	- Adaptation to underlying modules.
- SCLMISC, SCLTOOL:
	- As 'SCLMISC' is now again aware of 'CIO', 'text_oflow_rack___' is moved from 'SCLTOOL' to 'SCLMISC'.

## 2015-11-03:
- SCLTOOL:
	- For non-windows OSes, returns to CLI with failure value when success (and vice-versa).

## 2015-10-28:
- BTR, IDXBTR:
	- Fixing the inadvisable '&Var == NULL', where 'Var' is a reference.
- CSDLES:
	- Fixing Clang warning ; the function is used in a C++ context, so the warning can be igonred.

## 2015-10-27:
- RGSTRY:
	- Handle complaining of LLVM.

## 2015-10-07:
- ERR:
	- Skip the 'qRR' for a 't_Return' error type.

## 2015-10-06:
- DIR:
	- 'Filename' -> 'FileName'.

## 2015-10-01:
- XDHUJR:
	- 'XDHUJS' -> 'XDHUJR'.
- XDHUJS, XDHUJT:
	- 'XDHUJT' becomes 'XDHUJS'.

## 2015-09-30:
- XDHUTL:
	- 'FetchEventAbstracts(...)' now returns a boolean to report a key-related event without recognized key sequence.

## 2015-09-29:
- RGSTRY:
	- Adding 'tentry___[::Init](...)' variant.
- SCLFRNTND:
	- The language is now stored/retrieved in/from the registry.
- XDH...
	- Misc. changes.

## 2015-09-...:
- XDH...
	- Misc. changes.

## 2015-09-24:
- XDHCMN: 'args[)]' -> 'digest[_]'.

## 2015-09-23:
- SCLXDHTML:
	- Prise en compte modification modules sous-jacents.
- XDH*:
	- Renommage des modules.
	- Prise en charge gestion 'iframe's

## 2015-09-21:
- XDHCBK, XDHJSP, 
	- Handling iframes through document with an id.
- XDHJSP:
	- Removing '(XDHJSP)HandleExtensions(...)' and '(XDHJSP)HandleCastings(...)'. Corresponding actions now made
	  when the document/frame and their casting are set.

## 2015-09-20
- XDHCBK, XDJ*:
	- More changes so what called 'frame' is now an 'iframe' and no more an element's child.
- SCLFRNTND, XDHCBK:
	- Moving 'mode__' from 'SCLFRNTND' to 'XDHCBK'.
- SCLXDHTML, XDHUPS:
	- Adaptation to changes in underlying modules.
- XDHDWS:
	- Adaptation to changes in underlying modules.

## 2015-09-18:
- XDHCBK:
	- Adding 'SetDocument' and 'SetFrame' as 'function'.
	- The 'SetChildren' 'function' do no more act as 'SetDocument' when 'Id' == NULL.
	- 'SetCasting' -> 'HandleCasting'.
	- The 'HandleCasting' 'function' applies to a document (the root one or the one corresponding to an iframe)
	  when 'Id' == NULL, or the 'iframe' of id 'Id' when != NULL.
- XDHJSP, XDHJSR, XDHJST:
	- Adaptation to changes on 'XDHCBK'.

## 2015-09-07:
- FRDBSE, SCLFRNTND:
	- Moved the content of 'FRDBSE' into 'SCLFRNTND'. 'FRDBSE' is no mode used.
- SCLXDHTML, XDHCBK, XDHDWS:
	- Adaptation to changes in 'FRDBSE'/'SCLFRNTND'.'

## 2015-09-16:
- FRDBS:
	- Adding 'Fill(...)' functions.
	- Adding 'Dump(...)' functions.
- SCLMISC:
	- Adding missing '&'.

## 2015-09-10:
- RGSTRY:
	- Adding some variants of 'tentry___::tentry___(...)' and 'tentry___::Init(...)' methods.

## 2015-09-09
- AEM, CDGB64, DTFPTB, FBLTYP:
	- Adaptation to changes in 'BSO'.
- AGS, BCH, FLSQ, FLX, IAS, MNS, TYS, UYS, XPP:
	- Adaptation to changes in 'SDR'.
- BOMHDL, CRPTGR, CSDSNC, CSDUCL, FBLBKD, FLW, IOF, UTF, XTF:
	- Adaptation to changes in 'FDR'.
- BSO:
	- 'raw__' becomes 'byte__' (in the meaning of 'unit of digital information').
- CIO, CSDLEC, CSDSNS, DTFBSC, FBLBRQ, FLX, SCK, STR, TXF, XML:
	- Adaptation to changes in 'FLW'.
- FRD, SDR:
	- Adaptation to changes in 'BSO'.
	- 'datum__' becomes 'byte__' (in the meaning of 'unit of digital information').
- PLGN:
	- Adding 'retriever___::Init()' with alternative parameters.
- RGSTRY:
	- Adding 'multi_level_registry_::Convert( const tentry__ &, entry__ &)'.

## 2015-09-06:
- CSDBNS, CSDSNS:
	- Adaptation to changes in 'CSDSCB'.
- CSDLEC:
	- Passes now the executable which embed the server as 'Origin' parameter to server.
- CSDSCB, FBLBKD, NTVSTR, SCLBACKND, SCLDAEMON:
	- '[Client]Origin' parameter is now of type 'ntvstr::char__'.
- CSDUCL:
	- 'universal_client_core' -> 'universal_client_core___'
	- Storing server location in 'universal_client_core___'.
- DIR:
	- Adding 'GetSelPath[_](...)' function.
	- Adapting existing functions to use above new function.
- FBLOVL:
	- Incrementing protocol version due to changes in 'STR'.
- SCLFRNTND:
	- Adaptation to changes in 'CSDUCL'.
	- Moving 'kernel___' from '.h' to '.cpp'.
	- Adding 'GetBackendLocation(...)' function.
- STR
	- The size of the string is embedded in the flow for the '<<' and '>>' operator.

## 2015-09-04:
- SCLRGSTRY:
	- Fixing default project entry value.

## 2015-08-27:
- XDHJST:
	- Adding key-related events handling.

## 2015-08-25:
- XDHJSP:
	- Adding missing 'Log' features.
- XDHJSR:
	- Fixing 'Log'-related features.
	- Adding key-related events handling.

## 2015-08-21:
- XDHJSP, XDHUPS,
	- Remplacing heading '_' by a tailing one.'

## 2015-08-18
- SCLXDHTML:
	- When no backend, subcomponent initialization are now still called.

## 2015-08-16
- CSDUCL:
	- Add missing parameter.
- XDHCBK:
	- 'downstream_callback__' '[...]New(...)' becomes'[...]RetrieveCallback(...)'.
	- Adding 'downstream_callback__::ReleaseCallback(...). So the 'delete' of an 'session_callback__' is made by the same module ('DLL', f. ex.) as its 'new'.
- SCLXDHTML, XDHUPS:
	- Adaptation to changes in 'XDHCBK'.

## 2015-08-04:
- CPE:
	- Fixing Cygwin-related stuff.
- ERR:
	- Removing deprecated 'qRPrm()'.

## 2015-08-04:
- XPP:
	- Use of the correct 'PreservationLevel_' variable to know if variable replacement should be skipped or not.

## 2015-08-02:
- STSFSM:
	- Add missing flush in 'Add(...)'.

## 2015-08-01:
- SCLRGSTRY, SCLRGSTRY:
	- 'OGetValue(...)' returns now a value which indicates if the entry exists or no (no more use of the 'Missing' parameter.)

## 2015-07-23:
- RGSTRY:
	- Adding 'entry___::(Init|entry___)( const entry___ &)'.

## 2015-07-21:
- CSDUCL:
	- Adaptation to changes to 'FRDBSE'.
	- '...Plugin...' -> '...Library...'.
- FRDBSE:
	- Adding 'None' type of backend, when no backend is used.
- SCLFRNTND:
	- Adaptation to changes in 'CSDUCL'.
- XDHCBK:
	- 'XDHCBKLaunch(...)' is now of type 'void' instead of 'bso::bool__'.
- XDHDWS:
	- Adaptation to changes in 'FRDBSE'.
- XDHUPS:
	- Adaptation to changes in 'XDHCBK'.

## 2015-07-18:
- XDHCBK, XDHJST, XDHDWS:
	- Moving 'GetDialogBasic()' related functionnalities from 'XDHDWS' to 'XDHCBK' to be available for 'XDHJST'.

## 2015-07-07:
- XDHJST:
	- Adding 'TagName' parameters to 'GetEventsAbstract()' function to properly handle the default action.

## 2015-07-15:
- NTVSTR:
	- The methods returning a 'const bso::char__ *' return now an empty string when core pointer == 'NULL'.

## 2015-07-10:
- XPP:
	- Subsitutions occurs now to the 'href' and 'select' attributes of the 'expand' directive , and the 'value' attribute of the 'set' directive too.

## 2015-07-09:
- BCH
	- Adding 'Push()' method.
	- Adding missing 'const' to 'Top()' method.
- XPP:
	- Adding substitution handling in attribute value.

## 2015-07-08:
- SCLXDHTML:
	- Adding 'SCLXDHTML_DEFAULT_SUFFIX'.

## 2015-07-03
- GLOBAL:
	- Fixing '_' beginning identifier issue.
	- Adaptating to changes in 'UYS'.

## 2015-07-02:
- UYS:
	- Adaptating to changes in 'TOL'.
- GLOBAL:
	- Fixing '_' beginning identifier issue.
	- Adaptating to changes in 'UYS'.
- TOL:
	- Inntroducing new extended_enum mechanism.
- XDHCBK:
	- Adding inclusion of missing <stdarg.h> header.
- XDHDWS:
	- Use of the thread-safe version of 'tol::DateAndTime(...)'.

## 2015-06-30
- BSO:
	- Fixing ambiguous 'Convert(...)' functions problem for POSIX but non-Darwin systems when compiling in 64 bits.

## 2015-06-28:
- TAGSBS:
	- Introducing 'tvalues(_)' and related functions.

## 2015-06-24:
- BSO:
	- Adding specific 'Convert(...)' function from '(unsigned) long', to resolve some ambiguity.
	- Removing some Darwin-related 'Convert(...)', which should be obsoleted by previous 'Convert(...)'.

## 2015-06-23:
- SCLTOOL:
	- 'ProjectFileName' ->'ProjectFilename'

## 2015-06-20:
- TOL
	- 'E_ROW(...)' -> 'qROW(...)'
- GLOBAL:
	- Adaptation to changes in 'TOL'.

## 2015-06-19:
- GLOBAL:
	- Adaptation to modifications in 'CPE'.
- MTX:
	- Rewritting to use the standard <atomic> library by default.

## 2015-06-18:
- GLOBAL:
	- Adaptation to modifications in 'CPE'.

## 2015-06-17:
- CPE:
	- Rewritting.
- GLOBAL:
	- Adaptation to modifications in 'CPE'.

## 2015-06-12:
- MTHTMC:
	- Fixing 'g++' complainings.

## 2015-06-11:
- MTHTMC:
	- Introducing 'tokens__'.

## 2015-06-10:
- SCLLOCALE:
	- A translation now ignores carriage returns, line feed and tabulations. They have to be explicitly set using '\n', '\t' and '\p' (padding, shorter as a tabulation.).
- SCLMISC:
	- Adding 'RefreshBaseLanguage().'
- SCLTOOL:
	- The base language is now updated with the registry content once the project/command-line arguments/setup-related data are uploaded in the registry.
	This allows to change the language with the '--#Language=...' option, i.e. without having to modifiy the configuration file.

## 2015-06-09:
- MTHTMC:
	- timecode with separators are now handled correct.

## 2015-06-08:
- GLOBAL: 
	- Adaptaiton to changes in miscellaneous libraries.
- AGS:
	- 'A_STORAGE()' -> 'qAS(_)'.
- ERR:
	- Renaming/removing/adding some macros.
- MTHMTC:
	- Fixing misssing '&'.
- SDR:
	- 'E_NIL' -> 'qNIL'.
	- 'E_SDRIVER' ->'qSD__'.

## 2015-06-07
- MTHITG, MTHMTC:
	- Updating.
- RGSTRY, SCLMISC, SCLRGSTRY,:
	- Adding the possibility to add a value (i.e. do not write out an existing value) to a registry.
- SCLTOOL:
	- With an 'Link' 'index' at "", it now handles undefined amount of arguments.

## 2015-05-29:
- SCLTOOL:
	- Simplification allowed by the fact that 'MinGW' is now called with unicode aware options.

## 2015-05-28 (til...):
- GLOBAL:
	- Heavy modifications, due to third-party components, with lot of tries, many of them were reverted, so it was useless and too much time concumming to report them here
	as I didn't know which of them will be conserved.

## 2015-05-11:
- GLOBAL:
	- Reorganization of the client/server modules and the ones which depends on them.
- XDH...
	- Reorganization of all the 'XDH'-related modules.

## 2015-05-09:
- XDH...
	- Reorganization of all the 'XDH'-related modules.

## 2015-05-07:
- XDHAGT:
	- Adaptation to changes in 'XDHCBK'.
- XDHCBK:
	- Adding 'upstream_callback__::(XDHCBK)New(...)' and 'upstream_callback__::(XDHCBK)Delete(...)' to handle user pointer.

## 2015-05-06
- CSDLEC:
	- Fixing bug when in remote mode, by using changes in 'CSDLEO'.
- CSDLEO:
	- Removing useless 'callback__::(CSDLEO)ReleaseCallback(...)', as releasing is do by calling the destructor of the concerned 'csdscb::callback__callback__'.
	- Removing useless 'Context' parameter of 'csdleo::callback__::(CSDLEO)RetrieveCallback(...)'.
	- Fixing bug when in remote mode, by adding 'data__::Mode', which should be 'mEmbedded', except when used by the 'eDeamon' (name will change soon) tool.
- SCLBACKND:
	- Adaptation to changes in 'SCLDAEMON'.
	-'SCLBACKNDNewCallback(...)' -> 'SCLBACKNDGetCallback(...)'.
- SCLDAEMON:
	- Adaptation to changes in 'CSDLEO'.
	-'SCLDAEMONNewCallback(...)' -> 'SCLDAEMONGetCallback(...)'.
- XDH...
	- Adaptation to changes in 'XDHCBK'.
- XDHCBK:
	- 'proxy_callback__' -> 'upstream_callback__'.
	- 'session_callback__' -> 'downtream_callback__'.
	- Merging 'callback__' with 'downstream_callback__'.
	- A 'session_callback__' was created per session. Now, there is only one callback ('dwonstream_callback__') for all session.
	  data owned by each session are passed through a user pointer.

## 2015-05-05:
- SCLXDHTML, XDHAGT, XDHPXY:
	- Adapting to changes in 'XDHCBK'.
- XDHCBK:
	- Append 'UP' ('User Pointer') parameter globally, which can be different for each 'session_callback__',
	so it is possible in 'proxy_callback__' to distinguish between each 'session_callback__'.
- XDHJSP:
	- Adapting to changes in 'XDHCBK'.
	- Fixing bug from alert/confirm dialog box.

## 2015-05-04:
- TOL:
	- Adding 'Q37_M(P|R)MDF(...)' macros.

## 2015-05-03:
- XDHCBK:
	- Adding 'Result' parameter to 'callback::XDHCBKProcess(...)'.

## 2015-05-01:
- DHTPROXY:
	- Removing useless 'proxy__::_ProxyCallback' and related members.
- TOL 
	- Adding 'Q37_(P|R)MDF'.
- XHTCLLBK:
	- Using 'Q37_(P|R)MDF'.
	- Removing useless 'proxy_callback__' parameter from 'callback__:(XHTCLLBK)New(...)' method.

## 2015-04-31:
- DHTJSBASED:
	- Creation.
- XHTAGENT, XHTCLLBK:
	- Removing useless 'ExecuteJavascript' and 'GetLanguage' methods.
	- Adding 'Log(...)' method.

## 2015-04-30:
- BTR, IDXBTR, IDXQUE, QUE:
	- Adding references to 'hook'-related items, for more coherency.
- CSDBNS:
	- Standardisation of the global constructor/destructor.
- DHTPROXY:
	- Creation.
- IDXBTQ:
	- Adaptation to the 'hook' system.
- SCLDAEMON:
	- Some changes to avoid MSVC to get confused.

## 2015-04-28:
- GLOBAL:
	- Fixing *clang* issues.

## 2015-04-24
- GLOBAL:
	- Rewritten global (con|de)structor using '__attribute__((((con|de)structor))' under Mac OS, as dynamic libraries under Mac OS
	  does not execute global (con|de)structor when the library is loaded.
- TOL:
    - Adding 'Q37_G(C|D)TOR' macro to facilitate above changes.

## 2015-04-22:
- FLW:
    - Adding 'Driver(..)' methods.
	- Some simplification.
- CIO:
    - Adding 'set__' object.
    - Adding 'Initialize(...)' function which takes a 'set__'.
    - '_S(In|Out|Err)OutDriver' are from now on private.
	- Standardization of the 'In' ,'Out', 'Err' enumeration order.
    - Adding functions to retrieve some predefined 'set__'s.
- CSDLEO:
	- Adding 'CIO' member in 'shared_data__'.
- PLGNCORE:
	- Adding 'CIO' member in 'data__'.
- SCLMISC:
	- Adding 'CIO' parameters to 'Initialize(...)' functions.
- SCLDAEMON, SCLPLUGIN, SCLTOOL, SCLXHTML:
	- Adaptation to changes in 'SCLMISC'.
- XHTCLLBK:
	- Adding `(_)CIO` member in `shared_data__`.

## 2015-04-21:
- SCLMISC:
	- No more 'SEGFAULT' on early errors (missing configuration file, for example).
- SCLXHTML:
	- Adaptation toi changes in 'XHTCLLBK'.
- XHTCLLBK:
	- Adding localization, where to search configuration ans locale file. Useful under Mac OS, where the executable is not necessary on the same place as the dynamic library.

## 2015-04-14:
- FDR:
    - Fixing 'Xcode' complainings.
- FLW:
	- Adaptation to changes in 'FRD'.
- SCLRGSTRY:
	- The location of the file given to 'LoadProject(...)' is now again used as base path for files included by the given project file.

## 2015-04-13:
- GLOBAL:
	- Fixing file encoding issues which made crash 'Xcode'. NOTA: all non-ASCII characters were stripped.

## 2015-04-12:
- DLBRRY:
	- When guessing library filename, 'lib' is now also prepend to library affix for 'MacOS'.
- RGSTRY:
    - Adding method in 'multi_level_registry_' to find an entry in a given level.
    - Removing 'PathErrorRow' where not useful.
- SCLMISC:
	- The embedded configuration, project,... corresponding locales are now directly pointing to the base registry,
	  and no more duplicated.
- SCLRGSTRY:
	- 'Locale' section is now root sections, and no more 'Definitions' sub-section.
    - Adding 'Arguments' entry.
- SCLTOOL:
	- Using 'Arguments' entry from 'sclrgstry'.
	- 'Arguments' section is now root section, and no more 'Definitions' sub-section.

## 2015-04-09:
- BCH, CTN, LSTBCH, LSTCTN, UYS:
	- Adaptation to changes in 'FLS'.
- DIR:
	- Old 'CPE__MAC' replaced by new 'CPE_XCODE'.
- DTFPTB:
	- Fixing 'Xcode' issues.
- FLS:
	- Renaming to 'FLSQ' due to conflict with 'Xcode' headers.
- STR, XPP:
	- Fixing some 'Xcode' issues.
- STRMRG:
	- Fixing bad parameter name issues.
- STRNG
	- Due to changes to 'FLS', removing the path for 'Xcode'.

## 2015-04-08:
- BSO:
	- The size of the types were incorrect under Posix in 64 bits.
	- Use of '[u]int[8|16|32|64]_t'
- DTE, TME:
	- Fixing some issues due to the changes in 'BSO'.
- DTFPTB:
	- Fixing issue due to changes in 'BSO'.

## 2015-04-07:
- DLBRRY:
	- For 'POSIX' and 'Mac OS', to mimic 'Windows' behavior, the correct prefix and extension is added to the library filename when missing.
- PLGN:
	- Adaptation to changes in 'PLGNCORE'.
- PLGNCORE, SCLPLUGIN:
	- Removing useless 'Directory' entry in 'data__' since the configuration and locale is retrieved from caller.
- RGSTRY:
	- An empty level is now valid and correctly handled.
- SCLFRNTND:
	- For the predefined items, their values are added in the XML dump.
- SCLRGSTRY:
	- The different levels are now created empty, and not embedded (except the arguments one).

## 2015-04-03:
- GLOBAL:
	- The content of configuration and locale registries are now passed to the plugin, and no more red from a file.
- FRDRGY:
	- Adaptation to changes in 'RGSTRY'.
- RGSTRY:
	- All 'Push...()' becomes 'Push()'.
	- 'Erase(...)' can now be called on a imported level.
	- 'entry__' becomes 'entry__', because of its use outside of its module.
	- Changing some method parameters so they use 'entry__'.
- SCLLOCALE, SCLRGSTRY:
	- Adaptation to changes in 'RGSTRY'.
	- Adding the 'Set...(...)' functions.
- SCLMISC:
	- Adding 'Initialize(...)' function with 'rgstry::registry_' parameters.

## 2015-03-30:
- FLX:
	- Removing the use of 'FLXUnavailable(...)' from 'buffer_iflow_driver___'. To detect classes which overloaded it,
	    the returning type is now 'int' instead of 'void'.
- TOL:
	- Removing 'Init()' in 'object_', to better take care to initialization of underlying object.
	- Propagating 'reset(...)' in 'object_' to underlying object.

## 2015-03-29:
- FRD:
	- Replacing destructor/constructor definition by 'E_CVDTOR'.

## 2015-03-26:
- DIR, FIL:
	- Fixing the 'MinGW' missing 'ELOOP' issue.
- NTVSTR:
	- Fixing the conversion from 'NULL' issue under Windows.
- TOL:
	- Restricting some stuff to 'MSVC', using libraries not available for 'MinGW'.

## 2015-03-21:
- CSDLEO:
	- Adding a '[CSDLEO]Initialize(...)' method to relieve the C API (for the same reason as below).
- PLGNCORE, PLNG, SCLPLUGIN
	- Because the C functions exposed by a dynamic library don't allow interruptions, to simplify error handling,
	  the API of a dynamic library will be exposed through a (C++) callback (except, of course, the function
	  which provides this callback).
- SCLDAEMON:
	- Adaptation to changes in 'CSDLEO'.


## 2015-03-20:
- SCLMISC, SCLRGSTRY:
	- Adding plugin features, as a standard way to retrieve a plugin reference from a configuration file.


## 2015-03-19
- XHTCLLBK:
	- Adding the label of the action corresponding to the closing of a program.
- SCLXHTML:
	- '[_]prelaunch_callback__' becomes '[_]action_helper_callback__'
	- On above call, adding '...OnClose(...)'.
	- '...Prelaunch(...)' becomes '...OnBeforeAction(...)'.


## 2015-03-16:
- CTN, IAS, LST, LSTBCH, LSTCTN:
	- Adaptation to changes in 'UYS'.
- CSDBNS:
	- Adaptation to changes in 'ERR'. Errors are no more intercepted, so the program is terminated when a error occurs.
	  Not really happy about this behavior, but the errors should be intercepted downstream.
- FLX:
	- In 'void _(i|o)flow_driver___', 'Commit()' and 'Dismiss()' are always allowed, as they are automatically called
	  when the object is deleted.
	- Use of 'access__' instead of a boolean.
- FRDKRN:
	- Adaptation to changes in 'CSDLEO'.
- SCLBACKND:
	- Adaptation to changes in 'FLX'.
- SCLRGSTRY:
	- 'GetUnsigneNumber(...)' returns now the correct value.
	- Modifying 'GetSigneNumber_(...)' to take account of changes in 'STR'.
- SCLTOOL:
	- Retrieving signal handling from 'TOL'.
- STR:
	- Some signed 'ToNumber' functions took limits parameters before the value to retrieve ; this is now modified to be
	    coherent with similar functions.
- SCLTOOL:
	- Adaptation to changes in 'FLX'.
- SCLXHTML:
	- Adaptation to changes to 'ERR'.
	- The errors are no more intercepted, but passed to the calling code.
	- Removing references to 'CIO'.
- TOL:
	- Moving signal handling to 'SCLTOOL'.
- UYS:
	- Introducing 'mode__' to use instead 'fil::mode__'.
	- Introducing 'behavior' to use instead of a boolean.


## 2015-03-15:
- CSDLES:
	- Adaptation to changes in 'ERR'.
- ERR:
	- Removing all references to 'CIO'. The final error handling had to be handled upstream (through the modified 'ERRFEpilog()' macro).
- MTK:
	- A uncaught exception will now terminate the program. All exceptions shoud be caught dowstream.
- SCLDAEMON:
	- Removing 'DisplayModuleClos(ing|ed)Message()' functions, to remove reference to 'CIO' (unavailable when the daemon is embedded).
- SCLMISC:
	- Removing all dependences to 'CIO'.
	- Hence, moving 'text_oflow_rack___' to 'SCLTOOL'.
- SCLTOOL:
	- Due to changes in 'ERR', the final error message printing is now supplied by this module.
	- As one of the few (or the only one) to use the 'CIO' module, retrieving 'text_oflow_rack___' from 'SCLMISC'.
	- Removing forgotten bug tracking feature.


## 2015-03-14:
- CSDLEO:
	- No more 'C(Out|Err)' in the shared data, as the error handling object are now shared with the dynamic libraries.
- CSDLES:
	- Adaptation to changes to 'CSDLEO'.
	- The errors are no more intercepted, but passed from the dynamic library to the calling code.
- FLX:
	- For the 'void' drivers, by default (can be changed on initialization), an access to it launches an error.


## 2015-03-13:
- GLOBAL:
	- Adaptation to changes in 'UYS'.
- FNM:
	- Handling the 'NULL' case fot 'Type_(...)' function.'
- NTVSTR:
	- Change made the 11th active only for Windows, as g++ complains about.
- TOL:
	- Introducing 'E_XENUM', which simplifies error handling with 'enum's.
- UYS:
	- Using the new 'E_XENUM' from 'TOL'.
	- Renaming 'file_manager___' to 'file_hook___'
	- Introducing 'hook_filenames___'.


## 2015-03-12:
- SCLPLUGIN
	- Removing test feature.


## 2015-03-11:
- GLOBAL:
	- Généralization of the use of the terms:
		- 'path' for, in a generic way, a file or a directory.
		- 'directory' ('dir' in short) for a directory,
		- 'filename' for a file name, with or without its location,
		- 'basename' for a file name without its location, but with its extension,
		- 'location' for the location (the list of the parent directories) of a file,
		- 'affix' the 'basename' of a file without its extension.
- NTVSTR:
	- Adding method for int, to avoid to have to cast NULL (defined as '0') into '(const ntvstr::char__ *)'.
		
## Notes

[adpt]:#Adaptation
[stdz]:#Standardization
  
### Adaptation

Adaptation to changes in underlying modules. 

### Standardization

Modification to comply with current standards.