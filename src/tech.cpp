#include "tech.h"

#include <cstring>

namespace phydb {

void Tech::setDatabaseMicron(int database_micron) {
    PhyDbExpects(database_micron > 0, "Cannot set negative database microns: Tech::setDatabaseMicron()");
    _dbuPerMicron = database_micron;
}

void Tech::setManufacturingGrid(double manufacture_grid) {
    PhyDbExpects(manufacture_grid > 0, "Cannot set negative manufacturing grid: Circuit::setManufacturingGrid()");
    _manufacturingGrid = manufacture_grid;
}

void Tech::addSite(std::string const &name, std::string const &class_name, double width, double height) {
    _sites.emplace_back(name, class_name, width, height);
}

void Tech::setPlacementGrids(double placement_grid_value_x, double placement_grid_value_y) {
    if (is_placement_grid_set_) {
        std::cout << "Once set, grid_value cannot be changed!\n";
        return;
    }
    if (placement_grid_value_x_ <= 0 || placement_grid_value_y_ <= 0) {
        std::cout << "ERROR: negative placement grid value not allowed\n" << std::endl;
        exit(1);
    }
    placement_grid_value_x_ = placement_grid_value_x;
    placement_grid_value_y_ = placement_grid_value_y;
    is_placement_grid_set_ = true;
}

bool Tech::isLayerExist(std::string const &layer_name) {
    return _layer2id.find(layer_name) != _layer2id.end();
}

Layer *Tech::addLayer(std::string &layer_name) {
    PhyDbExpects(!isLayerExist(layer_name), "Layer name exists, cannot use again");
    int id = _layers.size();
    _layers.emplace_back(layer_name);
    _layer2id[layer_name] = id;
    return &(_layers[id]);
}

Layer *Tech::getLayerPtr(std::string const &layer_name) {
    if (!isLayerExist(layer_name)) {
        return nullptr;
    }
    int id = _layer2id[layer_name];
    return &(_layers[id]);
}

void Tech::ReportLayers() {
    std::cout << "Total number of layer: " << _layers.size() << "\n";
    for (auto &layer: _layers) {
        layer.Report();
    }
    std::cout << "\n";
}

bool Tech::isMacroExist(std::string const &macro_name) {
    return _macro2id.find(macro_name) != _macro2id.end();
}

Macro *Tech::addMacro(std::string &macro_name) {
    PhyDbExpects(!isMacroExist(macro_name), "Macro name exists, cannot use it again");
    int id = _macros.size();
    _macros.emplace_back(macro_name);
    _macro2id[macro_name] = id;
    return &(_macros[id]);
}

Macro &Tech::getMacro(string const &macro_name) {
    if (!isMacroExist(macro_name)) {
        std::cout << "Tech::getMacro, ERROR: Macro name does not exist, " + macro_name << std::endl;
        exit(1);
    }
    int id = _macro2id[macro_name];
    return _macros[id];
}

Macro *Tech::getMacroPtr(std::string const &macro_name) {
    if (!isMacroExist(macro_name)) {
        return nullptr;
    }
    int id = _macro2id[macro_name];
    return &(_macros[id]);
}

void Tech::setNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    if (is_n_well_layer_set_) {
        n_layer_ptr_->set_params(width, spacing, op_spacing, max_plug_dist, overhang);
    } else {
        n_layer_ptr_ = new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_n_well_layer_set_ = true;
    }
}

void Tech::setPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    if (is_p_well_layer_set_) {
        p_layer_ptr_->set_params(width, spacing, op_spacing, max_plug_dist, overhang);
    } else {
        p_layer_ptr_ = new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_p_well_layer_set_ = true;
    }
}

void Tech::setNpwellSpacing(double same_diff, double any_diff) {
    if (same_diff < 0 || any_diff < 0) {
        std::cout << "Negative values not allowed: Tech::setNpwellSpacing()" << std::endl;
        exit(1);
    }
    same_diff_spacing_ = same_diff;
    any_diff_spacing_ = any_diff;
}

bool Tech::IsWellInfoSet() const {
    return (n_layer_ptr_ == nullptr) && (p_layer_ptr_ == nullptr);
}

