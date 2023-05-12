//
// Created by christian on 5/9/23.
//

#ifndef ROT_SRC_SERVER_ENDPOINTS_H_
#define ROT_SRC_SERVER_ENDPOINTS_H_

#include "../lib/http.hpp"
#include "util.h"
#include "../lib/json.hpp"
#include "server.h"

namespace endpoints {

using json = nlohmann::json;
using namespace std;

void get_id(const httplib::Request &, httplib::Response &res) {
  res.set_content(to_string(gen_snowflake()), "text/plain");
}

void post_opt(const httplib::Request &req, httplib::Response &res) {
  auto server = Server::get();
  // get the body and turn it into json
  auto j = json::parse(req.body);
  server->clients[to_string(j["id"])] = j; // adds the client if it doesn't already exist
  if (server->queue[j["id"]].empty()) {
    res.set_content(R"({"new_state": 0, "message":"no tasks"})", "application/json");
    return;
  }

  res.set_content(server->queue[j["id"]][0].dump(), "application/json");
  server->queue[j["id"]].pop_front(); // remove the completed task from the queue
}

void get_clients(const httplib::Request &, httplib::Response &res) {
  res.set_header("Access-Control-Allow-Origin", "*");
  auto server = Server::get();
  json out = {};
  out["data"] = {};
  for (auto [id, data] : server->clients) {
    out["data"].push_back(
        {
            {"id", id},
            {"data", data}
        }
    );
  }
  res.set_content(out.dump(), "application/json");
}

void post_control(const httplib::Request &req, httplib::Response &res) {
  res.set_header("Access-Control-Allow-Origin", "*");
  auto server = Server::get();
  json data = json::parse(req.body);
  if (auto search = server->queue.find(data["id"]); search == server->queue.end()) {
    res.set_content(R"({"message":"invalid id"})", "application/json");
    return;
  }

  server->queue[data["id"]].push_back(json::parse(req.body));
  res.set_content(R"({"message":"added to queue"})", "application/json");

}

}

#endif //ROT_SRC_SERVER_ENDPOINTS_H_
