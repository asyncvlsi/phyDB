%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.3"
%defines
%define api.parser.class { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { phydb }
%code requires
{
    #include <stdint.h>
    #include <string.h>

    #include <iostream>
    #include <string>
    #include <vector>

    namespace phydb {
        class Scanner;
        class TechConfigParser;
    }
}

%code top
{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hpp"
    #include "techconfigparser.h"

    static phydb::Parser::symbol_type yylex(phydb::Scanner &scanner) {
        return scanner.get_next_token();
    }
    
    using namespace phydb;
}

%lex-param { phydb::Scanner &scanner }
%parse-param { phydb::Scanner &scanner }
%parse-param { phydb::TechConfigParser &tech_config_parser }
%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOKEN_}

%token <std::string> COUNT
%token <std::string> DENSITYMODEL
%token <std::string> DENSITYMODEL_END
%token <std::string> DENSITYRATE
%token <std::string> DIAGMODEL
%token <std::string> DIAGUNDER
%token <std::string> DIST
%token END 0
%token <std::string> ENTRIES
%token <std::string> EXTRACTION
%token <std::string> LAYERCOUNT
%token <std::string> METAL
%token <double> NUMBER
%token <std::string> OFF
%token <std::string> ON
%token <std::string> OVER
%token <std::string> OVERUNDER
%token <std::string> RESOVER
%token <std::string> TABLE_END
%token <std::string> UNDER
%token <std::string> WIDTH
%token <std::string> WIDTH_TABLE

%type<std::string> densitymodel densityrate diagmodel extraction info layercount
%type<std::string> relative_pos

%start tech_config_file

%%

tech_config_file : header model

header: info | header info

info: extraction | diagmodel | layercount | densityrate

extraction: EXTRACTION

diagmodel: DIAGMODEL ON
    {
        auto tech = tech_config_parser.UserData();
        tech->GetTechConfigRef().SetDiagModelOn(true);
    }
  | DIAGMODEL OFF
    {
        auto tech = tech_config_parser.UserData();
        tech->GetTechConfigRef().SetDiagModelOn(false);
    }

layercount: LAYERCOUNT NUMBER
    {
        auto tech = tech_config_parser.UserData();
        tech->GetTechConfigRef().SetLayerCount(int($2));
    }

densityrate: DENSITYRATE NUMBER
    {
        auto tech = tech_config_parser.UserData();
        tech->GetTechConfigRef().SetModelCount(int($2));
    }
  | densityrate NUMBER
    {
        auto tech = tech_config_parser.UserData();
        tech->GetTechConfigRef().AddDataRate($2);
    }

model: densitymodel metal_tables end_densitymodel

densitymodel: DENSITYMODEL NUMBER
    {
        auto tech = tech_config_parser.UserData();
        tech->AddTechConfigCorner(int($2));
    }

metal_tables: metal_header table | metal_tables table | metal_tables metal_header

metal_header: METAL NUMBER relative_pos WIDTH_TABLE NUMBER ENTRIES
  | METAL NUMBER relative_pos WIDTH_TABLE NUMBER ENTRIES NUMBER

relative_pos: RESOVER | OVER | UNDER | DIAGUNDER | OVERUNDER

table: table_header table_start table_body table_end
    {
        auto tech = tech_config_parser.UserData();
        if (tech_config_parser.tmp_res_over_ != nullptr) {
            tech->AddResOverTable(tech_config_parser.tmp_res_over_);
            delete tech_config_parser.tmp_res_over_;
            tech_config_parser.tmp_res_over_ = nullptr;
        } else if (tech_config_parser.tmp_cap_over_ != nullptr) {
            tech->AddCapOverTable(tech_config_parser.tmp_cap_over_);
            delete tech_config_parser.tmp_cap_over_;
            tech_config_parser.tmp_cap_over_ = nullptr;
        } else if (tech_config_parser.tmp_cap_under_ != nullptr) {
            tech->AddCapUnderTable(tech_config_parser.tmp_cap_under_);
            delete tech_config_parser.tmp_cap_under_;
            tech_config_parser.tmp_cap_under_ = nullptr;
        } else if (tech_config_parser.tmp_cap_diagunder_ != nullptr) {
            tech->AddCapDiagUnderTable(tech_config_parser.tmp_cap_diagunder_);
            delete tech_config_parser.tmp_cap_diagunder_;
            tech_config_parser.tmp_cap_diagunder_ = nullptr;
        } else if (tech_config_parser.tmp_cap_overunder_ != nullptr) {
            tech->AddCapOverUnderTable(tech_config_parser.tmp_cap_overunder_);
            delete tech_config_parser.tmp_cap_overunder_;
            tech_config_parser.tmp_cap_overunder_ = nullptr;
        } else {
            std::cout << "Error: this is impossible to happen" << std::endl;
            exit(1);
        }
    }

