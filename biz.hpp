#pragma once

#include <boost/filesystem.hpp>

#include <vector>
#include <string>

namespace crowd {
  struct Post {
    explicit Post(const boost::filesystem::path &);

    explicit Post(const Post &);

    explicit Post(Post &&);

    virtual ~Post();

    const std::string& title() const;

    Post &operator=(Post &&);

    bool load();

  private:
    boost::filesystem::path path_;
    std::string *title_ = nullptr;
    std::string *content_ = nullptr;
  };
}
