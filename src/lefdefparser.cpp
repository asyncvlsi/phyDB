/*******************************************************************************
 *
 * Copyright (c) 2021 Jiayuan He, Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/
#include "lefdefparser.h"

#include <algorithm>

#include "logging.h"

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
    phy_db_ptr->AddSite(site_name,
                        site_class_name,
                        site->sizeX(),
                        site->sizeY());
  } else {
    PhyDBExpects(false, "SITE SIZE information not provided");
  }
  return 0;
}

int getLefMacrosBegin(
    lefrCallbackType_e type,
    const char *str,
    lefiUserData data
) {
  assert(type == lefrMacroBeginCbkType);

  auto *phy_db_ptr = (PhyDB *) data;
  std::string tmpMacroName(str);
  phy_db_ptr->AddMacro(tmpMacroName); //add an empty macro

  return 0;
}

int getLefMacros(lefrCallbackType_e type, lefiMacro *macro, lefiUserData data) {

  if ((type != lefrMacroCbkType)) {
    std::cout << "Type is not lefrMacroCbkType!" << std::endl;
    exit(2);
  }

  double originX = macro->originX();
  double originY = macro->originY();
  double sizeX = macro->sizeX();
  double sizeY = macro->sizeY();

  PhyDBExpects(macro->hasClass(),
               "Macro has no class?" + std::string(macro->name()));
  std::string str_macro_class(macro->macroClass());
  MacroClass macro_class = StrToMacroClass(str_macro_class);

  auto *phy_db_ptr = (PhyDB *) data;

  Macro &m =
      phy_db_ptr->GetTechPtr()->GetMacrosRef().back(); //write to the last one
  m.SetOrigin(originX, originY);
  m.SetSize(sizeX, sizeY);
  m.SetClass(macro_class);

  return 0;
}

int getLefMacrosEnd(
    lefrCallbackType_e type,
    const char *str,
    lefiUserData data
) {
  // could probably delete this function

  assert(type == lefrMacroEndCbkType);
  return 0;
}

int getLefUnits(lefrCallbackType_e type, lefiUnits *units, lefiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;

  int database_unit = (int) units->databaseNumber();
  phy_db_ptr->SetDatabaseMicron(database_unit);

  return 0;
}

int getLefManufacturingGrid(lefrCallbackType_e type,
                            double number,
                            lefiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;
  phy_db_ptr->SetManufacturingGrid(number);
  return 0;
}

int getLefPins(lefrCallbackType_e type, lefiPin *pin, lefiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;
  if (type != lefrPinCbkType) {
    std::cout << "Type is not lefrPinCbkType!" << std::endl;
    exit(1);
  }

  // last macro
  Macro &last_macro = phy_db_ptr->GetTechPtr()->GetMacrosRef().back();

  std::string pin_name(pin->name());
  std::string pin_direction(pin->direction());
  std::string pin_use(pin->use());
  Pin *pin_ptr = last_macro.AddPin(
      pin_name,
      StrToSignalDirection(pin_direction),
      StrToSignalUse(pin_use)
  );

  int numPorts = pin->numPorts();
  if (numPorts <= 0) {
    PhyDBExpects(false, "No physical ports? Macro: " +
        last_macro.GetName() + ", pin: " + pin_name);
  }

  for (int i = 0; i < numPorts; ++i) {
    int numItems = pin->port(i)->numItems();
    LayerRect *layer_rect_ptr = nullptr;
    for (int j = 0; j < numItems; ++j) {
      int itemType = pin->port(i)->itemType(j);
      if (itemType == 1) { //layer
        std::string layer_name(pin->port(i)->getLayer(j));
        layer_rect_ptr = pin_ptr->AddLayerRect(layer_name);
      } else if (itemType == 8) {
        double x1 = pin->port(i)->getRect(j)->xl;
        double y1 = pin->port(i)->getRect(j)->yl;
        double x2 = pin->port(i)->getRect(j)->xh;
        double y2 = pin->port(i)->getRect(j)->yh;
        PhyDBExpects(layer_rect_ptr != nullptr, "unexpected error");
        layer_rect_ptr->AddRect(
            std::min(x1, x2),
            std::min(y1, y2),
            std::max(x1, x2),
            std::max(y1, y2)
        );
      } else {
        std::cout << "unsupported lefiGeometries!\n";
        continue;
      }
    }
  }
  return 0;
}

int getLefObs(
    lefrCallbackType_e type,
    lefiObstruction *obs,
    lefiUserData data
) {

  if (type != lefrObstructionCbkType) {
    std::cout << "Type is not lefrObstructionCbkType!" << std::endl;
    exit(1);
  }
  auto *phy_db_ptr = (PhyDB *) data;
  Macro &last_macro =
      phy_db_ptr->GetTechPtr()->GetMacrosRef().back(); // last macro

  LayerRect tmpLayerRect;
  Rect2D<double> tmpRect;

  auto geometry = obs->geometries();
  int numItems = geometry->numItems();

  OBS *obs_ptr = last_macro.GetObs();

  int isNewLayerRect = true;
  LayerRect *layer_rect_ptr = nullptr;
  for (int i = 0; i < numItems; ++i) {
    if (geometry->itemType(i) == lefiGeomLayerE) {
      std::string layer_name(geometry->getLayer(i));
      layer_rect_ptr = obs_ptr->AddLayerRect(layer_name);
    } else if (geometry->itemType(i) == lefiGeomRectE) {
      double x1 = geometry->getRect(i)->xl;
      double y1 = geometry->getRect(i)->yl;
      double x2 = geometry->getRect(i)->xh;
      double y2 = geometry->getRect(i)->yh;
      PhyDBExpects(layer_rect_ptr != nullptr,
                   "unexpected error in getLefObs()");
      layer_rect_ptr->AddRect(
          std::min(x1, x2),
          std::min(y1, y2),
          std::max(x1, x2),
          std::max(y1, y2)
      );
    } else {
      std::cout << "Warning: unsupported OBS\n";
      continue;
    }
  }

  return 0;
}

int getLefCornerSpacing(void *data, const std::string &stringProp) {
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
  if (type != lefrLayerCbkType) {
    std::cout << "Type is not lefrLayerCbkType!" << std::endl;
    exit(1);
  }

  auto *phy_db_ptr = (PhyDB *) data;

  if (strcmp(layer->type(), "ROUTING") == 0) {
    std::string metal_layer_name(layer->name());
    std::string layer_type(layer->type());
    std::string direction(layer->direction());

    Layer &last_layer = *(phy_db_ptr->AddLayer(
        metal_layer_name,
        StrToLayerType(layer_type),
        StrToMetalDirection(direction))
    );

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

    if (layer->numProps() > 1) {
      std::cout << "ignore some unsupported properties for layer:"
                << layer->name() << std::endl;
    }
    for (int i = 0; i < layer->numProps(); i++) {
      if (std::string(layer->propName(i)) == "LEF58_CORNERSPACING"
          && layer->propIsString(i)) {
        getLefCornerSpacing(data, layer->propValue(i));
      } else {
        std::cout << "WARNING: UNSUPPORTED PROPERTY: "
                  << layer->propName(i) << "\n";
      }
    }

    // read spacing rule
    for (int i = 0; i < layer->numSpacing(); ++i) {
      EolSpacing tmpSpacing;
      double spacing = layer->spacing(i);
      double eol_width = 0, eol_within = 0, par_edge = 0, par_within = 0;
      if (layer->hasSpacingEndOfLine(i)) {

        eol_width = layer->spacingEolWidth(i);
        eol_within = layer->spacingEolWithin(i);

        if (layer->hasSpacingParellelEdge(i)) {
          par_edge = layer->spacingParSpace(i);
          par_within = layer->spacingParWithin(i);
        }

        last_layer.AddEolSpacing(
            spacing,
            eol_width,
            eol_within,
            par_edge,
            par_within
        );

      } else {
        std::cout << "warning: no eol spacing!\n";
        last_layer.SetSpacing(spacing);
      }
    }

    // read spacingTable

    for (int i = 0; i < layer->numSpacingTable(); ++i) {
      auto spTable = layer->spacingTable(i);
      std::vector<double> v_parallel_run_length;
      std::vector<double> v_width;
      std::vector<double> v_spacing;
      if (spTable->isParallel() == 1) {
        auto parallel = spTable->parallel();

        for (int j = 0; j < parallel->numLength(); ++j) {
          v_parallel_run_length.push_back(parallel->length(j));
        }
        for (int j = 0; j < parallel->numWidth(); ++j) {
          v_width.push_back(parallel->width(j));
          for (int k = 0; k < parallel->numLength(); ++k) {
            v_spacing.push_back(parallel->widthSpacing(j, k));
          }
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
        for (int entry = 0; entry < influence->numInfluenceEntry();
             entry++) {
          last_layer.AddSpacingTableInfluence(
              influence->width(entry),
              influence->distance(entry),
              influence->spacing(entry));
        }
      } else {
        std::cout << "ignore unsupported spacing table!" << std::endl;
        //exit(1);
      }
    }

  } else if (strcmp(layer->type(), "CUT") == 0) { // cut layer
    std::string metal_layer_name(layer->name());
    std::string layer_type(layer->type());

    Layer &last_layer = *(phy_db_ptr->AddLayer(metal_layer_name,
                                               StrToLayerType(layer_type)));

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
    if (std::string(layer->name()) != "OVERLAP")
      std::cout << "unsupported layer type: " << layer->name() << ": "
                << layer->type() << std::endl;
  }

  return 0;
}

int getLefVias(lefrCallbackType_e type, lefiVia *via, lefiUserData data) {
  if (type != lefrViaCbkType) {
    std::cout << "Type is not lefrViaCbkType!" << std::endl;
    exit(1);
  }

  auto *phy_db_ptr = (PhyDB *) data;
  std::string via_name = via->name();
  LefVia &last_via = *(phy_db_ptr->AddLefVia(via_name));
  if (via->hasDefault())
    last_via.SetDefault();
  else
    last_via.UnsetDefault();

  if (via->numLayers() != 3) {
    std::cout << "Error: unsupported via (via layers != 3) " << via->name()
              << std::endl;
    exit(1);
  }
  std::string layer_name[3];
  std::vector<Rect2D<double>> rects[3];
  for (int i = 0; i < via->numLayers(); ++i) {
    layer_name[i] = via->layerName(i);
    for (int j = 0; j < via->numRects(i); ++j) {
      rects[i].emplace_back(
          via->xl(i, j),
          via->yl(i, j),
          via->xh(i, j),
          via->yh(i, j)
      );
    }
  }
  last_via.SetLayerRect(
      layer_name[0],
      rects[0],
      layer_name[1],
      rects[1],
      layer_name[2],
      rects[2]
  );

  return 0;
}

int getLefViaRuleGenerates(lefrCallbackType_e type,
                           lefiViaRule *viaRule,
                           lefiUserData data) {

  if (type != lefrViaRuleCbkType) {
    std::cout << "Type is not lefrViaRuleCbkType!" << std::endl;
    exit(1);
  }

  std::string name = viaRule->name();

  auto *phy_db_ptr = (PhyDB *) data;
  ViaRuleGenerate
      &last_viarule_generate = *(phy_db_ptr->AddViaRuleGenerate(name));

  if (viaRule->hasDefault())
    last_viarule_generate.SetDefault();
  else
    last_viarule_generate.UnsetDefault();

  if (viaRule->numLayers() != 3) {
    std::cout << "Error: unsupported via" << std::endl;
    exit(1);
  }
  ViaRuleGenerateLayer layer[3];

  for (int i = 0; i < viaRule->numLayers(); ++i) {
    auto viaRuleLayer = viaRule->layer(i);
    std::string layer_name = viaRuleLayer->name();
    layer[i].SetLayerName(layer_name);
    if (viaRuleLayer->hasEnclosure()) {
      layer[i].SetEnclosure(
          viaRuleLayer->enclosureOverhang1(),
          viaRuleLayer->enclosureOverhang2()
      );
    }

    if (viaRuleLayer->hasRect()) {
      layer[i].SetRect(
          viaRuleLayer->xl(),
          viaRuleLayer->yl(),
          viaRuleLayer->xh(),
          viaRuleLayer->xh()
      );
    }

    if (viaRuleLayer->hasSpacing()) {
      layer[i].SetSpacing(
          viaRuleLayer->spacingStepX(),
          viaRuleLayer->spacingStepY()
      );
    }
  }
  last_viarule_generate.SetLayers(layer[0], layer[1], layer[2]);

  return 0;
}

int getDefDesign(defrCallbackType_e type, const char *str, defiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;
  if (type == defrDesignStartCbkType) {
    std::string design_name(str);
    phy_db_ptr->SetDefName(design_name);
  }
  return 0;
}

int getDefRow(defrCallbackType_e type, defiRow *row, defiUserData data) {
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

  phy_db_ptr->AddRow(
      row_name,
      site_name,
      site_orientation,
      origX,
      origY,
      numX,
      numY,
      stepX,
      stepY
  );

  return 0;
}

int getDefString(defrCallbackType_e type, const char *str, defiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;
  if (type == defrDesignStartCbkType) {
    std::string design_name(str);
    phy_db_ptr->SetDefName(design_name);
  }
  return 0;
}

int getDefVoid(defrCallbackType_e type, void *variable, defiUserData data) {
  return 0;
}

int getDefDieArea(defrCallbackType_e type, defiBox *box, defiUserData data) {

  auto *phy_db_ptr = (PhyDB *) data;
  if ((type != defrDieAreaCbkType)) {
    std::cout << "Type is not defrDieAreaCbkType!" << std::endl;
    exit(1);
  }
  phy_db_ptr->SetDieArea(box->xl(), box->yl(), box->xh(), box->yh());

  return 0;
}

int getDefUnits(defrCallbackType_e type, double number, defiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;
  phy_db_ptr->SetUnitsDistanceMicrons(number);
  return 0;
}

int getDefTracks(defrCallbackType_e type, defiTrack *track, defiUserData data) {
  if ((type != defrTrackCbkType)) {
    std::cout << "Type is not defrTrackCbkType!" << std::endl;
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
    std::string layerName = track->layer(i);
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

int getDefComponents(
    defrCallbackType_e type,
    defiComponent *comp,
    defiUserData data
) {
  if (type != defrComponentCbkType) {
    std::cout << "Type is not defrComponentCbkType!" << std::endl;
    exit(1);
  }

  std::string comp_name(comp->id());
  std::string macro_name(comp->name());
  int llx = comp->placementX();
  int lly = comp->placementY();

  PlaceStatus place_status = phydb::PlaceStatus::UNPLACED;
  if (comp->isPlaced()) {
    place_status = phydb::PlaceStatus::PLACED;
  } else if (comp->isFixed()) {
    place_status = phydb::PlaceStatus::FIXED;
  } else if (comp->isUnplaced()) {
    place_status = phydb::PlaceStatus::UNPLACED;
    llx = 0;
    lly = 0;
  } else if (comp->isCover()) {
    place_status = phydb::PlaceStatus::COVER;
  } else {
    llx = 0;
    lly = 0;
  }

  std::string orient;
  if (place_status == phydb::PlaceStatus::UNPLACED) {
    orient = "N";
  } else {
    orient = comp->placementOrientStr();
  }

  std::string source = "NETLIST";
  if (comp->hasSource()) {
    source = comp->source();
  }

  auto *phy_db_ptr = (PhyDB *) data;
  Macro *macro_ptr = phy_db_ptr->GetMacroPtr(macro_name);
  PhyDBExpects(macro_ptr != nullptr,
               "Cannot find " + macro_name + " in PhyDB");
  phy_db_ptr->AddComponent(
      comp_name, macro_ptr,
      place_status, llx, lly,
      StrToCompOrient(orient),
      StrToCompSource(source)
  );

  return 0;
}

int getDefIOPins(defrCallbackType_e type, defiPin *pin, defiUserData data) {
  if (type != defrPinCbkType) {
    std::cout << "Type is not defrPinCbkType!" << std::endl;
    exit(1);
  }

  std::string iopin_name(pin->pinName());
  std::string signal_direction;
  if (pin->hasDirection())
    signal_direction = std::string(pin->direction());
  std::string signal_use = "SIGNAL";
  if (pin->hasUse()) {
    signal_use = std::string(pin->use());
  }

  auto *phy_db_ptr = (PhyDB *) data;
  IOPin *io_pin_ptr = phy_db_ptr->AddIoPin(
      iopin_name,
      StrToSignalDirection(signal_direction),
      StrToSignalUse(signal_use)
  );

  int iopin_x = 0;
  int iopin_y = 0;
  PlaceStatus place_status = phydb::PlaceStatus::UNPLACED;
  CompOrient orient = phydb::CompOrient::N;
  if (pin->isPlaced()) {
    place_status = phydb::PlaceStatus::PLACED;
    iopin_x = pin->placementX();
    iopin_y = pin->placementY();
    std::string str_orient(pin->orientStr());
    orient = StrToCompOrient(str_orient);
  } else if (pin->isUnplaced()) {
    place_status = phydb::PlaceStatus::UNPLACED;
  } else if (pin->isFixed()) {
    place_status = phydb::PlaceStatus::FIXED;
    iopin_x = pin->placementX();
    iopin_y = pin->placementY();
    std::string str_orient(pin->orientStr());
    orient = StrToCompOrient(str_orient);
  } else if (pin->isCover()) {
    place_status = phydb::PlaceStatus::COVER;
    iopin_x = pin->placementX();
    iopin_y = pin->placementY();
    std::string str_orient(pin->orientStr());
    orient = StrToCompOrient(str_orient);
  }
  io_pin_ptr->SetPlacement(place_status, iopin_x, iopin_y, orient);

  if (pin->hasPort()) {
    std::cout << "Error: multiple pin ports existing in DEF" << std::endl;
    exit(1);
  } else {
    for (int i = 0; i < pin->numLayer(); ++i) {
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
      pin->bounds(i, &x1, &y1, &x2, &y2);
      std::string layer_name(pin->layer(i));
      io_pin_ptr->SetShape(
          layer_name,
          std::min(x1, x2),
          std::min(y1, y2),
          std::max(x1, x2),
          std::max(y1, y2)
      );
    }
  }

  return 0;

}

int getDefNets(defrCallbackType_e type, defiNet *net, defiUserData data) {
  if (type != defrNetCbkType) {
    std::cout << "Type is not defrNetCbkType!" << std::endl;
    exit(1);
  }
  auto *phy_db_ptr = (PhyDB *) data;

  std::string net_name(net->name());
  phy_db_ptr->AddNet(net_name);

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

  if (type != defrSNetCbkType) {
    std::cout << "Type is not defr(S)NetCbkType!" << std::endl;
    exit(1);
  }
  auto *phy_db_ptr = (PhyDB *) data;
  std::string name = net->name();
  std::string use_str = net->use();
  SignalUse use = StrToSignalUse(use_str);

  auto *phydb_snet = phy_db_ptr->AddSNet(name, use);

  for (int i = 0; i < (int) net->numPolygons(); i++) {
    std::string layer_name = net->polygonName(i);
    auto points = net->getPolygon(i);
    auto phydb_polygon = phydb_snet->AddPolygon(layer_name);
    for (int j = 0; j < points.numPoints; j++) {
      phydb_polygon->AddRoutingPoint(points.x[j], points.y[j]);
    }
  }

  // read pre-route
  for (int i = 0; i < (int) net->numWires(); i++) {
    defiWire *tmpWire = net->wire(i);
    // each path is a def line
    for (int j = 0; j < (int) tmpWire->numPaths(); j++) {
      auto *phydb_path = phydb_snet->AddPath();
      defiPath *path = tmpWire->path(j);
      path->initTraverse();
      // initialize

      int pathId;
      bool hasBeginPoint = false;
      while ((pathId = path->next()) != DEFIPATH_DONE) {
        //cout << "  pathId = " << pathId << endl;
        switch (pathId) {
          case DEFIPATH_LAYER: {
            auto layer_name = std::string(path->getLayer());
            phydb_path->SetLayerName(layer_name);
            break;
          }
          case DEFIPATH_VIA: {
            auto via_name = std::string(path->getVia());
            phydb_path->SetViaName(via_name);
            break;
          }
          case DEFIPATH_WIDTH: {
            phydb_path->SetWidth(path->getWidth());
            break;
          }
          case DEFIPATH_POINT: {
            int X, Y;
            path->getPoint(&X, &Y);
            phydb_path->AddRoutingPoint(X, Y);
            break;
          }
          case DEFIPATH_FLUSHPOINT: {
            int X, Y, ext;
            path->getFlushPoint(&X, &Y, &ext);
            phydb_path->AddRoutingPoint(X, Y, ext);
            break;
          }
          case DEFIPATH_SHAPE: {
            auto shape = std::string(path->getShape());
            phydb_path->SetShape(shape);
            break;
          }
          case DEFIPATH_RECT: {
            int x1, y1, x2, y2;
            path->getViaRect(&x1, &y1, &x2, &y2);
            phydb_path->SetRect(
                std::min(x1, x2),
                std::min(y1, y2),
                std::max(x1, x2),
                std::max(y1, y2)
            );
            break;
          }
          case DEFIPATH_VIRTUALPOINT: {
            int X, Y;
            path->getPoint(&X, &Y);
            phydb_path->AddRoutingPoint(X, Y);
            break;
          }
          default : {
            std::cout << " net " << net->name()
                      << " unknown pathId "
                      << pathId << "\n";
            break;
          }
        }
      }
    } // end_ path
  } // end_ wire


  return 0;
}

int getDefVias(defrCallbackType_e type, defiVia *via, defiUserData data) {
  //bool enableOutput = true;
  bool enableOutput = false;
  if ((type != defrViaCbkType)) {
      std::cout <<"Type is not defrViaCbkType!" << std::endl;
      exit(1);
  }

  auto *phy_db_ptr = (PhyDB *) data;
  std::string via_name = via->name();
  DefVia &last_via = *(phy_db_ptr->AddDefVia(via_name));
  // viaRule defined via
  if (via->hasViaRule()) {
      char* via_rule_name_;
      char* botLayer;
      char* cutLayer;
      char* topLayer;
      int xSize, ySize, xCutSpacing, yCutSpacing, xBotEnc, yBotEnc, xTopEnc, yTopEnc;

      via->viaRule(&via_rule_name_, &xSize, &ySize, &botLayer, &cutLayer, &topLayer,
                   &xCutSpacing, &yCutSpacing, &xBotEnc, &yBotEnc, &xTopEnc, &yTopEnc);
      last_via.via_rule_name_ = via_rule_name_;
      last_via.cut_size_.Set(xSize, ySize);
      last_via.layers_[0] = std::string(botLayer);
      last_via.layers_[1] = std::string(cutLayer);
      last_via.layers_[2] = std::string(topLayer);

      last_via.cut_spacing_.Set(xCutSpacing, yCutSpacing);

      last_via.bot_enc_.Set(xBotEnc, yBotEnc);
      last_via.top_enc_.Set(xTopEnc, yTopEnc);

      int xOrigin = 0;
      int yOrigin = 0;
      if (via->hasOrigin()) {
          via->origin(&xOrigin, &yOrigin);
      }
      last_via.origin_.Set(xOrigin, yOrigin);

      int xBotOffset = 0;
      int yBotOffset = 0;
      int xTopOffset = 0;
      int yTopOffset = 0;
      if (via->hasOffset()) {
          via->offset(&xBotOffset, &yBotOffset, &xTopOffset, &yTopOffset);
      }
      last_via.bot_offset_.Set(xBotOffset, yBotOffset);
      last_via.top_offset_.Set(xTopOffset, yTopOffset);

      int num_cut_rows_ = 1;
      int num_cut_cols_ = 1;
      if (via->hasRowCol()) {
          via->rowCol(&num_cut_rows_, &num_cut_cols_);
      }
      last_via.num_cut_rows_ = num_cut_rows_;
      last_via.num_cut_cols_ = num_cut_cols_;


  }
  else // RECT defined via
  {
      if (via->numPolygons()) {
          std::cout <<"Error: unsupport polygon in def via" << std::endl;
          exit(1);
      }
      char* layer_name_;
      int xl;
      int yl;
      int xh;
      int yh;

      for (int i = 0; i < via->numLayers(); ++i) {
          via->layer(i, &layer_name_, &xl, &yl, &xh, &yh);
          Rect2DLayer<int> tmpRect2DLayer;
          std::string layer_name(layer_name_);
          tmpRect2DLayer.Set(layer_name, xl, yl, xh, yh);
      }
      //TODO:
  }
  
  return 0;
}

int getDefGcellGrid(defrCallbackType_e type,
                    defiGcellGrid *gcellGrid,
                    defiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;
  auto dir_str = std::string(gcellGrid->macro());
  XYDirection direction = StrToXYDirection(dir_str);
  phy_db_ptr->AddGcellGrid(
      direction,
      gcellGrid->x(),
      gcellGrid->xNum(),
      gcellGrid->xStep()
  );
  return 0;
}

int getDefVersion(defrCallbackType_e type, double version, defiUserData data) {
  ((PhyDB *) data)->SetDefVersion(version);
  return 0;
}

int getDefBusBit(defrCallbackType_e type,
                 const char *BusBit,
                 defiUserData data) {
  std::string bus_bit_char(BusBit);
  ((PhyDB *) data)->SetDefBusBitChar(bus_bit_char);
  return 0;
}

int getDefDivider(defrCallbackType_e type,
                  const char *divider,
                  defiUserData data) {
  std::string divider_chars(divider);
  ((PhyDB *) data)->SetDefDividerChar(divider_chars);
  return 0;
}

void Si2ReadLef(PhyDB *phy_db_ptr, std::string const &lef_file_name) {
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

  if ((f = fopen(lef_file_name.c_str(), "r")) == nullptr) {
    std::cout << "Couldn't open lef file" << std::endl;
    exit(2);
  }

  res = lefrRead(f, lef_file_name.c_str(), (lefiUserData) phy_db_ptr);
  if (res != 0) {
    std::cout << "LEF parser returns an error!" << std::endl;
    exit(2);
  }
  fclose(f);

  lefrClear();
}

void Si2ReadDef(PhyDB *phy_db_ptr, std::string const &def_file_name) {
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

  if ((f = fopen(def_file_name.c_str(), "r")) == 0) {
    std::cout << "Couldn't open def file" << std::endl;
    exit(2);
  }

  res = defrRead(f, def_file_name.c_str(), (defiUserData) phy_db_ptr, 1);
  if (res != 0) {
    std::cout << "DEF parser returns an error!" << std::endl;
    exit(2);
  }
  fclose(f);

  defrClear();
}

/**
 * Check the unit in the DEF file, and check if they have the same value.
 * In general, they can be different, but in our flow, they are supposed to be the same.
 *
 * @param type, callback type.
 * @param number, UNITS DISTANCE MICRONS in DEF.
 * @param data, void pointer to PhyDB.
 * @return 0 for success.
 */
