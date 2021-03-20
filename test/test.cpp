#include "DataType.h"
#include "spacingtable.h"
#include "layer.h"
#include "site.h"
#include "tech.h"

using namespace phydb;


void test_datatype( ) {

    cout << "this is test_datatype!"<< endl;

    //Point2D
        Point2D<int> a1;
        assert(a1.isEmpty());
        a1.x = 1;
        a1.y = 2;
        Point2D<int> b1(3, 0), c1;
        c1.set(2, 6);
        assert(a1 > b1 && b1 < c1);
        c1.reset( );
        assert(c1.isEmpty( ));
        cout << b1 << endl;
        cout << "Point2D test passes!" << endl;
    //Point3D
    {
        Point3D<float> a2;
        assert(a2.isEmpty());
        a2.x = 1.0;
        a2.y = 2.0;
        a2.z = 3.0;
        Point3D<float> b2(3.0, 0.0, 2.0), c2;
        c2.set(2.0, 1.5, 3.0);
        assert(a2 > b2 && a2 > c2);
        c2.reset( );
        assert(c2.isEmpty( ));
        cout << b2 << endl;
        cout << "Point3D test passes!" << endl;
    }
    //Rect2D 
    {
        Rect2D<float> a3;
        a3.set(1.0, 2.0, 3.0, 4.0);
        Point2D<float> p3(1.0, 4.0);
        Point2D<float> q3(1.5, 2.5);
        assert(!a3.boundaryExclusiveCover(p3) && a3.cover(p3));
        assert(a3.boundaryExclusiveCover(q3));
        cout << a3 << endl;
        a3.ur.x = 0.5;
        assert(!a3.isLegal());
        cout << "Rect2D test passes!" << endl;
    }
    //Rect3D
    {
        Rect3D<float> a4;
        a4.set(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
        cout << a4 << endl;

        cout << "Rect3D test passes!" << endl;
    }
    //Range
    Range<int> a5(1, 5);
    cout << a5 << endl;
    cout << "Range test passes!" << endl;

    //LayerRect
    string layerName = "Metal1";
    Rect2D<float> rect1(1.0, 2.0, 3.0, 4.0);
    Rect2D<float> rect2(10.0, 20.0, 30.0, 40.0);

    vector<Rect2D<float>> v;
    v.push_back(rect1);
    v.push_back(rect2);
    LayerRect lr(layerName, v);
    cout << lr << endl;
    cout << "LayerRect passes!" << endl;

}

void test_spacingtable( ) {
    int nCol = 5;
    int nRow = 6;

    vector<float> p;
    vector<float> w;
    vector<float> s;

    for(int i = 0; i < nCol; i++)
        p.push_back((float)i);
    for(int j = 0; j < nRow; j++) 
        w.push_back((float) (j + nCol));
    for(int i = 0; i < nRow * nCol; i++) {
        float f = (float) i;
        s.push_back(f / 10.0);
    }

    SpacingTable st(nCol, nRow, p, w, s);
    cout << st << endl;
    cout << "here?" << endl;

    st.setSpacingAt(4, 3, 100.0);
    cout << st << endl;
}

void test_layer( ) {
    Layer l("metal1", "ROUTING", "VERTICAL", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
    cout << l << endl;

}

void test_site( ) {
    Site s("AAA", "BBB", 0.1, 0.2);
    cout << s << endl;

}

void test_tech( ) {
    
    Tech tech;
    vector<Pin> pins;
    OBS obs;
    Macro macro1("testMacro", Point2D<float>(1.0, 2.0), Point2D<float>(3.0, 4.0), pins, obs);

    tech.addMacro(macro1);
    auto& macro2 = tech.getMacro("testMacro");

    cout << macro2.getOrigin() << endl;
    macro2.getOriginRef().x = 2.0;
    cout << macro2.getOriginRef() << endl;
    

}

int main() {
    cout << "this is a test!"<< endl;
    //
    //test_datatype( );
    
    //test_spacingtable();
    
    //test_layer();
    
    //test_site();
    
    //test_tech( );
    
    string lefFileName, defFileName;
    PhyDB db;
    db.readlef(string lefFileName);
    db.readdef(string defFileName);
    
    return 1;
}
