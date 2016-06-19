#include "cli.hpp"
#include "biz.hpp"

#include <boost/regex.hpp>

#include <iostream>

using namespace std;
namespace bf = boost::filesystem;

namespace {

}

namespace crowd {
  void CLIApp::build(boost::filesystem::path &path) {
    if (bf::exists(path) && bf::is_directory(path)) {
      bf::path post_path = path;
      post_path.append("posts");
      if (bf::exists(post_path) && bf::is_directory(post_path)) {
        boost::regex pattern(".*\\.md");
        vector<Post> posts;
        for (bf::recursive_directory_iterator iter(post_path), end;
             iter != end; iter++) {
          string file_name = iter->path().string();
          if (boost::regex_match(file_name, pattern)) {
            Post post(iter->path());
            if (post.load()) {
              posts.push_back(move(post));
              cout << file_name << endl;
            }
          }
        }

        cout << "Load " << posts.size() << " posts." << endl;
      }
    }
  }

  pair<COMMAND, bf::path> parse_cli_option(int argc, char **argv) {
    COMMAND cmd;
    bf::path path;
    if (argc < 3) {
      return std::make_pair(COMMAND::UNSUPPORT, path);
    }
    string parse_cmd = argv[1];
    if ("build" == parse_cmd) {
      path = argv[2];
      if (bf::exists(path) && bf::is_directory(path)) {
        cmd = COMMAND::BUILD;
      } else {
        cmd = COMMAND::UNSUPPORT;
      }
    } else if ("server" == parse_cmd) {
      cmd = COMMAND::SERVER;
    } else if ("init" == parse_cmd) {
      cmd = COMMAND::INIT;
    } else {
      cmd = COMMAND::UNSUPPORT;
    }
    return std::make_pair(cmd, path);
  }
}
