%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "techconfig.h"
	#include "parser.hpp"
	#include "location.hh"

	using namespace std;
	#define yyterminate() phydb::Parser::make_END(phydb::location());
	#define YY_USER_ACTION driver_.increaseLocation(yyleng);
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="phydb_"

%%

"Extraction Rules for OpenRCX" {
    return phydb::Parser::make_EXTRACTION(yytext, phydb::location()); 
}

"DIAGMODEL" {
    return phydb::Parser::make_DIAGMODEL(yytext, phydb::location()); 
}

"ON" {
    return phydb::Parser::make_ON(yytext, phydb::location()); 
}

"OFF" {
    return phydb::Parser::make_OFF(yytext, phydb::location()); 
}

"LayerCount" {
    return phydb::Parser::make_LAYERCOUNT(yytext, phydb::location()); 
}

"DensityRate" {
    return phydb::Parser::make_DENSITYRATE(yytext, phydb::location()); 
}

"DensityModel" {
    return phydb::Parser::make_DENSITYMODEL(yytext, phydb::location()); 
}

"WIDTH Table" {
    return phydb::Parser::make_WIDTH_TABLE(yytext, phydb::location()); 
}

"entries" {
    return phydb::Parser::make_ENTRIES(yytext, phydb::location()); 
}

[-+]?([0-9]*\.?[0-9]+|[0-9]+\.)(e[-+]?[0-9]+)?  {
                double dnum = atof(yytext);
                return phydb::Parser::make_NUMBER(dnum, phydb::location()); }

"DIST"      {
    return phydb::Parser::make_DIST(yytext, phydb::location()); 
}

"count"     {
    return phydb::Parser::make_COUNT(yytext, phydb::location()); 
}

"width"     {
    return phydb::Parser::make_WIDTH(yytext, phydb::location()); 
}

"Metal"     {
    return phydb::Parser::make_METAL(yytext, phydb::location()); 
}

"RESOVER"     {
    return phydb::Parser::make_RESOVER(yytext, phydb::location()); 
}

"OVER"     {
    return phydb::Parser::make_OVER(yytext, phydb::location()); 
}

"UNDER"     {
    return phydb::Parser::make_UNDER(yytext, phydb::location()); 
}

"DIAGUNDER"     {
    return phydb::Parser::make_DIAGUNDER(yytext, phydb::location()); 
}

"OVERUNDER"     {
    return phydb::Parser::make_OVERUNDER(yytext, phydb::location()); 
}

"END DIST"   {
    return phydb::Parser::make_TABLE_END(yytext, phydb::location()); 
}

"END DensityModel"  {
    return phydb::Parser::make_DENSITYMODEL_END(yytext, phydb::location()); 
}
            
[\n\t: ]     {
                /* ignore whitespace */
            }

.           { 
                cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }


%%
