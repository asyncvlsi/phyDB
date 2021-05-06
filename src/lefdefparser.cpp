#include "lefdefparser.h"
#include "logging.h"
#include <algorithm>
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
        PhyDBExpects(false, "SITE SIZE information not provided");
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

    double originX = macro->originX();
    double originY = macro->originY();
    double sizeX = macro->sizeX();
    double sizeY = macro->sizeY();
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

    std::string macro_name = last_macro.GetName();
    std::string pin_name(pin->name());
    std::string pin_direction(pin->direction());
    std::string pin_use(pin->use());
    Pin *pin_ptr = last_macro.AddPin(pin_name, StrToSignalDirection(pin_direction), StrToSignalUse(pin_use));

    if (enableOutput) {
        cout << "  PIN " << pin->name() << endl;
    }

    int numPorts = pin->numPorts();
    PhyDBExpects(numPorts > 0, "No physical pins, Macro: " + last_macro.GetName() + ", pin: " + pin_name);

    for (int i = 0; i < numPorts; ++i) {
        int numItems = pin->port(i)->numItems();

        if (enableOutput) {
            cout << "    PORT" << endl;
        }

        LayerRect *layer_rect_ptr = nullptr;
        for (int j = 0; j < numItems; ++j) {
            int itemType = pin->port(i)->itemType(j);
            if (itemType == 1) { //layer
                std::string layer_name(pin->port(i)->getLayer(j));
                layer_rect_ptr = pin_ptr->AddLayerRect(layer_name);
                if (enableOutput) {
                    cout << "    LAYER " << layer_name << " ;" << endl;
                }
            } else if (itemType == 8) {
                double x1 = pin->port(i)->getRect(j)->xl;
                double y1 = pin->port(i)->getRect(j)->yl;
                double x2 = pin->port(i)->getRect(j)->xh;
                double y2 = pin->port(i)->getRect(j)->yh;
                PhyDBExpects(layer_rect_ptr!= nullptr, "unexpected error in getLefPins()");
                layer_rect_ptr->AddRect(min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));

                if (enableOutput) {
                    cout << "      RECT " << min(x1, x2) << " " << min(y1, y2) << " " << max(x1, x2) << " " << max(y1, y2) << " ;" << endl;
                }
            } else {
                cout << "unsupported lefiGeometries!" << endl;
                continue;
                // exit(2);
            }
        }
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
    Macro &last_macro = phy_db_ptr->GetTechPtr()->GetMacrosRef().back(); // last macro

    LayerRect tmpLayerRect;
    Rect2D<double> tmpRect;

    if (enableOutput) {
        cout << "  OBS" << endl;
    }

    auto geometry = obs->geometries();
    int numItems = geometry->numItems();

    OBS* obs_ptr = last_macro.GetObs();

    int isNewLayerRect = true;
    LayerRect *layer_rect_ptr = nullptr;
    for (int i = 0; i < numItems; ++i) {
        if (geometry->itemType(i) == lefiGeomLayerE) {
            std::string layer_name(geometry->getLayer(i));
            layer_rect_ptr = obs_ptr->AddLayerRect(layer_name);
            if (enableOutput) {
                cout << "    LAYER " << tmpLayerRect.layer_name_ << " ;" << endl;
            }
        } else if (geometry->itemType(i) == lefiGeomRectE) {
            double x1 = geometry->getRect(i)->xl;
            double y1 = geometry->getRect(i)->yl;
            double x2 = geometry->getRect(i)->xh;
            double y2 = geometry->getRect(i)->yh;
            PhyDBExpects(layer_rect_ptr!= nullptr, "unexpected error in getLefObs()");
            layer_rect_ptr->AddRect(min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
            if (enableOutput) {
                cout << "      RECT " << min(x1, x2) << " " << min(y1, y2) << " " << max(x1, x2) << " " << max(y1, y2) << " ;" << endl;
            }
        } else {
            cout << "Warning: unsupported OBS" << endl;
            continue;
        }
    }

    return 0;
}

