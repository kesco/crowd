#pragma once

#include <string>

namespace crowd {
  std::string string_from_file(const std::string &);

  void erase_file(const std::string &);

  void string_to_file(const std::string &path, const std::string &content,
                      bool erase = false);
}
