#ifndef PHYDB_H
#define PHYDB_H

namespace phydb {

class PhyDB {
    public:
        Tech _tech;
        Design _design;


        PhyDB( ) { }

        void readLef(string );
        void readDef(string );

}



}


#endif
