#ifndef DEFVIA_H
#define DEFVIA_H

namespace phydb {

class defVia {
    public:
        string name;
        int idx;
        string viaRuleName;
        Size2D<int> cutSize;
        string layers[3];
        Size2D<int> cutSpacing;
        Size2D<int> botEnc;
        Size2D<int> topEnc;
        int numCutRows;
        int numCutCols;
        Size2D<int> origin;
        Size2D<int> botOffset;
        Size2D<int> topOffset;

        vector<Rect2DLayer<int>> rect2DLayers;
        string pattern;

        defVia( ) { }
        defVia(string& name): name(name) { }

        void Reset() {
            name = "";
            viaRuleName = "";
            cutSize.clear();
            layers[0] = "";
            layers[1] = "";
            layers[2] = "";
            cutSpacing.clear();
            botEnc.clear();
            topEnc.clear();
            numCutRows = 0;
            numCutCols = 0;
            origin.clear();
            botOffset.clear();
            topOffset.clear();
        }

        void Report() {
            cout << "Via: " << name << " VIARule: " << viaRuleName << endl;
            cout << "CUT: " << cutSize.x << " " << cutSize.y << endl;
            cout << "Layer: " << layers[0] << " " << layers[1] << " " << layers[2] << endl;
            cout << "CUTSPACING: " << cutSpacing.x << " " << cutSpacing.y << endl;
            cout << "botEnc: " << botEnc.x << " " << botEnc.y << endl;
            cout << "topEnc: " << topEnc.x << " " << topEnc.y << endl;
            cout << " row: " << numCutRows << " col: " << numCutCols << endl;
            cout << " origin: " << origin.x << " " << origin.y << endl;
            cout << " botOffset: " << botOffset.x << " " << botOffset.y << endl;
            cout << " topOffset: " << topOffset.x << " " << topOffset.y << endl;
        }
};

}

#endif
