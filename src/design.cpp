#include "design.h"

namespace phydb {

void Design::SetUnitsDistanceMicrons(int distance_microns) {
    PhyDbExpects(distance_microns > 0, "Negative distance micron?");
    dbu_per_micro_ = distance_microns;
}

void Design::SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y) {
    PhyDbExpects(upper_x > lower_x, "Right boundary is not larger than Left boundary?");
    PhyDbExpects(upper_y > lower_y, "Top boundary is not larger than Bottom boundary?");
    die_area_.ll.x = lower_x;
    die_area_.ll.y = lower_y;
    die_area_.ur.x = upper_x;
    die_area_.ur.y = upper_y;
}


Track *Design::AddTrack(string &direction, int start, int num_tracks, int step, vector<string> &layer_names) {
    tracks_.emplace_back(direction, start, num_tracks, step, layer_names);
}

vector<Track> &Design::GetTrackVec() {
    return tracks_;
}

bool Design::IsRowExist(string &row_name) {
    return (row_set_.find(row_name) != row_set_.end());
}

Row *Design::AddRow(string &name, string &site_name, string &site_orient, int origX, int origY, int numX,
                    int numY, int stepX, int stepY) {
    int id = rows_.size();
    PhyDbExpects(!IsRowExist(name), name + " row name_ exists, cannot use it again");
    rows_.emplace_back(name, site_name, site_orient, origX, origY, numX, numY, stepX, stepY);
    return &(rows_[id]);
}

vector<Row> &Design::GetRowVec() {
    return rows_;
}

bool Design::IsComponentExist(std::string &comp_name) {
    return component_2_id_.find(comp_name) != component_2_id_.end();
}

Component *Design::AddComponent(std::string &comp_name, std::string &macro_name, std::string &place_status,
                                int llx, int lly, std::string &orient) {
    PhyDbExpects(!IsComponentExist(comp_name), "Component name_ exists, cannot use it again");
    int id = components_.size();
    components_.emplace_back(comp_name, macro_name, place_status, llx, lly, orient);
    component_2_id_[comp_name] = id;
    return &(components_[id]);
}

Component *Design::GetComponentPtr(std::string &comp_name) {
    if (!IsComponentExist(comp_name)) {
        return nullptr;
    }
    int id = component_2_id_[comp_name];
    return &(components_[id]);
}

void Design::ReportComponent() {
    std::cout << "Total components: " << components_.size() << "\n";
    for (auto &component: components_) {
        std::cout << component << "\n";
    }
    std::cout << "\n";
}

bool Design::IsDefViaExist(std::string const &via_name) {
    return def_via_2_id_.find(via_name) != def_via_2_id_.end();
}

DefVia *Design::AddDefVia(std::string &via_name) {
    PhyDbExpects(!IsDefViaExist(via_name), "Macro name_ exists, cannot use it again");
    int id = vias_.size();
    vias_.emplace_back(via_name);
    via_2_id_[via_name] = id;
    return &(vias_[id]);
}

DefVia *Design::GetDefViaPtr(std::string const &via_name) {
    if (!IsDefViaExist(via_name)) {
        return nullptr;
    }
    int id = via_2_id_[via_name];
    return &(vias_[id]);
}

bool Design::IsIoPinExist(std::string &iopin_name) {
    return iopin_2_id_.find(iopin_name) != iopin_2_id_.end();
}

IOPin *Design::AddIoPin(std::string &iopin_name, std::string &place_status,
                        std::string &signal_use, std::string &signal_direction,
                        int lx, int ly) {
    PhyDbExpects(!IsIoPinExist(iopin_name), "IOPin name_ exists, cannot use it again");
    int id = iopins_.size();
    iopins_.emplace_back(iopin_name, place_status, signal_use, signal_direction, lx, ly);
    iopin_2_id_[iopin_name] = id;
    return &(iopins_[id]);
}

