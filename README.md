# PhyDB
[![CircleCI](https://circleci.com/gh/asyncvlsi/phyDB.svg?style=svg)](https://circleci.com/gh/asyncvlsi/phyDB)

### Recommended toolchain
* Ubuntu 20.04
* CMake, version >= 3.16.3
* GNU Compiler Collection (GCC), version >= 9.3.0

### Pre-requisite libraries
* Boost, version >= 1.71.0
* Si2 LEF/DEF parser, a mirror can be found [here](https://github.com/asyncvlsi/lefdef).
* [ACT](https://github.com/asyncvlsi/act): environment variable `ACT_HOME` determines the installation destination of this package, and contains some optional libraries.

### Clone, compile, and install
    $ git clone https://github.com/asyncvlsi/phyDB.git
    $ cd phyDB/
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ make install
this will create a folder `phydb` under folder `$ACT_HOME/include`, and a static library `libphydb.a` under folder `$ACT_HOME/lib`.
