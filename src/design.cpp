#include "design.h"

namespace phydb {

void Design::SetTech(Tech* tech_ptr) {
    _tech_ptr = tech_ptr;
}

void Design::GetTech( ) {
    return _ptech_ptr;
}

void Design::SetUnitsDistanceMicrons(int distance_microns) {
    PhyDbExpects(distance_microns > 0, "Negative distance micron?");
    _dbuPerMicro = distance_microns;
}

void Design::SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y) {
    PhyDbExpects(upper_x > lower_x, "Right boundary is not larger than Left boundary?");
    PhyDbExpects(upper_y > lower_y, "Top boundary is not larger than Bottom boundary?");
    _dieArea.ll.x = lower_x;
    _dieArea.ll.y = lower_y;
    _dieArea.ur.x = upper_x;
    _dieArea.ur.y = upper_y;
}

Track* Design::AddTrack(string& direction, int start, int numTracks, int step, 
        vector<string>& v_layer_names) {
    int id = tracks.size();
    for(auto layer_Name : layer_names) { 
        PhyDbExpects(IsLayerExist(layerName), layerName + " Layer name does not exists, cannot add track to it");
    }
    _tracks.emplace_back(direction, start, numTracks, step, layerNames);
}

vector<Track>& Design::GetTrackVec() {
    return _tracks;
}

bool Design::IsRowExist(string& row_name) {
    return (_row_set.find(row_name) != _row_set.end());
}


Row* Design::AddRow(string& name, string& site_name, string& site_orient, int origX, int origY, int numX,
        int numY, int stepX, int stepY) {
    int id = _rows.size();
    PhyDbExpects(!IsRowExist(layerName), name + " row name exists, cannot use it again");
    _rows.emplace_back(name, site_name, site_orient, origX, origY, numX, numY, stepX, stepY);
    return &(_rows[id]);
}

vector<Row>& Design::GetRowVec() {
    return _rows;
}

bool Design::IsComponentExist(std::string &comp_name) {
    return _component2idx.find(comp_name) != _component2idx.end();
}

Component *Design::AddComponent(std::string &comp_name, std::string &macro_name, std::string &place_status,
                                int llx, int lly, std::string &orient) {
    PhyDbExpects(!IsComponentExist(comp_name), "Component name exists, cannot use it again");
    int id = _components.size();
    _components.emplace_back(comp_name, macro_name, place_status, llx, lly, orient);
    _component2idx[comp_name] = id;
    return &(_components[id]);
}

Component *Design::GetComponentPtr(std::string &comp_name) {
    if (!IsComponentExist(comp_name)) {
        return nullptr;
    }
    int id = _component2idx[comp_name];
    return &(_components[id]);
}

void Design::ReportComponent() {
    std::cout << "Total components: " << _components.size() << "\n";
    for (auto &component: _components) {
        std::cout << component << "\n";
    }
    std::cout << "\n";
}

bool Tech::IsDefViaExist(std::string const &via_name) {
    return _defVia2id.find(via_name) != _defVia2id.end();
}

DefVia *Tech::AddDefVia(std::string &via_name) {
    PhyDbExpects(!IsDefViaExist(via_name), "Macro name exists, cannot use it again");
    int id = _vias.size();
    _vias.emplace_back(via_name);
    return &(_vias[id]);
}

DefVia *Tech::GetDefViaPtr(std::string const &via_name) {
    if (!IsDefViaExist(via_name)) {
        return nullptr;
    }
    int id = _via2id[via_name];
    return &(_vias[id]);
}


bool Design::IsIoPinExist(std::string &iopin_name) {
    return _iopin2idx.find(iopin_name) != _iopin2idx.end();
}

IOPin *Design::AddIoPin(std::string &iopin_name, std::string &place_status,
                        std::string &signal_use, std::string &signal_direction,
                        int lx, int ly) {
    PhyDbExpects(!IsIoPinExist(iopin_name), "IOPin name exists, cannot use it again");
    int id = _iopins.size();
    _iopins.emplace_back(iopin_name, place_status, signal_use, signal_direction, lx, ly);
    _iopin2idx[iopin_name] = id;
    return &(_iopins[id]);
}

IOPin *Design::GetIoPinPtr(std::string &iopin_name) {
    if (!IsIoPinExist(iopin_name)) {
        return nullptr;
    }
    int id = _iopin2idx[iopin_name];
    return &(_iopins[id]);
}

