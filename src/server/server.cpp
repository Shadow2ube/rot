//
// Created by christian on 5/12/23.
//

#include "server.h"

Server *Server::instance_ = nullptr;

Server *Server::get() {
  if (instance_ == nullptr) {
    instance_ = new Server();
  }
  return instance_;
}