int getLefCornerSpacing(void *data, const string &stringProp) {
//TODO: This function is for the ISPD2018/19 benchmarks, we can handle this later
    /*istringstream istr(stringProp);
    string token;
    Layer &last_layer = phy_db_ptr->GetTechPtr()->GetLayersRef().back(); //write to the last one

    while (!istr.eof()) {
        istr >> token;
        if (token == "EXCEPTEOL") {
            istr >> tmpLayer.cornerSpacing.eolWidth;
        } else if (token == "WIDTH") {
            double width_;
            istr >> width_;
            tmpLayer.cornerSpacing.width_.push_back(GetWidth);
        } else if (token == "SPACING") {
            double spacing;
            istr >> spacing;
            tmpLayer.cornerSpacing.spacing.push_back(GetSpacing);
        }
    }*/
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
        string layer_type(layer->type());
        string direction(layer->direction());

        Layer &last_layer =
            *(phy_db_ptr->AddLayer(metal_layer_name, StrToLayerType(layer_type), StrToMetalDirection(direction)));

        last_layer.SetWidth(layer->width());
        if (layer->hasMinwidth()) {
            last_layer.SetMinWidth(layer->minwidth());
        } else {
            last_layer.SetMinWidth(layer->width());
        }

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
            double minLength = last_layer.area / last_layer.width_;
            int multiple = (minLength / grid) / 6;
            if (minLength == ((double) multiple) * grid * 6)
                last_layer.minLength = minLength;
            else {
                last_layer.minLength = (multiple + 1) * grid * 6;
            }
        }
        */
        if (enableOutput)
            cout << "Layer" << layer->name() << " number of props " << layer->numProps() << endl;
        if (layer->numProps() > 1) {
            cout << "Error: enable to handle more than one properties:" << layer->name() << endl;
            exit(1);
        }
        for (int i = 0; i < layer->numProps(); i++) {
            if (string(layer->propName(i)) == string("LEF58_CORNERSPACING") && layer->propIsString(i)) {
                getLefCornerSpacing(data, layer->propValue(i));
            } else {
                cout << "WARNING: UNSUPPORTED PROPERTY: " << layer->propName(i) << endl;
            }
        }

        // read spacing rule
        for (int i = 0; i < layer->numSpacing(); ++i) {
            EolSpacing tmpSpacing;
            double spacing = layer->spacing(i);
            double eol_width = 0, eol_within = 0, par_edge = 0, par_within = 0;
            if (layer->hasSpacingEndOfLine(i)) {

                if (enableOutput) {
                    cout << "  SPACING " << layer->spacing(i) << " ENDOFLINE " << layer->spacingEolWidth(i)
                         << " WITHIN " << layer->spacingEolWithin(i);
                }
                eol_width = layer->spacingEolWidth(i);
                eol_within = layer->spacingEolWithin(i);

                if (layer->hasSpacingParellelEdge(i)) {
                    if (enableOutput) {
                        cout << " PARALLELEDGE " << layer->spacingParSpace(i) << " WITHIN "
                             << layer->spacingParWithin(i);
                    }
                    par_edge = layer->spacingParSpace(i);
                    par_within = layer->spacingParWithin(i);
                }

                last_layer.AddEolSpacing(spacing, eol_width, eol_within, par_edge, par_within);

            } else {
                cout << "warning: no eol spacing!" << endl;
                last_layer.SetSpacing(spacing);
            }
        }

        // read spacingTable

        for (int i = 0; i < layer->numSpacingTable(); ++i) {
            auto spTable = layer->spacingTable(i);
            vector<double> v_parallel_run_length;
            vector<double> v_width;
            vector<double> v_spacing;
            if (spTable->isParallel() == 1) {
                auto parallel = spTable->parallel();

                if (enableOutput) {
                    cout << "  SPACINGTABLE" << endl;
                    cout << "  PARALLELRUNLENGTH";
                }
                for (int j = 0; j < parallel->numLength(); ++j) {
                    v_parallel_run_length.push_back(parallel->length(j));
                }
                for (int j = 0; j < parallel->numWidth(); ++j) {
                    v_width.push_back(parallel->width(j));
                    for (int k = 0; k < parallel->numLength(); ++k) {
                        v_spacing.push_back(parallel->widthSpacing(j, k));
                    }
                }
                if (enableOutput) {
                    cout << " ;" << endl;
                }
                last_layer.SetSpacingTable(
                    parallel->numLength(),
                    parallel->numWidth(),
                    v_parallel_run_length,
                    v_width,
                    v_spacing
                );

            } else if (spTable->isInfluence()) {
                auto influence = spTable->influence();
                for (int i = 0; i < influence->numInfluenceEntry(); i++) {
                    last_layer.AddSpacingTableInfluence(
                        influence->width(i),
                        influence->distance(i),
                        influence->spacing(i));
                }
            } else {
                cout << "unsupported spacing table!" << endl;
                exit(1);
            }
        }

    } else if (strcmp(layer->type(), "CUT") == 0) { // cut layer
        string metal_layer_name(layer->name());
        string layer_type(layer->type());

        Layer &last_layer = *(phy_db_ptr->AddLayer(metal_layer_name, StrToLayerType(layer_type)));

        last_layer.SetWidth(layer->width());
        // read spacing constraint
        for (int i = 0; i < layer->numSpacing(); ++i) {

            if (layer->hasSpacingAdjacent(i)) {
                double spacing = layer->spacing(i);
                int adjacent_cuts = layer->spacingAdjacentCuts(i);
                int cut_within = layer->spacingAdjacentWithin(i);
                last_layer.SetAdjCutSpacing(spacing, adjacent_cuts, cut_within);
            } else {
                last_layer.SetSpacing(layer->spacing(i));
            }
        }

    } else {
        if (string(layer->name()) != "OVERLAP")
            cout << "unsupported layer type: " << layer->name() << ": " << layer->type() << endl;
    }

    return 0;
}