bool Design::IsNetExist(std::string &net_name) {
    return _net2idx.find(net_name) != _net2idx.end();
}

Net *Design::AddNet(std::string &net_name, double weight) {
    PhyDbExpects(!IsNetExist(net_name), "Net name exists, cannot use it again");
    _nets.emplace_back(net_name, weight);
    int id = _nets.size();
    _net2idx[net_name] = id;
    return &(_nets[id]);
}

void Design::AddIoPinToNet(std::string &iopin_name, std::string &net_name) {
    IOPin *iopin_ptr = GetIoPinPtr(iopin_name);
    PhyDbExpects(iopin_ptr != nullptr, "Cannot add a nonexistent IOPIN to a net");
    Net *net_ptr = GetNetPtr(net_name);
    PhyDbExpects(net_ptr != nullptr, "Cannot add to a nonexistent net");

    iopin_ptr->SetNetName(net_name);
    net_ptr->AddIoPin(iopin_name);
}

void Design::AddCompPinToNet(std::string &comp_name, std::string &pin_name, std::string &net_name) {
    Net *net_ptr = GetNetPtr(net_name);
    PhyDbExpects(net_ptr != nullptr, "Cannot add to a nonexistent net");

    net_ptr->AddCompPin(comp_name, pin_name);
}

Net *Design::GetNetPtr(std::string &net_name) {
    if (!IsNetExist(net_name)) {
        return nullptr;
    }
    int id = _net2idx[net_name];
    return &(_nets[id]);
}

Net &Design::addNet(Net n) {
    int id = _nets.size();
    _nets.push_back(n);
    string name = n.getName();
    _net2idx[name] = id;
    return _nets[id];
}

Row &Design::addRow(Row &row) {
    _rows.push_back(row);
}

Track &Design::addTrack(Track &track) {
    _tracks.push_back(track);
}

int getDefDesign(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    Design *design = (Design *) data;
    if (type == defrDesignStartCbkType) {
        design->_name = string(str);

        if (enableOutput) {
            cout << "reading def: DESIGN " << string(str) << endl;
        }
    }
    return 0;
}

int getDefRow(defrCallbackType_e type, defiRow *row, defiUserData data) {
    //bool enableOutput = true;
    Design *design = (Design *) data;
    Row tmpRow;

    tmpRow._name = row->name();
    tmpRow._siteName = row->macro();
    tmpRow._origX = row->x();
    tmpRow._origY = row->y();
    tmpRow._siteOrient = row->orientStr();
    tmpRow._numX = row->xNum();
    tmpRow._numY = row->yNum();
    tmpRow._stepX = row->xStep();
    tmpRow._stepY = row->yStep();

    design->addRow(tmpRow);

    return 0;
}

int getDefString(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    if ((type == defrDesignStartCbkType)) {
        ((Design *) data)->_name = string(str);

        if (enableOutput) {
            cout << "reading def: DESIGN " << string(str) << endl;
        }
    }
    return 0;
}

int getDefVoid(defrCallbackType_e type, void *variable, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    if ((type == defrDesignEndCbkType)) {

        if (enableOutput) {
            cout << "reading def done" << endl;
        }
    }
    return 0;
}

int getDefDieArea(defrCallbackType_e type, defiBox *box, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    Design *design = (Design *) data;
    if ((type != defrDieAreaCbkType)) {
        cout << "Type is not defrDieAreaCbkType!" << endl;
        exit(1);
    }
    design->_dieArea.set(box->xl(), box->yl(), box->xh(), box->yh());

    return 0;
}

int getDefUnits(defrCallbackType_e type, double number, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    Design *design = (Design *) data;
    design->_dbuPerMicro = number;
    if (enableOutput) {
        cout << "UNITS DISTANCE MICRONS " << design->_dbuPerMicro << " ;" << endl;
    }
    return 0;
}

int getDefTracks(defrCallbackType_e type, defiTrack *track, defiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    if ((type != defrTrackCbkType)) {
        cout << "Type is not defrTrackCbkType!" << endl;
        exit(1);
    }

    Track tmpTrack;

    tmpTrack._direction = track->macro();
    tmpTrack._start = track->x();
    tmpTrack._numTracks = track->xNum();
    tmpTrack._step = track->xStep();

    for (int i = 0; i < track->numLayers(); i++) {
        string layerName = track->layer(i);
        tmpTrack.layerNames.push_back(layerName);
    }

    ((Design *) data)->addTrack(tmpTrack);

    return 0;
}

