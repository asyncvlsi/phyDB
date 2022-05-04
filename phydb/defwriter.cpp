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

int WriteVersion(defwCallbackType_e c, defiUserData ud) {
  int status;
  double version = ((PhyDB *) ud)->GetDefVersion();

  int int_version = version * 10;
  status = defwVersion(int_version / 10, int_version % 10);
  defwNewLine();
  return 0;
}

int WriteBusBit(defwCallbackType_e c, defiUserData ud) {
  int status;
  status = defwBusBitChars(((PhyDB *) ud)->GetDefBusBitChar().c_str());
  defwNewLine();
  return 0;
}

int WriteDivider(defwCallbackType_e c, defiUserData ud) {
  int status;
  status = defwDividerChar(((PhyDB *) ud)->GetDefDividerChar().c_str());
  defwNewLine();
  return 0;
}

int WriteDesignName(defwCallbackType_e c, defiUserData ud) {
  int status;

  status = defwDesignName(((PhyDB *) ud)->GetDefName().c_str());
  defwNewLine();
  return 0;
}

int WriteDesignEnd(defwCallbackType_e c, defiUserData ud) {
  defwNewLine();
  defwEnd();
  return 0;
}

int WriteUnitsDistanceMicrons(defwCallbackType_e c, defiUserData ud) {
  int status;

  status =
      defwUnits(((PhyDB *) ud)->GetDesignPtr()->GetUnitsDistanceMicrons());
  defwNewLine();
  return 0;
}

int WriteDieArea(defwCallbackType_e c, defiUserData ud) {
  int status;
  Rect2D<int> dieArea = ((PhyDB *) ud)->GetDieArea();
  status =
      defwDieArea(dieArea.LLX(), dieArea.LLY(), dieArea.URX(), dieArea.URY());
  defwNewLine();
  return 0;
}

int WriteRows(defwCallbackType_e type, defiUserData data) {
  int status;
  auto rows = ((PhyDB *) data)->GetRowVec();
  for (auto row : rows) {
    status = defwRowStr(
        row.name_.c_str(),
        row.site_name_.c_str(),
        row.orig_x_,
        row.orig_y_,
        row.site_orient_.c_str(),
        row.num_x_,
        row.num_y_,
        row.step_x_,
        row.step_y_
    );
  }

  return 0;
}

int WriteTracks(defwCallbackType_e type, defiUserData data) {
  auto tracks = ((PhyDB *) data)->GetTracksRef();
  for (auto track : tracks) {
    int nlayers = track.GetLayerNames().size();
    const char **layer_names = new const char *[nlayers];
    for (int i = 0; i < nlayers; i++)
      layer_names[i] = track.GetLayerNames()[i].c_str();
    defwTracks(
        XYDirectionStr(track.GetDirection()).c_str(),
        track.GetStart(),
        track.GetNTracks(),
        track.GetStep(),
        nlayers,
        layer_names
    );

  }
  defwNewLine();
  return 0;
}

int WriteGcellGrids(defwCallbackType_e type, defiUserData data) {
  auto gcell_grids = ((PhyDB *) data)->GetGcellGridsRef();
  for (auto gcellgrid : gcell_grids) {
    defwGcellGrid(
        XYDirectionStr(gcellgrid.GetDirection()).c_str(),
        gcellgrid.GetStart(),
        gcellgrid.GetNBoundaries(),
        gcellgrid.GetStep()
    );
  }
  defwNewLine();
  return 0;
}

