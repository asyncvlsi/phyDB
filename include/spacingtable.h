#ifndef SPACINGTABLE_H
#define SPACINGTABLE_H

#include "header.h"

namespace phydb {

class SpacingTable {
    private: 
        int _nCol;
        int _nRow;
        vector<float> _parallelRunLength;
        vector<float> _width;
        vector<float> _spacing;

    public: 
        SpacingTable( ): _nCol(0), _nRow(0) { }
        SpacingTable(int nC, int nR): _nCol(nC), _nRow(nR) { 
            _parallelRunLength.resize(nC);
            _width.resize(nR);
            _spacing.resize(nC * nR);
        }
        SpacingTable(int nC, int nR, vector<float> parallelRunLengthVec,
                vector<float> widthVec, vector<float> spacingVec) {
            setSize(nC, nR);
            setParallelRunLengthVec(parallelRunLengthVec);
            setWidthVec(widthVec);
            setSpacingVec(spacingVec);
        }

        void setSize(int nC,int nR);
        void reset( );
        int getNCol( ) const;
        int getNRow( ) const;

        void setParallelRunLengthVec(vector<float> v);
        void setWidthVec(vector<float> v);
        void setSpacingVec(vector<float> v);
        
        vector<float> getParallelRunLengthVec() const;
        vector<float> getWidthVec() const;
        vector<float> getSpacingVec() const;

        void setParallelRunLengthAt(int col, float val);
        void setWidthAt(int row, float val);
        void setSpacingAt(int col, int row, float val); // (0, 0) is at top left
        
        float getParallelRunLengthAt(int col) const;
        float getWidthAt(int row) const;
        float getSpacingAt(int col, int row) const;

};

ostream& operator<< (ostream& , const SpacingTable& );

}


#endif

