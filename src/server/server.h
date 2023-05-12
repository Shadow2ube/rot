//
// Created by christian on 5/9/23.
//

#ifndef ROT_SRC_SERVER_SERVER_H_
#define ROT_SRC_SERVER_SERVER_H_

#include "../lib/http.hpp"
#include "client_list.h"

using namespace std;

struct Server {

  httplib::Server svr;
  mutex client_lock;
  client_list clients;
  unordered_map<string, deque<json>> queue;

 protected:
  Server() {}
  static Server *instance_;

 public:
  Server(Server &other) = delete;
  void operator=(const Server &) = delete;

  static Server *get();

};

Server *Server::instance_ = nullptr;

Server *Server::get() {
  if (instance_ == nullptr) {
    instance_ = new Server();
  }
  return instance_;
}

#endif //ROT_SRC_SERVER_SERVER_H_
