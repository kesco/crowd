#pragma once

#include <boost/filesystem.hpp>

#include <vector>
#include <string>
#include <memory>

namespace crowd {
  struct Post {
    explicit Post(const boost::filesystem::path &);

    explicit Post(const Post &);

    explicit Post(Post &&);

    virtual ~Post();

    const std::string &title() const;

    const std::string &content() const;

    Post &operator=(Post &&);

    bool load();

  private:
    boost::filesystem::path path_;
    std::string *title_ = nullptr;
    std::string *content_ = nullptr;
  };

  struct Theme {
    explicit Theme(const boost::filesystem::path &path = "");

    std::string post_tempalte() const;

  private:
    bool isValid() const;

    boost::filesystem::path path_;
  };

  struct Config {
    explicit Config(const boost::filesystem::path &);

    const Theme &theme() const;

  private:
    Theme theme_;
    boost::filesystem::path path_;
  };
}