IOPin *Design::GetIoPinPtr(std::string &iopin_name) {
    if (!IsIoPinExist(iopin_name)) {
        return nullptr;
    }
    int id = iopin_2_id_[iopin_name];
    return &(iopins_[id]);
}

bool Design::IsNetExist(std::string &net_name) {
    return net_2_id_.find(net_name) != net_2_id_.end();
}

Net *Design::AddNet(std::string &net_name, double weight) {
    PhyDbExpects(!IsNetExist(net_name), "Net name_ exists, cannot use it again");
    nets_.emplace_back(net_name, weight);
    int id = nets_.size();
    net_2_id_[net_name] = id;
    return &(nets_[id]);
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
    int id = net_2_id_[net_name];
    return &(nets_[id]);
}

int getDefDesign(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    Design *design = (Design *) data;
    if (type == defrDesignStartCbkType) {
        design->name_ = string(str);

        if (enableOutput) {
            cout << "reading def: DESIGN " << string(str) << endl;
        }
    }
    return 0;
}

int getDefRow(defrCallbackType_e type, defiRow *row, defiUserData data) {
    //bool enableOutput = true;
    auto *design = (Design *) data;

    std::string row_name(row->name());
    std::string site_name(row->macro());
    std::string site_orientation(row->orientStr());
    int origX = row->x();
    int origY = row->y();
    int numX = row->xNum();
    int numY = row->yNum();
    int stepX = row->xStep();
    int stepY = row->yStep();

    design->AddRow(row_name, site_name, site_orientation, origX, origY, numX, numY, stepX, stepY);

    return 0;
}