void Tech::ReportWellShape() {
    for (auto &well: wells_) {
        well.Report();
    }
}

int getLefSite(lefrCallbackType_e type, lefiSite *site, lefiUserData data) {
    if (type != lefrSiteCbkType) {
        std::cout << "Type is not lefrSiteCbkType!" << std::endl;
        exit(2);
    }
    if (site->lefiSite::hasSize()) {
        Tech *tech = (Tech *) data;
        std::string site_name(site->name());
        std::string site_class_name;
        if (site->hasClass()) {
            site_class_name = std::string(site->siteClass());
        }
        tech->addSite(site_name, site_class_name, site->sizeX(), site->sizeY());
    } else {
        PhyDbExpects(false, "SITE SIZE information not provided");
    }
    return 0;
}

int getLefMacrosBegin(lefrCallbackType_e type, const char *str, lefiUserData data) {
    assert(type == lefrMacroBeginCbkType);

    Tech *tech = (Tech *) data;
    string tmpMacroName(str);
    tech->addMacro(tmpMacroName); //add an empty macro

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
    Tech *tech = (Tech *) data;
    int nMacros = tech->_macros.size();

    Macro &m = tech->_macros.back(); //write to the last one
    m.setOrigin(originX, originY);
    m.setSize(sizeX, sizeY);

    return 0;
}

int getLefMacrosEnd(lefrCallbackType_e type, const char *str, lefiUserData data) {
    // could probably delete this function

    assert(type == lefrMacroEndCbkType);
    return 0;
}

int getLefUnits(lefrCallbackType_e type, lefiUnits *units, lefiUserData data) {
    bool enableOutput = false;
    Tech *tech = (Tech *) data;
    tech->_dbuPerMicron = units->databaseNumber();
    if (enableOutput) {
        cout << "DATABASE MICRONS " << ((Tech *) data)->_dbuPerMicron << endl;
    }
    return 0;
}

int getLefManufacturingGrid(lefrCallbackType_e type, double number, lefiUserData data) {
    bool enableOutput = false;
    Tech *tech = (Tech *) data;
    tech->_manufacturingGrid = number;
    if (enableOutput) {
        cout << "MANUFACTURINGGRID " << number << endl;
    }
    return 0;
}

