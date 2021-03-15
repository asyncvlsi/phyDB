#include "tech.h"


namespace phydb {

int Tech::addMacro(Macro m) {
    int id = _macros.size();
    //m._id = id;
    _macros.push_back(m);
    string name = m.getName();
    _macro2id[name] = id;
    return id;
}

Macro& Tech::getMacro(string name) {
    int id = _macro2id[name];

    return _macros[id];
}


}

