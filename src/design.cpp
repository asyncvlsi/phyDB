#include "design.h"

namespace phydb {

void Design::readDef(string defFileName) {


}

Component& Design::addComponent(Component c) {
    int id = _components.size();
    _components.push_back(c);
    string name = c.getName();
    _component2id[name] = id;
    return _components[id];
}

IOPin& Design::addIOPin(IOPin p) {
    int id = _iopins.size();
    _iopins.push_back(p);
    string name = p.getName();
    _iopin2id[name] = id;
    return _iopins[id];
}

Net& Design::addNet(Net n) {
    int id = _nets.size();
    _nets.push_back(n);
    string name = n.getName();
    _net2id[name] = id;
    return _nets[id];
}

int getDefDesign(defrCallbackType_e type, const char* str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    Design* design = (Design* ) data;
    if ((type == defrDesignStartCbkType)) {
        design->_name = string(str);

        if (enableOutput) {
            cout <<"reading def: DESIGN " << string(str) <<endl;
        }
    }
    return 0;
}

int getDefRow(defrCallbackType_e type, defiRow* row, defiUserData data) {
  //bool enableOutput = true;
    Design* design = (Design* ) data;
    parser::Row tmpRow;

    tmpRow._name = row->name();
    tmpRow._siteName = row->macro();
    tmpRow._origX = row->x();
    tmpRow._origY = row->y();
    tmpRow._siteOrient = row->orientStr();
    tmpRow._numX = row->xNum();
    tmpRow._numY = row->yNum();
    tmpRow._stepX = row->xStep();
    tmpRow._stepY = row->yStep();

    design->rows.addRow(tmpRow);
  
    return 0;
}

int getDefDieArea(defrCallbackType_e type, defiBox* box, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    Design* design = (Design* ) data;
    if ((type != defrDieAreaCbkType)) {
        cout <<"Type is not defrDieAreaCbkType!" <<endl;
        exit(1);
    }
    design->dieArea.set(box->xl(), box->yl(), box->xh(), box->yh());

    return 0;
}

int getDefUnits(defrCallbackType_e type, double number, defiUserData data) {
  //bool enableOutput = true;
    bool enableOutput = false;
    Design* design = (Design* ) data;
    design->dbuPerMicro = number;
    if (enableOutput) {
        cout <<"UNITS DISTANCE MICRONS " <<((parser::defDataBase*) data)->dbuPerMicro <<" ;" <<endl;
    }
    return 0;
}


int getDefTracks(defrCallbackType_e type, defiTrack* track, defiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    if ((type != defrTrackCbkType)) {
        cout <<"Type is not defrTrackCbkType!" <<endl;
        exit(1);
    }

    Track tmpTrack;

    tmpTrack.direction = track->macro();
    tmpTrack.start = track->x();
    tmpTrack.numTracks = track->xNum();
    tmpTrack.step = track->xStep();

    for(int i = 0; i < track->numLayers(); i++) {
        string layerName = track->layer(i);
        tmpTrack.layerNames.push_back(layerName);
    }

    ((design*) data)->tracks.addTrack(tmpTrack);
  
    return 0;
}

int getDefComponents(defrCallbackType_e type, defiComponent* comp, defiUserData data) {
  //bool enableOutput = true;
    bool enableOutput = false;
    if ((type != defrComponentCbkType)) {
        cout <<"Type is not defrComponentCbkType!" <<endl;
        exit(1);
    }

    Component tmpComp;
    tmpComp.name = comp->id();
    tmpComp.macroName = comp->name();
    if(comp->isPlaced())
        tmpComp.locationType = "PLACED";
    else if(comp->isFixed())
        tmpComp.locationType = "FIXED";
    else if(comp->isUnplaced())
        tmpComp.locationType = "UNPLACED";
    else
        tmpComp.locationType = "COVER";

    tmpComp.location.x = comp->placementX();
    tmpComp.location.y = comp->placementY();
    tmpComp.orient = string(comp->placementOrientStr());

    ((Design* ) data)->addComponent(tmpComp);

    return 0;
}

int getDefIOPins(defrCallbackType_e type, defiPin* pin, defiUserData data) {
  bool enableOutput = false;
  //bool enableOutput = true;
  if (type != defrPinCbkType) {
    cout <<"Type is not defrPinCbkType!" <<endl;
    exit(1);
  }

  IOPin tmpPin;
  tmpPin.name = pin->pinName();
  tmpPin.netName = pin->netName();
  if(pin->hasDirection())
      tmpPin.direction = pin->direction();
  if(pin->hasUse())
      tmpPin.use = pin->use();
  
  int llx, lly, urx, ury;
  llx = 0;
  lly = 0;
  urx = 0;
  ury = 0;
    
  if(pin->isPlaced())
      tmpPin.status = "PLACED";
  else if(pin->isUnplaced())
      tmpPin.status = "UNPLACED";
  else if(pin->isFixed())
      tmpPin.status = "FIXED";
  else if(pin->isCover())
      tmpPin.status = "COVER";

  if (pin->hasPort()) {
    cout <<"Error: multiple pin ports existing in DEF" <<endl;
    exit(1);
  } 
  else 
  {
    
    for (int i = 0; i < pin->numLayer(); ++i) 
    {
        tmpPin.layerName = pin->layer(i);
         
        tmpPin.location.x = pin->placementX();
        tmpPin.location.y = pin->placementY();
        tmpPin.orient = string(pin->orientStr());

        pin->bounds(i, &llx, &lly, &urx, &ury);
        tmpPin.rect.set(llx, lly, urx, ury);
    }
  }

  ((Design*)data)->addIOPin(tmpPin);
  
  return 0;

}

int getDefNets(defrCallbackType_e type, defiNet* net, defiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;

    if (type != defrNetCbkType ) {
        cout <<"Type is not defrNetCbkType!" <<endl;
        exit(1);
    }
    Net tmpNet;
    tmpNet.name = net->name();

    if (enableOutput) {
        cout <<"- " <<net->name();
    }
    for (int i = 0; i < net->numConnections(); i++) {
        tmpNet.componentNames.push_back(string(net->instance(i)));
        tmpNet.pinNames.push_back(string(net->pin(i)));
    }

    ((*Design) data)->addNet(tmpNet);

    return 0;
}

int getDefVersion(defrCallbackType_e type, double version, defiUserData data) {
    ((Design) *) data)->version = version;
    return 0;
}

int getDefBusBit(defrCallbackType_e type, const char* BusBit, defiUserData data) {
    ((Design) *) data)->busBitChars = BusBit;
    return 0;
}

int getDefDivider(defrCallbackType_e type, const char* divider, defiUserData data) {
    ((Design)*) data)->dividerChar = divider;
    return 0;
}




}
