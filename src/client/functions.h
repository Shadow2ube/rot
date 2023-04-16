//
// Created by christian on 4/16/23.
//

#ifndef ROT_SRC_CLIENT_FUNCTIONS_H_
#define ROT_SRC_CLIENT_FUNCTIONS_H_

#include <iostream>
#include "../lib/json.hpp"

using namespace std;
using json = nlohmann::json;

/**
 * @brief Prints out a series of strings, separated by new lines
 * @param data json - the data required, following fmt.json
 */
auto print(json data) -> void {
  for (auto const &[_, i] : data.items()) {
    cout << i << endl;
  }
}

auto exec(json data, bool blocking) -> void {
  for (auto const &[_, i] : data.items()) {
    system((i.get<string>() + (blocking ? "" : "&")).data());
  }
}

#endif //ROT_SRC_CLIENT_FUNCTIONS_H_
