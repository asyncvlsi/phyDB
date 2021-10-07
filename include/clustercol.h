#ifndef PHYDB_INCLUDE_CLUSTERCOL_H_
#define PHYDB_INCLUDE_CLUSTERCOL_H_

#include "logging.h"

namespace phydb {

class ClusterCol {
  private:
    std::string name_;
    std::string bot_signal_;
    int lx_;
    int ux_;
    std::vector<int> ly_;
    std::vector<int> uy_;

  public:
    ClusterCol() : lx_(0), ux_(0) {}
    ClusterCol(std::string name, std::string bot_signal) :
        name_(name), bot_signal_(bot_signal) {}
    ClusterCol(std::string name, std::string bot_signal, int lx, int ux) :
        name_(name), bot_signal_(bot_signal), lx_(lx), ux_(ux) {}
    ClusterCol(
        std::string name,
        std::string bot_signal,
        int lx,
        int ux,
        std::vector<int> ly,
        std::vector<int> uy
    ) :
        name_(name),
        bot_signal_(bot_signal),
        lx_(lx),
        ux_(ux),
        ly_(ly),
        uy_(uy) {}

    void SetName(std::string &);
    void SetBotSignal(std::string &);
    void SetXRange(int lx, int ux);
    void SetLX(int lx);
    void SetUX(int ux);
    void AddRow(int ly, int uy);

    std::string GetName() const;
    std::string GetBotSignal() const;
    int GetLX() const;
    int GetUX() const;
    std::vector<int> &GetLY();
    std::vector<int> &GetUY();

    void Report();

};

}

#endif //PHYDB_INCLUDE_CLUSTERCOL_H_
