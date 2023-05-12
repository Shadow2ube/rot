//
// Created by christian on 4/23/23.
//

#ifndef ROT_SRC_PRINTERS_H_
#define ROT_SRC_PRINTERS_H_

/**
 * @brief A nice little printer for debug info
 * @param in any cout printable object
 */
template<typename T>
inline void DEBUG(T in) {
  using namespace std;
  cout << "[DEBUG]: " << in << endl;
}

#endif //ROT_SRC_PRINTERS_H_
