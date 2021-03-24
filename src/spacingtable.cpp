#include "spacingtable.h"

namespace phydb {

void SpacingTable::SetSize(int nC, int nR) {
    Reset();
    n_col_ = nC;
    n_row_ = nR;
    parallel_run_length_.resize(nC);
    width_.resize(nR);
    spacing_.resize(nC * nR);
}

void SpacingTable::Reset( ) {
    n_col_ = 0;
    n_row_ = 0;
    parallel_run_length_.clear();
    width_.clear();
    spacing_.clear();
}

int SpacingTable::GetNCol( ) const {
    return n_col_;
}

int SpacingTable::GetNRow( ) const {
    return n_row_;
}

void SpacingTable::SetParallelRunLengthVec(const vector<double>& v) {
    assert(v.size() == n_col_);
    parallel_run_length_ = v;
}

void SpacingTable::SetWidthVec(const vector<double>& v) {
    assert(v.size() == n_row_);
    width_ = v;
}

void SpacingTable::SetSpacingVec(const vector<double>& v) {
    assert(v.size() == n_row_ * n_col_);
    spacing_ = v;
}

vector<double>& SpacingTable::GetParallelRunLengthVec() {
    return parallel_run_length_;
}

vector<double>& SpacingTable::GetWidthVec() {
    return width_;
}

vector<double>& SpacingTable::GetSpacingVec() {
    return spacing_;
}

void SpacingTable::SetParallelRunLengthAt(int col, double val) {
    assert(col < n_col_);
    parallel_run_length_[col] = val;
}

void SpacingTable::SetWidthAt(int row, double val) {
    assert(row < n_row_);
    width_[row] = val;
}

void SpacingTable::SetSpacingAt(int col, int row, double val) {
    assert(col + n_col_ * row < n_col_ * n_row_);
    spacing_[col + n_col_ * row] = val;
}

double SpacingTable::GetParallelRunLengthAt(int col) const {
    assert(col < n_col_);
    return parallel_run_length_[col];
}

double SpacingTable::GetWidthAt(int row) const {
    assert(row < n_row_);
    return width_[row];
}

double SpacingTable::GetSpacingAt(int col, int row) const {
    assert(col + n_col_ * row < n_col_ * n_row_);
    return spacing_[col + n_col_ * row];
}

ostream& operator<< (ostream& os, const SpacingTable& st) {
    int nCol = st.GetNCol();
    int nRow = st.GetNRow();
    os << "nCol: " << nCol << " nRow: " << nRow << endl;
    
    os << "\t";
    for(int c = 0; c < nCol; c++)
        os << st.GetParallelRunLengthAt(c) << "\t";
    os << endl;
    for(int r = 0; r < nRow; r++) {
        os << st.GetWidthAt(r) << "\t";
        for(int c = 0; c < nCol; c++) {
            os << st.GetSpacingAt(c, r) << "\t";
        }
        os << endl;
    }
    return os;
}


}
