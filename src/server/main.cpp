
#include "../lib/http.h"
#include "../lib/json.hpp"

using namespace std;

using json = nlohmann::json;

auto main() -> int {
  httplib::Server svr;

  svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
    res.set_content("Hello World!", "text/plain");
  });

  svr.Post("/opt", [](auto req, auto res) {
    auto j = json::parse(req.body);

  });

  svr.listen("0.0.0.0", 8080);
}