table_header: simple_table_header | simple_table_header under_layer

simple_table_header: METAL NUMBER relative_pos NUMBER
    {
        int layer_index_1 = (int) $2 - 1;
        int layer_index_2 = (int) $4 - 1;
        if ($3 == "RESOVER") {
            tech_config_parser.tmp_res_over_ = new ResOverTable(layer_index_1, layer_index_2);
        } else if ($3 == "OVER") {
            tech_config_parser.tmp_cap_over_ = new CapOverTable(layer_index_1, layer_index_2);
        } else if ($3 == "UNDER") {
            tech_config_parser.tmp_cap_under_ = new CapUnderTable(layer_index_1, layer_index_2);
        } else if ($3 == "DIAGUNDER") {
            tech_config_parser.tmp_cap_diagunder_ = new CapDiagUnderTable(layer_index_1, layer_index_2);
        } else {
            std::cout << "Error: impossible to happen" << std::endl;
            exit(1);
        }
    }

under_layer: UNDER NUMBER
    {
        int layer_index = tech_config_parser.tmp_cap_over_->LayerIndex();
        int over_index = tech_config_parser.tmp_cap_over_->OverIndex();
        delete tech_config_parser.tmp_cap_over_;
        tech_config_parser.tmp_cap_over_ = nullptr;

        int under_index = (int) $2 - 1;
        tech_config_parser.tmp_cap_overunder_ = new CapOverUnderTable(
            layer_index,
            over_index,
            under_index
        );
    }

table_start: DIST COUNT NUMBER WIDTH NUMBER
    {
        int sz = (int) $3;
        double width = $5;
        if (tech_config_parser.tmp_res_over_ != nullptr) {
            tech_config_parser.tmp_res_over_->GetTable().reserve(sz);
            tech_config_parser.tmp_res_over_->SetWidth(width);
        }
        if (tech_config_parser.tmp_cap_over_ != nullptr) {
            tech_config_parser.tmp_cap_over_->GetTable().reserve(sz);
            tech_config_parser.tmp_cap_over_->SetWidth(width);
        }
        if (tech_config_parser.tmp_cap_under_ != nullptr) {
            tech_config_parser.tmp_cap_under_->GetTable().reserve(sz);
            tech_config_parser.tmp_cap_under_->SetWidth(width);
        }
        if (tech_config_parser.tmp_cap_diagunder_ != nullptr) {
            tech_config_parser.tmp_cap_diagunder_->GetTable().reserve(sz);
            tech_config_parser.tmp_cap_diagunder_->SetWidth(width);
        }
        if (tech_config_parser.tmp_cap_overunder_ != nullptr) {
            tech_config_parser.tmp_cap_overunder_->GetTable().reserve(sz);
            tech_config_parser.tmp_cap_overunder_->SetWidth(width);
        }
    }

table_body: /* nothing */ | table_body table_entry

table_entry: NUMBER NUMBER NUMBER NUMBER
    {
        double distance = $1;
        double coupling_cap = $2;
        double fringe_cap = $3;
        double res = $4;
        if (tech_config_parser.tmp_res_over_ != nullptr) {
            tech_config_parser.tmp_res_over_->AddEntry(distance, coupling_cap, fringe_cap, res);
        } else if (tech_config_parser.tmp_cap_over_ != nullptr) {
            tech_config_parser.tmp_cap_over_->AddEntry(distance, coupling_cap, fringe_cap, res);
        } else if (tech_config_parser.tmp_cap_under_ != nullptr) {
            tech_config_parser.tmp_cap_under_->AddEntry(distance, coupling_cap, fringe_cap, res);
        } else if (tech_config_parser.tmp_cap_diagunder_ != nullptr) {
            tech_config_parser.tmp_cap_diagunder_->AddEntry(distance, coupling_cap, fringe_cap, res);
        } else if (tech_config_parser.tmp_cap_overunder_ != nullptr) {
            tech_config_parser.tmp_cap_overunder_->AddEntry(distance, coupling_cap, fringe_cap, res);
        } else {
            std::cout << "Error: no table is available to accept this entry?" << std::endl;
            exit(1);
        }
    }

table_end: TABLE_END

end_densitymodel: DENSITYMODEL_END NUMBER

%%

void phydb::Parser::error(const std::string &message) {
    std::cout << "Error: " << message << std::endl;
}
