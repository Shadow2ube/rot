//
// Created by christian on 4/15/23.
//

#ifndef ROT_SRC_STATE_H_
#define ROT_SRC_STATE_H_

enum class State {
  remove = -2, // remove the program and all files
  kill = -1,   // stop the program
  idle = 0,    // do nothing
  print = 1,   // print commands
  exec = 2,  // execute commands
};

enum class OS {
  MACOS = 0,
  UNIX = 1,
  WINDOWS = 2,
};

#endif //ROT_SRC_STATE_H_
