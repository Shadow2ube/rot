cmake_minimum_required(VERSION 3.24)
#cmake_minimum_required(VERSION 3.26)
project(rot CXX)

set(CMAKE_CROSSCOMPILING true)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(${PROJECT_NAME}_server src/server/main.cpp src/lib/http.hpp src/lib/json.hpp src/state.h src/server/util.h src/server/client_list.h src/config.h src/server/endpoints.h src/server/endpoints.cpp src/server/server.h src/server/server.cpp)
add_executable(${PROJECT_NAME}_client src/client/main.cpp src/lib/http.hpp src/lib/json.hpp src/state.h src/client/functions.h src/printers.h src/config.h)
add_executable(${PROJECT_NAME}_installer src/installer/main.cpp src/config.h)
