#pragma once

#include <stdexcept>

namespace crowd {
  enum class ERROR_TYPE {
    FILE_NOT_EXIST,
    IS_NOT_DIR
  };

  struct Exception : std::runtime_error {
    Exception(const std::string &what, ERROR_TYPE type)
        : std::runtime_error(what), type_(type) { }

    virtual ~Exception() { }

    virtual ERROR_TYPE type() const;

  private:
    ERROR_TYPE type_;
  };

  struct IOException : Exception {
    explicit IOException(const std::string &what,
                         ERROR_TYPE type = ERROR_TYPE::FILE_NOT_EXIST)
        : Exception(what, type) { }

    virtual ~IOException() { }
  };
}
