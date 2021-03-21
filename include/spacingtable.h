#ifndef SPACINGTABLE_H
#define SPACINGTABLE_H

#include "header.h"

namespace phydb {

class SpacingTable {
  private:
    int n_col_;
    int n_row_;
    vector<float> parallel_run_length_;
    vector<float> width_;
    vector<float> spacing_;

  public:
    SpacingTable() : n_col_(0), n_row_(0) {}
    SpacingTable(int nC, int nR) : n_col_(nC), n_row_(nR) {
        parallel_run_length_.resize(nC);
        width_.resize(nR);
        spacing_.resize(nC * nR);
    }
    SpacingTable(int nC, int nR, vector<float> parallelRunLengthVec,
                 vector<float> widthVec, vector<float> spacingVec) {
        SetSize(nC, nR);
        SetParallelRunLengthVec(parallelRunLengthVec);
        SetWidthVec(widthVec);
        SetSpacingVec(spacingVec);
    }

    void SetSize(int nC, int nR);
    void Reset();
    int GetNCol() const;
    int GetNRow() const;

    void SetParallelRunLengthVec(vector<float> v);
    void SetWidthVec(vector<float> v);
    void SetSpacingVec(vector<float> v);

    vector<float> GetParallelRunLengthVec() const;
    vector<float> GetWidthVec() const;
    vector<float> GetSpacingVec() const;

    void SetParallelRunLengthAt(int col, float val);
    void SetWidthAt(int row, float val);
    void SetSpacingAt(int col, int row, float val); // (0, 0) is at top left

    float GetParallelRunLengthAt(int col) const;
    float GetWidthAt(int row) const;
    float GetSpacingAt(int col, int row) const;

};

ostream &operator<<(ostream &, const SpacingTable &);

}

#endif

