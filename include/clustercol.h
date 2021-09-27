#ifndef CLUSTERCOL_H
#define CLUSTERCOL_H
#include "logging.h"

namespace phydb {

class ClusterCol {
  private:
    string name_;
    string bot_signal_;
    int lx_;
    int ux_;
    vector<int> ly_;
    vector<int> uy_;

  public:
    ClusterCol() : lx_(0), ux_(0) {}
    ClusterCol(string name, string bot_signal) :
        name_(name), bot_signal_(bot_signal) {}
    ClusterCol(string name, string bot_signal, int lx, int ux) :
        name_(name), bot_signal_(bot_signal), lx_(lx), ux_(ux) {}
    ClusterCol(string name, string bot_signal, int lx, int ux, vector<int> ly, vector<int> uy) :
        name_(name), bot_signal_(bot_signal), lx_(lx), ux_(ux), ly_(ly), uy_(uy){}

    void SetName(string& );
    void SetBotSignal(string& );
    void SetXRange(int lx, int ux);
    void SetLX(int lx);
    void SetUX(int ux);
    void AddRow(int ly, int uy);

    string GetName() const;
    string GetBotSignal() const;
    int GetLX() const;
    int GetUX() const;
    vector<int>& GetLY();
    vector<int>& GetUY();

    void Report();

};

}

#endif