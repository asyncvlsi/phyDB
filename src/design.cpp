#include "design.h"

namespace phydb {

void Design::SetName(std::string &name) {
    name_ = name;
}

void Design::SetVersion(double version) {
    version_ = version;
}
void Design::SetDividerChar(std::string &divider_char) {
    divider_char_ = divider_char;
}

void Design::SetBusBitChars(std::string &bus_bit_chars) {
    bus_bit_chars_ = bus_bit_chars;
}

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

void Design::SetComponentCount(int count) {
    components_.reserve(count);
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

void Design::SetIoPinCount(int count) {
    iopins_.reserve(count);
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

void Design::SetNetCount(int count) {
    nets_.reserve(count);
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

}
