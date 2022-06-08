# *XPPq* changelog

## 2022-06-08
- adaptation to *FreeBSD*;

## 2022-02-08
- adaptation to changes in undelying modules,

## 2019-09-18:
- new `xpp:marker` attribute, which allows to define the marker for variable substitution without having to put an extraneous `<xpp:bloc marker="…">…`.

## 2018-12-03:
- attribute delimiters are now preserved and no more systematically replaced with `"`,

## 2018-12-01:
- space-like characters (`\r`, `\n`, `\t`...) are now allowed in attribute values,
- fixing bad indentation,

## 2018-02-01:
- *xppqjre*:
  - adaptation to changes in underlying libraries,

## 2018-01-06:
- *xppqnjs*:
  - adaptation to modification of used components,

## 2017-12-19:
- *xppqnjs*:
  - adaptation to modification of used components,

## 2017-10-29:
- *xppqjre*:
  - adaptation to changes in *SCLJRE*,

## 2017-09-26:
- *xppqnjs*:
  - adding missing deletion of the callback given to the parser,

## 2017-09-17:
- *xppqnjs*
  - upgrading,

## 2017-09-15:
- *xppqnjs*
    - adaptation to changes in underlying libraries,

## 2017-09-13:
- *xppqnjs*
    - adding file for *Runkit* (`runkit.js`),

## 2017-09-12:
- *xppqnjs*
  - simplification in the hope it will help to fix some bugs,
  - workaround for some bugs, because there are some incoherencies in *Node.js* (see comments in source files),
  - fixing bad translation of error messages,

## 2017-09-08:
- *xppqnjs*
  - adding `DONE` tag, corresponding to the reaching the end of the stream,
  - fixing bad `stream.Readable.readable` event handling,

## 2017-08-31:
- *XPPq* is now available as *Node.js* *addon*,

## 2016-09-30:
- *AArch64* (*ARM* 64-bit) version available,

## 2015-10-11:
- `fdr::datum__` -> `fdr::byte__`,

## 2015-06-04:
- no changes in the software itself, but only in the content of the packages,

## 2015-05-28:
- `locale.(h|cpp)` becomes `i18n(.h|.cpp)` to avoid confusion with the `locale.h` standard *C* header,

## 2015-05-27
- creation, as replacement of *expp*.
