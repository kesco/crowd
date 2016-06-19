#include "biz.hpp"
#include "file.hpp"
#include "error.hpp"

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
}