int CheckDefUnits(defrCallbackType_e type, double number, defiUserData data) {
  auto *phy_db_ptr = (PhyDB *) data;

  int cur_unit = (int) number;
  int existing_unit = phy_db_ptr->GetDesignPtr()->unit_distance_micron_;
  PhyDBExpects(cur_unit == existing_unit,
               "UNITS DISTANCE MICRONS is not supposed to be changed in the placed DEF file");

  return 0;
}

/**
 * For a certain component, check if this component exists in PhyDB.
 * If yes, override its placement info by the new one.
 *
 * @param type, callback type.
 * @param comp, pointer to the component created by the parser.
 * @param data, void pointer to PhyDB.
 * @return 0 for success.
 */
int LoadDefComponentLoc(defrCallbackType_e type,
                        defiComponent *comp,
                        defiUserData data) {
  if ((type != defrComponentCbkType)) {
    std::cout << "Type is not defrComponentCbkType!" << std::endl;
    exit(1);
  }

  std::string comp_name(comp->id());
  std::string macro_name(comp->name());
  int llx = comp->placementX();
  int lly = comp->placementY();

  PlaceStatus place_status = phydb::PlaceStatus::UNPLACED;
  if (comp->isPlaced()) {
    place_status = phydb::PlaceStatus::PLACED;
  } else if (comp->isFixed()) {
    place_status = phydb::PlaceStatus::FIXED;
  } else if (comp->isUnplaced()) {
    place_status = phydb::PlaceStatus::UNPLACED;
    llx = 0;
    lly = 0;
  } else if (comp->isCover()) {
    place_status = phydb::PlaceStatus::COVER;
  } else {
    llx = 0;
    lly = 0;
  }

  std::string orient(comp->placementOrientStr());

  auto *phy_db_ptr = (PhyDB *) data;
  PhyDBExpects(phy_db_ptr->IsComponentExisting(comp_name),
               "Component " + comp_name + " is not in PhyDB database");
  Component *comp_ptr = phy_db_ptr->GetComponentPtr(comp_name);
  comp_ptr->SetLocation(llx, lly);
  comp_ptr->SetOrientation(StrToCompOrient(orient));
  comp_ptr->SetPlacementStatus(place_status);

  return 0;
}

/**
 * Load a DEF file with placed components, and then override component locations in PhyDB.
 * This is useful for detailed placement and legalization of (multi-height) standard cell designs,
 * because Dali does not support detailed placement and legalization for multi-height standard cells.
 *
 * @param phy_db_ptr, the pointer to the PhyDB database.
 * @param def_file_name, the DEF file name which contains new component locations.
 * @return void.
 */
void Si2LoadPlacedDef(PhyDB *phy_db_ptr, std::string const &def_file_name) {
  FILE *f;
  int res;

  defrInit();
  defrReset();

  defrInitSession(1);

  defrSetUserData((defiUserData) phy_db_ptr);

  defrSetUnitsCbk(CheckDefUnits);

  defrSetComponentStartCbk(getDefCountNumber);
  defrSetComponentCbk(LoadDefComponentLoc);

  if ((f = fopen(def_file_name.c_str(), "r")) == nullptr) {
    std::cout << "Couldn't open def file" << std::endl;
    exit(2);
  }

  res = defrRead(f, def_file_name.c_str(), (defiUserData) phy_db_ptr, 1);
  if (res != 0) {
    std::cout << "DEF parser returns an error!" << std::endl;
    exit(2);
  }
  fclose(f);

  defrClear();
}

}


