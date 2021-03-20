#ifndef NET_H
#define NET_H

#include "header.h"

namespace phydb {

class Net {
    public: 
        string name;
        string source;
        string use;
        
        vector<string> componentNames;
        vector<string> pinNames;
        
        Net() { }
        Net(string name, string source, string use, direction, int start, int numTracks, int step):
            _direction(direction),
            _start(start),
            _numTracks(numTracks),
            _step(step)
            { }

};

ostream& operator<< (ostream& , const Track &);


}


#endif
