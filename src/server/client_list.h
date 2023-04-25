//
// Created by christian on 4/23/23.
//

#ifndef ROT_SRC_SERVER_CLIENT_LIST_H_
#define ROT_SRC_SERVER_CLIENT_LIST_H_

#include <mutex>
#include "../lib/json.hpp"

using namespace std;
using json = nlohmann::json;

class client_list {
 private:
  mutex lock_;
  unordered_map<string, json> clients_;

 public:
  json &operator[](const string &key) {
    lock_guard l(lock_);
    return this->clients_.operator[](key);
  }

//  unordered_map<string, json>::iterator operator [](int i) {
  pair<string, json> operator[](int i) {
    lock_guard l(lock_);
//    auto out = clients_[i];
//    return this->clients_[i];
//    auto it = clients_.begin();
//    std::advance(it, i);
//    return it;
    return *std::next(clients_.begin(), i);
  }

  auto begin() {
    lock_guard l(lock_);
    return clients_.begin();
  }

  auto end() {
    lock_guard l(lock_);
    return clients_.end();
  }

  auto erase(const string &key) {
    lock_guard l(lock_);
    clients_.erase(key);
  }

  auto empty() {
    lock_guard l(lock_);
    return clients_.empty();
  }

  auto length() {
    return clients_.size();
  }

  auto size() {
    return length();
  }

};
#endif //ROT_SRC_SERVER_CLIENT_LIST_H_
