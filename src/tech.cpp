#include "tech.h"

#include <cstring>

namespace phydb {

void Tech::SetDatabaseMicron(int database_micron) {
    PhyDbExpects(database_micron > 0, "Cannot Set negative database microns: Tech::SetDatabaseMicron()");
    dbu_per_micron_ = database_micron;
}

void Tech::SetManufacturingGrid(double manufacture_grid) {
    PhyDbExpects(manufacture_grid > 0, "Cannot Set negative manufacturing grid: Tech::SetManufacturingGrid()");
    manufacturing_grid_ = manufacture_grid;
}

void Tech::AddSite(std::string const &name, std::string const &class_name, double width, double height) {
    sites_.emplace_back(name, class_name, width, height);
}

void Tech::SetPlacementGrids(double placement_grid_value_x, double placement_grid_value_y) {
    PhyDbExpects(placement_grid_value_x > 0 && placement_grid_value_y > 0, "negative placement grid value not allowed");
    placement_grid_value_x_ = placement_grid_value_x;
    placement_grid_value_y_ = placement_grid_value_y;
    is_placement_grid_set_ = true;
}

bool Tech::IsLayerExist(std::string const &layer_name) {
    return layer_2_id_.find(layer_name) != layer_2_id_.end();
}

Layer *Tech::AddLayer(std::string &layer_name) {
    PhyDbExpects(!IsLayerExist(layer_name), "LAYER name_ exists, cannot use again: " + layer_name);
    int id = layers_.size();
    layers_.emplace_back(layer_name);
    layer_2_id_[layer_name] = id;
    return &(layers_[id]);
}

Layer *Tech::GetLayerPtr(std::string const &layer_name) {
    if (!IsLayerExist(layer_name)) {
        return nullptr;
    }
    int id = layer_2_id_[layer_name];
    return &(layers_[id]);
}

int Tech::GetLayerId(const string& layer_name) {
    if (!IsLayerExist(layer_name)) {
        return -1;
    }
    int id = layer_2_id_[layer_name];
    return id;
}

void Tech::ReportLayers() {
    std::cout << "Total number of layer: " << layers_.size() << "\n";
    for (auto &layer: layers_) {
        layer.Report();
    }
    std::cout << "\n";
}

bool Tech::IsMacroExist(std::string const &macro_name) {
    return macro_2_id_.find(macro_name) != macro_2_id_.end();
}

Macro *Tech::AddMacro(std::string &macro_name) {
    PhyDbExpects(!IsMacroExist(macro_name), "Macro name_ exists, cannot use it again: " + macro_name);
    int id = macros_.size();
    macros_.emplace_back(macro_name);
    macro_2_id_[macro_name] = id;
    return &(macros_[id]);
}

Macro *Tech::GetMacroPtr(std::string const &macro_name) {
    if (!IsMacroExist(macro_name)) {
        return nullptr;
    }
    int id = macro_2_id_[macro_name];
    return &(macros_[id]);
}

bool Tech::IsLefViaExist(std::string const &via_name) {
    return via_2_id_.find(via_name) != via_2_id_.end();
}

LefVia *Tech::AddLefVia(std::string &via_name) {
    PhyDbExpects(!IsLefViaExist(via_name), "VIA name_ exists, cannot use it again: " + via_name);
    int id = vias_.size();
    vias_.emplace_back(via_name);
    via_2_id_[via_name] = id;
    return &(vias_[id]);
}

LefVia *Tech::GetLefViaPtr(std::string const &via_name) {
    if (!IsLefViaExist(via_name)) {
        return nullptr;
    }
    int id = via_2_id_[via_name];
    return &(vias_[id]);
}

bool Tech::IsViaRuleGenerateExist(std::string const &name) {
    return via_rule_generate_2_id_.find(name) != via_rule_generate_2_id_.end();
}

ViaRuleGenerate *Tech::AddViaRuleGenerate(std::string &name) {
    PhyDbExpects(!IsViaRuleGenerateExist(name), "Macro name_ exists, cannot use it again");
    int id = via_rule_generates_.size();
    via_rule_generates_.emplace_back(name);
    via_rule_generate_2_id_[name] = id;
    return &(via_rule_generates_[id]);
}

ViaRuleGenerate *Tech::GetViaRuleGeneratePtr(std::string const &name) {
    if (!IsViaRuleGenerateExist(name)) {
        return nullptr;
    }
    int id = via_rule_generate_2_id_[name];
    return &(via_rule_generates_[id]);
}

void Tech::SetNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    if (is_n_well_layer_set_) {
        n_layer_ptr_->SetParams(width, spacing, op_spacing, max_plug_dist, overhang);
    } else {
        n_layer_ptr_ = new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_n_well_layer_set_ = true;
    }
}

void Tech::SetPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    if (is_p_well_layer_set_) {
        p_layer_ptr_->SetParams(width, spacing, op_spacing, max_plug_dist, overhang);
    } else {
        p_layer_ptr_ = new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_p_well_layer_set_ = true;
    }
}

void Tech::SetNpwellSpacing(double same_diff, double any_diff) {
    PhyDbExpects(same_diff >= 0 && any_diff >= 0, "Negative values not allowed: Tech::SetNpwellSpacing()");
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
        tech->AddSite(site_name, site_class_name, site->sizeX(), site->sizeY());
    } else {
        PhyDbExpects(false, "SITE SIZE information not provided");
    }
    return 0;
}