int WriteComponents(defwCallbackType_e type, defiUserData data) {
  auto components = ((PhyDB *) data)->GetDesignPtr()->GetComponentsRef();
  defwStartComponents(components.size());

  for (auto comp : components) {
    defwComponentStr(
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
  }
  defwEndComponents();
  return 0;
}

int WriteIOPins(defwCallbackType_e type, defiUserData data) {
  auto iopins = ((PhyDB *) data)->GetDesignPtr()->GetIoPinsRef();
  defwStartPins(iopins.size());

  for (auto pin : iopins) {
    defwPinStr(
        pin.GetName().c_str(),
        pin.GetNetName().c_str(),
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
  }
  defwEndPins();

  return 0;
}

int WriteBlockages(defwCallbackType_e type, defiUserData data) {
  auto phydb_ptr = ((PhyDB *) data);
  auto blockages = phydb_ptr->design().GetBlockagesRef();
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
  auto phydb_ptr = ((PhyDB *) data);
  auto nets = phydb_ptr->GetDesignPtr()->GetNetsRef();
  defwStartNets((int) nets.size());
  auto pin_str = (char *) "PIN";
  for (auto net : nets) {
    defwNet(net.GetName().c_str());
    auto iopin_ids = net.GetIoPinIdsRef();
    for (auto &iopin_id : iopin_ids) {
      std::string iopin_name =
          phydb_ptr->GetDesignPtr()->GetIoPinsRef()[iopin_id].GetName();
      defwNetConnection(pin_str, iopin_name.c_str(), 0);
    }

    for (auto &pin : net.GetPinsRef()) {
      std::string component_name =
          phydb_ptr->GetDesignPtr()->components_[pin.InstanceId()].GetName();
      std::string macro_name =
          phydb_ptr->GetDesignPtr()->components_[pin.InstanceId()].GetMacro()->GetName();
      Macro *macro_ptr = phydb_ptr->GetMacroPtr(macro_name);
      PhyDBExpects(macro_ptr != nullptr, "Macro does not exist");
      std::string pin_name =
          macro_ptr->GetPinsRef()[pin.PinId()].GetName();
      defwNetConnection(component_name.c_str(), pin_name.c_str(), 0);
    }
    defwNetEndOneNet();
  }

  defwEndNets();

  return 0;
}

int WriteSNets(defwCallbackType_e c, defiUserData ud) {

  auto snet_vec = ((PhyDB *) ud)->GetSNetRef();

  if (snet_vec.size())
    defwStartSpecialNets(snet_vec.size()); //Number of special nets
  else
    return 0;

  for (int snet_id = 0; snet_id < snet_vec.size(); snet_id++) {
    auto snet = snet_vec[snet_id];
    defwSpecialNet(snet.GetName().c_str());
    defwSpecialNetConnection("*", snet.GetName().c_str(), 0);
    defwSpecialNetUse(SignalUseStr(snet.GetUse()).c_str());

    auto polygons = snet.GetPolygonsRef();
    for (int i = 0; i < polygons.size(); i++) {
      auto routing_points = polygons[i].GetRoutingPointsRef();
      double *x = new double[routing_points.size()];
      double *y = new double[routing_points.size()];

      for (int j = 0; j < routing_points.size(); j++) {
        x[j] = routing_points[j].x;
        y[j] = routing_points[j].y;
      }
      std::string layer_name = polygons[i].GetLayerName();

      defwSpecialNetPolygon(layer_name.c_str(), routing_points.size(), x, y);

      delete[] x;
      delete[] y;
    }

    auto paths = snet.GetPathsRef();
    for (int i = 0; i < paths.size(); i++) {
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

      for (int j = 0; j < routing_points.size(); j++) {
        x = routing_points[j].x;
        y = routing_points[j].y;
        ext = routing_points[j].z;
        if (ext == -1)
          defwSpecialNetPathPoint(1, &x, &y);
        else
          defwSpecialNetPathPointWithWireExt(1, &x, &y, &ext);
      }
      if (path.GetViaName() != "")
        defwSpecialNetPathVia(path.GetViaName().c_str());
    }
    defwSpecialNetPathEnd();
    defwSpecialNetEndOneNet();
  }

  defwEndSpecialNets();

  return 0;
}

std::string GetCurrentDateTime() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

void Si2WriteDef(PhyDB *phy_db_ptr, std::string const &defFileName) {
  FILE *f;
  int res;

  if ((f = fopen(defFileName.c_str(), "w")) == 0) {
    std::cout << "Couldn't open Write def file" << std::endl;
    exit(2);
  }
  std::cout << "Writing def to " << defFileName << std::endl;
  int status = defwInitCbk(f);

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

  res = defwWrite(f, defFileName.c_str(), (defiUserData) phy_db_ptr);
  if (res != 0) {
    std::cout << "DEF Writer returns an error!" << std::endl;
    exit(2);
  }
  fclose(f);
  std::cout << "def writing completes" << std::endl;
}

}

