#ifndef PHYDB_INCLUDE_SCANNER_H
#define PHYDB_INCLUDE_SCANNER_H

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer phydb_FlexLexer

#include <FlexLexer.h>

#endif

#undef YY_DECL
#define YY_DECL phydb::Parser::symbol_type phydb::Scanner::get_next_token()

#include "parser.hpp"

namespace phydb {

class TechConfigParser;

class Scanner : public yyFlexLexer {
public:
    explicit Scanner(TechConfigParser &driver) : driver_(driver) {}

    virtual ~Scanner() {}

    virtual phydb::Parser::symbol_type get_next_token();

private:
    TechConfigParser &driver_;
};

}

#endif //PHYDB_INCLUDE_SCANNER_H
