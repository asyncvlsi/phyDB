#include "helper.h"

namespace phydb {

/****
 * @brief Splits a line into word tokens
 *
 * @param line: a string containing many words
 * @param res: a list of word tokens
 */
void StrTokenize(std::string &line, std::vector<std::string> &res) {
    static std::vector<char> delimiter_list{' ', ':', ';', '\t', '\r', '\n'};

    res.clear();
    std::string empty_str;
    bool is_delimiter, old_is_delimiter = true;
    int current_field = -1;
    for (auto &c: line) {
        is_delimiter = false;
        for (auto &delimiter: delimiter_list) {
            if (c == delimiter) {
                is_delimiter = true;
                break;
            }
        }
        if (is_delimiter) {
            old_is_delimiter = is_delimiter;
            continue;
        } else {
            if (old_is_delimiter) {
                current_field++;
                res.push_back(empty_str);
            }
            res[current_field] += c;
            old_is_delimiter = is_delimiter;
        }
    }
}

}