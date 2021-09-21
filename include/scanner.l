%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "interpreter.h"
	#include "parser.hpp"
	#include "location.hh"

	using namespace std;

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
	// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
	#define yyterminate() phydb::Parser::make_END(phydb::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.
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