int getLefVias(lefrCallbackType_e type, lefiVia *via, lefiUserData data) {
    bool enableOutput = false;
    if (type != lefrViaCbkType) {
        cout << "Type is not lefrViaCbkType!" << endl;
    }

    auto *phy_db_ptr = (PhyDB *) data;
    string via_name = via->name();
    LefVia &last_via = *(phy_db_ptr->AddLefVia(via_name));
    if (via->hasDefault())
        last_via.SetDefault();
    else
        last_via.UnsetDefault();

    if (enableOutput) {
        cout << "VIA " << via->name();
        if (via->hasDefault()) {
            cout << " DEFAULT";
        }
        cout << endl;
    }
    if (via->numLayers() != 3) {
        cout << "Error: unsupported via (via layers != 3) " << via->name() << endl;
        exit(1);
    }
    string layer_name[3];
    vector<Rect2D<double>> rects[3];
    for (int i = 0; i < via->numLayers(); ++i) {
        layer_name[i] = via->layerName(i);
        for (int j = 0; j < via->numRects(i); ++j) {
            rects[i].emplace_back(via->xl(i, j), via->yl(i, j), via->xh(i, j), via->yh(i, j));
        }
    }
    last_via.SetLayerRect(layer_name[0], rects[0], layer_name[1], rects[1], layer_name[2], rects[2]);

    return 0;
}

int getLefViaRuleGenerates(lefrCallbackType_e type, lefiViaRule *viaRule, lefiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    if (type != lefrViaRuleCbkType) {
        cout << "Type is not lefrViaRuleCbkType!" << endl;
    }

    string name = viaRule->name();

    auto *phy_db_ptr = (PhyDB *) data;
    ViaRuleGenerate &last_viarule_generate = *(phy_db_ptr->AddViaRuleGenerate(name));

    if (viaRule->hasDefault())
        last_viarule_generate.SetDefault();
    else
        last_viarule_generate.UnsetDefault();

    if (viaRule->numLayers() != 3) {
        cout << "Error: unsupported via" << endl;
        exit(1);
    }
    ViaRuleGenerateLayer layer[3];

    for (int i = 0; i < viaRule->numLayers(); ++i) {
        auto viaRuleLayer = viaRule->layer(i);
        string layer_name = viaRuleLayer->name();
        layer[i].SetLayerName(layer_name);
        if (viaRuleLayer->hasEnclosure()) {
            layer[i].SetEnclosure(viaRuleLayer->enclosureOverhang1(), viaRuleLayer->enclosureOverhang2());
        }

        if (viaRuleLayer->hasRect()) {
            layer[i].SetRect(viaRuleLayer->xl(), viaRuleLayer->yl(), viaRuleLayer->xh(), viaRuleLayer->xh());
        }

        if (viaRuleLayer->hasSpacing()) {
            layer[i].SetSpacing(viaRuleLayer->spacingStepX(), viaRuleLayer->spacingStepY());
        }
    }
    last_viarule_generate.SetLayers(layer[0], layer[1], layer[2]);

    return 0;
}

