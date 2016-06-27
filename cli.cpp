#include "cli.hpp"
#include "biz.hpp"
#include "parser.hpp"
#include "error.hpp"

#include "mstch/mstch.hpp"

#include <boost/regex.hpp>

#include <iostream>

using namespace std;
namespace bf = boost::filesystem;

namespace {

}

namespace crowd {
  CLIApp::CLIApp() {
    temp_dir_ = bf::temp_directory_path();
    temp_dir_.append("crowd");
    if (bf::exists(temp_dir_)) {
      if (!bf::is_directory(temp_dir_)) {
        throw IOException("The temp directory of Crowd is invalid.");
      }
    } else {
      bf::create_directories(temp_dir_);
    }
  }

  void CLIApp::build(boost::filesystem::path &path) {
    if (bf::exists(path) && bf::is_directory(path)) {
      bf::path config_path = path;
      config_path.append("config.yml");
      const Config config = Config(config_path);
      const Theme theme = config.theme();
      const Parser *parser = new MarkdownParser();
      const string theme_style = theme.post_tempalte();
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
            }
          }
        }
        cout << "Load " << posts.size() << " posts." << endl;
        cout << "The output directory is " << temp_dir_ << endl;
        bf::path output_dir = temp_dir_;
        output_dir.append("posts");
        if (!bf::exists(output_dir)) {
          bf::create_directories(output_dir);
        }
        for (auto &post:posts) {
          bf::path file_path = output_dir;
          file_path.append(post.title() + ".html");
          if (bf::exists(file_path)) {
            bf::remove(file_path);
          }
          ofstream file(file_path.string());
          if (file.is_open()) {
            mstch::map content{
                {"content", mstch::lambda{[parser, post]() -> mstch::node {
                  return parser->parse(post.content());
                }}}
            };
            string convert = mstch::render(theme_style, content);
            file << convert;
            cout << post.title() << endl;
          }
        }
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