int getDefComponents(defrCallbackType_e type, defiComponent *comp, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    if ((type != defrComponentCbkType)) {
        cout << "Type is not defrComponentCbkType!" << endl;
        exit(1);
    }

    std::string comp_name(comp->id());
    std::string macro_name(comp->name());
    std::string place_status = "UNPLACED";
    if (comp->isPlaced())
        place_status = "PLACED";
    else if (comp->isFixed())
        place_status = "FIXED";
    else if (comp->isUnplaced())
        place_status = "UNPLACED";
    else
        place_status = "COVER";

    int llx = comp->placementX();
    int lly = comp->placementY();
    std::string orient(comp->placementOrientStr());

    ((Design *) data)->AddComponent(comp_name, macro_name, place_status, llx, lly, orient);

    return 0;
}

int getDefIOPins(defrCallbackType_e type, defiPin *pin, defiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    if (type != defrPinCbkType) {
        cout << "Type is not defrPinCbkType!" << endl;
        exit(1);
    }

    std::string iopin_name(pin->pinName());
    std::string signal_direction;
    if (pin->hasDirection())
        signal_direction = std::string(pin->direction());
    std::string signal_use;
    if (pin->hasUse())
        signal_use = std::string(pin->use());

    std::string place_status;
    if (pin->isPlaced())
        place_status = "PLACED";
    else if (pin->isUnplaced())
        place_status = "UNPLACED";
    else if (pin->isFixed())
        place_status = "FIXED";
    else if (pin->isCover())
        place_status = "COVER";

    IOPin *io_pin_ptr = ((Design *) data)->AddIoPin(iopin_name, place_status, signal_use, signal_direction);

    int llx, lly, urx, ury;
    llx = 0;
    lly = 0;
    urx = 0;
    ury = 0;
    if (pin->hasPort()) {
        cout << "Error: multiple pin ports existing in DEF" << endl;
        exit(1);
    } else {

        for (int i = 0; i < pin->numLayer(); ++i) {
            io_pin_ptr->_layerName = pin->layer(i);

            io_pin_ptr->_location.x = pin->placementX();
            io_pin_ptr->_location.y = pin->placementY();
            io_pin_ptr->_orient = string(pin->orientStr());

            pin->bounds(i, &llx, &lly, &urx, &ury);
            io_pin_ptr->_rect.set(llx, lly, urx, ury);
        }
    }

    return 0;

}

int getDefNets(defrCallbackType_e type, defiNet *net, defiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;

    if (type != defrNetCbkType) {
        cout << "Type is not defrNetCbkType!" << endl;
        exit(1);
    }
    Net tmpNet;
    tmpNet._name = net->name();

    if (enableOutput) {
        cout << "- " << net->name();
    }
    for (int i = 0; i < net->numConnections(); i++) {
        tmpNet.componentNames.push_back(string(net->instance(i)));
        tmpNet.pinNames.push_back(string(net->pin(i)));
    }

    ((Design *) data)->addNet(tmpNet);

    return 0;
}