int getDefDesign(defrCallbackType_e type, const char *str, defiUserData data) {
    //bool enableOutput = true;
    bool enableOutput = false;
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
    bool enableOutput = false;
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
    bool enableOutput = false;
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

    std::string dir_str(track->macro());
    XYDirection direction = StrToXYDirection(dir_str);
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
            PhyDBExpects(false, "Unsupported count types: BOGUS");
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
    int llx = comp->placementX();
    int lly = comp->placementY();

    std::string place_status = "UNPLACED";
    if (comp->isPlaced()) {
        place_status = "PLACED";
    } else if (comp->isFixed()) {
        place_status = "FIXED";
    } else if (comp->isUnplaced()) {
        place_status = "UNPLACED";
        llx = 0;
        lly = 0;
    } else if (comp->isCover()) {
        place_status = "COVER";
    } else {
        llx = 0;
        lly = 0;
    }

    std::string orient(comp->placementOrientStr());

    auto *phy_db_ptr = (PhyDB *) data;
    phy_db_ptr->AddComponent(comp_name, macro_name,
                             StrToPlaceStatus(place_status), llx, lly,
                             StrToCompOrient(orient));

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

    auto *phy_db_ptr = (PhyDB *) data;
    IOPin *io_pin_ptr = phy_db_ptr->AddIoPin(iopin_name,
                                             StrToSignalDirection(signal_direction),
                                             StrToSignalUse(signal_use));

    int iopin_x = 0;
    int iopin_y = 0;
    PlaceStatus place_status = UNPLACED;
    CompOrient orient = N;
    if (pin->isPlaced()) {
        place_status = PLACED;
        iopin_x = pin->placementX();
        iopin_y = pin->placementY();
        std::string str_orient(pin->orientStr());
        orient = StrToCompOrient(str_orient);
    } else if (pin->isUnplaced()) {
        place_status = UNPLACED;
    } else if (pin->isFixed()) {
        place_status = FIXED;
        iopin_x = pin->placementX();
        iopin_y = pin->placementY();
        std::string str_orient(pin->orientStr());
        orient = StrToCompOrient(str_orient);
    } else if (pin->isCover()) {
        place_status = COVER;
        iopin_x = pin->placementX();
        iopin_y = pin->placementY();
        std::string str_orient(pin->orientStr());
        orient = StrToCompOrient(str_orient);
    }
    io_pin_ptr->SetPlacement(place_status, iopin_x, iopin_y, orient);

    if (pin->hasPort()) {
        cout << "Error: multiple pin ports existing in DEF" << endl;
        exit(1);
    } else {
        for (int i = 0; i < pin->numLayer(); ++i) {
            int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
            pin->bounds(i, &x1, &y1, &x2, &y2);
            std::string layer_name(pin->layer(i));
            io_pin_ptr->SetShape(layer_name, min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
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
    
    bool enableOutput = false;
    //bool enableOutput = true;
    if (type != defrSNetCbkType) {
        cout <<"Type is not defr(S)NetCbkType!" <<endl;
        exit(1);
    }
    auto *phy_db_ptr = (PhyDB *) data;
    string name = net->name();
    string use_str = net->use();
    SignalUse use = StrToSignalUse(use_str);

    auto* phydb_snet = phy_db_ptr->AddSNet(name, use);

    // read pre-route
    for (int i = 0; i < (int)net->numWires(); i++) {
        defiWire* tmpWire = net->wire(i);
        // each path is a def line
        for (int j = 0; j < (int)tmpWire->numPaths(); j++) {
            auto* phydb_path = phydb_snet->AddPath();
            defiPath* path     = tmpWire->path(j);
            path->initTraverse();
            // initialize

            int pathId;
            bool hasBeginPoint = false;
            while ((pathId = path->next()) != DEFIPATH_DONE) {
                //cout << "  pathId = " << pathId << endl;
                switch(pathId) {
                    case DEFIPATH_LAYER: {
                        string layer_name = string(path->getLayer());
                        phydb_path->SetLayerName(layer_name);
                        break;
                    }
                    case DEFIPATH_VIA: {
                        string via_name = string(string(path->getVia()));
                        phydb_path->SetViaName(via_name);
                        break;
                    }
                    case DEFIPATH_WIDTH: {
                        phydb_path->SetWidth(path->getWidth());
                        break;
                    }
                    case DEFIPATH_POINT: {
                        if (!hasBeginPoint) {
                            int beginX, beginY;
                            path->getPoint(&beginX, &beginY);
                            phydb_path->SetBegin(beginX, beginY);
                            hasBeginPoint = true;
                        } else {
                            int endX, endY;
                            path->getPoint(&endX, &endY);
                            phydb_path->SetEnd(endX, endY);
                        }
                        break;
                    }
                    case DEFIPATH_FLUSHPOINT: {
                        if (!hasBeginPoint) {
                            int beginX, beginY, begin_ext;
                            path->getFlushPoint(&beginX, &beginY, &begin_ext);
                            hasBeginPoint = true;
                            phydb_path->SetBegin(beginX, beginY);
                            phydb_path->SetBeginExt(begin_ext);
                        } else {
                            int endX, endY, end_ext;
                            path->getFlushPoint(&endX, &endY, &end_ext);
                            phydb_path->SetEnd(endX, endY);
                            phydb_path->SetEndExt(end_ext);
                        }
                        break;
                    }
                    case DEFIPATH_SHAPE: {
                        string shape = string(path->getShape());
                        phydb_path->SetShape(shape);
                        break;
                    }
                    case DEFIPATH_RECT: {
                        int x1, y1, x2, y2;
                        path->getViaRect(&x1, &y1, &x2, &y2);
                        phydb_path->SetRect(min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
                        break;
                    }
                    case DEFIPATH_VIRTUALPOINT: {
                        if (!hasBeginPoint) {
                            int beginX, beginY;
                            path->getPoint(&beginX, &beginY);
                            phydb_path->SetBegin(beginX, beginY);
                            hasBeginPoint = true;
                        } else {
                            int endX, endY;
                            path->getPoint(&endX, &endY);
                            phydb_path->SetEnd(endX, endY);
                        }
                        break;
                    }
                    default : { 
                        cout <<" net " <<net->name() <<" unknown pathId " <<pathId <<endl; 
                        break;
                    }
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
        } // end_ path
    } // end_ wire


    return 0;
}

int getDefVias(defrCallbackType_e type, defiVia *via, defiUserData data) {
    /* TODO: This can be handle layer since ACT does not provide defVia
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

int getDefGcellGrid(defrCallbackType_e type, defiGcellGrid *gcellGrid, defiUserData data) {
    auto *phy_db_ptr = (PhyDB *) data;
    string dir_str = string(gcellGrid->macro());
    XYDirection direction = StrToXYDirection(dir_str);
    phy_db_ptr->AddGcellGrid(direction, gcellGrid->x(), gcellGrid->xNum(), gcellGrid->xStep());
    return 0;
}

int getDefVersion(defrCallbackType_e type, double version, defiUserData data) {
    ((PhyDB *) data)->SetDefVersion(version);
    return 0;
}

int getDefBusBit(defrCallbackType_e type, const char *BusBit, defiUserData data) {
    std::string bus_bit_char(BusBit);
    ((PhyDB *) data)->SetDefBusBitChar(bus_bit_char);
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
    lefrSetViaRuleCbk(getLefViaRuleGenerates);

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
    defrSetGcellGridCbk(getDefGcellGrid);

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

    defrClear();
}


}