int getLefMacrosBegin(lefrCallbackType_e type, const char *str, lefiUserData data) {
    assert(type == lefrMacroBeginCbkType);

    Tech *tech = (Tech *) data;
    string tmpMacroName(str);
    tech->AddMacro(tmpMacroName); //add an empty macro

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
    int nMacros = tech->macros_.size();

    Macro &m = tech->macros_.back(); //write to the last one
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
    Tech *tech = (Tech *) data;
    tech->dbu_per_micron_ = units->databaseNumber();
    if (enableOutput) {
        cout << "DATABASE MICRONS " << ((Tech *) data)->dbu_per_micron_ << endl;
    }
    return 0;
}

int getLefManufacturingGrid(lefrCallbackType_e type, double number, lefiUserData data) {
    bool enableOutput = false;
    Tech *tech = (Tech *) data;
    tech->manufacturing_grid_ = number;
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
    int nMacros = tech->macros_.size();
    // TODO: confirm this bug is fixed
    Macro &tmpMacro = tech->macros_.back(); //write to the last one

    Pin tmpPin;
    LayerRect tmpLayerRect;
    Rect2D<float> tmpRect;

    string pin_name(pin->name());
    tmpPin.SetName(pin_name);

    string pin_use(pin->use());
    tmpPin.SetUse(pin_use);

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
                    tmpLayerRect.Reset();
                } else {
                    tmpPin.AddLayerRect(tmpLayerRect);
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
        tmpPin.AddLayerRect(tmpLayerRect);
    }
    tmpMacro.AddPin(tmpPin);

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
    int nMacros = tech->macros_.size();
    Macro &tmpMacro = tech->macros_.back(); //write to the last one

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
                tmpMacro.AddObsLayerRect(tmpLayerRect);
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
    tmpMacro.AddObsLayerRect(tmpLayerRect);

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

    Tech *tech = (Tech *) data;
    int nLayers = tech->layers_.size();
    Layer &tmpLayer = tech->layers_.back(); //write to the last one


    if (strcmp(layer->type(), "ROUTING") == 0) {
        string metal_layer_name(layer->name());
        tmpLayer.SetName(metal_layer_name);
        string metal_type(layer->type());
        tmpLayer.SetType(metal_type);
        tmpLayer.SetWidth(layer->width());
        if (layer->hasMinwidth()) {
            tmpLayer.SetMinWidth(layer->minwidth());
        } else {
            tmpLayer.SetMinWidth(layer->width());
        }
        string layer_direction(layer->direction());
        tmpLayer.SetDirection(layer_direction);

        if (layer->hasXYPitch()) {
            tmpLayer.SetPitch(layer->pitchX(), layer->pitchY());
        } else {
            tmpLayer.SetPitch(layer->pitch(), layer->pitch());
        }
        tmpLayer.SetOffset(layer->offset());

        // read minArea rule
        if (layer->hasArea()) {
            tmpLayer.SetArea(layer->area());
        }
        /*
        double grid = ((parser::lefDataBase *) data)->manufacturingGrid;
        if (grid == 0)
            tmpLayer.minLength = tmpLayer.area / tmpLayer.width_;
        else {
            float minLength = tmpLayer.area / tmpLayer.width_;
            int multiple = (minLength / grid) / 6;
            if (minLength == ((float) multiple) * grid * 6)
                tmpLayer.minLength = minLength;
            else {
                tmpLayer.minLength = (multiple + 1) * grid * 6;
            }
        }

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

                tmpLayer.spacings.push_back(tmpSpacing);

            } else {
                cout << "no eol spacing!" << endl;
            }
        }

        // read spacingTable
        if (layer->numSpacingTable() > 1)
            cout << "warning: More than one spacing table: " << tmpLayer.name_ << endl;

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
                    tmpLayer.spacingTable.width_.push_back(parallel->GetWidth(j));
                    for (int k = 0; k < parallel->numLength(); ++k) {
                        tmpLayer.spacingTable.GetSpacing.push_back(parallel->widthSpacing(j, k));
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
        tmpLayer.idx_ = layerIdx;
        ((parser::lefDataBase *) data)->layer2idx.insert(pair<string, int>(tmpLayer.name_, layerIdx));
        ((parser::lefDataBase *) data)->layers_.push_back(tmpLayer);
         */

    } else if (strcmp(layer->type(), "CUT") == 0) { // cut layer
        /*
        tmpLayer.name_ = layer->name_();
        tmpLayer.type = layer->type();
        tmpLayer.GetWidth = layer->width_();

        // read spacing constraint
        for (int i = 0; i < layer->numSpacing(); ++i) {

            if (layer->hasSpacingAdjacent(i)) {
                parser::Spacing tmpSpacing;
                tmpSpacing.spacing = layer->spacing(i);
                tmpSpacing.adjacentCuts = layer->spacingAdjacentCuts(i);
                tmpSpacing.cutWithin = layer->spacingAdjacentWithin(i);
                tmpLayer.spacings.push_back(tmpSpacing);
            } else {
                tmpLayer.spacing = layer->GetSpacing(i);
            }
        }

        int layerIdx = ((parser::lefDataBase *) data)->layers_.size();
        tmpLayer.idx_ = layerIdx;
        ((parser::lefDataBase *) data)->layer2idx.insert(pair<string, int>(tmpLayer.name_, layerIdx));
        ((parser::lefDataBase *) data)->layers_.push_back(tmpLayer);
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

}

