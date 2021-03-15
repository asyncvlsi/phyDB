#include "spacingtable.h"

namespace phydb {

void SpacingTable::setSize(int nC, int nR) {
    reset();
    _nCol = nC;
    _nRow = nR;
    _parallelRunLength.resize(nC);
    _width.resize(nR);
    _spacing.resize(nC * nR);
}

void SpacingTable::reset( ) {
    _nCol = 0;
    _nRow = 0;
    _parallelRunLength.clear();
    _width.clear();
    _spacing.clear();
}

int SpacingTable::getNCol( ) const {
    return _nCol;
}

int SpacingTable::getNRow( ) const {
    return _nRow;
}

void SpacingTable::setParallelRunLengthVec(vector<float> v) {
    assert(v.size() == _nCol);
    _parallelRunLength = v;
}

void SpacingTable::setWidthVec(vector<float> v) {
    assert(v.size() == _nRow);
    _width = v;
}

void SpacingTable::setSpacingVec(vector<float> v) {
    assert(v.size() == _nRow * _nCol);
    _spacing = v;
}

vector<float> SpacingTable::getParallelRunLengthVec() const {
    return _parallelRunLength;
}

vector<float> SpacingTable::getWidthVec() const {
    return _width;
}

vector<float> SpacingTable::getSpacingVec() const {
    return _spacing;
}

void SpacingTable::setParallelRunLengthAt(int col, float val) {
    assert(col < _nCol);
    _parallelRunLength[col] = val;
}

void SpacingTable::setWidthAt(int row, float val) {
    assert(row < _nRow);
    _width[row] = val;
}

void SpacingTable::setSpacingAt(int col, int row, float val) {
    assert(col + _nCol * row < _nCol * _nRow);
    _spacing[col + _nCol * row] = val;
}

float SpacingTable::getParallelRunLengthAt(int col) const {
    assert(col < _nCol);
    return _parallelRunLength[col];
}

float SpacingTable::getWidthAt(int row) const {
    assert(row < _nRow);
    return _width[row];
}

float SpacingTable::getSpacingAt(int col, int row) const {
    assert(col + _nCol * row < _nCol * _nRow);
    return _spacing[col + _nCol * row];
}

ostream& operator<< (ostream& os, const SpacingTable& st) {
    int nCol = st.getNCol();
    int nRow = st.getNRow();
    os << "nCol: " << nCol << " nRow: " << nRow << endl;
    
    os << "\t";
    for(auto l : st.getParallelRunLengthVec( ))
        os << l << "\t";
    os << endl;
    for(int r = 0; r < nRow; r++) {
        os << st.getWidthAt(r) << "\t";
        for(int c = 0; c < nCol; c++) {
            os << st.getSpacingAt(c, r) << "\t";
        }
        os << endl;
    }
    return os;
}


}
