#include "biz.hpp"
#include "file.hpp"
#include "error.hpp"

#include "yaml-cpp/yaml.h"
#include "mstch/mstch.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

using namespace std;
namespace ba = boost::algorithm;
namespace bf = boost::filesystem;
namespace bg = boost::gregorian;

namespace {
  const string EMPTY_STR = "";
}

namespace crowd {
  Post::Post(const boost::filesystem::path &path) : _path(path),
                                                    _title(unique_ptr<string>(
                                                        nullptr)),
                                                    _content(unique_ptr<string>(
                                                        nullptr)),
                                                    _date(bg::date()),
                                                    _tags(vector<string>()) {
  }

  Post::Post(const Post &post) : _path(post._path),
                                 _date(bg::date(post._date)),
                                 _tags(vector<string>(post._tags)) {
    if (post._title != nullptr) {
      _title = unique_ptr<string>(new string(*post._title));
    } else {
      _title = unique_ptr<string>(nullptr);
    }
    if (post._content != nullptr) {
      _content = unique_ptr<string>(new string(*post._content));
    } else {
      _content = unique_ptr<string>(nullptr);
    }
  }

  Post::Post(Post &&post) : _path(move(post._path)),
                            _title(move(post._title)),
                            _content(move(post._content)),
                            _date(post._date),
                            _tags(move(post._tags)) {
  }

  Post &Post::operator=(Post &&post) {
    if (this != &post) {
      _path = move(post._path);
      _title = move(post._title);
      _content = move(post._content);
      _date = post._date;
      _tags = move(post._tags);
    }
    return *this;
  }

  Post::~Post() {
  }


  bool Post::load() {
    try {
      string load_str = string_from_file(_path.string());
      stringstream ss(load_str);
      int nu = 0;
      string md;
      string line;
      boost::regex pattern("^\\+{3}\\s*");
      boost::regex title_pattern("^title\\:");
      boost::regex date_pattern("^date\\:");
      boost::regex tag_pattern("^tags\\:");
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
          string config = ba::erase_first_copy(line, what[0]);
          ba::trim(config);
          _title.reset(new string(config));
        } else if (boost::regex_search(line, what, tag_pattern)) {
          string tags = ba::erase_first_copy(line, what[0]);
          ba::split(_tags, tags, ba::is_any_of(","));
          for (auto &tag:_tags) {
            ba::trim(tag);
          }
        } else if (boost::regex_search(line, what, date_pattern)) {
          string date = ba::erase_first_copy(line, what[0]);
          ba::trim(date);
          _date = bg::from_simple_string(date);
        }
      }
      _content.reset(new string(md));
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

  const boost::gregorian::date &Post::date() const {
    return _date;
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

  Config::Config(const boost::filesystem::path &path) : _path(path),
                                                        _theme_path(
                                                            path.parent_path()) {
    if (bf::is_regular_file(_path)) {
      auto yaml = YAML::LoadFile(_path.string());
      _theme_path.append("themes");
      string theme_name;
      if (yaml["theme"]) {
        theme_name = yaml["theme"].as<string>();
      } else {
        theme_name = EMPTY_STR;
      }
      _theme_path.append(theme_name);
    } else {
      throw IOException("Has not config file.");
    }
  }

  const boost::filesystem::path &Config::theme_path() const {
    return _theme_path;
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
        cout << post.title() << ":" <<
        bg::to_iso_extended_string(post.date()) << endl;
      } catch (IOException &ex) {
        cerr << ex.what() << endl;
      }
    }
  }
}
