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

/**
 * @brief Runs a series of commands in the clients default command processor
 * @param data json - the data to be run, following fmt.json
 * @param blocking bool - whether to run the commands one after another
 */
auto exec(json data, bool blocking) -> void {
  for (auto const &[_, i] : data.items()) {
    system((i.get<string>() + (blocking ? "" : "&")).data());
  }
}

#endif //ROT_SRC_CLIENT_FUNCTIONS_H_
