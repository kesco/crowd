#include "biz.hpp"
#include "file.hpp"
#include "error.hpp"

#include "yaml-cpp/yaml.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

using namespace std;
namespace bf = boost::filesystem;

namespace {
  const string EMPTY_STR = "";
}

namespace crowd {
  Post::Post(const boost::filesystem::path &path) : path_(path) {
  }

  Post::Post(const Post &post) : path_(post.path_) {
    if (post.title_ != nullptr) {
      title_ = new string(*post.title_);
    }
    if (post.content_ != nullptr) {
      content_ = new string(*post.content_);
    }
  }

  Post::Post(Post &&post) {
    path_ = move(post.path_);
    if (post.title_ != nullptr) {
      title_ = post.title_;
      post.title_ = nullptr;
    }
    if (post.content_ != nullptr) {
      content_ = post.content_;
      post.content_ = nullptr;
    }
  }

  Post &Post::operator=(Post &&post) {
    if (this != &post) {
      path_ = move(post.path_);
      if (post.title_ != nullptr) {
        title_ = post.title_;
        post.title_ = nullptr;
      }
      if (post.content_ != nullptr) {
        content_ = post.content_;
        post.content_ = nullptr;
      }
    }
    return *this;
  }

  Post::~Post() {
    if (title_ != nullptr) {
      delete title_;
    }
    if (content_ != nullptr) {
      delete content_;
    }
  }


  bool Post::load() {
    try {
      string load_str = string_from_file(path_.string());
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
          title_ = new string(config);
        }
      }
      content_ = new string(md);
      return true;
    } catch (IOException &ex) {
      return false;
    }
  }

  const string &Post::title() const {
    return title_ == nullptr ? EMPTY_STR : *title_;
  }

  const string &Post::content() const {
    return content_ == nullptr ? EMPTY_STR : *content_;
  }

  Theme::Theme(const boost::filesystem::path &path) : path_(path) {
  }

  string Theme::post_tempalte() const {
    bf::path post_temp_path = path_;
    post_temp_path.append("post.mustache");
    bool x = isValid();
    bool y = bf::is_regular_file(post_temp_path);
    if (!isValid() || !bf::is_regular_file(post_temp_path)) {
      return EMPTY_STR;
    }
    return string_from_file(post_temp_path.string());
  }

  bool Theme::isValid() const {
    return !path_.empty() && bf::is_directory(path_);
  }

  Config::Config(const boost::filesystem::path &path) : path_(path) {
    if (bf::is_regular_file(path_)) {
      auto yaml = YAML::LoadFile(path_.string());
      bf::path theme_path = path_.parent_path();
      theme_path.append("themes");
      string theme_name;
      if (yaml["theme"]) {
        theme_name = yaml["theme"].as<string>();
      } else {
        theme_name = EMPTY_STR;
      }
      theme_path.append(theme_name);
      theme_ = Theme(theme_path);
    } else {
      throw IOException("Has not config file.");
    }
  }

  const Theme &Config::theme() const {
    return theme_;
  }
}



