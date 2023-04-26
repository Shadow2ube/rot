
#include "../lib/http.h"
#include "../lib/json.hpp"
#include "util.h"
#include "client_list.h"
#include "../printers.h"

// how long should we wait for a client to ping us
#define timeout 20 // seconds

using namespace std;

using json = nlohmann::json;

char *get_local_ip() {
  char host[256];
  int hostname = gethostname(host, sizeof(host));

  if (hostname == -1) std::cout << "Error: Get Host Name" << std::endl;

  struct hostent *host_entry;
  host_entry = gethostbyname(host);

  if (host_entry == nullptr) std::cout << "Error: Get Host Entry" << std::endl;

  char *IP;
  IP = inet_ntoa(*((struct in_addr *) host_entry->h_addr_list[0]));

  return IP;

}

auto main() -> int {
  httplib::Server svr;
  mutex client_lock;
  client_list clients;
  unordered_map<string, deque<json>> queue;

  // loop through the clients
  // if there is one that hasn't responded in a while, remove it
  thread client_killer([&clients]() {
    while (true) {
      if (clients.empty()) continue; // do nothing if empty
      for (int i = 0; i < clients.size(); ++i) { // loop through the clients
        auto [key, val] = clients[i]; // get the key (id) and val (last ping)
        if ((time(nullptr) - ((int) val["time"])) >= timeout) {
          cout << "removed client (" << key << ") due to inactivity" << endl;
          clients.erase(key); // remove the client
          if (clients.size() == 0) break; // fixes weird edge case
        }
      }
      this_thread::sleep_for(1s); // this doesn't need to be very fast
    }
  });

  // gives a new ID to a client
  svr.Get("/id", [](auto, auto &res) {
    res.set_content(to_string(gen_snowflake()), "text/plain");
  });

  // gives a client the next task in the queue
  svr.Post("/opt", [&queue, &clients](auto &req, auto &res) {
    // get the body and turn it into json
    auto j = json::parse(req.body);
    clients[to_string(j["id"])] = j; // adds the client if it doesn't already exist
    if (queue[j["id"]].empty()) {
      res.set_content(R"({"new_state": 0, "message":"no tasks"})", "application/json");
      return;
    }

    res.set_content(queue[j["id"]][0].dump(), "application/json");
    queue[j["id"]].pop_front(); // remove the completed task from the queue
  });

  // for the frontend, gets a task and the clients ID, then adds it to the queue.
  svr.Post(R"(/control)", [&queue](auto &req, auto &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    json data = json::parse(req.body);
    if (auto search = queue.find(data["id"]); search == queue.end()) {
      res.set_content(R"({"message":"invalid id"})", "application/json");
      return;
    }

    queue[data["id"]].push_back(json::parse(req.body));
    res.set_content(R"({"message":"added to queue"})", "application/json");

  });

  // returns a list of the clients
  svr.Get("/clients", [&clients](auto, auto &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    json out = {};
    out["data"] = {};
    for (auto [id, data] : clients) {
      out["data"].push_back(
          {
              {"id", id},
              {"data", data}
          }
      );
    }
    res.set_content(out.dump(), "application/json");
  });

  // add a spot for a client to download a new client
  svr.set_mount_point("/content", "/home/christian/Documents/school/gr12/s2p1-compSci/rot/content");

  // prettifies logging info
  svr.set_logger([](auto req, auto res) {
    cout << "Method: " << req.method
         << "\tpath: " << req.path
         << "\treq: " << req.body
         << "\tres: " << res.body
         << endl;
  });

  // default error handling
  svr.set_error_handler([](auto &req, auto &res) {
    res.set_content(R"({"message":"there was an error","new_state": 0})", "application/json");
  });

  // get the ip
  auto ip = get_local_ip();
  cout << "LOCAL IP ADDRESS: " << ip << endl;
  cout << "Server started" << endl;
  svr.listen(ip, 8080); // run the server
}