//
// Created by christian on 4/23/23.
//

#ifndef ROT_SRC_SERVER_CLIENT_LIST_H_
#define ROT_SRC_SERVER_CLIENT_LIST_H_

#include <mutex>
#include "../lib/json.hpp"

using namespace std;
using json = nlohmann::json;

/**
 * A thread safe list of clients, doesn't have the issue of race conditions
 * through having a mutex
 */
class client_list {
 private:
  mutex lock_;
  unordered_map<string, json> clients_;

 public:
  /**
   * @brief Thread safe unordered_map::operator[](string)
   */
  json &operator[](const string &key) {
    lock_guard l(lock_);
    return this->clients_.operator[](key);
  }

  /**
   * @brief Thread safe unordered_map::operator[](int)
   */
  pair<string, json> operator[](int i) {
    lock_guard l(lock_);
    return *std::next(clients_.begin(), i);
  }

  /**
   * @brief Thread safe unordered_map::begin()
   */
  auto begin() {
    lock_guard l(lock_);
    return clients_.begin();
  }

  /**
   * @brief Thread safe unordered_map::end()
   */
  auto end() {
    lock_guard l(lock_);
    return clients_.end();
  }

  /**
   * @brief Thread safe unordered_map::erase(string)
   */
  auto erase(const string &key) {
    lock_guard l(lock_);
    clients_.erase(key);
  }

  /**
   * @brief Thread safe unordered_map::empty()
   */
  auto empty() {
    lock_guard l(lock_);
    return clients_.empty();
  }

  /**
   * @brief Thread safe unordered_map::length()
   */
  auto length() {
    return clients_.size();
  }

  /**
   * @brief Thread safe unordered_map::size()
   */
  auto size() {
    return length();
  }

};
#endif //ROT_SRC_SERVER_CLIENT_LIST_H_
