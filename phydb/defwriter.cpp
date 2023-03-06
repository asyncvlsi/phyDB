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
#include "defwriter.h"

#include <ctime>

#include <def/defrReader.hpp>
#include <def/defwWriter.hpp>
#include <def/defwWriterCalls.hpp>
#include <lef/lefrReader.hpp>
#include <lef/lefwWriter.hpp>
#include <lef/lefwWriterCalls.hpp>

#include "phydb/common/logging.h"

void CheckStatus(int status) {
  if (status) {
    defwPrintError(status);
  }
}

namespace phydb {

int WriteVersion(defwCallbackType_e type, defiUserData ud) {
  if (type != defwVersionCbkType) {
    std::cout << "Type is not defwVersionCbkType!" << std::endl;
    exit(2);
  }
  double version = ((PhyDB *) ud)->GetDefVersion();

  int int_version = version * 10;
  int status = defwVersion(int_version / 10, int_version % 10);
  CheckStatus(status);
  defwNewLine();
  return 0;
}

int WriteBusBit(defwCallbackType_e type, defiUserData ud) {
  if (type != defwBusBitCbkType) {
    std::cout << "Type is not defwBusBitCbkType!" << std::endl;
    exit(2);
  }
  int status = defwBusBitChars(((PhyDB *) ud)->GetDefBusBitChar().c_str());
  CheckStatus(status);
  defwNewLine();
  return 0;
}

int WriteDivider(defwCallbackType_e type, defiUserData ud) {
  if (type != defwDividerCbkType) {
    std::cout << "Type is not defwDividerCbkType!" << std::endl;
    exit(2);
  }
  int status = defwDividerChar(((PhyDB *) ud)->GetDefDividerChar().c_str());
  CheckStatus(status);
  defwNewLine();
  return 0;
}

int WriteDesignName(defwCallbackType_e type, defiUserData ud) {
  if (type != defwDesignCbkType) {
    std::cout << "Type is not defwDesignCbkType!" << std::endl;
    exit(2);
  }
  int status = defwDesignName(((PhyDB *) ud)->GetDefName().c_str());
  CheckStatus(status);
  defwNewLine();
  return 0;
}

int WriteDesignEnd(defwCallbackType_e type, defiUserData ud) {
  if (type != defwDesignEndCbkType) {
    std::cout << "Type is not defwDesignEndCbkType!" << std::endl;
    exit(2);
  }
  defwNewLine();
  int status = defwEnd();
  CheckStatus(status);
  return 0;
}

int WriteUnitsDistanceMicrons(defwCallbackType_e type, defiUserData ud) {
  if (type != defwUnitsCbkType) {
    std::cout << "Type is not defwUnitsCbkType!" << std::endl;
    exit(2);
  }
  int status =
      defwUnits(((PhyDB *) ud)->GetDesignPtr()->GetUnitsDistanceMicrons());
  CheckStatus(status);
  defwNewLine();
  return 0;
}

int WriteDieArea(defwCallbackType_e type, defiUserData ud) {
  if (type != defwDieAreaCbkType) {
    std::cout << "Type is not defwDieAreaCbkType!" << std::endl;
    exit(2);
  }

  std::vector<Point2D<int>> &die_area =
      ((PhyDB *) ud)->RectilinearPolygonDieAreaRef();
  int num_points = static_cast<int>(die_area.size());
  std::vector<int> x(num_points, 0);
  std::vector<int> y(num_points, 0);
  for (int i = 0; i < num_points; ++i) {
    x[i] = die_area[i].x;
    y[i] = die_area[i].y;
  }

  int status = 0;
  if (num_points == 2) {
    status = defwDieArea(x[0], y[0], x[1], y[1]);
  } else if (num_points >= 4) {
    status = defwDieAreaList(num_points, x.data(), y.data());
  }
  CheckStatus(status);
  defwNewLine();
  return 0;
}

int WriteRows(defwCallbackType_e type, defiUserData data) {
  if (type != defwRowCbkType) {
    std::cout << "Type is not defwRowCbkType!" << std::endl;
    exit(2);
  }
  auto rows = ((PhyDB *) data)->GetRowVec();
  auto sites = ((PhyDB *) data)->GetSitesRef();
  for (auto &row : rows) {
    int status = defwRowStr(
        row.GetName().c_str(),
        sites[row.GetSiteId()].GetName().c_str(),
        row.GetOriginX(),
        row.GetOriginY(),
        CompOrientStr(row.GetOrient()).c_str(),
        row.GetNumX(),
        row.GetNumY(),
        row.GetStepX(),
        row.GetStepY()
    );
    CheckStatus(status);
  }
  defwNewLine();
  return 0;
}

int WriteTracks(defwCallbackType_e type, defiUserData data) {
  if (type != defwTrackCbkType) {
    std::cout << "Type is not defwTrackCbkType!" << std::endl;
    exit(2);
  }
  auto tracks = ((PhyDB *) data)->GetTracksRef();
  for (auto &track : tracks) {
    int nlayers = static_cast<int>(track.GetLayerNames().size());
    const char **layer_names = new const char *[nlayers];
    for (int i = 0; i < nlayers; i++) {
      layer_names[i] = track.GetLayerNames()[i].c_str();
    }
    int status = defwTracks(
        XYDirectionStr(track.GetDirection()).c_str(),
        track.GetStart(),
        track.GetNTracks(),
        track.GetStep(),
        nlayers,
        layer_names
    );
    CheckStatus(status);
  }
  defwNewLine();
  return 0;
}

int WriteGcellGrids(defwCallbackType_e type, defiUserData data) {
  if (type != defwGcellGridCbkType) {
    std::cout << "Type is not defwGcellGridCbkType!" << std::endl;
    exit(2);
  }
  auto gcell_grids = ((PhyDB *) data)->GetGcellGridsRef();
  for (auto &gcellgrid : gcell_grids) {
    int status = defwGcellGrid(
        XYDirectionStr(gcellgrid.GetDirection()).c_str(),
        gcellgrid.GetStart(),
        gcellgrid.GetNBoundaries(),
        gcellgrid.GetStep()
    );
    CheckStatus(status);
  }
  defwNewLine();
  return 0;
}

int WriteComponents(defwCallbackType_e type, defiUserData data) {
  if (type != defwComponentCbkType) {
    std::cout << "Type is not defwComponentCbkType!" << std::endl;
    exit(2);
  }
  auto &components = ((PhyDB *) data)->GetDesignPtr()->GetComponentsRef();
  auto &fillers = ((PhyDB *) data)->GetDesignPtr()->GetFillersRef();
  int total_count = static_cast<int>(components.size() + fillers.size());
  int status = defwStartComponents(total_count);
  CheckStatus(status);

  for (auto &comp : components) {
    status = defwComponentStr(
        comp.GetName().c_str(),
        comp.GetMacro()->GetName().c_str(),
        0,
        NULL, NULL, NULL, NULL,
        comp.GetSourceStr().c_str(),
        0,
        NULL, NULL, NULL, NULL,
        PlaceStatusStr(comp.GetPlacementStatus()).c_str(),
        comp.GetLocation().x, comp.GetLocation().y,
        CompOrientStr(comp.GetOrientation()).c_str(),
        0,
        NULL,
        0, 0,
        0, 0
    );
    CheckStatus(status);
  }
  for (auto &comp : fillers) {
    status = defwComponentStr(
        comp.GetName().c_str(),
        comp.GetMacro()->GetName().c_str(),
        0,
        NULL, NULL, NULL, NULL,
        comp.GetSourceStr().c_str(),
        0,
        NULL, NULL, NULL, NULL,
        PlaceStatusStr(comp.GetPlacementStatus()).c_str(),
        comp.GetLocation().x, comp.GetLocation().y,
        CompOrientStr(comp.GetOrientation()).c_str(),
        0,
        NULL,
        0, 0,
        0, 0
    );
    CheckStatus(status);
  }
  status = defwEndComponents();
  CheckStatus(status);

  return 0;
}

int WriteIOPins(defwCallbackType_e type, defiUserData data) {
  if (type != defwPinCbkType) {
    std::cout << "Type is not defwPinCbkType!" << std::endl;
    exit(2);
  }

  Design *design_ptr = ((PhyDB *) data)->GetDesignPtr();
  auto iopins = design_ptr->GetIoPinsRef();
  int status = defwStartPins(static_cast<int>(iopins.size()));
  CheckStatus(status);

  for (auto &pin : iopins) {
    Net &net = design_ptr->GetNetsRef()[pin.GetNetId()];
    status = defwPinStr(
        pin.GetName().c_str(),
        net.GetName().c_str(),
        0,
        SignalDirectionStr(pin.GetDirection()).c_str(),
        SignalUseStr(pin.GetUse()).c_str(),
        PlaceStatusStr(pin.GetPlacementStatus()).c_str(),
        pin.GetLocation().x,
        pin.GetLocation().y,
        CompOrientStr(pin.GetOrientation()).c_str(),
        pin.GetLayerName().c_str(),
        pin.GetRect().LLX(),
        pin.GetRect().LLY(),
        pin.GetRect().URX(),
        pin.GetRect().URY()
    );
    CheckStatus(status);
  }
  status = defwEndPins();
  CheckStatus(status);

  return 0;
}

int WriteBlockages(defwCallbackType_e type, defiUserData data) {
  if (type != defwBlockageCbkType) {
    std::cout << "Type is not defwBlockageCbkType!" << std::endl;
    exit(2);
  }

  auto phydb_ptr = ((PhyDB *) data);
  auto &blockages = phydb_ptr->design().GetBlockagesRef();
  if (blockages.empty()) return 0;

  defwStartBlockages(static_cast<int>(blockages.size()));
  int status;
  for (auto &blockage : blockages) {
    if (blockage.GetLayer() != nullptr) {
      std::string layer_name = blockage.GetLayer()->GetName();
      status = defwBlockagesLayer(layer_name.c_str());
      CheckStatus(status);
      if (blockage.IsSlots()) {
        status = defwBlockagesLayerSlots();
        CheckStatus(status);
      } else if (blockage.IsFills()) {
        status = defwBlockagesLayerFills();
        CheckStatus(status);
      }
      if (blockage.IsPushdown()) {
        status = defwBlockagesLayerPushdown();
        CheckStatus(status);
      }
      if (blockage.IsExceptpgnet()) {
        status = defwBlockagesLayerExceptpgnet();
        CheckStatus(status);
      }
      if (blockage.GetComponent() != nullptr) {
        std::string comp_name = blockage.GetComponent()->GetName();
        status = defwBlockagesLayerComponent(comp_name.c_str());
        CheckStatus(status);
      }

      if (blockage.GetMaskNum() > 0) {
        status = defwBlockagesLayerMask(blockage.GetMaskNum());
        CheckStatus(status);
      }

      if (blockage.GetSpacing() >= 0) {
        status = defwBlockagesLayerSpacing(blockage.GetSpacing());
        CheckStatus(status);
      } else if (blockage.GetDesignRuleWidth() >= 0) {
        status =
            defwBlockagesLayerDesignRuleWidth(blockage.GetDesignRuleWidth());
        CheckStatus(status);
      }
    } else if (blockage.IsPlacement()) {
      status = defwBlockagesPlacement();
      CheckStatus(status);
      if (blockage.IsSoft()) {
        status = defwBlockagesPlacementSoft();
        CheckStatus(status);
      } else if (blockage.GetMaxPlacementDensity() > 0) {
        status =
            defwBlockagesPlacementPartial(blockage.GetMaxPlacementDensity());
        CheckStatus(status);
      }
      if (blockage.IsPushdown()) {
        status = defwBlockagesPlacementPushdown();
        CheckStatus(status);
      }
      if (blockage.GetComponent() != nullptr) {
        status =
            defwBlockagesPlacementComponent(blockage.GetComponent()->GetName().c_str());
        CheckStatus(status);
      }
    } else {
      PhyDBExpects(false, "blockage has no layer and placement?");
    }

    for (auto &rect : blockage.GetRectsRef()) {
      status =
          defwBlockagesRect(rect.LLX(), rect.LLY(), rect.URX(), rect.URY());
      CheckStatus(status);
    }
    for (auto &polygon : blockage.GetPolygonRef()) {
      int num_points = static_cast<int>(polygon.GetPointsRef().size());
      int *x = (int *) malloc(sizeof(int) * num_points);
      int *y = (int *) malloc(sizeof(int) * num_points);
      for (int i = 0; i < num_points; ++i) {
        Point2D<int> &point = polygon.GetPoint(i);
        x[i] = point.x;
        y[i] = point.y;
      }
      status = defwBlockagesPolygon(num_points, x, y);
      free(x);
      free(y);
      CheckStatus(status);
    }
  }
  status = defwEndBlockages();
  CheckStatus(status);

  return 0;
}

int WriteNets(defwCallbackType_e type, defiUserData data) {
  if (type != defwNetCbkType) {
    std::cout << "Type is not defwNetCbkType!" << std::endl;
    exit(2);
  }

  auto phydb_ptr = ((PhyDB *) data);
  auto &nets = phydb_ptr->GetDesignPtr()->GetNetsRef();
  int status = defwStartNets(static_cast<int>( nets.size()));
  CheckStatus(status);

  auto &components = phydb_ptr->GetDesignPtr()->GetComponentsRef();
  auto &io_pins = phydb_ptr->GetDesignPtr()->GetIoPinsRef();

  auto pin_str = (char *) "PIN";
  for (auto &net : nets) {
    status = defwNet(net.GetName().c_str());
    CheckStatus(status);

    auto iopin_ids = net.GetIoPinIdsRef();
    for (auto &iopin_id : iopin_ids) {
      std::string iopin_name = io_pins[iopin_id].GetName();
      status = defwNetConnection(pin_str, iopin_name.c_str(), 0);
      CheckStatus(status);
    }

    for (auto &pin : net.GetPinsRef()) {
      int comp_id = pin.InstanceId();
      std::string component_name = components[comp_id].GetName();
      std::string macro_name = components[comp_id].GetMacro()->GetName();
      Macro *macro_ptr = phydb_ptr->GetMacroPtr(macro_name);
      PhyDBExpects(macro_ptr != nullptr, "Macro does not exist");
      std::string pin_name = macro_ptr->GetPinsRef()[pin.PinId()].GetName();
      status = defwNetConnection(component_name.c_str(), pin_name.c_str(), 0);
      CheckStatus(status);
    }
    status = defwNetEndOneNet();
    CheckStatus(status);
  }
  status = defwEndNets();
  CheckStatus(status);

  return 0;
}

int WriteSNets(defwCallbackType_e type, defiUserData ud) {
  if (type != defwSNetCbkType) {
    std::cout << "Type is not defwSNetCbkType!" << std::endl;
    exit(2);
  }
  auto &snet_vec = ((PhyDB *) ud)->GetSNetRef();

  if (snet_vec.empty()) {
    return 0;
  } else {
    int status = defwStartSpecialNets(static_cast<int>(snet_vec.size()));
    CheckStatus(status);
  }

  for (auto &snet : snet_vec) {
    defwSpecialNet(snet.GetName().c_str());
    defwSpecialNetConnection("*", snet.GetName().c_str(), 0);
    defwSpecialNetUse(SignalUseStr(snet.GetUse()).c_str());

    auto &polygons = snet.GetPolygonsRef();
    for (auto &polygon : polygons) {
      auto &routing_points = polygon.GetRoutingPointsRef();
      auto *x = new double[routing_points.size()];
      auto *y = new double[routing_points.size()];
      for (size_t j = 0; j < routing_points.size(); j++) {
        x[j] = routing_points[j].x;
        y[j] = routing_points[j].y;
      }
      std::string layer_name = polygon.GetLayerName();

      defwSpecialNetPolygon(
          layer_name.c_str(),
          static_cast<int>(routing_points.size()),
          x, y
      );
      delete[] x;
      delete[] y;
    }

    auto paths = snet.GetPathsRef();
    for (size_t i = 0; i < paths.size(); i++) {
      auto path = paths[i];

      if (i == 0)
        defwSpecialNetPathStart("ROUTED");
      else
        defwSpecialNetPathStart("NEW");

      defwSpecialNetPathLayer(path.GetLayerName().c_str());
      defwSpecialNetPathWidth(path.GetWidth());
      defwSpecialNetPathShape("STRIPE");

      auto routing_points = path.GetRoutingPointsRef();
      double x, y, ext;

      for (auto &point : routing_points) {
        x = point.x;
        y = point.y;
        ext = point.z;
        if (ext == -1) {
          defwSpecialNetPathPoint(1, &x, &y);
        } else {
          defwSpecialNetPathPointWithWireExt(1, &x, &y, &ext);
        }
      }
      if (!path.GetViaName().empty()) {
        defwSpecialNetPathVia(path.GetViaName().c_str());
      }
    }
    defwSpecialNetPathEnd();
    defwSpecialNetEndOneNet();
  }
  defwEndSpecialNets();

  return 0;
}

std::string GetCurrentDateTime() {
  time_t now = time(nullptr);
  struct tm tstruct = *localtime(&now);
  char buf[80];
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

void Si2WriteDef(PhyDB *phy_db_ptr, std::string const &defFileName) {
  FILE *f = fopen(defFileName.c_str(), "w");
  PhyDBExpects(f != nullptr, "Couldn't open Write def file");
  std::cout << "Writing def to " << defFileName << std::endl;

  defwInitCbk(f);

  defwSetVersionCbk(WriteVersion);
  defwSetBusBitCbk(WriteBusBit);
  defwSetDividerCbk(WriteDivider);

  defwSetDesignCbk(WriteDesignName);
  defwSetDesignEndCbk(WriteDesignEnd);

  defwSetUnitsCbk(WriteUnitsDistanceMicrons);
  defwSetDieAreaCbk(WriteDieArea);
  defwSetRowCbk(WriteRows);

  defwSetTrackCbk(WriteTracks);
  defwSetGcellGridCbk(WriteGcellGrids);
  defwSetComponentCbk(WriteComponents);
  defwSetPinCbk(WriteIOPins);
  defwSetBlockageCbk(WriteBlockages);
  defwSetNetCbk(WriteNets);
  defwSetSNetCbk(WriteSNets);

  fprintf(f, "###########################\n");
  fprintf(f, "# Written by PhyDB at ");
  fprintf(f, "%s", GetCurrentDateTime().c_str());
  fprintf(f, "\n");
  fprintf(f, "###########################\n");

  int res = defwWrite(f, defFileName.c_str(), (defiUserData) phy_db_ptr);
  fclose(f);

  PhyDBExpects(res == 0, "DEF Writer returns an error!");
  std::cout << "def writing completes" << std::endl;
}

}