int getDefString(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    if ((type == defrDesignStartCbkType)) {
        ((Design *) data)->name_ = string(str);

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
    design->die_area_.Set(box->xl(), box->yl(), box->xh(), box->yh());

    return 0;
}

int getDefUnits(defrCallbackType_e type, double number, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    Design *design = (Design *) data;
    design->dbu_per_micro_ = number;
    if (enableOutput) {
        cout << "UNITS DISTANCE MICRONS " << design->dbu_per_micro_ << " ;" << endl;
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

    auto *design = (Design *) data;

    std::string direction(track->macro());
    int start = track->x();
    int num_tracks = track->xNum();
    int step = track->xStep();
    std::vector<std::string> layer_names;

    for (int i = 0; i < track->numLayers(); i++) {
        string layerName = track->layer(i);
        layer_names.push_back(layerName);
    }
    design->AddTrack(direction, start, num_tracks, step, layer_names);

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
            io_pin_ptr->layer_name_ = pin->layer(i);

            io_pin_ptr->location_.x = pin->placementX();
            io_pin_ptr->location_.y = pin->placementY();
            io_pin_ptr->orient_ = string(pin->orientStr());

            pin->bounds(i, &llx, &lly, &urx, &ury);
            io_pin_ptr->rect_.Set(llx, lly, urx, ury);
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
    auto *design = (Design *) data;

    std::string net_name(net->name());
    design->AddNet(net_name);

    if (enableOutput) {
        cout << "- " << net->name();
    }
    // TODO: IOPINs are missing? Cannot check sanity if call AddCompPinToNet at design level, should do it at PhyDB level
    for (int i = 0; i < net->numConnections(); i++) {
        std::string comp_name(net->instance(i));
        std::string pin_name(net->pin(i));
        design->AddCompPinToNet(comp_name, pin_name, net_name);
    }

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
    tmpSNet.name_ = net->name_();


    // read pre-route
    //cout << "Net " << net->name_() << " has " << net->numWires() << " wires\n";
    //cout << "Net " << net->name_() << " has " << net->numPaths() << " paths\n"; // no paths
    //cout << "Net " << net->name_() << " has " << net->numVpins() << " vpins\n"; // no vpins

    // initialize
    string layer_name_   = "";
    string via_name_     = "";
    string shape_       = "";
    bool hasBeginPoint = false;
    bool hasEndPoint   = false;
    int beginX     = -1;
    int beginY     = -1;
    int begin_ext_   = -1;
    int endX       = -1;
    int endY       = -1;
    int end_ext_     = -1;
    bool hasRect   = false;
    int llx        = -1;
    int lly        = -1;
    int urx        = -1;
    int ury        = -1;
    int width_      = 0;
    for (int i = 0; i < (int)net->numWires(); i++) {
        defiWire* tmpWire = net->wire(i);
        //cout << "Wire " << i << "\n";
        //cout << "  Type: " << tmpWire->wireType() << endl;
        //cout << "  has " << tmpWire->numPaths() << " paths\n";

        if (enableOutput) {
            cout << "SNET " << tmpSNet.name_ << endl;
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
            layer_name_     = "";
            via_name_       = "";
            shape_         = "";
            hasBeginPoint = false;
            hasEndPoint   = false;
            beginX        = -1;
            beginY        = -1;
            begin_ext_      = -1;
            endX          = -1;
            endY          = -1;
            end_ext_        = -1;
            hasRect       = false;
            llx           = -1;
            lly           = -1;
            urx           = -1;
            ury           = -1;
            width_         = 0;
            //cout <<"path here" <<endl;

            int pathId;
            while ((pathId = path->next()) != DEFIPATH_DONE) {
                //cout << "  pathId = " << pathId << endl;
                switch(pathId) {
                    case DEFIPATH_LAYER:
                        tmpPath.layer_name_ = string(path->getLayer());
                        break;
                    case DEFIPATH_VIA:
                        tmpPath.via_name_ = string(path->getVia());
                        break;
                    case DEFIPATH_WIDTH:
                        tmpPath.GetWidth = path->GetWidth();
                        break;
                    case DEFIPATH_POINT:
                        if (!hasBeginPoint) {
                            path->getPoint(&beginX, &beginY);
                            hasBeginPoint = true;
                            tmpPath.begin_.x = beginX;
                            tmpPath.begin_.y = beginY;
                        } else {
                            path->getPoint(&endX, &endY);
                            hasEndPoint = true;
                            tmpPath.end_.x = endX;
                            tmpPath.end_.y = endY;
                        }
                        break;
                    case DEFIPATH_FLUSHPOINT:
                        if (!hasBeginPoint) {
                            path->getFlushPoint(&beginX, &beginY, &begin_ext_);
                            hasBeginPoint = true;
                            tmpPath.begin_.x = beginX;
                            tmpPath.begin_.y = beginY;
                            tmpPath.begin_ext_ = begin_ext_;
                        } else {
                            path->getFlushPoint(&endX, &endY, &end_ext_);
                            hasEndPoint = true;
                            tmpPath.end_.x = endX;
                            tmpPath.end_.y = endY;
                            tmpPath.end_ext_ = end_ext_;
                        }
                        break;
                    case DEFIPATH_SHAPE:
                        tmpPath.shape_ = path->getShape();
                        break;
                    case DEFIPATH_RECT:
                        path->getViaRect(&llx, &lly, &urx, &ury);
                        tmpPath.rect_.Set(llx, lly, urx, ury);
                        hasRect = true;
                        break;
                    case DEFIPATH_VIRTUALPOINT:
                        if (!hasBeginPoint) {
                            path->getVirtualPoint(&beginX, &beginY);
                            hasBeginPoint = true;

                            tmpPath.begin_.x = beginX;
                            tmpPath.begin_.y = beginY;
                        } else {
                            path->getVirtualPoint(&endX, &endY);
                            hasEndPoint = true;

                            tmpPath.end_.x = endX;
                            tmpPath.end_.y = endY;
                        }
                        break;
                    default : cout <<" net " <<net->name_() <<" unknown pathId " <<pathId <<endl; break;
                }
            }


            // add via
            /*if (via_name_ != "") {
              if (((io::Parser*)data)->tech->name2via.find(via_name_) == ((io::Parser*)data)->tech->name2via.end_()) {
                if (VERBOSE > -1) {
                  cout <<"Error: unsupported via: " <<via_name_ <<endl;
                }
              } else {
                frPoint p;
                if (hasEndPoint) {
                  p.Set(endX, endY);
                } else {
                  p.Set(beginX, beginY);
                }
                auto viaDef = ((io::Parser*)data)->tech->name2via[via_name_];
                auto tmpP = make_unique<frVia>(viaDef);
                tmpP->SetOrigin(p);
                tmpP->addToNet(netIn);
                netIn->addVia(tmpP);
              }
            }*/
    /*
            tmpSNet.paths.push_back(tmpPath);
        } // end_ path
    } // end_ wire

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

    parser::DefVia tmpVia;
    tmpVia.name_ = via->name_();

    // viaRule defined via
    if (via->hasViaRule()) {
        char* via_rule_name_;
        char* botLayer;
        char* cutLayer;
        char* topLayer;
        int xSize, ySize, xCutSpacing, yCutSpacing, xBotEnc, yBotEnc, xTopEnc, yTopEnc;

        via->viaRule(&via_rule_name_, &xSize, &ySize, &botLayer, &cutLayer, &topLayer,
                     &xCutSpacing, &yCutSpacing, &xBotEnc, &yBotEnc, &xTopEnc, &yTopEnc);
        tmpVia.via_rule_name_ = via_rule_name_;
        tmpVia.cut_size_.set(xSize, ySize);
        tmpVia.layers_[0] = string(botLayer);
        tmpVia.layers_[1] = string(cutLayer);
        tmpVia.layers_[2] = string(topLayer);

        tmpVia.cut_spacing_.set(xCutSpacing, yCutSpacing);

        tmpVia.bot_enc_.set(xBotEnc, yBotEnc);
        tmpVia.top_enc_.Set(xTopEnc, yTopEnc);

        int xOrigin = 0;
        int yOrigin = 0;
        if (via->hasOrigin()) {
            via->origin_(&xOrigin, &yOrigin);
        }
        tmpVia.origin_.Set(xOrigin, yOrigin);

        int xBotOffset = 0;
        int yBotOffset = 0;
        int xTopOffset = 0;
        int yTopOffset = 0;
        if (via->hasOffset()) {
            via->offset(&xBotOffset, &yBotOffset, &xTopOffset, &yTopOffset);
        }
        tmpVia.bot_offset_.Set(xBotOffset, yBotOffset);
        tmpVia.top_offset_.Set(xTopOffset, yTopOffset);

        int num_cut_rows_ = 1;
        int num_cut_cols_ = 1;
        if (via->hasRowCol()) {
            via->rowCol(&num_cut_rows_, &num_cut_cols_);
        }
        tmpVia.num_cut_rows_ = num_cut_rows_;
        tmpVia.num_cut_cols_ = num_cut_cols_;


    }
    else // RECT defined via
    {
        if (via->numPolygons()) {
            cout <<"Error: unsupport polygon in def via" <<endl;
            exit(1);
        }
        char* layer_name_;
        int xl;
        int yl;
        int xh;
        int yh;

        for (int i = 0; i < via->numLayers(); ++i) {
            via->layer(i, &layer_name_, &xl, &yl, &xh, &yh);
            parser::Rect2DLayer<int> tmpRect2DLayer;
            tmpRect2DLayer.Set(layer_name_, xl, yl, xh, yh);
        }
    }

    tmpVia.idx_ = ((parser::defDataBase*) data)->vias.size();
    ((parser::defDataBase*) data)->defVia2idx.insert( pair<string, int> (tmpVia.name_, tmpVia.idx_));
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
    ((Design *) data)->version_ = version;
    return 0;
}

int getDefBusBit(defrCallbackType_e type, const char *BusBit, defiUserData data) {
    ((Design *) data)->bus_bit_chars_ = BusBit;
    return 0;
}

int getDefDivider(defrCallbackType_e type, const char *divider, defiUserData data) {
    ((Design *) data)->divider_char_ = divider;
    return 0;
}

}
