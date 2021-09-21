//
// Created by yihangyang on 9/21/21.
//

#ifndef PHYDB_SCANNER_H
#define PHYDB_SCANNER_H

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer phydb_FlexLexer

#include <FlexLexer.h>

#endif

#undef YY_DECL
#define YY_DECL phydb::Parser::symbol_type phydb::Scanner::get_next_token()

#include "parser.hpp"

namespace phydb {

class Interpreter;

class Scanner : public yyFlexLexer {
public:
    Scanner(Interpreter &driver) : m_driver(driver) {}

    virtual ~Scanner() {}

    virtual phydb::Parser::symbol_type get_next_token();

private:
    Interpreter &m_driver;
};

}

#endif //PHYDB_SCANNER_H
