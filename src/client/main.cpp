//
// Created by christian on 4/13/23.
//

#include <string>
#include "../lib/http.h"
using namespace std;

const string server_addr = "http://localhost:8080";

auto main() -> int {
  httplib::Client cli(server_addr);

//  auto res = cli.Get("/hi");
  auto res = cli.Post("/opt", "hi", "text/plain");
  cout << res->body << endl;
}