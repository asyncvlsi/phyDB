#include "DataType.h"
#include "spacingtable.h"
#include "layer.h"
#include "phydb.h"
#include "site.h"
#include "tech.h"

using namespace phydb;


void test_datatype( ) {

    cout << "this is test_datatype!"<< endl;

    //Point2D
        Point2D<int> a1;
        assert(a1.IsEmpty());
        a1.x = 1;
        a1.y = 2;
        Point2D<int> b1(3, 0), c1;
    c1.Set(2, 6);
        assert(a1 > b1 && b1 < c1);
    c1.Reset();
        assert(c1.IsEmpty());
        cout << b1 << endl;
        cout << "Point2D test passes!" << endl;
    //Point3D
    {
        Point3D<double> a2;
        assert(a2.IsEmpty());
        a2.x = 1.0;
        a2.y = 2.0;
        a2.z = 3.0;
        Point3D<double> b2(3.0, 0.0, 2.0), c2;
        c2.Set(2.0, 1.5, 3.0);
        assert(a2 > b2 && a2 > c2);
        c2.Reset();
        assert(c2.IsEmpty());
        cout << b2 << endl;
        cout << "Point3D test passes!" << endl;
    }
    //Rect2D 
    {
        Rect2D<double> a3;
        a3.Set(1.0, 2.0, 3.0, 4.0);
        Point2D<double> p3(1.0, 4.0);
        Point2D<double> q3(1.5, 2.5);
        assert(!a3.BoundaryExclusiveCover(p3) && a3.Cover(p3));
        assert(a3.BoundaryExclusiveCover(q3));
        cout << a3 << endl;
        a3.ur.x = 0.5;
        assert(!a3.IsLegal());
        cout << "Rect2D test passes!" << endl;
    }
    //Rect3D
    {
        Rect3D<double> a4;
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
    Rect2D<double> rect1(1.0, 2.0, 3.0, 4.0);
    Rect2D<double> rect2(10.0, 20.0, 30.0, 40.0);

    vector<Rect2D<double>> v;
    v.push_back(rect1);
    v.push_back(rect2);
    LayerRect lr(layerName, v);
    cout << lr << endl;
    cout << "LayerRect passes!" << endl;

}

void test_spacingtable( ) {
    int nCol = 5;
    int nRow = 6;

    vector<double> p;
    vector<double> w;
    vector<double> s;

    for(int i = 0; i < nCol; i++)
        p.push_back((double)i);
    for(int j = 0; j < nRow; j++) 
        w.push_back((double) (j + nCol));
    for(int i = 0; i < nRow * nCol; i++) {
        double f = (double) i;
        s.push_back(f / 10.0);
    }

    SpacingTable st(nCol, nRow, p, w, s);
    cout << st << endl;
    cout << "here?" << endl;

    st.SetSpacingAt(4, 3, 100.0);
    cout << st << endl;
}

void test_layer( ) {
    Layer l("metal1", ROUTING, VERTICAL, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
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
    Macro macro1("testMacro", Point2D<double>(1.0, 2.0), Point2D<double>(3.0, 4.0), pins, obs);

    std::string macro_name = "testMacro";
    tech.AddMacro(macro_name);
    auto *macro2 = tech.GetMacroPtr("testMacro");

    cout << macro2->GetOrigin() << endl;
    macro2->GetOriginRef().x = 2.0;
    cout << macro2->GetOriginRef() << endl;
    

}

int main() {
    cout << "this is a test!"<< endl;
    //
    //test_datatype( );
    
    //test_spacingtable();
    
    //test_layer();
    
    //test_site();
    
    //test_tech( );

    PhyDB db;

    //string clusterFileName = "circuit_router.cluster";
    //db.ReadCluster(clusterFileName);

    string lefFileName = "processor.lef";
    db.ReadLef(lefFileName);
    cout << "lef reading done" << endl;

    string defFileName = "pwroute.def";
    db.ReadDef(defFileName);
    cout << "def reading done" << endl;

    string cellFileName = "processor.cell";
    db.ReadCell(cellFileName);

    //db.GetTechPtr()->Report();
    db.GetDesignPtr()->Report();
    
    return 0;
}
