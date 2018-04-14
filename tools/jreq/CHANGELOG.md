# *JREq* changelog

## 2018-04-14
- adaptation to changes in underlying modules,

## 2018-03-26
- displayed *JNI* version was false,

## 2018-01-15
- adding handling of booleans,

## 2017-12-20
- the wrapper can now be loaded more than once; two *Java* component based on this wrapper can now be used together,

## 2017-11-29
- wrapped function can now return `NULL` when no return value are awaited, instead of `scljre::Null()` (which was actually buggy, but is now fixed),
- adding the handling of the *Short* *Java* type,

## 2017-10-27
- adaptation to changes to *N4ALLW*,

## 2017-10-10
- using new `n4allw::Launch(...)` function.
