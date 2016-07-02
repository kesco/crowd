#include "biz.hpp"
#include "file.hpp"
#include "error.hpp"

#include "yaml-cpp/yaml.h"
#include "mstch/mstch.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

using namespace std;
namespace bf = boost::filesystem;

namespace {
  const string EMPTY_STR = "";
}

namespace crowd {
  Post::Post(const boost::filesystem::path &path) : _path(path) {
  }

  Post::Post(const Post &post) : _path(post._path) {
    if (post._title != nullptr) {
      _title = new string(*post._title);
    }
    if (post._content != nullptr) {
      _content = new string(*post._content);
    }
  }

  Post::Post(Post &&post) {
    _path = move(post._path);
    if (post._title != nullptr) {
      _title = post._title;
      post._title = nullptr;
    }
    if (post._content != nullptr) {
      _content = post._content;
      post._content = nullptr;
    }
  }

  Post &Post::operator=(Post &&post) {
    if (this != &post) {
      _path = move(post._path);
      if (post._title != nullptr) {
        _title = post._title;
        post._title = nullptr;
      }
      if (post._content != nullptr) {
        _content = post._content;
        post._content = nullptr;
      }
    }
    return *this;
  }

  Post::~Post() {
    if (_title != nullptr) {
      delete _title;
    }
    if (_content != nullptr) {
      delete _content;
    }
  }


  bool Post::load() {
    try {
      string load_str = string_from_file(_path.string());
      stringstream ss(load_str);
      int nu = 0;
      string md;
      string config;
      string line;
      boost::regex pattern("^\\+{3}\\s*");
      boost::regex title_pattern("^title\\:");
      boost::smatch what;
      while (getline(ss, line, '\n')) {
        if (nu > 1) {
          if (line != "") {
            md += line;
            md += '\n';
          }
        } else if (boost::regex_match(line, pattern)) {
          nu += 1;
        } else if (boost::regex_search(line, what, title_pattern)) {
          config = boost::algorithm::erase_first_copy(line, what[0]);
          boost::algorithm::trim(config);
          _title = new string(config);
        }
      }
      _content = new string(md);
      return true;
    } catch (IOException &ex) {
      return false;
    }
  }

  const string &Post::title() const {
    return _title == nullptr ? EMPTY_STR : *_title;
  }

  const string &Post::content() const {
    return _content == nullptr ? EMPTY_STR : *_content;
  }

  Theme::Theme(const boost::filesystem::path &path) : _path(path),
                                                      _post_template() {
  }

  Theme::Theme(const Theme &theme) : _path(theme._path),
                                     _post_template(theme._post_template) {
  }

  Theme::Theme(Theme &&theme) {
    _path = move(theme._path);
    _post_template = move(theme._post_template);
  }

  Theme &Theme::operator=(const Theme &copy) {
    if (this != &copy) {
      _path = copy._path;
      _post_template = copy._post_template;
    }
    return *this;
  }

  string Theme::post_template() {
    if (EMPTY_STR == _post_template) {
      bf::path post_temp_path = _path;
      post_temp_path.append("post.mustache");
      if (isValid() && bf::is_regular_file(post_temp_path)) {
        _post_template = string_from_file(post_temp_path.string());
      }
    } else {
      return _post_template;
    }
    return _post_template;
  }

  bool Theme::isValid() const {
    return !_path.empty() && bf::is_directory(_path);
  }


  const boost::filesystem::path &Theme::path() const {
    return _path;
  }

  Config::Config(const boost::filesystem::path &path) : _path(path) {
    if (bf::is_regular_file(_path)) {
      auto yaml = YAML::LoadFile(_path.string());
      bf::path theme_path = _path.parent_path();
      theme_path.append("themes");
      string theme_name;
      if (yaml["theme"]) {
        theme_name = yaml["theme"].as<string>();
      } else {
        theme_name = EMPTY_STR;
      }
      theme_path.append(theme_name);
      _theme = Theme(theme_path);
    } else {
      throw IOException("Has not config file.");
    }
  }

  const Theme &Config::theme() const {
    return _theme;
  }

  const boost::filesystem::path Config::theme_path() const {
    return _theme.path();
  }

  Blog::Blog(const crowd::Config &config) : _parser(
      unique_ptr<Parser>(new MarkdownParser())),
                                            _config(config),
                                            _theme(config.theme_path()),
                                            _posts(
                                                unique_ptr<PostList>(nullptr)) {
  }

  Blog::Blog(Blog &&rhs) : _parser(move(rhs._parser)),
                           _config(move(rhs._config)), _theme(move(rhs._theme)),
                           _posts(move(rhs._posts)) {
  }

  void Blog::posts(PostList *posts) {
    _posts.reset(posts);
  }


  const Blog::PostList *Blog::posts() const {
    return _posts.get();
  }

  void Blog::write_to(bf::path &to) {
    if (!bf::exists(to)) bf::create_directories(to);
    const string theme_style = _theme.post_template();
    for (auto &post:*_posts) {
      bf::path file_path = to;
      file_path.append(post.title() + ".html");
      mstch::map content{
          {"content", _parser->parse(post.content())}
      };
      string convert = mstch::render(theme_style, content);
      try {
        bool is_rewrite = bf::exists(file_path);
        string_to_file(file_path.string(), convert, is_rewrite);
        cout << post.title() << endl;
      } catch (IOException &ex) {
        cerr << ex.what() << endl;
      }
    }
  }
}
