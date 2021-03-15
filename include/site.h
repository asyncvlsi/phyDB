#ifndef SITE_H
#define SITE_H

#include "header.h"

namespace phydb {

class Site {
    private:
        string _name;
        string _className;
        float _width;
        float _height;
        
    public:
        Site( ): _name(""), _className(""), _width(0), _height(0) { }
        Site(string name, string className, float width, float height):
            _name(name), _className(className), _width(width), _height(height) { }

        void setName(string );
        void setClassName(string );
        void setWidth(float );
        void setHeight(float );

        string getName( ) const;
        string getClassName( ) const;
        float getWidth( ) const;
        float getHeight( ) const;

};

ostream& operator<< (ostream& , const Site& );

}



#endif 
