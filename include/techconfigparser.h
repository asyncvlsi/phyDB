#ifndef PHYDB_INCLUDE_TECHCONFIGPARSER_H_
#define PHYDB_INCLUDE_TECHCONFIGPARSER_H_

#include "tech.h"

namespace phydb {

class TechConfigParser {
    friend class Parser;
    friend class Scanner;
  public:
    explicit TechConfigParser(Tech *tech);
    int Parse();
    void SetInputStream(std::istream *is);
    Tech *UserData();

  private:
    Scanner scanner_;
    Parser parser_;
    Tech *tech_ = nullptr;

    /**** temporary pointers for parsing technology configuration files ****/
    ResOverTable *tmp_res_over_ = nullptr;
    CapOverTable *tmp_cap_over_ = nullptr;
    CapUnderTable *tmp_cap_under_ = nullptr;
    CapDiagUnderTable *tmp_cap_diagunder_ = nullptr;
    CapOverUnderTable *tmp_cap_overunder_ = nullptr;
};

}

#endif //PHYDB_INCLUDE_TECHCONFIGPARSER_H_
