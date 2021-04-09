# PhyDB

### Build tools
* CMake, version >= 3.16
* GNU Compiler Collection (GCC), version >= 4.8.5

### Pre-requisite libraries
* Boost, version >= 1.61.0
* Si2 LEF/DEF parser: [LEF parser](https://github.com/The-OpenROAD-Project/lef) and [DEF parser](https://github.com/The-OpenROAD-Project/def). Use `cmake .. -DCMAKE_INSTALL_PREFIX=path/to/install` to specify the installation destination for each parser, and export the path as environment variable `LEF_ROOT` and `DEF_ROOT`. This LEF/DEF parser requires BISON (>= 3.0.4)
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

### Notice
* The LEF parser has some memory leak issues. This LEF parser is modified version of the original LEF parser from Si2, and thus some bugs may also be introduced during this process.
