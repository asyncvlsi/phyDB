#include "clustercol.h"

namespace phydb {

void ClusterCol::SetName(string& name) {
    name_ = name;
}

void ClusterCol::SetBotSignal(string& bot_signal) {
    bool e = (bot_signal == "GND") || (bot_signal == "VDD");
    PhyDbExpects(e, "ERROR: bot signal in cluster col is not GND or VDD");
    bot_signal_ = bot_signal;
}
void ClusterCol::SetXRange(int lx, int ux) {
    PhyDbExpects(lx < ux, "ERROR: in cluster col lx >= ux");
    lx_ = lx,
    ux_ = ux;
}
void ClusterCol::SetLX(int lx) {
    lx_ = lx;
}
void ClusterCol::SetUX(int ux) {
    ux_ = ux;
}
void ClusterCol::AddRow(int ly, int uy) {
    PhyDbExpects(ly < uy, "ERROR: in cluster col lx >= ux");
    ly_.push_back(ly);
    uy_.push_back(uy);
}

string ClusterCol::GetName() const {
    return name_;
}

string ClusterCol::GetBotSignal() const {
    return bot_signal_;
}
int ClusterCol::GetLX() const {
    return lx_;
}
int ClusterCol::GetUX() const {
    return ux_;
}
vector<int>& ClusterCol::GetLY() {
    return ly_;
}
vector<int>& ClusterCol::GetUY() {
    return uy_;
}


void ClusterCol::Report() {
    cout << "signal: " << bot_signal_ << " " << lx_ << " " << ux_ << endl;
    for(int i = 0; i < ly_.size(); i++) {
        cout << ly_[i] << " " << uy_[i] << endl;
    }

}


}