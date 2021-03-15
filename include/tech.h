#ifndef TECH_H
#define TECH_H

#include "layer.h"
#include "site.h"
#include "macro.h"
namespace phydb {

class Tech {
    private:
        string _version;
        string _busBitChar;
        string _divierChar;
        string _clearanceMeasure;
        double _manufacturingGrid;

        int _dbuPerMicron;

        vector<Site> _sites;
        vector<Layer> _layers;
        vector<Macro> _macros;

        map<string, int> _layer2id;
        map<string, int> _macro2id;

    public:
        Tech( ): _manufacturingGrid(0), _dbuPerMicron(0) { }


        int addSite(Site );
        int addLayer(Layer );
        int addMacro(Macro );

        Macro& getMacro(string name);
        Macro& getMacro(int id);

        friend ostream& operator<< (ostream& , const Tech& );
        

};

ostream& operator<< (ostream& , const Tech& );

}

#endif

