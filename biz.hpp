#pragma once

#include "parser.hpp"

#include <boost/filesystem.hpp>

#include <vector>
#include <string>
#include <memory>

namespace crowd {
  struct Post {
    explicit Post(const boost::filesystem::path &);

    Post(const Post &);

    Post(Post &&);

    virtual ~Post();

    const std::string &title() const;

    const std::string &content() const;

    Post &operator=(Post &&);

    bool load();

  private:
    boost::filesystem::path _path;
    std::string *_title = nullptr;
    std::string *_content = nullptr;
  };

  struct Theme {
    explicit Theme(const boost::filesystem::path &path = "");

    Theme(const Theme &);

    Theme(Theme &&);

    Theme &operator=(const Theme &);

    std::string post_template();

    const boost::filesystem::path &path() const;

  private:
    bool isValid() const;

    boost::filesystem::path _path;
    std::string _post_template;
  };

  struct Config {
    explicit Config(const boost::filesystem::path &);

    const Theme &theme() const;

    const boost::filesystem::path theme_path() const;

  private:
    Theme _theme;
    boost::filesystem::path _path;
  };

  struct Blog {
    typedef std::vector<Post> PostList;

    explicit Blog(const Config &);

    Blog(Blog &&);

    void posts(PostList *);

    const PostList *posts() const;

    void write_to(boost::filesystem::path &);

  private:
    std::unique_ptr<Parser> _parser;
    Config _config;
    Theme _theme;
    std::unique_ptr<PostList> _posts;
  };
}
