#include "cli.hpp"
#include "biz.hpp"
#include "error.hpp"

#include "mstch/mstch.hpp"

#include <boost/regex.hpp>
#include <boost/format.hpp>

#include <iostream>

using namespace std;
namespace bf = boost::filesystem;

namespace {
  using namespace crowd;

  Blog _generate_blog(const bf::path &path) {
    if (bf::is_directory(path)) {
      bf::path config_path = path;
      config_path.append("config.yml");
      if (bf::is_regular_file(config_path)) {
        Config config = Config(config_path);
        return Blog(config);
      } else {
        boost::format error_msg =
            boost::format("%s is not a blog directory.") % path.string();
        throw IOException(error_msg.str());
      }
    } else {
      boost::format error_msg =
          boost::format("%s is invalid.") % path.string();
      throw IOException(error_msg.str(), ERROR_TYPE::IS_NOT_DIR);
    }
  }

  Blog::PostList *_generate_posts(const bf::path &path) {
    bf::path post_path = path;
    post_path.append("posts");
    if (bf::is_directory(post_path)) {
      boost::regex pattern(".*\\.md");
      Blog::PostList *posts = new Blog::PostList();
      for (bf::recursive_directory_iterator iter(post_path), end;
           iter != end; iter++) {
        string file_name = iter->path().string();
        if (boost::regex_match(file_name, pattern)) {
          Post post(iter->path());
          if (post.load()) {
            posts->push_back(move(post));
          }
        }
      }
      return posts;
    } else {
      boost::format error_msg =
          boost::format("%s is not a posts directory.") % post_path.string();
      throw IOException(error_msg.str(), ERROR_TYPE::IS_NOT_DIR);
    }
  }
}

namespace crowd {
  CLIApp::CLIApp() {
    _temp_dir = bf::temp_directory_path();
    _temp_dir.append("crowd");
    if (bf::exists(_temp_dir)) {
      if (!bf::is_directory(_temp_dir)) {
        throw IOException("The temp directory of Crowd is invalid.");
      }
    } else {
      bf::create_directories(_temp_dir);
    }
  }

  void CLIApp::build(bf::path &path) {
    Blog blog = _generate_blog(path);
    bf::path post_path = path;
    post_path.append("posts");
    Blog::PostList *posts = _generate_posts(path);
    blog.posts(posts);
    cout << "Load " << posts->size() << " posts." << endl;
    cout << "The output directory is " << _temp_dir << endl;
    bf::path output_dir = _temp_dir;
    output_dir.append("posts");
    blog.write_to(output_dir);
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
