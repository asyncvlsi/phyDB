%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
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

    using namespace std;

    namespace phydb {
        class Scanner;
        class Interpreter;
    }
}

%code top
{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hpp"
    #include "techconfig.h"

    static phydb::Parser::symbol_type yylex(phydb::Scanner &scanner) {
        return scanner.get_next_token();
    }
    
    using namespace phydb;
}

%lex-param { phydb::Scanner &scanner }
%parse-param { phydb::Scanner &scanner }
%parse-param { phydb::Interpreter &driver }
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
        driver.UserData()->SetDiagModelOn(true);
    }
  | DIAGMODEL OFF
    {
        driver.UserData()->SetDiagModelOn(false);
    }

layercount: LAYERCOUNT NUMBER
    {
        driver.UserData()->SetLayerCount(int($2));
    }

densityrate: DENSITYRATE NUMBER
    {
        driver.UserData()->SetModelCount(int($2));
    }
  | densityrate NUMBER
    {
        driver.UserData()->AddDataRate($2);
    }

model: densitymodel metal_tables end_densitymodel

densitymodel: DENSITYMODEL NUMBER
    {
        driver.UserData()->AddModel(int($2));
    }

metal_tables: metal_header table | metal_tables table | metal_tables metal_header


metal_header: METAL NUMBER relative_pos WIDTH_TABLE NUMBER ENTRIES
  | METAL NUMBER relative_pos WIDTH_TABLE NUMBER ENTRIES NUMBER

relative_pos: RESOVER | OVER | UNDER | DIAGUNDER | OVERUNDER

table: table_header table_start table_body table_end
    {
        auto model = driver.UserData()->GetLastModel();
        if (model != nullptr) {
            model->MarkNothing();
        }
    }

table_header: simple_table_header | simple_table_header under_layer

simple_table_header: METAL NUMBER relative_pos NUMBER
    {
        auto model = driver.UserData()->GetLastModel();
        if (model != nullptr) {
            int layer_index_1 = (int) $2;
            int layer_index_2 = (int) $4;
            if ($3 == "RESOVER") {
                model->res_over_.emplace_back(layer_index_1, layer_index_2);
                model->MarkResOver();
            } else if ($3 == "OVER") {
                model->cap_over_.emplace_back(layer_index_1, layer_index_2);
                model->MarkCapOver();
            } else if ($3 == "UNDER") {
                model->cap_under_.emplace_back(layer_index_1, layer_index_2);
                model->MarkCapUnder();
            } else if ($3 == "DIAGUNDER") {
                model->cap_diagunder_.emplace_back(layer_index_1, layer_index_2);
                model->MarkCapDiagUnder();
            } else {
                cout << "Error: impossible to happen\n";
                exit(1);
            }
        }
    }

under_layer: UNDER NUMBER
    {
        auto model = driver.UserData()->GetLastModel();
        if (model != nullptr) {
            int layer_index = model->cap_over_.back().LayerIndex();
            int over_index = model->cap_over_.back().OverIndex();
            model->cap_over_.pop_back();

            int under_index = (int) $2;
            model->cap_overunder_.emplace_back(
                layer_index,
                over_index,
                under_index
            );
            model->MarkCapOverUnder();
        }
    }

table_start: DIST COUNT NUMBER WIDTH NUMBER
    {
        auto model = driver.UserData()->GetLastModel();
        int sz = (int) $3;
        double width = $5;
        if (model != nullptr) {
            if (model->tmp_res_over_ != nullptr) {
                model->tmp_res_over_->GetTable().reserve(sz);
                model->tmp_res_over_->SetWidth(width);
            }
            if (model->tmp_cap_over_ != nullptr) {
                model->tmp_cap_over_->GetTable().reserve(sz);
                model->tmp_cap_over_->SetWidth(width);
            }
            if (model->tmp_cap_under_ != nullptr) {
                model->tmp_cap_under_->GetTable().reserve(sz);
                model->tmp_cap_under_->SetWidth(width);
            }
            if (model->tmp_cap_diagunder_ != nullptr) {
                model->tmp_cap_diagunder_->GetTable().reserve(sz);
                model->tmp_cap_diagunder_->SetWidth(width);
            }
            if (model->tmp_cap_overunder_ != nullptr) {
                model->tmp_cap_overunder_->GetTable().reserve(sz);
                model->tmp_cap_overunder_->SetWidth(width);
            }
        }
    }

table_body: /* nothing */ | table_body table_entry

table_entry: NUMBER NUMBER NUMBER NUMBER
    {
        auto model = driver.UserData()->GetLastModel();
        double distance = $1;
        double coupling_cap = $2;
        double fringe_cap = $3;
        double res = $4;
        if (model != nullptr) {
            if (model->tmp_res_over_ != nullptr) {
                model->tmp_res_over_->AddEntry(distance, coupling_cap, fringe_cap, res);
            } else if (model->tmp_cap_over_ != nullptr) {
                model->tmp_cap_over_->AddEntry(distance, coupling_cap, fringe_cap, res);
            } else if (model->tmp_cap_under_ != nullptr) {
                model->tmp_cap_under_->AddEntry(distance, coupling_cap, fringe_cap, res);
            } else if (model->tmp_cap_diagunder_ != nullptr) {
                model->tmp_cap_diagunder_->AddEntry(distance, coupling_cap, fringe_cap, res);
            } else if (model->tmp_cap_overunder_ != nullptr) {
                model->tmp_cap_overunder_->AddEntry(distance, coupling_cap, fringe_cap, res);
            } else {
                cout << "Error: no table is available to accept this entry?" << endl;
                exit(1);
            }
        }
    }

table_end: TABLE_END

end_densitymodel: DENSITYMODEL_END NUMBER

%%

void phydb::Parser::error(const string &message) {
    cout << "Error: " << message << endl;
}
