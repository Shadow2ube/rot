
#include "../lib/http.h"
#include "../lib/json.hpp"
#include "util.h"

using namespace std;

using json = nlohmann::json;

auto main() -> int {
  httplib::Server svr;
  unordered_map<string, pair<int, json>> clients;
  unordered_map<string, deque<json>> queue;

  svr.Get("/id", [](auto, auto &res) {
    res.set_content(to_string(gen_snowflake()), "text/plain");
  });

  svr.Post("/opt", [&queue, &clients](auto &req, auto &res) {
    auto j = json::parse(req.body);
    clients[j["id"]] = j["time"];
    if (queue[j["id"]].empty()) {
      res.set_content(R"({"new_state": 0, "message":"no tasks"})", "application/json");
      return;
    }

    res.set_content(queue[j["id"]][0].dump(), "application/json");
    queue[j["id"]].pop_front();
  });

  svr.Post(R"(/control)", [&queue](auto &req, auto &res) {
    json data = json::parse(req.body);
    if (auto search = queue.find(data["id"]); search == queue.end()) {
      res.set_content(R"({"message":"invalid id"})", "application/json");
      return;
    }

    queue[data["id"]].push_back(json::parse(req.body));
    res.set_content(R"({"message":"added to queue"})", "application/json");

  });

  svr.Get("/clients", [&clients](auto, auto &res) {
    json out = {};
    for (auto [id, data] : clients) {
      out["data"].push_back(
          {
              {"id", id},
              {"last_ping", data.first},
              {"data", data.second}
          }
      );
    }
    res.set_content(out.dump(), "application/json");
  });

  svr.set_logger([](auto req, auto res) {
    cout << "path: " << req.path << "\treq: " << req.body << "\tres: " << res.body << endl;
  });

  svr.listen("0.0.0.0", 8080);
}