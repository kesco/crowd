#include "main.hpp"

#include <iostream>

using namespace std;
using namespace crowd;

namespace bf = boost::filesystem;

int main(int argc, char** argv) {
  crowd::CLIApp app;
  auto option = crowd::parse_cli_option(argc, argv);
  COMMAND cmd = std::get<0>(option);
  if (COMMAND::BUILD == cmd) {
    bf::path path = std::get<1>(option);
    app.build(path);
    return 0;
  } else {
    cerr << "Can not support the command for the moment." << endl;
    return -1;
  }
}