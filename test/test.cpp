/*******************************************************************************
 *
 * Copyright (c) 2021 Jiayuan He, Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/

#include "datatype.h"
#include "spacingtable.h"
#include "layer.h"
#include "phydb.h"
#include "site.h"
#include "tech.h"

using namespace phydb;

void test_datatype() {
    std::cout << "this is test_datatype!" << std::endl;

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
    std::cout << b1 << std::endl;
    std::cout << "Point2D test passes!" << std::endl;
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
        std::cout << b2 << std::endl;
        std::cout << "Point3D test passes!" << std::endl;
    }
    //Rect2D 
    {
        Rect2D<double> a3;
        a3.Set(1.0, 2.0, 3.0, 4.0);
        Point2D<double> p3(1.0, 4.0);
        Point2D<double> q3(1.5, 2.5);
        assert(!a3.BoundaryExclusiveCover(p3) && a3.Cover(p3));
        assert(a3.BoundaryExclusiveCover(q3));
        std::cout << a3 << std::endl;
        a3.ur.x = 0.5;
        assert(!a3.IsLegal());
        std::cout << "Rect2D test passes!" << std::endl;
    }
    //Rect3D
    {
        Rect3D<double> a4;
        a4.set(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
        std::cout << a4 << std::endl;

        std::cout << "Rect3D test passes!" << std::endl;
    }
    //Range
    Range<int> a5(1, 5);
    std::cout << a5 << std::endl;
    std::cout << "Range test passes!" << std::endl;

    //LayerRect
    std::string layerName = "Metal1";
    Rect2D<double> rect1(1.0, 2.0, 3.0, 4.0);
    Rect2D<double> rect2(10.0, 20.0, 30.0, 40.0);

    std::vector<Rect2D<double>> v;
    v.push_back(rect1);
    v.push_back(rect2);
    LayerRect lr(layerName, v);
    std::cout << lr << std::endl;
    std::cout << "LayerRect passes!" << std::endl;

}

void test_spacingtable() {
    int nCol = 5;
    int nRow = 6;

    std::vector<double> p;
    std::vector<double> w;
    std::vector<double> s;

    for (int i = 0; i < nCol; i++)
        p.push_back((double) i);
    for (int j = 0; j < nRow; j++)
        w.push_back((double) (j + nCol));
    for (int i = 0; i < nRow * nCol; i++) {
        double f = (double) i;
        s.push_back(f / 10.0);
    }

    SpacingTable st(nCol, nRow, p, w, s);
    std::cout << st << std::endl;
    std::cout << "here?" << std::endl;

    st.SetSpacingAt(4, 3, 100.0);
    std::cout << st << std::endl;
}

void test_layer() {
    Layer l("metal1", ROUTING, VERTICAL, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6);
    std::cout << l << std::endl;

}

void test_site() {
    Site s("AAA", "BBB", 0.1, 0.2);
    std::cout << s << std::endl;

}

void test_tech() {

    Tech tech;
    std::vector<Pin> pins;
    OBS obs;
    Macro macro1("testMacro",
                 Point2D<double>(1.0, 2.0),
                 Point2D<double>(3.0, 4.0),
                 pins,
                 obs);

    std::string macro_name = "testMacro";
    tech.AddMacro(macro_name);
    auto *macro2 = tech.GetMacroPtr("testMacro");

    std::cout << macro2->GetOrigin() << std::endl;
    macro2->GetOriginRef().x = 2.0;
    std::cout << macro2->GetOriginRef() << std::endl;

}

int main() {
    std::cout << "this is a test!" << std::endl;
    //
    //test_datatype( );

    //test_spacingtable();

    //test_layer();

    //test_site();

    //test_tech( );

    PhyDB db;

    std::string clusterFileName = "circuit_router.cluster";
    db.ReadCluster(clusterFileName);

    std::string lefFileName = "processor.lef";
    db.ReadLef(lefFileName);
    std::cout << "lef reading done" << std::endl;

    std::string defFileName = "pwroute.def";
    db.ReadDef(defFileName);
    std::cout << "def reading done" << std::endl;

    std::string cellFileName = "processor.cell";
    db.ReadCell(cellFileName);

    //db.GetTechPtr()->Report();
    //db.GetDesignPtr()->Report();
    std::string write_def_file_name = "new.def";
    db.WriteDef(write_def_file_name);

    //string write_cluster_file_name = "new.cluster";
    //db.WriteCluster(write_cluster_file_name);

    return 0;
}
