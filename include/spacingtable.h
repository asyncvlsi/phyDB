#ifndef PHYDB_INCLUDE_SPACINGTABLE_H_
#define PHYDB_INCLUDE_SPACINGTABLE_H_

#include "logging.h"

namespace phydb {

class SpacingTable {
  private:
    int n_col_;
    int n_row_;
    std::vector<double> parallel_run_length_;
    std::vector<double> width_;
    std::vector<double> spacing_;

  public:
    SpacingTable() : n_col_(0), n_row_(0) {}
    SpacingTable(int nC, int nR) : n_col_(nC), n_row_(nR) {
        parallel_run_length_.resize(nC);
        width_.resize(nR);
        spacing_.resize(nC * nR);
    }
    SpacingTable(
        int nC,
        int nR,
        const std::vector<double> &v_parallel_run_length,
        const std::vector<double> &v_width,
        const std::vector<double> &v_spacing
    ) {
        SetSize(nC, nR);
        SetParallelRunLengthVec(v_parallel_run_length);
        SetWidthVec(v_width);
        SetSpacingVec(v_spacing);
    }

    void SetSize(int nC, int nR);
    void Reset();
    int GetNCol() const;
    int GetNRow() const;

    void SetParallelRunLengthVec(const std::vector<double> &v);
    void SetWidthVec(const std::vector<double> &v);
    void SetSpacingVec(const std::vector<double> &v);

    std::vector<double> &GetParallelRunLengthVec();
    std::vector<double> &GetWidthVec();
    std::vector<double> &GetSpacingVec();

    void SetParallelRunLengthAt(int col, double val);
    void SetWidthAt(int row, double val);
    void SetSpacingAt(int col, int row, double val); // (0, 0) is at top left

    double GetParallelRunLengthAt(int col) const;
    double GetWidthAt(int row) const;
    double GetSpacingAt(int col, int row) const;

};

std::ostream &operator<<(std::ostream &, const SpacingTable &);

}

#endif //PHYDB_INCLUDE_SPACINGTABLE_H_
