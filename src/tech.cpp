#include "tech.h"


namespace phydb {

Macro& Tech::addMacro(Macro m) {
    int id = _macros.size();
    //m._id = id;
    _macros.push_back(m);
    string name = m.getName();
    _macro2id[name] = id;
    return _macros[id];
}

Macro& Tech::getMacro(string name) {
    int id = _macro2id[name];

    return _macros[id];
}

Site& Tech::addSite(Site s) {
    int id = _sites.size();
    _sites.push_back(s);
    return _sites[id];
}

Layer& Tech::addLayer(Layer l) {

    int id = _layers.size();
    _layers.push_back(l);
    string name = l.getName();
    _layer2id[name] = id;
    return _layers[id];
}

int getLefMacros(lefrCallbackType_e type, lefiMacro* macro, lefiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    if ((type != lefrMacroCbkType)) {
        cout <<"Type is not lefrMacroCbkType!" <<endl;
        exit(2);
    }

    float originX = macro->originX(); 
    float originY = macro->originY();
    float sizeX   = macro->sizeX();
    float sizeY   = macro->sizeY();
    if (enableOutput) {
        cout <<"  ORIGIN " <<originX  <<" " 
                           <<originY  <<" ;" <<endl;
        cout <<"  SIZE   " <<sizeX    <<" " 
                           <<sizeY    <<endl;
    }
    Tech* tech = (Tech*) data;
    int nMacros = tech->_macros.size();
    Macro& m = ((Tech*) data)->getMacro(nMacros); //write to the last one
    m.setOrigin(originX, originY);
    m.setSize(sizeX, sizeY);

    return 0;
}

int getLefMacrosBegin(lefrCallbackType_e type, const char* str, lefiUserData data) {
    assert(type == lefrMacroBeginCbkType);
    
    Tech* tech = (Tech*) data;
    Macro& m = tech->addMacro(Macro()); //add an empty macro
    m._name = string(str);
    return 0;  
} 
  
int getLefMacrosEnd(lefrCallbackType_e type, const char* str, lefiUserData data) { 
    // could probably delete this function

    assert(type == lefrMacroEndCbkType);
    return 0;
}


int getLefUnits(lefrCallbackType_e type, lefiUnits* units, lefiUserData data) {
    bool enableOutput = false;
    Tech* tech = (Tech*) data;
    tech->dbuPerMicron = units->databaseNumber();
    if (enableOutput) {
        cout <<"DATABASE MICRONS " << ((parser::lefDataBase*) data)->dbuPerMicron <<endl;
    }
    return 0;
}


int getLefManufacturingGrid(lefrCallbackType_e type, double number, lefiUserData data) {
    bool enableOutput = false;
    Tech* tech = (Tech*) data;
    tech->manufacturingGrid = number;
    if (enableOutput) {
        cout <<"MANUFACTURINGGRID " <<number <<endl;
    }
    return 0;
}

int getLefPins(lefrCallbackType_e type, lefiPin* pin, lefiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    Tech* tech = (Tech*) data;
    if (type != lefrPinCbkType) {
        cout <<"Type is not lefrPinCbkType!" <<endl;
        exit(1);
    }

    // term
    int nMacros = tech->_macros.size();
    Macro& tmpMacro = ((Tech*) data)->getMacro(nMacros); //write to the last one

    parser::Pin tmpPin;
    parser::LayerRect tmpLayerRect;
    parser::Rect2D<float> tmpRect;

    tmpPin.name = pin->name();
    tmpPin.use = pin->use(); 

    if (enableOutput) {
        cout <<"  PIN " <<pin->name() <<endl;
    }

    int numPorts = pin->numPorts();

    for (int i = 0; i < numPorts; ++i) {
        int numItems = pin->port(i)->numItems();
        
        if (enableOutput) {
          cout <<"    PORT" <<endl;
        }

        int isNewLayerRect = true;

        for (int j = 0; j < numItems; ++j) {
            int itemType = pin->port(i)->itemType(j);
            if (itemType == 1) 
            { //layer
                if(isNewLayerRect)
                {
                    isNewLayerRect = false;
                    tmpLayerRect.reset();
                }
                else
                {
                    tmpPin.layerRects.push_back(tmpLayerRect);
                    tmpLayerRect.reset();
                }
                tmpLayerRect.layerName = pin->port(i)->getLayer(j);

                if (enableOutput) {
                    cout <<"    LAYER " << tmpLayerRect.layerName <<" ;" <<endl;
                }
            } 
            else if (itemType == 8) 
            {
                float llx = pin->port(i)->getRect(j)->xl;
                float lly = pin->port(i)->getRect(j)->yl;
                float urx = pin->port(i)->getRect(j)->xh;
                float ury = pin->port(i)->getRect(j)->yh;
                tmpRect.set(llx, lly, urx, ury);

                tmpLayerRect.rects.push_back(tmpRect);

                if (enableOutput) {
                    cout <<"      RECT " <<llx <<" " <<lly <<" "<<urx <<" " <<ury <<" ;" <<endl;
                }      
            } 
            else 
            {
                cout <<"unsupported lefiGeometries!" <<endl;
                continue;
                // exit(2);
            }
        }
        tmpPin.layerRects.push_back(tmpLayerRect);
    }
    tmpMacro.pins.push_back(tmpPin);

    //tmpPin.print();
    if (enableOutput) 
    {
        cout <<"  END " <<pin->name() <<endl;
    }

    //checkLargePin(tmpPin);

    return 0;
}



int getLefObs(lefrCallbackType_e type, lefiObstruction* obs, lefiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = FALSE;

    if (type != lefrObstructionCbkType) {
        cout <<"Type is not lefrObstructionCbkType!" <<endl;
        exit(1);
    }
    Tech* tech = (Tech*) data;
    int nMacros = tech->_macros.size();
    Macro& tmpMacro = ((Tech*) data)->getMacro(nMacros); //write to the last one

    parser::LayerRect tmpLayerRect;
    parser::Rect2D<float> tmpRect;

    if (enableOutput) {
        cout <<"  OBS" <<endl;
    }

    auto geometry = obs->geometries();
    int numItems  = geometry->numItems();

    int isNewLayerRect = true;
    for (int i = 0; i < numItems; ++i) 
    {
        if (geometry->itemType(i) == lefiGeomLayerE) 
        {
            if(isNewLayerRect)
            {
                isNewLayerRect = false;
                tmpLayerRect.reset();
            }
            else
            {
                tmpMacro.obs.layerRects.push_back(tmpLayerRect);
                tmpLayerRect.reset();
            }
            tmpLayerRect.layerName = geometry->getLayer(i);

            if (enableOutput) {
            cout <<"    LAYER " <<tmpLayerRect.layerName <<" ;" <<endl;
            }
        } 
        else if (geometry->itemType(i) == lefiGeomRectE) 
        {

            float llx = geometry->getRect(i)->xl;
            float lly = geometry->getRect(i)->yl;
            float urx = geometry->getRect(i)->xh;
            float ury = geometry->getRect(i)->yh;
            tmpRect.set(llx, lly, urx, ury);

            tmpLayerRect.rects.push_back(tmpRect);

            if (enableOutput) {
                cout <<"      RECT " <<llx <<" " <<lly <<" "  <<urx <<" " <<ury <<" ;" <<endl;
            }
        } 
        else 
        {
            cout <<"Warning: unsupported OBS" <<endl;
            continue;
        }
    }
    tmpMacro.obs.layerRects.push_back(tmpLayerRect);

    return 0;
}


int getLefCornerSpacing(void* data, const string& stringProp)
{
    istringstream istr(stringProp);
    string token;
    auto& tmpLayer = ((parser::lefDataBase*) data)->tmpLayer;

    while(!istr.eof())
    {
        istr >> token;
        if(token == "EXCEPTEOL")
        {
            istr >> tmpLayer.cornerSpacing.eolWidth;
        }
        else if(token == "WIDTH")
        {
            float width;
            istr >> width;
            tmpLayer.cornerSpacing.width.push_back(width);
        }
        else if(token == "SPACING")
        {
            float spacing;
            istr >> spacing;
            tmpLayer.cornerSpacing.spacing.push_back(spacing);
        }
    }
    return 0;
}

int getLefLayers(lefrCallbackType_e type, lefiLayer* layer, lefiUserData data) {

    bool enableOutput = false;

    if (type != lefrLayerCbkType) {
        cout <<"Type is not lefrLayerCbkType!" <<endl;
        exit(1);
    }

    Tech* tech = (Tech*) data;
    int nLayers = tech->_layers.size();
    Macro& tmpLayer = ((Tech*) data)->getLayer(nLayers); //write to the last one


  if (strcmp(layer->type(), "ROUTING") == 0) 
  {
    tmpLayer.name = layer->name();
    tmpLayer.type = layer->type();
    tmpLayer.width = layer->width();
    if (layer->hasMinwidth()) {
        tmpLayer.minWidth = layer->minwidth();
    } else {
        tmpLayer.minWidth = layer->width();
    }
    tmpLayer.direction = layer->direction();

    if (layer->hasXYPitch()) {
        tmpLayer.pitchx = layer->pitchX();
        tmpLayer.pitchy = layer->pitchY();
    } else {
        tmpLayer.pitchx = layer->pitch();
        tmpLayer.pitchy = layer->pitch();
    }
    tmpLayer.offset = layer->offset();

    // read minArea rule
    if (layer->hasArea()) {
      tmpLayer.area = layer->area();
    }
    
    double grid = ((parser::lefDataBase* )data)->manufacturingGrid;  
    if(grid == 0)
        tmpLayer.minLength = tmpLayer.area / tmpLayer.width;
    else {
        float minLength = tmpLayer.area / tmpLayer.width;
        int multiple = (minLength / grid) / 6;
        if(minLength == ((float) multiple )* grid * 6)
            tmpLayer.minLength = minLength;
        else {
            tmpLayer.minLength = (multiple + 1) * grid * 6;
        }
    }
    

    if(enableOutput)
        cout <<"Layer" << layer->name() << " number of props " <<layer->numProps() <<endl;
    if(layer->numProps() > 1)
    {
        cout << "Error: enable to handle more than one properties:" << layer->name() << endl;
    }
    for (int i = 0; i < layer->numProps(); i++) {
        if (string(layer->propName(i)) == string("LEF58_CORNERSPACING") && layer->propIsString(i)) 
        {
            getLefCornerSpacing(data, layer->propValue(i));
        } 
        else 
        {
            cout << "UNSUPPORTED PROPERTY: " << layer->propName(i) << endl;
        }
    }

    // read spacing rule
    for (int i = 0; i < layer->numSpacing(); ++i) {
        parser::Spacing tmpSpacing;
        tmpSpacing.spacing = layer->spacing(i);

        if (layer->hasSpacingEndOfLine(i)) {

            if (enableOutput) {
                cout <<"  SPACING " <<layer->spacing(i) <<" ENDOFLINE " <<layer->spacingEolWidth(i)
                <<" WITHIN " <<layer->spacingEolWithin(i);
            }
            tmpSpacing.eolWidth = layer->spacingEolWidth(i);
            tmpSpacing.eolWithin = layer->spacingEolWithin(i);

            if (layer->hasSpacingParellelEdge(i)) {
                if (enableOutput) {
                    cout <<" PARALLELEDGE " <<layer->spacingParSpace(i) <<" WITHIN " <<layer->spacingParWithin(i);
                }
                tmpSpacing.parEdge = layer->spacingParSpace(i);
                tmpSpacing.parWithin = layer->spacingParWithin(i);
            }

            tmpLayer.spacings.push_back(tmpSpacing);

        } 
        else { 
            cout << "no eol spacing!"<<endl;
        }
    }

    // read spacingTable
    if(layer->numSpacingTable() > 1)
        cout << "warning: More than one spacing table: " << tmpLayer.name << endl;

    for (int i = 0; i < layer->numSpacingTable(); ++i) {
        auto spTable = layer->spacingTable(i);
        if (spTable->isParallel() == 1) {
        //if (spTable->parallel()) {
            auto parallel = spTable->parallel();

            if (enableOutput) {
                cout <<"  SPACINGTABLE" <<endl;
                cout <<"  PARALLELRUNLENGTH";
            }
            for (int j = 0; j < parallel->numLength(); ++j) {
                tmpLayer.spacingTable.parallelRunLength.push_back(parallel->length(j));
            }
            for (int j = 0; j < parallel->numWidth(); ++j) {
                tmpLayer.spacingTable.width.push_back(parallel->width(j));
                for (int k = 0; k < parallel->numLength(); ++k) {
                    tmpLayer.spacingTable.spacing.push_back( parallel->widthSpacing(j, k));
                }
            }
            if (enableOutput) {
                cout <<" ;" <<endl;
            }
        } 
        else if(spTable->isInfluence()) {

        }
        else { 
            cout << "unsupported spacing table!" << endl;
        }
    }

    int layerIdx = ((parser::lefDataBase*)data)->layers.size();
    tmpLayer.idx = layerIdx;
    ((parser::lefDataBase*)data)->layer2idx.insert( pair<string, int> (tmpLayer.name, layerIdx));
    ((parser::lefDataBase*)data)->layers.push_back(tmpLayer);

  } 
  else if (strcmp(layer->type(), "CUT") == 0) // cut layer
  { 

    tmpLayer.name  = layer->name();
    tmpLayer.type  = layer->type();
    tmpLayer.width  = layer->width();
    
    // read spacing constraint
    for (int i = 0; i < layer->numSpacing(); ++i) {

        if(layer->hasSpacingAdjacent(i))
        {
            parser::Spacing tmpSpacing;
            tmpSpacing.spacing = layer->spacing(i);
            tmpSpacing.adjacentCuts = layer->spacingAdjacentCuts(i);
            tmpSpacing.cutWithin = layer->spacingAdjacentWithin(i);
            tmpLayer.spacings.push_back(tmpSpacing);
        }
        else
        {
            tmpLayer.spacing = layer->spacing(i);
        }
    }
    
    int layerIdx = ((parser::lefDataBase*) data)->layers.size();
    tmpLayer.idx = layerIdx;
    ((parser::lefDataBase*) data)->layer2idx.insert( pair<string, int> (tmpLayer.name, layerIdx));
    ((parser::lefDataBase*) data)->layers.push_back(tmpLayer);

  } 
  else 
  {
    if(string(layer->name()) != "OVERLAP")
        cout << "unsupported layer type: " << layer->name() << ": " << layer->type() << endl;

  }

  if(enableOutput)
    tmpLayer.print();

  return 0;
}



}