int getDefSNets(defrCallbackType_e type, defiNet *net, defiUserData data) {
    /*
    bool enableOutput = false;
    //bool enableOutput = true;

    if (type != defrSNetCbkType) {
        cout <<"Type is not defr(S)NetCbkType!" <<endl;
        exit(1);
    }
    parser::SNet tmpSNet;
    tmpSNet.name = net->name();


    // read pre-route
    //cout << "Net " << net->name() << " has " << net->numWires() << " wires\n";
    //cout << "Net " << net->name() << " has " << net->numPaths() << " paths\n"; // no paths
    //cout << "Net " << net->name() << " has " << net->numVpins() << " vpins\n"; // no vpins

    // initialize
    string layerName   = "";
    string viaName     = "";
    string shape       = "";
    bool hasBeginPoint = false;
    bool hasEndPoint   = false;
    int beginX     = -1;
    int beginY     = -1;
    int beginExt   = -1;
    int endX       = -1;
    int endY       = -1;
    int endExt     = -1;
    bool hasRect   = false;
    int llx        = -1;
    int lly        = -1;
    int urx        = -1;
    int ury        = -1;
    int width      = 0;
    for (int i = 0; i < (int)net->numWires(); i++) {
        defiWire* tmpWire = net->wire(i);
        //cout << "Wire " << i << "\n";
        //cout << "  Type: " << tmpWire->wireType() << endl;
        //cout << "  has " << tmpWire->numPaths() << " paths\n";

        if (enableOutput) {
            cout << "SNET " << tmpSNet.name << endl;
            cout <<"  + " <<tmpWire->wireType();
            cout << "  has " << net->numWires() << " wires\n";
            cout << "  has " << tmpWire->numPaths() << " paths\n";
        }
        // each path is a def line
        for (int j = 0; j < (int)tmpWire->numPaths(); j++) {
            parser::Path tmpPath;
            defiPath* path     = tmpWire->path(j);
            path->initTraverse();
            // initialize
            layerName     = "";
            viaName       = "";
            shape         = "";
            hasBeginPoint = false;
            hasEndPoint   = false;
            beginX        = -1;
            beginY        = -1;
            beginExt      = -1;
            endX          = -1;
            endY          = -1;
            endExt        = -1;
            hasRect       = false;
            llx           = -1;
            lly           = -1;
            urx           = -1;
            ury           = -1;
            width         = 0;
            //cout <<"path here" <<endl;

            int pathId;
            while ((pathId = path->next()) != DEFIPATH_DONE) {
                //cout << "  pathId = " << pathId << endl;
                switch(pathId) {
                    case DEFIPATH_LAYER:
                        tmpPath.layerName = string(path->getLayer());
                        break;
                    case DEFIPATH_VIA:
                        tmpPath.viaName = string(path->getVia());
                        break;
                    case DEFIPATH_WIDTH:
                        tmpPath.width = path->getWidth();
                        break;
                    case DEFIPATH_POINT:
                        if (!hasBeginPoint) {
                            path->getPoint(&beginX, &beginY);
                            hasBeginPoint = true;
                            tmpPath.begin.x = beginX;
                            tmpPath.begin.y = beginY;
                        } else {
                            path->getPoint(&endX, &endY);
                            hasEndPoint = true;
                            tmpPath.end.x = endX;
                            tmpPath.end.y = endY;
                        }
                        break;
                    case DEFIPATH_FLUSHPOINT:
                        if (!hasBeginPoint) {
                            path->getFlushPoint(&beginX, &beginY, &beginExt);
                            hasBeginPoint = true;
                            tmpPath.begin.x = beginX;
                            tmpPath.begin.y = beginY;
                            tmpPath.beginExt = beginExt;
                        } else {
                            path->getFlushPoint(&endX, &endY, &endExt);
                            hasEndPoint = true;
                            tmpPath.end.x = endX;
                            tmpPath.end.y = endY;
                            tmpPath.endExt = endExt;
                        }
                        break;
                    case DEFIPATH_SHAPE:
                        tmpPath.shape = path->getShape();
                        break;
                    case DEFIPATH_RECT:
                        path->getViaRect(&llx, &lly, &urx, &ury);
                        tmpPath.rect.set(llx, lly, urx, ury);
                        hasRect = true;
                        break;
                    case DEFIPATH_VIRTUALPOINT:
                        if (!hasBeginPoint) {
                            path->getVirtualPoint(&beginX, &beginY);
                            hasBeginPoint = true;

                            tmpPath.begin.x = beginX;
                            tmpPath.begin.y = beginY;
                        } else {
                            path->getVirtualPoint(&endX, &endY);
                            hasEndPoint = true;

                            tmpPath.end.x = endX;
                            tmpPath.end.y = endY;
                        }
                        break;
                    default : cout <<" net " <<net->name() <<" unknown pathId " <<pathId <<endl; break;
                }
            }


            // add via
            /*if (viaName != "") {
              if (((io::Parser*)data)->tech->name2via.find(viaName) == ((io::Parser*)data)->tech->name2via.end()) {
                if (VERBOSE > -1) {
                  cout <<"Error: unsupported via: " <<viaName <<endl;
                }
              } else {
                frPoint p;
                if (hasEndPoint) {
                  p.set(endX, endY);
                } else {
                  p.set(beginX, beginY);
                }
                auto viaDef = ((io::Parser*)data)->tech->name2via[viaName];
                auto tmpP = make_unique<frVia>(viaDef);
                tmpP->setOrigin(p);
                tmpP->addToNet(netIn);
                netIn->addVia(tmpP);
              }
            }*/
    /*
            tmpSNet.paths.push_back(tmpPath);
        } // end path
    } // end wire

    ((parser::defDataBase*) data)->snets.push_back(tmpSNet);

    if(enableOutput)
        tmpSNet.print();
    */
    return 0;
}

