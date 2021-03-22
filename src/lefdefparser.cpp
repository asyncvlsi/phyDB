#include "lefdefparser.h"

namespace phydb {

int getLefSite(lefrCallbackType_e type, lefiSite *site, lefiUserData data) {
    if (type != lefrSiteCbkType) {
        std::cout << "Type is not lefrSiteCbkType!" << std::endl;
        exit(2);
    }
    if (site->lefiSite::hasSize()) {
        auto *phy_db_ptr = (PhyDB *) data;
        std::string site_name(site->name());
        std::string site_class_name;
        if (site->hasClass()) {
            site_class_name = std::string(site->siteClass());
        }
        phy_db_ptr->AddSite(site_name, site_class_name, site->sizeX(), site->sizeY());
    } else {
        PhyDbExpects(false, "SITE SIZE information not provided");
    }
    return 0;
}

int getLefMacrosBegin(lefrCallbackType_e type, const char *str, lefiUserData data) {
    assert(type == lefrMacroBeginCbkType);

    auto *phy_db_ptr = (PhyDB *) data;
    string tmpMacroName(str);
    phy_db_ptr->AddMacro(tmpMacroName); //add an empty macro

    return 0;
}

int getLefMacros(lefrCallbackType_e type, lefiMacro *macro, lefiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    if ((type != lefrMacroCbkType)) {
        cout << "Type is not lefrMacroCbkType!" << endl;
        exit(2);
    }

    float originX = macro->originX();
    float originY = macro->originY();
    float sizeX = macro->sizeX();
    float sizeY = macro->sizeY();
    if (enableOutput) {
        cout << "  ORIGIN " << originX << " "
             << originY << " ;" << endl;
        cout << "  SIZE   " << sizeX << " "
             << sizeY << endl;
    }
    auto *phy_db_ptr = (PhyDB *) data;

    Macro &m = phy_db_ptr->GetTechPtr()->GetMacrosRef().back(); //write to the last one
    m.SetOrigin(originX, originY);
    m.SetSize(sizeX, sizeY);

    return 0;
}

int getLefMacrosEnd(lefrCallbackType_e type, const char *str, lefiUserData data) {
    // could probably delete this function

    assert(type == lefrMacroEndCbkType);
    return 0;
}

int getLefUnits(lefrCallbackType_e type, lefiUnits *units, lefiUserData data) {
    bool enableOutput = false;
    auto *phy_db_ptr = (PhyDB *) data;
    phy_db_ptr->SetDatabaseMicron(units->databaseNumber());
    if (enableOutput) {
        cout << "DATABASE MICRONS " << units->databaseNumber() << endl;
    }
    return 0;
}

int getLefManufacturingGrid(lefrCallbackType_e type, double number, lefiUserData data) {
    bool enableOutput = false;
    auto *phy_db_ptr = (PhyDB *) data;
    phy_db_ptr->SetManufacturingGrid(number);
    if (enableOutput) {
        cout << "MANUFACTURINGGRID " << number << endl;
    }
    return 0;
}

int getLefPins(lefrCallbackType_e type, lefiPin *pin, lefiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    auto *phy_db_ptr = (PhyDB *) data;
    if (type != lefrPinCbkType) {
        cout << "Type is not lefrPinCbkType!" << endl;
        exit(1);
    }

    Macro &last_macro = phy_db_ptr->GetTechPtr()->GetMacrosRef().back(); // last macro
    // TODO, this Callback function need to use API to add PIN to MACRO

    LayerRect tmpLayerRect;
    Rect2D<float> tmpRect;

    std::string pin_name(pin->name());
    std::string pin_direction(pin->direction());
    Pin *pin_ptr = last_macro.AddPin(pin_name, StrToSignalDirection(pin_direction));

    string pin_use(pin->use());
    pin_ptr->SetUse(pin_use);

    if (enableOutput) {
        cout << "  PIN " << pin->name() << endl;
    }

    int numPorts = pin->numPorts();
    PhyDbExpects(numPorts > 0, "No physical pins, Macro: " + last_macro.GetName() + ", pin: " + pin_name);

    for (int i = 0; i < numPorts; ++i) {
        int numItems = pin->port(i)->numItems();

        if (enableOutput) {
            cout << "    PORT" << endl;
        }

        int isNewLayerRect = true;

        for (int j = 0; j < numItems; ++j) {
            int itemType = pin->port(i)->itemType(j);
            if (itemType == 1) { //layer
                if (isNewLayerRect) {
                    isNewLayerRect = false;
                    tmpLayerRect.Reset();
                } else {
                    pin_ptr->AddLayerRect(tmpLayerRect);
                    tmpLayerRect.Reset();
                }
                tmpLayerRect.layer_name_ = pin->port(i)->getLayer(j);

                if (enableOutput) {
                    cout << "    LAYER " << tmpLayerRect.layer_name_ << " ;" << endl;
                }
            } else if (itemType == 8) {
                float llx = pin->port(i)->getRect(j)->xl;
                float lly = pin->port(i)->getRect(j)->yl;
                float urx = pin->port(i)->getRect(j)->xh;
                float ury = pin->port(i)->getRect(j)->yh;
                tmpRect.Set(llx, lly, urx, ury);

                tmpLayerRect.rects.push_back(tmpRect);

                if (enableOutput) {
                    cout << "      RECT " << llx << " " << lly << " " << urx << " " << ury << " ;" << endl;
                }
            } else {
                cout << "unsupported lefiGeometries!" << endl;
                continue;
                // exit(2);
            }
        }
        pin_ptr->AddLayerRect(tmpLayerRect);
    }

    //tmpPin.print();
    if (enableOutput) {
        cout << "  END " << pin->name() << endl;
    }

    //checkLargePin(tmpPin);

    return 0;
}

int getLefObs(lefrCallbackType_e type, lefiObstruction *obs, lefiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = FALSE;

    if (type != lefrObstructionCbkType) {
        cout << "Type is not lefrObstructionCbkType!" << endl;
        exit(1);
    }
    auto *phy_db_ptr = (PhyDB *) data;
    Macro &m = phy_db_ptr->GetTechPtr()->GetMacrosRef().back(); // last macro

    LayerRect tmpLayerRect;
    Rect2D<float> tmpRect;

    if (enableOutput) {
        cout << "  OBS" << endl;
    }

    auto geometry = obs->geometries();
    int numItems = geometry->numItems();

    int isNewLayerRect = true;
    for (int i = 0; i < numItems; ++i) {
        if (geometry->itemType(i) == lefiGeomLayerE) {
            if (isNewLayerRect) {
                isNewLayerRect = false;
                tmpLayerRect.Reset();
            } else {
                m.AddObsLayerRect(tmpLayerRect);
                tmpLayerRect.Reset();
            }
            tmpLayerRect.layer_name_ = geometry->getLayer(i);

            if (enableOutput) {
                cout << "    LAYER " << tmpLayerRect.layer_name_ << " ;" << endl;
            }
        } else if (geometry->itemType(i) == lefiGeomRectE) {

            float llx = geometry->getRect(i)->xl;
            float lly = geometry->getRect(i)->yl;
            float urx = geometry->getRect(i)->xh;
            float ury = geometry->getRect(i)->yh;
            tmpRect.Set(llx, lly, urx, ury);

            tmpLayerRect.rects.push_back(tmpRect);

            if (enableOutput) {
                cout << "      RECT " << llx << " " << lly << " " << urx << " " << ury << " ;" << endl;
            }
        } else {
            cout << "Warning: unsupported OBS" << endl;
            continue;
        }
    }
    m.AddObsLayerRect(tmpLayerRect);

    return 0;
}

int getLefCornerSpacing(void *data, const string &stringProp) {
    /*
    istringstream istr(stringProp);
    string token;
    auto &tmpLayer = ((parser::lefDataBase *) data)->tmpLayer;

    while (!istr.eof()) {
        istr >> token;
        if (token == "EXCEPTEOL") {
            istr >> tmpLayer.cornerSpacing.eolWidth;
        } else if (token == "WIDTH") {
            float width_;
            istr >> width_;
            tmpLayer.cornerSpacing.width_.push_back(GetWidth);
        } else if (token == "SPACING") {
            float spacing;
            istr >> spacing;
            tmpLayer.cornerSpacing.spacing.push_back(GetSpacing);
        }
    }
     */
    return 0;
}

int getLefLayers(lefrCallbackType_e type, lefiLayer *layer, lefiUserData data) {

    bool enableOutput = false;

    if (type != lefrLayerCbkType) {
        cout << "Type is not lefrLayerCbkType!" << endl;
        exit(1);
    }

    auto *phy_db_ptr = (PhyDB *) data;
    //Layer &last_layer = phy_db_ptr->GetTechPtr()->GetLayersRef().back(); //write to the last one

    if (strcmp(layer->type(), "ROUTING") == 0) {
        string metal_layer_name(layer->name());
        Layer &last_layer = *(phy_db_ptr->AddLayer(metal_layer_name));
        string metal_type(layer->type());
        last_layer.SetType(metal_type);
        last_layer.SetWidth(layer->width());
        if (layer->hasMinwidth()) {
            last_layer.SetMinWidth(layer->minwidth());
        } else {
            last_layer.SetMinWidth(layer->width());
        }
        string layer_direction(layer->direction());
        last_layer.SetDirection(layer_direction);

        if (layer->hasXYPitch()) {
            last_layer.SetPitch(layer->pitchX(), layer->pitchY());
        } else {
            last_layer.SetPitch(layer->pitch(), layer->pitch());
        }
        last_layer.SetOffset(layer->offset());

        // read minArea rule
        if (layer->hasArea()) {
            last_layer.SetArea(layer->area());
        }


        /* TODO: The following code might be useful for PWRoute
        double grid = ((parser::lefDataBase *) data)->manufacturingGrid;
        if (grid == 0)
            last_layer.minLength = last_layer.area / last_layer.width_;
        else {
            float minLength = last_layer.area / last_layer.width_;
            int multiple = (minLength / grid) / 6;
            if (minLength == ((float) multiple) * grid * 6)
                last_layer.minLength = minLength;
            else {
                last_layer.minLength = (multiple + 1) * grid * 6;
            }
        }
        */
        if (enableOutput)
            cout << "Layer" << layer->name_() << " number of props " << layer->numProps() << endl;
        if (layer->numProps() > 1) {
            cout << "Error: enable to handle more than one properties:" << layer->name_() << endl;
        }
        for (int i = 0; i < layer->numProps(); i++) {
            if (string(layer->propName(i)) == string("LEF58_CORNERSPACING") && layer->propIsString(i)) {
                getLefCornerSpacing(data, layer->propValue(i));
            } else {
                cout << "UNSUPPORTED PROPERTY: " << layer->propName(i) << endl;
            }
        }

        // read spacing rule
        for (int i = 0; i < layer->numSpacing(); ++i) {
            parser::Spacing tmpSpacing;
            tmpSpacing.spacing = layer->spacing(i);

            if (layer->hasSpacingEndOfLine(i)) {

                if (enableOutput) {
                    cout << "  SPACING " << layer->spacing(i) << " ENDOFLINE " << layer->spacingEolWidth(i)
                         << " WITHIN " << layer->spacingEolWithin(i);
                }
                tmpSpacing.eolWidth = layer->spacingEolWidth(i);
                tmpSpacing.eolWithin = layer->spacingEolWithin(i);

                if (layer->hasSpacingParellelEdge(i)) {
                    if (enableOutput) {
                        cout << " PARALLELEDGE " << layer->spacingParSpace(i) << " WITHIN "
                             << layer->spacingParWithin(i);
                    }
                    tmpSpacing.parEdge = layer->spacingParSpace(i);
                    tmpSpacing.parWithin = layer->spacingParWithin(i);
                }

                last_layer.spacings.push_back(tmpSpacing);

            } else {
                cout << "no eol spacing!" << endl;
            }
        }

        // read spacingTable
        if (layer->numSpacingTable() > 1)
            cout << "warning: More than one spacing table: " << last_layer.name_ << endl;

        for (int i = 0; i < layer->numSpacingTable(); ++i) {
            auto spTable = layer->spacingTable(i);
            if (spTable->isParallel() == 1) {
                //if (spTable->parallel()) {
                auto parallel = spTable->parallel();

                if (enableOutput) {
                    cout << "  SPACINGTABLE" << endl;
                    cout << "  PARALLELRUNLENGTH";
                }
                for (int j = 0; j < parallel->numLength(); ++j) {
                    last_layer.spacingTable.parallelRunLength.push_back(parallel->length(j));
                }
                for (int j = 0; j < parallel->numWidth(); ++j) {
                    last_layer.spacingTable.width_.push_back(parallel->GetWidth(j));
                    for (int k = 0; k < parallel->numLength(); ++k) {
                        last_layer.spacingTable.GetSpacing.push_back(parallel->widthSpacing(j, k));
                    }
                }
                if (enableOutput) {
                    cout << " ;" << endl;
                }
            } else if (spTable->isInfluence()) {

            } else {
                cout << "unsupported spacing table!" << endl;
            }
        }

        int layerIdx = ((parser::lefDataBase *) data)->layers_.size();
        last_layer.idx_ = layerIdx;
        ((parser::lefDataBase *) data)->layer2idx.insert(pair<string, int>(last_layer.name_, layerIdx));
        ((parser::lefDataBase *) data)->layers_.push_back(last_layer);
         */

    } else if (strcmp(layer->type(), "CUT") == 0) { // cut layer
        /*
        last_layer.name_ = layer->name_();
        last_layer.type = layer->type();
        last_layer.GetWidth = layer->width_();

        // read spacing constraint
        for (int i = 0; i < layer->numSpacing(); ++i) {

            if (layer->hasSpacingAdjacent(i)) {
                parser::Spacing tmpSpacing;
                tmpSpacing.spacing = layer->spacing(i);
                tmpSpacing.adjacentCuts = layer->spacingAdjacentCuts(i);
                tmpSpacing.cutWithin = layer->spacingAdjacentWithin(i);
                last_layer.spacings.push_back(tmpSpacing);
            } else {
                last_layer.spacing = layer->GetSpacing(i);
            }
        }

        int layerIdx = ((parser::lefDataBase *) data)->layers_.size();
        last_layer.idx_ = layerIdx;
        ((parser::lefDataBase *) data)->layer2idx.insert(pair<string, int>(last_layer.name_, layerIdx));
        ((parser::lefDataBase *) data)->layers_.push_back(last_layer);
        */
    } else {
        if (string(layer->name()) != "OVERLAP")
            cout << "unsupported layer type: " << layer->name() << ": " << layer->type() << endl;

    }

    return 0;
}

int getLefVias(lefrCallbackType_e type, lefiVia *via, lefiUserData data) {
    /*
    bool enableOutput = false;
    //bool enableOutput = true;
    if (type != lefrViaCbkType) {
        cout <<"Type is not lefrViaCbkType!" <<endl;
        // exit(1);
    }
    parser::lefVia tmpVia;

    tmpVia.name_ = via->name_();

    if (enableOutput) {
        cout <<"VIA " <<via->name_();
        if (via->hasDefault()) {
            cout <<" DEFAULT";
        }
        cout <<endl;
    }
    if (via->numLayers() != 3) {
        cout <<"Error: unsupported via: " << via->name_() << endl;
        exit(1);
    }
    string topLayerName;
    for (int i = 0; i < via->numLayers(); ++i) {
        parser::LayerRect tmpLayerRect;
        tmpLayerRect.layer_name_ = via->layer_name_(i);
        for (int j = 0; j < via->numRects(i); ++j)
        {
            parser::Rect2D<float> rect_;
            rect_.Set(via->xl(i, j), via->yl(i, j), via->xh(i, j), via->yh(i, j));
            tmpLayerRect.rects.push_back(rect_);
        }
        tmpVia.layer_rects_.push_back(tmpLayerRect);
        if(i == via->numLayers() - 1)
            topLayerName = via->layer_name_(i);
    }

    int viaIdx = ((parser::lefDataBase*) data)->vias.size();
    int topLayerIdx = ((parser::lefDataBase* ) data)->layer2idx[topLayerName];
    ((parser::lefDataBase*) data)->lefVia2idx.insert( pair<string, int> (tmpVia.name_, viaIdx));
    ((parser::lefDataBase*) data)->topLayerIdx2ViaIdx.insert( pair<int, int> (topLayerIdx, viaIdx));
    ((parser::lefDataBase*) data)->vias.push_back(tmpVia);

    if(enableOutput)
        tmpVia.print();
        */
    return 0;
}

int getLefViaGenerateRules(lefrCallbackType_e type, lefiViaRule *viaRule, lefiUserData data) {
    /*
    bool enableOutput = false;
    //bool enableOutput = true;
    if (type != lefrViaRuleCbkType) {
        cout <<"Type is not lefrViaRuleCbkType!" <<endl;
        // exit(1);
    }
    parser::ViaRuleGenerate tmpViaRule;

    if (viaRule->numLayers() != 3)
    {
        cout <<"Error: unsupported via" <<endl;
        exit(1);
    }
    tmpViaRule.name_ = viaRule->name_();

    for (int i = 0; i < viaRule->numLayers(); ++i)
    {
        auto viaRuleLayer = viaRule->layer(i);
        auto& layer = tmpViaRule.layers_[i];

        layer.layer_name_ = viaRuleLayer->name_();
        if(viaRuleLayer->hasEnclosure())
        {
            layer.enclosureOverhang.x = viaRuleLayer->enclosureOverhang1();
            layer.enclosureOverhang.y = viaRuleLayer->enclosureOverhang2();
        }

        if(viaRuleLayer->hasRect())
        {
            layer.rect_.Set(viaRuleLayer->xl(), viaRuleLayer->yl(), viaRuleLayer->xh(), viaRuleLayer->xh());
        }

        if(viaRuleLayer->hasSpacing())
        {
            layer.spacing.x = viaRuleLayer->spacingStepX();
            layer.GetSpacing.y = viaRuleLayer->spacingStepY();
        }

    }

    int viaRuleIdx = ((parser::lefDataBase*) data)->viaRuleGenerates.size();
    ((parser::lefDataBase*) data)->viaRuleGenerate2idx.insert( pair<string, int> (tmpViaRule.name_, viaRuleIdx));
    ((parser::lefDataBase*) data)->viaRuleGenerates.push_back(tmpViaRule);

    if(enableOutput)
        tmpViaRule.print();
    */
    return 0;
}

int getDefDesign(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    auto *phy_db_ptr = (PhyDB *) data;
    if (type == defrDesignStartCbkType) {
        std::string design_name(str);
        phy_db_ptr->SetDefName(design_name);

        if (enableOutput) {
            cout << "reading def: DESIGN " << string(str) << endl;
        }
    }
    return 0;
}

int getDefRow(defrCallbackType_e type, defiRow *row, defiUserData data) {
    //bool enableOutput = true;
    auto *phy_db_ptr = (PhyDB *) data;

    std::string row_name(row->name());
    std::string site_name(row->macro());
    std::string site_orientation(row->orientStr());
    int origX = row->x();
    int origY = row->y();
    int numX = row->xNum();
    int numY = row->yNum();
    int stepX = row->xStep();
    int stepY = row->yStep();

    phy_db_ptr->AddRow(row_name, site_name, site_orientation, origX, origY, numX, numY, stepX, stepY);

    return 0;
}

int getDefString(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = true;
    auto *phy_db_ptr = (PhyDB *) data;
    if ((type == defrDesignStartCbkType)) {
        std::string design_name(str);
        phy_db_ptr->SetDefName(design_name);

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
    auto *phy_db_ptr = (PhyDB *) data;
    if ((type != defrDieAreaCbkType)) {
        cout << "Type is not defrDieAreaCbkType!" << endl;
        exit(1);
    }
    phy_db_ptr->SetDieArea(box->xl(), box->yl(), box->xh(), box->yh());

    return 0;
}

int getDefUnits(defrCallbackType_e type, double number, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
    auto *phy_db_ptr = (PhyDB *) data;
    phy_db_ptr->SetUnitsDistanceMicrons(number);
    if (enableOutput) {
        cout << "UNITS DISTANCE MICRONS " << number << " ;" << endl;
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

    auto *phy_db_ptr = (PhyDB *) data;

    std::string direction(track->macro());
    int start = track->x();
    int num_tracks = track->xNum();
    int step = track->xStep();
    std::vector<std::string> layer_names;

    for (int i = 0; i < track->numLayers(); i++) {
        string layerName = track->layer(i);
        layer_names.push_back(layerName);
    }
    phy_db_ptr->AddTrack(direction, start, num_tracks, step, layer_names);

    return 0;
}

int getDefCountNumber(defrCallbackType_e type, int num, defiUserData data) {
    std::string name;
    auto *phy_db_ptr = (PhyDB *) data;
    switch (type) {
        case defrComponentStartCbkType : {
            name = "COMPONENTS";
            phy_db_ptr->SetComponentCount(num);
            break;
        }
        case defrStartPinsCbkType : {
            name = "PINS";
            phy_db_ptr->SetIoPinCount(num);
            break;
        }
        case defrNetStartCbkType : {
            name = "NETS";
            phy_db_ptr->SetNetCount(num);
            break;
        }
        default : {
            name = "BOGUS";
            PhyDbExpects(false, "Unsupported count types: BOGUS");
        }
    }
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

    auto *phy_db_ptr = (PhyDB *) data;
    phy_db_ptr->AddComponent(comp_name, macro_name, place_status, llx, lly, orient);

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

    auto *phy_db_ptr = (PhyDB *) data;
    IOPin *io_pin_ptr = phy_db_ptr->AddIoPin(iopin_name, place_status, signal_use, signal_direction);

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
    auto *phy_db_ptr = (PhyDB *) data;

    std::string net_name(net->name());
    phy_db_ptr->AddNet(net_name);

    if (enableOutput) {
        cout << "- " << net->name();
    }

    for (int i = 0; i < net->numConnections(); i++) {
        std::string comp_name(net->instance(i));
        std::string pin_name(net->pin(i));
        if (comp_name == "PIN") {
            phy_db_ptr->AddIoPinToNet(pin_name, net_name);
        } else {
            phy_db_ptr->AddCompPinToNet(comp_name, pin_name, net_name);
        }
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
              if (((io::Parser*)data)->phy_db_ptr->name2via.find(via_name_) == ((io::Parser*)data)->phy_db_ptr->name2via.end_()) {
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
                auto viaDef = ((io::Parser*)data)->phy_db_ptr->name2via[via_name_];
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
    ((PhyDB *) data)->SetDefVersion(version);
    return 0;
}

int getDefBusBit(defrCallbackType_e type, const char *BusBit, defiUserData data) {
    std::string bus_bit_chars(BusBit);
    ((PhyDB *) data)->SetDefBusBitChars(bus_bit_chars);
    return 0;
}

int getDefDivider(defrCallbackType_e type, const char *divider, defiUserData data) {
    std::string divider_chars(divider);
    ((PhyDB *) data)->SetDefDividerChar(divider_chars);
    return 0;
}

void Si2ReadLef(PhyDB *phy_db_ptr, string const &lefFileName) {
    FILE *f;
    int res;

    lefrInitSession(1);

    lefrSetUserData((lefiUserData) phy_db_ptr);

    lefrSetMacroCbk(getLefMacros);
    lefrSetMacroBeginCbk(getLefMacrosBegin);
    lefrSetMacroEndCbk(getLefMacrosEnd);
    lefrSetUnitsCbk(getLefUnits);
    lefrSetManufacturingCbk(getLefManufacturingGrid);
    lefrSetSiteCbk(getLefSite);
    lefrSetPinCbk(getLefPins);
    lefrSetObstructionCbk(getLefObs);
    lefrSetLayerCbk(getLefLayers);
    lefrSetViaCbk(getLefVias);
    lefrSetViaRuleCbk(getLefViaGenerateRules);

    if ((f = fopen(lefFileName.c_str(), "r")) == 0) {
        cout << "Couldn't open lef file" << endl;
        exit(2);
    }

    res = lefrRead(f, lefFileName.c_str(), (lefiUserData) phy_db_ptr);
    if (res != 0) {
        cout << "LEF parser returns an error!" << endl;
        exit(2);
    }
    fclose(f);

    lefrClear();
}

void Si2ReadDef(PhyDB *phy_db_ptr, string const &defFileName) {
    FILE *f;
    int res;

    defrInit();
    defrReset();

    defrInitSession(1);

    defrSetUserData((defiUserData) phy_db_ptr);

    defrSetVersionCbk(getDefVersion);
    defrSetBusBitCbk(getDefBusBit);
    defrSetDividerCbk(getDefDivider);

    defrSetDesignCbk(getDefString);
    defrSetDesignEndCbk(getDefVoid);
    defrSetDieAreaCbk(getDefDieArea);
    defrSetUnitsCbk(getDefUnits);
    defrSetRowCbk(getDefRow);
    defrSetTrackCbk(getDefTracks);

    defrSetComponentStartCbk(getDefCountNumber);
    defrSetComponentCbk(getDefComponents);

    defrSetStartPinsCbk(getDefCountNumber);
    defrSetPinCbk(getDefIOPins);

    defrSetNetStartCbk(getDefCountNumber);
    defrSetSNetCbk(getDefSNets);
    defrSetAddPathToNet();
    defrSetNetCbk(getDefNets);

    defrSetViaCbk(getDefVias);
    defrSetGcellGridCbk(getDefGcell);

    if ((f = fopen(defFileName.c_str(), "r")) == 0) {
        cout << "Couldn't open def file" << endl;
        exit(2);
    }

    res = defrRead(f, defFileName.c_str(), (defiUserData) phy_db_ptr, 1);
    if (res != 0) {
        cout << "DEF parser returns an error!" << endl;
        exit(2);
    }
    fclose(f);

    //numPins = readPinCnt;

    defrClear();
}

}


