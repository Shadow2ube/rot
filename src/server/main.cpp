
#include "../lib/http.hpp"
#include "../lib/json.hpp"
#include "util.h"
#include "client_list.h"
#include "../printers.h"
#include "endpoints.h"
#include "server.h"

// how long should we wait for a client to ping us
#define timeout 20 // seconds

using namespace std;

using json = nlohmann::json;

string get_local_ip() {
  char host[256];
  int hostname = gethostname(host, sizeof(host));
  if (hostname == -1) std::cout << "Error: Get Host Name" << std::endl;

  struct hostent *host_entry;
  host_entry = gethostbyname(host);

  if (host_entry == nullptr) std::cout << "Error: Get Host Entry" << std::endl;

  return inet_ntoa(*((struct in_addr *) host_entry->h_addr_list[0]));

}

/**
 * INFINITE -- A function that checks for clients who reached the timeout for the last ping & removes them
 */
void client_killer() {
  auto server = Server::get();
  while (true) {
    if (server->clients.empty()) continue; // do nothing if empty
    for (int i = 0; i < server->clients.size(); ++i) { // loop through the clients
      auto [key, val] = server->clients[i]; // get the key (id) and val (last ping)
      if ((time(nullptr) - ((int) val["time"])) >= timeout) {
        cout << "removed client (" << key << ") due to inactivity" << endl;
        server->clients.erase(key); // remove the client
        if (server->clients.size() == 0) break; // fixes weird edge case
      }
    }
    this_thread::sleep_for(1s); // this doesn't need to be very fast
  }
}

auto main(int argc, char **argv) -> int {
  auto *server = Server::get();

  // loop through the clients
  // if there is one that hasn't responded in a while, remove it
  thread ck(client_killer);

  server->svr.Get("/id", endpoints::get_id);             // IDs
  server->svr.Post("/opt", endpoints::post_opt);         // Clients asking for new info
  server->svr.Post("/control", endpoints::post_control); // control from the front end
  server->svr.Get("/clients", endpoints::get_clients);   // list all clients

  // add a spot for a client to download a new client
  server->svr.set_mount_point("/content", "/home/christian/Documents/school/gr12/s2p1-compSci/rot/content");

  // prettifies logging info
  server->svr.set_logger([](auto req, auto res) {
    cout << "Method: " << req.method
         << "\tpath: " << req.path
         << "\treq: " << req.body
         << "\tres: " << res.body
         << endl;
  });

  // default error handling
  server->svr.set_error_handler([](auto &req, auto &res) {
    res.set_content(R"({"message":"there was an error","new_state": 0})", "application/json");
  });

  // get the ip
  auto ip = get_local_ip();
  if (argc != 1) ip = argv[1];
  cout << "Server started on " << ip << endl;
  server->svr.listen(ip, 8080); // run the server
  return -1;
}