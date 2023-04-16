cmake_minimum_required(VERSION 3.24)
project(rot)

set(CMAKE_CXX_STANDARD 17)

add_executable(rot_server src/server/main.cpp src/lib/http.h src/lib/json.hpp src/state.h src/server/util.h)
add_executable(rot_client src/client/main.cpp src/lib/http.h src/lib/json.hpp src/state.h src/client/functions.h)
