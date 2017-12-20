# *JREq* changelog

2017-12-20:
- the wrapper can now be loaded more than once; two *Java* component based on this wrapper can now be used together,

2017-11-29:
- wrapped function can now return `NULL` when no return value are awaited, instead of `scljre::Null()` (which was actually buggy, but is now fixed),
- adding the handling of the *Short* *Java* type,

2017-10-27:
- adaptation to changes to *N4ALLW*,

2017-10-10:
- using new `n4allw::Launch(...)` function.






