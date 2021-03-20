#ifndef ROW_H
#define ROW_H

#include "header.h"

namespace phydb {

class Row {
    public: 
        string _name;
        string _siteName;
        string _siteOrient;
        int _origX;
        int _origY;
        int _numX;
        int _numY;
        int _stepX;
        int _stepY;
    
        Row() { }
        Row(string name, string siteName, string siteOrient,
                int origX, int origY, int numX, int numY, int stepX, int stepY):
            _name(name),
            _siteName(siteName),
            _siteOrient(siteOrient),
            _origX(origX),
            _origY(origY),
            _numX(numX),
            _numY(numY),
            _stepX(stepX),
            _stepY(stepY)
            { }

};

ostream& operator<< (ostream& , const Row &);


}


#endif