int getLefPins(lefrCallbackType_e type, lefiPin *pin, lefiUserData data) {
    bool enableOutput = false;
    //bool enableOutput = true;
    Tech *tech = (Tech *) data;
    if (type != lefrPinCbkType) {
        cout << "Type is not lefrPinCbkType!" << endl;
        exit(1);
    }

    // term
    int nMacros = tech->_macros.size();
    // TODO: confirm this bug is fixed
    Macro &tmpMacro = tech->_macros.back(); //write to the last one

    Pin tmpPin;
    LayerRect tmpLayerRect;
    Rect2D<float> tmpRect;

    string pin_name(pin->name());
    tmpPin.setName(pin_name);

    string pin_use(pin->use());
    tmpPin.setUse(pin_use);

    if (enableOutput) {
        cout << "  PIN " << pin->name() << endl;
    }

    int numPorts = pin->numPorts();

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
                    tmpLayerRect.reset();
                } else {
                    tmpPin.addLayerRect(tmpLayerRect);
                    tmpLayerRect.reset();
                }
                tmpLayerRect.layerName = pin->port(i)->getLayer(j);

                if (enableOutput) {
                    cout << "    LAYER " << tmpLayerRect.layerName << " ;" << endl;
                }
            } else if (itemType == 8) {
                float llx = pin->port(i)->getRect(j)->xl;
                float lly = pin->port(i)->getRect(j)->yl;
                float urx = pin->port(i)->getRect(j)->xh;
                float ury = pin->port(i)->getRect(j)->yh;
                tmpRect.set(llx, lly, urx, ury);

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
        tmpPin.addLayerRect(tmpLayerRect);
    }
    tmpMacro.addPin(tmpPin);

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
    Tech *tech = (Tech *) data;
    int nMacros = tech->_macros.size();
    Macro &tmpMacro = tech->_macros.back(); //write to the last one

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
                tmpLayerRect.reset();
            } else {
                tmpMacro.addObsLayerRect(tmpLayerRect);
                tmpLayerRect.reset();
            }
            tmpLayerRect.layerName = geometry->getLayer(i);

            if (enableOutput) {
                cout << "    LAYER " << tmpLayerRect.layerName << " ;" << endl;
            }
        } else if (geometry->itemType(i) == lefiGeomRectE) {

            float llx = geometry->getRect(i)->xl;
            float lly = geometry->getRect(i)->yl;
            float urx = geometry->getRect(i)->xh;
            float ury = geometry->getRect(i)->yh;
            tmpRect.set(llx, lly, urx, ury);

            tmpLayerRect.rects.push_back(tmpRect);

            if (enableOutput) {
                cout << "      RECT " << llx << " " << lly << " " << urx << " " << ury << " ;" << endl;
            }
        } else {
            cout << "Warning: unsupported OBS" << endl;
            continue;
        }
    }
    tmpMacro.addObsLayerRect(tmpLayerRect);

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
            float width;
            istr >> width;
            tmpLayer.cornerSpacing.width.push_back(width);
        } else if (token == "SPACING") {
            float spacing;
            istr >> spacing;
            tmpLayer.cornerSpacing.spacing.push_back(spacing);
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

    Tech *tech = (Tech *) data;
    int nLayers = tech->_layers.size();
    Layer &tmpLayer = tech->_layers.back(); //write to the last one


    if (strcmp(layer->type(), "ROUTING") == 0) {
        string metal_layer_name(layer->name());
        tmpLayer.setName(metal_layer_name);
        string metal_type(layer->type());
        tmpLayer.setType(metal_type);
        tmpLayer.setWidth(layer->width());
        if (layer->hasMinwidth()) {
            tmpLayer.setMinWidth(layer->minwidth());
        } else {
            tmpLayer.setMinWidth(layer->width());
        }
        string layer_direction(layer->direction());
        tmpLayer.setDirection(layer_direction);

        if (layer->hasXYPitch()) {
            tmpLayer.setPitch(layer->pitchX(), layer->pitchY());
        } else {
            tmpLayer.setPitch(layer->pitch(), layer->pitch());
        }
        tmpLayer.setOffset(layer->offset());

        // read minArea rule
        if (layer->hasArea()) {
            tmpLayer.setArea(layer->area());
        }
        /*
        double grid = ((parser::lefDataBase *) data)->manufacturingGrid;
        if (grid == 0)
            tmpLayer.minLength = tmpLayer.area / tmpLayer.width;
        else {
            float minLength = tmpLayer.area / tmpLayer.width;
            int multiple = (minLength / grid) / 6;
            if (minLength == ((float) multiple) * grid * 6)
                tmpLayer.minLength = minLength;
            else {
                tmpLayer.minLength = (multiple + 1) * grid * 6;
            }
        }

        if (enableOutput)
            cout << "Layer" << layer->name() << " number of props " << layer->numProps() << endl;
        if (layer->numProps() > 1) {
            cout << "Error: enable to handle more than one properties:" << layer->name() << endl;
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

                tmpLayer.spacings.push_back(tmpSpacing);

            } else {
                cout << "no eol spacing!" << endl;
            }
        }

        // read spacingTable
        if (layer->numSpacingTable() > 1)
            cout << "warning: More than one spacing table: " << tmpLayer.name << endl;

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
                    tmpLayer.spacingTable.parallelRunLength.push_back(parallel->length(j));
                }
                for (int j = 0; j < parallel->numWidth(); ++j) {
                    tmpLayer.spacingTable.width.push_back(parallel->width(j));
                    for (int k = 0; k < parallel->numLength(); ++k) {
                        tmpLayer.spacingTable.spacing.push_back(parallel->widthSpacing(j, k));
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

        int layerIdx = ((parser::lefDataBase *) data)->layers.size();
        tmpLayer.idx = layerIdx;
        ((parser::lefDataBase *) data)->layer2idx.insert(pair<string, int>(tmpLayer.name, layerIdx));
        ((parser::lefDataBase *) data)->layers.push_back(tmpLayer);
         */

    } else if (strcmp(layer->type(), "CUT") == 0) { // cut layer
        /*
        tmpLayer.name = layer->name();
        tmpLayer.type = layer->type();
        tmpLayer.width = layer->width();

        // read spacing constraint
        for (int i = 0; i < layer->numSpacing(); ++i) {

            if (layer->hasSpacingAdjacent(i)) {
                parser::Spacing tmpSpacing;
                tmpSpacing.spacing = layer->spacing(i);
                tmpSpacing.adjacentCuts = layer->spacingAdjacentCuts(i);
                tmpSpacing.cutWithin = layer->spacingAdjacentWithin(i);
                tmpLayer.spacings.push_back(tmpSpacing);
            } else {
                tmpLayer.spacing = layer->spacing(i);
            }
        }

        int layerIdx = ((parser::lefDataBase *) data)->layers.size();
        tmpLayer.idx = layerIdx;
        ((parser::lefDataBase *) data)->layer2idx.insert(pair<string, int>(tmpLayer.name, layerIdx));
        ((parser::lefDataBase *) data)->layers.push_back(tmpLayer);
        */
    } else {
        if (string(layer->name()) != "OVERLAP")
            cout << "unsupported layer type: " << layer->name() << ": " << layer->type() << endl;

    }

    if (enableOutput)
        tmpLayer.Report();

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

    tmpVia.name = via->name();

    if (enableOutput) {
        cout <<"VIA " <<via->name();
        if (via->hasDefault()) {
            cout <<" DEFAULT";
        }
        cout <<endl;
    }
    if (via->numLayers() != 3) {
        cout <<"Error: unsupported via: " << via->name() << endl;
        exit(1);
    }
    string topLayerName;
    for (int i = 0; i < via->numLayers(); ++i) {
        parser::LayerRect tmpLayerRect;
        tmpLayerRect.layerName = via->layerName(i);
        for (int j = 0; j < via->numRects(i); ++j)
        {
            parser::Rect2D<float> rect;
            rect.set(via->xl(i, j), via->yl(i, j), via->xh(i, j), via->yh(i, j));
            tmpLayerRect.rects.push_back(rect);
        }
        tmpVia.layerRects.push_back(tmpLayerRect);
        if(i == via->numLayers() - 1)
            topLayerName = via->layerName(i);
    }

    int viaIdx = ((parser::lefDataBase*) data)->vias.size();
    int topLayerIdx = ((parser::lefDataBase* ) data)->layer2idx[topLayerName];
    ((parser::lefDataBase*) data)->lefVia2idx.insert( pair<string, int> (tmpVia.name, viaIdx));
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
    tmpViaRule.name = viaRule->name();

    for (int i = 0; i < viaRule->numLayers(); ++i)
    {
        auto viaRuleLayer = viaRule->layer(i);
        auto& layer = tmpViaRule.layers[i];

        layer.layerName = viaRuleLayer->name();
        if(viaRuleLayer->hasEnclosure())
        {
            layer.enclosureOverhang.x = viaRuleLayer->enclosureOverhang1();
            layer.enclosureOverhang.y = viaRuleLayer->enclosureOverhang2();
        }

        if(viaRuleLayer->hasRect())
        {
            layer.rect.set(viaRuleLayer->xl(), viaRuleLayer->yl(), viaRuleLayer->xh(), viaRuleLayer->xh());
        }

        if(viaRuleLayer->hasSpacing())
        {
            layer.spacing.x = viaRuleLayer->spacingStepX();
            layer.spacing.y = viaRuleLayer->spacingStepY();
        }

    }

    int viaRuleIdx = ((parser::lefDataBase*) data)->viaRuleGenerates.size();
    ((parser::lefDataBase*) data)->viaRuleGenerate2idx.insert( pair<string, int> (tmpViaRule.name, viaRuleIdx));
    ((parser::lefDataBase*) data)->viaRuleGenerates.push_back(tmpViaRule);

    if(enableOutput)
        tmpViaRule.print();
    */
    return 0;
}

}

