#pragma once

#include "parser.hpp"

#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

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

    const boost::gregorian::date& date() const;

    Post &operator=(Post &&);

    bool load();

  private:
    boost::filesystem::path _path;
    std::unique_ptr<std::string> _title;
    std::unique_ptr<std::string> _content;
    boost::gregorian::date _date;
    std::vector<std::string> _tags;
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

    const boost::filesystem::path &theme_path() const;

  private:
    boost::filesystem::path _path;
    boost::filesystem::path _theme_path;
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
