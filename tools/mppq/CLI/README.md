# mppq-cli

## Presentation

A *markdown* presentation processor with command-line interface.

Intended to be used in conjunction with *[Marp](https://q37.info/s/w/marp)*.

Currently, generates a new *markdown* file from an existing one with, for each slide containing a list, a slide with none of the list items, then one with only the first, then another with the 2 firsts and so on. So, during the presentation, the items of a list will appear one after the other.

Other features will be provided in the future.

You may be interested into some informations about the [configuration](http://q37.info/s/c/conf/]).

<!--Go to [http://q37.info/tools/mppq/](http://q37.info/s/t/mppq/) for more information.-->

## Running

Retrieve this repository. Under *Windows*, retrieve the wanted binary under the *release* section and put them at the root of the repository. For *POSIX* system (*Cygwin*, *GNU/Linux*, *macOS*...), launch `make` (both `make` command and a *C++* compiler must be installed).

Launch `mppq --help` at the root of the repository to see how it works.

## Status
*GNU/Linux* & *OS X* : [![Travis CI](https://travis-ci.org/epeios-q37/mppq-cli.png)](https://travis-ci.org/epeios-q37/mppq-cli)
 
*Windows* : [![AppVeyor](http://ci.appveyor.com/api/projects/status/github/epeios-q37/mppq-cli)](http://ci.appveyor.com/project/epeios-q37/mppq-cli)

