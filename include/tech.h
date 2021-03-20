#ifndef TECH_H
#define TECH_H

#include <lefrReader.hpp>

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


        Site& addSite(Site );
        Layer& addLayer(Layer );
        Macro& addMacro(Macro );

        Macro& getMacro(string name);

        friend ostream& operator<< (ostream& , const Tech& );
        
        friend int getLefMacros(lefrCallbackType_e , lefiMacro* , lefiUserData );
        friend int getLefString(lefrCallbackType_e , const char* , lefiUserData );
        friend int getLefUnits(lefrCallbackType_e , lefiUnits* , lefiUserData );
        friend int getLefManufacturingGrid(lefrCallbackType_e , double , lefiUserData );
        friend int getLefPins(lefrCallbackType_e , lefiPin* , lefiUserData );
        friend int getLefObs(lefrCallbackType_e , lefiObstruction* , lefiUserData );
        friend int getLefLayers(lefrCallbackType_e , lefiLayer* , lefiUserData );
        friend int getLefVias(lefrCallbackType_e , lefiVia* , lefiUserData );
        friend int getLefViaGenerateRules(lefrCallbackType_e , lefiViaRule* , lefiUserData );


};

int getLefMacrosBegin(lefrCallbackType_e type, const char *str, lefiUserData data);
int getLefMacros(lefrCallbackType_e , lefiMacro* , lefiUserData );
int getLefMacrosEnd(lefrCallbackType_e type, const char *str, lefiUserData data);
int getLefString(lefrCallbackType_e , const char* , lefiUserData );
int getLefUnits(lefrCallbackType_e , lefiUnits* , lefiUserData );
int getLefManufacturingGrid(lefrCallbackType_e , double , lefiUserData );
int getLefPins(lefrCallbackType_e , lefiPin* , lefiUserData );
int getLefObs(lefrCallbackType_e , lefiObstruction* , lefiUserData );
int getLefLayers(lefrCallbackType_e , lefiLayer* , lefiUserData );
int getLefVias(lefrCallbackType_e , lefiVia* , lefiUserData );
int getLefViaGenerateRules(lefrCallbackType_e , lefiViaRule* , lefiUserData );

ostream& operator<< (ostream& , const Tech& );

}

#endif

