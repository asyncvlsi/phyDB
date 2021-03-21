#ifndef SNET_H
#define SNET_H

#include "header.h"

namespace phydb {


class Path {
    public:
        string layerName;
        int width;
        string shape;
        string viaName;

        int beginExt;
        int endExt;
        Rect2D<int> rect;
        Point2D<int> begin;
        Point2D<int> end;
        
        Path( )  {
            layerName = "";
            width = 0;
            shape = "";
            viaName = "";
            beginExt = 0;
            endExt = 0;
        }
        void Report() {
            cout << " NEW " << layerName << " " << width << " + SHAPE " << shape;
            if(!rect.empty())
                cout << " (" << rect.lowerLeft.x << " " << rect.lowerLeft.y << " " << rect.upperRight.x << " " << rect.upperRight.y << ")";
            if(!begin.empty())
                cout << " (" << begin.x << " " << begin.y << " ) ";
            if(!end.empty())
                cout << " (" << end.x << " " << end.y << " ) ";
            if(beginExt != 0 || endExt != 0)
                cout << " EXT " << beginExt << " " << endExt;
            if(viaName != "")
                cout << viaName;
            cout << endl;
        }
};

class SNet {
    public:
        string _name;
        vector<Path> _paths;

        SNet( ): { }

        void setName(string );

        string getName( ) const;
};

ostream& operator<< (ostream& , const SNet& );

}



#endif 
