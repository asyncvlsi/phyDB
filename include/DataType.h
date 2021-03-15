#ifndef DATATYPES_H
#define DATATYPES_H

#include "header.h"

namespace phydb {

template <typename T>
class Point2D {
    public: 
        T x;
        T y;
 
        Point2D( ): x(0), y(0) { }
        Point2D(T xx, T yy): x(xx), y(yy) { }
        void set(T xx, T yy) {
            x = xx;
            y = yy;
        }
        void reset( ) {
            x = 0;
            y = 0;
        }
        bool isEmpty() {
            return (x == 0 && y == 0);
        }
        //define '<' and '>' for comparison in std set and map
        bool operator< (const Point2D<T> p) const {
            if(y < p.y) return true;
            else if(y > p.y) return false;
            else return (x < p.x);
        }
        bool operator> (const Point2D<T> p) const {
            if(y > p.y) return true;
            else if(y < p.y) return false;
            else return (x > p.x);
        }
};

template <typename T>
using Size2D = Point2D<T>;

template <typename T>
class Point3D {
    public: 
        T x;
        T y;
        T z;
        Point3D( ): x(0), y(0), z(0) { }
        Point3D(T xx, T yy, T zz): x(xx), y(yy), z(zz) { }
        void set(T xx, T yy, T zz) {
            x = xx;
            y = yy;
            z = zz;
        }
        void reset( ) {
            x = 0;
            y = 0;
            z = 0;
        }
        bool isEmpty( ) {
            return (x == 0 && y == 0 && z == 0);
        }

        //define '<' and '>' for comparison in set amd map
        bool operator< (const Point3D<T> p) const {
            if(z < p.z) return true;
            else if(z > p.z) return false;
            else if(y < p.y) return true;
            else if(y > p.y) return false;
            else return (x < p.x);
        }
        bool operator> (const Point3D<T> p) const {
            if(z > p.z) return true;
            else if(z < p.z) return false;
            else if(y > p.y) return true;
            else if(y < p.y) return false;
            else return (x > p.x);
        }
};

template <typename T>
class Rect2D {
    public: 
        Point2D<T> ll; //lower left
        Point2D<T> ur; //upper right
        
        Rect2D( ): ll(0, 0), ur(0, 0) { }
        Rect2D(Point2D<T> LL, Point2D<T> UR): ll(LL), ur(UR) {
            assert(this->isLegal());
        }
        Rect2D(T llx, T lly, T urx, T ury): ll(llx, lly), ur(urx, ury) {
            assert(this->isLegal());
        }
        bool isLegal( ) {
            return (ll.x < ur.x && ll.y < ur.y);
        }
        void set(Point2D<T> LL, Point2D<T> UR) {
            ll = LL;
            ur = UR;
            assert(this->isLegal());
        }
        void set(T llx, T lly, T urx, T ury) {
            ll.x = llx;
            ll.y = lly;
            ur.x = urx;
            ur.y = ury;
            assert(this->isLegal());
        }
        bool isEmpty( ) {
            return ll.isEmpty( ) && ur.isEmpty( );
        }
        
        bool boundaryExclusiveCover(Point2D<T> p) {
            return (ll.x < p.x && ll.y < p.y && ur.x > p.x && ur.y > p.y);
        }
        bool cover(Point2D<T> p) {
            return (ll.x <= p.x && ll.y <= p.y && ur.x >= p.x && ur.y >= p.y);
        }

};

template <typename T>
class Rect3D {
    public:
        Point3D<T> ll;
        Point3D<T> ur;
        
        Rect3D( ): ll(0, 0, 0), ur(0, 0, 0) { }
        Rect3D(Point3D<T> LL, Point3D<T> UR): ll(LL), ur(UR) { }
        Rect3D(T llx, T lly, T llz, T urx, T ury, T urz): ll(llx, lly, llz), ur(urx, ury, urz) { }
       
        bool isLegal( ) { //on the same layer is legal
            return (ll.x < ur.x && ll.y < ur.y && ll.z <= ur.z);
        }
        void set(Point3D<T> LL, Point3D<T> UR) {
            ll = LL;
            ur = UR;
            assert(this->isLegal());
        }
        void set(T llx, T lly, T llz, T urx, T ury, T urz) {
            ll.set(llx, lly, llz);
            ur.set(urx, ury, urz);
            assert(this->isLegal());
        }
};

template <typename T>
class Range {
public:
    T begin;
    T end;
    
    Range( ): begin(0), end(0) { }
    Range(T b, T e): begin(b), end(e) {
        assert(b < e);
    }
};

class LayerRect {
public: 
    string layerName;
    vector<Rect2D<float>> rects;
    
    LayerRect( ): layerName("") { }
    LayerRect(string layerName, vector<Rect2D<float>> rects): layerName(layerName), rects(rects) { }
};


template <typename T>
ostream& operator<< (ostream& os, const Point2D<T>& p) {
    os << "(" << p.x << ", " << p.y << ") ";
    return os;
}

template <typename T>
ostream& operator<< (ostream& os, const Point3D<T>& p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ") ";
    return os;
}

template <typename T>
ostream& operator<< (ostream& os, const Rect2D<T>& p) {
    os << "Rect: " << p.ll  <<  " " << p.ur << " ";
    return os;
}

template <typename T>
ostream& operator<< (ostream& os, const Rect3D<T>& p) {
    os << "Rect: " << p.ll  <<  " " << p.ur << " ";
    return os;
}

template <typename T>
ostream& operator<< (ostream& os, const Range<T>& r) {
    os << "Range [" << r.begin << ", " << r.end << "] ";
    return os;
}

ostream& operator<< (ostream& os, const LayerRect& lr);


} //end of namespace phydb

#endif


