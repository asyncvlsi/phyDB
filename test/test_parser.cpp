//
// Created by yihang on 9/22/21.
//

#include <iostream>
#include <fstream>
#include "techconfig.h"

using namespace phydb;
using namespace std;

int main(int argc, char **argv) {
    if (argc < 1) {
        cout << "Please provide a technology configuration file\n";
        return 1;
    }
    TechConfig tech_config;
    Interpreter i(&tech_config);
    std::string input_tech_config_file_name(argv[1]);
    std::ifstream ist(input_tech_config_file_name);
    std::istream &s = ist;
    i.switchInputStream(&s);
    int res = i.parse();
    cout << "Parse complete. Result = " << res << endl;

    tech_config.Report();
    tech_config.CalculateSimplifiedResistanceTable(true);

    return res;
}
