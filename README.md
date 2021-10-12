# PhyDB

### Build tools
* CMake, version >= 3.16
* GNU Compiler Collection (GCC), version >= 4.8.5

### Pre-requisite libraries
* Boost, version >= 1.61.0
* Bison, version >= 3.2.0
* Flex, version >= 2.6.4 (lower versions may also work, not tested)
* Si2 LEF/DEF parser, a mirror can be found [here](https://github.com/asyncvlsi/lefdef).
* [ACT](https://github.com/asyncvlsi/act): environment variable `ACT_HOME` determines the installation destination of this package

### Clone, compile, and install
    $ git clone https://github.com/asyncvlsi/phyDB.git
    $ cd phyDB/
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ make install
this will create a folder `phydb` under folder `$ACT_HOME/include`, and a static library `libphydb.a` under folder `$ACT_HOME/lib`.