int getDefVias(defrCallbackType_e type, defiVia *via, defiUserData data) {
    /*
    //bool enableOutput = true;
    bool enableOutput = false;
    if ((type != defrViaCbkType)) {
        cout <<"Type is not defrViaCbkType!" <<endl;
        exit(1);
    }

    parser::defVia tmpVia;
    tmpVia.name = via->name();

    // viaRule defined via
    if (via->hasViaRule()) {
        char* viaRuleName;
        char* botLayer;
        char* cutLayer;
        char* topLayer;
        int xSize, ySize, xCutSpacing, yCutSpacing, xBotEnc, yBotEnc, xTopEnc, yTopEnc;

        via->viaRule(&viaRuleName, &xSize, &ySize, &botLayer, &cutLayer, &topLayer,
                     &xCutSpacing, &yCutSpacing, &xBotEnc, &yBotEnc, &xTopEnc, &yTopEnc);
        tmpVia.viaRuleName = viaRuleName;
        tmpVia.cutSize.set(xSize, ySize);
        tmpVia.layers[0] = string(botLayer);
        tmpVia.layers[1] = string(cutLayer);
        tmpVia.layers[2] = string(topLayer);

        tmpVia.cutSpacing.set(xCutSpacing, yCutSpacing);

        tmpVia.botEnc.set(xBotEnc, yBotEnc);
        tmpVia.topEnc.set(xTopEnc, yTopEnc);

        int xOrigin = 0;
        int yOrigin = 0;
        if (via->hasOrigin()) {
            via->origin(&xOrigin, &yOrigin);
        }
        tmpVia.origin.set(xOrigin, yOrigin);

        int xBotOffset = 0;
        int yBotOffset = 0;
        int xTopOffset = 0;
        int yTopOffset = 0;
        if (via->hasOffset()) {
            via->offset(&xBotOffset, &yBotOffset, &xTopOffset, &yTopOffset);
        }
        tmpVia.botOffset.set(xBotOffset, yBotOffset);
        tmpVia.topOffset.set(xTopOffset, yTopOffset);

        int numCutRows = 1;
        int numCutCols = 1;
        if (via->hasRowCol()) {
            via->rowCol(&numCutRows, &numCutCols);
        }
        tmpVia.numCutRows = numCutRows;
        tmpVia.numCutCols = numCutCols;


    }
    else // RECT defined via
    {
        if (via->numPolygons()) {
            cout <<"Error: unsupport polygon in def via" <<endl;
            exit(1);
        }
        char* layerName;
        int xl;
        int yl;
        int xh;
        int yh;

        for (int i = 0; i < via->numLayers(); ++i) {
            via->layer(i, &layerName, &xl, &yl, &xh, &yh);
            parser::Rect2DLayer<int> tmpRect2DLayer;
            tmpRect2DLayer.set(layerName, xl, yl, xh, yh);
        }
    }

    tmpVia.idx = ((parser::defDataBase*) data)->vias.size();
    ((parser::defDataBase*) data)->defVia2idx.insert( pair<string, int> (tmpVia.name, tmpVia.idx));
    ((parser::defDataBase*) data)->vias.push_back(tmpVia);

    if(enableOutput)
        tmpVia.print();
    */
    return 0;
}

int getDefGcell(defrCallbackType_e type, defiGcellGrid *gcellGrid, defiUserData data) {
    /*
    bool enableOutput = false;

    parser::GcellGrid tmpGcellGrid;
    tmpGcellGrid.direction = string(gcellGrid->macro());
    tmpGcellGrid.start = gcellGrid->x();
    tmpGcellGrid.numBoundaries = gcellGrid->xNum();
    tmpGcellGrid.step = gcellGrid->xStep();

    ((parser::defDataBase*) data)->gcellGrids.push_back(tmpGcellGrid);

    if(enableOutput)
        tmpGcellGrid.print();
    */
    return 0;
}

int getDefVersion(defrCallbackType_e type, double version, defiUserData data) {
    ((Design *) data)->_version = version;
    return 0;
}

int getDefBusBit(defrCallbackType_e type, const char *BusBit, defiUserData data) {
    ((Design *) data)->_busBitChars = BusBit;
    return 0;
}

int getDefDivider(defrCallbackType_e type, const char *divider, defiUserData data) {
    ((Design *) data)->_dividerChar = divider;
    return 0;
}

}
