#ifndef SNET_H
#define SNET_H

#include "header.h"

namespace phydb {

class SNet {
    public:
        string _name;
    
        SNet( ): _name(""), _className(""), _width(0), _height(0) { }
        SNet(string name):_name(name) { }

        void setName(string );

        string getName( ) const;
};

ostream& operator<< (ostream& , const SNet& );

}



#endif 
