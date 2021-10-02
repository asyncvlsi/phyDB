%{
    #include <cstdlib>

    #include <iostream>

    #include "scanner.h"
    #include "parser.hpp"
    #include "techconfig.h"

    using namespace std;
    #define yyterminate() phydb::Parser::make_END();
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="phydb_"

%%

"Extraction Rules for OpenRCX" {
    return phydb::Parser::make_EXTRACTION(yytext);
}

"DIAGMODEL" {
    return phydb::Parser::make_DIAGMODEL(yytext);
}

"ON" {
    return phydb::Parser::make_ON(yytext);
}

"OFF" {
    return phydb::Parser::make_OFF(yytext);
}

"LayerCount" {
    return phydb::Parser::make_LAYERCOUNT(yytext);
}

"DensityRate" {
    return phydb::Parser::make_DENSITYRATE(yytext);
}

"DensityModel" {
    return phydb::Parser::make_DENSITYMODEL(yytext);
}

"WIDTH Table" {
    return phydb::Parser::make_WIDTH_TABLE(yytext);
}

"entries" {
    return phydb::Parser::make_ENTRIES(yytext);
}

[-+]?([0-9]*\.?[0-9]+|[0-9]+\.)(e[-+]?[0-9]+)? {
    double dnum = atof(yytext);
    return phydb::Parser::make_NUMBER(dnum);
}

"DIST" {
    return phydb::Parser::make_DIST(yytext);
}

"count" {
    return phydb::Parser::make_COUNT(yytext);
}

"width" {
    return phydb::Parser::make_WIDTH(yytext);
}

"Metal" {
    return phydb::Parser::make_METAL(yytext);
}

"RESOVER" {
    return phydb::Parser::make_RESOVER(yytext);
}

"OVER" {
    return phydb::Parser::make_OVER(yytext);
}

"UNDER" {
    return phydb::Parser::make_UNDER(yytext);
}

"DIAGUNDER" {
    return phydb::Parser::make_DIAGUNDER(yytext);
}

"OVERUNDER" {
    return phydb::Parser::make_OVERUNDER(yytext);
}

"END DIST" {
    return phydb::Parser::make_TABLE_END(yytext);
}

"END DensityModel" {
    return phydb::Parser::make_DENSITYMODEL_END(yytext);
}
            
[\n\t: ] {
    /* ignore whitespace */
}

. {
    std::cout << "Scanner: unknown character [" << yytext << "]" << std::endl;
}
            
<<EOF>>     { return yyterminate(); }


%%
