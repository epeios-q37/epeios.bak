# *XDHELCq* *CHANGELOG*

## 2018-04-16

- adaptation to changes in underlying modules,

## 2018-03-22

- handling the fact that the dynamic libraries are not prefixed with `lib` under *POSIX* systems, because they are compiled using *node-(pre)-gyp*,

## 2018-03-16

- forcing the version of *node-pre-gyp* to `0.8.x`, as the latest `0.9.0` does not properly handle the `302`*http* response from *GitHub*,

## 2018-03-08

- using `IsNull()` and `IsUndefined()` functions instead of more recent `IsUndefinedOrNull()` function,

## 2018-02-19

- modification so that the initial action on new session is now explicitly launched.