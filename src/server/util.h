//
// Created by christian on 4/15/23.
//

#ifndef ROT_SRC_SERVER_UTIL_H_
#define ROT_SRC_SERVER_UTIL_H_

#include <chrono>

uint64_t seq;
/**
 * Generates a snowflake id similar to that of twitter and discord,
 * the whole id is unique enough for the scope of this program
 * @param mid the machine id
 * @return the snowflake as an int
 */
uint64_t gen_snowflake(uint64_t mid = 1234) {
  using namespace std::chrono;
  uint64_t t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  uint64_t out = (t << 23) | ((mid << 52) >> 40) | ((seq << 52) >> 52);

  seq++;
  return out;
}

#endif //ROT_SRC_SERVER_UTIL_H_
